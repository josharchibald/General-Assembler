#include <cstring>
#include <iostream>
#include <string>
#include <cstdlib> // For system()
#include <filesystem>
#include <fstream>
#include <bitset>
#include <algorithm>
#include <cctype>

extern "C" {

    // Define Register Symbols used in AVR.
    const size_t XL = 26;
    const size_t XH = 27;
    const size_t YL = 28;
    const size_t YH = 29;
    const size_t ZL = 30;
    const size_t ZH = 31;

    const size_t OPCODE_DEFAULT_LENGTH = 16; // Default instruction length is 16 bit
    const size_t OPCODE_EXTRA_LENGTH = 32;      // But a few instructions are 32

    /**
     * Converts the string representation of a register or value to its decimal 
     * equivalent. For example R16 would return 16.
     * 
     * @param val the value to parse
     * @return the decimal representation of the value.
    */
    size_t parse_value(std::string val){
        if (val.empty()) {
            return 0;
        }
        // convert the string to upper case.
        std::transform(val.begin(), val.end(), val.begin(),
                    [](unsigned char c) { return std::toupper(c); });
        // get the part of the value that contains the register designator
        char first_char = val[0];
        std::cout << first_char << std::endl;
        switch (first_char)
        {
        // Handle register parsing
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
            // Else assume the value is a literal decimal
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
    void print_binary_with_spaces_32(uint32_t number) {
        std::bitset<32> binary(number);

        std::cout << number << " in binary (32-bit): ";
        for (int i = 31; i >= 0; --i) {
            std::cout << binary[i];
            if (i % 4 == 0) std::cout << " ";
        }
        std::cout << std::endl;
    }

    // 4 bit opcode instructions: includes ALU ops with immediates
    // general format is #### KKKK dddd KKKK
    size_t parse_alu_imm(size_t opcode, std::string reg, std::string immediate){
        size_t immediate_toi = 0;
        immediate_toi = parse_value(immediate);
        size_t reg_toi = parse_value(reg);
        size_t data = 0;
        size_t opcode_bit_length = 4;
        size_t opcode_shifted = (opcode) << (OPCODE_DEFAULT_LENGTH - opcode_bit_length);
        size_t immediate_first_byte = ((immediate_toi >> 4)) << 8;
        size_t immediate_last_byte = immediate_toi & 0b1111;
        size_t reg_last_4_bits = (reg_toi & 0b1111) << 4;
        data = opcode_shifted | immediate_first_byte | reg_last_4_bits | immediate_last_byte;
        return data;
    }
    // Some ALU ops with 1 bit.
    size_t parse_alu_1(size_t opcode, std::string reg_d){
        size_t reg_d_toi = parse_value(reg_d) & 0b111111;
        size_t opcode_bit_length = 6;
        size_t opcode_shifted = opcode << (OPCODE_DEFAULT_LENGTH - opcode_bit_length);
        return opcode_shifted | reg_d_toi;
    }
    // Some ALU operations with 2 inputs, format is #### ##rd dddd rrrr
    size_t parse_alu_2(size_t opcode, std::string reg_d, std::string reg_r){
        size_t reg_d_toi = parse_value(reg_d);
        size_t reg_r_toi  = parse_value(reg_r);
        size_t opcode_bit_length = 6;
        size_t opcode_shifted = opcode << (OPCODE_DEFAULT_LENGTH - opcode_bit_length);
        size_t reg_r_first_bit_shifted = (reg_r_toi >> 4) << 9;
        size_t reg_r_last_4bits  = reg_r_toi & 0b1111; 
        size_t reg_d_first_bit_shifted = (reg_d_toi >> 4) << 8;
        size_t reg_d_last_4bits_shifted = (reg_d_toi & 0b1111) << 4;
        return opcode_shifted | reg_r_first_bit_shifted | reg_d_first_bit_shifted 
                            | reg_d_last_4bits_shifted | reg_r_last_4bits;
    }


    //Full length instructions. op code is #### #### #### #####
    size_t parse_full_length(size_t opcode){
        return opcode;
    }

    // Branch instructions except BRBC/BRBS, format is #### ##kk kkkk k###
    size_t parse_branch(size_t opcode, std::string label, size_t program_counter){
        size_t label_toi = parse_value(label) & 0b1111111;
        size_t offset = label_toi - program_counter;
        size_t opcode_bit_length = 9;
        size_t opcode_first_6_shifted = (opcode >> (opcode_bit_length - 6)) << OPCODE_DEFAULT_LENGTH - 6;
        size_t opcode_last_3 = opcode & 0b111;
        size_t offset_first_7_shifted = (offset & 0b1111111) << 3;
        return opcode_first_6_shifted | offset_first_7_shifted | opcode_last_3;
    }
    // Branch instructions for BRBC/BRBS
    size_t parse_branch_with_bit(size_t opcode, std::string label, std::string sreg_bit, size_t program_counter){
        size_t label_toi = parse_value(label) & 0b1111111;
        size_t offset = label_toi - program_counter;
        size_t sreg_bit_toi = parse_value(label) & 0b111;
        size_t opcode_shifted = opcode << OPCODE_DEFAULT_LENGTH - 6;
        size_t offset_shifted = (label_toi & 0b1111111) << 3;
        return opcode_shifted | offset_shifted | sreg_bit_toi;
    }

    // Some ALU and LD/ST instructions. Format is #### ###d dddd ####
    size_t parse_ld_st_stck_alu(size_t opcode, std::string reg){
        size_t reg_toi = parse_value(reg);
        size_t opcode_bit_length = 11;
        size_t opcode_first7_shifted = (opcode >> (opcode_bit_length - 7)) 
                                        << (OPCODE_DEFAULT_LENGTH - 7);
        size_t opcode_last4 = opcode & 0b1111;
        size_t reg_shifted = (reg_toi & 0b11111) << 4;
        return opcode_first7_shifted | reg_shifted | opcode_last4;
    }
    // Most multiplication. Format is #### #### #ddd #rrr except MULS
    // Default ocode format is #### #### dddd ####  
    size_t parse_mul(size_t opcode, std::string reg_d, std::string reg_r){
        size_t reg_d_toi = parse_value(reg_d);
        size_t reg_r_toi  = parse_value(reg_r);
        switch (opcode)
        {
        // MULS is 12 bit long instruction and different format
        case 2:
        {
            size_t opcode_bit_length = 8;
            size_t opcode_first_9bits_shifted = opcode << (OPCODE_DEFAULT_LENGTH - opcode_bit_length);
            size_t reg_r_last_4bits  = reg_r_toi & 0b1111; 
            size_t reg_d_last_4bits_shifted = (reg_d_toi & 0b1111) << 4;
            return opcode_first_9bits_shifted | reg_d_last_4bits_shifted | reg_r_last_4bits;
        }
        default:
        {
            size_t opcode_bit_length = 10;
            size_t opcode_first_9bits_shifted = (opcode >> 1) << (OPCODE_DEFAULT_LENGTH - 9);
            size_t opcode_last_bit_shifted = (opcode & 0b1) << 3;
            size_t reg_r_last_3bits  = reg_r_toi & 0b111; 
            size_t reg_d_last_3bits_shifted = (reg_d_toi & 0b111) << 4;
            return opcode_first_9bits_shifted | opcode_first_9bits_shifted 
                    | reg_d_last_3bits_shifted |opcode_last_bit_shifted 
                    | reg_r_last_3bits;
        }
        }
    }
    // I/O instructions, format is #### #AAd dddd AAAA
    size_t parse_io(size_t opcode, std::string op1, std::string op2){
        // IN format is IN R, Port
        size_t reg_toi;
        size_t port_toi;
        if (opcode == 22) {
            reg_toi = parse_value(op1) & 0b11111;
            port_toi = parse_value(op2) & 0b11111;
        // OUT format is OUT PORT, R
        } else  {
            reg_toi = parse_value(op2) & 0b11111;
            port_toi = parse_value(op1) & 0b11111;
        }
        size_t opcode_bit_length = 5;
        size_t opcode_shifted = opcode << (OPCODE_DEFAULT_LENGTH - opcode_bit_length);
        size_t port_first_2_shifted = (port_toi >> 3) << 9;
        size_t reg_shifted = reg_toi << 4;
        size_t port_last_4 = port_toi & 0b1111;
        return opcode_shifted | port_first_2_shifted | reg_shifted | port_last_4;
    }
    // DES and SER instruction
    size_t parse_des_ser(size_t opcode, std::string val){
        size_t val_toi = parse_value(val) & 0b1111;
        size_t val_shifted = val_toi << 4;
        size_t opcode_first_8_shifted = (opcode >> 4) << (OPCODE_DEFAULT_LENGTH - 8);
        size_t opcode_last_4 = opcode & 0b1111; 
        return opcode_first_8_shifted | val_shifted  | opcode_last_4;
    }

    //BCLR/BSET
    size_t parse_modify_flags(size_t opcode, std::string flag_bit){
        size_t flag_bit_toi = parse_value(flag_bit) & 0b111;
        size_t flag_bit_shifted = flag_bit_toi << 4;
        size_t opcode_first_9_shifted = (opcode >> 4) << (OPCODE_DEFAULT_LENGTH - 9);
        size_t opcode_last_4 = opcode & 0b1111;
        return opcode_first_9_shifted | flag_bit_shifted | opcode_last_4;
    }
    //SBRC/SBRS and BLD/BST
    size_t parse_bit_check_load_store(size_t opcode, std::string val, std::string bit){
        size_t val_toi = parse_value(val) & 0b1111;
        size_t bit_toi = parse_value(bit) & 0b111;
        size_t opcode_first_7_shifted = opcode << (OPCODE_DEFAULT_LENGTH - 7);
        size_t opcode_last_shifted = (opcode & 0b1) << 3;
        size_t val_shifted = val_toi << 8;
        return opcode_first_7_shifted | val_shifted | opcode_last_shifted | bit_toi;
    }
    // SBI
    size_t parse_clear_set_bit(size_t opcode, std::string io_reg, std::string bit){
        size_t io_reg_toi = parse_value(io_reg) & 0b11111;
        size_t bit_toi = parse_value(bit) & 0b111;
        size_t opcode_length = 8;
        size_t opcode_shifted = opcode << (OPCODE_DEFAULT_LENGTH - opcode_length);
        size_t io_reg_shifted = io_reg_toi << 7;
        return opcode_shifted | io_reg_shifted | bit_toi;
    }

    //32 bit LDS and STS
    size_t parse_load_store_32(size_t opcode, std::string val1, std::string val2){
        size_t reg_toi;
        size_t imm_toi;
        // LDS
        if (opcode == 1152) {
            reg_toi = parse_value(val1) & 0b11111;
            imm_toi = parse_value(val2) & 0xFFFF;
        // STS
        } else {
            reg_toi = parse_value(val2) & 0b11111;
            imm_toi = parse_value(val1) & 0xFFFF;
        }
        size_t opcode_first_7_shifted = (opcode >> 4) << (OPCODE_EXTRA_LENGTH - 7);
        size_t opcode_last_4_shifted = (opcode & 0b1111) << (OPCODE_DEFAULT_LENGTH);
        size_t reg_toi_shifted = reg_toi << (OPCODE_EXTRA_LENGTH - 12);
        return opcode_first_7_shifted | reg_toi_shifted | opcode_last_4_shifted | imm_toi;
    }

    size_t parse_load_store_16(size_t opcode, std::string val1, std::string val2){
        size_t reg_toi;
        size_t imm_toi;
        size_t opcode_length = 5;
        size_t data = 0;
        //store
        if (opcode == 21 || opcode > 17) {
            reg_toi = parse_value(val2) & 0b1111;
            imm_toi = parse_value(val1) & 0b111111;
        // load
        } else {
            reg_toi = parse_value(val1) & 0b1111;
            imm_toi = parse_value(val2) & 0b111111;
        }
        // STS/LDS
        if (opcode >= 20) {
            size_t opcode_shifted =  opcode << (OPCODE_DEFAULT_LENGTH - opcode_length);
            size_t imm_first_3_shifted = (imm_toi >> 3) << 8;
            size_t reg_shifted = reg_toi << 4;
            size_t imm_last_4 = imm_toi & 0b1111;
            return opcode_shifted | imm_first_3_shifted | reg_shifted | imm_last_4;
        // STD/LD
        } else {
            size_t opcode_first_2_shifted =  (opcode >> 3) << (OPCODE_DEFAULT_LENGTH - 2);
            size_t imm_first_shifted = (imm_toi >> 5) << (OPCODE_DEFAULT_LENGTH - 3);
            size_t opcode_middle_1_shifted = ((opcode >> 2) & 0b1) << (OPCODE_DEFAULT_LENGTH - 4);
            size_t imm_middle2_shifted = ((imm_toi >> 3) & 0b11) << 10;
            size_t opcode_middle_2_shifted = (opcode & 0b10) << 9;
            size_t reg_shifted = reg_toi << 4;
            size_t opcode_last_shifted = (opcode & 0b1) << 3;
            size_t imm_last_3 = imm_toi & 0b111;
            return opcode_first_2_shifted | imm_first_shifted | opcode_middle_1_shifted |
                imm_middle2_shifted | opcode_middle_2_shifted | reg_shifted | 
                    opcode_last_shifted | imm_last_3;
        }
        
    }
    // ADIW, SUBIW
    size_t parse_add_sub_word(size_t opcode, std::string reg, std::string word){
        size_t reg_toi = parse_value(reg);
        size_t word_toi = parse_value(word);
        size_t opcode_length = 8;
        size_t opcode_shifted = opcode << (OPCODE_DEFAULT_LENGTH - opcode_length);
        // special limitations on the register value
        size_t reg_as_two_bits_shifted = (((reg_toi - 24) / 2) & 0b11 ) << 4;
        size_t word_first_2_shifted = (word_toi  >> 4) << 6;
        size_t word_last_4 = word_toi & 0b1111;
        return opcode_shifted | word_first_2_shifted | reg_as_two_bits_shifted | word_last_4;
    }
    // MOVW
    size_t parse_mov_word(size_t opcode, std::string reg_d, std::string reg_r){
        size_t reg_d_toi = parse_value(reg_d) & 0b1111;
        size_t reg_r_toi = parse_value(reg_r) & 0b1111;
        size_t opcode_length = 8;
        size_t opcode_shifted = opcode << (OPCODE_DEFAULT_LENGTH - opcode_length);
        size_t reg_d_shifted = reg_d_toi << 4;
        return opcode_shifted | reg_d_shifted | reg_r_toi;
    }
    // Relative jump and call
    size_t parse_rcall_rjmp(size_t opcode, std::string label, const size_t program_counter){
        size_t opcode_length = 4;
        size_t opcode_shifted = opcode << (OPCODE_DEFAULT_LENGTH - opcode_length);
        size_t label_toi = parse_value(label) & 0xFFF;
        size_t offset = (label_toi - program_counter) & 0xFFF;
        return opcode_shifted | offset;
    }
    // Format #### ###k kkkk ###k kkkk kkkk kkkk kkkk
    size_t parse_call_jmp(size_t opcode, std::string label){
        size_t label_toi = parse_value(label);
        size_t opcode_first_7 = (opcode >> 3) << (OPCODE_EXTRA_LENGTH - 7);
        size_t opcode_last_3_shifted = (opcode & 0b111) << (OPCODE_EXTRA_LENGTH - 15);
        size_t label_first_5_shifted = (label_toi >> 17);
        size_t label_last_17 = label_toi & 0x1FFFF;
        return opcode_first_7 | label_first_5_shifted | opcode_last_3_shifted | label_last_17;
    }
}

    int main() {
        std::string rd = "ZL";
        std::string rr = "ZH";
        size_t bit_size = 6;
        size_t opcode = 7;
        size_t data;

        // std::string d = "R24";
        // std::string k = "200";
        data = parse_io(23, "", ""); 
        print_binary_with_spaces(data);
        print_binary_with_spaces(parse_alu_imm(14, "", ""));
        print_binary_with_spaces(parse_bit_check_load_store(250, "", "7"));
        print_binary_with_spaces(parse_clear_set_bit(155, "", ""));
        print_binary_with_spaces(parse_des_ser(2379, "300"));
        print_binary_with_spaces(parse_des_ser(3839, "300"));
        print_binary_with_spaces(parse_full_length(37912));
        print_binary_with_spaces(parse_ld_st_stck_alu(1177, "300"));
        print_binary_with_spaces(parse_load_store_16(20, "40", "300"));
        print_binary_with_spaces(parse_mul(15, "300", "17"));
        print_binary_with_spaces(parse_add_sub_word(150, "", ""));
        // print_binary_with_spaces(parse_rcall_rjmp(150, "", ""));
        print_binary_with_spaces(parse_load_store_16(17, "", ""));
        print_binary_with_spaces(parse_load_store_16(21, "", ""));
        print_binary_with_spaces(parse_mov_word(1, "r16", "r17"));
        print_binary_with_spaces_32(parse_call_jmp(598, "9392239"));
        print_binary_with_spaces_32(parse_call_jmp(599, "9392239"));
        print_binary_with_spaces_32(parse_load_store_32(1152, "r93", "9392239"));
        print_binary_with_spaces_32(parse_load_store_32(1168, "9392239", "r93"));
        print_binary_with_spaces(parse_modify_flags(4760, "300"));
        print_binary_with_spaces(parse_modify_flags(4744, "300"));
        print_binary_with_spaces(parse_branch(483, "939", 1));
        print_binary_with_spaces(parse_branch(494, "939", 1));
        print_binary_with_spaces(parse_branch_with_bit(61, "939", "11", 1));
        print_binary_with_spaces(parse_branch_with_bit(60, "939", "11", 1));
        return 0;
}