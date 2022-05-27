// #include "common/limits.h"
// #include "parser/expression/cast_expression.h"
// #include "parser/expression/constant_expression.h"
// #include "parser/transformer.h"
// #include "common/operator/cast_operators.h"

// namespace bustub {

// unique_ptr<ParsedExpression> Transformer::TransformTypeCast(bustub_libpgquery::PGTypeCast *root) {
// 	D_ASSERT(root);

// 	// get the type to cast to
// 	auto type_name = root->typeName;
// 	LogicalType target_type = TransformTypeName(type_name);

// 	// check for a constant BLOB value, then return ConstantExpression with BLOB
// 	if (!root->tryCast && target_type == LogicalType::BLOB && root->arg->type == bustub_libpgquery::T_PGAConst) {
// 		auto c = reinterpret_cast<bustub_libpgquery::PGAConst *>(root->arg);
// 		if (c->val.type == bustub_libpgquery::T_PGString) {
// 			return make_unique<ConstantExpression>(Value::BLOB(string(c->val.val.str)));
// 		}
// 	}
// 	// transform the expression node
// 	auto expression = TransformExpression(root->arg);
// 	bool try_cast = root->tryCast;

// 	// now create a cast operation
// 	return make_unique<CastExpression>(target_type, move(expression), try_cast);
// }

// } // namespace bustub
