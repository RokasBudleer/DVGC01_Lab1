/**********************************************************************/
/* lab 1 DVG C01 - Driver OBJECT                                      */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <stdio.h>
#include <ctype.h>
#include <string.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
#include "keytoktab.h"

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
/**********************************************************************/
/* type definitions                                                   */
/**********************************************************************/
typedef struct tab {
	char 	* text;
	int 	token;
	} tab;
	
/**********************************************************************/
/* data objects (tables)                                              */
/**********************************************************************/
static tab tokentab[ ] = {
        {"id", 	        id},
	{"number", 	    number},
	{":=", 	        assign},
	{"undef", 	    undef},
	{"predef", 	    predef},
	{"tempty", 	    tempty},
	{"error",        error},
	{"type",         typ},
	{"$",            '$'},
	{"(",            '('},
	{")",            ')'},
	{"*",            '*'},
	{"+",            '+'},
	{",",            ','},
	{"-",            '-'},
	{".",            '.'},
	{"/",            '/'},
	{":",            ':'},
	{";",            ';'},
	{"=",            '='},
	{"TERROR", 	nfound}
        };


static tab keywordtab[ ] = {
	{"program", 	program},
	{"input", 	input},
	{"output", 	output},
	{"var", 	var},
	{"begin", 	begin},
	{"end", 	end},
	{"boolean", 	boolean},
	{"integer", 	integer},
	{"real", 	real},
	{"KERROR", 	nfound}
	};
   
/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* Display the tables                                                 */
/**********************************************************************/
void p_toktab()
{  
	printf("\nKeywordtab:");
   for(int i = 0; i < sizeof(keywordtab)/sizeof(tab); i++){
		printf("\n%s	%d", keywordtab[i].text, keywordtab[i].token);
   }
   printf("\nTokentab:");
   for(int i = 0; i < sizeof(tokentab)/sizeof(tab); i++){
		printf("\n%s	%d", tokentab[i].text, tokentab[i].token);
   }
}

/**********************************************************************/
/* lex2tok - convert a lexeme to a token                              */
/**********************************************************************/
toktyp lex2tok(char * fplex)
{
   for(int i = 0; i < sizeof(tokentab)/sizeof(tab); i++){
		if(!strcmp(fplex, tokentab[i].text)){
			return tokentab[i].token;
		}
   }
   if(isdigit(*fplex)){
		return number;
   }
   return nfound;
}

/**********************************************************************/
/* key2tok - convert a keyword to a token                             */
/**********************************************************************/
toktyp key2tok(char * fplex)
{
   for(int i = 0; i < sizeof(keywordtab)/sizeof(tab); i++){
		if(!strcmp(fplex, keywordtab[i].text)){
			return keywordtab[i].token;
		}
   }
   return id;
}

/**********************************************************************/
/* tok2lex - convert a token to a lexeme                              */
/**********************************************************************/
char * tok2lex(toktyp ftok)
{
   for(int i = 0; i < sizeof(keywordtab)/sizeof(tab); i++){
		if(keywordtab[i].token == ftok){
			return keywordtab[i].text;
		}
   }

   for(int i = 0; i < sizeof(tokentab)/sizeof(tab); i++){
		if(tokentab[i].token == ftok){
			return tokentab[i].text;
		}
   }
   return "Error, could not convert token to lexeme";
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/

