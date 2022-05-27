// #include "common/exception.h"
// #include "parser/tableref.h"
// #include "parser/transformer.h"

// namespace bustub {

// unique_ptr<TableRef> Transformer::TransformTableRefNode(bustub_libpgquery::PGNode *n) {
// 	// auto stack_checker = StackCheck();

// 	switch (n->type) {
// 	case bustub_libpgquery::T_PGRangeVar:
// 		return TransformRangeVar(reinterpret_cast<bustub_libpgquery::PGRangeVar *>(n));
// 	case bustub_libpgquery::T_PGJoinExpr:
// 		return TransformJoin(reinterpret_cast<bustub_libpgquery::PGJoinExpr *>(n));
// 	case bustub_libpgquery::T_PGRangeSubselect:
// 		return TransformRangeSubselect(reinterpret_cast<bustub_libpgquery::PGRangeSubselect *>(n));
// 	case bustub_libpgquery::T_PGRangeFunction:
// 		return TransformRangeFunction(reinterpret_cast<bustub_libpgquery::PGRangeFunction *>(n));
// 	default:
// 		throw NotImplementedException("From type not supported");
// 	}
// }

// } // namespace bustub
