#include <Storages/registerStorages.h>
#include <Storages/StorageFactory.h>

#include "config.h"

namespace DB
{

void registerStorageLog(StorageFactory & factory);
void registerStorageStripeLog(StorageFactory & factory);
void registerStorageMergeTree(StorageFactory & factory);
void registerStorageNull(StorageFactory & factory);
void registerStorageMerge(StorageFactory & factory);
void registerStorageBuffer(StorageFactory & factory);
void registerStorageDistributed(StorageFactory & factory);
void registerStorageMemory(StorageFactory & factory);
void registerStorageFile(StorageFactory & factory);
void registerStorageURL(StorageFactory & factory);
void registerStorageDictionary(StorageFactory & factory);
void registerStorageSet(StorageFactory & factory);
void registerStorageJoin(StorageFactory & factory);
void registerStorageView(StorageFactory & factory);
void registerStorageMaterializedView(StorageFactory & factory);
void registerStorageLiveView(StorageFactory & factory);
void registerStorageGenerateRandom(StorageFactory & factory);
void registerStorageExecutable(StorageFactory & factory);
void registerStorageWindowView(StorageFactory & factory);
#if USE_RAPIDJSON || USE_SIMDJSON
void registerStorageFuzzJSON(StorageFactory & factory);
#endif


#if USE_PARQUET
void registerStorageDeltaLake(StorageFactory & factory);
#endif
#if USE_AVRO
void registerStorageIceberg(StorageFactory & factory);
#endif

#if USE_HDFS
void registerStorageHDFS(StorageFactory & factory);

#if USE_HIVE
void registerStorageHive(StorageFactory & factory);
#endif

#endif

void registerStorageODBC(StorageFactory & factory);
void registerStorageJDBC(StorageFactory & factory);

#if USE_MYSQL
void registerStorageMySQL(StorageFactory & factory);
#endif

void registerStorageMongoDB(StorageFactory & factory);
void registerStorageRedis(StorageFactory & factory);


#if USE_RDKAFKA
void registerStorageKafka(StorageFactory & factory);
#endif

#if USE_AMQPCPP
void registerStorageRabbitMQ(StorageFactory & factory);
#endif

#if USE_NATSIO
void registerStorageNATS(StorageFactory & factory);
#endif

#if USE_ROCKSDB
void registerStorageEmbeddedRocksDB(StorageFactory & factory);
#endif

#if USE_LIBPQXX
void registerStoragePostgreSQL(StorageFactory & factory);
void registerStorageMaterializedPostgreSQL(StorageFactory & factory);
#endif

#if USE_MYSQL || USE_LIBPQXX
void registerStorageExternalDistributed(StorageFactory & factory);
#endif

#if USE_FILELOG
void registerStorageFileLog(StorageFactory & factory);
#endif

#if USE_SQLITE
void registerStorageSQLite(StorageFactory & factory);
#endif

void registerStorageKeeperMap(StorageFactory & factory);

void registerStorages()
{
    auto & factory = StorageFactory::instance();

    registerStorageLog(factory);
    registerStorageStripeLog(factory);
    registerStorageMergeTree(factory);
    registerStorageNull(factory);
    registerStorageMerge(factory);
    registerStorageBuffer(factory);
    registerStorageDistributed(factory);
    registerStorageMemory(factory);
    registerStorageFile(factory);
    registerStorageURL(factory);
    registerStorageDictionary(factory);
    registerStorageSet(factory);
    registerStorageJoin(factory);
    registerStorageView(factory);
    registerStorageMaterializedView(factory);
    registerStorageLiveView(factory);
    registerStorageGenerateRandom(factory);
    registerStorageExecutable(factory);
    registerStorageWindowView(factory);
#if USE_RAPIDJSON || USE_SIMDJSON
    registerStorageFuzzJSON(factory);
#endif

}

}
