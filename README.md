

# Virtual Machine

This project is a simple **Virtual Machine (VM)** designed to execute files written in **P-Code-like** intermediate code. The VM interprets p-code instructions and simulates the execution of the program, providing insights into the computation process of an abstract machine.

## Features

- Executes **P-Code-like** instructions in a simulated environment.
- Supports basic operations such as **arithmetic**, **logic** and **function calls**.
- Allows step-by-step execution for debugging and educational purposes.
- Provides a **stack-based architecture** similar to the p-code used in early compilers like Pascal.

## Requirements

- A valid **P-Code-like file** (intermediate code in the form of simple instructions).

## Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/yourusername/p-code-vm.git
   cd p-code-vm
   ```

2. **Compile the C source code**:
   ```bash
   make
   ```

3. **Run the Virtual Machine**:
   Ensure you have a p-code file ready (e.g., `program.pcode`), and run the VM with the following command:
   ```bash
   ./ program.txt
   ```

## Usage

The virtual machine will interpret a p-code file and execute the corresponding instructions. The file should consist of a series of p-code operations, each corresponding to a specific instruction.

### Example

```text
LOAD 5       // Load value 5 onto the stack
LOCI 5       // Load value  onto the stack
ADDI         // Add the two top  value of the stack
STOR 1      // Store the result in variable 1
WRIT 1      // Output the value of variable 1
HALT         // Stop execution
```

### Instruction Set


- **ADDI**  
  Replaces the two integers on `ostack` with the result of their sum.  

- **ADDR**  
  Replaces the two real numbers on `ostack` with the result of their sum.  

- **APOP**  
  Pops `astack` (i.e., decrements `ap--`).  

- **CARD**  
  Replaces the array on top of `ostack` with its cardinality.  

- **CIDX**  
  Checks whether the index on top of `ostack` is within the range `[0..num]`, where `num` is the cardinality of the array below the top of `ostack`. If not, prints an error message and terminates program execution.  

- **CODE size**  
  Allocates `prog` (code memory) with a size of `size` (number of subsequent N-code instructions in the file).  

- **DIVI**  
  Replaces the two integers on `ostack` with the result (integer) of their division.  

- **DIVR**  
  Replaces the two real numbers on `ostack` with the result (real) of their division.  

- **EMPT**  
  Replaces the array on top of `ostack` with a boolean value (integer) indicating whether the array is empty.  

- **EQUA**  
  Replaces the two objects on `ostack` with the boolean value (integer) resulting from the equality operation.  

- **FUNC fid**  
  Instruction that is not executed: serves as a syntactic frame for function definition in the N-code program.  

- **GEQI**  
  Replaces the two integers on `ostack` with the boolean value (integer) resulting from the `≥` comparison.  

- **GEQR**  
  Replaces the two real numbers on `ostack` with the boolean value (integer) resulting from the `≥` comparison.  

- **GEQS**  
  Replaces the two strings (pointers) on `ostack` with the boolean value (integer) resulting from the `≥` comparison.  

- **GTHI**  
  Replaces the two integers on `ostack` with the boolean value (integer) resulting from the `>` comparison.  

- **GTHR**  
  Replaces the two real numbers on `ostack` with the boolean value (integer) resulting from the `>` comparison.  

- **GTHS**  
  Replaces the two strings (pointers) on `ostack` with the boolean value (integer) resulting from the `>` comparison.  

- **HALT**  
  Stops the execution of the program.  

- **HEAD**  
  Replaces the array on top of `ostack` with its first element (head). If the array is empty, prints an error message and terminates execution.  

- **INDL offset size**  
  Replaces the object on top of `ostack` (with an `addr` field) with an object whose value (of size `size`) is a copy of the value starting at `addr + offset`. Sets `size = size` and `num = 1` in the descriptor of the resulting object.  

- **IXAD scale**  
  Pops an integer value `val` (array index or field offset) from `ostack` and increments the `addr` of the underlying object (now on top of `ostack`): `addr += val * scale` (the `size` and `num` fields in the descriptor are not relevant).  

- **JUMP entry**  
  Updates the program counter: `pc = entry`.  

- **LEQI**  
  Replaces the two integers on `ostack` with the boolean value (integer) resulting from the `≤` comparison.  

- **LEQR**  
  Replaces the two real numbers on `ostack` with the boolean value (integer) resulting from the `≤` comparison.  

- **LEQS**  
  Replaces the two strings (pointers) on `ostack` with the boolean value (integer) resulting from the `≤` comparison.  

- **LOAD envoid**  
  Loads onto `ostack` the value of the object identified by `oid` in the environment (a variable if `env = 0`, a parameter if `env = 1`).  

- **LOCI intconst**  
  Loads an integer with value `intconst` onto `ostack`.  

- **LOCR realconst**  
  Loads a real number with value `realconst` onto `ostack`.  

- **LOCS strconst**  
  Loads onto `ostack` a pointer (unique for identical strings) to a string with value `strconst`.  

- **LODA env oid**  
  Loads onto `ostack` a copy of the descriptor of the object identified by `oid` in the environment (a variable if `env = 0`, a parameter if `env = 1`): all fields of the descriptor copy (including `addr`) retain the same value.  

- **LTHI**  
  Replaces the two integers on `ostack` with the boolean value (integer) resulting from the `<` comparison.  

- **LTHR**  
  Replaces the two real numbers on `ostack` with the boolean value (integer) resulting from the `<` comparison.  

- **LTHS**  
  Replaces the two strings (pointers) on `ostack` with the boolean value (integer) resulting from the `<` comparison.

- **MEMB**  
  Replaces the array on top of `ostack` and the element below it with a boolean value (integer) indicating whether the element belongs to the array.  

- **MULI**  
  Replaces the two integers on `ostack` with the result of their multiplication.  

- **MULR**  
  Replaces the two real numbers on `ostack` with the result of their multiplication.  

- **NEGB**  
  Replaces the boolean value (integer) on top of `ostack` with its negated value (0 becomes 1, and 1 becomes 0).  

- **NEGI**  
  Replaces the integer number `num` on top of `ostack` with `-num` (sign change).  

- **NEGR**  
  Replaces the real number `num` on top of `ostack` with `-num` (sign change).  

- **NEQU**  
  Replaces the two objects on `ostack` with the boolean value (integer) resulting from the inequality operation.  

- **NEWO size num**  
  Allocates an object in `vars[vp++]` where: `size = size`, `num = num`. If `num > 0`, allocates a byte string of length `size * num` in the heap and assigns its pointer to the `addr` field of the descriptor; otherwise (if `num = 0`), `addr = NULL`.  

- **PACK num size card**  
  Replaces `num` objects on `ostack` with a single object whose value (pointed to by `addr`) is the concatenation of the values of the original objects (record fields or array elements), setting `size = size` and `num = card` in the descriptor of the resulting object.  

- **PUSH n**  
  Pushes a new `RA` onto `astack`, where: `num = n`, `objs = op - n`, `ret = pc + 1`.  

- **READ oid format**  
  Assigns the value of variable `oid` by dynamically reading it from standard input in the given `format`.  

- **RETN**  
  Replaces the current function's actual parameters (below the top of `ostack`) in `astack` with the object on top of `ostack`; then updates `pc = astack[ap - 1].ret`.  

- **SKIP offset**  
  Updates `pc += offset - 1`.  

- **SKPF offset**  
  If the boolean value (integer) on top of `ostack` is `0`, updates `pc += offset - 1`; then, in any case, pops the boolean value (integer) from the top of `ostack`.    

- **STOR**  
  Assigns the value at the `addr` of the object below the top of `ostack` with the value of the object on top of `ostack`; then pops both objects from `ostack`. If the size (`num * size`) of the value being assigned differs from that of the object below the top of `ostack` (in the case of an array), the old value (pointed to by `addr`) is deallocated, and a new value is allocated with a number of bytes equal to the size of the value of the object on top of `ostack`.  

- **SUBI**  
  Replaces the two integers on `ostack` with their difference.  

- **SUBR**  
  Replaces the two real numbers on `ostack` with their difference.  

- **TAIL**  
  Replaces the array on top of `ostack` with its tail (all elements after the head). If the array is empty, prints an error message and terminates execution.  

- **TOIN**  
  Replaces the real number on top of `ostack` with the integer resulting from the cast operation.  

- **TORE**  
  Replaces the integer on top of `ostack` with the real number resulting from the cast operation.  

- **VARS num**  
  Allocates `vars` (global variable memory) with a size of `num`.  

- **WRIT format**  
  Prints to standard output, in the given `format`, the instance of the object on top of `ostack`; then pops `ostack`.  


### Example P-Code File (program.pcode)

```text
LODA 0 1     // load the address of variable with oid 1
LOAD 0 10      // Load 10 onto the stack
LOAD 0 20      // Load 20 onto the stack
ADDI        // Add top two values (10 + 20)
STOR      // Store result (30) in variable 1
LOAD 0 1       // Load variable 1 onto the stack
WRIT i     // Print the value of integer variable 1 (30)
HALT         // Stop execution
```

### Output

The program will output:
```bash
30
```

## How it Works

1. The VM reads the p-code file and parses each instruction.
2. Each instruction is executed in a loop until the **HALT** instruction is encountered.
3. The stack is used to store intermediate values and results.
4. The VM provides feedback on the execution process, such as printing values when instructed.

## Contributing

Feel free to fork the repository, submit issues, or contribute improvements! Contributions are welcome, especially if you want to extend the instruction set or add new features to the VM.
