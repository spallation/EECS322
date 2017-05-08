#include <algorithm>
#include <set>
#include <iterator>
#include <iostream>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <stdint.h>
#include <unistd.h>
#include <iostream>
#include <fstream> 
#include <parser.h>
#include <vector>
#include <map>
#include <stack>
#include <climits>

using namespace std;

std::vector<std::string> args = {"rdi", "rsi", "rdx", "rcx", "r8", "r9"};
std::vector<std::string> caller_save = {"r10", "r11", "r8", "r9", "rax", "rcx", "rdi", "rdx", "rsi"};
std::vector<std::string> callee_save = {"r12", "r13", "r14", "r15", "rbp", "rbx"};

std::vector<std::string> GP_registers = {"rdi", "rsi", "rdx", "rcx", "r8", "r9", "rax", "r10", "r11", "r12", "r13", "r14", "r15", "rbp", "rbx"};
std::vector<std::string> registers_without_rcx = {"rdi", "rsi", "rdx", "r8", "r9", "rax", "r10", "r11", "r12", "r13", "r14", "r15", "rbp", "rbx"};
std::vector<std::string> H1_sorted_color = {"r10", "r11", "r8", "r9", "rax", "rcx", "rdi", "rdx", "rsi", "r12", "r13", "r14", "r15", "rbp", "rbx"};

std::map<std::string, int> function_invokeNum;
std::map<std::string, int> function_argNum;

bool isStrDigit(std::string s) {
    if (s.empty()) return false;
    if (isdigit(s.at(0))) {
        return true;
    }
    else if (s.size() > 1 && (s.at(0)=='+' || s.at(0)=='-') && isdigit(s.at(1))) {
        return true;
    }
    else return false;
}

bool isop(std::string s) {
  return (s == "+" || s == "-" || s == "*" || s == "&" 
    || s == "<<" || s == ">>" || s == "load" || s == "store");
}

void swapItem(std::string &s1, std::string &s2) {
  std::string tmp;
  tmp = s1;
  s1 = s2;
  s2 = tmp;
}

bool item_type_eq(L3::Item_Type i1, L3::Item_Type i2) {
  if (i1 >= L3::ASGN && i2 >= L3::ASGN) {
    return i1 == i2;
  }
  else if (i1 < L3::ASGN && i2 < L3::ASGN) {
    return i1 >= i2;
  }
  else {
    return false;
  }
}

bool node_eq(L3::Node * n1, L3::Node * n2) {
  if (item_type_eq(n1->ittp, n2->ittp) || item_type_eq(n1->second_ittp, n2->ittp)) {
    return true;
  }
  if (!n1->val.empty()) {
    if (n2->ittp == L3::N1) {
      cout << "n2 is N1" << endl;
      n1->printNode();
      cout << endl;
    }
    return item_type_eq(n1->val_ittp, n2->ittp);
  }
}

void print_instruction(L3::Instruction *i) {
  cout << i->toString() << endl;
}

// void print_program(L3::Program p) {
//   ofstream out;
//   out.open("prog.L1", ios::out);

//   out << "(" << p.entryPointLabel << " ";
//   for (auto f : p.functions) {
//     out << "(" << f->name << " " << f->arguments << " " << f->locals << endl;
//     for (auto i : f->instructions) {
//       // cout << i->instr_string << endl;
//       if (i->opt == "<-" || i->opt == "+=" || i->opt == "-=" || i->opt == "*=" || i->opt == "&=" || i->opt == "<<=" || i->opt == ">>=") {
//         if (!i->mem_opt.empty()) {
//           if (i->is_right_mem) {
//             out << "(" << i->operands[0] << " " << i->opt << "(mem " << i->operands[1] << " " << i->operands[2] << "))" << endl;
//           }
//           else {
//             // cout << "here" << endl;
//             out << "(" << "(mem " << i->operands[0] << " " << i->operands[1] << ")" << i->opt << i->operands[2] << ")" << endl;
//             // cout << "here1s" << endl;
//           }
//         }
//         else if (!i->cmp_opt.empty()) {
//           out << "(" << i->operands[0] << " " << i->opt << " " << i->operands[1] << " " << i->cmp_opt << " " << i->operands[2] << ")" << endl;
//         }
//         else {
//           out << "(" << i->operands[0] << " " << i->opt << " " << i->operands[1] << ")" << endl;
//         }
//       }
//       else if (i->opt == "cjump") {
//         out << "(" << i->opt << " " << i->operands[0] << " " << i->cmp_opt << " " << i->operands[1] << " " << i->operands[2] << " " << i->operands[3] << ")" << endl;
//       }
//       else if (i->opt == "label") {
//         out << i->operands[0] << endl;
//       }
//       else if (i->opt == "goto") {
//         out << "(" << i->opt << " " << i->operands[0] << ")" << endl;
//       }
//       else if (i->opt == "return") {
//         out << "(" << i->opt << ")" << endl;
//       }
//       else if (i->opt == "call") {
//         out << "(" << i->opt << " " << i->operands[0] << " " << i->operands[1] << ")" << endl;
//       }
//       else if (i->opt == "++" || i->opt == "--") {
//         out << "(" << i->operands[0] << i->opt << ")" << endl;
//       }
//       else if (i->opt == "@") {
//         out << "(" << i->operands[0] << " " << i->opt << " " << i->operands[1] << " " << i->operands[2] << " " << i->operands[3] << ")" << endl;
//       }
//       else {
//         cout << "unknown operator" << endl;
//       }
//     }
//     out << ")" << endl;
//   }
//   out << ")" << endl;

//   out.close();
// }


// void convert_L3_to_L2(L3::Program p) {
//   for (auto f : p.functions) {
//     vector<L3::Tree *> forest = create_forest(f);
//     merge_forest(forest);
//     L2_tiles = generate_L2_tiles();
//     write_tiles(tiling(forest, L2_tiles));
//   }
// }

// vector<L3::Tree *> L2_tiles;


vector<L3::Tree *> create_forest(L3::Function * f) {
  vector<L3::Tree *> forest;

  for (auto i : f->instructions) {
    i->genTree();
    forest.push_back(i->getTree());
  }

  return forest;
}

// L3::Node * find_node(L3::Node * var, L3::Node * node) {
//   // node->printNode();
//   // cout << endl;
//   // var->printNode();
//   // cout << endl;

//   if (!node) {
//     // cout << "no" << endl;
//     return NULL;
//   }

//   L3::Node * tmp;
//   for (auto ch : node->children) {
//     // cout << '1' << endl;
//     if (ch) {
//       if (ch->item == var->item) {
//         // cout << '2' << endl;
//         return node;
//       }
//       // cout << '3' << endl;
//       tmp = find_node(var, ch);
//       if (tmp) {
//         // cout << "wtf" << endl;
//         // cout << tmp->item << endl;
//         return tmp;
//       }
//     }
//   }

//   // cout << '4' << endl;
//   return NULL;
// }

// bool replace_node(L3::Node * to_replace, L3::Node * var, L3::Node * alter_var) {
//   if (!to_replace) {
//     // cout << '2' << endl;
//     return false;
//   }

//   for (int i = 0; i < to_replace->children.size(); i++) {
//     if (to_replace->children[i] && to_replace->children[i]->item == var->item) {
//       to_replace->children[i] = alter_var;
//       return true;
//     }
//   }

//   return false;
// }

bool replace_node(L3::Node * node, L3::Node * var, L3::Node * alter_var) {
  if (!node) {
    // cout << '2' << endl;
    return false;
  }

  bool success = false;
  for (int i = 0; i < node->children.size(); i++) {
    if (node->children[i]) {
      if (node->children[i]->item == var->item) {
        // cout << "replace: ";
        // node->children[i]->printNode();
        // cout << " with ";
        // var->printNode();
        // cout << endl;

        node->children[i] = alter_var;
        success = true;
      }
      else if(replace_node(node->children[i], var, alter_var)) {
        success = true;
      }
    }
  }

  return success;
}

bool merge_tree(L3::Tree * t, L3::Tree * merged_tree) {
  if (!t->root || t->root->item != "<-") {
    // cout << '2' << endl;
    return false;
  }
  if (t->root->children[1]->item == "call") {
    return false;
  }

  // cout << '1' << endl;

  // cout << t->root->children[0]->item << endl;
  // cout << t->root->children[1]->item << endl;

  L3::Node * var = t->root->children[0];
  L3::Node * alter_var = t->root->children[1];

  // L3::Node * to_replace = find_node(var, merged_tree->root);

  // if (!to_replace) {
  //   cout << '1' << endl;
  // }

  // to_replace->printNode();
  if ((merged_tree->root->item == "return" 
    || merged_tree->root->item == "call" 
    || merged_tree->root->item == "br") && (alter_var->ittp >= L3::ASGN)) {
      return false;
  }

  if (merged_tree->root && merged_tree->root->item == "<-") {
    return replace_node(merged_tree->root->children[1], var, alter_var);
  }
  return replace_node(merged_tree->root, var, alter_var);
}

vector<L3::Tree *> merge_forest(vector<L3::Tree *> forest) {

  cout << "merging" << endl;

  vector<L3::Tree *> new_forest;
  
  if (forest.empty()) {
    return new_forest;
  }

  for (int i = 1; i < forest.size(); i++) {
    // cout << "merging: ";
    // forest[i-1]->printTree();
    // cout << "with next tree: ";
    // forest[i]->printTree();
    // cout << endl;
    // if (forest[i]) {
    //   forest[i-1]->printTree();
    //   forest[i]->printTree();
    // }
    if (!merge_tree(forest[i-1],forest[i])) {
      // cout << '3' << endl;
      new_forest.push_back(forest[i-1]);
      // cout << "merge failed: ";
      // forest[i-1]->printTree();
      // cout << endl;
    }
    // else {
    //   cout << "merge success: ";
    //   forest[i]->printTree();
    //   cout << endl;
    // }
  }
  new_forest.push_back(forest.back());

  return new_forest;
}

L3::Tree * L2_gen_simple_assign_tree() {
  L3::Tree * t = new L3::Tree();
  t->tile_type = L3::SIMPLE_ASSIGN;
  t->root = new L3::Node("<-", L3::ASGN);
  t->root->children.push_back(new L3::Node(L3::VAR));
  t->root->children.push_back(new L3::Node(L3::S));
  return t;
  // t->printTree();  
}

L3::Tree * L2_gen_left_mem_assign_tree() {
  L3::Tree * t = new L3::Tree();
  t->tile_type = L3::LEFT_MEM_ASSIGN;

  t->root = new L3::Node("store", L3::STORE);

  // t->root = new L3::Node("<-", L3::ASGN);

  // L3::Node * assign_left = new L3::Node("store", L3::STORE);
  
  // L3::Node * store_var_node = new L3::Node("+", L3::PLUS);
  // store_var_node->children.push_back(new L3::Node(L3::VAR));
  // store_var_node->children.push_back(new L3::Node(L3::M));

  // assign_left->children.push_back(store_var_node);

  // t->root->children.push_back(assign_left);
  // t->root->children.push_back(new L3::Node(L3::VAR));

  return t;
  // t->printTree();  
}

L3::Tree * L2_gen_right_mem_assign_tree() {
  L3::Tree * t = new L3::Tree();
  t->tile_type = L3::RIGHT_MEM_ASSIGN;
  t->root = new L3::Node("load", L3::LOAD);

  // t->root = new L3::Node("<-", L3::ASGN);

  // L3::Node *assign_right = new L3::Node("load", L3::LOAD);
  
  // L3::Node * load_var_node = new L3::Node("+", L3::PLUS);
  // load_var_node->children.push_back(new L3::Node(L3::VAR));
  // load_var_node->children.push_back(new L3::Node(L3::M));

  // assign_right->children.push_back(load_var_node);
  
  // t->root->children.push_back(new L3::Node(L3::VAR));
  // t->root->children.push_back(assign_right);

  return t;
  // t->printTree();  
}

L3::Tree * L2_gen_aop_sop_cmp_asgn(L3::Item_Type ittp) {
  L3::Tree * t = new L3::Tree();
  switch (ittp) {
    case L3::PLUS:
      t->tile_type = L3::AOP_SOP_CMP_ASGN_PLUS;
      break;
    case L3::MINUS:
      t->tile_type = L3::AOP_SOP_CMP_ASGN_MINUS;
      break;
    case L3::MUL:
      t->tile_type = L3::AOP_SOP_CMP_ASGN_MUL;
      break;
    case L3::AD:
      t->tile_type = L3::AOP_SOP_CMP_ASGN_AD;
      break;
    case L3::CMP:
      t->tile_type = L3::AOP_SOP_CMP_ASGN_CMP;
      break;
    case L3::SHIFT:
      t->tile_type = L3::AOP_SOP_CMP_ASGN_SHIFT;
      break;
    default:
      cout << "Unknown item type" << endl;
  }

  t->root = new L3::Node("<-", L3::ASGN);
  
  L3::Node * assign_right = new L3::Node(ittp);

  assign_right->children.push_back(new L3::Node(L3::T));
  assign_right->children.push_back(new L3::Node(L3::T));
  
  t->root->children.push_back(new L3::Node( L3::VAR));
  t->root->children.push_back(assign_right);

  return t;
}

L3::Tree * L2_gen_aop_sop_cmp(L3::Item_Type ittp) {
  L3::Tree * t = new L3::Tree();
  switch (ittp) {
    case L3::PLUS:
      t->tile_type = L3::AOP_SOP_CMP_PLUS;
      break;
    case L3::MINUS:
      t->tile_type = L3::AOP_SOP_CMP_MINUS;
      break;
    case L3::MUL:
      t->tile_type = L3::AOP_SOP_CMP_MUL;
      break;
    case L3::AD:
      t->tile_type = L3::AOP_SOP_CMP_AD;
      break;
    case L3::CMP:
      t->tile_type = L3::AOP_SOP_CMP_CMP;
      break;
    case L3::SHIFT:
      t->tile_type = L3::AOP_SOP_CMP_SHIFT;
      break;
    default:
      cout << "Unknown item type" << endl;
  }
  
  
  t->root = new L3::Node(ittp);

  t->root->children.push_back(new L3::Node(L3::T));
  t->root->children.push_back(new L3::Node(L3::T));
  

  // t->root = new L3::Node("<-", L3::ASGN);
  
  // L3::Node * assign_right = new L3::Node(ittp);

  // assign_right->children.push_back(new L3::Node(L3::T));
  // assign_right->children.push_back(new L3::Node(L3::T));
  
  // t->root->children.push_back(new L3::Node( L3::VAR));
  // t->root->children.push_back(assign_right);
  return t;
}

// L3::Tree * L2_gen_sop_tree() {
//   L3::Tree * t = new L3::Tree();
//   t->root = new L3::Node("<-", L3::ASGN);
  
//   L3::Node * assign_right = new L3::Node(op, SHIFT);

//   assign_right->children.push_back(new L3::Node(op_left, L3::T));
//   assign_right->children.push_back(new L3::Node(op_right, L3::T));
  
//   t->root->children.push_back(new L3::Node(L3::VAR));
//   t->root->children.push_back(assign_right);
//   return t;
// }

// L3::Tree * L2_gen_cmp_assign_tree() {
//   L3::Tree * t = new L3::Tree();
//   t->root = new L3::Node("<-", L3::ASGN);

//   L3::Node *assign_right = new L3::Node(L3::CMP);
//   assign_right->children.push_back(new L3::Node(L3::T));
//   assign_right->children.push_back(new L3::Node(L3::T));

//   t->root->children.push_back(new L3::Node(L3::VAR));
//   t->root->children.push_back(assign_right);

//   return t;
//   // t->printTree();  
// }

L3::Tree * L2_gen_left_mem_plus_tree() {
  L3::Tree * t = new L3::Tree();
  t->tile_type = L3::LEFT_MEM_PLUS;
  t->root = new L3::Node("<-", L3::ASGN);

  L3::Node *assign_left = new L3::Node("load", L3::LOAD);
  L3::Node * left_load_var_node = new L3::Node("+", L3::PLUS);
  left_load_var_node->children.push_back(new L3::Node(L3::VAR));
  left_load_var_node->children.push_back(new L3::Node(L3::M));
  assign_left->children.push_back(left_load_var_node);

  L3::Node *assign_right = new L3::Node("+", L3::PLUS);
  L3::Node *assign_right_left = new L3::Node("load", L3::LOAD);
  L3::Node * right_load_var_node = new L3::Node("+", L3::PLUS);
  right_load_var_node->children.push_back(new L3::Node(L3::VAR));
  right_load_var_node->children.push_back(new L3::Node(L3::M));
  assign_right_left->children.push_back(right_load_var_node);
  
  assign_right->children.push_back(assign_right_left);
  assign_right->children.push_back(new L3::Node(L3::T));
  
  t->root->children.push_back(assign_left);
  t->root->children.push_back(assign_right);

  return t;
  // t->printTree();  
}

L3::Tree * L2_gen_left_mem_minus_tree() {
  L3::Tree * t = new L3::Tree();
  t->tile_type = L3::LEFT_MEM_MINUS;
  t->root = new L3::Node("<-", L3::ASGN);

  L3::Node *assign_left = new L3::Node("load", L3::LOAD);
  L3::Node * left_load_var_node = new L3::Node("+", L3::PLUS);
  left_load_var_node->children.push_back(new L3::Node(L3::VAR));
  left_load_var_node->children.push_back(new L3::Node(L3::M));
  assign_left->children.push_back(left_load_var_node);

  L3::Node *assign_right = new L3::Node("-", L3::MINUS);
  L3::Node *assign_right_left = new L3::Node("load", L3::LOAD);
  L3::Node * right_load_var_node = new L3::Node("+", L3::PLUS);
  right_load_var_node->children.push_back(new L3::Node(L3::VAR));
  right_load_var_node->children.push_back(new L3::Node(L3::M));
  assign_right_left->children.push_back(right_load_var_node);
  assign_right->children.push_back(assign_right_left);
  assign_right->children.push_back(new L3::Node(L3::T));
  
  t->root->children.push_back(assign_left);
  t->root->children.push_back(assign_right);

  return t;
  // t->printTree();  
}

L3::Tree * L2_gen_right_mem_plus_tree() {
  L3::Tree * t = new L3::Tree();
  t->tile_type = L3::RIGHT_MEM_PLUS;
  t->root = new L3::Node("<-", L3::ASGN);

  L3::Node *assign_right = new L3::Node("+", L3::PLUS);
  L3::Node *assign_right_right = new L3::Node("load", L3::LOAD);
  L3::Node * load_var_node = new L3::Node("+", L3::PLUS);
  load_var_node->children.push_back(new L3::Node(L3::VAR));
  load_var_node->children.push_back(new L3::Node(L3::M));
  assign_right_right->children.push_back(load_var_node);
  assign_right->children.push_back(new L3::Node(L3::T));
  assign_right->children.push_back(assign_right_right);
  
  t->root->children.push_back(new L3::Node(L3::VAR));
  t->root->children.push_back(assign_right);

  return t;
  // t->printTree();  
}

L3::Tree * L2_gen_right_mem_minus_tree() {
  L3::Tree * t = new L3::Tree();
  t->tile_type = L3::RIGHT_MEM_MINUS;
  t->root = new L3::Node("<-", L3::ASGN);

  L3::Node *assign_right = new L3::Node("-", L3::MINUS);
  L3::Node *assign_right_right = new L3::Node("load", L3::LOAD);
  L3::Node * load_var_node = new L3::Node("+", L3::PLUS);
  load_var_node->children.push_back(new L3::Node(L3::VAR));
  load_var_node->children.push_back(new L3::Node(L3::M));
  assign_right_right->children.push_back(load_var_node);
  assign_right->children.push_back(new L3::Node(L3::T));
  assign_right->children.push_back(assign_right_right);
  
  t->root->children.push_back(new L3::Node(L3::VAR));
  t->root->children.push_back(assign_right);

  return t;
  // t->printTree();
}

L3::Tree * L2_gen_cjump_tree() {
  L3::Tree * t = new L3::Tree();
  t->tile_type = L3::CJUMP;
  t->root = new L3::Node("br", L3::BR);
  t->root->children.push_back(new L3::Node(L3::VAR));
  t->root->children.push_back(new L3::Node(L3::LBL));
  t->root->children.push_back(new L3::Node(L3::LBL));
  return t;
}

L3::Tree * L2_gen_label_tree() {
  L3::Tree * t = new L3::Tree();
  t->tile_type = L3::TILE_LABEL;
  t->root = new L3::Node("label", L3::LBL);
  return t;
}

L3::Tree * L2_gen_goto_tree() {
  L3::Tree * t = new L3::Tree();
  t->tile_type = L3::GOTO;
  t->root = new L3::Node("br", L3::BR);
  t->root->children.push_back(new L3::Node(L3::LBL));
  return t;
}

L3::Tree * L2_gen_return_tree() {
  L3::Tree * t = new L3::Tree();
  t->tile_type = L3::TILE_RETURN;
  t->root = new L3::Node("return", L3::RETURN);
  return t;
}

L3::Tree * L2_gen_return_var_tree() {
  L3::Tree * t = new L3::Tree();
  t->tile_type = L3::TILE_VAR_RETURN;
  t->root = new L3::Node("return", L3::RETURN);
  t->root->children.push_back(new L3::Node(L3::T));
  return t;
}

L3::Tree * L2_gen_call_assign_tree() {
  L3::Tree * t = new L3::Tree();
  t->tile_type = L3::TILE_CALL_ASGN;
  t->root = new L3::Node("<-", L3::ASGN);
  t->root->children.push_back(new L3::Node(L3::VAR));
  t->root->children.push_back(new L3::Node("call", L3::CALL));
  return t;
}

L3::Tree * L2_gen_call_tree() {
  L3::Tree * t = new L3::Tree();
  t->tile_type = L3::TILE_CALL;
  t->root = new L3::Node("call", L3::CALL);
  return t;
}

L3::Tree * L2_gen_inc_tree() {
  L3::Tree * t = new L3::Tree();
  t->tile_type = L3::INC;
  t->root = new L3::Node("<-", L3::ASGN);
  
  L3::Node * assign_right = new L3::Node(L3::PLUS);

  assign_right->children.push_back(new L3::Node(L3::VAR));
  assign_right->children.push_back(new L3::Node(L3::N1));
  
  t->root->children.push_back(new L3::Node(L3::VAR));
  t->root->children.push_back(assign_right);
  return t;
}

L3::Tree * L2_gen_dec_tree() {
  L3::Tree * t = new L3::Tree();
  t->tile_type = L3::DEC;
  t->root = new L3::Node("<-", L3::ASGN);
  
  L3::Node * assign_right = new L3::Node(L3::MINUS);

  assign_right->children.push_back(new L3::Node(L3::VAR));
  assign_right->children.push_back(new L3::Node(L3::N1));
  
  t->root->children.push_back(new L3::Node(L3::VAR));
  t->root->children.push_back(assign_right);
  return t;
}

L3::Tree * L2_gen_lea_tree() {
  L3::Tree * t = new L3::Tree();
  t->tile_type = L3::LEA;
  t->root = new L3::Node("<-", L3::ASGN);

  L3::Node *assign_right = new L3::Node("+", L3::PLUS);
  L3::Node *assign_right_right = new L3::Node("*", L3::MUL);
  
  assign_right_right->children.push_back(new L3::Node(L3::VAR));
  assign_right_right->children.push_back(new L3::Node(L3::E));
  
  assign_right->children.push_back(new L3::Node(L3::VAR));
  assign_right->children.push_back(assign_right_right);
  
  t->root->children.push_back(new L3::Node(L3::VAR));
  t->root->children.push_back(assign_right);

  return t;
  // t->printTree();  
}

L3::Tree * generate_L2_tile_by_type(L3::L2_Tile_Type tt) {
  switch (tt) {
    case L3::LEFT_MEM_PLUS :
      return L2_gen_left_mem_plus_tree();
    case L3::LEFT_MEM_MINUS :
      return L2_gen_right_mem_minus_tree();
    case L3::RIGHT_MEM_PLUS :
      return L2_gen_right_mem_plus_tree();
    case L3::RIGHT_MEM_MINUS :
      return L2_gen_right_mem_minus_tree();
    case L3::LEFT_MEM_ASSIGN :
      return L2_gen_left_mem_assign_tree();
    case L3::RIGHT_MEM_ASSIGN :
      return L2_gen_right_mem_assign_tree();
    case L3::LEA :
      return L2_gen_lea_tree();
    case L3::INC :
      return L2_gen_inc_tree();
    case L3::DEC :
      return L2_gen_dec_tree();
    case L3::AOP_SOP_CMP_ASGN_PLUS :
      return L2_gen_aop_sop_cmp_asgn(L3::PLUS);
    case L3::AOP_SOP_CMP_ASGN_MINUS :
      return L2_gen_aop_sop_cmp_asgn(L3::MINUS);
    case L3::AOP_SOP_CMP_ASGN_MUL :
      return L2_gen_aop_sop_cmp_asgn(L3::MUL);
    case L3::AOP_SOP_CMP_ASGN_AD :
      return L2_gen_aop_sop_cmp_asgn(L3::AD);
    case L3::AOP_SOP_CMP_ASGN_CMP :
      return L2_gen_aop_sop_cmp_asgn(L3::CMP);
    case L3::AOP_SOP_CMP_ASGN_SHIFT :
      return L2_gen_aop_sop_cmp_asgn(L3::SHIFT);
    case L3::AOP_SOP_CMP_PLUS :
      return L2_gen_aop_sop_cmp(L3::PLUS);
    case L3::AOP_SOP_CMP_MINUS :
      return L2_gen_aop_sop_cmp(L3::MINUS);
    case L3::AOP_SOP_CMP_MUL :
      return L2_gen_aop_sop_cmp(L3::MUL);
    case L3::AOP_SOP_CMP_AD :
      return L2_gen_aop_sop_cmp(L3::AD);
    case L3::AOP_SOP_CMP_CMP :
      return L2_gen_aop_sop_cmp(L3::CMP);
    case L3::AOP_SOP_CMP_SHIFT :
      return L2_gen_aop_sop_cmp(L3::SHIFT);
    case L3::SIMPLE_ASSIGN :
      return L2_gen_simple_assign_tree();
    case L3::CJUMP :
      return L2_gen_cjump_tree();
    case L3::TILE_LABEL :
      return L2_gen_label_tree();
    case L3::GOTO :
      return L2_gen_goto_tree();
    case L3::TILE_RETURN :
      return L2_gen_return_tree();
    case L3::TILE_VAR_RETURN:
      return L2_gen_return_var_tree();
    case L3::TILE_CALL:
      return L2_gen_call_tree();
    case L3::TILE_CALL_ASGN:
      return L2_gen_call_assign_tree();
    default:
      cout << "Unknown tile type." << endl;
  }
}

std::vector<L3::Tree *> generate_L2_tiles() {
  std::vector<L3::Tree *> tiles;
  
  tiles.push_back(L2_gen_left_mem_plus_tree());
  tiles.push_back(L2_gen_left_mem_minus_tree());
  tiles.push_back(L2_gen_right_mem_plus_tree());
  tiles.push_back(L2_gen_right_mem_minus_tree());

  tiles.push_back(L2_gen_left_mem_assign_tree());
  tiles.push_back(L2_gen_right_mem_assign_tree());

  tiles.push_back(L2_gen_lea_tree());

  tiles.push_back(L2_gen_inc_tree());
  tiles.push_back(L2_gen_dec_tree());

  tiles.push_back(L2_gen_aop_sop_cmp_asgn(L3::PLUS));
  tiles.push_back(L2_gen_aop_sop_cmp_asgn(L3::MINUS));
  tiles.push_back(L2_gen_aop_sop_cmp_asgn(L3::MUL));
  tiles.push_back(L2_gen_aop_sop_cmp_asgn(L3::AD));
  tiles.push_back(L2_gen_aop_sop_cmp_asgn(L3::CMP));
  tiles.push_back(L2_gen_aop_sop_cmp_asgn(L3::SHIFT));

  tiles.push_back(L2_gen_aop_sop_cmp(L3::PLUS));
  tiles.push_back(L2_gen_aop_sop_cmp(L3::MINUS));
  tiles.push_back(L2_gen_aop_sop_cmp(L3::MUL));
  tiles.push_back(L2_gen_aop_sop_cmp(L3::AD));
  tiles.push_back(L2_gen_aop_sop_cmp(L3::CMP));
  tiles.push_back(L2_gen_aop_sop_cmp(L3::SHIFT));

  tiles.push_back(L2_gen_call_assign_tree());

  tiles.push_back(L2_gen_simple_assign_tree());

  tiles.push_back(L2_gen_cjump_tree());
  tiles.push_back(L2_gen_goto_tree());
  tiles.push_back(L2_gen_return_var_tree());
  tiles.push_back(L2_gen_return_tree());
  tiles.push_back(L2_gen_call_tree());

  tiles.push_back(L2_gen_label_tree());

  return tiles;
}

bool match_call_node(L3::Node * node, L3::Node * pattern, L3::Tree * rest_tree) {
  if (pattern->item != "call") return false;

  pattern->ittp = node->ittp;
  pattern->second_ittp = node->second_ittp;
  pattern->val = node->val;
  pattern->val_ittp = node->val_ittp;

  for (auto ch : node->children) {
    pattern->children.push_back(ch);
  }

  return true;
}

bool match_load_store_node(L3::Node * node, L3::Node * pattern, L3::Tree * rest_tree) {
  // cout << "match_load_store_node: ";
  // node->printNode();
  // cout << endl;
  if (pattern->item != "load" && pattern->item != "store") return false;
  if (pattern->item != node->item) return false;

  pattern->ittp = node->ittp;
  pattern->second_ittp = node->second_ittp;
  pattern->val = node->val;
  pattern->val_ittp = node->val_ittp;

  for (auto ch : node->children) {
    pattern->children.push_back(ch);
  }

  return true;
}

bool match_node(L3::Node * node, L3::Node * pattern, L3::Tree * rest_tree) {
  if (!pattern) {
    return true;
  }

  if (!node || !node_eq(node,pattern)) {//!item_type_eq(node->ittp, pattern->ittp)) {//node->ittp < pattern->ittp) {
    return false;
  }

  pattern->item = node->item;
  // pattern->ittp = node->ittp;
  // pattern->second_ittp = node->second_ittp;
  pattern->val = node->val;
  // pattern->val_ittp = node->val_ittp;

  if (pattern->children.empty()) {
    if (!node->children.empty()) {
      // cout << "rest_tree in match_node: " << endl;
      // node->printNode();
      // cout << endl;
      rest_tree->root = node;
    }
    return true;
  }
  
  if (node->children.size() != pattern->children.size()) {
    return false;
  }

  for (int i = 0; i < node->children.size(); i++) {
    if (!match_node(node->children[i], pattern->children[i], rest_tree)) {
      return false;
    }
  }

  return true;
}

// L3::Node * search_node(L3::Node * n, L3::Item_Type ittp) {
//   if (!n || n->children.empty()) return NULL;
  
//   for (auto ch : n->children) {
//     if (!ch) continue;
//     if (node_eq(ch,)) {//item_type_eq(ch->ittp,ittp)) {
//       // cout << '1' << endl;
//       return n;
//     }
//     else {
//       L3::Node * pos = search_node(ch, ittp);
//       if (pos) return pos;
//     }
//   }

//   return NULL;
// }

// L3::Node * search_tree(L3::Tree * t, L3::Item_Type ittp) {
//   if (item_type_eq(t->root->ittp, ittp)) return t->root;

//   // t->printTree();
//   // cout << ittp << endl;

//   return search_node(t->root, ittp);

// }

// bool match_tree(L3::Tree * t, L3::Tree * tile) {
//   if (!t) return false;

//   L3::Node * start_node = search_tree(t, tile->root->ittp);

//   if (!start_node) {
//     // cout << '1' << endl;
//     return false;
//   }

//   // cout << "in match_tree:" << endl;
//   // cout << "start_node: ";
//   // start_node->printNode();
//   // cout << endl;

//   // cout << "tile: ";
//   // tile->printTree();

//   // start_node is root
//   if (item_type_eq(start_node->ittp, tile->root->ittp) && match_node(start_node, tile->root)) {
//     t->root = NULL;
//     return true;
//   }

//   // start_node is one of root->chilren
//   for (auto ch : start_node->children) {
//     if (ch && item_type_eq(ch->ittp, tile->root->ittp) && match_node(ch, tile->root)) {
//       ch->item = ch->val;
//       ch->ittp = L3::S;
//       ch->children.clear();
//       return true;
//     }
//   }

//   return false;
// }

// L3::Node * get_rest_tree(L3::Node * L3_tree, L3::Node tile) {

// }

void tiling(L3::Node * L3_tree, vector<L3::Tree *> L2_tiles, vector<L3::Tree *> &tiles) {
  if (!L3_tree) return;

  // cout << "in tiling function:" << endl;
  // L3_tree->printTree();
 
  for (auto tile : L2_tiles) {
    // tile->printTree();
    // L3_tree->printTree();
    L3::Tree * rest_tree = new L3::Tree();
    // if (rest_tree->root) {
    //   cout << "????" << endl;
    // }
    L3::Tree * tile_copy = generate_L2_tile_by_type(tile->tile_type);
    // tile_copy->printTree();
    if (L3_tree->item == "call" && match_call_node(L3_tree, tile_copy->root, rest_tree)) {
      tiles.push_back(tile_copy);
      return;
    }
    else if ((L3_tree->item == "load" || L3_tree->item == "store") && match_load_store_node(L3_tree, tile_copy->root, rest_tree)) {
      tiles.push_back(tile_copy);
      return;
    }
    else if (match_node(L3_tree, tile_copy->root, rest_tree)) {
      tiles.push_back(tile_copy);
      // tile_copy->printTree();
       // = get_rest_tree(L3_tree, tile->root);
      // if (rest_tree->root) {
      //   cout << "rest tree" << endl;
      //   rest_tree->printTree();
      //   cout << endl;
      // }
      tiling(rest_tree->root, L2_tiles, tiles);
      return;
    }
  }
}

std::string write_L2_gen_left_mem_plus_minus_tree(L3::Tree * t, L3::L2_Tile_Type tt) {
  std::string x, M, t1, inst;
  
  x = t->root->children[0]->children[0]->children[0]->item;
  M = t->root->children[0]->children[0]->children[1]->item;
  
  if (t->root->children[1]->children[0]->item == "load") {
    t1 = t->root->children[1]->children[1]->item;
  }
  else {
    t1 = t->root->children[1]->children[0]->item;
  }

  inst = "";
  if (tt == L3::LEFT_MEM_PLUS) {
    inst += "((mem " + x + " " + M + ") += " + t1 + ")\n";
  }
  else {
    inst += "((mem " + x + " " + M + ") -= " + t1 + ")\n";
  }

  return inst;
}

std::string write_L2_gen_right_mem_plus_minus_tree(L3::Tree * t, L3::L2_Tile_Type tt) {
  std::string w, x, M, inst;
  
  w = t->root->children[0]->item;
  
  
  if (t->root->children[1]->children[1]->item == "load") {
    x = t->root->children[1]->children[1]->children[0]->children[0]->item;
    M = t->root->children[1]->children[1]->children[0]->children[1]->item;
  }
  else {
    x = t->root->children[1]->children[0]->children[0]->children[0]->item;
    M = t->root->children[1]->children[0]->children[0]->children[1]->item;
  }

  if (isStrDigit(x)) {
    swapItem(x,M);
  }

  inst = "";
  if (tt == L3::LEFT_MEM_PLUS) {
    inst += "(" + w + " += (mem " + x + " " + M + "))\n";
  }
  else {
    inst += "(" + w + " -= (mem " + x + " " + M + "))\n";
  }

  return inst;
}

std::string write_L2_gen_left_mem_assign_tree(L3::Tree * t) {
  std::string var, x, M, inst;

  var = t->root->val;

  if (t->root->children[0]->item != "+") {
    x = t->root->children[0]->item;
    M = "0";
  }
  else {
    x = t->root->children[0]->children[0]->item;
    M = t->root->children[0]->children[1]->item;
  }

  if (isStrDigit(x)) {
    swapItem(x,M);
  }

  inst = "";
  inst += "((mem " + x + " " + M + ") <- " + var + ")\n";

  return inst;
}

std::string write_L2_gen_right_mem_assign_tree(L3::Tree * t) {
  std::string var, x, M, inst;

  var = t->root->val;
  if (t->root->children[0]->item != "+") {
    x = t->root->children[0]->item;
    M = "0";
  }
  else {
    x = t->root->children[0]->children[0]->item;
    M = t->root->children[0]->children[1]->item;
  }

  if (isStrDigit(x)) {
    swapItem(x,M);
  }

  inst = "";
  if (!isStrDigit(M)) {
    inst += "(" + x + " += " + M + ")\n";
    inst += "(" + var + " <- (mem " + x + " 0))\n";
  }
  else {
    inst += "(" + var + " <- (mem " + x + " " + M + "))\n";
  }

  return inst;
}

std::string write_L2_gen_lea_tree(L3::Tree * t) {
  std::string v1, v2, v3, e, inst;

  v1 = t->root->children[0]->item;

  if (t->root->children[1]->children[1]->item == "*") {
    v2 = t->root->children[1]->children[0]->item;
    v3 = t->root->children[1]->children[1]->children[0]->item;
    e = t->root->children[1]->children[1]->children[1]->item;
  }
  else {
    v2 = t->root->children[1]->children[1]->item;
    v3 = t->root->children[1]->children[0]->children[0]->item;
    e = t->root->children[1]->children[0]->children[1]->item;
  }

  if (e != "0" && e != "2" && e != "4" && e != "8") {
    swapItem(v3,e);
  }

  inst = "";
  inst += "(" + v1 + " @ " + v2 + " " + v3 + " " + e + ")\n";

  return inst;
}

std::string write_L2_gen_inc_tree(L3::Tree * t) {
  std::string w, tmp, inst;

  w = t->root->children[0]->item;
  
  tmp = t->root->children[1]->children[0]->item;
  if (tmp == "1") {
    tmp = t->root->children[1]->children[1]->item;
  }

  inst = "";
  if(isStrDigit(tmp)) {
    inst += "(" + w + " <- " + tmp + ")\n";
  }
  inst += "(" + w + "++)\n";

  return inst;
}

std::string write_L2_gen_dec_tree(L3::Tree * t) {
  std::string w, tmp, inst;

  w = t->root->children[0]->item;

  tmp = t->root->children[1]->children[0]->item;
  if (tmp == "1") {
    tmp = t->root->children[1]->children[1]->item;
  }

  inst = "";
  if(isStrDigit(tmp)) {
    inst += "(" + w + " <- " + tmp + ")\n";
  }
  inst += "(" + w + "--)\n";

  return inst;
}

std::string write_L2_gen_aop_sop_cmp_asgn(L3::Tree * t, L3::Item_Type tt) {
  std::string var, op, t1, t2, tmp, inst;

  var = t->root->children[0]->item;
  op = t->root->children[1]->item;
  t1 = t->root->children[1]->children[0]->item;
  t2 = t->root->children[1]->children[1]->item;

  if (isop(t1)) {
    t1 = t->root->children[1]->children[0]->val;
  }
  if (isop(t2)) {
    t2 = t->root->children[1]->children[1]->val;
  }

  inst = "";

  if (tt == L3::CMP) {
    inst += "(" + var + " <- " + t1 + " " + op + " " + t2 + ")\n";
  }
  else {
    if (t1 == var) {
      inst += "(" + var + " " + op + "= " + t2 + ")\n";
    }
    else if (t2 == var) {
      inst += "(" + var + " " + op + "= " + t1 + ")\n";
    }
    else {
        inst += "(" + var + " <- " + t1 + ")\n";
        inst += "(" + var + " " + op + "= " + t2 + ")\n";
    }
  }

  return inst;

}

std::string write_L2_gen_aop_sop_cmp(L3::Tree * t, L3::Item_Type tt) {
  std::string op, t1, t2, val, inst;

  op = t->root->item;
  t1 = t->root->children[0]->item;
  t2 = t->root->children[1]->item;
  val = t->root->val;

  if (isop(t1)) {
    t1 = t->root->children[0]->val;
  }
  if (isop(t2)) {
    t2 = t->root->children[1]->val;
  }

  inst = "";
  if (val != t1) {
    inst += "(" + val + " <- " + t1 + ")\n";
  }
  inst += "(" + val + " " + op + "= " + t2 + ")\n";

  return inst;
}

std::string write_L2_gen_simple_assign_tree(L3::Tree * t) {
  std::string inst = "";
  std::string left = t->root->children[0]->item;
  std::string right = t->root->children[1]->item;
  if (isop(left)) {
    left = t->root->children[0]->val;
  }
  if (isop(right)) {
    right = t->root->children[1]->val;
  }
  if (left != right) {
    inst += "(" + left + " <- " + right + ")\n";
  }
  return inst;
}

std::string write_L2_gen_cjump_tree(L3::Tree * t) {
  std::string var, l1, l2, inst;
  var = t->root->children[0]->item;
  l1 = t->root->children[1]->item;
  l2 = t->root->children[2]->item;

  inst = "";
  inst += "(cjump " + var + " = 1 " + l1 + " " + l2 + ")\n";
  return inst;
}

std::string write_L2_gen_label_tree(L3::Tree * t) {
  return t->root->item + "\n";
}

std::string write_L2_gen_goto_tree(L3::Tree * t) {
  std::string l, inst;
  l = t->root->children[0]->item;
  inst = "";
  inst += "(goto " + l + ")\n";
  return inst;
}

std::string write_L2_gen_return_tree(L3::Tree * t) {
  // cout << "1" << endl;
  return "(return)\n";
}

std::string write_L2_gen_return_var_tree(L3::Tree * t) {
  std::string l, inst;
  l = t->root->children[0]->item;
  inst = "";
  inst += "(rax <- " + l + ")\n";
  inst += "(return)\n";
  return inst;
}

std::string write_L2_gen_call_tree(L3::Tree * t) {
  std::string inst = "";
  std::vector<std::string> args_vec;
  std::string callee = t->root->children[0]->item;
  for (int i = 1; i < t->root->children.size(); i++) {
    args_vec.push_back(t->root->children[i]->item);
  }


  for (int i = 0; i < args_vec.size() && i < 6; i++) {
    inst += "(" + args[i] + " <- " + args_vec[i] + ")\n";
  }
  
  if (function_invokeNum.find(callee) == function_invokeNum.end()) {
    function_invokeNum[callee] = 1;
  }
  else {
    function_invokeNum[callee]++;
  }

  if (callee.at(0) == ':') {
    inst += "((mem rsp -8) <- " + 
      callee + "_ret" + std::to_string(function_invokeNum[callee]) + ")\n";
  }

  if (args_vec.size() > 6) {
    int offset = 16;
    for (int i = 6; i < args_vec.size(); i++) {
      offset += (i-6)*8;
      inst += "((mem rsp -" + std::to_string(offset) + ") <- " + args_vec[i] + ")\n";
    }
  }
  inst += "(call " + callee + " " + std::to_string(args_vec.size()) + ")\n";
  
  if (callee.at(0) == ':') {
    inst += callee + "_ret" + std::to_string(function_invokeNum[callee]) + "\n";
  }

  return inst;
}

std::string write_L2_gen_call_assign_tree(L3::Tree * t) {
  std::string var, inst;

  var = t->root->children[0]->item;

  inst = "";
  // t->root = t->root->children[1];
  // inst += write_L2_gen_call_tree(t);
  inst += "(" + var + " <- " + "rax)\n";
  return inst;
}

std::string write_tiles(std::vector<L3::Tree *> matched_tiles) {
  std::string s = "";
  // cout << matched_tiles.size() << endl;
  for (auto t : matched_tiles) {
    cout << "writting: ";
    t->printTree();
    cout << t->tile_type << endl;
    // cout << t->tile_type << endl;
    switch (t->tile_type) {
      case L3::LEFT_MEM_PLUS :
      case L3::LEFT_MEM_MINUS :
        s += write_L2_gen_left_mem_plus_minus_tree(t,t->tile_type);
        break;
      case L3::RIGHT_MEM_PLUS :
      case L3::RIGHT_MEM_MINUS :
        s +=  write_L2_gen_right_mem_plus_minus_tree(t,t->tile_type);
        break;
      case L3::LEFT_MEM_ASSIGN :
        s +=  write_L2_gen_left_mem_assign_tree(t);
        break;
      case L3::RIGHT_MEM_ASSIGN :
        s +=  write_L2_gen_right_mem_assign_tree(t);
        break;
      case L3::LEA :
        s +=  write_L2_gen_lea_tree(t);
        break;
      case L3::INC :
        s +=  write_L2_gen_inc_tree(t);
        break;
      case L3::DEC :
        s +=  write_L2_gen_dec_tree(t);
        break;
      case L3::AOP_SOP_CMP_ASGN_PLUS :
        s +=  write_L2_gen_aop_sop_cmp_asgn(t,L3::PLUS);
        break;
      case L3::AOP_SOP_CMP_ASGN_MINUS :
        s +=  write_L2_gen_aop_sop_cmp_asgn(t,L3::MINUS);
        break;
      case L3::AOP_SOP_CMP_ASGN_MUL :
        s +=  write_L2_gen_aop_sop_cmp_asgn(t,L3::MUL);
        break;
      case L3::AOP_SOP_CMP_ASGN_AD :
        s +=  write_L2_gen_aop_sop_cmp_asgn(t,L3::AD);
        break;
      case L3::AOP_SOP_CMP_ASGN_CMP :
        s +=  write_L2_gen_aop_sop_cmp_asgn(t,L3::CMP);
        break;
      case L3::AOP_SOP_CMP_ASGN_SHIFT :
        s +=  write_L2_gen_aop_sop_cmp_asgn(t,L3::SHIFT);
        break;
      case L3::AOP_SOP_CMP_PLUS :
        s +=  write_L2_gen_aop_sop_cmp(t,L3::PLUS);
        break;
      case L3::AOP_SOP_CMP_MINUS :
        s +=  write_L2_gen_aop_sop_cmp(t,L3::MINUS);
        break;
      case L3::AOP_SOP_CMP_MUL :
        s +=  write_L2_gen_aop_sop_cmp(t,L3::MUL);
        break;
      case L3::AOP_SOP_CMP_AD :
        s +=  write_L2_gen_aop_sop_cmp(t,L3::AD);
        break;
      // case L3::AOP_SOP_CMP_CMP :
      //   s +=  write_L2_gen_aop_sop_cmp(t,L3::CMP);
      case L3::AOP_SOP_CMP_SHIFT :
        s +=  write_L2_gen_aop_sop_cmp(t,L3::SHIFT);
        break;
      case L3::SIMPLE_ASSIGN :
        s +=  write_L2_gen_simple_assign_tree(t);
        break;
      case L3::CJUMP :
        s +=  write_L2_gen_cjump_tree(t);
        break;
      case L3::TILE_LABEL :
        s +=  write_L2_gen_label_tree(t);
        break;
      case L3::GOTO :
        s +=  write_L2_gen_goto_tree(t);
        break;
      case L3::TILE_RETURN :
        s +=  write_L2_gen_return_tree(t);
        break;
      case L3::TILE_VAR_RETURN:
        s +=  write_L2_gen_return_var_tree(t);
        break;
      case L3::TILE_CALL:
        s +=  write_L2_gen_call_tree(t);
        break;
      case L3::TILE_CALL_ASGN:
        s += write_L2_gen_call_assign_tree(t);
        break;
      default:
        cout << "Unknown tile type." << t->tile_type << endl;
    }
  }
  // cout << s << endl;
  return s;
}

int main(
  int argc, 
  char **argv
  ){
  bool verbose;

  /* Check the input.
   */
  if( argc < 2 ) {
    std::cerr << "Usage: " << argv[ 0 ] << " SOURCE [-v]" << std::endl;
    return 1;
  }
  int32_t opt;
  while ((opt = getopt(argc, argv, "v")) != -1) {
    switch (opt){
      case 'v':
        verbose = true;
        break ;

      default:
        std::cerr << "Usage: " << argv[ 0 ] << "[-v] SOURCE" << std::endl;
        return 1;
    }
  }

  L3::Program p = L3::L3_parse_file(argv[optind]);

  // cout << "printing program:" << endl;
  // for (auto f : p.functions) {
  //   cout << f->name;
  //   int n = 1;
  //   for (auto arg : f->args) {
  //     cout << " " << arg;
  //   }
  //   cout << endl;
  //   for (auto i : f->instructions) {
  //     cout << n++ << ": ";
  //     print_instruction(i);
  //   }
  //   cout << endl;
  // }

  for (auto f : p.functions) {
    cout << f->name;
    int n = 1;
    for (auto arg : f->args) {
      cout << " " << arg;
    }
    cout << endl;
    for (auto i : f->instructions) {
      cout << n++ << ": ";
      // print_instruction(i);
      i->genTree();
      // cout << match_tree(i->t, i->t) << " ";
      i->printTree();
    }
    cout << endl;
  }

  std::string program_str = "";
  program_str += "(:main\n";
  cout << "printing functions:" << endl;
  for (auto f : p.functions) {
    cout << endl;
    cout << "function: ";
    cout << f->name << endl;

    program_str += "(" + f->name + " " + std::to_string(f->args.size()) + " 0\n";
    for (int i = 0; i < f->args.size() && i < 6; i++) {
      program_str += "(" + f->args[i] + " <- " + args[i] + ")\n";
    }
    
    for (int i = 6; i < f->args.size(); i++) {
      int offset = (f->args.size() - i) * 8;
      program_str += "(" + f->args[i] + " <- (stack-arg " + std::to_string(offset) + "))\n";
    }

    vector<L3::Tree *> forest = create_forest(f);
    for (auto t : forest) {
      t->printTree();
    }
    cout << endl;
  // }
  

  // L3::Op_Assign_Instruction *op1 = new L3::Op_Assign_Instruction("v1", "*", "v2", "4");
  // // cout << '1' << endl;
  // L3::Op_Assign_Instruction *op2 = new L3::Op_Assign_Instruction("v2", "+", "v1", "p1");
  // // cout << '2' << endl;
  // L3::Simple_Assign_Instruction *i1 = new L3::Simple_Assign_Instruction("v3","5");
  // // cout << '3' << endl;
  // L3::Simple_Assign_Instruction *i2 = new L3::Simple_Assign_Instruction("v4","v5");

  // L3::Op_Assign_Instruction *op3 = new L3::Op_Assign_Instruction("v7", "+", "v8", "4");
  // // cout << '4' << endl;
  // L3::Load_Assign_Instruction *l1 = new L3::Load_Assign_Instruction("v6","v7");

  // L3::Op_Assign_Instruction *lea_p1 = new L3::Op_Assign_Instruction("v4", "*", "v3", "4");
  // // cout << '1' << endl;
  // L3::Op_Assign_Instruction *lea_p2 = new L3::Op_Assign_Instruction("v1", "+", "v2", "v4");

  // // cout << '5' << endl;
  // vector<L3::Tree *> forest;
  // forest.push_back(op1->genTree());
  // forest.push_back(op2->genTree());
  // forest.push_back(i1->genTree());
  // forest.push_back(i2->genTree());
  // forest.push_back(op3->genTree());
  // forest.push_back(l1->genTree());
  // forest.push_back(lea_p1->genTree());
  // forest.push_back(lea_p2->genTree());
  
  // cout << "printing forest:" << endl;
  // for (auto t : forest) {
  //   if (t)
  //     t->printTree();
  // }
  // cout << endl;

  vector<L3::Tree *> merged_forest = merge_forest(forest);

  // for (auto t : merged_forest) {
  //   if (t->root->item == "<-" && t->root-)
  // }
  // reform_load_assign_instruction();

  cout << "printing new forest:" << endl;
  for (auto t : merged_forest) {
    if (t)
      t->printTree();
  }
  cout << endl;

  // cout << "tiling" << endl;
  // L3::Tree * plus_assign = L2_gen_aop_sop_cmp(L3::PLUS);
  // L3::Tree * lea = L2_gen_lea_tree();
  // lea->printTree();

  // L3::Tree * simple_assign = L2_gen_simple_assign_tree();
  // L3::Tree * mul_assign = L2_gen_aop_sop_cmp(L3::MUL);

  // std::vector<L3::Tree *> tiles;
  std::vector<L3::Tree *> tiles =  generate_L2_tiles();
  // tiles.push_back(lea);
  // tiles.push_back(plus_assign);
  // tiles.push_back(mul_assign);
  // tiles.push_back(simple_assign);
  // merged_forest[0]->printTree();

  std::vector<L3::Tree *> matched_tiles;
  std::vector<L3::Tree *> current_matched_tiles;
  // tiling(merged_forest[0]->root, tiles, matched_tiles);
  for (auto tr : merged_forest) {
    // cout << "tiling: ";
    // tr->printTree();
    tiling(tr->root, tiles, current_matched_tiles);
    
    while (!current_matched_tiles.empty()) {
      matched_tiles.push_back(current_matched_tiles.back());
      current_matched_tiles.pop_back();
    }
  }
  cout << "result:" << endl;
  for (auto mt : matched_tiles) {
    mt->printTree();
  }

  cout << "write_tiles: " << endl;
  program_str += write_tiles(matched_tiles);
  program_str += ")\n";

}
program_str += ")\n";

cout << program_str;

  ofstream out;
  out.open("prog.L2", ios::out);
  out << program_str;
  out.close();
  // cout << "printing L2 tiles:" << endl;
  // std::vector<L3::Tree *> tiles =  generate_L2_tiles();
  // for (auto t : tiles) {
  //   t->printTree();
  // }
  // L2_gen_simple_assign_tree();

  // L3::Simple_Assign_Instruction *i1 = new L3::Simple_Assign_Instruction();
  // i1->instr_type = L3::SIMPLE_ASSIGN;
  // i1->assign_left = "rdi";
  // i1->assign_right = "5";
  // print_instruction(i1);

  // i1->genTree();
  // i1->printTree();

  // L3::Simple_Assign_Instruction *i2 = new L3::Simple_Assign_Instruction();
  // i2->instr_type = L3::SIMPLE_ASSIGN;
  // i2->assign_left = "rdi";
  // i2->assign_right = "10";
  // print_instruction(i2);

  // i2->genTree();
  // i2->printTree();

  // L3::Tree * t1 = new L3::Tree();
  // t1->root = new L3::Node("<-");
  // L3::Node * n1 = new L3::Node("v1");
  // n1->children.push_back(new L3::Node("dumb"));
  // t1->root->children.push_back(n1);

  // L3::Node * ln1 = new L3::Node("load");
  // L3::Node * plus1 = new L3::Node("+");
  // plus1->children.push_back(new L3::Node("v2"));
  // plus1->children.push_back(new L3::Node("v3"));

  // ln1->children.push_back(plus1);
  // t1->root->children.push_back(ln1);

  // L3::Tree * t2 = new L3::Tree();
  // t2->root = new L3::Node("<-");
  // L3::Node * n2 = new L3::Node("v1");
  // t2->root->children.push_back(n2);

  // L3::Node * ln2 = new L3::Node("load");
  // L3::Node * plus2 = new L3::Node("+");
  // plus2->children.push_back(new L3::Node("v2"));
  // plus2->children.push_back(new L3::Node("v3"));

  // ln2->children.push_back(plus2);
  // t2->root->children.push_back(ln2);

  // t1->printTree();
  // t2->printTree();

  // cout << match_tree(t1, t2) << endl;

  // L3::Br2_Instruction *newI = new L3::Br2_Instruction();
  // newI->instr_type = L3::BR2;
  // newI->var = "rdi";
  // newI->label1 = ":true";
  // newI->label2 = ":false";
  // print_instruction(newI);

  // convert_L3_to_L2(p);
  // print_program(p);
  
  return 0;
}
