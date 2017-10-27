#ifndef tokens_h
#define tokens_h
/* tokens.h -- List of labelled tokens and stuff
 *
 * Generated from: mountains.g
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * ANTLR Version 1.33MR33
 */
#define zzEOF_TOKEN 1
#define IS 2
#define IF 3
#define WHILE 4
#define AND 5
#define OR 6
#define NOT 7
#define ENDIF 8
#define ENDWHILE 9
#define LESS 10
#define MORE 11
#define EQUALS 12
#define LESSOREQUAL 13
#define MOREOREQUAL 14
#define SOST 15
#define PER 16
#define COMA 17
#define PLUS 18
#define CIM 19
#define ASC 20
#define CONCAT 21
#define LP 22
#define RP 23
#define DRAW 24
#define PEAK 25
#define VALLEY 26
#define COMPLETE 27
#define MATCH 28
#define HEIGHT 29
#define WELLFORMED 30
#define DESC 31
#define NUM 32
#define ID 33
#define SPACE 34

#ifdef __USE_PROTOS
void mountains(AST**_root);
#else
extern void mountains();
#endif

#ifdef __USE_PROTOS
void assign(AST**_root);
#else
extern void assign();
#endif

#ifdef __USE_PROTOS
void interior(AST**_root);
#else
extern void interior();
#endif

#ifdef __USE_PROTOS
void clause(AST**_root);
#else
extern void clause();
#endif

#ifdef __USE_PROTOS
void peak(AST**_root);
#else
extern void peak();
#endif

#ifdef __USE_PROTOS
void valley(AST**_root);
#else
extern void valley();
#endif

#ifdef __USE_PROTOS
void obj(AST**_root);
#else
extern void obj();
#endif

#ifdef __USE_PROTOS
void draw(AST**_root);
#else
extern void draw();
#endif

#ifdef __USE_PROTOS
void complete(AST**_root);
#else
extern void complete();
#endif

#ifdef __USE_PROTOS
void expr(AST**_root);
#else
extern void expr();
#endif

#ifdef __USE_PROTOS
void term(AST**_root);
#else
extern void term();
#endif

#ifdef __USE_PROTOS
void par(AST**_root);
#else
extern void par();
#endif

#ifdef __USE_PROTOS
void expr_m(AST**_root);
#else
extern void expr_m();
#endif

#ifdef __USE_PROTOS
void term_m(AST**_root);
#else
extern void term_m();
#endif

#ifdef __USE_PROTOS
void par_m(AST**_root);
#else
extern void par_m();
#endif

#ifdef __USE_PROTOS
void condic(AST**_root);
#else
extern void condic();
#endif

#ifdef __USE_PROTOS
void iter(AST**_root);
#else
extern void iter();
#endif

#ifdef __USE_PROTOS
void bool_expr(AST**_root);
#else
extern void bool_expr();
#endif

#ifdef __USE_PROTOS
void bool_term(AST**_root);
#else
extern void bool_term();
#endif

#ifdef __USE_PROTOS
void bool_par(AST**_root);
#else
extern void bool_par();
#endif

#ifdef __USE_PROTOS
void bool_atom(AST**_root);
#else
extern void bool_atom();
#endif

#ifdef __USE_PROTOS
void bool_comp(AST**_root);
#else
extern void bool_comp();
#endif

#ifdef __USE_PROTOS
void bool_simb(AST**_root);
#else
extern void bool_simb();
#endif

#ifdef __USE_PROTOS
void bool_func(AST**_root);
#else
extern void bool_func();
#endif

#ifdef __USE_PROTOS
void match(AST**_root);
#else
extern void match();
#endif

#ifdef __USE_PROTOS
void height(AST**_root);
#else
extern void height();
#endif

#ifdef __USE_PROTOS
void wellformed(AST**_root);
#else
extern void wellformed();
#endif

#endif
extern SetWordType zzerr1[];
extern SetWordType setwd1[];
extern SetWordType zzerr2[];
extern SetWordType zzerr3[];
extern SetWordType zzerr4[];
extern SetWordType zzerr5[];
extern SetWordType setwd2[];
extern SetWordType zzerr6[];
extern SetWordType zzerr7[];
extern SetWordType zzerr8[];
extern SetWordType zzerr9[];
extern SetWordType setwd3[];
extern SetWordType zzerr10[];
extern SetWordType zzerr11[];
extern SetWordType setwd4[];
extern SetWordType zzerr12[];
extern SetWordType zzerr13[];
extern SetWordType zzerr14[];
extern SetWordType zzerr15[];
extern SetWordType zzerr16[];
extern SetWordType setwd5[];
