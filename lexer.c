/**********************************************************************/
/* lab 1 DVG C01 - Lexer OBJECT                                       */
/**********************************************************************/

/**********************************************************************/
/* Include files                                                      */
/**********************************************************************/
#include <ctype.h>
#include <stdio.h>
#include <string.h>

/**********************************************************************/
/* Other OBJECT's METHODS (IMPORTED)                                  */
/**********************************************************************/
#include "keytoktab.h"

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define BUFSIZE 1024
#define LEXSIZE 30
static char buffer[BUFSIZE];
static char lexbuf[LEXSIZE];
static int pbuf = 0; /* current index program buffer  */
static int plex = 0; /* current index lexeme  buffer  */

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
/**********************************************************************/
/* buffer functions                                                   */
/**********************************************************************/

/**********************************************************************/
/* Read the input file into the buffer                                */
/**********************************************************************/

static void get_prog() {
    memset(buffer, 0, BUFSIZE);
    char c;
	int i = 0;
	while((c=getc(stdin))!=EOF)
		buffer[i++] = c;
    buffer[i] = '$';
}

/**********************************************************************/
/* Display the buffer                                                 */
/**********************************************************************/

static void pbuffer() {
    printf("\n________________________________________________________ ");
    printf("\n THE PROGRAM TEXT");
    printf("\n________________________________________________________ ");
    printf("\n%s", buffer);
    printf("\n________________________________________________________ ");
}

/**********************************************************************/
/* Copy a character from the program buffer to the lexeme buffer      */
/**********************************************************************/

static void get_char() {
    lexbuf[plex++] = buffer[pbuf++];
}

/**********************************************************************/
/* End of buffer handling functions                                   */
/**********************************************************************/

/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/
/**********************************************************************/
/* Return a token                                                     */
/**********************************************************************/
int get_token() {
    if(pbuf == 0){
        get_prog();
        pbuffer();
    }
    
    memset(lexbuf, 0, LEXSIZE);
    plex = 0;

    /* skip whitespace */
    while(isspace(buffer[pbuf]))
        pbuf++;
    /* copy first character from buffert to lefbuf*/
    get_char();

    if(isdigit(lexbuf[0])) {
        while(isdigit(buffer[pbuf])) {
            get_char();
        }
        return lex2tok(lexbuf);
    
    } else if(isspecial(lexbuf[0])){
        return lex2tok(lexbuf);
    }else if(isalpha(lexbuf[0])) {
        while (isalnum(buffer[pbuf])) {
            get_char();
        }
        return key2tok(lexbuf);
    } else if((lexbuf[0] == ':') && (buffer[pbuf] == '=')){
        get_char();
        return lex2tok(lexbuf);
    }
    return lex2tok(lexbuf);
}

/**********************************************************************/
/* Return a lexeme                                                    */
/**********************************************************************/
char *get_lexeme() {
    return lexbuf;
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
