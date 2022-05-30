// #include "parser/parsed_expression.h"
// #include "parser/transformer.h"
// #include "parser/query_node/select_node.h"
// #include "parser/expression_map.h"
// #include "parser/expression/function_expression.h"

// namespace bustub {

// static void CheckGroupingSetMax(uint64_t count) {
// 	static constexpr const uint64_t MAX_GROUPING_SETS = 65535;
// 	if (count > MAX_GROUPING_SETS) {
// 		throw ParserException("Maximum grouping set count of %d exceeded", MAX_GROUPING_SETS);
// 	}
// }

// struct GroupingExpressionMap {
// 	expression_map_t<uint64_t> map;
// };

// static GroupingSet VectorToGroupingSet(vector<uint64_t> &indexes) {
// 	GroupingSet result;
// 	for (uint64_t i = 0; i < indexes.size(); i++) {
// 		result.insert(indexes[i]);
// 	}
// 	return result;
// }

// static void MergeGroupingSet(GroupingSet &result, GroupingSet &other) {
// 	CheckGroupingSetMax(result.size() + other.size());
// 	result.insert(other.begin(), other.end());
// }

// void Transformer::AddGroupByExpression(unique_ptr<ParsedExpression> expression, GroupingExpressionMap &map,
//                                        GroupByNode &result, vector<uint64_t> &result_set) {
// 	if (expression->type == ExpressionType::FUNCTION) {
// 		auto &func = (FunctionExpression &)*expression;
// 		if (func.function_name == "row") {
// 			for (auto &child : func.children) {
// 				AddGroupByExpression(move(child), map, result, result_set);
// 			}
// 			return;
// 		}
// 	}
// 	auto entry = map.map.find(expression.get());
// 	uint64_t result_idx;
// 	if (entry == map.map.end()) {
// 		result_idx = result.group_expressions.size();
// 		map.map[expression.get()] = result_idx;
// 		result.group_expressions.push_back(move(expression));
// 	} else {
// 		result_idx = entry->second;
// 	}
// 	result_set.push_back(result_idx);
// }

// static void AddCubeSets(const GroupingSet &current_set, vector<GroupingSet> &result_set,
//                         vector<GroupingSet> &result_sets, uint64_t start_idx = 0) {
// 	CheckGroupingSetMax(result_sets.size());
// 	result_sets.push_back(current_set);
// 	for (uint64_t k = start_idx; k < result_set.size(); k++) {
// 		auto child_set = current_set;
// 		MergeGroupingSet(child_set, result_set[k]);
// 		AddCubeSets(child_set, result_set, result_sets, k + 1);
// 	}
// }

// void Transformer::TransformGroupByExpression(bustub_libpgquery::PGNode *n, GroupingExpressionMap &map,
//                                              GroupByNode &result, vector<uint64_t> &indexes) {
// 	auto expression = TransformExpression(n);
// 	AddGroupByExpression(move(expression), map, result, indexes);
// }

// // If one GROUPING SETS clause is nested inside another,
// // the effect is the same as if all the elements of the inner clause had been written directly in the outer clause.
// void Transformer::TransformGroupByNode(bustub_libpgquery::PGNode *n, GroupingExpressionMap &map, SelectNode &result,
//                                        vector<GroupingSet> &result_sets) {
// 	if (n->type == bustub_libpgquery::T_PGGroupingSet) {
// 		auto grouping_set = (bustub_libpgquery::PGGroupingSet *)n;
// 		switch (grouping_set->kind) {
// 		case bustub_libpgquery::GROUPING_SET_EMPTY:
// 			result_sets.emplace_back();
// 			break;
// 		case bustub_libpgquery::GROUPING_SET_ALL: {
// 			result.aggregate_handling = AggregateHandling::FORCE_AGGREGATES;
// 			break;
// 		}
// 		case bustub_libpgquery::GROUPING_SET_SETS: {
// 			for (auto node = grouping_set->content->head; node; node = node->next) {
// 				auto pg_node = (bustub_libpgquery::PGNode *)node->data.ptr_value;
// 				TransformGroupByNode(pg_node, map, result, result_sets);
// 			}
// 			break;
// 		}
// 		case bustub_libpgquery::GROUPING_SET_ROLLUP: {
// 			vector<GroupingSet> rollup_sets;
// 			for (auto node = grouping_set->content->head; node; node = node->next) {
// 				auto pg_node = (bustub_libpgquery::PGNode *)node->data.ptr_value;
// 				vector<uint64_t> rollup_set;
// 				TransformGroupByExpression(pg_node, map, result.groups, rollup_set);
// 				rollup_sets.push_back(VectorToGroupingSet(rollup_set));
// 			}
// 			// generate the subsets of the rollup set and add them to the grouping sets
// 			GroupingSet current_set;
// 			result_sets.push_back(current_set);
// 			for (uint64_t i = 0; i < rollup_sets.size(); i++) {
// 				MergeGroupingSet(current_set, rollup_sets[i]);
// 				result_sets.push_back(current_set);
// 			}
// 			break;
// 		}
// 		case bustub_libpgquery::GROUPING_SET_CUBE: {
// 			vector<GroupingSet> cube_sets;
// 			for (auto node = grouping_set->content->head; node; node = node->next) {
// 				auto pg_node = (bustub_libpgquery::PGNode *)node->data.ptr_value;
// 				vector<uint64_t> cube_set;
// 				TransformGroupByExpression(pg_node, map, result.groups, cube_set);
// 				cube_sets.push_back(VectorToGroupingSet(cube_set));
// 			}
// 			// generate the subsets of the rollup set and add them to the grouping sets
// 			GroupingSet current_set;
// 			AddCubeSets(current_set, cube_sets, result_sets, 0);
// 			break;
// 		}
// 		default:
// 			throw Exception("Unsupported GROUPING SET type %d", grouping_set->kind);
// 		}
// 	} else {
// 		vector<uint64_t> indexes;
// 		TransformGroupByExpression(n, map, result.groups, indexes);
// 		result_sets.push_back(VectorToGroupingSet(indexes));
// 	}
// }

// // If multiple grouping items are specified in a single GROUP BY clause,
// // then the final list of grouping sets is the cross product of the individual items.
// bool Transformer::TransformGroupBy(bustub_libpgquery::PGList *group, SelectNode &select_node) {
// 	if (!group) {
// 		return false;
// 	}
// 	auto &result = select_node.groups;
// 	GroupingExpressionMap map;
// 	for (auto node = group->head; node != nullptr; node = node->next) {
// 		auto n = reinterpret_cast<bustub_libpgquery::PGNode *>(node->data.ptr_value);
// 		vector<GroupingSet> result_sets;
// 		TransformGroupByNode(n, map, select_node, result_sets);
// 		CheckGroupingSetMax(result_sets.size());
// 		if (result.grouping_sets.empty()) {
// 			// no grouping sets yet: use the current set of grouping sets
// 			result.grouping_sets = move(result_sets);
// 		} else {
// 			// compute the cross product
// 			vector<GroupingSet> new_sets;
// 			uint64_t grouping_set_count = result.grouping_sets.size() * result_sets.size();
// 			CheckGroupingSetMax(grouping_set_count);
// 			new_sets.reserve(grouping_set_count);
// 			for (uint64_t current_idx = 0; current_idx < result.grouping_sets.size(); current_idx++) {
// 				auto &current_set = result.grouping_sets[current_idx];
// 				for (uint64_t new_idx = 0; new_idx < result_sets.size(); new_idx++) {
// 					auto &new_set = result_sets[new_idx];
// 					GroupingSet set;
// 					set.insert(current_set.begin(), current_set.end());
// 					set.insert(new_set.begin(), new_set.end());
// 					new_sets.push_back(move(set));
// 				}
// 			}
// 			result.grouping_sets = move(new_sets);
// 		}
// 	}
// 	return true;
// }

// } // namespace bustub
