/* -*- C -*- */
/* I hate this bloody country. Smash. */
/* This file is a part of Metaresc project */
%option 8bit reentrant bison-bridge bison-locations
%option warn nounput never-interactive noyywrap
%option extra-type="mr_load_t*"
%option prefix="mr_yaml1_"
%{
#include <string.h>
#include <complex.h>
#include <math.h>
#include <ctype.h>

#include <metaresc.h>
#include <flt_values.h>
#include <lexer.h>
#define MR_YAML1_LTYPE mr_token_lloc_t
#include <yaml1_load.tab.h>
#define YYSTYPE MR_YAML1_STYPE
#define YYLTYPE MR_YAML1_LTYPE

// -----------------------------------------------------------------------------
// Helper structure which will hold all stack info.
typedef struct {
    int* array;
    int top;
    int size;
} token_stack;

token_stack* ts_create() {
    token_stack* stack = (token_stack*)malloc(sizeof(token_stack));
    stack->array = (int*)malloc(10 * sizeof(int));
    stack->top = -1;
    stack->size = 10;
    return stack;
}

void ts_push(token_stack* stack, int item) {
    if (stack->top == stack->size - 2) {
        stack->size *= 2;
        stack->array = (int*)realloc(stack->array, stack->size * sizeof(int));
    }
    stack->array[++stack->top] = item;
}

void ts_display(token_stack* stack) {
    if (stack->top != -1) {
        printf("Stack is empty.\n");
        return;
    }
    printf("Stack: ");
    for (int i = stack->top; i >= 0; i--) {
        printf("%d ", stack->array[i]);
    }
    printf("\n");
}

int ts_pop(token_stack* stack) {
    return (stack != NULL && stack->top != -1) ? stack->array[stack->top--] : -1;
}

int ts_peek(token_stack* stack) {
    return (stack != NULL && stack->top != -1) ? stack->array[stack->top] : -1;
}

void ts_destroy(token_stack* stack) {
    free(stack->array);
    free(stack);
}

#define TOKEN_STACK_PUSH(token)                         \
    do {                                                \
        if (t_stack == NULL)                            \
            t_stack = ts_create();                      \
        ts_push(t_stack, token);                        \
    } while (0)

#define STACK_POP_N_TOKENS(amount)                      \
    do {                                                \
        int current_token = -1;                         \
        for (int i = 0; i < amount; i++) {              \
            current_token = ts_pop(t_stack);            \
            if (current_token != -1) {                  \
                SEND_TOKEN(current_token);              \
            }                                           \
        }                                               \
    } while (0)

// -----------------------------------------------------------------------------
// Definitions
#define SEND_TOKEN(token) do {                          \
  if (pstate == NULL)                                   \
  {                                                     \
    pstate = mr_yaml1_pstate_new();                     \
  }                                                     \
  mr_yaml1_push_parse(pstate, token, yylval, yylloc, yyscanner);           \
  last_pushed_token = token;                            \
} while (0)

#define CLEANUP()                                       \
    do {                                                \
        if (pstate != NULL) {                           \
            mr_yaml1_pstate_delete(pstate);             \
        }                                               \
        if (t_stack != NULL) {                          \
          ts_destroy(t_stack);                          \
        }                                               \
    } while (0)

int indent_level = 0;
int last_pushed_token = -1;
token_stack* t_stack = NULL;

mr_yaml1_pstate *pstate = NULL;
// -----------------------------------------------------------------------------
%}
%option noyywrap

DASH                  "-"
COLON                 ":"
SPACE                 " "
INDENT                "  "
NULL                  "null"
TRUE                  "true"
FALSE                 "false"
NEWLINE               "\n"
YAML_BLOCK_START      {DASH}{DASH}{DASH}{NEWLINE}
YAML_BLOCK_END        "..."
STRING                "\""([^\"\\]|\\.)*"\""
INT_NUMBER            [+-]?[[:digit:]]+
FLOAT_NAN             [+-]?[Nn][Aa][Nn]
FLOAT_INF             [+-]?[Ii][Nn][Ff]
FLOAT_NUMBER          {FLOAT_NAN}|{FLOAT_INF}|{INT_NUMBER}("."[[:digit:]]*)?([eE]{INT_NUMBER})?
KEY                   [a-zA-Z_][a-zA-Z0-9_\-]*
ARR_START             {DASH}{SPACE}
%%

{NULL}                  { SEND_TOKEN(TOK_YAML1_NULL); }
{TRUE}                  { 
                          yylval->value.value_type = MR_VT_INT; 
                          yylval->value.vt_int = true; 
                          return (TOK_YAML1_NUMBER); 
                        }
{FALSE}                 { 
                          yylval->value.value_type = MR_VT_INT; 
                          yylval->value.vt_int = false; 
                          return (TOK_YAML1_NUMBER); 
                        }
{STRING}                {
                          yylval->string.str = &yytext[1];
                          yylval->string.length = strlen (yytext) - 2 * sizeof (yytext[0]);
                          return (TOK_YAML1_STRING); 
                        }
{INT_NUMBER}            { 
                          yylval->value.value_type = MR_VT_INT; 
                          yylval->value.vt_int = mr_strtouintmax (yytext, NULL, 0); 
                          return (TOK_YAML1_NUMBER); 
                        }
{FLOAT_NUMBER}          { 
                          yylval->value.value_type = MR_VT_FLOAT; 
                          yylval->value.vt_float = strtold (yytext, NULL); 
                          return (TOK_YAML1_NUMBER); 
                        }
{COLON}                 { SEND_TOKEN(TOK_YAML1_COLON);}
{ARR_START}             {
                          /*
                            Nested array should have the name (so last token
                            would be a colon).
                            Also, array element can be very first element of
                            file.
                          */
                          if (ts_peek(t_stack) != TOK_YAML1_ARR_END \
                            || last_pushed_token == TOK_YAML1_COLON)
                            {
                              SEND_TOKEN(TOK_YAML1_ARR_START);
                              TOKEN_STACK_PUSH(TOK_YAML1_ARR_END);
                            }
                        }
{NEWLINE}{INDENT}+{ARR_START}*       { 
                          int new_indent_level = (yyleng - 1) / 2;
                          int diff = new_indent_level - indent_level;
                          /* 
                            If indentation level wasn't changed, then next
                            element exists and belongs to current struct.
                          */
                          if (diff == 0) { SEND_TOKEN(TOK_YAML1_NEWLINE); }
                          else if (diff < 0) { STACK_POP_N_TOKENS(-diff); }
                          /*
                            Bad magic done with indentation level.
                            Now I can't move the dash processing logic elsewhere
                            because of indent level update process.
                          */
                          if (yytext[yyleng - 2] == '-')
                          {
                            if (ts_peek(t_stack) != TOK_YAML1_ARR_END \
                            || last_pushed_token == TOK_YAML1_COLON)
                            {
                              SEND_TOKEN(TOK_YAML1_ARR_START);
                              TOKEN_STACK_PUSH(TOK_YAML1_ARR_END);
                            }
                          }
                          indent_level = new_indent_level;
                        }
{YAML_BLOCK_START}      { SEND_TOKEN(TOK_YAML1_BLOCK_START); }
{KEY}                   {
                          /*
                            - If a structure was packed inside an array,
                              last pushed token would be an ARRAY_START marker.
                            - If a structure was packed inside another struct,
                              last pushed token would be a COLON marker.
                          */
                          if (last_pushed_token == TOK_YAML1_ARR_START \
                          || last_pushed_token == TOK_YAML1_COLON){
                            SEND_TOKEN(TOK_YAML1_OBJ_START);
                            TOKEN_STACK_PUSH(TOK_YAML1_OBJ_END);
                          }
                          /* 
                            If something was present after a nested 
                            struct / array, then add a separator between elems.
                          */
                          if (last_pushed_token == TOK_YAML1_ARR_END \
                          || last_pushed_token == TOK_YAML1_OBJ_END)
                          {
                            SEND_TOKEN(TOK_YAML1_NEWLINE);
                          }
                          SEND_TOKEN(TOK_YAML1_KEY);
                        }
{YAML_BLOCK_END}        {
                          STACK_POP_N_TOKENS(indent_level);
                          indent_level = 0;
                          CLEANUP();
                          return TOK_YAML1_BLOCK_END; 
                        }
<<EOF>>                 {  
                          STACK_POP_N_TOKENS(indent_level);
                          indent_level = 0;
                          //SEND_TOKEN(TOK_YAML1_BLOCK_END);
                          CLEANUP();
                          return 0;  
                        }
.                       { /* Ignore all other characters */ }


