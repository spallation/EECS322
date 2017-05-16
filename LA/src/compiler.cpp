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

std::map<std::string, int> function_invokeNum;

void print_instruction(LA::Instruction *i) {
  cout << i->toString() << endl;
}

std::string remove_percent(std::string s) {
    if (!s.empty() && s.at(0) == '%') {
        s = s.substr(1);
    }
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

  LA::Program p = LA::LA_parse_file(argv[optind]);

  cout << "printing program:" << endl;
  for (auto f : p.functions) {
    cout << f->return_type << " ";
    cout << f->name;
    
    for (auto arg : f->args) {
      cout << " " << arg;
    }
    int n = 1;
    cout << endl;
    for (auto i : f->instructions) {
      cout << n++ << ": ";
      print_instruction(i);
    }
    cout << endl;
  }

  // std::string program_str = "";
  // cout << "After translating:" << endl;
  // for (auto f : p.functions) {
  //   f->declare_var();
    
  //   cout << f->return_type << " ";
  //   cout << f->name;
  //   program_str += "define " + f->name + "(";
    
  //   for (int i = 0; i < f->args.size(); i++) {
  //     cout << " " << remove_percent(f->args[i]);
  //     if (i == 1) {
  //       program_str += remove_percent(f->args[i]);
  //     }
  //     else if (i % 2 == 1) {
  //       program_str += ", " + remove_percent(f->args[i]);
  //     }
  //   }
  //   program_str += ") {\n";
    
  //   cout << endl;
  //   for (auto bb : f->bbs) {
  //     cout << bf->label << endl;
  //     program_str += bf->label + "\n";
  //     int n = 1;
  //     for (auto i : bf->instructions) {
  //       cout << n++ << ": ";
  //       cout << i->translate_to_IR(bb);
  //       program_str += i->translate_to_IR(bb);
  //     }
  //     cout << bf->terminator->translate_to_IR(bb);
  //     program_str += bf->terminator->translate_to_IR(bb);
  //   }
  //   program_str += "}\n";
  //   cout << endl;
  // }

  // ofstream out;
  // out.open("prog.L3", ios::out);
  // out << program_str;
  // out.close();

  return 0;
}
