# Object Oriented Analysis

Joshua Archibald, Steven Lei, Christian Miranda

## asm_line

>### Description
>
>>This object implements an assembly line. This object contains the information
>>parsed from a `string` line of assembly from an assembly or include `file`.
>
>### Base Class
>
>>None.
>
>### Data Members
>
>* `origin_file_name` - Name of the file the line of assembly originally
>                       belonged to as a `string`.
>* `line_num` - The line number of the assembly line in its file as an `int`.
>* `text` - The assembly line as a text as a `string`.
>* `label` - The label in the assembly line as a`string`.
>* `op_name` - The operation name in the assembly line as a `string`.
>* `operand` - The operand in the assembly line as a `string`.
>
>### Constructors
>
>* `construct_asm_line`:
>   * **Description:** Parses a line of assembly and updates all data.
>   * **Arguments:** Line of assembly as a `string` and an `isa` object.
>
>### Destructors
>
>* `destruct_asm_line`
>
>### Methods
>
>>#### Accessors
>>
>>* `origin_file`
>>   * **Return Value:** Name of the assembly line's file of origin as a
>>                       `string`.
>>* `line_num`
>>   * **Return Value:** Name assembly line's line number in its file.
>>* `text`
>>   * **Return Value:** The assembly line as text as a `string`.
>>* `label`
>>   * **Return Value:** The assembly line's label as a `string`.
>>
>>#### Modifiers
>>
>>>None.
>>
>>#### General
>>
>>* `assemble`
>>   * **Description:** Provide the assembled program data corresponding to this
>>                      assembly line.
>>   * **Arguments:** `isa` object and `symbol_table` `string` to `int` `map`
>>                     object.
>>   * **Return Value:** The program data as an `int`.
>
>### Operators
>
>>None.
>
>### Type Conversions
>
>>None.
>
>### Error Handling
>
>>`construct_asm_line` will return `NULL` if a line of assembly is unable to be
>> parsed.
>
>### Helper Functions
>
>>None.

## isa (Instruction Set Architecture)

>### Description
>
>>This object implements an Instruction Set Architecture (ISA) object. This
>>object contains the relationship between operation names, operands, and
>>assembled program data. This object has the ability to translate `asm_line`
>>object information to program data.
>
>### Base Class
>
>>None.
>
>### Data Members
>
>* `code_map` - Maps operation names to `code_macro` objects. Type `map`.
>* `style` - List that holds the specified order of assembly line elements and
>            their delimiters. Type `string` `list`.
>
>### Constructors
>
>* `construct_isa`
>   * **Description:** Parses the ISA file and updates all data.
>   * **Arguments:** Name of ISA file as a `string`.
>
>### Destructors
>
>* `destruct_isa`
>
>### Methods
>
>>#### Accessors
>>
>>>None.
>>
>>#### Modifiers
>>
>>>None.
>>
>>#### General
>>
>>* `translate`
>>   * **Description:** Provides the program data corresponding to an operation
>>                      name and an operand.
>>   * **Arguments:** An operation name and an operand as `string` objects.
>>   * **Return Value:** The program data as an `int`.
>>
>>* `parse_asm`
>>   * **Description:** Provides an `asm_line` from a string of assembly.
>>   * **Arguments:** A line of assembly as a `string`.
>>   * **Return Value:** The line of assembly as an `asm_line`.
>
>### Operators
>
>>None.
>
>### Type Conversions
>
>>None.
>
>### Error Handling
>
>>`translate` will return `NULL` if the provided operation name and operand do
>> not have a corresponding piece of program data. `parse` will return `NULL` if
>> the provided assembly line cannot be parsed.
>
>### Helper Functions
>
>>None.

## code_macro

>### Description
>
>>This object implements a code macro. This object contains the relationship
>>between an operation code, an operand, and the assembled program data.
>
>### Base Class
>
>>None.
>
>### Data Members
>
>* `op_code` - `int` that represents a unique `op name`.
>* `operand_template` -  `string` template for how the operand should look.
>* `prog_data_template` - `string` template for how the program data should
>                          look.
>
>### Constructors
>
>* `construct_code_macro`
>   * **Description:** Constructs the code macro object.
>   * **Arguments:** Operand template and program data template as `string`
>                    objects and the operation code as an `int`.
>
>### Destructors
>
>* `destruct_code_macro`
>
>### Methods
>
>>#### Accessors
>>
>>>None.
>>
>>#### Modifiers
>>
>>>None.
>>
>>#### General
>>
>>* `prog_data`
>>   * **Description:** Provides the program data corresponding to an operand.
>>   * **Arguments:** An operand as a `string`.
>>   * **Return Value:** The program data as an `int`.
>
>### Operators
>
>>None.
>
>### Type Conversions
>
>>None.
>
>### Error Handling
>
>>`prog_data` will return `NULL` if the provided operand does not follow the
>>`operand_template`.
>
>### Helper Functions
>
>>None.

## assembler

>### Description
>
>>This object implements the assembler. This object can assemble assembly and
>>include `file` objects into machine code and listing `file` objects.
>
>### Base Class
>
>>None.
>
>### Data Members
>
>* `pc` - The program counter as an `int`.
>* `symbol_table` - Maps `labels`, including `data` `labels`, to their address
>                   in program memory as an `int`
>* `isa` - The `isa` object created from and ISA file.
>* `asm_files` - Ordered list of assembly and include files as a `list` of
>                `strings`.
>* `asm_prog` - Ordered `list` of `asm_line` objects.
>
>### Constructors
>
>* `construct_assembler`
>   * **Description:** Constructs the assembler object and the `isa` object
>                      contained in it. `asm_files` data is also updated.
>   * **Arguments:** Ordered list of files to be assembled as a `string` `list`
>                    and name of ISA file as a `string`.
>
>### Destructors
>
>* `destruct_assembler`
>
>### Methods
>
>>#### Accessors
>>
>>>None.
>>
>>#### Modifiers
>>
>>>None.
>>
>>#### General
>>
>>* `first_pass`
>>   * **Description:** Performs the first pass on the assembly and include
>>                      `file` objects. This populates the `symbol_table` and
>>                      `asm_prog` data.
>>   * **Arguments:** None.
>>   * **Return Value:** None.
>>
>>* `second_pass`
>>   * **Description:** Performs the second pass on the `asm_line` objects. This
>>                      assembles all the code and writes to machine code and
>>                      listing `file` objects.
>>   * **Arguments:** None.
>>   * **Return Value:** None.
>
>### Operators
>
>>None.
>
>### Type Conversions
>
>>None.
>
>### Error Handling
>
>>Displays the appropriate error message to the user if any method calls another
>>method that is unsuccessful. In this case the assembly process continues but
>>a machine code `file` object will not be generated.
>
>### Helper Functions
>
>>None.
