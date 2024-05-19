#include <cstring>
#include <iostream>
#include <string>
#include <cstdlib> // For system()
#include <filesystem>
#include <fstream>
#include <bitset>

#define XL 26
#define XH 27

#define YL 28
#define YH 29

#define ZL 30
#define ZH 31

#define OP_CODE_MAX_SIZE 16
size_t value_parser(std::string val){
    if (val.empty()) {
        return 0;
    }
    std::cout << "val: " << val << std::endl;
    // get the part of the value that contains the register designator
    char first_char = val[0];
    switch (first_char)
    {
    case 'R':
        return std::stoi(val.substr(1));
    case 'X':
        if(val[1]=='L') {
            return XL;
        } else {
            return XH;
        }
    case 'Y':
        if(val[1] =='L') {
            return YL;
        } else {
            return YH;
        }
    case 'Z': 
        if(val[1]=='L') {
            return ZL;
        } else {
            return ZH;
        }    
    default:
        // assume it is a constant 
        return std::stoi(val);
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
// Arithmetic and Logical operations
// bit code formatting is #### ##rd rrrr dddd , syntax is IST Rd, Rr
size_t F1(size_t op_code, std::string val1, std::string val2) {
    size_t val1_toi = value_parser(val1);
    size_t val2_toi = value_parser(val2);
    std::cout << "val1 " << val1_toi << std::endl;
    std::cout << "val2 " << val2_toi << std::endl;
    size_t data = 0;
    data = (op_code) << (OP_CODE_MAX_SIZE - 6);
    size_t r_msb = ((val2_toi >> 4)) << 9;
    size_t d_msb = ((val1_toi >> 4)) << 8;
    size_t r_rest = (val2_toi & 0b1111) << 4;
    size_t d_rest = val1_toi & 0b1111;
    data = data | r_msb | d_msb | r_rest | d_rest;
    return data;
}
// formatting is #### KKKK dddd KKKK where dddd 16 - 31
size_t F2(size_t op_code, std::string val1, std::string val2) {
    size_t val1_toi = value_parser(val1);
    size_t val2_toi = value_parser(val2);
    size_t data;
    if(val1_toi < 16 && val2_toi < 16) {
        std::cerr << "Value must be in register range 16-31";
        return (size_t)NULL;
    }
    switch (op_code)
    {
    default:
        data = op_code << (OP_CODE_MAX_SIZE - 4);
        size_t k_first_4 = ((val2_toi) >> 4) << 8;
        size_t k_last_4 = (val2_toi & 0b1111);
        size_t d_last_4 = (val1_toi & 0b1111) << 4;
        data = data | k_first_4 | d_last_4 | k_last_4;
        return data;
    }
}


// Default ocode format is #### #### dddd ####  
size_t F3(size_t op_code, std::string val1, std::string val2) {
    size_t val1_toi = value_parser(val1);
    size_t data;
    if(val1_toi < 16) {
        std::cerr << "Value must be in register range 16-31";
        return (size_t)NULL;
    }
    switch (op_code)
    {
    default:
        size_t op_code_first_7 = ((op_code) >> 4) << OP_CODE_MAX_SIZE - 7;
        size_t op_code_last_4 = (op_code & 0b1111);
        size_t d_shifted = val1_toi << 4;
        data = op_code_first_7 | d_shifted | op_code_last_4;
        return data;
    }
}
size_t F4() {
    return (size_t)NULL;
}
size_t F5() {
    return (size_t)NULL;
}
size_t F6() {
    return (size_t)NULL;
}
size_t F7() {
    return (size_t)NULL;
}
size_t F8() {
    return (size_t)NULL;
}

int main() {
    std::string rd = "ZL";
    std::string rr = "ZH";
    size_t bit_size = 6;
    size_t opcode = 7;
    size_t data = F1(opcode, rd, rr);

    std::string d = "R24";
    std::string k = "200";
    data = F3(1189, d, k); 
    print_binary_with_spaces(data);
    return 0;
}