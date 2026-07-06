# 32-Bit MIPS ALU (Arithmetic Logic Unit) Simulator

## Overview
This project is a standalone, architectural software simulation of a 32-bit MIPS Arithmetic Logic Unit (ALU) written in C. It models physical processor hardware by accepting raw 32-bit hexadecimal machine instructions, decoding them into individual control fields via bit-masking, and routing the operands through functional combinational logic pathways to simulate real silicon execution.

## Key Architectural Features
* **Instruction Field Decoding Engine:** Simulates hardware instruction decoding pathways. It utilizes bitwise right-shift operators (`>>`) and hexadecimal bit-masks (`0x3F` and `0x1F`) to cleanly isolate the `opcode`, source registers (`rs`, `rt`), destination register (`rd`), shift amount (`shamt`), and function code (`funct`) from a single 32-bit instruction stream.
* **Hardware Register File Testbench:** Simulates the MIPS CPU register layout using a 32-element array. The testbench populates registers dynamically with signed positive and negative values while strictly enforcing the hardware constraint that Register 0 (`$zero`) remains hardwired to ground (`0`).
* **Hardware Exception Handling (Sign Overflow):** Explicitly models physical processor overflow trap flags. If an addition or subtraction operation violates signed integer boundaries (such as adding two positive values and yielding a negative result), the simulator detects the overflow exception, prints a warning flag, and gracefully isolates the execution trap.

## Supported R-Type Operations
The ALU execution unit fully handles core MIPS R-type functions based on their hardware function codes (`funct`):
* **Arithmetic Routing:** Signed Addition (`add`), Signed Subtraction (`sub`).
* **Logical Operations:** Bitwise AND (`and`), Bitwise OR (`or`).
* **Shifter Unit:** Shift Left Logical (`sll`), Shift Right Logical (`srl`) using programmatic execution shifts.
* **Conditional Flags:** Set Less Than (`slt`) for signed comparison tracking.

## How to Compile and Run
To compile this standalone hardware simulator using the `gcc` compiler:

```bash
gcc -Wall mips_alu_implementation.c -o alu_sim
