#include "parser/statement/relation_statement.h"

namespace bustub {

RelationStatement::RelationStatement(shared_ptr<Relation> relation)
    : SQLStatement(StatementType::RELATION_STATEMENT), relation(move(relation)) {
}

unique_ptr<SQLStatement> RelationStatement::Copy() const {
	return unique_ptr<RelationStatement>(new RelationStatement(*this));
}

} // namespace bustub
