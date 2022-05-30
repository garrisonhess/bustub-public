
#include "common/printer.h"

#include <cstdio>

namespace bustub {
using std::string;

void Printer::Print(const string &str) {
#ifndef BUSTUB_DISABLE_PRINT
  fprintf(stderr, "%s\n", str.c_str());
#endif
}

}  // namespace bustub
