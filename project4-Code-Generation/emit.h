///////////////////////////////////////////////////////////////////////
//
//  Assembly Code Naming Conventions:
//
//     Dispatch table            <classname>_dispTab
//     Method entry point        <classname>.<method>
//     Class init code           <classname>_init
//     Abort method entry        <classname>.<method>.Abort
//     Prototype object          <classname>_protObj
//     Integer constant          int_const<Symbol>
//     String constant           str_const<Symbol>
//
///////////////////////////////////////////////////////////////////////

#include "stringtab.h"

#define MAXINT  100000000    
#define WORD_SIZE    4
#define LOG_WORD_SIZE 2     // for logical shifts

// Global names
#define GLOBAL                  "\t.globl\t"


// Symbol type
#define SYMBOL_TYPE             "\t.type\t"
#define INTTAG                  "\t.quad\t"
#define STRINGTAG               "\t.string\t"
#define FLOATTAG                "\t.long\t"
#define BOOLTAG                 "\t.long\t"
#define ALIGN                   "\t.align\t"

// comma
#define COMMA                   ", "

// Sections
#define SECTION                 "\t.section\t"
#define TEXT                    "\t.text\t"
#define RODATA                  "\t.rodata\t"
#define DATA                    "\t.data\t"
#define OBJECT                  "@object"
#define FUNCTION                "@function"
#define SIZE                    "\t.size\t"

// Prefixs
#define STRINGCONST_PREFIX      ".LC"
#define FLOATCONST_PREFIX       ".FL"
#define POSITION                ".POS"
//
// register names
//
#define RAX     "%rax"		// rax general purpose register
#define RBX     "%rbx"		// rbx general purpose register
#define RCX     "%rcx"		// rcx general purpose register
#define RDX     "%rdx"		// rdx general purpose register
#define RDI     "%rdi"		// rdi general purpose register
#define RSI     "%rsi"		// rsi general purpose register
#define R8      "%r8"		// r8 general purpose register
#define R9      "%r9"		// r9 general purpose register
#define R10     "%r10"		// r10 general purpose register
#define R11     "%r11"		// r11 general purpose register
#define R12     "%r12"		// r12 general purpose register
#define R13     "%r13"		// r13 general purpose register
#define R14     "%r14"		// r14 general purpose register
#define R15     "%r15"		// r15 general purpose register
#define RBP     "%rbp"		// rbp stack bottom
#define RSP     "%rsp"		// rsp stack top
#define RIP     "%rip"		// pc register
#define XMM0    "%xmm0"		// float register
#define XMM1    "%xmm1"     // float register
#define XMM2    "%xmm2"     // float register
#define XMM3    "%xmm3"     // float register
#define XMM4    "%xmm4"     // float register
#define XMM5    "%xmm5"     // float register
#define XMM6    "%xmm6"     // float register
#define XMM7    "%xmm7"     // float register

//
// Opcodes
// int
#define MOV     "\tmovq\t"  

#define CALL    "\tcall\t"
#define RET     "\tret\t"
#define LEAVE   "\tleave\t"
#define POP     "\tpopq\t"
#define PUSH    "\tpushq\t"

#define ADD     "\taddq\t"
#define NEG     "\tnegq\t"
#define SUB     "\tsubq\t"     
#define DIV     "\tidivq\t"
#define CQTO    "\tcqto\t"
#define MUL     "\timulq\t"
#define AND     "\tandq\t"
#define OR      "\torq\t"
#define NOT     "\tnotq\t"
#define XOR     "\txorq\t"
#define CMP     "\tcmpq\t"
#define JMP     "\tjmp\t"
#define JL      "\tjl\t"
#define JLE     "\tjle\t"
#define JE      "\tje\t"
#define JNE     "\tjne\t"
#define JG      "\tjg\t"
#define JGE     "\tjge\t"
#define TEST    "\ttestq\t"
#define JZ      "\tjz\t"
#define JNZ     "\tjnz\t"
// float
#define MOVSD   "\tmovsd\t" 

#define ADDSD    "\taddsd\t"
#define SUBSD    "\tsubsd\t"
#define MULSD    "\tmulsd\t"
#define DIVSD    "\tdivsd\t"
#define XORPD    "\txorpd\t"

#define UCOMPISD "\tucomisd\t"
#define JB      "\tjb\t"
#define JBE     "\tjbe\t"
#define JA      "\tja\t"
#define JAE     "\tjae\t"
#define JP      "\tjp\t"
#define JP      "\tjp\t"

// transform from float to int
#define CVTTSD2SIQ "\tcvttsd2siq\t"
#define CVTSI2SDQ "\tcvtsi2sdq\t"

// convert xmm to xmm
#define MOVAPS "\tmovaps\t"

// printf
#define MOVL     "\tmovl\t" 
#define EAX     "%eax"      // 32 bit general purpose register