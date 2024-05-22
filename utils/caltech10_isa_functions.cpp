#include <cstring>
#include <iostream>
#include <string>
#include <cstdlib> // For system()
#include <filesystem>
#include <fstream>
#include <bitset>

extern "C" {

    const size_t OP_CODE_FULL_SIZE = 16;

    size_t parse_operator(std::string op){
        if (op.empty()) {
            return 0;
        }
        std::cout << "val: " << op << std::endl;
        return std::stoi(op);
    }
    void print_binary_with_spaces(uint16_t number) {
        std::bitset<16> binary(number);

        std::cout << number << " in binary (16-bit): ";
        for (int i = 15; i >= 0; --i) {
            std::cout << binary[i];
            if (i % 4 == 0) std::cout << " ";
        }
        std::cout << std::endl;
    }
    // Most alu, load, store, and i/o instructions. Format is #### #### vvvv vvvv 
    // (8 bit opcode + 8 bit immediate/offest/address)
    size_t f1(size_t op_code, std::string op1) {
        size_t data;
        size_t op_code_bit_count = 8;
        size_t op1_toi = parse_operator(op1);
        // shift the op_code such that the first op_code_bit_count bits are the code
        data = op_code << (OP_CODE_FULL_SIZE - op_code_bit_count);
        data = data | op1_toi;
        return data;
    }
    // Opcode is directly represented. Applies for stack operations, NOP, Index 
    // Register instructions, Flag instructions, and some ALU operations, NOP
    size_t f2(size_t op_code) {
        return op_code;
    }

    // JMP and Call instructions. Format is ###a aaaa aaaa aaaa
    size_t f3(size_t op_code, std::string op1){
        size_t data;
        size_t op_code_bit_count = 3;
        size_t op1_toi = parse_operator(op1);
        // shift the op_code such that the first op_code_bit_count bits are the code
        data = op_code << (OP_CODE_FULL_SIZE - op_code_bit_count);
        data = data | op1_toi;
        return data;
    }
}
// int main() {
//     std::string rd = "ZL";
//     std::string rr = "ZH";
//     size_t bit_size = 6;
//     size_t opcode = 7;
//     size_t data = f1(opcode, rd, rr);

//     std::string d = "R24";
//     std::string k = "200";
//     data = f3(1189, d, k); 
//     print_binary_with_spaces(data);
//     return 0;
// }