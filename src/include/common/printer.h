//===----------------------------------------------------------------------===//
//                         DuckDB
//
// common/printer.h
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <string>

namespace bustub {

//! Printer is a static class that allows printing to logs or stdout/stderr
class Printer {
 public:
  //! Print the object to stderr
  static void Print(const std::string &str);
  //! Prints Progress
  static void PrintProgress(int percentage, const char *pbstr, int pbwidth);
  //! Prints an empty line when progress bar is done
  static void FinishProgressBarPrint(const char *pbstr, int pbwidth);
  //! Whether or not we are printing to a terminal
  static bool IsTerminal();
};
}  // namespace bustub