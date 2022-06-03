// #include "common/tree_renderer.h"
// #include "common/string_util.h"

// #include "utf8proc_wrapper.hpp"

// #include <sstream>

// namespace bustub {

// RenderTree::RenderTree(uint64_t width_p, uint64_t height_p) : width_(width_p), height_(height_p) {
//   nodes_ = unique_ptr<unique_ptr<RenderTreeNode>[]>(new unique_ptr<RenderTreeNode>[(width_ + 1) * (height_ + 1)]);
// }

// RenderTreeNode *RenderTree::GetNode(uint64_t x, uint64_t y) {
//   if (x >= width_ || y >= height_) {
//     return nullptr;
//   }
//   return nodes_[GetPosition(x, y)].get();
// }

// bool RenderTree::HasNode(uint64_t x, uint64_t y) {
//   if (x >= width_ || y >= height_) {
//     return false;
//   }
//   return nodes_[GetPosition(x, y)] != nullptr;
// }

// uint64_t RenderTree::GetPosition(uint64_t x, uint64_t y) { return y * width_ + x; }

// void RenderTree::SetNode(uint64_t x, uint64_t y, unique_ptr<RenderTreeNode> node) {
//   nodes_[GetPosition(x, y)] = move(node);
// }

// void TreeRenderer::RenderTopLayer(RenderTree &root, std::ostream &ss, uint64_t y) {
//   for (uint64_t x = 0; x < root.width_; x++) {
//     if (x * config_.node_render_width_ >= config_.maximum_render_width_) {
//       break;
//     }
//     if (root.HasNode(x, y)) {
//       ss << config_.ltcorner_;
//       ss << StringUtil::Repeat(config_.horizontal_, config_.node_render_width_ / 2 - 1);
//       if (y == 0) {
//         // top level node: no node above this one
//         ss << config_.horizontal_;
//       } else {
//         // render connection to node above this one
//         ss << config_.dmiddle_;
//       }
//       ss << StringUtil::Repeat(config_.horizontal_, config_.node_render_width_ / 2 - 1);
//       ss << config_.rtcorner_;
//     } else {
//       ss << StringUtil::Repeat(" ", config_.node_render_width_);
//     }
//   }
//   ss << std::endl;
// }

// void TreeRenderer::RenderBottomLayer(RenderTree &root, std::ostream &ss, uint64_t y) {
//   for (uint64_t x = 0; x <= root.width_; x++) {
//     if (x * config_.node_render_width_ >= config_.maximum_render_width_) {
//       break;
//     }
//     if (root.HasNode(x, y)) {
//       ss << config_.ldcorner_;
//       ss << StringUtil::Repeat(config_.horizontal_, config_.node_render_width_ / 2 - 1);
//       if (root.HasNode(x, y + 1)) {
//         // node below this one: connect to that one
//         ss << config_.tmiddle_;
//       } else {
//         // no node below this one: end the box
//         ss << config_.horizontal_;
//       }
//       ss << StringUtil::Repeat(config_.horizontal_, config_.node_render_width_ / 2 - 1);
//       ss << config_.rdcorner_;
//     } else if (root.HasNode(x, y + 1)) {
//       ss << StringUtil::Repeat(" ", config_.node_render_width_ / 2);
//       ss << config_.vertical_;
//       ss << StringUtil::Repeat(" ", config_.node_render_width_ / 2);
//     } else {
//       ss << StringUtil::Repeat(" ", config_.node_render_width_);
//     }
//   }
//   ss << std::endl;
// }

// string AdjustTextForRendering(string source, uint64_t max_render_width) {
//   uint64_t cpos = 0;
//   uint64_t render_width = 0;
//   vector<std::pair<uint64_t, uint64_t>> render_widths;
//   while (cpos < source.size()) {
//     uint64_t char_render_width = Utf8Proc::RenderWidth(source.c_str(), source.size(), cpos);
//     cpos = Utf8Proc::NextGraphemeCluster(source.c_str(), source.size(), cpos);
//     render_width += char_render_width;
//     render_widths.emplace_back(cpos, render_width);
//     if (render_width > max_render_width) {
//       break;
//     }
//   }
//   if (render_width > max_render_width) {
//     // need to find a position to truncate
//     for (uint64_t pos = render_widths.size(); pos > 0; pos--) {
//       if (render_widths[pos - 1].second < max_render_width - 4) {
//         return source.substr(0, render_widths[pos - 1].first) + "..." +
//                string(max_render_width - render_widths[pos - 1].second - 3, ' ');
//       }
//     }
//     source = "...";
//   }
//   // need to pad with spaces
//   uint64_t total_spaces = max_render_width - render_width;
//   uint64_t half_spaces = total_spaces / 2;
//   uint64_t extra_left_space = total_spaces % 2 == 0 ? 0 : 1;
//   return string(half_spaces + extra_left_space, ' ') + source + string(half_spaces, ' ');
// }

// static bool NodeHasMultipleChildren(RenderTree &root, uint64_t x, uint64_t y) {
//   for (; x < root.width_ && !root.HasNode(x + 1, y); x++) {
//     if (root.HasNode(x + 1, y + 1)) {
//       return true;
//     }
//   }
//   return false;
// }

// void TreeRenderer::RenderBoxContent(RenderTree &root, std::ostream &ss, uint64_t y) {
//   // we first need to figure out how high our boxes are going to be
//   vector<vector<string>> extra_info;
//   uint64_t extra_height = 0;
//   extra_info.resize(root.width_);
//   for (uint64_t x = 0; x < root.width_; x++) {
//     auto node = root.GetNode(x, y);
//     if (node != nullptr) {
//       SplitUpExtraInfo(node->extra_text_, extra_info[x]);
//       if (extra_info[x].size() > extra_height) {
//         extra_height = extra_info[x].size();
//       }
//     }
//   }
//   extra_height = std::min<uint64_t>(extra_height, config_.max_extra_lines_);
//   uint64_t halfway_point = (extra_height + 1) / 2;
//   // now we render the actual node
//   for (uint64_t render_y = 0; render_y <= extra_height; render_y++) {
//     for (uint64_t x = 0; x < root.width_; x++) {
//       if (x * config_.node_render_width_ >= config_.maximum_render_width_) {
//         break;
//       }
//       auto node = root.GetNode(x, y);
//       if (node == nullptr) {
//         if (render_y == halfway_point) {
//           bool has_child_to_the_right = NodeHasMultipleChildren(root, x, y);
//           if (root.HasNode(x, y + 1)) {
//             // node right below this one
//             ss << StringUtil::Repeat(config_.horizontal_, config_.node_render_width_ / 2);
//             ss << config_.rtcorner_;
//             if (has_child_to_the_right) {
//               // but we have another child to the right! keep rendering the line
//               ss << StringUtil::Repeat(config_.horizontal_, config_.node_render_width_ / 2);
//             } else {
//               // only a child below this one: fill the rest with spaces
//               ss << StringUtil::Repeat(" ", config_.node_render_width_ / 2);
//             }
//           } else if (has_child_to_the_right) {
//             // child to the right, but no child right below this one: render a full line
//             ss << StringUtil::Repeat(config_.horizontal_, config_.node_render_width_);
//           } else {
//             // empty spot: render spaces
//             ss << StringUtil::Repeat(" ", config_.node_render_width_);
//           }
//         } else if (render_y >= halfway_point) {
//           if (root.HasNode(x, y + 1)) {
//             // we have a node below this empty spot: render a vertical line
//             ss << StringUtil::Repeat(" ", config_.node_render_width_ / 2);
//             ss << config_.vertical_;
//             ss << StringUtil::Repeat(" ", config_.node_render_width_ / 2);
//           } else {
//             // empty spot: render spaces
//             ss << StringUtil::Repeat(" ", config_.node_render_width_);
//           }
//         } else {
//           // empty spot: render spaces
//           ss << StringUtil::Repeat(" ", config_.node_render_width_);
//         }
//       } else {
//         ss << config_.vertical_;
//         // figure out what to render
//         string render_text;
//         if (render_y == 0) {
//           render_text = node->name_;
//         } else {
//           if (render_y <= extra_info[x].size()) {
//             render_text = extra_info[x][render_y - 1];
//           }
//         }
//         render_text = AdjustTextForRendering(render_text, config_.node_render_width_ - 2);
//         ss << render_text;

//         if (render_y == halfway_point && NodeHasMultipleChildren(root, x, y)) {
//           ss << config_.lmiddle_;
//         } else {
//           ss << config_.vertical_;
//         }
//       }
//     }
//     ss << std::endl;
//   }
// }

// string TreeRenderer::ToString(const LogicalOperator &op) {
//   std::stringstream ss;
//   Render(op, ss);
//   return ss.str();
// }

// void TreeRenderer::Render(const LogicalOperator &op, std::ostream &ss) {
//   auto tree = CreateTree(op);
//   ToStream(*tree, ss);
// }

// void TreeRenderer::ToStream(RenderTree &root, std::ostream &ss) {
//   while (root.width_ * config_.node_render_width_ > config_.maximum_render_width_) {
//     if (config_.node_render_width_ - 2 < config_.minimum_render_width_) {
//       break;
//     }
//     config_.node_render_width_ -= 2;
//   }

//   for (uint64_t y = 0; y < root.height_; y++) {
//     // start by rendering the top layer
//     RenderTopLayer(root, ss, y);
//     // now we render the content of the boxes
//     RenderBoxContent(root, ss, y);
//     // render the bottom layer of each of the boxes
//     RenderBottomLayer(root, ss, y);
//   }
// }

// bool TreeRenderer::CanSplitOnThisChar(char l) {
//   return (l < '0' || (l > '9' && l < 'A') || (l > 'Z' && l < 'a')) && l != '_';
// }

// bool TreeRenderer::IsPadding(char l) { return l == ' ' || l == '\t' || l == '\n' || l == '\r'; }

// string TreeRenderer::RemovePadding(string l) {
//   uint64_t start = 0;
//   uint64_t end = l.size();
//   while (start < l.size() && IsPadding(l[start])) {
//     start++;
//   }
//   while (end > 0 && IsPadding(l[end - 1])) {
//     end--;
//   }
//   return l.substr(start, end - start);
// }

// void TreeRenderer::SplitStringBuffer(const string &source, vector<string> &result) {
//   uint64_t max_line_render_size = config_.node_render_width_ - 2;
//   // utf8 in prompt, get render width
//   uint64_t cpos = 0;
//   uint64_t start_pos = 0;
//   uint64_t render_width = 0;
//   uint64_t last_possible_split = 0;
//   while (cpos < source.size()) {
//     // check if we can split on this character
//     if (CanSplitOnThisChar(source[cpos])) {
//       last_possible_split = cpos;
//     }
//     size_t char_render_width = Utf8Proc::RenderWidth(source.c_str(), source.size(), cpos);
//     uint64_t next_cpos = Utf8Proc::NextGraphemeCluster(source.c_str(), source.size(), cpos);
//     if (render_width + char_render_width > max_line_render_size) {
//       if (last_possible_split <= start_pos + 8) {
//         last_possible_split = cpos;
//       }
//       result.push_back(source.substr(start_pos, last_possible_split - start_pos));
//       start_pos = last_possible_split;
//       cpos = last_possible_split;
//       render_width = 0;
//     }
//     cpos = next_cpos;
//     render_width += char_render_width;
//   }
//   if (source.size() > start_pos) {
//     result.push_back(source.substr(start_pos, source.size() - start_pos));
//   }
// }

// void TreeRenderer::SplitUpExtraInfo(const string &extra_info, vector<string> &result) {
//   if (extra_info.empty()) {
//     return;
//   }
//   auto splits = StringUtil::Split(extra_info, "\n");
//   if (!splits.empty() && splits[0] != "[INFOSEPARATOR]") {
//     result.push_back(ExtraInfoSeparator());
//   }
//   for (auto &split : splits) {
//     if (split == "[INFOSEPARATOR]") {
//       result.push_back(ExtraInfoSeparator());
//       continue;
//     }
//     string str = RemovePadding(split);
//     if (str.empty()) {
//       continue;
//     }
//     SplitStringBuffer(str, result);
//   }
// }

// string TreeRenderer::ExtraInfoSeparator() {
//   return StringUtil::Repeat(string(config_.horizontal_) + " ", (config_.node_render_width_ - 7) / 2);
// }

// unique_ptr<RenderTreeNode> TreeRenderer::CreateRenderNode(string name, string extra_info) {
//   auto result = make_unique<RenderTreeNode>();
//   result->name_ = move(name);
//   result->extra_text_ = move(extra_info);
//   return result;
// }

// class TreeChildrenIterator {
//  public:
//   template <class T>
//   static bool HasChildren(const T &op) {
//     return !op.children_.empty();
//   }
//   template <class T>
//   static void Iterate(const T &op, const std::function<void(const T &child)> &callback) {
//     for (auto &child : op.children_) {
//       callback(*child);
//     }
//   }
// };

// template <class T>
// static void GetTreeWidthHeight(const T &op, uint64_t &width, uint64_t &height) {
//   if (!TreeChildrenIterator::HasChildren(op)) {
//     width = 1;
//     height = 1;
//     return;
//   }
//   width = 0;
//   height = 0;

//   TreeChildrenIterator::Iterate<T>(op, [&](const T &child) {
//     uint64_t child_width;
//     uint64_t child_height;
//     GetTreeWidthHeight<T>(child, child_width, child_height);
//     width += child_width;
//     height = std::max<uint64_t>(height, child_height);
//   });
//   height++;
// }

// template <class T>
// uint64_t TreeRenderer::CreateRenderTreeRecursive(RenderTree &result, const T &op, uint64_t x, uint64_t y) {
//   auto node = TreeRenderer::CreateNode(op);
//   result.SetNode(x, y, move(node));

//   if (!TreeChildrenIterator::HasChildren(op)) {
//     return 1;
//   }
//   uint64_t width = 0;
//   // render the children of this node
//   TreeChildrenIterator::Iterate<T>(
//       op, [&](const T &child) { width += CreateRenderTreeRecursive<T>(result, child, x + width, y + 1); });
//   return width;
// }

// template <class T>
// unique_ptr<RenderTree> TreeRenderer::CreateRenderTree(const T &op) {
//   uint64_t width;
//   uint64_t height;
//   GetTreeWidthHeight<T>(op, width, height);

//   auto result = make_unique<RenderTree>(width, height);

//   // now fill in the tree
//   CreateRenderTreeRecursive<T>(*result, op, 0, 0);
//   return result;
// }

// unique_ptr<RenderTreeNode> TreeRenderer::CreateNode(const LogicalOperator &op) {
//   return CreateRenderNode(op.GetName(), op.ParamsToString());
// }

// unique_ptr<RenderTree> TreeRenderer::CreateTree(const LogicalOperator &op) {
//   return CreateRenderTree<LogicalOperator>(op);
// }

// }  // namespace bustub