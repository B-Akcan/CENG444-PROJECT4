using namespace std;

#include <iostream>
#include <fstream>
#include <string.h>
#include <vector>
#include <limits.h>

#include "dgevalsyn.tab.hh"
#include "MyParser.h"
#include "FlexLexer.h"
#include "MyFlexLexer.h"

// Important aspects for x64 Linux ABI
// - Stack is aligned at 16 byte boundaries
// - Shadow (spill) area is not required!
// - Six registers receive first 6 integral parameters RDI, RSI, RDX, RCX, R8, R9
// - xmm0 - xmm7 receive first 8 double parameters

int X64CodeBag::regs[4]={IREG_RDI, IREG_RSI, IREG_RDX, IREG_RCX};

X64CodeBag::X64CodeBag(DGEval *pDgEval)
{
   bagSize=DELTA;
   codeBase=(unsigned char *)malloc(DELTA);
   codeLen=0;
   dgEval=pDgEval;
}

X64CodeBag::~X64CodeBag()
{
   free(codeBase);
}

void X64CodeBag::emitBytes(int len, ...)
{
   va_list list;

   va_start(list, len);

   for (int i=0;i<len;i++)
   {
      unsigned char b=va_arg(list, int);
      emitCodeFrag(b);
      // emitCodeFrag((unsigned char)va_arg(list, unsigned char));
   }
   va_end(list);
}

void *X64CodeBag::createCodeBase()
{
   int      pageSize=getpagesize();
   size_t   pageCount=(codeLen+pageSize-1)/pageSize,
            allocSize=pageSize*pageCount;

   void *retVal=aligned_alloc(getpagesize(), allocSize);

   memmove(retVal, codeBase, codeLen);

   mprotect(retVal, allocSize, PROT_EXEC);

   return retVal;
}

#define _USE_ENTERLEAVE

void X64CodeBag::emitPrologue(int varCount)
{
   int varArea=varCount<<3;

   // Write prologue allocating stack space
#ifdef USE_ENTERLEAVE
   // Less instruction code, more execution cycles
   // This is the way enter implemented
   emitBytes(1, 0xc8);
   emitCodeFrag((uint16_t)varArea);     // completes above instruction
   emitBytes(1,0);
#else
   emitBytes(7,
         0x55,                // push rbp
         0x48, 0x89, 0xE5,    // move rbp, rsp
         0x48, 0x81, 0xEC     // sub rsp, varArea in part
         );
   emitCodeFrag(varArea);     // completes above instruction
#endif
   emitBytes(2, 0x41, 0x54);  // push r12 save r12 to use as stack balancer
}

void X64CodeBag::emitEpilogue()
{
   // Write epilogue deallocating stack space

   #ifdef USE_ENTERLEAVE
   emitBytes(7,
         0x41, 0x5C,          // pop r12
         0xC9,                // leave
         0xc3);               // ret
#else
   emitBytes(7,
         0x41, 0x5C,          // pop r12
         0x48, 0x89, 0xEC,    // mov rsp, rbp
         0x5d,                // pop rbp
         0xc3);               // ret
#endif

   //                            ; This is for exception unwinding return
   // 4889EC                     mov rsp, rbp
   // 5D                         pop rbp
   // C3                         ret
   unwindLocation=codeLen;
   setupImmediateIntegralPara(0, (uint64_t)dgEval);
   emitCall((void *)DGEval::postexecutecleanup);
   emitBytes(5,
         0x48, 0x89, 0xEC,
         0x5d,                // pop rbp
         0xc3);               // ret

   int cnt=(int)unwindFixups.size();
   for (int i=0;i<cnt;i++)
   {
      int f=unwindFixups[i];
      *(uint32_t*)(codeBase+f)=unwindLocation-f-4;
   }
}

void X64CodeBag::xmmArithInstruction(unsigned char cb)
{
   // F20F10442408                movsd xmm0, [rsp+8]
   // F20F580424                  addsd xmm0, [rsp]
   //     ^^ This is the critical byte (cb) that determines
   //        arithmetic operations. Sample above is add.

   emitBytes(11,
         0xF2, 0x0F, 0x10, 0x44, 0x24, 0x08,
         0xF2, 0x0F, cb, 0x04, 0x24);

   // 4883C408                    add rsp, 8
   // F20F110424                  movsd [rsp], xmm0
   emitBytes(9,
         0x48, 0x83, 0xC4, 0x08,
         0xF2, 0x0F, 0x11, 0x04, 0x24);
}

void X64CodeBag::comparisonInstruction(DGTypeDesc td, unsigned char cb)
{

   if (td.dim==0)
   {
      switch (td.type)
      {
         case DGEvalType::DGNumber:
            // 4831C9                     xor rcx, rcx
            emitBytes(3, 0x48, 0x31, 0xC9);

            // 4883C410                   add rsp, 16
            // F20F104424F8               movsd xmm0, [rsp-8]
            // 660F2F4424F0               comisd xmm0, [rsp-16]

            emitBytes(16,
                  0x48, 0x83, 0xC4, 0x10,
                  0xF2, 0x0F, 0x10, 0x44, 0x24, 0xF8,
                  0x66, 0x0F, 0x2F, 0x44, 0x24, 0xF0
            );

            // The following is to sanitize double precision comparison instruction
            // Example: jle -> jbe. Effectively swapping bit 1 and bit 2 for gt, gte, lt, lte.
            // A readable version could be coded by switch or if :)
            if (cb>0x78)
               cb=((cb << 1) & 4) | 2 | (cb & (unsigned char)0xf1);
            break;
         case DGEvalType::DGBoolean:
            // 4831C9                     xor rcx, rcx
            emitBytes(3, 0x48, 0x31, 0xC9);
            // 5F                         pop rdi
            // 58                         pop rax
            // 4839F8                     cmp rax, rdi
            emitBytes(5,
            0x5f,
            0x58,
            0x48, 0x39, 0xF8);
            break;
         case DGEvalType::DGString:
            setupParameter(1, false);
            setupParameter(0, false);
            emitCall((void *)DGEval::strcmp);
            // 4831C9                     xor rcx, rcx
            emitBytes(3, 0x48, 0x31, 0xC9);
            // cmp rax, 0
            emitBytes(4, 0x48, 0x83, 0xF8, 0x00);
            break;
         default:
            break;
      }
   }
   else
   {
      setupParameter(1, false);
      setupParameter(0, false);
      emitCall((void *)DGEval::arrcmp);
      // 4831C9                     xor rcx, rcx
      emitBytes(3, 0x48, 0x31, 0xC9);
      // cmp rax, 0
      emitBytes(4, 0x48, 0x83, 0xF8, 0x00);
   }

   // ; sample for gt
   // 7E03                       jle l1
   // 48FFC1                     inc rcx
   //                        l1:
   // 51                         push ecx

   emitBytes(6, cb, 0x03, 0x48, 0xFF, 0xC1, 0x51);
}

void X64CodeBag::emitCall(void *callAddr)
{
   // 0000003E 48B800000000040000-        mov rax, 0x400000000    ; Example call address is 0x400000000
   // 00000047 00
   // 00000048 4989E4                     mov r12, rsp            ; r12 is saved across calls
   // 0000004B 4883E4F0                   and rsp, 0fffffff0h
   // 0000004E FFD0                       call rax
   // // 00000050 50                         push rax                ; Return value is in rax
   // 00000051 4C89E4                     mov rsp, r12


   //4883E4F0

   emitBytes(2, 0x48, 0xB8);
   emitCodeFrag(callAddr);
   emitBytes(12,  0x49, 0x89, 0xE4,
                  0x48, 0x83, 0xE4, 0xF0,
                  0xFF, 0xD0,
                  //0x50,
                  0x4C, 0x89, 0xE4);
}

// This is good enough up to 4 integral, and 8 double parameters
void X64CodeBag::setupParameter(int ndx, bool isDouble)
{
   emitBytes(1, 0x58); // pop rax

   if (isDouble)
   {
      unsigned char cb=0xc0+8*ndx;

      emitBytes(5, 0x66, 0x48, 0x0F, 0x6E, cb);
   }
   else
   {
      emitBytes(3, 0x48, 0x89, 0xC0+regs[ndx]);
   }

   /* This commented block is for direct preparation from stack, which is not needed for the assignment
   unsigned char o=8*ndx;

   if (isDouble)
   {
      if (ndx==0)
         emitBytes(5, 0xF2, 0x0F, 0x10, 0x04, 0x24);
      else
      {
         unsigned char cb=0x3C+8*ndx;
         emitBytes(0xF2, 0x0F, 0x10, cb, 0x24, o);
      }
   }
   else
   {
      if (ndx==0)
      {
         emitBytes(4, 0x48, 0x8B, 0x3C, 0x24);
      }
      else
      {
         unsigned char cb=0x44+8*regs[ndx];

         emitBytes(5, 0x48, 0x8B, cb, 0x24, o);
      }
   }
   */
}

void X64CodeBag::setupImmediateIntegralPara(int ndx, uint64_t p)
{
   unsigned char cb=0xB8 + regs[ndx];
   emitBytes(2, 0x48, cb);
   emitCodeFrag(p);
}

void X64CodeBag::setupImmediateDoublePara(int ndx, double p)
{
   unsigned char cb=0xC0 + 8*regs[ndx];

   emitBytes(2, 0x48, 0xB8);
   emitCodeFrag(p);

   emitBytes(0x66, 0x48, 0x0F, 0x6E, cb);
}

void X64CodeBag::placeResultOnStack(bool isDouble)
{
   if (isDouble)
      emitBytes(5, 0x66, 0x48, 0x0F, 0x7E, 0xC0); // movq rax, xmm0

   emitBytes(1, 0x50); // push rax
}

void X64CodeBag::translateInstruction(DGEvalICInst *inst)
{
   int      count=(int)inst->numConstant,
            ic=0,
            dc=0;
   uint64_t p;

   inst->codeOffset=codeLen;
   switch (inst->opCode)
   {
      case CONST: {
         // Push constant value onto stack
         if (inst->type.dim == 0) {
            switch (inst->type.type) {
               case DGEvalType::DGNumber:
                  // Push double constant
                  emitBytes(2, 0x48, 0xB8); // mov rax, value
                  emitCodeFrag(inst->numConstant);
                  emitBytes(1, 0x50); // push rax
                  break;
               case DGEvalType::DGBoolean:
                  // Push boolean constant
                  emitBytes(2, 0x48, 0xB8); // mov rax, value
                  emitCodeFrag((uint64_t)inst->boolConstant);
                  emitBytes(1, 0x50); // push rax
                  break;
               case DGEvalType::DGString:
                  // String constants are handled via LRT
                  break;
            }
         }
         break;
      }
      
      case OP_ADD: {
         if (inst->type.dim == 0 && inst->type.type == DGEvalType::DGNumber) {
            // Double addition
            xmmArithInstruction(0x58); // addsd
         } else {
            // Integer addition
            emitBytes(5, 0x58, 0x5A, 0x48, 0x01, 0xD0); // pop rax, pop rdx, add rax, rdx
            emitBytes(1, 0x50); // push rax
         }
         break;
      }
      
      case OP_SUB: {
         if (inst->type.dim == 0 && inst->type.type == DGEvalType::DGNumber) {
            // Double subtraction
            xmmArithInstruction(0x5C); // subsd
         } else {
            // Integer subtraction
            emitBytes(5, 0x5A, 0x58, 0x48, 0x29, 0xC2); // pop rdx, pop rax, sub rdx, rax
            emitBytes(3, 0x48, 0x89, 0xD0); // mov rax, rdx
            emitBytes(1, 0x50); // push rax
         }
         break;
      }
      
      case OP_MUL: {
         if (inst->type.dim == 0 && inst->type.type == DGEvalType::DGNumber) {
            // Double multiplication
            xmmArithInstruction(0x59); // mulsd
         } else {
            // Integer multiplication
            emitBytes(5, 0x58, 0x5A, 0x48, 0xF7, 0xEA); // pop rax, pop rdx, imul rdx
            emitBytes(1, 0x50); // push rax
         }
         break;
      }
      
      case OP_DIV: {
         if (inst->type.dim == 0 && inst->type.type == DGEvalType::DGNumber) {
            // Double division
            xmmArithInstruction(0x5E); // divsd
         } else {
            // Integer division
            emitBytes(5, 0x5A, 0x58, 0x48, 0xF7, 0xFA); // pop rdx, pop rax, idiv rdx
            emitBytes(1, 0x50); // push rax
         }
         break;
      }
      
      case OP_MINUS: {
         if (inst->type.dim == 0 && inst->type.type == DGEvalType::DGNumber) {
            // Double negation
            emitBytes(11, 0xF2, 0x0F, 0x10, 0x04, 0x24, // movsd xmm0, [rsp]
                     0xF2, 0x0F, 0x57, 0xC0, 0xF2, 0x0F, 0x5C); // xorpd xmm0, xmm0, subsd xmm0, xmm0
            emitBytes(5, 0x66, 0x48, 0x0F, 0x7E, 0xC0); // movq rax, xmm0
            emitBytes(1, 0x50); // push rax
         } else {
            // Integer negation
            emitBytes(4, 0x58, 0x48, 0xF7, 0xD8); // pop rax, neg rax
            emitBytes(1, 0x50); // push rax
         }
         break;
      }
      
      case OP_NOT: {
         // Boolean negation
         emitBytes(4, 0x58, 0x48, 0x83, 0xF0); // pop rax, xor rax, 1
         emitBytes(1, 0x01);
         emitBytes(1, 0x50); // push rax
         break;
      }
      
      case OP_BAND: {
         // Boolean AND
         emitBytes(5, 0x58, 0x5A, 0x48, 0x21, 0xD0); // pop rax, pop rdx, and rax, rdx
         emitBytes(1, 0x50); // push rax
         break;
      }
      
      case OP_BOR: {
         // Boolean OR
         emitBytes(5, 0x58, 0x5A, 0x48, 0x09, 0xD0); // pop rax, pop rdx, or rax, rdx
         emitBytes(1, 0x50); // push rax
         break;
      }
      
      case OP_EQ:
      case OP_NEQ:
      case OP_LT:
      case OP_LTE:
      case OP_GT:
      case OP_GTE: {
         // Comparison operations
         unsigned char jumpOp;
         switch (inst->opCode) {
            case OP_EQ: jumpOp = 0x74; break;  // je
            case OP_NEQ: jumpOp = 0x75; break; // jne
            case OP_LT: jumpOp = 0x7C; break;  // jl
            case OP_LTE: jumpOp = 0x7E; break; // jle
            case OP_GT: jumpOp = 0x7F; break;  // jg
            case OP_GTE: jumpOp = 0x7D; break; // jge
            default: jumpOp = 0x74; break;
         }
         comparisonInstruction(inst->type, jumpOp);
         break;
      }
      
      case OP_ASSIGN: {
         // Assignment operation
         // Store value from stack to variable
         emitBytes(1, 0x58); // pop rax
         // Store in variable (symbol table lookup would be needed here)
         // For now, just push the value back
         emitBytes(1, 0x50); // push rax
         break;
      }
      
      case INSID: {
         // Load variable value onto stack
         // Variable lookup would be needed here
         // For now, push a placeholder value
         emitBytes(2, 0x48, 0xB8); // mov rax, 0
         emitCodeFrag((uint64_t)0);
         emitBytes(1, 0x50); // push rax
         break;
      }
      
      case POP: {
         // Pop specified number of values from stack
         for (int i = 0; i < inst->p1; i++) {
            emitBytes(1, 0x58); // pop rax
         }
         break;
      }
      
      case JMP: {
         // Unconditional jump
         emitBytes(1, 0xE9); // jmp
         // Jump target would need to be resolved
         emitCodeFrag((uint32_t)0); // placeholder offset
         break;
      }
      
      case JF: {
         // Jump if false
         emitBytes(1, 0x58); // pop rax
         emitBytes(3, 0x48, 0x85, 0xC0); // test rax, rax
         emitBytes(1, 0x74); // je
         // Jump target would need to be resolved
         emitCodeFrag((uint8_t)0); // placeholder offset
         break;
      }
      
      case JT: {
         // Jump if true
         emitBytes(1, 0x58); // pop rax
         emitBytes(3, 0x48, 0x85, 0xC0); // test rax, rax
         emitBytes(1, 0x75); // jne
         // Jump target would need to be resolved
         emitCodeFrag((uint8_t)0); // placeholder offset
         break;
      }
      
      case OP_CALL:
         DGEvalFuncDesc   *fd;
         fd=dgEval->libFunctionAt((int)inst->numConstant);

         for (int i=fd->paraCount;i>0;i--)
            if (fd->types[i].dim==0 && fd->types[i].type==DGEvalType::DGNumber)
               dc++;
            else
               ic++;

         if (fd->requiresDGEval)
            ic++;

         for (int i=fd->paraCount;i>0;i--)
         {
            bool isDouble=fd->types[i].dim==0 && fd->types[i].type==DGEvalType::DGNumber;

            setupParameter(isDouble?(--dc):(--ic), isDouble);
         }

         if (fd->requiresDGEval)
            setupImmediateIntegralPara(0, (uint64_t)dgEval);

         emitCall(fd->f);

         placeResultOnStack(fd->types[0].dim==0 && fd->types[0].type==DGEvalType::DGNumber);
         break;
      case LRT:
         switch (inst->p1)
         {
            case 0:
               // Prepare the 4th parameter / Elements in reverse order
               // mov rcx, rsp
               emitBytes(3, 0x48, 0x89, 0xE1);
               p=*(uint64_t *)(&inst->type);

               setupImmediateIntegralPara(2, count);
               setupImmediateIntegralPara(1, p);
               setupImmediateIntegralPara(0, (uint64_t)dgEval);

               emitCall((void *)DGEval::allocatearray);

               // Clean up array elements on stack 0x40000000 is a dummy for instruction synthesis
               // 4881C400000040             add rsp, 0x40000000
               emitBytes(3, 0x48, 0x81, 0xC4);
               emitCodeFrag((uint32_t)(count*8));

               placeResultOnStack(inst->type.type==DGEvalType::DGNumber && inst->type.dim==0);
               break;
            case 1:
               setupParameter(0, true);
               // Convert the 2nd parameter to int64_t cvtsd2si rdx, xmm0
               emitBytes(5, 0xF2, 0x48, 0x0F, 0x2D, 0xD0);

               setupParameter(1, false);
               setupImmediateIntegralPara(0, (uint64_t)dgEval);
               emitCall((void *)DGEval::arrayelement);
               placeResultOnStack(inst->type.type==DGEvalType::DGNumber && inst->type.dim==0);

               setupImmediateIntegralPara(0, (uint64_t)dgEval);
               emitCall((void *)DGEval::checkexception);
               // or rax, rax
               emitBytes(3, 0x48, 0x09, 0xC0);
               // 0F 85 cd is JNE with 32 bit cd offset
               unwindFixups.push_back(codeLen+2);
               emitBytes(6, 0x0f, 0x85, 0, 0 ,0 ,0);
               break;
            case 2:
               setupParameter(1, false);
               setupParameter(0, false);
               emitCall((void *)DGEval::appendelement);
               placeResultOnStack(false);
               break;
            case 3:
               setupImmediateIntegralPara(1, (uint64_t)inst->strConstant);
               setupImmediateIntegralPara(0, (uint64_t)dgEval);
               emitCall((void *)DGEval::allocatestring);
               placeResultOnStack(false);
               break;
            case 4:
               setupParameter(2, false);
               setupParameter(1, false);
               setupImmediateIntegralPara(0, (uint64_t)dgEval);
               emitCall((void *)DGEval::catstring);
               placeResultOnStack(false);
               break;
            case 5:
               setupParameter(0, true);
               setupImmediateIntegralPara(0, (uint64_t)dgEval);
               emitCall((void *)DGEval::number2str);
               placeResultOnStack(false);
               break;
            case 6:
               setupImmediateIntegralPara(2, (int64_t)inst->numConstant);
               setupParameter(1, false);
               setupParameter(0, false);
               emitCall((void *)DGEval::strcmp);
               placeResultOnStack(false);
               break;
            case 7:
               setupParameter(1, false);
               setupParameter(0, false);
               emitCall((void *)DGEval::arrcmp);
               placeResultOnStack(false);
               break;
            case 8:
               setupImmediateIntegralPara(0, (uint64_t)dgEval);
               emitCall((void *)DGEval::postexecutecleanup);
               break;
         }
   }
}


