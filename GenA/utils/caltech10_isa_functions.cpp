#include <cstring>
#include <iostream>
#include <string>
#include <cstdlib> // For system()
#include <filesystem>
#include <fstream>
#include <bitset>
#include <vector>

extern "C" {

    const size_t opcode_FULL_SIZE = 16;

    size_t parse_value(const std::string &op) {
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
    size_t parse_alu_st_ld_io(size_t opcode, const std::vector<std::string> &args) {
        size_t opcode_bit_count = 8;
        size_t op1_toi = parse_value(args[0]);
        size_t data = opcode << (opcode_FULL_SIZE - opcode_bit_count);
        data |= op1_toi;
        return data;
    }

    // Opcode is directly represented. Applies for stack operations, NOP, Index 
    // Register instructions, Flag instructions, and some ALU operations, NOP
    size_t parse_full_length(size_t opcode, const std::vector<std::string> &) {
        return opcode;
    }

    // JMP and Call instructions. Format is ###a aaaa aaaa aaaa
    size_t parse_jump_call(size_t opcode, const std::vector<std::string> &args) {
        size_t opcode_bit_count = 3;
        size_t label_toi = parse_value(args[0]);
        size_t opcode_shifted = opcode << (opcode_FULL_SIZE - opcode_bit_count);
        return opcode_shifted | label_toi;
    }

    size_t parse_rel_jump(size_t opcode, const std::vector<std::string> &args) {
        size_t program_counter = std::stoul(args[1]);
        size_t opcode_shifted = opcode << 8;
        size_t label_toi = parse_value(args[0]) & 0xFF;
        size_t offset = label_toi - program_counter;
        return opcode_shifted | offset;
    }
}
