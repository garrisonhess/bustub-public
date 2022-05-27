
#include "common/printer.h"
// #include "common/progress_bar.h"

#include <cstdio>


namespace bustub {
	using std::string;

void Printer::Print(const string &str) {
#ifndef BUSTUB_DISABLE_PRINT
	fprintf(stderr, "%s\n", str.c_str());
#endif
}

// void Printer::PrintProgress(int percentage, const char *pbstr, int pbwidth) {
// #ifndef BUSTUB_DISABLE_PRINT
// 	int lpad = static_cast<int>(percentage / 100.0 * pbwidth);
// 	int rpad = pbwidth - lpad;
// 	printf("\r%3d%% [%.*s%*s]", percentage, lpad, pbstr, rpad, "");
// 	fflush(stdout);
// #endif
// }

// void Printer::FinishProgressBarPrint(const char *pbstr, int pbwidth) {
// #ifndef BUSTUB_DISABLE_PRINT
// 	PrintProgress(100, pbstr, pbwidth);
// 	printf(" \n");
// 	fflush(stdout);
// #endif
// }


} // namespace bustub
