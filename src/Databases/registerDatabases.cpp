#include <Databases/DatabaseFactory.h>
#include <Databases/registerDatabases.h>


namespace DB
{

void registerDatabaseAtomic(DatabaseFactory & factory);
void registerDatabaseOrdinary(DatabaseFactory & factory);
void registerDatabaseDictionary(DatabaseFactory & factory);
void registerDatabaseMemory(DatabaseFactory & factory);
void registerDatabaseLazy(DatabaseFactory & factory);
void registerDatabaseFilesystem(DatabaseFactory & factory);
void registerDatabaseReplicated(DatabaseFactory & factory);

#if USE_LIBPQXX
void registerDatabasePostgreSQL(DatabaseFactory & factory);

void registerDatabaseMaterializedPostgreSQL(DatabaseFactory & factory);
#endif


#if USE_HDFS
void registerDatabaseHDFS(DatabaseFactory & factory);
#endif

void registerDatabases()
{
    auto & factory = DatabaseFactory::instance();
    registerDatabaseAtomic(factory);
    registerDatabaseOrdinary(factory);
    registerDatabaseDictionary(factory);
    registerDatabaseMemory(factory);
    registerDatabaseLazy(factory);
    registerDatabaseFilesystem(factory);
    registerDatabaseReplicated(factory);

#if USE_LIBPQXX
    registerDatabasePostgreSQL(factory);
    registerDatabaseMaterializedPostgreSQL(factory);
#endif


#if USE_HDFS
    registerDatabaseHDFS(factory);
#endif
}
}
