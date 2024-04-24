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
>* `text` - The assembly line as a text as a`string`.
>* `label` - The label in the assembly line as a`string`.
>* `op_name` - The operation name in the assembly line as a`string`.
>* `operand` - The operand in the assembly line as a`string`.
>
>### Constructors
>
>* `construct_asm_line`
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
>>   * **Arguments:** `isa` object and `Symbol_table`.
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
>>assembled program data and thus the ability to translate `asm_line` objects to
>>program data.  
>
>### Base Class
>
>>None.
>
>### Data Members
>
>* `code_map` - Maps operation names to `code_macro` objects. Type `map`.
>
>### Constructors
>
>* `construct_isa`
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
>> not have a corresponding piece of program data.
>
>### Helper Functions
>
>>None.
