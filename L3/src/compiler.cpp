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

L3::Node * find_node(L3::Node * var, L3::Node * node) {
  // node->printNode();
  // cout << endl;
  // var->printNode();
  // cout << endl;

  if (!node) {
    // cout << "no" << endl;
    return NULL;
  }

  L3::Node * tmp;
  for (auto ch : node->children) {
    // cout << '1' << endl;
    if (ch) {
      if (ch->item == var->item) {
        // cout << '2' << endl;
        return node;
      }
      // cout << '3' << endl;
      tmp = find_node(var, ch);
      if (tmp) {
        // cout << "wtf" << endl;
        // cout << tmp->item << endl;
        return tmp;
      }
    }
  }

  // cout << '4' << endl;
  return NULL;
}

bool replace_node(L3::Node * to_replace, L3::Node * var, L3::Node * alter_var) {
  if (!to_replace) {
    // cout << '2' << endl;
    return false;
  }

  for (int i = 0; i < to_replace->children.size(); i++) {
    if (to_replace->children[i] && to_replace->children[i]->item == var->item) {
      to_replace->children[i] = alter_var;
      return true;
    }
  }

  return false;
}

bool merge_tree(L3::Tree * t, L3::Tree * merged_tree) {
  if (!t->root || t->root->item != "<-") {
    // cout << '2' << endl;
    return false;
  }

  // cout << '1' << endl;

  // cout << t->root->children[0]->item << endl;
  // cout << t->root->children[1]->item << endl;

  L3::Node * var = t->root->children[0];
  L3::Node * alter_var = t->root->children[1];

  L3::Node * to_replace = find_node(var, merged_tree->root);

  // if (!to_replace) {
  //   cout << '1' << endl;
  // }

  // to_replace->printNode();

  return replace_node(to_replace, var, alter_var);
}

vector<L3::Tree *> merge_forest(vector<L3::Tree *> forest) {

  vector<L3::Tree *> new_forest;
  
  if (forest.empty()) {
    return new_forest;
  }

  for (int i = 1; i < forest.size(); i++) {
    // if (forest[i]) {
    //   forest[i-1]->printTree();
    //   forest[i]->printTree();
    // }
    if (!merge_tree(forest[i-1],forest[i])) {
      // cout << '3' << endl;
      new_forest.push_back(forest[i-1]);
    }
  }
  new_forest.push_back(forest.back());

  return new_forest;
}

L3::Tree * L2_gen_simple_assign_tree() {
  L3::Tree * t = new L3::Tree();
  t->root = new L3::Node("<-", L3::ASGN);
  t->root->children.push_back(new L3::Node(L3::VAR));
  t->root->children.push_back(new L3::Node(L3::S));
  return t;
  // t->printTree();  
}

L3::Tree * L2_gen_left_mem_assign_tree() {
  L3::Tree * t = new L3::Tree();
  t->root = new L3::Node("<-", L3::ASGN);

  L3::Node * assign_left = new L3::Node("store", L3::STORE);
  
  L3::Node * store_var_node = new L3::Node("+", L3::PLUS);
  store_var_node->children.push_back(new L3::Node(L3::VAR));
  store_var_node->children.push_back(new L3::Node(L3::M));

  assign_left->children.push_back(store_var_node);

  t->root->children.push_back(assign_left);
  t->root->children.push_back(new L3::Node(L3::VAR));

  return t;
  // t->printTree();  
}

L3::Tree * L2_gen_right_mem_assign_tree() {
  L3::Tree * t = new L3::Tree();
  t->root = new L3::Node("<-", L3::ASGN);

  L3::Node *assign_right = new L3::Node("load", L3::LOAD);
  
  L3::Node * load_var_node = new L3::Node("+", L3::PLUS);
  load_var_node->children.push_back(new L3::Node(L3::VAR));
  load_var_node->children.push_back(new L3::Node(L3::M));

  assign_right->children.push_back(load_var_node);
  
  t->root->children.push_back(new L3::Node(L3::VAR));
  t->root->children.push_back(assign_right);

  return t;
  // t->printTree();  
}

L3::Tree * L2_gen_aop_sop_cmpassign_tree(L3::Item_Type ittp) {
  L3::Tree * t = new L3::Tree();
  t->root = new L3::Node("<-", L3::ASGN);
  
  L3::Node * assign_right = new L3::Node(ittp);

  assign_right->children.push_back(new L3::Node(L3::T));
  assign_right->children.push_back(new L3::Node(L3::T));
  
  t->root->children.push_back(new L3::Node( L3::VAR));
  t->root->children.push_back(assign_right);
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
  t->root = new L3::Node("br", L3::BR);
  t->root->children.push_back(new L3::Node(L3::VAR));
  t->root->children.push_back(new L3::Node(L3::LBL));
  t->root->children.push_back(new L3::Node(L3::LBL));
  return t;
}

L3::Tree * L2_gen_label_tree() {
  L3::Tree * t = new L3::Tree();
  t->root = new L3::Node("label", L3::LBL);
  return t;
}

L3::Tree * L2_gen_goto_tree() {
  L3::Tree * t = new L3::Tree();
  t->root = new L3::Node("br", L3::BR);
  t->root->children.push_back(new L3::Node(L3::LBL));
  return t;
}

L3::Tree * L2_gen_return_tree() {
  L3::Tree * t = new L3::Tree();
  t->root = new L3::Node("return", L3::RETURN);
  return t;
}

L3::Tree * L2_gen_return_var_tree() {
  L3::Tree * t = new L3::Tree();
  t->root = new L3::Node("return", L3::RETURN);
  t->root->children.push_back(new L3::Node(L3::LBL));
  return t;
}

L3::Tree * L2_gen_call_tree() {
  L3::Tree * t = new L3::Tree();
  t->root = new L3::Node("call", L3::CALL);
  return t;
}

L3::Tree * L2_gen_inc_tree() {
  L3::Tree * t = new L3::Tree();
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

std::vector<L3::Tree *> generate_L2_tiles() {
  std::vector<L3::Tree *> tiles;
  
  tiles.push_back(L2_gen_simple_assign_tree());
  tiles.push_back(L2_gen_left_mem_assign_tree());
  tiles.push_back(L2_gen_right_mem_assign_tree());
  tiles.push_back(L2_gen_simple_assign_tree());
  tiles.push_back(L2_gen_aop_sop_cmpassign_tree(L3::PLUS));
  tiles.push_back(L2_gen_aop_sop_cmpassign_tree(L3::MINUS));
  tiles.push_back(L2_gen_aop_sop_cmpassign_tree(L3::MUL));
  tiles.push_back(L2_gen_aop_sop_cmpassign_tree(L3::AD));
  tiles.push_back(L2_gen_aop_sop_cmpassign_tree(L3::CMP));
  tiles.push_back(L2_gen_aop_sop_cmpassign_tree(L3::SHIFT));
  tiles.push_back(L2_gen_left_mem_plus_tree());
  tiles.push_back(L2_gen_left_mem_minus_tree());
  tiles.push_back(L2_gen_right_mem_plus_tree());
  tiles.push_back(L2_gen_right_mem_minus_tree());
  tiles.push_back(L2_gen_cjump_tree());
  tiles.push_back(L2_gen_label_tree());
  tiles.push_back(L2_gen_goto_tree());
  tiles.push_back(L2_gen_return_tree());
  tiles.push_back(L2_gen_call_tree());
  tiles.push_back(L2_gen_inc_tree());
  tiles.push_back(L2_gen_dec_tree());
  tiles.push_back(L2_gen_lea_tree());

  return tiles;
} 

bool match_node(L3::Node * node, L3::Node * pattern) {
  if (!pattern) {
    return true;
  }

  if (!node || node->ittp < pattern->ittp) {
    return false;
  }

  if (pattern->children.empty()) {
    return true;
  }
  
  if (node->children.size() != pattern->children.size()) {
    return false;
  }

  for (int i = 0; i < node->children.size(); i++) {
    if (!match_node(node->children[i], pattern->children[i])) {
      return false;
    }
  }

  return true;
}

bool match_tree(L3::Tree * t1, L3::Tree * t2) {
  return match_node(t1->root, t2->root);
}

vector<L3::Tree *> tiling(L3::Tree * L3_tree, vector<L3::Tree *> L2_tiles) {
  // L3_tree->printTree();
 
  vector<L3::Tree *> tiles;
  for (auto tile : L2_tiles) {
    if (match_tree(L3_tree,tile)) {
      tiles.push_back(tile);
    }
  }
  return tiles;
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

  // cout << "printing forest:" << endl;
  // for (auto f : p.functions) {
  //   cout << f->name << endl;
  //   vector<L3::Tree *> forest = create_forest(f);
  //   for (auto t : forest) {
  //     t->printTree();
  //   }
  //   cout << endl;
  // }

  L3::Op_Assign_Instruction *op1 = new L3::Op_Assign_Instruction("v1", "*", "v2", "4");
  // cout << '1' << endl;
  L3::Op_Assign_Instruction *op2 = new L3::Op_Assign_Instruction("v2", "+", "v1", "p1");
  // cout << '2' << endl;
  L3::Simple_Assign_Instruction *i1 = new L3::Simple_Assign_Instruction("v3","5");
  // cout << '3' << endl;
  L3::Simple_Assign_Instruction *i2 = new L3::Simple_Assign_Instruction("v4","v5");
  // cout << '4' << endl;
  L3::Load_Assign_Instruction *l1 = new L3::Load_Assign_Instruction("v6","v7");

  L3::Op_Assign_Instruction *lea_p1 = new L3::Op_Assign_Instruction("v4", "*", "v3", "4");
  // cout << '1' << endl;
  L3::Op_Assign_Instruction *lea_p2 = new L3::Op_Assign_Instruction("v1", "+", "v2", "v4");

  // cout << '5' << endl;
  vector<L3::Tree *> forest;
  // op1->genTree();
  // op1->printTree();
  forest.push_back(op1->genTree());
  // cout << '1' << endl;
  // op2->genTree();
  // op2->printTree();
  forest.push_back(op2->genTree());
  forest.push_back(i1->genTree());
  forest.push_back(i2->genTree());
  forest.push_back(l1->genTree());
  forest.push_back(lea_p1->genTree());
  forest.push_back(lea_p2->genTree());
  

  cout << "printing forest:" << endl;
  for (auto t : forest) {
    if (t)
      t->printTree();
  }
  cout << endl;

  cout << "printing new forest:" << endl;
  vector<L3::Tree *> merged_forest = merge_forest(forest);
  for (auto t : merged_forest) {
    if (t)
      t->printTree();
  }
  cout << endl;

  cout << "tiling" << endl;
  L3::Tree * lea = L2_gen_lea_tree();
  // lea->printTree();

  std::vector<L3::Tree *> tiles;// =  generate_L2_tiles();
  tiles.push_back(lea);
  std::vector<L3::Tree *> matched_tiles = tiling(merged_forest[4], tiles);
  for (auto mt : matched_tiles) {
    mt->printTree();
  }


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
