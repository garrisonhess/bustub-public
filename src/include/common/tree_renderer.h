//===----------------------------------------------------------------------===//
//                         DuckDB
//
// duckdb/common/tree_renderer.hpp
//
//
//===----------------------------------------------------------------------===//

#pragma once

#include <vector>
#include "common/constants.h"

namespace bustub {
class LogicalOperator;
class PhysicalOperator;

struct RenderTreeNode {
  string name_;
  string extra_text_;
};

struct RenderTree {
  RenderTree(uint64_t width, uint64_t height);

  unique_ptr<unique_ptr<RenderTreeNode>[]> nodes_;
  uint64_t width_;
  uint64_t height_;

 public:
  RenderTreeNode *GetNode(uint64_t x, uint64_t y);
  void SetNode(uint64_t x, uint64_t y, unique_ptr<RenderTreeNode> node);
  bool HasNode(uint64_t x, uint64_t y);

  uint64_t GetPosition(uint64_t x, uint64_t y);
};

struct TreeRendererConfig {
  void EnableDetailed() {
    max_extra_lines_ = 1000;
    detailed_ = true;
  }

  void EnableStandard() {
    max_extra_lines_ = 30;
    detailed_ = false;
  }

  uint64_t maximum_render_width_ = 240;
  uint64_t node_render_width_ = 29;
  uint64_t minimum_render_width_ = 15;
  uint64_t max_extra_lines_ = 30;
  bool detailed_ = false;

#ifndef BUSTUB_ASCII_TREE_RENDERER
  const char *ltcorner_ = "\342\224\214";  // "┌";
  const char *rtcorner_ = "\342\224\220";  // "┐";
  const char *ldcorner_ = "\342\224\224";  // "└";
  const char *rdcorner_ = "\342\224\230";  // "┘";

  const char *middle_ = "\342\224\274";   // "┼";
  const char *tmiddle_ = "\342\224\254";  // "┬";
  const char *lmiddle_ = "\342\224\234";  // "├";
  const char *rmiddle_ = "\342\224\244";  // "┤";
  const char *dmiddle_ = "\342\224\264";  // "┴";

  const char *vertical_ = "\342\224\202";    // "│";
  const char *horizontal_ = "\342\224\200";  // "─";
#else
  // ASCII version
  const char *LTCORNER = "<";
  const char *RTCORNER = ">";
  const char *LDCORNER = "<";
  const char *RDCORNER = ">";

  const char *MIDDLE = "+";
  const char *TMIDDLE = "+";
  const char *LMIDDLE = "+";
  const char *RMIDDLE = "+";
  const char *DMIDDLE = "+";

  const char *VERTICAL = "|";
  const char *HORIZONTAL = "-";
#endif
};

class TreeRenderer {
 public:
  explicit TreeRenderer(TreeRendererConfig config_p = TreeRendererConfig()) : config_(config_p) {}

  string ToString(const LogicalOperator &op);

  void Render(const LogicalOperator &op, std::ostream &ss);

  void ToStream(RenderTree &root, std::ostream &ss);

  void EnableDetailed() { config_.EnableDetailed(); }
  void EnableStandard() { config_.EnableStandard(); }

 private:
  unique_ptr<RenderTree> CreateTree(const LogicalOperator &op);

  string ExtraInfoSeparator();
  unique_ptr<RenderTreeNode> CreateRenderNode(string name, string extra_info);
  unique_ptr<RenderTreeNode> CreateNode(const LogicalOperator &op);

 private:
  //! The configuration used for rendering
  TreeRendererConfig config_;

 private:
  void RenderTopLayer(RenderTree &root, std::ostream &ss, uint64_t y);
  void RenderBoxContent(RenderTree &root, std::ostream &ss, uint64_t y);
  void RenderBottomLayer(RenderTree &root, std::ostream &ss, uint64_t y);

  bool CanSplitOnThisChar(char l);
  bool IsPadding(char l);
  string RemovePadding(string l);
  void SplitUpExtraInfo(const string &extra_info, vector<string> &result);
  void SplitStringBuffer(const string &source, vector<string> &result);

  template <class T>
  uint64_t CreateRenderTreeRecursive(RenderTree &result, const T &op, uint64_t x, uint64_t y);

  template <class T>
  unique_ptr<RenderTree> CreateRenderTree(const T &op);
};

}  // namespace bustub