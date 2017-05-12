#pragma once

#include <vector>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

namespace IR {

class Instruction {
    public:
        virtual std::string toString() const { return ""; }
        // virtual void replace_label(std::string, std::string) { return; }
};

class Type_Instruction : public Instruction {
    public:
        std::string type;
        std::string var;

        Type_Instruction() {}
        Type_Instruction(std::string v) : var(v) {}

        std::string toString() const {
            std::string s("");
            s += type + " " + var;
            return s;
        }
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
};

class Right_Array_Assign_Instruction : public Instruction {
    public:
        std::string assign_left;
        std::string array_name;
        std::vector<std::string> indices;

        Right_Array_Assign_Instruction() {}
        Right_Array_Assign_Instruction(std::string l, std::string an, std::vector<std::string> ind)
        : assign_left(l), array_name(an), indices(ind) {}

        std::string toString() const {
            std::ostringstream oss;

            if (!indices.empty()) {
                // Convert all but the last element to avoid a trailing ","
                std::copy(indices.begin(), indices.end()-1, std::ostream_iterator<std::string>(oss, "]["));
                // Now add the last element with no delimiter
                oss << indices.back();
            }

            std::string s("");
            s += assign_left + " <- " + array_name + "[" + oss.str() + "]";
            return s;
        }

};

class Left_Array_Assign_Instruction : public Instruction {
    public:
        std::string assign_right;
        std::string array_name;
        std::vector<std::string> indices;

        Left_Array_Assign_Instruction() {}
        Left_Array_Assign_Instruction(std::string r, std::string an, std::vector<std::string> ind)
        : assign_right(r), array_name(an), indices(ind) {}

        std::string toString() const {
            std::ostringstream oss;

            if (!indices.empty()) {
                // Convert all but the last element to avoid a trailing ","
                std::copy(indices.begin(), indices.end()-1, std::ostream_iterator<std::string>(oss, "]["));
                // Now add the last element with no delimiter
                oss << indices.back();
            }

            std::string s("");
            s +=  array_name + "[" + oss.str() + "]" + " <- " + assign_right;
            return s;
        }

};

class Length_Assign_Instruction : public Instruction {
    public:
        std::string assign_left;
        std::string var;
        std::string t;

        Length_Assign_Instruction() {}
        Length_Assign_Instruction(std::string l, std::string v) : assign_left(l), var(v) {}

        std::string toString() const {
            std::string s("");
            s += assign_left + " <- length " + var + " " + t;
            return s;
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
};

class Return_Instruction : public Instruction {
    public:
        std::string toString() const {
            return "return";
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
};

class Label_Instruction : public Instruction {
    public:
        std::string label;

        Label_Instruction() {}
        Label_Instruction(std::string l) : label(l) {}

        std::string toString() const {
            return label;
        }
};

class New_Array_Assign_Instruction : public Instruction {
    public:
        std::string assign_left;
        std::vector<std::string> args;

        New_Array_Assign_Instruction() {}
        New_Array_Assign_Instruction(std::string l, std::vector<std::string> a)
         : assign_left(l), args(a) {}

        std::string toString() const {
            std::ostringstream oss;

            if (!args.empty()) {
                // Convert all but the last element to avoid a trailing ","
                std::copy(args.begin(), args.end()-1, std::ostream_iterator<std::string>(oss, ","));
                // Now add the last element with no delimiter
                oss << args.back();
            }

            std::string s("");
            s += assign_left + " <- new Array(" + oss.str() + ")";

            return s;
        }
};

class New_Tuple_Assign_Instruction : public Instruction {
    public:
        std::string assign_left;
        std::string t;

        New_Tuple_Assign_Instruction() {}
        New_Tuple_Assign_Instruction(std::string l, std::string t) 
        : assign_left(l), t(t) {}

        std::string toString() const {
            

            std::string s("");
            s += assign_left + " <- new Tuple(" + t + ")";

            return s;
        }
};

class New_Array_Instruction : public Instruction {
    public:
        std::vector<std::string> args;

        New_Array_Instruction() {}
        New_Array_Instruction(std::vector<std::string> a) : args(a) {}

        std::string toString() const {
            std::ostringstream oss;

            if (!args.empty()) {
                // Convert all but the last element to avoid a trailing ","
                std::copy(args.begin(), args.end()-1, std::ostream_iterator<std::string>(oss, ","));
                // Now add the last element with no delimiter
                oss << args.back();
            }

            std::string s("");
            s = s + "new Array(" + oss.str() + ")";

            return s;
        }
};

class New_Tuple_Instruction : public Instruction {
    public:
        std::string t;

        New_Tuple_Instruction() {}
        New_Tuple_Instruction(std::string t) : t(t) {}

        std::string toString() const {
            std::string s("");
            s = s + "new Tuple(" + t + ")";

            return s;
        }
};

class BB{
    public:
        std::string label;
        std::vector<IR::Instruction *> instructions;
        IR::Instruction * terminator;
};

class Function{
    public:
        std::string name;
        std::string return_type;
        std::vector<std::string> args;
        std::vector<BB *> bbs;
        std::set<std::string> labels;
        //std::set<string> callee_registers_to_save;
};

class Program{
    public:
        std::string entryPointLabel;
        std::vector<IR::Function *> functions;
        int64_t label_suffix;
};
} // IR
