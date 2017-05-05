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



void convert_L3_to_L2(L3::Program p) {
  for (auto f : p.functions) {
    forest = create_forest(f);
    merge_trees(forest);
    L2_tiles = generate_L2_tiles();
    write_tiles(tiling(forest, L2_tiles));
  }
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
      i->printTree();
    }
    cout << endl;
  }

  // L3::Simple_Assign_Instruction *newI = new L3::Simple_Assign_Instruction();
  // newI->instr_type = L3::SIMPLE_ASSIGN;
  // newI->assign_left = "rdi";
  // newI->assign_right = "5";
  // print_instruction(newI);

  // newI->genTree();
  // newI->printTree();

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
