#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include <stdio.h>
#include <fcntl.h>
#include <algorithm>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>
#include <stdlib.h>
#include <string>
#include <list>

extern llvm::LLVMContext TheContext;
extern llvm::IRBuilder<> Builder;
extern llvm::Module *TheModule;
extern std::map<std::string, llvm::Value *> NamedValues;

extern llvm::Function *MainFunction;
extern llvm::Constant *PutFunction;
extern llvm::BasicBlock *MainBlock;

using namespace std;

extern map<string,int> symtab;
extern map<string, int*> arrtab;

class elenode
{
    
    
};

class prntele : public elenode
{
public :
    virtual void print() = 0;
    virtual void* codegen() = 0;
};

class textnode : public prntele
{
    string text;
public:
    textnode(string value);
    void print();
    virtual void* codegen();
};

class exprnode
{
public :
    virtual int evaluate() = 0;
    virtual void *codegen() = 0;    
};

class operandnode : public exprnode
{
    
};

class varnode : public operandnode, public prntele
{

public:
    string id;
    virtual int evaluate() = 0;
    virtual void declare() = 0;
    virtual void print() = 0;
    virtual void assign(int value) = 0;
    virtual void *codegen() = 0;
};

class opnode : public exprnode
{
public :
    exprnode *left;
    exprnode *right;    

    opnode(exprnode *L, exprnode *R);
    virtual int evaluate() = 0;
    virtual void* codegen() = 0;
};
    
class numnode: public operandnode
{
    int val;
public:   
    numnode(int value);
    int evaluate();
    virtual void *codegen();
};

class identnode: public varnode
{

public:
    identnode(string value);
    void print();
    int evaluate();
    void declare();
    void assign(int value);
    virtual void *codegen();
};

class arrnode: public varnode
{
    int idx;
    
public:
    exprnode *expridx;
    arrnode(string name,int idx);
    arrnode(string name,string ident);
    arrnode(string name,exprnode* expr);
    int evaluate();
    void print();
    void declare();
    void assign(int value);
    virtual void *codegen();
};

class plusnode: public opnode
{
    
public :
    plusnode(exprnode *L,exprnode *R) : opnode(L,R) {}
    int evaluate();
    virtual void *codegen();    
};

class mulnode: public opnode
{

public :
    mulnode(exprnode *L,exprnode *R) : opnode(L,R) {}
    int evaluate();
    virtual void *codegen();    
};

class subnode: public opnode
{
public :
    subnode(exprnode *L,exprnode *R) : opnode(L,R) {}
    int evaluate();
    virtual void *codegen();    
};

class boolnode
{
public:
    virtual bool evaluate() = 0;
    virtual void* codegen() = 0;
};

class boolopnode : public boolnode
{
public:
    boolnode *left;
    boolnode *right;

    boolopnode(boolnode *L, boolnode *R);
};

class andnode : public boolopnode
{
public:
    andnode(boolnode *L, boolnode *R) : boolopnode(L,R) {}
    bool evaluate();
    virtual void* codegen();
};

class ornode : public boolopnode
{
public:
    ornode(boolnode *L, boolnode *R) : boolopnode(L,R) {}
    bool evaluate();
    virtual void* codegen();
};

class relnode : public boolnode
{
   
};
    
class relopnode : public relnode
{
public:
    operandnode *left;
    operandnode *right;
    relopnode(operandnode *L,operandnode *R);
    virtual void* codegen() = 0;
};

class gtnode : public relopnode
{
public:
    gtnode(operandnode *L, operandnode *R) : relopnode(L,R) {}
    bool evaluate();
    virtual void* codegen();
};

class ltnode : public relopnode
{
public:
    ltnode(operandnode *L, operandnode *R) : relopnode(L,R) {}
    bool evaluate();
    virtual void* codegen();
};

class genode : public relopnode
{
public:
    genode(operandnode *L, operandnode *R) : relopnode(L,R) {}
    bool evaluate();
    virtual void* codegen();
};


class lenode : public relopnode
{
public:
    lenode(operandnode *L, operandnode *R) : relopnode(L,R) {}
    bool evaluate();
    virtual void* codegen();
};

class eqnode : public relopnode
{
public:
    eqnode(operandnode *L, operandnode *R) : relopnode(L,R) {}
    bool evaluate();
    virtual void* codegen();
};


class nenode : public relopnode
{
public:
    nenode(operandnode *L, operandnode *R) : relopnode(L,R) {}
    bool evaluate();
    virtual void* codegen();
};

class truenode : public relnode
{
public:
    bool evaluate();
    virtual void* codegen();
};

class falsenode : public relnode 
{
public:
    bool evaluate();
    virtual void* codegen();
};
    
class stmtnode
{
public :
    virtual void evaluate() = 0;
    virtual void* codegen() = 0;
};

class codeblock
{
    list<stmtnode*> *codelist;
public:
    codeblock(list<stmtnode*> *tlist);
    codeblock();
    void evaluate();
    virtual void* codegen();
};

class assignstmt : public stmtnode
{
    varnode *var;
    exprnode *exp;
    
 public:
    assignstmt(varnode *idvalue,exprnode *exprvalue);
    void evaluate();
    virtual void* codegen();
};

class printstmt : public stmtnode
{
    list<prntele*> *printlist;
    bool endline;
public:
    printstmt(list<prntele*> *plistnode,bool val);
    void evaluate();
    virtual void* codegen();
};

class readstmt : public stmtnode
{
    varnode *id;
public :
    readstmt(varnode *idvalue);
    void evaluate();
    virtual void* codegen();
};

class ifstmt : public stmtnode
{
    boolnode *bval;
    codeblock *code;
public:
    ifstmt(boolnode *bexpr,codeblock *cblock);
    void evaluate();
    virtual void* codegen();
};

class ifelsestmt : public stmtnode
{
    boolnode *bval;
    codeblock *ifcode;
    codeblock *elsecode;
public:
    ifelsestmt(boolnode *bexpr,codeblock *code1,codeblock *code2);
    void evaluate();
    virtual void* codegen();
};

class whilestmt : public stmtnode
{
    boolnode *bval;
    codeblock *code;
public:
    whilestmt(boolnode *bexpr,codeblock *cblock);
    void evaluate();
    virtual void* codegen();
};

class forstmt : public stmtnode
{
    string id;
    operandnode *from,*to,*skip;
    codeblock *code;

public:
    forstmt(string id,operandnode* f,operandnode* t,operandnode* s,codeblock *cblock);
    void evaluate();
    virtual void* codegen();
};
    
class declnode
{
public:
    list<varnode*> *instancelist;
    declnode(list<varnode*> *varlist);
    void evaluate();
    virtual void* codegen();
};

class declblock
{
    list<declnode*> *declist; //declaration list
public:
    declblock(list<declnode*> *tlist);
    declblock();
    void evaluate();
    virtual void* codegen();
};

class prognode
{
    declblock *decl_block;
    codeblock *code_block;
    
public :
    prognode(declblock *L,codeblock *R);
    void evaluate();
    virtual void* codegen();
};



// Local Variables:
// mode: c++
// End:
