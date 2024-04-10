#include <Storages/Cache/registerRemoteFileMetadatas.h>
#include <Storages/Cache/RemoteFileMetadataFactory.h>
#include "config.h"

namespace DB
{


void registerRemoteFileMetadatas()
{
    [[maybe_unused]] auto & factory = RemoteFileMetadataFactory::instance();

}

}
