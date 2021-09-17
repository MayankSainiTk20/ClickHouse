#pragma once

#include <DataStreams/IBlockOutputStream.h>
#include <DataStreams/BlockIO.h>
#include <Interpreters/IInterpreter.h>
#include <Parsers/ASTInsertQuery.h>
#include <Storages/StorageInMemoryMetadata.h>
#include <IO/ReadBuffer.h>

namespace DB
{

struct ExceptionKeepingTransformRuntimeData;
using ExceptionKeepingTransformRuntimeDataPtr = std::shared_ptr<ExceptionKeepingTransformRuntimeData>;

class Chain;

/** Interprets the INSERT query.
  */
class InterpreterInsertQuery : public IInterpreter, WithContext
{
public:
    InterpreterInsertQuery(
        const ASTPtr & query_ptr_,
        ContextPtr context_,
        bool allow_materialized_ = false,
        bool no_squash_ = false,
        bool no_destination_ = false);

    /** Prepare a request for execution. Return block streams
      * - the stream into which you can write data to execute the query, if INSERT;
      * - the stream from which you can read the result of the query, if SELECT and similar;
      * Or nothing if the request INSERT SELECT (self-sufficient query - does not accept the input data, does not return the result).
      */
    BlockIO execute() override;
    Chain buildChain();

    /// Returns only sinks, without input sources.
    Processors getSinks();

    StorageID getDatabaseTable() const;

    Chain buildChain(
        const StoragePtr & table,
        const StorageMetadataPtr & metadata_snapshot,
        const Names & columns,
        ExceptionKeepingTransformRuntimeDataPtr runtime_data = nullptr);

    void extendQueryLogElemImpl(QueryLogElement & elem, const ASTPtr & ast, ContextPtr context_) const override;
    Block getSampleBlock(const ASTInsertQuery & query, const StoragePtr & table, const StorageMetadataPtr & metadata_snapshot) const;
    Block getSampleBlock(const Names & names, const StoragePtr & table, const StorageMetadataPtr & metadata_snapshot) const;
    StoragePtr getTable(ASTInsertQuery & query);

private:
    std::pair<BlockIO, BlockOutputStreams> executeImpl(
        const StoragePtr & table, const StorageMetadataPtr & metadata_snapshot, Block & sample_block);

    ASTPtr query_ptr;
    const bool allow_materialized;
    const bool no_squash;
    const bool no_destination;

    Chain buildChainImpl(
        const StoragePtr & table,
        const StorageMetadataPtr & metadata_snapshot,
        const Block & query_sample_block,
        ExceptionKeepingTransformRuntimeDataPtr runtime_data = nullptr);
};


}
