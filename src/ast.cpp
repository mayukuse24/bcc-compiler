#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <list>
#include "ast.h"

using namespace std;

map<string,int> symtab;
map<string, int*> arrtab;

textnode::textnode(string value)
{
    text = value;
}

void textnode::print()
{
    cout<<text<<" ";
}

identnode::identnode(string value)
{
    id = value;
}

int identnode::evaluate()
{
    return symtab[id];
}

void identnode::print()
{
    cout<<symtab[id]<<" ";
}

void identnode::declare()
{
    symtab[id] = 0;
}

void identnode::assign(int value)
{
    symtab[id] = value;
}

arrnode::arrnode(string name,int index)
{
    id = name;
    idx = index;
}

arrnode::arrnode(string name,string ident)
{
    id = name;
    idx = symtab[ident];
}

arrnode::arrnode(string name, exprnode* expr)
{
    id = name;
    expridx = expr;
}

void arrnode::declare()
{
    arrtab[id] = new int[expridx->evaluate()];
}

void arrnode::print()
{
    cout<<arrtab[id][expridx->evaluate()]<<" ";
}

int arrnode::evaluate()
{
    return arrtab[id][expridx->evaluate()];
}

void arrnode::assign(int value)
{
    arrtab[id][expridx->evaluate()] = value;
}

int plusnode::evaluate()
{
    return left->evaluate() + right->evaluate();
}

int mulnode::evaluate()
{
    return left->evaluate() * right->evaluate();
}

int subnode::evaluate()
{
    return left->evaluate() - right->evaluate();
}

numnode::numnode(int value)
{
    val = value;
}

int numnode::evaluate()
{
    return val;
}

opnode::opnode(exprnode *L, exprnode *R)
{
    left = L;
    right = R;
}

boolopnode::boolopnode(boolnode *L, boolnode *R)
{
    left = L;
    right = R;
}

bool andnode::evaluate()
{
    return left->evaluate() && right->evaluate();
}

bool ornode::evaluate()
{
    return left->evaluate() || right->evaluate();
}

relopnode::relopnode(operandnode *L,operandnode *R)
{
    left = L;
    right = R;
}

bool gtnode::evaluate()
{
    return left->evaluate() > right->evaluate();
}

bool ltnode::evaluate()
{
    return left->evaluate() < right->evaluate();
}

bool genode::evaluate()
{
    return left->evaluate() >= right->evaluate();
}

bool lenode::evaluate()
{
    return left->evaluate() <= right->evaluate();
}

bool eqnode::evaluate()
{
    return left->evaluate() == right->evaluate();
}

bool nenode::evaluate()
{
    return left->evaluate() != right->evaluate();
}

bool truenode::evaluate()
{
    return true;
}

bool falsenode::evaluate()
{
    return false;
}

codeblock::codeblock(list<stmtnode*> *tlist)
{
    codelist = tlist;
}

codeblock::codeblock()
{
    codelist = new list<stmtnode*>();
}

void codeblock::evaluate()
{
    list<stmtnode *>::iterator stmtiter;
    for (stmtiter = codelist->begin(); stmtiter != codelist->end();stmtiter++)
    {
	(*stmtiter)->evaluate();
    }    
}

assignstmt::assignstmt(varnode *idvalue,exprnode *exprvalue)
{
    var = idvalue;
    exp = exprvalue;
}

void assignstmt::evaluate()
{
    int result = exp->evaluate();
    var->assign(result);
}

printstmt::printstmt(list<prntele*> *plistnode,bool val)
{
    printlist = plistnode;
    endline = val;
}

void printstmt::evaluate()
{
    list<prntele*>::iterator peiter;

    for(peiter=printlist->begin();peiter != printlist->end();peiter++)
    {
	(*peiter)->print();
    }
	
    if(endline)
    {
	cout<<'\b'<<endl; 
    }
    else
    {
	cout<<'\b'; //eliminate final space
    }
}

readstmt::readstmt(varnode *idvalue)
{
    id = idvalue;
}

void readstmt::evaluate()
{
    int temp;
    cin>>temp;
    id->assign(temp);
}

ifstmt::ifstmt(boolnode *bexpr,codeblock *cblock)
{
    bval = bexpr;
    code = cblock;
}

void ifstmt::evaluate()
{
    if(bval->evaluate())
    {
	code->evaluate();
    }
}

ifelsestmt::ifelsestmt(boolnode *bexpr,codeblock *code1,codeblock *code2)
{
    bval = bexpr;
    ifcode = code1;
    elsecode = code2;
}

void ifelsestmt::evaluate()
{
    if(bval->evaluate())
    {
	ifcode->evaluate();
    }
    else
    {
	elsecode->evaluate();
    }
}

whilestmt::whilestmt(boolnode *bexpr,codeblock *cblock)
{
    bval = bexpr;
    code = cblock;
}

void whilestmt::evaluate()
{
    while(bval->evaluate())
    {
	code->evaluate();
    }
}

forstmt::forstmt(string name,operandnode* f,operandnode* t,operandnode* s,codeblock *cblock)
{
    id = name;
    from = f;
    to = t;
    skip = s;
    code = cblock;
}

void forstmt::evaluate()
{
    for(symtab[id]=from->evaluate();symtab[id]<to->evaluate();symtab[id]+=skip->evaluate())
    {
	code->evaluate();
    }
}

declnode::declnode(list<varnode*> *varlist)
{
    instancelist = varlist;
}

void declnode::evaluate()
{
    list<varnode*>::iterator variter;
    for(variter=instancelist->begin();variter!=instancelist->end();variter++)
    {
	(*variter)->declare();
    }
}

declblock::declblock(list<declnode*> *tlist)
{
    declist = tlist;
}

declblock::declblock()
{
    declist = new list<declnode*>();
}

void declblock::evaluate()
{
    list<declnode*>::iterator deiter;
    for(deiter=declist->begin();deiter!=declist->end();deiter++)
    {
	(*deiter)->evaluate();
    }	
}

prognode::prognode(declblock *L,codeblock *R)
{
    decl_block = L;
    code_block = R;
}

void prognode::evaluate()
{
    decl_block->evaluate();
    code_block->evaluate();
}
