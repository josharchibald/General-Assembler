# General Assembler System Specification

## Preface

This is the System Specification for GenA, a General assembly Program.

Throughout this specification, whenever one section references an idea from
another section, there will be a reference and a hyperlink to the
referenced section. The purpose of this, above all, is to stress the
modularization of the system’s specification. A consequence of this,
hopefully, is a swift conclusion to the inquiries of GenA’s users, no matter
the nature of their inquiry, so long as it’s about what GenA does.

In any file mentioned in this specification, GenA will concatenate the next line
after a line that ends with the character ‘\’. It will then remove any white
space between the beginning of a line and the first character and also remove
any white space between the last character and the end of the line.

All things mentioned in this specification are case insensitive unless otherwise
stated.

Henceforth, mention of AVR syntax and grammar[^1] will be in reference to this
resource
[AVR® Instruction Set Manual (microchip.com)](https://www.microchip.com).

How general is general? GenA can assemble code to **any** digital processor.
GenA comes with an AVR ISA (Instruction Set Architecture) file
(see [ISA File](#isa-file)) and if the user wishes to assemble code for a
different processor, they must use the format stated in the
[ISA File](#isa-file) section to create the ISA for their processor.

Groups of identical delimiters/separators, will be referred to in this document
as a single unit, and will be recognized in inputs as a single unit, that unit
being, naturally, the only unit present in the group.

## Description

GenA is a two pass general assembly program capable of assembling assembly
programs (see[Assembly Files](#assembly-files)) written for any processor.
Assembled programs are converted into Intel HEX machine code and a listing file
is provided (see [Outputs](#outputs)). GenA supports labels, forward references,
and a variety of pseudo operations (see[Pseudo Operations](#pseudo-operations)).
GenA is distributed with an ISA file for the AVR processor but also allows for
user specified ISA files (see [ISA File](#isa-file)). GenA is command line based
(see [User Interface](#user-interface)) and written in C++.

## Inputs

### Assembly Files

GenA must take in either a list of file names or a specified folder containing a
minimum of one assembly file via the command line
(see [User Interface](#user-interface)). All assembly files must have the same
extension.

Assembly files must adhere to the syntax and grammar specified in the user
provided ISA file (see the [ISA File](#isa-file)).
GenA is distributed with an AVR SGrammer file and it can be used to assemble AVR
Assembly programs.

### Include Files

Include files[^2] are optional but will also be included in the list of files or
the specified input folder (see [User Interface](#user-interface)). Include
files must have extension `.inc` and are used to store symbolic constants. They
can only be referenced by assembly files through the .include pseudo-op
(see [Pseudo Operations](#pseudo-operations)). Comments in include files can be
declared by using the “;” character  and end with the end of the line.

### Pseudo Operations

GenA supports pseudo operation usage for the provided assembly files. Only one
pseudo operation can be used per line and they must follow the syntactical
format specified. The supported pseudo operations are:

* File inclusion

* Section headers

* Code organization

* Constant literals

* Symbolic constants

* In line expressions

#### File Inclusion

A file can reference another file by including it in the following format:

`.include <filename>`

The file must an assembly or include file.

During assembly, the contents of the included assembly files are put
in place of this pseudo operation. Files that have already been included by
other files will not be included again by GenA. In general, it is best practice
to not have circular includes.

#### Section Headers

Section headers are used to divide the assembly files up into code and data
sections. `.code` and `.data` must exist as the only string in their line.

##### `.code`

All assembly code must be written in a `.code` section. A `.code` section is a
section of an assembly file between a line with the `.code` pseudo operation and
either another section, or the end of the file.

##### `.data`

(optional) This section can be used to declare the data segment for variables.
The section is defined as a section of the assembly file between a `.data`
pseudo operation and either another section or the end of the file. Two types of
variables are supported in the `.data` section, local and global.

Local variables must be uniquely named across both local variables within the
same file and all global variables in all files. For more on global variables
see below. Local variables do not have to be uniquely named across all local
variables in other files. Local variables must be declared like so:

`VariableName: BYTECOUNT`

Global variables are variables that must be uniquely named across all other global
variables and all local variables and all labels in all files. For example, if
there is a global “count” variable in one file, no other file can have a global
or local “count” variable or label. Global variables must be declared like so:

`.global VariableName: BYTECOUNT`

The `VariableName` is the name of the variable (cannot include whitespaces) and
`BYTECOUNT` is the decimal representation of the amount of bytes allocated to
the variable.

Only `.global` variables can be referenced from any scope outside the assembly
file it is contained in. Labels behave like variables when it comes to their
naming rules and relationship with the `.global` pseudo operation.

It is best to have unique local, global, and label names across all files, as
this is how these elements are normally incorporated into assembly files.

All variables and labels will be replaced in assembly code by their address in
data and program memory respectively at assembly time. More on these memory
spaces can be seen in the [ISA file](#isa-file) section.

#### Code organization

The `.org` pseudo operation is used in the following manner:  

`.org <ADDRESS>`

to set the line in code it precedes to that address in program memory. More on
this piece of memory in the [ISA file](#isa-file) section.

#### Constant Literals

Constant literals are non-aliased values put into assembly code. By default,
these values are interpreted as decimal numbers, however, it may be prefixed in
the following ways:

* `0DVALUE` - decimal representation

* `0HVALUE` - hexadecimal representation

* `0QVALUE` - octal representation

* `0BVALUE` - binary representation

An example usage could be:

`ADD R1, 0B00000001 ; add 1 to Register 1`

#### Symbolic constants

Symbolic constants can only be declared inside an include (.inc) file. Symbolic
constants must be declared like so:

`.define NAME VALUE`

where `NAME` is the name of the symbol (no white spaces) and  `VALUE` is the
associated value. `VALUE` is interpreted like how a constant literal is by GenA.

An example definition could be:

```
.define BINARY_ONE 0B00000001                ;binary form of 1
```

For assembly, whenever a string matching any constant is found in the
operands[^3] of an assembly file[^4], it will be replaced with its corresponding
symbolic value. Symbolic value names must be distinct across all include files.

#### In-Line Expressions

The operators, `+`, `-`, `*`, `/`, `%`, `<<`, and `>>` are supported between any
combination of symbolic constants, literals, labels, and variable names.

## ISA File

A single ISA (Instruction Set Architecture) file must be included in the list of
files of in the folder specified by the user through the command line (see
[User Interface](#user-interface)). GenA comes with an AVR ISA file
(avr_isa.txt) and if the user wants to supply their own for another processor,
it must be in the following format.

The first line must be the absolute path to a C++ file. This file functions only
within the scope of this ISA file and the standard for such a file is rigorously
defined and so its format, as it pertains to the ISA file, will briefly
explained without an entirely new section. This C++ file must contain one or
more functions with the **case sensitive** names `F1, F2, F3, ...` and so on.
These functions will be discussed in detail later. An example is as follows:

`C:\Users\user\GenA_ISA\functions.cpp`

The second line must be the decimal representation of the number of bits in a
single word of the processor's program data then the data data separated by a
whitespace. If your processor shares program and data space then only put a
single number.

The third line must be the decimal representation of the number of words in the
processor's program data then the data data separated by a whitespace. If your
processor shares program and data space then only put a single number.

In line four, the user must define the order and delineation of the elements in
a line of assembly. A line of assembly can contain the following elements:

* Label

* OpName

* Operand

* Comment

Delimiters must be distinct, and the last element must be the Comment, which is
delimited by the end of the line. The other elements can be arranged in any
order. In this first line there cannot be white space between any element and
its following delimiter unless the delimiter is a white space. An example of 
this line is as follows:

`Label: OpName Operand; Comment`

Lines of assembly must follow this order and delineation, although
they may not need to include all elements.

Every element can stand alone except the Operand, which must always have an
OpName in the same line. Every element can exist in the same line with any
number of the other elements. Every element can only exist in a line once.

Labels follow the same uniqueness rules as variable names and hold the same
relationship to the `.global` pseudo operation (see
[Pseudo Operations](#pseudo-operations)).

All following lines must take the form of a Code Macro.

A Code Macro  is used to specify how the OpName and Operand can be converted to
machine code. A Code Macro consists of the following strings separated by white
spaces.

* 1st string: the string representation of the `OpName`.

* 2nd string: the hex representation of the `OpCode` number.

* Next set of strings: combination of Symbols and Values.
  * Symbols will be represented as `Sym` followed by the string corresponding
    to it.
  * Values will be represented as `Val`. (e.g. `SymR Val` for R16)

* Last strings must be the **case sensitive** function name of one of the
  functions `F1, F2, F3, ...` within the specified C++ file followed by the
  decimal representation of the number of bits in the instruction. These are to
  be separated by a white space.

The `OpName` is used to identify Code Macros while parsing assembly and the
functions specified in those Code Macros are used to turn the `OpCode` and the
`Operand`, in the form of symbols and values, into machine code.

Each function must take in the `OpCode` as an integer, and each `Val` as a
string. Each function must output and integer that fits into the specified bit
count for the Code Macro it is used in.

For example the Code Macro for the instruction `ADD Rd, Rs` might look as
follows:

`ADD AA SymR Val Sym,R Val F1 16`

`F1` must, in this case, return a 16 bit integer which might derive from the
`OpCode` followed by the `d` and `s` as integer encodings. When parsing a line
of assembly that looks like this:

`ADD R1, R2`

the inputs to `F1` would be `AA` as an integer and `1` and `2` as strings. These
could be converted to decimals and concatenated to the `OpCode` to produce the
instruction but in general, Code Macros and their accompanying functions can
produce the ISA for **any** digital processor.

## Outputs

Upon successful completion, GenA will output a “[output_name] Assembled” message.
GenA outputs two files, whose names are specified on the command line, to a single
folder, whose name is also specified on the command line (see [User Interface](#user-interface)).
The first is the machine code object file. One object file will be produced per
successful assembly, and it will follow Intel HEX format.

The second is a listing file (.lst). The first thing on every line of the listing
file is the line number relative to the listing file. Everything past the “;”
character in a line is considered a comment. Comments contain the header, the
symbol table, and user code. For lines where user code has instructions, the program
data and address precede the comment in hexadecimal format. An example portion of
a listing file output could be:

```

1; FolderName assembled by GenA with config file “config.txt”
2; Start of code:
3 0000       ; UpdateTimer:                ; Update the timer by 1s
4 0000 1A34  ; LDD R16, counter            ; retrieve the counter
5 0001 1C7F  ; ADIW R16, 1_SECOND          ; add 1 second


6 0010 261A  ; STS counter, R16            ; store the new value

```

## User Interface

To use GenA the user should first put the provided “gena” executable in their
executable folder (on *Nix like environments, this is either in “/usr/local/bin”
or “/usr/bin”; on Windows this is in "%LocalAppData%\Programs” or any directory
in the %PATH% environment variable). Alternatively, users can just make symbolic
links to these directories. Now the user can run GenA by typing “gena” in the
parent directory of a folder that contains the files the user wishes to assemble
as well as the SGrammar file (see [Inputs](#inputs)) . Upon running the GenA program,
the user will be shown a list of commands and their possible arguments. Commands
not following this format will be ignored:

* `help`  

* the help command will display a scrollable screen that explains how to use
  GenA in the terminal. The interface is similar to a Linux man page.

* `asm [input_folder] [output_folder] [output_name] [sgrammar_name.txt]`

* the assemble command assembles all the files in the specified input folder
  with the SGrammar file specified and outputs an object file and listing file
  with the output name to a folder with the output folder name. The output folder
  will be located in the parent directory.

* By default, the AVR SGrammar file (avr_sgrammar.txt) is included with  GenA
  and must be copied into the input folder for usage. However, the user can
  create their  own SGrammer file and specify it in the input folder (see
  [Syntax and Grammar File](#syntax-and-grammar-sgrammar-file)).

* If the output folder does not exist, GenA will create it in the same directory.

* If the output file name already exists, GenA will overwrite it ONLY if it
  successfully assembles

* For more on input and output files see the [Inputs](#inputs) and
  [Outputs](#outputs) sections.

* `version`

* the version command will display a version number, last update date, and a
  link to the most recent github repository.

* `exit`

* the exit command will exit the program.

After the asm command is run, and before “[output_name] Assembled” is displayed,
“Assembling…” is displayed and user input is blocked.

## Error Handling
>
>All previous sections are required to understand this section.
>
>When the user runs the asm command, there are two possible types of errors that
>can occur: file finding errors and file syntax errors. When GenA encounters an
>error, it immediately terminates the assembling process and does not write to
>the target file if it already exists. Thus, if there are multiple syntax errors,
>GenA will only report the first error it finds.
>
>File finding errors occur when specified folders or files are not found. Possible
>errors are:
>
>* `Error - SGrammar [file name] file not found`
>
>* the specified SGrammar file (see [Syntax and Grammar File](#syntax-and-grammar-sgrammar-file))
>   is not found
>
>* `Error - Folder [folder name] not found`
>
>* the specified input folder is not found
>
>* `Error - Main file not found in [folder name]`
>
>* the specified folder does not include a main.s file in the folder specified
>
>* `Error - File [file name] not found in include`
>
>* the specified folder does not include the .inc or .asm file referenced by other
>  files through the .include pseudo operation
>
>File syntax errors occur when files have syntax errors. This could be:
>
>* `Error - Style mismatch in file [file name] at line [line number]`
>
>* the specified line of code in the specified file is not formatted as according
>  to the style (first line) in the SGrammar file (see [Syntax and Grammar File]
>  (#syntax-and-grammar-sgrammar-file))
>
>* `Error - Duplicate local reference of [label/variable] in [file]`
>
>* the specified line of code in the the specified file has a two local
>  variables/global labels that are the same
>
>* `Error - Duplicate global reference of [label/variable]`
>
>* the code contains two local/global variables or labels that are the same name

---

[^1]: Syntax refers to the style in which the assembly is written, and grammar
refers to the interactions between the assembly code and the machine code as
defined by a processor’s instruction set and assembly manual.

[^2]: Not to be confused with the File inclusion pseudo operation.

[^3]: For more on exactly what “the operands” means see the [Assembly Files](#assembly-files) section.

[^4]: Not another include file.
