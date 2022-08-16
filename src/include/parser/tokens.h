//===----------------------------------------------------------------------===//
//                         BusTub
//
// bustub/parser/tokens.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "planner/expressions/abstract_expression.h"

namespace bustub {

//===--------------------------------------------------------------------===//
// Statements
//===--------------------------------------------------------------------===//
class SQLStatement;

class CreateStatement;
class DeleteStatement;
class InsertStatement;
class SelectStatement;
class UpdateStatement;

//===--------------------------------------------------------------------===//
// Query Node
//===--------------------------------------------------------------------===//
class QueryNode;
class SelectNode;

//===--------------------------------------------------------------------===//
// Expressions
//===--------------------------------------------------------------------===//
class AbstractExpression;

class ColumnRefExpression;
class ComparisonExpression;
class ConstantExpression;

//===--------------------------------------------------------------------===//
// TableRefs
//===--------------------------------------------------------------------===//
class TableRef;

}  // namespace bustub
