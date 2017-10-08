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
#define AND 4
#define OR 5
#define NOT 6
#define ENDIF 7
#define LESS 8
#define MORE 9
#define EQUALS 10
#define LESSOREQUAL 11
#define MOREOREQUAL 12
#define DIV 13
#define MINUS 14
#define DESC 15
#define SOST 16
#define PER 17
#define COMA 18
#define PLUS 19
#define CONCAT 20
#define LP 21
#define RP 22
#define DRAW 23
#define PEAK 24
#define VALLEY 25
#define COMPLETE 26
#define MATCH 27
#define HEIGHT 28
#define WELLFORMED 29
#define NUM 30
#define ID 31
#define SPACE 32

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
void lit(AST**_root);
#else
extern void lit();
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
void condic(AST**_root);
#else
extern void condic();
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
extern SetWordType zzerr2[];
extern SetWordType zzerr3[];
extern SetWordType setwd1[];
extern SetWordType zzerr4[];
extern SetWordType zzerr5[];
extern SetWordType zzerr6[];
extern SetWordType setwd2[];
extern SetWordType zzerr7[];
extern SetWordType zzerr8[];
extern SetWordType zzerr9[];
extern SetWordType setwd3[];
extern SetWordType zzerr10[];
extern SetWordType zzerr11[];
extern SetWordType zzerr12[];
extern SetWordType zzerr13[];
extern SetWordType zzerr14[];
extern SetWordType setwd4[];
extern SetWordType setwd5[];
