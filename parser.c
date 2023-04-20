/**********************************************************************/
/* lab 1 DVG C01 - Parser OBJECT                                      */
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
#include "keytoktab.h" /* when the keytoktab is added   */
#include "lexer.h"     /* when the lexer     is added   */
#include "optab.h"     /* when the optab     is added   */
#include "symtab.h"    /* when the symtab    is added   */

/**********************************************************************/
/* OBJECT ATTRIBUTES FOR THIS OBJECT (C MODULE)                       */
/**********************************************************************/
#define DEBUG 0
static int lookahead = 0;
static int is_parse_ok = 1;

static toktyp expr();
static toktyp factor();
static toktyp term();

/**********************************************************************/
/*  PRIVATE METHODS for this OBJECT  (using "static" in C)            */
/**********************************************************************/
static void esyntax(char *msg) {
    printf("\nSYNTAX: %s", msg);
    is_parse_ok = 0;
}
static void esemantic(char *msg){
    printf("\nSEMANTIC: %s", msg);
    is_parse_ok = 0;
}
static void p_parse_status() {
    if (is_parse_ok)
        printf("\nPARSE SUCCESSFUL! ");
    printf("\n________________________________________________________ ");
}
/**********************************************************************/
/* The Parser functions                                               */
/**********************************************************************/
static void match(int t) {
    if (DEBUG)
        printf("\n --------In match expected: %s found: %s",
               tok2lex(t), get_lexeme());
    if (lookahead == t)
        lookahead = get_token();
    else {
        is_parse_ok = 0;
        printf("\n SYNTAX: expected: %s found: %s (in match)",
               tok2lex(t), get_lexeme());
    }
}

/**********************************************************************/
/* The grammar functions                                              */
/**********************************************************************/
/***************     prog_part    ******************/

static void prog_header() {
    if (DEBUG)
        printf("\n *** In  program_header");
    if (lookahead == program)
        match(program);
    else
        esyntax("Did not find program token");

    if (lookahead == id) {
        addp_name(get_lexeme());
        match(id);
    } else {
        addp_name("???");
        match(id);
    }

    match('(');
    match(input);
    match(',');
    match(output);
    match(')');
    match(';');
}

/***************     var_part    ******************/
static void type() {
    if (DEBUG)
        printf("\n *** In type");
    if (lookahead == integer) {
        setv_type(integer);
        match(integer);
    } else if (lookahead == real) {
        setv_type(real);
        match(real);
    } else if (lookahead == boolean) {
        setv_type(boolean);
        match(boolean);
    } else {
        esemantic("type not defined");
        setv_type(error);
    }
}
static void id_list() {
    if (DEBUG)
        printf("\n *** In id_list");
    if(lookahead == id){
        if (!find_name(get_lexeme())) {
            addv_name(get_lexeme());
            match(id);
        } else {
            esemantic("duplicate variable name");
            match(id);
        }
    }
    if (lookahead == ',') {
        match(',');
        id_list();
    }
}
static void var_dec() {
    if (DEBUG)
        printf("\n *** In var_dec");

    id_list();
    match(':');
    type();
    match(';');
}
static void var_dec_list() {
    if (DEBUG)
        printf("\n *** In var_dec_list");
    var_dec();
    if (lookahead == id)
        var_dec_list();
}
static void var_part() {
    if (DEBUG)
        printf("\n *** In var_part");
    if (lookahead == var) {
        match(var);
        var_dec_list();
    } else {
        printf("\n*** NOTE: Did not find var, no variables declared. ***");
    }
}

/***************     stat_part   ******************/
static toktyp operand() {
    if (DEBUG)
        printf("\n *** In operand");
    toktyp toktyp_R = undef;
    if (lookahead == id) {
        if (find_name(get_lexeme())) {
            toktyp_R = get_ntype(get_lexeme());
            match(id);
        } else {
            esemantic("id not found in ST");
        }
    } else if (lookahead == number) {
        match(number);
        toktyp_R = integer;
    } else {
        esemantic("no operand found");
    }
    return toktyp_R;
}
static toktyp factor() {
    if (DEBUG)
        printf("\n *** In factor");
    toktyp toktyp_R = undef;
    if (lookahead == '(') {
        match('(');
        toktyp_R = expr();
        match(')');
    } else if (lookahead == id || lookahead == number) {
        toktyp_R = operand();
    } else {
        esemantic("*** UNEXPECTED SYMBOL ***");
    }
    return toktyp_R;
}
static toktyp term() {
    if (DEBUG)
        printf("\n *** In term");
    toktyp toktyp_R = undef;
    toktyp_R = factor();
    if (lookahead == '*') {
        match('*');
        toktyp_R = get_otype('*', toktyp_R, term());
    }
    return toktyp_R;
}
static toktyp expr() {
    if (DEBUG)
        printf("\n *** In expr");
    toktyp toktyp_R = undef;
    toktyp_R = term();
    if (lookahead == '+') {
        match('+');
        toktyp_R = expr();
    }
    return toktyp_R;
}
static void assign_stat() {
    if (DEBUG)
        printf("\n *** In assign_stat");
    toktyp toktyp_L = undef;
    if (lookahead == id) {
        if (find_name(get_lexeme())) {
            toktyp_L = get_ntype(get_lexeme());
        } else {
            esemantic("assign id not found in ST");
        }
    }
    match(id);
    match(assign);
    if (toktyp_L != expr()) {
        esemantic("type mismatch in assign");
    }
}
static void stat() {
    if (DEBUG)
        printf("\n *** In stat");
    assign_stat();
}
static void stat_list() {
    if (DEBUG)
        printf("\n *** In stat_list");
    stat();
    if (lookahead == ';') {
        match(';');
        stat_list();
    }
}
static void stat_part() {
    if (DEBUG)
        printf("\n *** In stat_part");
    match(begin);

    stat_list();
    match(end);
    match('.');
}
/**********************************************************************/
/*  PUBLIC METHODS for this OBJECT  (EXPORTED)                        */
/**********************************************************************/

int parser() {
    if (DEBUG)
        printf("\n *** In  parser");
    lookahead = get_token(); // get the first token
    if(lookahead != '$'){
        prog_header();
        var_part();
        stat_part();
    } else {
        printf("\n WARNING: input file is empty");
        is_parse_ok = 0;
    }

    if(lookahead == '$'){
        match('$');
    }

    /*if(get_token() != nfound){
            is_parse_ok = 0;
            esyntax("Symbols found after end of parse!");
    }*/

    p_parse_status();
    p_symtab();
    return is_parse_ok; // status indicator
}

/**********************************************************************/
/* End of code                                                        */
/**********************************************************************/
