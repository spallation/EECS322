#pragma once

#include <vector>
#include <sstream>
// #include <iostream>

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

    class Instruction {
        public:
            L3::Instruction_Type instr_type;

            virtual std::string toString() const { return ""; };
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
    };

    class Br1_Instruction : public Instruction {
        public:
            std::string label;
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
                // switch(callee) {
                //     case L3::PRINT :
                //         s = s + "call print " + " (" + oss.str() + ")";
                //         break;
                //     case L3::ALLOCATE :
                //         s = s + "call allocate " + " (" + oss.str() + ")";
                //         break;
                //     case L3::ARRAYERROR :
                //         s = s + "call array-error " + " (" + oss.str() + ")";
                //         break;
                //     default :
                //         break;
                // }
                return s;
            }
    };

    class Label_Instruction : public Instruction {
        public:
            std::string label;
            std::string toString() const {
                return label;
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
