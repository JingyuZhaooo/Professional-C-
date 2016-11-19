%{
#include "node.h"
#include <iostream>
#include <string>
extern int yylex();
extern void yyerror(const char* s);

// Global for the main block
NBlock* g_MainBlock = nullptr;

// Disable the warnings that Bison creates
#pragma warning(disable: 4065)
%}

/* Add one union member for each Node* type */
%union {
    Node* node;
	NBlock* block;
	NStatement* statement;
	NNumeric* numeric;
	NBoolean* boolean;
	std::string* string;
	int token;
}

%error-verbose

/* Terminal symbols */
%token <string> TINTEGER
%token <token> TLBRACE TRBRACE TSEMI TLPAREN TRPAREN
%token <token> TMAIN TROTATE TFORWARD TIF TELSE TISZOMBIE TISHUMAN TATTACK TISPASSABLE TISRANDOM TRANGEDATTACK

/* Statements */
%type <block> main_loop block
%type <statement> statement rotate ifelse attack ranged_attack forward
%type <boolean> boolean is_zombie is_passable is_human is_random

/* Expressions */
%type <numeric> numeric

%%

main_loop	: TMAIN TLBRACE block TRBRACE { std::cout << "Main entry point found!" << std::endl; g_MainBlock = $3; g_MainBlock->SetMainBlock(); }
;

block		: statement { std::cout << "Single statement" << std::endl; $$ = new NBlock(); $$->AddStatement($1); }	
			| block statement { std::cout << "Multiple statements" << std::endl; $$->AddStatement($2); }

/* TODO: Add support for multiple statements in a block */
;

statement	: rotate TSEMI { $$ = $1; }
			| ranged_attack TSEMI { $$ = $1; }
			| forward TSEMI { $$ = $1; }
			| ifelse { $$ = $1; }
			| attack TSEMI { $$ = $1; }
;
	
boolean		: is_zombie { $$ = $1; }
			| is_human { $$ = $1; }
			| is_passable { $$ = $1; }
			| is_random { $$ = $1; }
;
		
rotate		: TROTATE TLPAREN numeric TRPAREN { std::cout << "Rotate command" << std::endl; $$ = new NRotate($3); }
;

forward		: TFORWARD TLPAREN TRPAREN { std::cout << "Forward command" << std::endl; $$ = new NForward(); }
;		
	
ifelse		: TIF TLPAREN boolean TRPAREN TLBRACE block TRBRACE TELSE TLBRACE block TRBRACE { std::cout << "ifelse command" << std::endl; $$ = new NIfelse($3, $6, $10); }
;

is_zombie	: TISZOMBIE TLPAREN numeric TRPAREN { std::cout << "Is_zombie statement" << std::endl; $$ = new NIs_Zombie($3); }
;

is_human	: TISHUMAN TLPAREN numeric TRPAREN  { std::cout << "Is_human statement" << std::endl;  $$ = new NIs_Human($3); }
;

attack		: TATTACK TLPAREN TRPAREN { std::cout << "Attack command" << std::endl; $$ = new NAttack(); }
;

is_passable	: TISPASSABLE TLPAREN TRPAREN { std::cout << "Is_passable statement" << std::endl; $$ = new NIs_Passable(); }
;

is_random	: TISRANDOM TLPAREN TRPAREN { std::cout << "Is_random statement" << std::endl; $$ = new NIs_random(); }
;

ranged_attack	: TRANGEDATTACK TLPAREN TRPAREN { std::cout << "Ranged_attack command" << std::endl; $$ = new NRanged_Attack(); }
;

numeric		: TINTEGER { std::cout << "Numeric value of " << *($1) << std::endl; $$ = new NNumeric(*$1); }
;

%%
