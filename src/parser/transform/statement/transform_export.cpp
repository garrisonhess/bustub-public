#include "bustub/parser/statement/export_statement.hpp"
#include "bustub/parser/transformer.hpp"

namespace bustub {

unique_ptr<ExportStatement> Transformer::TransformExport(bustub_libpgquery::PGNode *node) {
	auto stmt = reinterpret_cast<bustub_libpgquery::PGExportStmt *>(node);
	auto info = make_unique<CopyInfo>();
	info->file_path = stmt->filename;
	info->format = "csv";
	info->is_from = false;
	// handle export options
	TransformCopyOptions(*info, stmt->options);

	return make_unique<ExportStatement>(move(info));
}

} // namespace bustub
