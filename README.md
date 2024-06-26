# General-Assembler

Two-pass command line based general assembler. Please read all of the below notes.

## Update as of May 25 2024:
The listing file output is not necessarily correct, but should assemble someoutput using the the `isa.txt`. The outputs have not been rigorously tested. Currently the listing file output format is only supported. The output will not contain any hex formatted code, but the raw hex from the opcodes can be seen in the listing file (use the `-l` flag). The utils files contains a `main.s` assembly file that you can use for testing, but again bear in mind the output will not necessarily be correct. The pseudo operations may not work as intended/cause assembly problems.

## Usage

Run `make` (you will need to have it installed) in the General-Assembler directory. Then build using `make`. `cd` into the GenA directory and you can then run gena with the command `./gena` for further instructions.

## Options

* `-f`, `--file <main file path>`  
  Specify the main file path (required).

* `-i`, `--isa <ISA file path>`  
  Specify the ISA file path (required).

* `-o`, `--output <output file>`  
  Specify the output file path (optional).

* `-t`, `--list`  
  Produce a listing file.

* `-l`, `--log`  
  Log all output to `gena.log` in the current directory.

* `-v`, `--verbose`  
  Output all information to the terminal.

* `-h`, `--help`  
  Display this help message and exit.

* `-r`, `--version`  
  Display the version information and exit.

## Notes

- The `--output` flag is optional. If not specified, the default output file will be `output_gena.HEX` in the working directory.
- Both `--file` and `--isa` flags must be used with valid paths.
- Both `--log` and `--verbose` flags cannot be used simultaneously.

---
What makes this assembler general is the ISA file. This can describe any harvard or 
princeton architecture even with variable sized instruction sets. A user library
and a processors properties must be defined at the top. The bulk of the file
are code macros.

## Code Macro

<instruction> <opcode> <operand template> <parsing function> <instruction size>

Each instruction in a processor must be defined in this manner. 

The AVR and Caltech10 ISA files and user libraries are in utils/ for reference.
Additionally documentation inside deprecated/ may be helpful while current 
documentation improves. 

## Pseudo Ops

Supported pseudo operations are code location, file inclusion, and variable 
declarations. GenA also supports forward referencing. Use the `.org` tag to specify the code segment and the `.data` tag before every variable declaration for data segmenting.




