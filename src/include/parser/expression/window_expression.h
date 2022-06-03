//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/parser/expression/window_expression.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include "common/exception.h"
#include "common/string_util.h"
#include "parser/parsed_expression.h"
#include "parser/query_node.h"

namespace bustub {

enum class WindowBoundary : uint8_t {
  INVALID = 0,
  UNBOUNDED_PRECEDING = 1,
  UNBOUNDED_FOLLOWING = 2,
  CURRENT_ROW_RANGE = 3,
  CURRENT_ROW_ROWS = 4,
  EXPR_PRECEDING_ROWS = 5,
  EXPR_FOLLOWING_ROWS = 6,
  EXPR_PRECEDING_RANGE = 7,
  EXPR_FOLLOWING_RANGE = 8
};

//! The WindowExpression represents a window function in the query. They are a special case of aggregates which is why
//! they inherit from them.
class WindowExpression : public ParsedExpression {
 public:
  WindowExpression(ExpressionType type, string schema_name, const string &function_name);

  //! Schema of the aggregate function
  string schema_;
  //! Name of the aggregate function
  string function_name_;
  //! The child expression of the main window aggregate
  vector<unique_ptr<ParsedExpression>> children_;
  //! The set of expressions to partition by
  vector<unique_ptr<ParsedExpression>> partitions_;
  //! The set of ordering clauses
  vector<OrderByNode> orders_;
  //! True to ignore NULL values
  bool ignore_nulls_;
  //! The window boundaries
  WindowBoundary start_ = WindowBoundary::INVALID;
  WindowBoundary end_ = WindowBoundary::INVALID;

  unique_ptr<ParsedExpression> start_expr_;
  unique_ptr<ParsedExpression> end_expr_;
  //! Offset and default expressions for WINDOW_LEAD and WINDOW_LAG functions
  unique_ptr<ParsedExpression> offset_expr_;
  unique_ptr<ParsedExpression> default_expr_;

 public:
  bool IsWindow() const override { return true; }

  //! Convert the Expression to a String
  string ToString() const override;

  static bool Equals(const WindowExpression *a, const WindowExpression *b);

  unique_ptr<ParsedExpression> Copy() const override;

  void Serialize(FieldWriter &writer) const override;
  static unique_ptr<ParsedExpression> Deserialize(ExpressionType type, FieldReader &reader);

 public:
  template <class T, class BASE, class ORDER_NODE>
  static string ToString(const T &entry, const string &schema, const string &function_name) {
    // Start with function call
    string result = schema.empty() ? function_name : schema + "." + function_name;
    result += "(";
    result += StringUtil::Join(entry.children, entry.children.size(), ", ",
                               [](const unique_ptr<BASE> &child) { return child->ToString(); });
    // Lead/Lag extra arguments
    if (entry.offset_expr.get()) {
      result += ", ";
      result += entry.offset_expr->ToString();
    }
    if (entry.default_expr.get()) {
      result += ", ";
      result += entry.default_expr->ToString();
    }
    // IGNORE NULLS
    if (entry.ignore_nulls) {
      result += " IGNORE NULLS";
    }
    // Over clause
    result += ") OVER(";
    string sep;

    // Partitions
    if (!entry.partitions.empty()) {
      result += "PARTITION BY ";
      result += StringUtil::Join(entry.partitions, entry.partitions.size(), ", ",
                                 [](const unique_ptr<BASE> &partition) { return partition->ToString(); });
      sep = " ";
    }

    // Orders
    if (!entry.orders.empty()) {
      result += sep;
      result += "ORDER BY ";
      result += StringUtil::Join(entry.orders, entry.orders.size(), ", ",
                                 [](const ORDER_NODE &order) { return order.ToString(); });
      sep = " ";
    }

    // Rows/Range
    string units = "ROWS";
    string from;
    switch (entry.start) {
      case WindowBoundary::CURRENT_ROW_RANGE:
      case WindowBoundary::CURRENT_ROW_ROWS:
        from = "CURRENT ROW";
        units = (entry.start == WindowBoundary::CURRENT_ROW_RANGE) ? "RANGE" : "ROWS";
        break;
      case WindowBoundary::UNBOUNDED_PRECEDING:
        if (entry.end != WindowBoundary::CURRENT_ROW_RANGE) {
          from = "UNBOUNDED PRECEDING";
        }
        break;
      case WindowBoundary::EXPR_PRECEDING_ROWS:
      case WindowBoundary::EXPR_PRECEDING_RANGE:
        from = entry.start_expr->ToString() + " PRECEDING";
        units = (entry.start == WindowBoundary::EXPR_PRECEDING_RANGE) ? "RANGE" : "ROWS";
        break;
      case WindowBoundary::EXPR_FOLLOWING_ROWS:
      case WindowBoundary::EXPR_FOLLOWING_RANGE:
        from = entry.start_expr->ToString() + " FOLLOWING";
        units = (entry.start == WindowBoundary::EXPR_FOLLOWING_RANGE) ? "RANGE" : "ROWS";
        break;
      default:
        throw Exception("Unrecognized FROM in WindowExpression");
    }

    string to;
    switch (entry.end) {
      case WindowBoundary::CURRENT_ROW_RANGE:
        if (entry.start != WindowBoundary::UNBOUNDED_PRECEDING) {
          to = "CURRENT ROW";
          units = "RANGE";
        }
        break;
      case WindowBoundary::CURRENT_ROW_ROWS:
        to = "CURRENT ROW";
        units = "ROWS";
        break;
      case WindowBoundary::UNBOUNDED_PRECEDING:
        to = "UNBOUNDED PRECEDING";
        break;
      case WindowBoundary::UNBOUNDED_FOLLOWING:
        to = "UNBOUNDED FOLLOWING";
        break;
      case WindowBoundary::EXPR_PRECEDING_ROWS:
      case WindowBoundary::EXPR_PRECEDING_RANGE:
        to = entry.end_expr->ToString() + " PRECEDING";
        units = (entry.end == WindowBoundary::EXPR_PRECEDING_RANGE) ? "RANGE" : "ROWS";
        break;
      case WindowBoundary::EXPR_FOLLOWING_ROWS:
      case WindowBoundary::EXPR_FOLLOWING_RANGE:
        to = entry.end_expr->ToString() + " FOLLOWING";
        units = (entry.end == WindowBoundary::EXPR_FOLLOWING_RANGE) ? "RANGE" : "ROWS";
        break;
      default:
        throw Exception("Unrecognized TO in WindowExpression");
    }

    if (!from.empty() || !to.empty()) {
      result += sep + units;
    }
    if (!from.empty() && !to.empty()) {
      result += " BETWEEN ";
      result += from;
      result += " AND ";
      result += to;
    } else if (!from.empty()) {
      result += " ";
      result += from;
    } else if (!to.empty()) {
      result += " ";
      result += to;
    }

    result += ")";

    return result;
  }
};
}  // namespace bustub
