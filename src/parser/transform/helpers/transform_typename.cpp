#include "common/exception.h"
#include "common/pair.h"
#include "common/unordered_set.h"

#include "parser/transformer.h"
#include "common/types/decimal.h"

namespace bustub {

Type Transformer::TransformTypeName(bustub_libpgquery::PGTypeName *type_name) {
	if (type_name->type != bustub_libpgquery::T_PGTypeName) {
		throw ParserException("Expected a type");
	}
	auto stack_checker = StackCheck();

	auto name = (reinterpret_cast<bustub_libpgquery::PGValue *>(type_name->names->tail->data.ptr_value)->val.str);
	// transform it to the SQL type
	TypeId base_type = TransformStringToTypeId(name);

	Type result_type;
	if (base_type == TypeId::STRUCT) {
		if (!type_name->typmods || type_name->typmods->length == 0) {
			throw ParserException("Struct needs a name and entries");
		}
		child_list_t<Type> children;
		unordered_set<string> name_collision_set;

		for (auto node = type_name->typmods->head; node; node = node->next) {
			auto &type_val = *((bustub_libpgquery::PGList *)node->data.ptr_value);
			if (type_val.length != 2) {
				throw ParserException("Struct entry needs an entry name and a type name");
			}

			auto entry_name_node = (bustub_libpgquery::PGValue *)(type_val.head->data.ptr_value);
			D_ASSERT(entry_name_node->type == bustub_libpgquery::T_PGString);
			auto entry_type_node = (bustub_libpgquery::PGValue *)(type_val.tail->data.ptr_value);
			D_ASSERT(entry_type_node->type == bustub_libpgquery::T_PGTypeName);

			auto entry_name = string(entry_name_node->val.str);
			D_ASSERT(!entry_name.empty());

			if (name_collision_set.find(entry_name) != name_collision_set.end()) {
				throw ParserException("Duplicate struct entry name \"%s\"", entry_name);
			}
			name_collision_set.insert(entry_name);

			auto entry_type = TransformTypeName((bustub_libpgquery::PGTypeName *)entry_type_node);
			children.push_back(make_pair(entry_name, entry_type));
		}
		D_ASSERT(!children.empty());
		result_type = Type::STRUCT(move(children));
	} else if (base_type == TypeId::MAP) {
		//! We transform MAP<TYPE_KEY, TYPE_VALUE> to STRUCT<LIST<key: TYPE_KEY>, LIST<value: TYPE_VALUE>>

		if (!type_name->typmods || type_name->typmods->length != 2) {
			throw ParserException("Map type needs exactly two entries, key and value type");
		}
		child_list_t<Type> children;
		auto key_type = TransformTypeName((bustub_libpgquery::PGTypeName *)type_name->typmods->head->data.ptr_value);
		auto value_type = TransformTypeName((bustub_libpgquery::PGTypeName *)type_name->typmods->tail->data.ptr_value);

		children.push_back({"key", Type::LIST(key_type)});
		children.push_back({"value", Type::LIST(value_type)});

		D_ASSERT(children.size() == 2);

		result_type = Type::MAP(move(children));
	} else {
		int8_t width, scale;
		if (base_type == TypeId::DECIMAL) {
			// default decimal width/scale
			width = 18;
			scale = 3;
		} else {
			width = 0;
			scale = 0;
		}
		// check any modifiers
		int modifier_idx = 0;
		if (type_name->typmods) {
			for (auto node = type_name->typmods->head; node; node = node->next) {
				auto &const_val = *((bustub_libpgquery::PGAConst *)node->data.ptr_value);
				if (const_val.type != bustub_libpgquery::T_PGAConst ||
				    const_val.val.type != bustub_libpgquery::T_PGInteger) {
					throw ParserException("Expected an integer constant as type modifier");
				}
				if (const_val.val.val.ival < 0) {
					throw ParserException("Negative modifier not supported");
				}
				if (modifier_idx == 0) {
					width = const_val.val.val.ival;
				} else if (modifier_idx == 1) {
					scale = const_val.val.val.ival;
				} else {
					throw ParserException("A maximum of two modifiers is supported");
				}
				modifier_idx++;
			}
		}
		switch (base_type) {
		case TypeId::VARCHAR:
			if (modifier_idx > 1) {
				throw ParserException("VARCHAR only supports a single modifier");
			}
			// FIXME: create CHECK constraint based on varchar width
			width = 0;
			result_type = Type::VARCHAR;
			break;
		case TypeId::DECIMAL:
			if (modifier_idx == 1) {
				// only width is provided: set scale to 0
				scale = 0;
			}
			if (width <= 0 || width > Decimal::MAX_WIDTH_DECIMAL) {
				throw ParserException("Width must be between 1 and %d!", (int)Decimal::MAX_WIDTH_DECIMAL);
			}
			if (scale > width) {
				throw ParserException("Scale cannot be bigger than width");
			}
			result_type = Type::DECIMAL(width, scale);
			break;
		case TypeId::INTERVAL:
			if (modifier_idx > 1) {
				throw ParserException("INTERVAL only supports a single modifier");
			}
			width = 0;
			result_type = Type::INTERVAL;
			break;
		case TypeId::USER: {
			string user_type_name {name};
			result_type = Type::USER(user_type_name);
			break;
		}
		default:
			if (modifier_idx > 0) {
				throw Exception("Type %s does not support any modifiers!");
			}
			result_type = Type(base_type);
			break;
		}
	}
	if (type_name->arrayBounds) {
		// array bounds: turn the type into a list
		uint64_t extra_stack = 0;
		for (auto cell = type_name->arrayBounds->head; cell != nullptr; cell = cell->next) {
			result_type = Type::LIST(move(result_type));
			StackCheck(extra_stack++);
		}
	}
	return result_type;
}

} // namespace bustub
