# x86-64
---
Also known as amd64 or i64 is a 64-bit Complex Instruction Set Computing (CISC) architecture. This basically means that the registers used for this architecture extend an extra 32-bits on Intel's x86 architecture. CISC means that a single instruction can do a bunch of different things at once such as memory accesses, register reads, etc. It  is also a variable-length instruction set which means different instructions can be different sizes ranging from1 to 16 bytes long. And finally x86-64 allows for multi-sized register access which means that you can access certain parts of a register which are different sizes.
### Registers
A key component of x86-64 registers is multi-sized access which means the register RAX can have its lower 32 bits accessed with EAX. The next lower 16 bits can be accessed with AX and the lowest 8 bits can be accessed with AL which allows for the computer to make optimizations which boost program execution.

*Important registers to note:*
-RIP: the instruction pointer
-RSP: the stack pointer
-RSB: the base pointer

---

## Instructions
An **Instruction** represents a single operation for the CPU to peform.

There are different types of instructions including:
* Data Movement: `mov rax, [rsp = 0x40]`
* Arithmetic: `add rbx, rcx`
* Control-flow: `jne 0x8000400`

---

## Control Flow

How can we express conditionals in x86-64? We use condition jumps such as:
* `jnz <address>`
*  `je <address>`
*  `jge <address>`
* etc.

They check the address and jump if their condition is true, otherwise going to the next instruction. These conditionals are checking EFLAGS which are special registers which store flags on certain instructions such as `add rax, rbx` which sets the o (overflow) flag if the sum is greater than a 64-bit register can hold, then wraps around. You can jump based on that with a `jo` instructin. The most important thing to remember is the cmp instruction:

```x86-64
cmp rax, rbx
jle error
```
*This assembly jumps if RAX <= RBX*
