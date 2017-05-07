#pragma once

#include <vector>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

namespace L3 {

// enum Instruction_Type {
//     SIMPLE_ASSIGN,
//     OP_ASSIGN,
//     CMP_ASSIGN,
//     LOAD_ASSIGN,
//     STORE_ASSIGN,
//     CALL_ASSIGN,
//     LABEL,
//     BR1,
//     BR2,
//     RETURN,
//     VAR_RETURN,
//     CALL
// };

enum Item_Type {
    NUMBER,
    VAR,
    LBL,
    T,
    U,
    S,
    M,
    E,
    EM,
    N1,
    ASGN,
    CMP,
    SHIFT,
    // L,
    // LE,
    // EQ,
    // LS,
    // RS,
    PLUS,
    MINUS,
    MUL,
    AD,
    PRINT,
    ALLOCATE,
    ARRAYERROR,
    LOAD,
    STORE,
    BR,
    CALL,
    RETURN
};

// enum Callee_Type {
//     PRINT,
//     ALLOCATE,
//     ARRAYERROR
// };

class Node {
    public:
        std::string item;
        std::string val;
        Item_Type ittp;
        std::vector< Node * > children;

        Node () {}
        Node (std::string it) {
            item = it;
            int n;
            if (isdigit(item.at(0)) || ((item.at(0)=='+' || item.at(0)=='-') && isdigit(item.at(1)))) {
                if (item.at(0) == '-') {
                    ittp = NUMBER;
                }
                else {
                    if (isdigit(item.at(0))) {
                        n = std::stoi(item);
                    }
                    else {
                        n = std::stoi(item.substr(1));
                    }
                    if (n == 0 || n == 8) {
                        ittp = EM;
                    }
                    else if (n == 2 || n == 4) {
                        ittp = E;
                    }
                    else if (n % 8 == 0) {
                        ittp = M;
                    }
                    else if (n == 1) {
                        ittp = N1;
                    }
                    else {
                        ittp = NUMBER;
                    }
                }
            }
        }
        Node (Item_Type itp) : ittp(itp) {}
        Node (std::string it, Item_Type itp) : item(it), ittp(itp) {}

        bool operator==(Node * another_node) {
            if (!another_node) {
                return false;
            }
            return item == another_node->item;
        }

        void printNode() {
            cout << item << "(" << ittp << ") ";
            if (!children.empty()) {
                for (auto ch : children) {
                    if (ch)
                        ch->printNode();
                }
            }
        }

        // void 
        // Node(std::string it, std::vector<std::string> vec) {
        //     item = it;
        //     for (auto v : vec) {
        //         Node * ch = new Node(v);
        //         children.push_back(ch);
        //     }
        // }
};

// Node::Node(std::string it) : item(it) {}

class Tree {
    public:
        Node * root;
        // std::vector<std::string> L2_instr_operands;

        void printTree() {
            root->printNode();
            cout << endl;
        }
};

class Instruction {
    public:
        // L3::Instruction_Type instr_type;
        Tree *t;

        virtual std::string toString() const { return ""; }
        virtual Tree * genTree() { return t; }

        void printTree() { t->printTree(); }
        Tree * getTree() { return t; }
};

class Simple_Assign_Instruction : public Instruction {
    public:
        std::string assign_left;
        std::string assign_right;

        Simple_Assign_Instruction() {}
        Simple_Assign_Instruction(std::string l, std::string r) 
        : assign_left(l), assign_right(r) {}

        std::string toString() const {
            std::string s("");
            s += assign_left + " <- " + assign_right;
            return s;
        }

        Tree * genTree() {
            t = new Tree();
            t->root = new Node("<-", ASGN);
            t->root->children.push_back(new Node(assign_left, VAR));
            t->root->children.push_back(new Node(assign_right, S));

            return t;
        }
};

class Op_Assign_Instruction : public Instruction {
    public:
        std::string assign_left;
        std::string op;
        std::string op_left;
        std::string op_right;

        Op_Assign_Instruction() {}
        Op_Assign_Instruction(std::string l, std::string o, std::string oleft, std::string oright)
        : assign_left(l), op(o), op_left(oleft), op_right(oright) {}

        std::string toString() const {
            std::string s("");
            s += assign_left + " <- " + op_left + " " + op + " " + op_right;
            return s;
        }

        Tree * genTree() {
            t = new Tree();
            t->root = new Node("<-", ASGN);
            
            Node * assign_right;
            if (op == "+") {
                assign_right = new Node(op, PLUS);
            }
            else if (op == "-") {
                assign_right = new Node(op, MINUS);
            }
            else if (op == "*") {
                // cout << '1' << endl;
                assign_right = new Node(op, MUL);
            }
            else if (op == "&") {
                assign_right = new Node(op, AD);
            }
            else {
                assign_right = new Node(op, SHIFT);
                // cout << "Unknown op." << endl;
            }


            assign_right->children.push_back(new Node(op_left, T));
            // cout << '2' << endl;

            assign_right->children.push_back(new Node(op_right, T));
            
            t->root->children.push_back(new Node(assign_left, VAR));
            t->root->children.push_back(assign_right);


            return t;
        }
};

class Cmp_Assign_Instruction : public Instruction {
    public:
        std::string assign_left;
        std::string cmp;
        std::string cmp_left;
        std::string cmp_right;

        Cmp_Assign_Instruction() {}
        Cmp_Assign_Instruction(std::string l, std::string c, std::string cleft, std::string cright)
        : assign_left(l), cmp(c), cmp_left(cleft), cmp_right(cright) {}

        std::string toString() const {
            std::string s("");
            s += assign_left + " <- " + cmp_left + " " + cmp + " " + cmp_right;
            return s;
        }

        void reverseTree(std::string new_cmp) {
            Node * cmp_node = t->root->children[1];

            cmp_node->item = new_cmp;
            
            Node * cmp_right_node = cmp_node->children.back();
            cmp_node->children.pop_back();

            Node * cmp_left_node = cmp_node->children.back();
            cmp_node->children.pop_back();

            cmp_node->children.push_back(cmp_right_node);
            cmp_node->children.push_back(cmp_left_node);
        }

        Tree * genTree() {
            t = new Tree();
            t->root = new Node("<-", ASGN);

            Node * assign_right;
            assign_right = new Node(cmp, CMP);
            // if (cmp == "<") {
            //     assign_right = new Node(cmp, L);
            // }
            // else if (cmp == "<=") {
            //     assign_right = new Node(cmp, LE);
            // }
            // else if (cmp == "=") {
            //     assign_right = new Node(cmp, EQ);
            // }
            // else if (cmp == ">=") {
            //     assign_right = new Node("<=", LE); // reverved item and ittp
            // }
            // else if (cmp == ">") {
            //     assign_right = new Node("<", L); // reverved item and ittp
            // }
            // else {
            //     cout << "Unknown cmp." << endl;
            // }
            assign_right->children.push_back(new Node(cmp_left, T));
            assign_right->children.push_back(new Node(cmp_right, T));

            t->root->children.push_back(new Node(assign_left, VAR));
            t->root->children.push_back(assign_right);

            if (cmp == ">=") {
                reverseTree("<=");
            }
            if (cmp == ">") {
                reverseTree("<");
            }

            return t;
        }
};

class Load_Assign_Instruction : public Instruction {
    public:
        std::string assign_left;
        std::string var;

        Load_Assign_Instruction() {}
        Load_Assign_Instruction(std::string l, std::string v) : assign_left(l), var(v) {}

        std::string toString() const {
            std::string s("");
            s += assign_left + " <- load " + var;
            return s;
        }

        Tree * genTree() {
            t = new Tree();
            t->root = new Node("<-", ASGN);

            Node *assign_right = new Node("load", LOAD);
            assign_right->children.push_back(new Node(var, VAR));
            
            t->root->children.push_back(new Node(assign_left, VAR));
            t->root->children.push_back(assign_right);

            return t;
        }
};

class Store_Assign_Instruction : public Instruction {
    public:
        std::string assign_right;
        std::string var;

        Store_Assign_Instruction() {}
        Store_Assign_Instruction(std::string v, std::string r) : assign_right(r), var(v) {}

        std::string toString() const {
            std::string s("");
            s += "store " + var + " <- " + assign_right;
        return s;
        }

        Tree * genTree() {
            t = new Tree();
            t->root = new Node("<-", ASGN);

            Node *assign_left = new Node("store", STORE);
            assign_left->children.push_back(new Node(var, VAR));

            t->root->children.push_back(assign_left);
            t->root->children.push_back(new Node(assign_right, S));

            return t;
        }
};

class Call_Assign_Instruction : public Instruction {
    public:
        std::string assign_left;
        std::string callee;
        std::vector<std::string> args;

        Call_Assign_Instruction() {}
        Call_Assign_Instruction(std::string l, std::string cle, std::vector<std::string> a) 
        : assign_left(l), callee(cle), args(a) {}


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

        Tree * genTree() {
            t = new Tree();
            t->root = new Node("<-", ASGN);

            Node *assign_right = new Node("call", CALL);
            if (callee == "print") {
                assign_right->children.push_back(new Node(callee, PRINT));
            }
            else if (callee == "allocate") {
                assign_right->children.push_back(new Node(callee, ALLOCATE));
            }
            else if (callee == "array-error") {
                assign_right->children.push_back(new Node(callee, ARRAYERROR));
            }
            else {
                assign_right->children.push_back(new Node(callee, U));
            }
            for (auto arg : args) {
                assign_right->children.push_back(new Node(arg, T));
            }

            t->root->children.push_back(new Node(assign_left, VAR));
            t->root->children.push_back(assign_right);

            return t;
        }
};

class Br1_Instruction : public Instruction {
    public:
        std::string label;

        Br1_Instruction() {}
        Br1_Instruction(std::string l) : label(l) {}
        
        std::string toString() const {
            std::string s("");
            s += "br " + label;
            return s;
        }

        Tree * genTree() {
            t = new Tree();
            t->root = new Node("br", BR);
            t->root->children.push_back(new Node(label, LBL));
            return t;
        }
};

class Br2_Instruction : public Instruction {
    public:
        std::string var;
        std::string label1;
        std::string label2;

        Br2_Instruction() {}
        Br2_Instruction(std::string v, std::string l1, std::string l2) 
        : var(v), label1(l1), label2(l2) {}
        
        std::string toString() const {
            std::string s("");
            s += "br " + var + " " + label1 + " " + label2;
            return s;
        }

        Tree * genTree() {
            t = new Tree();
            t->root = new Node("br", BR);
            t->root->children.push_back(new Node(var, VAR));
            t->root->children.push_back(new Node(label1, LBL));
            t->root->children.push_back(new Node(label2, LBL));
            return t;
        }
};

class Return_Instruction : public Instruction {
    public:
        std::string toString() const {
            return "return";
        }

        Tree * genTree() {
            t = new Tree();
            t->root = new Node("return", RETURN);
            return t;
        }
};

class Var_Return_Instruction : public Return_Instruction {
    public:
        std::string var;

        Var_Return_Instruction() {}
        Var_Return_Instruction(std::string v) : var(v) {}
        
        std::string toString() const {
            std::string s("");
            s += "return " + var;
            return s;
        }

        Tree * genTree() {
            t = new Tree();
            t->root = new Node("return", RETURN);
            t->root->children.push_back(new Node(var, T));
            return t;
        }
};

class Call_Instruction : public Instruction {
    public:
        std::string callee;
        std::vector<std::string> args;

        Call_Instruction() {}
        Call_Instruction(std::string cle, std::vector<std::string> a) : callee(cle), args(a) {}

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

        Tree * genTree() {
            t = new Tree();
            t->root = new Node("call", CALL);
            if (callee == "print") {
                t->root->children.push_back(new Node(callee, PRINT));
            }
            else if (callee == "allocate") {
                t->root->children.push_back(new Node(callee, ALLOCATE));
            }
            else if (callee == "array-error") {
                t->root->children.push_back(new Node(callee, ARRAYERROR));
            }
            else {
                t->root->children.push_back(new Node(callee, U));
            }
            for (auto arg : args) {
                t->root->children.push_back(new Node(arg, T));
            }
            return t;
        }
};

class Label_Instruction : public Instruction {
    public:
        std::string label;

        Label_Instruction() {}
        Label_Instruction(std::string l) : label(l) {}

        std::string toString() const {
            return label;
        }

        Tree * genTree() {
            t = new Tree();
            t->root = new Node(label, LBL);
            return t;
        }
};

// class L2_Simple_Assign_Instruction : Instruction {
//     public:
//         std::string assign_left;
//         std::string assign_right;

//         Simple_Assign_Instruction() {}
//         Simple_Assign_Instruction(std::string l, std::string r) 
//         : assign_left(l), assign_right(r) {}

//         std::string toString() const {
//             std::string s("");
//             s += assign_left + " <- " + assign_right;
//             return s;
//         }

//         Tree * genTree() {
//             t = new Tree();
//             t->root = new Node("<-", ASGN);
//             t->root->children.push_back(new Node(assign_left, VAR));
//             t->root->children.push_back(new Node(assign_right, VAR));

//             return t;
//         }

// };

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
