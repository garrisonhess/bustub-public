// #include "parser/expression/constant_expression.h"
// #include "parser/transformer.h"
// #include "common/operator/cast_operators.h"
// #include "common/limits.h"
// #include "type/type/decimal.h"

// namespace bustub {

// unique_ptr<ConstantExpression> Transformer::TransformValue(bustub_libpgquery::PGValue val) {
// 	switch (val.type) {
// 	case bustub_libpgquery::T_PGInteger:
// 		assert(val.val.ival <= NumericLimits<int32_t>::Maximum());
// 		return std::make_unique<ConstantExpression>(Value::INTEGER((int32_t)val.val.ival));
// 	case bustub_libpgquery::T_PGBitString: // FIXME: this should actually convert to BLOB
// 	case bustub_libpgquery::T_PGString:
// 		return std::make_unique<ConstantExpression>(Value(string(val.val.str)));
// 	case bustub_libpgquery::T_PGFloat: {
// 		string_t str_val(val.val.str);
// 		bool try_cast_as_integer = true;
// 		bool try_cast_as_decimal = true;
// 		int decimal_position = -1;
// 		for (uint64_t i = 0; i < str_val.GetSize(); i++) {
// 			if (val.val.str[i] == '.') {
// 				// decimal point: cast as either decimal or double
// 				try_cast_as_integer = false;
// 				decimal_position = i;
// 			}
// 			if (val.val.str[i] == 'e' || val.val.str[i] == 'E') {
// 				// found exponent, cast as double
// 				try_cast_as_integer = false;
// 				try_cast_as_decimal = false;
// 			}
// 		}
// 		if (try_cast_as_integer) {
// 			int64_t bigint_value;
// 			// try to cast as bigint first
// 			if (TryCast::Operation<string_t, int64_t>(str_val, bigint_value)) {
// 				// successfully cast to bigint: bigint value
// 				return make_unique<ConstantExpression>(Value::BIGINT(bigint_value));
// 			}
// 			int64_t hugeint_value;
// 			// if that is not successful; try to cast as hugeint
// 			if (TryCast::Operation<string_t, hugeint_t>(str_val, hugeint_value)) {
// 				// successfully cast to bigint: bigint value
// 				return make_unique<ConstantExpression>(Value::HUGEINT(hugeint_value));
// 			}
// 		}
// 		uint64_t decimal_offset = val.val.str[0] == '-' ? 3 : 2;
// 		if (try_cast_as_decimal && decimal_position >= 0 &&
// 		    str_val.GetSize() < Decimal::MAX_WIDTH_DECIMAL + decimal_offset) {
// 			// figure out the width/scale based on the decimal position
// 			auto width = uint8_t(str_val.GetSize() - 1);
// 			auto scale = uint8_t(width - decimal_position);
// 			if (val.val.str[0] == '-') {
// 				width--;
// 			}
// 			if (width <= Decimal::MAX_WIDTH_DECIMAL) {
// 				// we can cast the value as a decimal
// 				Value val = Value(str_val);
// 				val = val.CastAs(Type::DECIMAL(width, scale));
// 				return make_unique<ConstantExpression>(move(val));
// 			}
// 		}
// 		// if there is a decimal or the value is too big to cast as either hugeint or bigint
// 		double dbl_value = Cast::Operation<string_t, double>(str_val);
// 		return make_unique<ConstantExpression>(Value::DOUBLE(dbl_value));
// 	}
// 	case bustub_libpgquery::T_PGNull:
// 		return make_unique<ConstantExpression>(Value(Type::SQLNULL));
// 	default:
// 		throw NotImplementedException("Value not implemented!");
// 	}
// }

// unique_ptr<ParsedExpression> Transformer::TransformConstant(bustub_libpgquery::PGAConst *c) {
// 	return TransformValue(c->val);
// }

// } // namespace bustub
