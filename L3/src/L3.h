#pragma once

#include <vector>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

namespace L3 {

    enum Instruction_Type {
        SIMPLE_ASSIGN,
        OP_ASSIGN,
        CMP_ASSIGN,
        LOAD_ASSIGN,
        STORE_ASSIGN,
        CALL_ASSIGN,
        LABEL,
        BR1,
        BR2,
        RETURN,
        VAR_RETURN,
        CALL
    };

    // enum Callee_Type {
    //     PRINT,
    //     ALLOCATE,
    //     ARRAYERROR
    // };

    class Node {
        public:
            std::string item;
            std::vector< Node * > children;

            Node ();
            Node (std::string it) {
                item = it;
            }
            // Node(std::string it, std::vector<std::string> vec) {
            //     item = it;
            //     for (auto v : vec) {
            //         Node * ch = new Node(v);
            //         children.push_back(ch);
            //     }
            // }
    };


    class Tree {
        public:
            Node * root;
    };

    class Instruction {
        public:
            L3::Instruction_Type instr_type;
            Tree *t;

            virtual std::string toString() const { return ""; };
            virtual void genTree() { return; };

            void printTree() {
                printNode(t->root);
                cout << endl;
            }
            void printNode(Node *n) {
                cout << n->item << " ";
                for (auto ch : n->children) {
                    printNode(ch);
                }
            }
    };

    class Simple_Assign_Instruction : public Instruction {
        public:
            std::string assign_left;
            std::string assign_right;

            std::string toString() const {
                std::string s("");
                s += assign_left + " <- " + assign_right;
                return s;
            }

            void genTree() {
                t = new Tree();
                t->root = new Node("<-");
                t->root->children.push_back(new Node(assign_left));
                t->root->children.push_back(new Node(assign_right));
            }
    };

    class Op_Assign_Instruction : public Instruction {
        public:
            std::string assign_left;
            std::string op;
            std::string op_left;
            std::string op_right;

            std::string toString() const {
                std::string s("");
                s += assign_left + " <- " + op_left + " " + op + " " + op_right;
                return s;
            }

            void genTree() {
                t = new Tree();
                t->root = new Node("<-");
                
                Node *assign_right = new Node(op);
                assign_right->children.push_back(new Node(op_left));
                assign_right->children.push_back(new Node(op_right));
                
                t->root->children.push_back(new Node(assign_left));
                t->root->children.push_back(assign_right);
            }
    };

    class Cmp_Assign_Instruction : public Instruction {
        public:
            std::string assign_left;
            std::string cmp;
            std::string cmp_left;
            std::string cmp_right;

            std::string toString() const {
                std::string s("");
                s += assign_left + " <- " + cmp_left + " " + cmp + " " + cmp_right;
                return s;
            }

            void genTree() {
                t = new Tree();
                t->root = new Node("<-");

                Node *assign_right = new Node(cmp);
                assign_right->children.push_back(new Node(cmp_left));
                assign_right->children.push_back(new Node(cmp_right));

                t->root->children.push_back(new Node(assign_left));
                t->root->children.push_back(assign_right);
            }
    };

    class Load_Assign_Instruction : public Instruction {
        public:
            std::string assign_left;
            std::string var;

            std::string toString() const {
                std::string s("");
                s += assign_left + " <- load " + var;
                return s;
            }

            void genTree() {
                t = new Tree();
                t->root = new Node("<-");

                Node *assign_right = new Node("load");
                assign_right->children.push_back(new Node(var));
                
                t->root->children.push_back(new Node(assign_left));
                t->root->children.push_back(assign_right);
            }
    };

    class Store_Assign_Instruction : public Instruction {
        public:
            std::string assign_right;
            std::string var;

            std::string toString() const {
                std::string s("");
                s += "store " + var + " <- " + assign_right;
            return s;
            }

            void genTree() {
                t = new Tree();
                t->root = new Node("<-");

                Node *assign_left = new Node("store");
                assign_left->children.push_back(new Node(var));

                t->root->children.push_back(assign_left);
                t->root->children.push_back(new Node(assign_right));
            }
    };

    class Call_Assign_Instruction : public Instruction {
        public:
            std::string assign_left;
            std::string callee;
            std::vector<std::string> args;

            std::string toString() const {

                std::ostringstream oss;

                if (!args.empty()) {
                    // Convert all but the last element to avoid a trailing ","
                    std::copy(args.begin(), args.end()-1, std::ostream_iterator<std::string>(oss, ","));
                    // Now add the last element with no delimiter
                    oss << args.back();
                }

                std::string s("");
                s += assign_left + " <- call " + callee + " (" + oss.str() + ")";
                return s;
            }

            void genTree() {
                t = new Tree();
                t->root = new Node("<-");

                Node *assign_right = new Node("call");
                assign_right->children.push_back(new Node(callee));
                for (auto arg : args) {
                    assign_right->children.push_back(new Node(arg));
                }

                t->root->children.push_back(new Node(assign_left));
                t->root->children.push_back(assign_right);
            }
    };

    class Br1_Instruction : public Instruction {
        public:
            std::string label;
            
            std::string toString() const {
                std::string s("");
                s += "br " + label;
                return s;
            }

            void genTree() {
                t = new Tree();
                t->root = new Node("br");
                t->root->children.push_back(new Node(label));
            }
    };

    class Br2_Instruction : public Instruction {
        public:
            std::string var;
            std::string label1;
            std::string label2;
            
            std::string toString() const {
                std::string s("");
                s += "br " + var + " " + label1 + " " + label2;
                return s;
            }

            void genTree() {
                t = new Tree();
                t->root = new Node("br");
                t->root->children.push_back(new Node(var));
                t->root->children.push_back(new Node(label1));
                t->root->children.push_back(new Node(label2));
            }
    };

    class Return_Instruction : public Instruction {
        public:
            std::string toString() const {
                return "return";
            }

            void genTree() {
                t = new Tree();
                t->root = new Node("return");
            }
    };

    class Var_Return_Instruction : public Return_Instruction {
        public:
            std::string var;
            
            std::string toString() const {
                std::string s("");
                s += "return " + var;
                return s;
            }

            void genTree() {
                t = new Tree();
                t->root = new Node("return");
                t->root->children.push_back(new Node(var));
            }
    };

    class Call_Instruction : public Instruction {
        public:
            std::string callee;
            std::vector<std::string> args;

            std::string toString() const {

                std::ostringstream oss;

                if (!args.empty()) {
                    // Convert all but the last element to avoid a trailing ","
                    std::copy(args.begin(), args.end()-1, std::ostream_iterator<std::string>(oss, ","));
                    // Now add the last element with no delimiter
                    oss << args.back();
                }

                std::string s("");
                s = s + "call " + callee + " (" + oss.str() + ")";
                
                return s;
            }

            void genTree() {
                t = new Tree();
                t->root = new Node("call");
                t->root->children.push_back(new Node(callee));
                for (auto arg : args) {
                    t->root->children.push_back(new Node(arg));
                }
            }
    };

    class Label_Instruction : public Instruction {
        public:
            std::string label;
            std::string toString() const {
                return label;
            }

            void genTree() {
                t = new Tree();
                t->root = new Node(label);
            }
    };

    class Function{
        public:
            std::string name;
            int64_t arguments;
            int64_t locals;
            std::vector<L3::Instruction *> instructions;
            std::vector<std::string> args;
            //std::set<string> callee_registers_to_save;
    };

    class Program{
        public:
            std::string entryPointLabel;
            std::vector<L3::Function *> functions;
    };
} // L3
