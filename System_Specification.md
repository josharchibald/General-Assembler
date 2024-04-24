# Assembler System Specification

## Preface:

This is the System Specification for Gena, a General assembly Program.

Throughout this specification, whenever one section references an idea from
 another section, there will be a reference and a hyperlink to the
  referenced section. The purpose of this, above all, is to stress the
 modularization of the system’s specification. A consequence of this,
 hopefully, is a swift conclusion to the inquiries of Gena’s users, no matter
 the nature of their inquiry, so long as it’s about what Gena does.

In any file mentioned in this specification, Gena will concatenate the next line
after a line that ends with the character ‘\’. It will then remove any white space
between the beginning of a line and the first character and also remove any white
space between the last character and the end of the line.

All things mentioned in this specification are case insensitive.

Henceforth, mention of AVR syntax and grammar[^1] will be in reference to this
resource [AVR® Instruction Set Manual (microchip.com)](https://www.microchip.com).

How general is general? Gena cannot assemble assembly code for processors with
variable program data length. Besides this, Gena is unlimited as far as digital
processors go. Gena comes with an AVR SGrammer file (see [Syntax and Grammar File]
(#syntax-and-grammar-sgrammar-file)) that the user must provide (see
[User Interface](#user-interface)). If the user wishes to assemble code for a
different processor they must use the format stated in the [Syntax and Grammar File]
(#syntax-and-grammar-sgrammar-file) section to create their own SGrammar file.

Groups of identical delimiters[^2], will be referred to in this document as a
single delimiter, and will be recognized in inputs as a single delimiter, that
delimiter being, naturally, the only delimiter present in the group.

## Description:

Gena is a general assembly program capable of assembling assembly programs (see
[Assembly Files](#assembly-files)) written for any processor with non-variable
program data length. Assembled programs are converted into Intel HEX machine
code and a listing file is provided (see [Outputs](#outputs)). Gena supports
labels, forward references, and a variety of pseudo operations (see
[Pseudo Operations](#pseudo-operations)) in assembly code. Gena is distributed
with an SGrammar file for the AVR processor but also allows for user specified
syntax and grammar files (see [Syntax and Grammar File](#syntax-and-grammar-sgrammar-file)).
Gena is command line based (see [User Interface](#user-interface)) and written in C++.

## Inputs:

### Assembly Files:

Gena must take in a specified folder containing a minimum of one assembly file
via the command line. Exactly one assembly file in this folder must be named `main.asm`.
(See more on usage in [User Interface](#user-interface)).

Assembly files must adhere to the syntax and grammar specified in the user
provided SGrammar file (see the [Syntax and Grammar File](#syntax-and-grammar-sgrammar-file)).
Gena is distributed with an AVR SGrammer file and it can be used to assemble AVR
Assembly programs. For pseudo operations supported within assembly files, see the
[Pseudo Operations](#pseudo-operations) section below.

### Pseudo Operations:

Gena supports pseudo operation usage for the provided assembly files. Only one
pseudo operation can be used per line and they must follow the syntactical format
specified. The supported pseudo operations are:

* File inclusion

* Section headers (`.code` and  `.data`)

* Literal constants

* In line expressions

#### File Inclusion:

A file can reference another file by including it in the following format:
`.include filename`
Where filename is the name of the file to refer to. The file must be a .asm or a
.inc file (See [Include Files](#include-files)). Includes must be declared before
any other section of code or data in a file. Files cannot include the `main.asm` file.

During assembler operation, the contents of the included file are concatenated to
the bottom of the file that declared them. Files that have already been included
by other files will not be included again by Gena. In general, it is best practice
to not have circular includes.

#### Section Headers:

Section headers are used to divide the assembly files up into .code and .data
sections. `.code` and `.data` must exist as the only string in their line.

##### `.code`:

All assembly code must be written in a `.code` section. A `.code` section is a
section of an assembly file between a line with the `.code` pseudo operation and
either another section, or the end of the file.

##### `.data`:

(optional) This section can be used

to declare the data segment for variables. The section is defined as a section
of the assembly file between a `.data` pseudo operation and either another section
or the end of the file. Two types of variables are supported in the `.data` section.

Local variables must be uniquely named across both local variables within the same
file and all global variables in all files. For more on global variables see below.
Local variables do not have to be uniquely named across all local variables in
other files. Local variables cannot share the same name as local labels (see
[Syntax and Grammar File](#syntax-and-grammar-sgrammar-file)) within the same file
or global labels in all files. Local variables must be declared like so:

`VariableName: BYTECOUNT`

Global variables are variables that must be uniquely named across all other global
variables and all local variables and all labels in all files. For example, if 
there is a global “count” variable in one file, no other file can have a global
or local “count” variable or label. Global variables must be declared like so:

`.global VariableName: BYTECOUNT`

The `VariableName` is the name of the variable (cannot include whitespaces) and
`BYTECOUNT` is the decimal representation of the amount of bytes allocated to the
variable.

Only `.global` variables can be referenced from any scope outside the assembly
file it is contained in. Labels behave like variables when it comes to their
naming rules and relationship with the `.global` pseudo operation.

It is best to have unique local, global, and label names across all files, as
this is how these elements are normally incorporated into assembly files.

All variables and labels will be replaced in assembly code by their address in
data and program memory respectively at assembly time.

#### Constant Literals and In-Line Expressions:

Constant literals are non-aliased values put into assembly code. By default,
these values are interpreted as decimal numbers, however, it may be prefixed in
the following ways:

* `DVALUE` - decimal representation

* `HVALUE` - hexadecimal representation

* `QVALUE` - octal representation

* `BVALUE` - binary representation

An example usage could be:

`ADD R1, B00000001 ; add 1 to Register 1`

The operators, `+`, `-`, `*`, `/`, `%`, `<<`, and `>>` are supported between any
combination of symbolic constants (See [Include Files](#include-files)), literals,
labels, and variable names.

### Include Files:

Include files[^3] are optional but must also be included in the specified input
folder (see [User Interface](#user-interface)) if they are referenced by assembly
files. Include files (.inc) are used to store symbolic constants. They can only
be referenced by .asm files through the .include pseudo-op. Comments in include
files can be declared by using the “;” character  and end with the end of line. 

#### Symbolic constants

Symbolic constants can only be declared inside an include (.inc) file. Symbolic
constants must be declared like so:

`.define NAME VALUE`

where `NAME` is the name of the symbol (no white spaces) and  `VALUE` is the
associated value. `VALUE` is interpreted like how a constant literal is (see
Constant literals) by Gena.

An example definition could be:

```
.define BINARY_ONE B00000001                ;binary form of 1
```

For assembly, whenever a string matching any constant is found in the
operands[^4] of an assembly file[^5], it will be replaced with its corresponding
symbolic value. Symbolic value names must be distinct across all include files.

## Syntax and Grammar (SGrammar) File:

A Syntax and Grammar (SGrammar) file (.txt) must be included in the input folder
and specified before assembly time (see [User Interface](#user-interface)). Gena
will come with an AVR SGrammer file, but if the user wishes to assemble assembly
code that does not conform to the AVR syntax and grammar, they must produce their
own SGrammer file that is compatible with the desired processor's syntax and grammar.  

In the first line of the SGrammar file, the user must define the syntax they wish
to write assembly in. A line of assembly can contain the following elements:

* Label

* OpName

* Operand

* Comment

Labels follow the same uniqueness rules as variable names and hold the same
relationship to the `.global` pseudo operation (see [Pseudo Operations](#pseudo-operations)).
Every element can stand alone except the Operand, which must always have an OpName
in the same line. Every element can exist in the same line with any number of the
other elements. Every element can only exist in a line once.

This line must also specify the order in which they wish to write the elements
and what delimiters to use between them. Delimiters must be distinct, and the
last element must be the Comment, which is delimited by the end of the line.  
The other elements could be arranged in any order. In this first line there cannot
be white space between any element and its following delimiter unless the delimiter
is a white space. So, `Label: OpName Operand; Comment`

 would be valid but

`Label : OpName Operand ; Comment`

is invalid. Lines of assembly must follow this order and delineation, although
they may not need to include all elements.

The next line must be formatted like so:

`Program Data Length: LENGTH`

where `LENGTH` is a non-variable length, in bits, of the desired processor’s
program data.

The line after that must be formatted like so:

`Data Data Size: SIZE`

where `SIZE` is the size, in bytes, of the desired processor’s data data memory. 

All other lines will then take the form of a Code Macro. A Code Macro will define
the grammar of the desired processor, which is how the line elements are to be
converted to machine code.

The Code Macro  is used to specify how the OpName and Operand can be converted
to machine code. The Code Macro is delimited by white spaces and in the following
order

* 1st string: the hex representation of the OpCode number

* 2nd string: OpName

* Next set of strings: combination of Symbols and Values

* Symbols will be represented as Sym followed by the character corresponding
  to it (e.g. SymR would represents the R in R16)

* Values will be represented as ViDes where i represents the i’th indexed
  value[^6] in the Code Macro[^7] and Des and signifies how the value will be
  interpreted (B for binary, D for decimal, H for hex, Q for octal)

* Following strings:  CODE string followed by strings representing  the order of
  the Value and OpCode bits within machine code.

* OPn represents the n’th OpCode bit

* Vnm represents the m’th bit of the n’th bit of the value.

Here is a Code Macro example . Suppose this is the assembly Code:

`ADD Rd, Rs`

And this is the corresponding machine code

`1011 DDDD SSSS`

where 1011 is the OpCode, ADD is the OpName and Rd, Rs, exist as the Operand which,
in this case, is a collection of symbols and values that can be mapped to DDDD SSSS
via the register number.

In code macro form  this would be:

`0B ADD SymR V0D Sym, SymR V1D CODE OP0 OP1 OP2 OP3 OP4 V00/ V01 V02 V03 V04 V10 V11 V12 V13`

In our example, `OP0 OP1 OP2 OP3 OP4 V00 V01 V02 V03 V04 V10 V11 V12 V13` means
that the 4 bits of the OpCode come first, followed by the 4 bits of the first value,
and then proceeded by the 4 bits of the second value. There can be any number of
bits for the OpCode and Values, and the ordering of them can be whatever, so long
as the number of bits used in the machine code portion is consistent with the size
of the Values and OpCode. Two values cannot be positioned next to each other without
being separated by a symbol. This structure can implement the instruction set of
any non-variable length digital processor[^8]. Gena uses this file while parsing
assembly files, to identify, on each line, the OpCode and if the Operand matches
the configuration of Symbols and Values for that OpCode, in which case the program
can use the end of the Code Macro to translate it to machine code.

## Outputs:

Upon successful completion, Gena will output a “[output_name] Assembled” message.
Gena outputs two files, whose names are specified on the command line, to a single
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

1; FolderName assembled by Gena with config file “config.txt”
2; Start of code:
3 0000       ; UpdateTimer:                ; Update the timer by 1s
4 0000 1A34  ; LDD R16, counter            ; retrieve the counter
5 0001 1C7F  ; ADIW R16, 1_SECOND          ; add 1 second


6 0010 261A  ; STS counter, R16            ; store the new value

```

## User Interface:

To use Gena the user should first put the provided “gena” executable in their
executable folder (on *Nix like environments, this is either in “/usr/local/bin”
or “/usr/bin”; on Windows this is in "%LocalAppData%\Programs” or any directory
in the %PATH% environment variable). Alternatively, users can just make symbolic
links to these directories. Now the user can run Gena by typing “gena” in the
parent directory of a folder that contains the files the user wishes to assemble
as well as the SGrammar file (see [Inputs](#inputs)) . Upon running the Gena program,
the user will be shown a list of commands and their possible arguments. Commands
not following this format will be ignored:

* `help`  

* the help command will display a scrollable screen that explains how to use
  Gena in the terminal. The interface is similar to a Linux man page.

* `asm [input_folder] [output_folder] [output_name] [sgrammar_name.txt]`

* the assemble command assembles all the files in the specified input folder
  with the SGrammar file specified and outputs an object file and listing file
  with the output name to a folder with the output folder name. The output folder
  will be located in the parent directory.

* By default, the AVR SGrammar file (avr_sgrammar.txt) is included with  Gena
  and must be copied into the input folder for usage. However, the user can
  create their  own SGrammer file and specify it in the input folder (see
  [Syntax and Grammar File](#syntax-and-grammar-sgrammar-file)).

* If the output folder does not exist, Gena will create it in the same directory.

* If the output file name already exists, Gena will overwrite it ONLY if it
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

## Error Handling:

All previous sections are required to understand this section.

When the user runs the asm command, there are two possible types of errors that
can occur: file finding errors and file syntax errors. When Gena encounters an
error, it immediately terminates the assembling process and does not write to
the target file if it already exists. Thus, if there are multiple syntax errors,
Gena will only report the first error it finds.

File finding errors occur when specified folders or files are not found. Possible
errors are:

* `Error - SGrammar [file name] file not found`

* the specified SGrammar file (see [Syntax and Grammar File](#syntax-and-grammar-sgrammar-file))
  is not found

* `Error - Folder [folder name] not found`

* the specified input folder is not found

* `Error - Main file not found in [folder name]`

* the specified folder does not include a main.s file in the folder specified

* `Error - File [file name] not found in include`

* the specified folder does not include the .inc or .asm file referenced by other
  files through the .include pseudo operation

File syntax errors occur when files have syntax errors. This could be:

* `Error - Style mismatch in file [file name] at line [line number]`

* the specified line of code in the specified file is not formatted as according
  to the style (first line) in the SGrammar file (see [Syntax and Grammar File]
  (#syntax-and-grammar-sgrammar-file))

* `Error - Duplicate local reference of [label/variable] in [file]`

* the specified line of code in the the specified file has a two local
  variables/global labels that are the same

* `Error - Duplicate global reference of [label/variable]`

* the code contains two local/global variables or labels that are the same name

---

[^1]: Syntax refers to the style in which the assembly is written, and grammar
refers to the interactions between the assembly code and the machine code as
defined by a processor’s instruction set and assembly manual.

[^2]: Identical delimiters appearing together in a sequence without any other
character between each delimiter.

[^3]: Not to be confused with the File inclusion pseudo operation.

[^4]: For more on exactly what “the operands” means see the [Assembly Files](#assembly-files) section.

[^5]: Not another include file.

[^6]: Zero indexed.

[^7]: As seen left to right.

[^8]: This is a very general statement. And it’s true. Although digital processors
can have a wide range of instruction sets, the symbol value structure is very
versatile and can be molded to any instruction set, sometimes requiring extra code
macros to support processor encodings.
