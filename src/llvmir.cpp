#include "ast.h"

using namespace llvm;

Module *TheModule;
Function *MainFunction;
Constant *PutFunction;
BasicBlock *MainBlock;
LLVMContext TheContext;
IRBuilder<> Builder(TheContext);
std::map<std::string, llvm::Value *> NamedValues;

void *numnode::codegen()
{
    return ConstantInt::get(TheContext, APInt(64,val, true));
}

void *identnode::codegen()
{
    Value *V = NamedValues[id];
    
    return Builder.CreateLoad(V,"tmp");
}

void *arrnode::codegen()
{
    Value *V;
    return V;
}

void *plusnode::codegen()
{
    Value *L = static_cast<Value*>(left->codegen());
    Value *R = static_cast<Value*>(right->codegen());
    if (!L || !R)
	return nullptr;
    
    return Builder.CreateAdd(L, R, "addtmp");
}

void *mulnode::codegen()
{
    Value *L = static_cast<Value*>(left->codegen());
    Value *R = static_cast<Value*>(right->codegen());
    if (!L || !R)
	return nullptr;
    
    return Builder.CreateMul(L, R, "multmp");
}

void* subnode::codegen()
{
    Value *L = static_cast<Value*>(left->codegen());
    Value *R = static_cast<Value*>(right->codegen());
    if (!L || !R) return nullptr;
    
    return Builder.CreateSub(L, R, "subtmp");
}

void* codeblock::codegen()
{
    for(auto itr=codelist->begin();itr!=codelist->end();itr++)
    {
	    (*itr)->codegen();
    }
    return nullptr;
}

void* declnode::codegen()
{
    for(auto itr=instancelist->begin();itr!=instancelist->end();itr++)
    {
        arrnode *arrid = dynamic_cast<arrnode *>(*itr);
        if(arrid)
        {
            Value *index = static_cast<Value*>(arrid->expridx->codegen());
            GlobalVariable *gvar = new GlobalVariable(*TheModule, ArrayType::get(Type::getInt64Ty(TheContext), index), false, GlobalValue::CommonLinkage, NULL, arrid->id);
            gvar->setInitializer(ConstantAggregateZero::get(ArrayType::get(Type::getInt64Ty(TheContext), index)));    

        }
        else
        {
            cout<<"varname "<<TheModule<<endl;
            GlobalVariable *gvar = new GlobalVariable(*TheModule, Type::getInt64Ty(TheContext), false, GlobalValue::CommonLinkage, NULL, (*itr)->id);
            gvar->setInitializer(ConstantInt::get(TheContext, APInt(64, StringRef("0"), 10)));
        
        }
        
        NamedValues[(*itr)->id] = gvar;
    }
    return nullptr;
}

void* declblock::codegen()
{
    for(auto itr=declist->begin();itr!=declist->end();itr++)
    {
	(*itr)->codegen();
    }
    return nullptr;
}

void* prognode::codegen()
{
    decl_block->codegen();
    code_block->codegen();
    return nullptr;
}

void* andnode::codegen()
{
    return Builder.CreateAnd(static_cast<Value*>(left->codegen()), static_cast<Value*>(right->codegen()), "andtmp");
}

void* ornode::codegen()
{
    return Builder.CreateOr(static_cast<Value*>(left->codegen()), static_cast<Value*>(right->codegen()), "ortmp");
}

void* gtnode::codegen()
{
    return Builder.CreateZExt(Builder.CreateICmpUGT(static_cast<Value*>(left->codegen()), static_cast<Value*>(right->codegen()), "gttmp"),Type::getInt64Ty(TheContext));
}

void* ltnode::codegen()
{
    return Builder.CreateZExt(Builder.CreateICmpULT(static_cast<Value*>(left->codegen()), static_cast<Value*>(right->codegen()), "lttmp"),Type::getInt64Ty(TheContext));
}

void* genode::codegen()
{
    return Builder.CreateZExt(Builder.CreateICmpUGE(static_cast<Value*>(left->codegen()), static_cast<Value*>(right->codegen()), "getmp"),Type::getInt64Ty(TheContext));
}

void* lenode::codegen()
{
    return Builder.CreateZExt(Builder.CreateICmpULE(static_cast<Value*>(left->codegen()), static_cast<Value*>(right->codegen()), "letmp"),Type::getInt64Ty(TheContext));
}

void* eqnode::codegen()
{
    return Builder.CreateZExt(Builder.CreateICmpEQ(static_cast<Value*>(left->codegen()), static_cast<Value*>(right->codegen()), "eqtmp"),Type::getInt64Ty(TheContext));
}

void* nenode::codegen()
{
    return Builder.CreateZExt(Builder.CreateICmpNE(static_cast<Value*>(left->codegen()), static_cast<Value*>(right->codegen()), "netmp"),Type::getInt64Ty(TheContext));
}

void* truenode::codegen()
{
    return ConstantInt::get(TheContext, APInt(64,1, true));
}

void* falsenode::codegen()
{
    return ConstantInt::get(TheContext, APInt(64,0, true));
}

void* forstmt::codegen()
{
    return nullptr;
}

/*
void* forstmt::codegen()
{
    // Emit the start code first, without 'variable' in scope.
    Value *StartVal = from->codegen();
    if (!StartVal)
	return nullptr;

    // Make the new basic block for the loop header, inserting after current
    // block.
    BasicBlock *PreheaderBB = Builder.GetInsertBlock();
    BasicBlock *LoopBB = BasicBlock::Create(TheContext, "loop", TheFunction);
    
    // Insert an explicit fall through from the current block to the LoopBB.
    Builder.CreateBr(LoopBB);

    // Start insertion in LoopBB.
    Builder.SetInsertPoint(LoopBB);
    
    // Start the PHI node with an entry for Start.
    PHINode *Variable = Builder.CreatePHI(Type::getDoubleTy(TheContext), 2, id.c_str());
    Variable->addIncoming(StartVal, PreheaderBB);

    // Within the loop, the variable is defined equal to the PHI node.  If it
    // shadows an existing variable, we have to restore it, so save it now.
    Value *OldVal = NamedValues[id];
    NamedValues[id] = Variable;
    
    // Emit the body of the loop.  This, like any other expr, can change the
    // current BB.  Note that we ignore the value computed by the body, but don't
    // allow an error.
    if (!code->codegen())
	return nullptr;

    // Emit the step value.
    Value *StepVal = skip->codegen(); //defaults to 1
    if (!StepVal)
	return nullptr;
    
    // Compute the end condition.
    Value *EndCond = to->codegen();
    if (!EndCond)
	return nullptr;
    
    // Convert condition to a bool by comparing non-equal to 0.0.
    EndCond = Builder.CreateFCmpONE(EndCond, ConstantFP::get(TheContext, APFloat(0.0)), "loopcond");

    Value *NextVar = Builder.CreateFAdd(Variable, StepVal, "nextvar");

    // Create the "after loop" block and insert it.
    BasicBlock *LoopEndBB = Builder.GetInsertBlock();
    BasicBlock *AfterBB = BasicBlock::Create(TheContext, "afterloop", TheFunction);

    // Insert the conditional branch into the end of LoopEndBB.
    Builder.CreateCondBr(EndCond, LoopBB, AfterBB);
    
    // Any new code will be inserted in AfterBB.
    Builder.SetInsertPoint(AfterBB);
    
    // Add a new entry to the PHI node for the backedge.
    Variable->addIncoming(NextVar, LoopEndBB);
    
    // Restore the unshadowed variable.
    if (OldVal)
	NamedValues[id] = OldVal;
    else
	NamedValues.erase(id);
    
    // for expr always returns 0.0.
    return Constant::getNullValue(Type::getDoubleTy(TheContext));
}
*/

void* whilestmt::codegen()
{
    BasicBlock *EntryBB = Builder.GetInsertBlock();
    BasicBlock *HeaderBB = BasicBlock::Create(TheContext, "while_header", MainFunction);
    BasicBlock *BodyBB = BasicBlock::Create(TheContext, "while_body", MainFunction);
    BasicBlock *AfterBB = BasicBlock::Create(TheContext, "while_after", MainFunction, 0);
    
    Builder.CreateBr(HeaderBB); //from entry to header
    Builder.SetInsertPoint(HeaderBB);
    
    Value *loopcond = static_cast<Value*>(bval->codegen());
    ICmpInst *CondV = new ICmpInst(*HeaderBB, ICmpInst::ICMP_NE, loopcond, ConstantInt::get(Type::getInt64Ty(TheContext), 0, true), "ifcond");
    //loopcond = Builder.CreateFCmpONE(loopcond, static_cast<Value*>(ConstantInt::get(TheContext, APInt(64,0, true))), "loopcond");
    
    Builder.CreateCondBr(CondV, BodyBB, AfterBB);

    Builder.SetInsertPoint(BodyBB);
    code->codegen();

    Builder.CreateBr(HeaderBB);
    
    Builder.SetInsertPoint(AfterBB);
    
    return Constant::getNullValue(Type::getDoubleTy(TheContext));
}

void* ifstmt::codegen()
{
    return NULL;   
}

void* ifelsestmt::codegen()
{
    BasicBlock *EntryBB = Builder.GetInsertBlock();
    
    Value *condition = static_cast<Value *>(bval->codegen());
    ICmpInst *CondV = new ICmpInst(*EntryBB, ICmpInst::ICMP_NE, condition, ConstantInt::get(Type::getInt64Ty(TheContext), 0, true), "ifcond");
    //Value* CondV = Builder.CreateFCmpONE(condition, static_cast<Value*>(ConstantFP::get(TheContext, APFloat(0.0))), "ifcond");
    
    BasicBlock *ThenBB = BasicBlock::Create(TheContext, "then",MainFunction);
    BasicBlock *ElseBB = BasicBlock::Create(TheContext, "else");
    BasicBlock *MergeBB = BasicBlock::Create(TheContext,"merge");

    Builder.CreateCondBr(CondV, ThenBB, ElseBB);
	
    Builder.SetInsertPoint(ThenBB);
    Value* ThenV = static_cast<Value*>(ifcode->codegen());
    Builder.CreateBr(MergeBB);
    MainFunction->getBasicBlockList().push_back(ElseBB);

    Builder.SetInsertPoint(ElseBB);
    Value* ElseV = static_cast<Value*>(elsecode->codegen());
    Builder.CreateBr(MergeBB);
    MainFunction->getBasicBlockList().push_back(MergeBB);

    Builder.SetInsertPoint(MergeBB);
    //PHINode *PN = Builder.CreatePHI(Type::getDoubleTy(TheContext), 2, "iftmp");
    //PN->addIncoming(ThenV, ThenBB);
    //PN->addIncoming(ElseV, ElseBB);

    return Constant::getNullValue(Type::getDoubleTy(TheContext));
    //return Constant::getNullValue(Type::getDoubleTy(TheContext));
    //return PN;
}

void* textnode::codegen()
{
    return Builder.CreateGlobalStringPtr(text);
}

void* printstmt::codegen()
{
    std::vector<Value*> print_list;
    for(auto itr=printlist->begin();itr!=printlist->end();itr++)
    {
	print_list.push_back(static_cast<Value*>((*itr)->codegen()));
    }
    return Builder.CreateCall(PutFunction, print_list ,"printfCall");
}

void* readstmt::codegen()
{
    
}

void* assignstmt::codegen()
{
    //LoadExpr NamedValues?
    Value* exprval = static_cast<Value*>(exp->codegen());
    //Store Expr value to LHS
    Value* lhsvar = NamedValues[(*var).id];
	
    return Builder.CreateStore(exprval, lhsvar, false);
}

