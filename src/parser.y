%{
#include<iostream>
#include<list>
#include "ast.h"

using namespace std;

extern FILE *yyin;

extern "C" {
 int yylex();
}

void yyerror (char const *s);

prognode *root;
%}

%union {
    int num;
    char *id;
    arrnode *anode;
    operandnode *oper;
    varnode *var;
    prntele *pele;
    exprnode *expression;
    declnode *decl;
    declblock *dblock;
    codeblock *cblock;
    stmtnode *stmt;
    boolnode *bexpr;
    relnode *rexpr;
    list<declnode*> *dlist;
    list<stmtnode*> *clist;
    list<varnode*> *varlist;
    list<prntele*> *ele;
    prognode *prog;
};
			
%error-verbose
%token DB CB
%token <num> NUMBER
%token <id> IDENTIFIER TEXT
%token INTEGER STRING
%token LABEL
%token DATATYPE
%token TRUE FALSE
%token ARRAY
%token ETOK
%token	<num> AND OR
%token IF WHILE FOR ELSE GOTO GOT
%token PRINT PRINTLN READ  
%token <num> GT LT GE LE EQ NE
%left '='
%left '+' '-'
%left '*'
%left AND OR GT LT GE LE EQ NE

%type	<anode>		array
%type	<expression>	expr
%type	<oper>	 operand
%type	<var>	 variable
%type	<bexpr>		boolexpr
%type	<rexpr>		relexpr
%type	<ele>		element
%type	<pele>		printables			
%type	<dlist>		declaration_list
%type	<clist>		statement_list
%type	<decl>		declaration
%type	<stmt>		statement
%type	<varlist>	instance
%type	<cblock>	code_block
%type	<dblock>	decl_block
%type	<prog>		program		       
%%

program: 	DB decl_block CB code_block {$$ = new prognode($2,$4); root = $$;}	       
		
decl_block:     '{'declaration_list'}' {$$ = new declblock($2);}
	|	'{'  '}' {$$ = new declblock();}
		;

declaration_list:      declaration {$$ = new list<declnode*>();$$->push_back($1);}
	|	       declaration_list declaration {$1->push_back($2);$$ = $1;}
 		;

declaration:	DATATYPE instance ';' {$$ = new declnode($2);}	 
		;

instance: 	instance ',' variable {$$ = $1; $1->push_back($3);}
	|	variable {$$ = new list<varnode*>();$$->push_back($1);}
		;

		
code_block:  '{'statement_list '}' {$$ = new codeblock($2);}
	|	'{' '}' {$$ = new codeblock();}
 		;

statement_list:  statement {$$ = new list<stmtnode*>();$$->push_back($1);}
	|	statement_list statement {$1->push_back($2);$$ = $1;}
	| 	statement_list LABEL ':' statement {$1->push_back($4);$$ = $1;} ;


statement: 	IF boolexpr code_block {$$ = new ifstmt($2,$3);}
	|	IF boolexpr code_block ELSE code_block {$$ = new ifelsestmt($2,$3,$5);}
	|	WHILE boolexpr code_block {$$ = new whilestmt($2,$3);}
	|	FOR IDENTIFIER '=' operand ',' operand code_block {$$ = new forstmt($2,$4,$6,new numnode(1),$7);}
	|	FOR IDENTIFIER '=' operand ',' operand ',' operand code_block {$$ = new forstmt($2,$4,$6,$8,$9);}
	|	GOTO LABEL IF boolexpr ';'
	|	GOT LABEL ';'
	|	variable '=' expr ';' {$$ = new assignstmt($1,$3);}
	|	PRINT element ';' {$$ = new printstmt($2,false);}
	|	PRINTLN element ';' {$$ = new printstmt($2,true);}
	|	READ variable ';' {$$ = new readstmt($2);}
		;

expr:		expr '+' expr {$$ = new plusnode($1,$3);}
	|	expr '-' expr {$$ = new subnode($1,$3);}
	|	expr '*' expr {$$ = new mulnode($1,$3);}
	|	operand {$$ = $1;}
		;

boolexpr:	boolexpr AND boolexpr {$$ = new andnode($1,$3);}
	|	boolexpr OR boolexpr {$$ = new ornode($1,$3);}
	|	relexpr {$$ = $1;}
	;

relexpr:	operand GT operand {$$ = new gtnode($1,$3);}
	|	operand LT operand {$$ = new ltnode($1,$3);}
	|	operand GE operand {$$ = new genode($1,$3);}
	|	operand LE operand {$$ = new lenode($1,$3);}
	|	operand EQ operand {$$ = new eqnode($1,$3);}
	|	operand NE operand {$$ = new nenode($1,$3);}
	|	TRUE {$$ = new truenode();}
	|	FALSE {$$ = new falsenode();}
		;

element:	element ',' printables {$1->push_back($3);$$ = $1;}
	|	printables {$$ = new list<prntele*>();$$->push_back($1);}
	;

printables:	variable {$$ = $1;}
	|	TEXT {$$ = new textnode($1);}
	;

operand:	NUMBER {$$ = new numnode($1);}
	|	variable {$$ = $1;}
	;

variable:	IDENTIFIER {$$ = new identnode($1);}
	|	array {$$ = $1;}
	;

array:		IDENTIFIER '[' expr ']' {$$ = new arrnode($1,$3);}		     ;
%%

void yyerror (char const *s)
{
       fprintf (stderr, "%s\n", s);
}

void initllvm()
{
	cout<<"Generating IR code"<<endl;
	
	TheModule = new llvm::Module("BccLLVMir", TheContext);
	TheModule->setTargetTriple("x86_64-pc-linux-gnu");

	cout<<"Module status"<<TheModule<<endl;
	MainFunction = llvm::Function::Create(llvm::FunctionType::get(llvm::Type::getVoidTy(TheContext), false), llvm::GlobalValue::ExternalLinkage, "main", TheModule);

	MainBlock = llvm::BasicBlock::Create(TheContext, "entrypoint", MainFunction);
        
	Builder.SetInsertPoint(MainBlock);

	PutFunction = TheModule->getOrInsertFunction("printf", llvm::FunctionType::get(llvm::IntegerType::getInt64Ty(TheContext), llvm::PointerType::get(llvm::Type::getInt8Ty(TheContext), 0), true)); 
                                                  
	root->codegen();
	Builder.CreateRetVoid();
	TheModule->print(llvm::errs(), nullptr);

	int fd = open("output.ll", O_RDWR | O_CREAT);
	llvm::raw_ostream *out = new llvm::raw_fd_ostream(fd, "",nullptr);
	TheModule->print(*out, nullptr);    
}

int main(int argc, char *argv[])
{
	if (argc == 1 ) {
		fprintf(stderr, "Correct usage: bcc filename\n");
		exit(1);
	}

	if (argc > 2) {
		fprintf(stderr, "Passing more arguments than necessary.\n");
		fprintf(stderr, "Correct usage: bcc filename\n");
	}


	yyin = fopen(argv[1], "r");

	yyparse();
	cout<<endl<<endl;
	
	//cout<<"Running Interpreter"<<endl;
	//root->evaluate();
	
	initllvm();
	//MainFunction->viewCFGOnly();
	return 0;
}
