#include "config.h"

#include <Parsers/ASTExternalDDLQuery.h>
#include <Parsers/ASTSelectWithUnionQuery.h>
#include <Parsers/CommonParsers.h>
#include <Parsers/ExpressionListParsers.h>
#include <Parsers/ParserDropQuery.h>
#include <Parsers/ParserExternalDDLQuery.h>
#include <Parsers/ParserRenameQuery.h>


namespace DB
{

bool ParserExternalDDLQuery::parseImpl([[maybe_unused]] IParser::Pos & _pos, [[maybe_unused]] ASTPtr  & _node, [[maybe_unused]] Expected & _expected)
{
    return false;
}

}
