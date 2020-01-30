#include <llvm-c/Analysis.h> 
#include <llvm-c/BitReader.h> 
#include <llvm-c/BitWriter.h> 
#include <llvm-c/Comdat.h> 
#include <llvm-c/Core.h> 
#include <llvm-c/DataTypes.h> 
#include <llvm-c/DebugInfo.h> 
#include <llvm-c/Disassembler.h> 
#include <llvm-c/DisassemblerTypes.h> 
#include <llvm-c/Error.h> 
#include <llvm-c/ErrorHandling.h> 
#include <llvm-c/ExecutionEngine.h> 
#include <llvm-c/Initialization.h> 
#include <llvm-c/IRReader.h> 
#include <llvm-c/Linker.h> 
#include <llvm-c/LinkTimeOptimizer.h> 
#include <llvm-c/lto.h> 
#include <llvm-c/Object.h> 
#include <llvm-c/OrcBindings.h> 
#include <llvm-c/Remarks.h> 
#include <llvm-c/Support.h> 
#include <llvm-c/Target.h> 
#include <llvm-c/TargetMachine.h> 
#include <llvm-c/Types.h>


#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>

#include "ll.h"
#include "ast.h"
#include "semantic.h"

void create_printf_function (LLVMModuleRef mod) {
  LLVMTypeRef printf_args[] = { LLVMPointerType(LLVMInt8Type(), 0) };
  LLVMTypeRef printf_ty = LLVMFunctionType(LLVMInt32Type(), printf_args, 0, 1);
  LLVMValueRef printf = LLVMAddFunction(mod, "printf", printf_ty);
}

void create_pow_function (LLVMModuleRef mod) {
  LLVMTypeRef pow_args[] = {LLVMDoubleType(), LLVMDoubleType()};
  LLVMTypeRef pow_ty = LLVMFunctionType(LLVMDoubleType(), pow_args, 2, 0);
  LLVMValueRef pow = LLVMAddFunction(mod, "pow", pow_ty);
}

void call_printf (LLVMModuleRef mod, LLVMBuilderRef builder, LLVMValueRef exp) {

  LLVMTypeRef type = LLVMTypeOf(exp);
  LLVMValueRef format;
  if (type == LLVMInt32Type()) {
    format = LLVMBuildGlobalStringPtr(builder, "%d\n", "format_int");
  } else if (type == LLVMDoubleType()) {
    format = LLVMBuildGlobalStringPtr(builder, "%f\n", "format_float");
  }

  LLVMValueRef args[] = { format, exp };
  LLVMValueRef printf = LLVMGetNamedFunction(mod, "printf");
  LLVMBuildCall(builder, printf, args, 2, "printf");
}

LLVMValueRef call_pow (LLVMModuleRef mod, LLVMBuilderRef builder, LLVMValueRef x, LLVMValueRef y) {
  LLVMValueRef args[] = { x, y };
  LLVMValueRef pow = LLVMGetNamedFunction(mod, "pow");
  return LLVMBuildCall(builder, pow, args, 2, "pow");
}

LLVMTypeRef gen_type (int type) {
  switch (type) {
    case TYPEINTEGER:
      return LLVMInt32Type();
    case TYPEFLOAT:
      return LLVMDoubleType();
		default:
			printf("Undefined type, %d\n", type);
			abort();
  }
}

LLVMValueRef gen_expression(LLVMModuleRef mod, LLVMBuilderRef builder, struct ast *a) {

  struct symbol *symbol;
  LLVMValueRef l, r;

  switch(a->nodetype) {
    case T_INTEGER:
      return LLVMConstInt(LLVMInt32Type(), (int)((struct numvar *)a)->number, 0);
    case T_FLOAT:
      return LLVMConstReal(LLVMDoubleType(), ((struct numvar *)a)->number);
    case T_VAR:
      symbol = getcreate_symbol(((struct var *)a)->name);
      return LLVMBuildLoad(builder, symbol->llvm_value, symbol->name);

    case T_ADD:
			l = gen_expression(mod, builder, a->l);
			r = gen_expression(mod, builder, a->r);
      if (LLVMTypeOf(l) == LLVMInt32Type())
        return LLVMBuildAdd(builder, l, r, "add");
      else
  			return LLVMBuildFAdd(builder, l, r, "fadd");
    
    case T_SUB:
      l = gen_expression(mod, builder, a->l);
			r = gen_expression(mod, builder, a->r);
      if (LLVMTypeOf(l) == LLVMInt32Type())
        return LLVMBuildSub(builder, l, r, "sub");
      else
  			return LLVMBuildFSub(builder, l, r, "fsub");
  
    case T_MUL:
      l = gen_expression(mod, builder, a->l);
			r = gen_expression(mod, builder, a->r);
      if (LLVMTypeOf(l) == LLVMInt32Type()) 
        return LLVMBuildMul(builder, l, r, "mul");
      else 
  			return LLVMBuildFMul(builder, l, r, "fmul");
      
    case T_DIV:
      l = gen_expression(mod, builder, a->l);
			r = gen_expression(mod, builder, a->r);
      if (LLVMTypeOf(l) == LLVMInt32Type())
        return LLVMBuildUDiv(builder, l, r, "udiv");
      else 
  			return LLVMBuildFDiv(builder, l, r, "fdiv");

    case T_MINUS:
      l = gen_expression(mod, builder, a->l);
      if (LLVMTypeOf(l) == LLVMInt32Type()) 
        return LLVMBuildMul(builder, LLVMConstInt(LLVMInt32Type(), -1, 0), l, "neg");
      else 
        return LLVMBuildFMul(builder, LLVMConstReal(LLVMDoubleType(), -1.0), l, "fneg");

    case T_EXP:
      l = gen_expression(mod, builder, a->l);
			r = gen_expression(mod, builder, a->r);
      if (LLVMTypeOf(l) == LLVMInt32Type()) {
        l = LLVMBuildSIToFP(builder, l, LLVMDoubleType(), "fcast");
        r = LLVMBuildSIToFP(builder, r, LLVMDoubleType(), "fcast");
        LLVMValueRef result = call_pow(mod, builder, l, r);
        return LLVMBuildFPToSI(builder, result, LLVMInt32Type(), "icast");
      }
      return call_pow(mod, builder, l, r);

    default:
			printf("Undefined nodetype, %d\n", a->nodetype);
			abort();
  }
}

void gen_statement (LLVMModuleRef mod, LLVMBuilderRef builder, struct ast *a) {

  struct symbol *symbol;
  LLVMValueRef exp;

  if(!a) return;

  switch(a->nodetype) {
    case T_ASSIGNMENT:
      symbol = getcreate_symbol(((struct var *)a->l)->name);
      LLVMValueRef decl = LLVMBuildAlloca(builder, gen_type(symbol->type), symbol->name);
      symbol->llvm_value = decl;

			exp = gen_expression(mod, builder, a->r);
      LLVMBuildStore(builder, exp, decl);
			break;
    case T_PRINT:
      exp = gen_expression(mod, builder, a->l);
      call_printf(mod, builder, exp);
			break;
    default: 
			printf("Invalid Statement");
    	return;
  }
}

void gen (char *out_filename, llnode list) {
  llnode p = list;
  LLVMModuleRef mod = LLVMModuleCreateWithName("calculator");
	LLVMBuilderRef builder = LLVMCreateBuilder();

  // extern int printf(char*, ...)
  create_printf_function(mod);
  // extern double pow(double, double)
  create_pow_function(mod);

  // void main(void)
  LLVMValueRef main = LLVMAddFunction(mod, "main", LLVMFunctionType(LLVMVoidType(), NULL, 0, 0));
  LLVMBasicBlockRef bb = LLVMAppendBasicBlock(main, "entrypoint");
  LLVMPositionBuilderAtEnd(builder, bb);

  while(p != NULL){
    gen_statement(mod, builder, p->tree);
    p = p->next;
  }

  // return;
  LLVMBuildRetVoid(builder);

  FILE *ll_file = fopen("temp.ll", "w");
  if (!ll_file){
    printf("ERRO ao criar o arquivo .ll\n");
    exit(1);
  }

  fputs(LLVMPrintModuleToString(mod), ll_file);
  fclose(ll_file);

  char command[200]; 
  sprintf(command, "clang-9 temp.ll -Wno-override-module -lm -o %s", out_filename);
  system(command);
}
