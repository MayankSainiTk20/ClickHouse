#include <Interpreters/InterpreterFactory.h>
#include <Interpreters/InterpreterShowColumnsQuery.h>

#include <Common/quoteString.h>
#include <Common/escapeString.h>
#include <IO/Operators.h>
#include <IO/WriteBufferFromString.h>
#include <Parsers/ASTShowColumnsQuery.h>
#include <Parsers/formatAST.h>
#include <Interpreters/ClientInfo.h>
#include <Interpreters/Context.h>
#include <Interpreters/executeQuery.h>


namespace DB
{


InterpreterShowColumnsQuery::InterpreterShowColumnsQuery(const ASTPtr & query_ptr_, ContextMutablePtr context_)
    : WithMutableContext(context_)
    , query_ptr(query_ptr_)
{
}


String InterpreterShowColumnsQuery::getRewrittenQuery()
{
    const auto & query = query_ptr->as<ASTShowColumnsQuery &>();


    WriteBufferFromOwnString buf_database;
    String resolved_database = getContext()->resolveDatabase(query.database);
    String database = escapeString(resolved_database);
    String table = escapeString(query.table);

    String rewritten_query;

    rewritten_query += R"(
SELECT
    name_ AS field,
    )";
    rewritten_query += R"(
    type_ AS type,
        )";
        

    rewritten_query += R"(
    multiIf(startsWith(type_, 'Nullable('), 'YES', startsWith(type_, 'LowCardinality(Nullable('), 'YES', 'NO') AS `null`,
    trim(concatWithSeparator(' ', if (is_in_primary_key_, 'PRI', ''), if (is_in_sorting_key_, 'SOR', ''))) AS key,
    if (default_kind_ IN ('ALIAS', 'DEFAULT', 'MATERIALIZED'), default_expression_, NULL) AS default,
    '' AS extra )";

    // TODO Interpret query.extended. It is supposed to show internal/virtual columns. Need to fetch virtual column names, see
    // IStorage::getVirtualsList(). We can't easily do that via SQL.

    if (query.full)
    {
        /// "Full" mode is mostly for MySQL compat
        /// - collation: no such thing in ClickHouse
        /// - comment
        /// - privileges: <not implemented, TODO ask system.grants>
        rewritten_query += R"(,
    NULL AS collation,
    comment_ AS comment,
    '' AS privileges )";
    }

    rewritten_query += fmt::format(R"(
-- need to rename columns of the base table to avoid "CYCLIC_ALIASES" errors
FROM (SELECT name AS name_,
             database AS database_,
             table AS table_,
             type AS type_,
             is_in_primary_key AS is_in_primary_key_,
             is_in_sorting_key AS is_in_sorting_key_,
             default_kind AS default_kind_,
             default_expression AS default_expression_,
             comment AS comment_
      FROM system.columns)
WHERE
    database_ = '{}'
    AND table_ = '{}' )", database, table);

    if (!query.like.empty())
    {
        rewritten_query += " AND field ";
        if (query.not_like)
            rewritten_query += "NOT ";
        if (query.case_insensitive_like)
            rewritten_query += "ILIKE ";
        else
            rewritten_query += "LIKE ";
        rewritten_query += fmt::format("'{}'", query.like);
    }
    else if (query.where_expression)
        rewritten_query += fmt::format(" AND ({})", query.where_expression);

    rewritten_query += " ORDER BY field, type, null, key, default, extra";

    if (query.limit_length)
        rewritten_query += fmt::format(" LIMIT {}", query.limit_length);

    return rewritten_query;
}


BlockIO InterpreterShowColumnsQuery::execute()
{
    return executeQuery(getRewrittenQuery(), getContext(), QueryFlags{ .internal = true }).second;
}

void registerInterpreterShowColumnsQuery(InterpreterFactory & factory)
{
    auto create_fn = [] (const InterpreterFactory::Arguments & args)
    {
        return std::make_unique<InterpreterShowColumnsQuery>(args.query, args.context);
    };
    factory.registerInterpreter("InterpreterShowColumnsQuery", create_fn);
}

}
