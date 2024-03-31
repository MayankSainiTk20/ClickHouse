#include <Access/Authentication.h>
#include <Access/AuthenticationData.h>
#include <Access/Credentials.h>
#include <Access/ExternalAuthenticators.h>
#include <Access/LDAPClient.h>
#include <Access/GSSAcceptor.h>
#include <Common/Exception.h>
#include <Poco/SHA1Engine.h>
#include <Common/typeid_cast.h>
#include <Common/SSH/Wrappers.h>


namespace DB
{
namespace ErrorCodes
{
    extern const int NOT_IMPLEMENTED;
    extern const int SUPPORT_IS_DISABLED;
}

namespace
{
    using Digest = AuthenticationData::Digest;
    using Util = AuthenticationData::Util;

    bool checkPasswordPlainText(const String & password, const Digest & password_plaintext)
    {
        return (Util::stringToDigest(password) == password_plaintext);
    }

    bool checkPasswordDoubleSHA1(std::string_view password, const Digest & password_double_sha1)
    {
        return (Util::encodeDoubleSHA1(password) == password_double_sha1);
    }

    bool checkPasswordBcrypt(std::string_view password, const Digest & password_bcrypt)
    {
        return Util::checkPasswordBcrypt(password, password_bcrypt);
    }

    bool checkPasswordSHA256(std::string_view password, const Digest & password_sha256, const String & salt)
    {
        return Util::encodeSHA256(String(password).append(salt)) == password_sha256;
    }

#if USE_SSH
    bool checkSshSignature(const std::vector<ssh::SSHKey> & keys, std::string_view signature, std::string_view original)
    {
        for (const auto & key: keys)
            if (key.isPublic() && key.verifySignature(signature, original))
                return true;
        return false;
    }
#endif
}


bool Authentication::areCredentialsValid(
    const Credentials & credentials,
    const AuthenticationData & auth_data,
    const ExternalAuthenticators & external_authenticators,
    SettingsChanges & settings)
{
    if (!credentials.isReady())
        return false;

    if (const auto * gss_acceptor_context = typeid_cast<const GSSAcceptorContext *>(&credentials))
    {
        switch (auth_data.getType())
        {
            case AuthenticationType::NO_PASSWORD:
            case AuthenticationType::PLAINTEXT_PASSWORD:
            case AuthenticationType::SHA256_PASSWORD:
            case AuthenticationType::DOUBLE_SHA1_PASSWORD:
            case AuthenticationType::BCRYPT_PASSWORD:
            case AuthenticationType::LDAP:
            case AuthenticationType::HTTP:
                throw Authentication::Require<BasicCredentials>("ClickHouse Basic Authentication");

            case AuthenticationType::KERBEROS:
                return external_authenticators.checkKerberosCredentials(auth_data.getKerberosRealm(), *gss_acceptor_context);

            case AuthenticationType::SSL_CERTIFICATE:
                throw Authentication::Require<BasicCredentials>("ClickHouse X.509 Authentication");

            case AuthenticationType::SSH_KEY:
                throw Authentication::Require<SshCredentials>("Ssh Keys Authentication");

            case AuthenticationType::MAX:
                break;
        }
    }

    if (const auto * basic_credentials = typeid_cast<const BasicCredentials *>(&credentials))
    {
        switch (auth_data.getType())
        {
            case AuthenticationType::NO_PASSWORD:
                return true; // N.B. even if the password is not empty!

            case AuthenticationType::PLAINTEXT_PASSWORD:
                return checkPasswordPlainText(basic_credentials->getPassword(), auth_data.getPasswordHashBinary());

            case AuthenticationType::SHA256_PASSWORD:
                return checkPasswordSHA256(basic_credentials->getPassword(), auth_data.getPasswordHashBinary(), auth_data.getSalt());

            case AuthenticationType::DOUBLE_SHA1_PASSWORD:
                return checkPasswordDoubleSHA1(basic_credentials->getPassword(), auth_data.getPasswordHashBinary());

            case AuthenticationType::LDAP:
                return external_authenticators.checkLDAPCredentials(auth_data.getLDAPServerName(), *basic_credentials);

            case AuthenticationType::KERBEROS:
                throw Authentication::Require<GSSAcceptorContext>(auth_data.getKerberosRealm());

            case AuthenticationType::SSL_CERTIFICATE:
                throw Authentication::Require<BasicCredentials>("ClickHouse X.509 Authentication");

            case AuthenticationType::SSH_KEY:
                throw Authentication::Require<SshCredentials>("Ssh Keys Authentication");

            case AuthenticationType::BCRYPT_PASSWORD:
                return checkPasswordBcrypt(basic_credentials->getPassword(), auth_data.getPasswordHashBinary());

            case AuthenticationType::HTTP:
                switch (auth_data.getHTTPAuthenticationScheme())
                {
                    case HTTPAuthenticationScheme::BASIC:
                        return external_authenticators.checkHTTPBasicCredentials(
                            auth_data.getHTTPAuthenticationServerName(), *basic_credentials, settings);
                }

            case AuthenticationType::MAX:
                break;
        }
    }

    if (const auto * ssl_certificate_credentials = typeid_cast<const SSLCertificateCredentials *>(&credentials))
    {
        switch (auth_data.getType())
        {
            case AuthenticationType::NO_PASSWORD:
            case AuthenticationType::PLAINTEXT_PASSWORD:
            case AuthenticationType::SHA256_PASSWORD:
            case AuthenticationType::DOUBLE_SHA1_PASSWORD:
            case AuthenticationType::BCRYPT_PASSWORD:
            case AuthenticationType::LDAP:
            case AuthenticationType::HTTP:
                throw Authentication::Require<BasicCredentials>("ClickHouse Basic Authentication");

            case AuthenticationType::KERBEROS:
                throw Authentication::Require<GSSAcceptorContext>(auth_data.getKerberosRealm());

            case AuthenticationType::SSL_CERTIFICATE:
                return auth_data.getSSLCertificateCommonNames().contains(ssl_certificate_credentials->getCommonName());

            case AuthenticationType::SSH_KEY:
                throw Authentication::Require<SshCredentials>("Ssh Keys Authentication");

            case AuthenticationType::MAX:
                break;
        }
    }

    if (const auto * ssh_credentials = typeid_cast<const SshCredentials *>(&credentials))
    {
        switch (auth_data.getType())
        {
            case AuthenticationType::NO_PASSWORD:
            case AuthenticationType::PLAINTEXT_PASSWORD:
            case AuthenticationType::SHA256_PASSWORD:
            case AuthenticationType::DOUBLE_SHA1_PASSWORD:
            case AuthenticationType::BCRYPT_PASSWORD:
            case AuthenticationType::LDAP:
            case AuthenticationType::HTTP:
                throw Authentication::Require<BasicCredentials>("ClickHouse Basic Authentication");

            case AuthenticationType::KERBEROS:
                throw Authentication::Require<GSSAcceptorContext>(auth_data.getKerberosRealm());

            case AuthenticationType::SSL_CERTIFICATE:
                throw Authentication::Require<SSLCertificateCredentials>("ClickHouse X.509 Authentication");

            case AuthenticationType::SSH_KEY:
#if USE_SSH
                return checkSshSignature(auth_data.getSSHKeys(), ssh_credentials->getSignature(), ssh_credentials->getOriginal());
#else
                throw Exception(ErrorCodes::SUPPORT_IS_DISABLED, "SSH is disabled, because ClickHouse is built without OpenSSL");
#endif
            case AuthenticationType::MAX:
                break;
        }
    }

    if ([[maybe_unused]] const auto * always_allow_credentials = typeid_cast<const AlwaysAllowCredentials *>(&credentials))
        return true;

    throw Exception(ErrorCodes::NOT_IMPLEMENTED, "areCredentialsValid(): authentication type {} not supported", toString(auth_data.getType()));
}

}
