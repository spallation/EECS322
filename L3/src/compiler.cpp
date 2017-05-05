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
  // switch(i->instr_type) {
  //   case L3::SIMPLE_ASSIGN :
  //     cout << ((Simple_Assign_Instruction *)i)->toString() << endl;
  //     break;
  //   case L3::OP_ASSIGN :
  //     cout << ((Op_Assign_Instruction *)i)->toString() << endl;
  //     break;
  //   case L3::CMP_ASSIGN :
  //     cout << ((Cmp_Assign_Instruction *)i)->toString() << endl;
  //     break;
  //   case L3::LOAD_ASSIGN :
  //     cout << ((Load_Assign_Instruction *)i)->toString() << endl;
  //     break;
  //   case L3::CALL_ASSIGN :
  //     cout << ((Call_Assign_Instruction *)i)->toString() << endl;
  //     break;
  //   case L3::LABLE :
  //     cout << ((Lable_Instruction *)i)->toString() << endl;
  //     break;
  //   case L3::BR1 :
  //     cout << ((Br1_Instruction *)i)->toString() << endl;
  //     break;
  //   case L3::BR2 :
  //     cout << ((Br2_Instruction *)i)->toString() << endl;
  //     break;
  //   case L3::RETURN :
  //     cout << ((Return_Instruction *)i)->toString() << endl;
  //     break;
  //   case L3::VAR_RETURN :
  //     cout << ((Var_Return_Instruction *)i)->toString() << endl;
  //     break;
  //   case L3::CALL :
  //     cout << ((Call_Instruction *)i)->toString() << endl;
  //     break;
  //   default :
  //     cerr << "Unknown Instruction Type." << endl;
  //     break;
  // }
}

// void print_function(L3::Function *f) {
//   cout << f->name << " has " << f->arguments << " parameters and " << f->locals << " local variables" << "; instructions:" << endl;
//   for (auto i : f->instructions) {
//   // for (auto i : new_instructions) {
//     print_instruction(i);
//   }
// }

// void print_GEN_KILL(std::vector<std::set<std::string>> & GEN, std::vector<std::set<std::string>> & KILL) {
//   cout << "GENi:" << endl;
//   for (auto i : GEN[0]) {
//     cout << i;
//   }
//   cout << endl;
//   cout << "KILLi:" << endl;
//   for (auto i : KILL[0]) {
//     cout << i << endl;
//   }
//   cout << endl;
// }

// void print_result(std::vector<std::vector<std::string> > IN, std::vector<std::vector<std::string> > OUT) {
//   cout << "(" << endl;
//   cout << "(in" << endl;
//   for (int k = 0; k < IN.size(); k++) {
//     cout << "(";
//     if (IN[k].size() == 0) {
//       cout << ")" << endl;
//     }
//     else {
//       int i = 0;
//       for (; i < IN[k].size()-1; i++) {
//         cout << IN[k][i] << " ";
//       }
//       cout << IN[k][i] << ")" << endl;
//     }
//   }
//   cout << ")" << endl;
//   cout << endl;
//   cout << "(out" << endl;
//   for (int k = 0; k < OUT.size(); k++) {
//     cout << "(";
//     if (OUT[k].size() == 0) {
//       cout << ")" << endl;
//     }
//     else {
//       int i = 0;
//       for (; i < OUT[k].size()-1; i++) {
//         cout << OUT[k][i] << " ";
//       }
//       cout << OUT[k][i] << ")" << endl;
//     }
//   }
//   cout << ")" << endl;
//   cout << endl;
//   cout << ")" << endl;
// }

// bool myisdigit(std::string s) {
//   if (s.at(0) == '+' || s.at(0) == '-') {
//     return isdigit(s.at(1));
//   }
//   return isdigit(s.at(0));
// }

// bool islabel(std::string s) {
//   if (s.at(0) == ':') {
//     return true;
//   }
//   return false;
// }

// bool isGPRegister(std::string s) {
//   std::set<std::string> register_set (GP_registers.cbegin(),GP_registers.cend());
//   if (register_set.find(s) != register_set.end()) {
//     return true;
//   }
//   return false;
// }

// // bool isvar(std::string s) {
// //   return (!myisdigit(s) && !isGPRegister(s));
// // }

// bool isvar(std::string s) {
//   if (!myisdigit(s) && !islabel(s)) {
//     return true;
//   }
//   return false;
// }

// void my_push_back(std::string s, std::vector<std::string> & vec) {
//   if (isvar(s) && s != "rsp") {
//     vec.push_back(s);
//   }
// }

// void my_insert(std::string s, std::set<std::string> & vec) {
//   if (isvar(s) && s != "rsp") {
//     vec.insert(s);
//   }
// }

// int find_instruction(string _label, L3::Function *f) {
//   int j = 0;
//   for (auto i : f->instructions) {
//     if (i->opt == "label" && i->operands[0] == _label) {
//       return j;
//     }
//     ++j;
//   }
//   return -1;
// }

// std::vector<int> get_successor(int i, L3::Function *f) {
//   std::vector<int> out;
//   if (f->instructions[i]->opt == "cjump") {
//     int s1 = find_instruction(f->instructions[i]->operands[2], f)+1;
//     int s2 = find_instruction(f->instructions[i]->operands[3], f)+1;
//     if (s1 < f->instructions.size()) {
//       out.push_back(s1);
//     }
//     if (s2 < f->instructions.size()) {
//       out.push_back(s2);
//     }
//   }
//   else if (f->instructions[i]->opt == "goto") {
//     int s = find_instruction(f->instructions[i]->operands[0], f) + 1;
//     if (s < f->instructions.size()) {
//       out.push_back(s);
//     }
//   } 
//   else {
//     if (i+1 < f->instructions.size()) {
//       out.push_back(i+1);
//     }
//   }
//   return out;
// }

// void get_GEN_KILL(L3::Instruction *i) {

//   std::string opt = i->opt;
//   // cout << j << " " << i->opt << endl;
  
//   // get GEN and KILL
//   if (opt == "<-") {
//     if (!i->cmp_opt.empty()) {
//       // assign cmp
//       std::string cmp_opt = i->cmp_opt;
//       std::string w = i->operands[0];
//       std::string cmp_left = i->operands[1];
//       std::string cmp_right = i->operands[2];

//       my_insert(w,i->KILL);
//       my_insert(cmp_left,i->GEN);
//       my_insert(cmp_right,i->GEN);
//     }
//     else if (!i->mem_opt.empty()) {
//       // assign mem
//       // "mem" at left 
//       std::string x = "";
//       if (!i->is_right_mem) {
//         // (mem x M) <- s
//         std::string s = i->operands[2]; // s
//         my_insert(s,i->GEN);
//         x = i->operands[0]; // x
//       }
//     // "mem" at right
//       else {
//         // w <- (mem x M)
//         std::string w = i->operands[0]; // w
//         my_insert(w,i->KILL);
//         x = i->operands[1]; // x
//       }
//       my_insert(x,i->GEN);
//     }
//     else {
//         // simple assign w <- s
//       std::string s = i->operands.back();
//       std::string w = i->operands.front();
    
//       my_insert(w,i->KILL);
//       my_insert(s,i->GEN);
//     }
//   }
//   else if (opt == "call") {                              // call _label
//     std::string call_function_name = i->operands.front();
//     // cout << "---------------------------------------" << endl;
//     // cout << i->instr_string << ": ";
//     // cout << i->operands.back() << endl;
//     int N = std::stoi(i->operands.back());

//     // cout << call_function_name << endl;
//     // cout << N << endl;

//     if (call_function_name == "print" || call_function_name == "allocate" || call_function_name == "array-error") {
//       for (int k = 0; k < N; ++k) {
//         i->GEN.insert(args[k]);
//       }
//       for (int k = 0; k < 9; ++k) {
//         i->KILL.insert(caller_save[k]);
//       }
//     }
//     else {
//       // cout << "?1" << endl;
//       my_insert(call_function_name, i->GEN);
//       // cout << "?2" << endl;
//       for (int k = 0; k < (N>6?6:N); ++k) {
//         i->GEN.insert(args[k]);
//       }
//       // cout << "?3" << endl;
//       for (int k = 0; k < 9; ++k) {
//         i->KILL.insert(caller_save[k]);
//       }
//     }

//   }
//   else if (opt == "return") {                            // retq
//     i->GEN.insert("rax");
//     for (int k = 0; k < 6; ++k) {
//       i->GEN.insert(callee_save[k]);
//     }
//   }
//   else if (opt == "cjump") {                 // cjump
//     std::string cmp_opt = i->cmp_opt;
//     std::string cmp_left = i->operands[0];
//     std::string cmp_right = i->operands[1];
//     my_insert(cmp_left,i->GEN);
//     my_insert(cmp_right,i->GEN);
//   }
//   else if (opt == "++" || opt == "--") {                                // inc %rdi
//     my_insert(i->operands.front(), i->GEN);
//     my_insert(i->operands.front(), i->KILL);  
//   } 
//   else if (opt == "@") {
//     my_insert(i->operands[0], i->KILL);
//     my_insert(i->operands[1], i->GEN);
//     my_insert(i->operands[2], i->GEN);
//   }
  
//   // aop instruction
//   // addq(subq, imulq, andq) %rax, %rdi(number) | addq(subq) M(%rsp), %rdi | addq(subq) %rdi, M(%rsp)
//   else if (opt == "+=" || opt == "-=" || opt == "*=" || opt == "&=") {     
//   // "mem" at left
//     if (!i->mem_opt.empty() && !i->is_right_mem) {
//       my_insert(i->operands[0], i->GEN);
//       my_insert(i->operands[2], i->GEN);
//     }
//     // "mem" at right
//     else if (!i->mem_opt.empty() && i->is_right_mem) {
//       my_insert(i->operands[0], i->KILL);
//       my_insert(i->operands[0], i->GEN);
//       my_insert(i->operands[2], i->GEN);
//     }
//     // no "mem"
//     else {
//       my_insert(i->operands[0], i->KILL);
//       my_insert(i->operands[0], i->GEN);
//       my_insert(i->operands[1], i->GEN);
//     }
//   }
//   // sop instrction
//   else if (opt == "<<=" || opt == ">>=") {
//     my_insert(i->operands[0], i->KILL);
//     my_insert(i->operands[0], i->GEN);
//     my_insert(i->operands[1], i->GEN);
//   }
//   else {
//     // cout << "label" << endl;
//     return;
//   }
// }

// void liveness(L3::Function *f, 
//   std::vector<std::vector<std::string>> & IN,
//   std::vector<std::vector<std::string>> & OUT) {

//   // cout << "wtf" << endl;

//   int j = 0; // instruction index
//   for (auto i : f->instructions) {
//     get_GEN_KILL(i);
//   }
  
//   // print_GEN_KILL(GEN,KILL);

//   // initialize IN and OUT
//   for (auto i : f->instructions) {
//     OUT.push_back(std::vector<std::string>());
//     IN.push_back(std::vector<std::string>());
//   }

//   // get IN and OUT
//   bool changes;
//   do {
//     changes = false;
//     for (int i = 0; i < f->instructions.size(); ++i) {
//       std::set<std::string> prev_in (IN[i].cbegin(), IN[i].cend());         // in before the algorithm
//       std::set<std::string> prev_out (OUT[i].cbegin(), OUT[i].cend());      // out before the algorithm
//       std::set<std::string> out_set (OUT[i].cbegin(), OUT[i].cend());       // out after the algorithm
//       std::set<std::string> gen_set (f->instructions[i]->GEN.cbegin(), f->instructions[i]->GEN.cend());     // in after the algorithm

//       // IN[i] = GEN[i] + (OUT[i] - KILL[i])
//       for (auto s : f->instructions[i]->KILL) {
//         out_set.erase(s);
//       }
//       gen_set.insert(out_set.cbegin(), out_set.cend());
//       IN[i] = std::vector<std::string> (gen_set.cbegin(), gen_set.cend());

//       // OUT[i] = U_s_a_successor_of_i IN[s]
//       std::vector<int> successor = get_successor(i, f);
//       out_set.clear();
//       for (int s : successor) {
//         out_set.insert(IN[s].cbegin(), IN[s].cend());
//       }
//       OUT[i] = std::vector<std::string> (out_set.cbegin(), out_set.cend());

//       if (prev_out != out_set || prev_in != gen_set) {
//         changes = true;
//       }
//     }
//   } while (changes);

//   // print_result(IN,OUT);
// }


// struct Node
// { 
//   std::string var;
//   int num_edge;
//   std::string color;
//   std::map<std::string, struct Node*> next_map;
// };

// struct Graph
// { 
//   // int V;
//   int num_var;
//   int num_no_color_var;
//   std::map<std::string, struct Node*> node_map;
//   // std::vector<pair<std::string, struct Node*> > sorted_array;
//   Graph() : num_var(0), num_no_color_var(0), node_map() {}
// };

// struct Node* newNode(std::string var) {
//   // struct Node* newNode = (struct Node*) malloc(sizeof(struct Node));
//   Node *newNode = new Node;
//   newNode->var = var;
//   newNode->num_edge = 0;
//   if (isGPRegister(var)) {
//     newNode->color = var;
//   } else {
//     newNode->color = "";
//   }
//   return newNode;
// }

// void addEdge(struct Node* n1, struct Node* n2) {
//   if (n1->var == n2->var) return;
//   if (n1->next_map.empty() || n1->next_map.find(n2->var) == n1->next_map.end()) {
//     n1->next_map[n2->var] = n2;
//     n1->num_edge++;
//   }
// }

// void addNode(Graph *g, Node *n) {
//   g->node_map[n->var] = n;
//   if (!isGPRegister(n->var)) {
//     g->num_var++;
//   }
// }

// bool var_exists(Graph *g, string var) {
//   return g->node_map.find(var) != g->node_map.end();
// }

// void print_graph(Graph* g) {
//   cout << "graph has " << g->num_var << " vars" << endl;
//   cout << g->num_no_color_var << " vars have no color." << endl;
//   for (auto const &p : g->node_map) {
//     cout << p.first;
//     // cout << p.second->var << endl;
//     if (p.second == NULL) {
//       continue;
//     }
//     cout << "(" << p.second->num_edge << "," << p.second->color << ")" << endl;
//     for (auto const &p2 : p.second->next_map) {
//       if (p2.second == NULL) {
//         continue;
//       }
//       cout << p2.second->var << " ";
//     }
//     cout << endl;
//     cout << endl;
//   }
// }

// void print_node(Node* curNode) {
//   cout << curNode->var << "(" << curNode->num_edge << "," << curNode->color << ")" << endl;
//   for (auto const &p : curNode->next_map) {
//     cout << p.first;
//     if (p.second == NULL) {
//       cout << " -" << endl;
//     }
//   }
//   cout << endl;
// }

// void connect_two_var(Graph * g, std::string v1, std::string v2) {
//   if (!var_exists(g, v1)) {
//     addNode(g, newNode(v1));
//   }
//   if (!var_exists(g, v2)) {
//     addNode(g, newNode(v2));
//   }
//   addEdge(g->node_map[v1],g->node_map[v2]);
// }

// void connect_var_in_vec(Graph * g, std::vector<std::string> vec) {
//   for (int i = 0; i < vec.size(); i++) {
//     for (int j = 0; j < vec.size(); j++) {
//       if (i != j) {
//         connect_two_var(g, vec[i], vec[j]);
//       }
//     }
//   }
// }

// void create_node_for_var(Graph *g, L3::Function *f) { // std::vector<std::set<std::string>> & GEN, std::vector<std::set<std::string>> & KILL) {
//   for (auto i : f->instructions) {
//     for (auto var : i->GEN) {
//       if (!var_exists(g, var)) {
//         addNode(g, newNode(var));
//       }
//     }
//     for (auto var : i->KILL) {
//       if (!var_exists(g, var)) {
//         addNode(g, newNode(var));
//       }
//     }
//   }
// }

// bool sort_by(pair<std::string, struct Node*> const &a, pair<std::string, struct Node*> const &b) {
//   return a.second->num_edge < b.second->num_edge;
// }

// std::string find_var_with_least_edge(Graph *g) {
//   int min_edge = INT_MAX;
//   std::string least_edge_node_name;
//   for (auto p : g->node_map) {
//     if (isGPRegister(p.first)) {
//       continue;
//     }
//     if (p.second->num_edge < min_edge) {
//       min_edge = p.second->num_edge;
//       least_edge_node_name = p.first;
//     }
//   }
//   return least_edge_node_name;
// }

// void delete_edge(Graph* g, std::string node_name) {
//   for (auto p1 : g->node_map) {
//     Node* curNode = p1.second;
//     if (p1.first == node_name) {
//       for (auto p2 : curNode->next_map) {
//         curNode->next_map[p2.first] = NULL;
//         curNode->num_edge--;
//       }
//     }
//     else {
//       if (curNode->next_map.find(p1.first) != curNode->next_map.end()) {
//         curNode->next_map[p1.first] = NULL;
//         curNode->num_edge--;
//       }
//     }
//   }
// }

// void recover_edges(Graph *g, Node* curNode) {
//   for (auto p : curNode->next_map) {
//     if (var_exists(g, p.first)) {
//       if (curNode->next_map[p.first] == NULL) {
//         curNode->next_map[p.first] = g->node_map[p.first];
//         curNode->num_edge++;
//       }
//       if (g->node_map[p.first]->next_map[curNode->var] == NULL) {
//         g->node_map[p.first]->next_map[curNode->var] = curNode;
//         g->node_map[p.first]->num_edge++;
//       }
//     }
//   }
// }

// // void rebuild_edges(Graph* g) {
// //   for (auto p1 : g->node_map) {
// //     Node* curNode = p1.second;
// //     for (auto p2 : curNode->next_map) {
// //       if (g->node_map.find(p2.first) != g->node_map.end()) {
// //         curNode->next_map[p2.first] = g->node_map[p2.first];
// //         curNode->num_edge++;
// //       }
// //     }
// //   }
// // }

// bool node_color_conflict(Graph *g, Node *curNode) {
//   for (auto p : curNode->next_map) {
//     if (p.second != NULL && p.second->color == curNode->color) {
//       // cout << "conflict" << endl;
//       return true;
//     }
//   }
//   return false;
// }

// bool assign_color(Graph *g, Node *curNode, vector<string> color_list) {
//   // cout << "coloring " << curNode->var << " ";
//   if (color_list.empty()) {
//     // cout << "not enough color" << endl;
//     return false;
//   }
  
//   // cout << curNode->color << endl;

//   while (!color_list.empty()) {
//     curNode->color = color_list.back();
//     color_list.pop_back();
//     if (!node_color_conflict(g, curNode)) {
//       // cout << g->num_no_color_var << endl;
//       g->num_no_color_var--;
//       // cout << g->num_no_color_var << endl;
//       return true;
//     }
//   }
  
//   curNode->color = "";
//   return false;
// }

// struct Graph * gen_inference_graph(L3::Function *f, std::vector<std::vector<std::string>> & IN, std::vector<std::vector<std::string>> & OUT) {
//   // Graph * g = (struct Graph*) malloc(sizeof(struct Graph));
//   Graph *g = new Graph;

//   create_node_for_var(g, f);

//   // connect variables in IN and OUT
//   for (auto i : IN) {
//     connect_var_in_vec(g,i);
//   }
//   for (auto i : OUT) {
//     connect_var_in_vec(g,i);
//   }

//   // connect registers
//   // connect_var_in_vec(g,temp);
//   connect_var_in_vec(g,GP_registers);

//   // connect KILL and OUT
//   for (int i = 0; i < f->instructions.size(); i++)  {
//     if (f->instructions[i]->opt == "<-" 
//       && f->instructions[i]->mem_opt.empty()
//       && f->instructions[i]->cmp_opt.empty()) {
//         continue;
//     }
//     for (auto kvar : f->instructions[i]->KILL) {
//       for (auto ovar : OUT[i]) {
//         connect_two_var(g,kvar,ovar);
//       }
//     }
//   }

//   // preserve constrains in L1
//   for (int i = 0; i < f->instructions.size(); i++) {
//     L3::Instruction *instr = f->instructions[i];
//     if (instr->opt == "<<=" || instr->opt == ">>=") {
//       std::string sop_right = instr->operands[1];
//       if (!myisdigit(sop_right)) {
//         for (auto regi : registers_without_rcx) {
//           connect_two_var(g, sop_right, regi);
//         }
//       }
//     }
//   }

//   g->num_no_color_var = g->num_var;
//   // cout << g->num_var << endl;
//   // cout << g->num_no_color_var << endl;
//   return g;
// }

// struct Graph * gen_coloring_graph(Graph * g, std::stack<string> &spill_stack) {
//   // std::vector<string> vec;
//   // g->sorted_array = std::vector<pair<std::string, struct Node*> > (g->node_map.begin(), g->node_map.end());
  
//   // print_graph(g);
//   // cout << "graph has " << g->num_var << " vars" << endl;
//   // cout << g->num_no_color_var << " vars have no color." << endl;

//   std::stack<Node*> var_stack;
//   // vector<string> color_list (H1_sorted_color.begin(), H1_sorted_color.end());
//   vector<string> color_list (H1_sorted_color.begin(), H1_sorted_color.end());
//   // cout << color_list[0] << endl;

//   // remove node with the least edges and add it to stack
//   // until graph is empty
//   while (g->num_var > 0) {
//   // while(!g->node_map.empty()) {
//     std::string start_node_name = find_var_with_least_edge(g);
//     Node* curNode = g->node_map[start_node_name];
    
//     delete_edge(g, start_node_name);
//     g->node_map.erase(start_node_name);
    
//     var_stack.push(curNode);
//     g->num_var--;
//   }

//   // rebuild graph and assign color
//   while (!var_stack.empty()) {
//     Node* curNode = var_stack.top();
//     var_stack.pop();

//     addNode(g, curNode);
//     recover_edges(g, curNode);

//     if (!assign_color(g, curNode, color_list)) {
//       spill_stack.push(curNode->var);
//     }
//   }

//   // cout << "after rebuilding" << endl;
//   // print_graph(g);
//   // cout << "graph has " << g->num_var << " vars" << endl;
//   // cout << g->num_no_color_var << " vars have no color." << endl;
// }

// void coloring(L3::Function *f, Graph *g) {
//   for (auto i : f->instructions) {
//     for (int k = 0; k < i->operands.size(); k++) {
//       string opd = i->operands[k];
//       // cout << "coloring " << opd << " ";
//       if (g->node_map.find(opd) != g->node_map.end()) {
//         // cout << g->node_map[opd]->color;
//         i->operands[k] = g->node_map[opd]->color;
//       }
//       // cout << endl;
//     }
//   }
// }

// int count_var_in_set(string v, std::set<std::string> const seti) {
//   int cnt = 0;
//   for (auto i : seti) {
//     if (i == v) {
//       cnt++;
//     }
//   }
//   return cnt;
// }

// void replace_old_instruction(L3::Function *f,
//                              std::vector<L3::Instruction *> & new_instructions,
//                             int i, string v, int stack_loc) {

//   string s_var = "S" + to_string(stack_loc/8);
//   // cout << f->instructions[i]->opt << "(";
//   for (vector<string>::iterator it = f->instructions[i]->operands.begin(); it < f->instructions[i]->operands.end(); it++) {
//     // cout << *it;
//     if ((*it) == v) {
//       (*it) = s_var;
//     }
//   }

//   L3::Instruction *curI = f->instructions[i];

//   L3::Instruction *newI = new L3::Instruction();
//   newI->opt = curI->opt;
//   newI->mem_opt = curI->mem_opt;
//   newI->cmp_opt = curI->cmp_opt;
//   newI->stack_opt = curI->stack_opt;
//   newI->is_right_mem = curI->is_right_mem;
//   newI->operands = curI->operands;
//   // newI->instr_string = curI->instr_string;

//   new_instructions.push_back(newI);
//   // cout << endl;
//   // for (auto opd : f->instructions[i]->operands) {
//   //   cout << opd << " ";
//   // }
//   // cout << ")" << endl;
// }

// void replace_writes_to_v(L3::Function *f, 
//                          std::vector<L3::Instruction *> & new_instructions,
//                          int i, string v, int stack_loc) {
//   // int num_of_writes_to_v = get_writes_idx(*it, v);
//   string s_var = "S" + to_string(stack_loc/8);
//   // cout << s_var << endl;

//   int num_of_writes_to_v = count_var_in_set(v,f->instructions[i]->GEN);
//   // cout << num_of_writes_to_v << endl;

//   for (int k = 0; k < num_of_writes_to_v; k++) {
//     L3::Instruction *newI = new L3::Instruction();
//     newI->opt = "<-";
//     newI->mem_opt = "mem";
//     newI->is_right_mem = true;
//     newI->operands.push_back(s_var);
//     newI->operands.push_back("rsp");
//     newI->operands.push_back(to_string(stack_loc));
//     // newI->instr_string = s_var + " <- (mem rsp " + to_string(stack_loc) + ")";

//     new_instructions.push_back(newI);
//     // cout << newI->opt << endl;
//     // f->instructions.insert(f->instructions.begin()+i, newI);
//     // cout << f->instructions[i-k]->instr_string << endl;
//     // cout << f->instructions[0]->opt << endl;
//     // cout << f->instructions[1]->opt << endl;
//     // cout << (*(++it))->opt << endl;
//   }
// }

// void replace_reads_from_v(L3::Function *f, 
//                           std::vector<L3::Instruction *> & new_instructions,
//                           int i, string v, int stack_loc) {
//   if (f->instructions[i]->KILL.find(v) == f->instructions[i]->KILL.end()) {
//     return;
//   }
//   // cout << "replace_reads_from_v" << endl;
//   string s_var = "S" + to_string(stack_loc/8);
//   // cout << s_var << endl;
  
//   L3::Instruction *newI = new L3::Instruction();
//   // newI->instr_string = "(mem rsp 0) <-";
//   newI->opt = "<-";
//   newI->mem_opt = "mem";
//   newI->is_right_mem = false;
//   newI->operands.push_back("rsp");
//   newI->operands.push_back(to_string(stack_loc));
//   newI->operands.push_back(s_var);

//   // for (auto opd : newI->operands) {
//   //   cout << opd << " ";
//   // }
//   // cout << endl;
  
//   // newI->instr_string = "(mem rsp 0) <- ";// + s_var;
//    // + to_string(stack_loc) + ") <- "

//   // cout << newI->instr_string << endl;
//   // f->instructions.insert(f->instructions.begin()+i+1, newI);
//   new_instructions.push_back(newI);

//   // cout << new->instr_string << endl;
// }

// void spill(L3::Function *f, string v, string s) {
//   cout << "spill " << v << endl;

//   std::vector<L3::Instruction *> new_instructions;
//   // for (vector<L3::Instruction*>::iterator it = f->instructions.begin(); it < f->instructions.end(); it++) {
//   for (int i = 0; i < f->instructions.size(); i++) {
//     // if (!contain_var(*it)) {
//     //   continue;
//     // }
//     L3::Instruction *instr = f->instructions[i];
//     // cout << f->instructions[i]->instr_string << endl;
//     int stack_loc = (f->locals++) * 8;
    
//     replace_writes_to_v(f, new_instructions, i, v, stack_loc);
//     // cout << (*it)->opt << endl;
//     if (instr->opt != "<-") {
//       replace_old_instruction(f, new_instructions, i, v, stack_loc);
//     }
//     replace_reads_from_v(f, new_instructions, i, v, stack_loc);
//   }

//   // return new_instructions;
//   f->instructions = new_instructions;
//   // std::vector<L3::Instruction *> (new_instructions.begin(), new_instructions.end());
// }

// bool all_colored(Graph *g) {
//   return g->num_no_color_var == 0;
// }

// void translate_stack_arg(L3::Function *f) {
//   for (auto i : f->instructions) {
//     if (!i->stack_opt.empty()) {
//       std::vector<string> new_operands;
//       new_operands.push_back(i->operands[0]); // w
//       new_operands.push_back("rsp");
//       int offset = std::stoi(i->operands[1]) + (f->locals * 8); // M + space for locals
//       new_operands.push_back(std::to_string(offset));
//       i->mem_opt = "mem";
//       i->is_right_mem = true;
//       i->instr_string = i->operands[0] + " <- (mem rsp " + std::to_string(offset) + ")";
//       i->operands = new_operands;
//     }
//   }
// }

// std::vector<string> get_caller_save_vars(std::vector<std::string> & IN, 
//                                          std::vector<std::string> & OUT) {
//   std::vector<string> vars;
//   for (auto v : OUT) {
//     if (isvar(v) && !isGPRegister(v)) {
//       vars.push_back(v);
//     }
//   }
//   return vars;
// }

// void write_caller_callee_save_regiester(string r, int offset, std::vector<L3::Instruction *> &new_instructions) {
//   L3::Instruction *newI = new L3::Instruction();

//   newI->opt = "<-";
//   newI->mem_opt = "mem";
//   newI->is_right_mem = false;
//   newI->operands.push_back("rsp");
//   newI->operands.push_back(std::to_string(offset));
//   newI->operands.push_back(r);

//   new_instructions.push_back(newI);

//   // print_instruction(newI);
// }

// void read_caller_callee_save_regiester(string r, int offset, std::vector<L3::Instruction *> &new_instructions) {
//   L3::Instruction *newI = new L3::Instruction();

//   newI->opt = "<-";
//   newI->mem_opt = "mem";
//   newI->is_right_mem = true;
//   newI->operands.push_back(r);
//   newI->operands.push_back("rsp");
//   newI->operands.push_back(std::to_string(offset));

//   new_instructions.push_back(newI);

//   // print_instruction(newI);
// }

// bool is_caller_save_register(string r) {
//   std::set<string> caller_save_set (caller_save.begin(), caller_save.end());
//   return caller_save_set.find(r) != caller_save_set.end();
// }

// bool is_callee_save_register(string r) {
//   std::set<string> callee_save_set (callee_save.begin(), callee_save.end());
//   return callee_save_set.find(r) != callee_save_set.end();
// }

// void caller_saving_register(L3::Function *f, Graph *g,
//                        std::vector<std::vector<std::string>> & IN,
//                        std::vector<std::vector<std::string>> & OUT) {

//   std::vector<L3::Instruction *> new_instructions;

//   for (int i = 0; i < f->instructions.size(); i++) {
//     if (f->instructions[i]->opt == "call") {
//       std::vector<string> to_save_vars = get_caller_save_vars(IN[i],OUT[i]);
//       std::vector<string> caller_save_vars;

//       for (auto v : to_save_vars) {
//         string color = g->node_map[v]->color;
//         if (is_caller_save_register(color)) {
//           // cout << v << " " << color << endl;
//           caller_save_vars.push_back(color);
//         }
//       }
      
//       int offset = 0;
//       for (auto v : caller_save_vars) {
//         write_caller_callee_save_regiester(v, offset, new_instructions);
//         offset += 8;
//         f->locals++;
//       }
    
//       new_instructions.push_back(f->instructions[i]);
//       if (i < f->instructions.size() - 1 && islabel(f->instructions[i]->operands[0])) {
//         new_instructions.push_back(f->instructions[++i]);
//       }

//       offset = 0;
//       for (auto v : caller_save_vars) {
//         read_caller_callee_save_regiester(v, offset, new_instructions);
//         offset += 8;
//       }
//     }
//     else {
//       new_instructions.push_back(f->instructions[i]);
//     }
//   }

//   f->instructions = new_instructions;
// }

// bool is_runtime_call(L3::Instruction *i) {
//   string function_name = i->operands[0];
//   return i->opt == "call" && (function_name == "print" || function_name == "allocate" || function_name == "array-error");
// }

// bool is_user_function(L3::Instruction *i) {
//   return i->opt == "call" && !is_runtime_call(i);
// }

// void save_callee_save_registers(L3::Function *f, std::set<std::string> registers_to_save) {
  
//   cout << "saving callee save registers" << endl;

//   std::vector<L3::Instruction *> new_instructions;

//   // cout << KILL.size() << endl;
//   // cout << f->instructions.size() << endl;

//   for (int i = 0; i < f->instructions.size(); i++) {
//     string kill_regi;
//     if (f->instructions[i]->KILL.size() > 1) {
//       new_instructions.push_back(f->instructions[i]);
//       continue;
//     }
//     for (auto r : f->instructions[i]->KILL) {
//       // cout << r << " ";
//       kill_regi = r;
//     }
//     // cout << endl;
//     if (registers_to_save.find(kill_regi) != registers_to_save.end()) {
//       // cout << f->instructions[i]->instr_string << endl;
//       print_instruction(f->instructions[i]);
//       // cout << kill_regi << endl;
//       write_caller_callee_save_regiester(kill_regi, 0, new_instructions);
//       new_instructions.push_back(f->instructions[i]);
//       read_caller_callee_save_regiester(kill_regi, 0, new_instructions);
//     }
//     else {
//       new_instructions.push_back(f->instructions[i]);
//     }
//   }

//   f->instructions = new_instructions;
// }

// void callee_save_register(L3::Program p, L3::Function *f, Graph *g) {

//   // cout << "callee_saving_var" << endl;

//   // if (!f->callee_registers_to_save.empty()) {

//   // }

//   int i;
//   int call_index = f->instructions.size();
//   string function_name;

//   for (i = 0; i < f->instructions.size(); i++) {
//     if (i < call_index) {
//       if (is_user_function(f->instructions[i])) {
//         call_index = i;
//         function_name = f->instructions[i]->operands[0];
//         // cout << function_name << endl;
//       }
//     }
//     else {
//       std::set<string> registers_to_save;
//       // cout << call_index << endl;
      
//       // get registers need to save
//       for (int j = call_index + 1; j < f->instructions.size(); j++) {
//         // cout << f->instructions[j]->opt << endl;
//         if (is_user_function(f->instructions[j]) || f->instructions[j]->operands.empty()) {
//           call_index = f->instructions.size();
//           i = j;
//           break;
//         }
//         for (auto opd : f->instructions[j]->operands) {
//           if (is_callee_save_register(opd)) {
//             // cout << opd << endl;
//             registers_to_save.insert(opd);
//           }
//         }
//       }

//       if (!registers_to_save.empty()) {
//         // save callee-save registers
//         for (auto tf : p.functions) {
//           if (tf->name == function_name) {
//             save_callee_save_registers(tf,registers_to_save);
//             break;
//           }
//         }
//       }
//     }
//   }
// }

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

  cout << "printing program:" << endl;
  for (auto f : p.functions) {
    cout << f->name;
    int n = 1;
    for (auto arg : f->args) {
      cout << " " << arg;
    }
    cout << endl;
    for (auto i : f->instructions) {
      cout << n++ << ": ";
      print_instruction(i);
    }
    cout << endl;
  }

  // L3::Simple_Assign_Instruction *newI = new L3::Simple_Assign_Instruction();
  // newI->instr_type = L3::SIMPLE_ASSIGN;
  // newI->assign_left = "rdi";
  // newI->assign_right = "5";
  // print_instruction(newI);

  // L3::Br2_Instruction *newI = new L3::Br2_Instruction();
  // newI->instr_type = L3::BR2;
  // newI->var = "rdi";
  // newI->label1 = ":true";
  // newI->label2 = ":false";
  // print_instruction(newI);
  
  return 0;
}
