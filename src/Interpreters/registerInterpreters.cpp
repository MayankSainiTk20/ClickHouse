#include <Interpreters/InterpreterFactory.h>

namespace DB
{

void registerInterpreterSelectQuery(InterpreterFactory & factory);
void registerInterpreterSelectQueryAnalyzer(InterpreterFactory & factory);
void registerInterpreterSelectWithUnionQuery(InterpreterFactory & factory);
void registerInterpreterSelectIntersectExceptQuery(InterpreterFactory & factory);
void registerInterpreterInsertQuery(InterpreterFactory & factory);
void registerInterpreterCreateQuery(InterpreterFactory & factory);
void registerInterpreterDropQuery(InterpreterFactory & factory);
void registerInterpreterUndropQuery(InterpreterFactory & factory);
void registerInterpreterRenameQuery(InterpreterFactory & factory);
void registerInterpreterShowTablesQuery(InterpreterFactory & factory);
void registerInterpreterShowColumnsQuery(InterpreterFactory & factory);
void registerInterpreterShowIndexesQuery(InterpreterFactory & factory);
void registerInterpreterShowSettingQuery(InterpreterFactory & factory);
void registerInterpreterShowEnginesQuery(InterpreterFactory & factory);
void registerInterpreterShowFunctionsQuery(InterpreterFactory & factory);
void registerInterpreterUseQuery(InterpreterFactory & factory);
void registerInterpreterSetQuery(InterpreterFactory & factory);
void registerInterpreterSetRoleQuery(InterpreterFactory & factory);
void registerInterpreterOptimizeQuery(InterpreterFactory & factory);
void registerInterpreterExistsQuery(InterpreterFactory & factory);
void registerInterpreterShowCreateQuery(InterpreterFactory & factory);
void registerInterpreterDescribeQuery(InterpreterFactory & factory);
void registerInterpreterDescribeCacheQuery(InterpreterFactory & factory);
void registerInterpreterExplainQuery(InterpreterFactory & factory);
void registerInterpreterShowProcesslistQuery(InterpreterFactory & factory);
void registerInterpreterAlterQuery(InterpreterFactory & factory);
void registerInterpreterAlterNamedCollectionQuery(InterpreterFactory & factory);
void registerInterpreterCheckQuery(InterpreterFactory & factory);
void registerInterpreterKillQueryQuery(InterpreterFactory & factory);
void registerInterpreterSystemQuery(InterpreterFactory & factory);
void registerInterpreterWatchQuery(InterpreterFactory & factory);
void registerInterpreterCreateUserQuery(InterpreterFactory & factory);
void registerInterpreterCreateRoleQuery(InterpreterFactory & factory);
void registerInterpreterCreateQuotaQuery(InterpreterFactory & factory);
void registerInterpreterCreateRowPolicyQuery(InterpreterFactory & factory);
void registerInterpreterCreateSettingsProfileQuery(InterpreterFactory & factory);
void registerInterpreterDropAccessEntityQuery(InterpreterFactory & factory);
void registerInterpreterMoveAccessEntityQuery(InterpreterFactory & factory);
void registerInterpreterDropNamedCollectionQuery(InterpreterFactory & factory);
void registerInterpreterGrantQuery(InterpreterFactory & factory);
void registerInterpreterShowCreateAccessEntityQuery(InterpreterFactory & factory);
void registerInterpreterShowGrantsQuery(InterpreterFactory & factory);
void registerInterpreterShowAccessEntitiesQuery(InterpreterFactory & factory);
void registerInterpreterShowAccessQuery(InterpreterFactory & factory);
void registerInterpreterShowPrivilegesQuery(InterpreterFactory & factory);
void registerInterpreterTransactionControlQuery(InterpreterFactory & factory);
void registerInterpreterCreateFunctionQuery(InterpreterFactory & factory);
void registerInterpreterDropFunctionQuery(InterpreterFactory & factory);
void registerInterpreterCreateIndexQuery(InterpreterFactory & factory);
void registerInterpreterCreateNamedCollectionQuery(InterpreterFactory & factory);
void registerInterpreterDropIndexQuery(InterpreterFactory & factory);
void registerInterpreterBackupQuery(InterpreterFactory & factory);
void registerInterpreterDeleteQuery(InterpreterFactory & factory);

void registerInterpreters()
{
    auto & factory = InterpreterFactory::instance();

    registerInterpreterSelectQuery(factory);
    registerInterpreterSelectQueryAnalyzer(factory);
    registerInterpreterSelectWithUnionQuery(factory);
    registerInterpreterSelectIntersectExceptQuery(factory);
    registerInterpreterInsertQuery(factory);
    registerInterpreterCreateQuery(factory);
    registerInterpreterDropQuery(factory);
    registerInterpreterUndropQuery(factory);
    registerInterpreterRenameQuery(factory);
    registerInterpreterShowTablesQuery(factory);
    registerInterpreterShowColumnsQuery(factory);
    registerInterpreterShowIndexesQuery(factory);
    registerInterpreterShowSettingQuery(factory);
    registerInterpreterShowEnginesQuery(factory);
    registerInterpreterShowFunctionsQuery(factory);
    registerInterpreterUseQuery(factory);
    registerInterpreterSetQuery(factory);
    registerInterpreterSetRoleQuery(factory);
    registerInterpreterOptimizeQuery(factory);
    registerInterpreterExistsQuery(factory);
    registerInterpreterShowCreateQuery(factory);
    registerInterpreterDescribeQuery(factory);
    registerInterpreterDescribeCacheQuery(factory);
    registerInterpreterExplainQuery(factory);
    registerInterpreterShowProcesslistQuery(factory);
    registerInterpreterAlterQuery(factory);
    registerInterpreterAlterNamedCollectionQuery(factory);
    registerInterpreterCheckQuery(factory);
    registerInterpreterKillQueryQuery(factory);
    registerInterpreterSystemQuery(factory);
    registerInterpreterWatchQuery(factory);
    registerInterpreterCreateUserQuery(factory);
    registerInterpreterCreateRoleQuery(factory);
    registerInterpreterCreateQuotaQuery(factory);
    registerInterpreterCreateRowPolicyQuery(factory);
    registerInterpreterCreateSettingsProfileQuery(factory);
    registerInterpreterDropAccessEntityQuery(factory);
    registerInterpreterMoveAccessEntityQuery(factory);
    registerInterpreterDropNamedCollectionQuery(factory);
    registerInterpreterGrantQuery(factory);
    registerInterpreterShowCreateAccessEntityQuery(factory);
    registerInterpreterShowGrantsQuery(factory);
    registerInterpreterShowAccessEntitiesQuery(factory);
    registerInterpreterShowAccessQuery(factory);
    registerInterpreterShowPrivilegesQuery(factory);
    registerInterpreterTransactionControlQuery(factory);
    registerInterpreterCreateFunctionQuery(factory);
    registerInterpreterDropFunctionQuery(factory);
    registerInterpreterCreateIndexQuery(factory);
    registerInterpreterCreateNamedCollectionQuery(factory);
    registerInterpreterDropIndexQuery(factory);
    registerInterpreterBackupQuery(factory);
    registerInterpreterDeleteQuery(factory);
}
}
