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
%type <statement> statement rotate ifelse is_zombie is_human attack is_passable is_random ranged_attack
 
/* Expressions */
%type <numeric> numeric

%%

main_loop	: TMAIN TLBRACE block TRBRACE { std::cout << "Main entry point found!" << std::endl; }
;

block		: statement { std::cout << "Single statement" << std::endl; }	
			| block statement { std::cout << "Multiple statements" << std::endl; }
/* TODO: Add support for multiple statements in a block */
;

statement	: rotate TSEMI | forward TSEMI | ifelse | bool | is_zombie | is_human | attack TSEMI | is_passable | is_random | ranged_attack TSEMI
;
	
bool		: is_zombie | is_human | is_passable | is_random
;
		
rotate		: TROTATE TLPAREN numeric TRPAREN { std::cout << "Rotate command" << std::endl; }
;

forward		: TFORWARD TLPAREN TRPAREN { std::cout << "Forward command" << std::endl; }
;		
	
ifelse		: TIF TLPAREN bool TRPAREN { std::cout << "If command" << std::endl; } TLBRACE block TRBRACE TELSE TLBRACE block TRBRACE { std::cout << "Else command" << std::endl; }
;

is_zombie	: TISZOMBIE TLPAREN numeric TRPAREN { std::cout << "Is_zombie statement" << std::endl; }
;

is_human	: TISHUMAN TLPAREN numeric TRPAREN  { std::cout << "Is_human statement" << std::endl; }
;

attack		: TATTACK TLPAREN TRPAREN { std::cout << "Attack command" << std::endl; }
;

is_passable	: TISPASSABLE TLPAREN TRPAREN { std::cout << "Is_passable statement" << std::endl; }
;

is_random	: TISRANDOM TLPAREN TRPAREN { std::cout << "Is_random statement" << std::endl; }
;

ranged_attack	: TATTACK TLPAREN TRPAREN { std::cout << "Ranged_attack command" << std::endl; }

numeric		: TINTEGER { std::cout << "Numeric value of " << *($1) << std::endl; }
;

%%
