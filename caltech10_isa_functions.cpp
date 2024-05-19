#include <cstring>
#include <iostream>
#include <string>
#include <cstdlib> // For system()
#include <filesystem>
#include <fstream>
#include <bitset>

#define OP_CODE_MAX_SIZE 16

const size_t X_REG_VALUE = 10;
const size_t S_REG_VALUE = 11;
size_t operator_parser(std::string op){
    if (op.empty()) {
        return 0;
    }
    std::cout << "val: " << op << std::endl;
    // get the part of the value that contains the register designator
    char first_char = op[0];
    switch (first_char)
    {
    case 'X':
        return X_REG_VALUE;
    case 'S':
        return S_REG_VALUE;
    default:
        // assume it is a constant 
        return std::stoi(op);
    }
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
// Most ALU operations. Format is #### #### vvvv vvvv (8 bit opcode + 8 bit 
// immediate/offest/address)
size_t f1(size_t op_code, std::string op1, std::string op2) {
    size_t data;
    size_t default_op_code_bit_count = 8;
    size_t op1_toi = operator_parser(op1);
    data = op_code << (OP_CODE_MAX_SIZE - default_op_code_bit_count);
    data = data | op1_toi;
    return data;
}
// Load/Store instructions. Format is #### #### vvvv vvvv
size_t f2(size_t op_code, std::string op1, std::string op2) {
    size_t op1_toi = operator_parser(op1);
    size_t data;
    size_t default_op_code_bit_count = 8;
    size_t op1_toi = operator_parser(op1);
    data = op_code << (OP_CODE_MAX_SIZE - default_op_code_bit_count);
    data = data | op1_toi;
    return data;
}


// Jump instrctions. Aside from JMP, formatting is #### #### rrrr rrrr 
size_t f3(size_t op_code, std::string op1, std::string op2){
    size_t data;
    size_t default_op_code_bit_count = 8;
    size_t jmp_code_bit_count = 3;
    size_t op1_toi = operator_parser(op1);
    switch(op_code)
    {   
        // JMP is 110a aaaa aaaa aaaaa
        case(6):
            data = op_code << (OP_CODE_MAX_SIZE - jmp_code_bit_count);
            data = data | op1_toi;
            return data;
        default:
            data = op_code << (OP_CODE_MAX_SIZE - default_op_code_bit_count);
            data = data | op1_toi;
            return data;
    }
}
// Opcode is directly represented. Applies for stack operations, NOP, Index 
// Register instructions, Flag instructions, and some ALU operations
size_t f4(size_t op_code, std::string op1, std::string op2){
    return op_code;
}
// Call instruction, 111a aaaa aaaa aaaa
size_t f5(size_t op_code, std::string op1, std::string op2){
    size_t data;
    size_t default_op_code_bit_count = 3;
    size_t op1_toi = operator_parser(op1);
    data = op_code << (OP_CODE_MAX_SIZE - default_op_code_bit_count);
    data = data | op1_toi;
    return data;
}

// Other stack instructions: RTS, POPF, PUSHF.  Format is all 16 bits.
size_t f6(size_t op_code, std::string op1, std::string op2){    
    return op_code;
}
// I/O instructions. Format is #### #### pppp pppp.
size_t f7(size_t op_code, std::string op1, std::string op2){
    size_t data;
    size_t default_op_code_bit_count = 8;
    size_t op1_toi = operator_parser(op1);
    data = op_code << (OP_CODE_MAX_SIZE - default_op_code_bit_count);
    data = data | op1_toi;
    return data;
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