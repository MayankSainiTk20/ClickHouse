#include "config.h"
#include <Disks/ObjectStorages/ObjectStorageFactory.h>
#if USE_HDFS && !defined(CLICKHOUSE_KEEPER_STANDALONE_BUILD)
#include <Disks/ObjectStorages/HDFS/HDFSObjectStorage.h>
#include <Storages/HDFS/HDFSCommon.h>
#endif
#ifndef CLICKHOUSE_KEEPER_STANDALONE_BUILD
#include <Disks/ObjectStorages/Web/WebObjectStorage.h>
#include <Disks/ObjectStorages/Local/LocalObjectStorage.h>
#include <Disks/loadLocalDiskConfig.h>
#endif
#include <Disks/ObjectStorages/MetadataStorageFactory.h>
#include <Disks/ObjectStorages/PlainObjectStorage.h>
#include <Interpreters/Context.h>
#include <Common/Macros.h>


namespace DB
{
namespace ErrorCodes
{
    extern const int NO_ELEMENTS_IN_CONFIG;
    extern const int UNKNOWN_ELEMENT_IN_CONFIG;
    extern const int BAD_ARGUMENTS;
    extern const int LOGICAL_ERROR;
}

namespace
{
    bool isPlainStorage(
        ObjectStorageType type,
        const Poco::Util::AbstractConfiguration & config,
        const std::string & config_prefix)
    {
        auto compatibility_hint = MetadataStorageFactory::getCompatibilityMetadataTypeHint(type);
        auto metadata_type = MetadataStorageFactory::getMetadataType(config, config_prefix, compatibility_hint);
        return metadataTypeFromString(metadata_type) == MetadataStorageType::Plain;
    }

    template <typename BaseObjectStorage, class ...Args>
    ObjectStoragePtr createObjectStorage(
        ObjectStorageType type,
        const Poco::Util::AbstractConfiguration & config,
        const std::string & config_prefix,
        Args && ...args)
    {
        if (isPlainStorage(type, config, config_prefix))
        {
            return std::make_shared<PlainObjectStorage<BaseObjectStorage>>(std::forward<Args>(args)...);
        }
        else
        {
            return std::make_shared<BaseObjectStorage>(std::forward<Args>(args)...);
        }
    }
}

ObjectStorageFactory & ObjectStorageFactory::instance()
{
    static ObjectStorageFactory factory;
    return factory;
}

void ObjectStorageFactory::registerObjectStorageType(const std::string & type, Creator creator)
{
    if (!registry.emplace(type, creator).second)
    {
        throw Exception(ErrorCodes::LOGICAL_ERROR,
                        "ObjectStorageFactory: the metadata type '{}' is not unique", type);
    }
}

ObjectStoragePtr ObjectStorageFactory::create(
    const std::string & name,
    const Poco::Util::AbstractConfiguration & config,
    const std::string & config_prefix,
    const ContextPtr & context,
    bool skip_access_check) const
{
    std::string type;
    if (config.has(config_prefix + ".object_storage_type"))
    {
        type = config.getString(config_prefix + ".object_storage_type");
    }
    else if (config.has(config_prefix + ".type")) /// .type -- for compatibility.
    {
        type = config.getString(config_prefix + ".type");
    }
    else
    {
        throw Exception(ErrorCodes::NO_ELEMENTS_IN_CONFIG, "Expected `object_storage_type` in config");
    }

    const auto it = registry.find(type);

    if (it == registry.end())
    {
        throw Exception(ErrorCodes::UNKNOWN_ELEMENT_IN_CONFIG,
                        "ObjectStorageFactory: unknown object storage type: {}", type);
    }

    return it->second(name, config, config_prefix, context, skip_access_check);
}

#if USE_HDFS && !defined(CLICKHOUSE_KEEPER_STANDALONE_BUILD)
void registerHDFSObjectStorage(ObjectStorageFactory & factory)
{
    factory.registerObjectStorageType("hdfs", [](
        const std::string & /* name */,
        const Poco::Util::AbstractConfiguration & config,
        const std::string & config_prefix,
        const ContextPtr & context,
        bool /* skip_access_check */) -> ObjectStoragePtr
    {
        auto uri = context->getMacros()->expand(config.getString(config_prefix + ".endpoint"));
        checkHDFSURL(uri);
        if (uri.back() != '/')
            throw Exception(ErrorCodes::BAD_ARGUMENTS, "HDFS path must ends with '/', but '{}' doesn't.", uri);

        std::unique_ptr<HDFSObjectStorageSettings> settings = std::make_unique<HDFSObjectStorageSettings>(
            config.getUInt64(config_prefix + ".min_bytes_for_seek", 1024 * 1024),
            config.getInt(config_prefix + ".objects_chunk_size_to_delete", 1000),
            context->getSettingsRef().hdfs_replication
        );

        return createObjectStorage<HDFSObjectStorage>(ObjectStorageType::HDFS, config, config_prefix, uri, std::move(settings), config);
    });
}
#endif


#ifndef CLICKHOUSE_KEEPER_STANDALONE_BUILD
void registerWebObjectStorage(ObjectStorageFactory & factory)
{
    factory.registerObjectStorageType("web", [](
        const std::string & /* name */,
        const Poco::Util::AbstractConfiguration & config,
        const std::string & config_prefix,
        const ContextPtr & context,
        bool /* skip_access_check */) -> ObjectStoragePtr
    {
        auto uri = context->getMacros()->expand(config.getString(config_prefix + ".endpoint"));
        if (!uri.ends_with('/'))
            throw Exception(
                ErrorCodes::BAD_ARGUMENTS, "URI must end with '/', but '{}' doesn't.", uri);
        try
        {
            Poco::URI poco_uri(uri);
        }
        catch (const Poco::Exception & e)
        {
            throw Exception(
                ErrorCodes::BAD_ARGUMENTS, "Bad URI: `{}`. Error: {}", uri, e.what());
        }

        return createObjectStorage<WebObjectStorage>(ObjectStorageType::Web, config, config_prefix, uri, context);
    });
}

void registerLocalObjectStorage(ObjectStorageFactory & factory)
{
    auto creator = [](
        const std::string & name,
        const Poco::Util::AbstractConfiguration & config,
        const std::string & config_prefix,
        const ContextPtr & context,
        bool /* skip_access_check */) -> ObjectStoragePtr
    {
        String object_key_prefix;
        UInt64 keep_free_space_bytes;
        loadDiskLocalConfig(name, config, config_prefix, context, object_key_prefix, keep_free_space_bytes);
        /// keys are mapped to the fs, object_key_prefix is a directory also
        fs::create_directories(object_key_prefix);
        return createObjectStorage<LocalObjectStorage>(ObjectStorageType::Local, config, config_prefix, object_key_prefix);
    };

    factory.registerObjectStorageType("local_blob_storage", creator);
    factory.registerObjectStorageType("local", creator);
}
#endif

void registerObjectStorages()
{
    auto & factory = ObjectStorageFactory::instance();

#if USE_HDFS && !defined(CLICKHOUSE_KEEPER_STANDALONE_BUILD)
    registerHDFSObjectStorage(factory);
#endif

#ifndef CLICKHOUSE_KEEPER_STANDALONE_BUILD
    registerWebObjectStorage(factory);
    registerLocalObjectStorage(factory);
#endif
}

}
