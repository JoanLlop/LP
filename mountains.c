/*
 * A n t l r  T r a n s l a t i o n  H e a d e r
 *
 * Terence Parr, Will Cohen, and Hank Dietz: 1989-2001
 * Purdue University Electrical Engineering
 * With AHPCRC, University of Minnesota
 * ANTLR Version 1.33MR33
 *
 *   antlr -gt mountains.g
 *
 */

#define ANTLR_VERSION	13333
#include "pcctscfg.h"
#include "pccts_stdio.h"

#include <string>
#include <iostream>
#include <map>
using namespace std;

// struct to store information about tokens
typedef struct {
  string kind;
  string text;
} Attrib;

// function to fill token information (predeclaration)
void zzcr_attr(Attrib *attr, int type, char *text);

// fields for AST nodes
#define AST_FIELDS string kind; string text;
#include "ast.h"

// macro to create a new AST node (and function predeclaration)
#define zzcr_ast(as,attr,ttype,textt) as=createASTnode(attr,ttype,textt)
AST* createASTnode(Attrib* attr, int ttype, char *textt);
#define GENAST

#include "ast.h"

#define zzSET_SIZE 8
#include "antlr.h"
#include "tokens.h"
#include "dlgdef.h"
#include "mode.h"

/* MR23 In order to remove calls to PURIFY use the antlr -nopurify option */

#ifndef PCCTS_PURIFY
#define PCCTS_PURIFY(r,s) memset((char *) &(r),'\0',(s));
#endif

#include "ast.c"
zzASTgvars

ANTLR_INFO

#include <cstdlib>
#include <cmath>

//global structures
AST *root;


// function to fill token information
void zzcr_attr(Attrib *attr, int type, char *text) {
  if (type == ID) {
    attr->kind = "id";
    attr->text = text;
  }
  else if (type == NUM) {
    attr->kind = "intconst";
    attr->text = text;
  }
  else {
    attr->kind = text;
    attr->text = "";
  }
}

// function to create a new AST node
AST* createASTnode(Attrib* attr, int type, char* text) {
  AST* as = new AST;
  as->kind = attr->kind; 
  as->text = attr->text;
  as->right = NULL; 
  as->down = NULL;
  return as;
}

/// create a new "list" AST node with one element
AST* createASTlist(AST *child) {
  AST *as = new AST;
  as->kind = "list";
  as->right = NULL;
  as->down = child;
  return as;
}

/// get nth child of a tree. Count starts at 0.
/// if no such child, returns NULL
AST* child(AST *a, int n) {
  AST *c = a->down;
  for (int i=0; c!=NULL && i<n; i++) c = c->right;
  return c;
}


/// print AST, recursively, with indentation
void ASTPrintIndent(AST *a, string s) {
  if (a == NULL) return;
  
  cout << a->kind;
  if (a->text != "") cout << "(" << a->text << ")";
  cout << endl;
  
  AST *i = a->down;
  while (i != NULL && i->right != NULL) {
    cout << s+"  \\__";
    ASTPrintIndent(i, s+"  |"+string(i->kind.size()+i->text.size(), ' '));
    i = i->right;
  }
  
  if (i != NULL) {
    cout << s+"  \\__";
    ASTPrintIndent(i, s+"   "+string(i->kind.size()+i->text.size(), ' '));
    i = i->right;
  }
}

/// print AST 
void ASTPrint(AST *a) {
  while (a != NULL) {
    cout << " ";
    ASTPrintIndent(a, "");
    a = a->right;
  }
}

void error (string s){
  cout << s << endl;
  exit(-1);
}

bool es_montanya (AST *a,  map<string, string>& montanyes){
  if (a == NULL) return false;
  string s = a->text.c_str();
  if (a->kind == ";") return true;
  else if (a->kind == "id" and (montanyes.find(s) != montanyes.end())) return true;
  else if (a->kind == "Peak") return true;
  else if (a->kind == "Valley") return true;
  else if (a->kind == "*" and (child(a, 1)->kind == "/" or child(a, 1)->kind == "-" or child(a, 1)->kind == "\\")) return true;
  else return (es_montanya(a->right, montanyes) or es_montanya(a->down, montanyes));
}

int evaluate(AST *a, map<string, int>& constants){
  if (a == NULL) return 0;
  else if (a->kind == "intconst") 
  return atoi(a->text.c_str());
  else if (a->kind == "+")
  return evaluate(a->down, constants) + evaluate(a->down->right, constants);
  else if (a->kind == "-")
  return evaluate(child(a,0), constants) - evaluate(child(a,1), constants);
  else if (a->kind == "*")
  return evaluate(child(a,0), constants) * evaluate(child(a,1), constants);
  else if (a->kind == "/")
  return evaluate(child(a,0), constants) / evaluate(child(a,1), constants);
  else if (a->kind == "id" and constants.find(a->text.c_str()) != constants.end())
  return constants[a->text.c_str()];
  else error ("evaluar no és un int");
}

string convertir_en_muntanya (AST *a, map<string, string>& montanyes, map<string, int>& constants){
  if (a == NULL) return "";
  

}

void execute(AST *a, map<string, string>& montanyes, map<string, int>& constants) {
  if (a == NULL) return;
  else if (a->kind == "list"){
    execute (a->down, montanyes, constants);
  }
  else if (a->kind == "is"){
    if (es_montanya(child(a, 1), montanyes)){
      //montanyes[child(a, 0)->text.c_str()] = convertir_en_muntanya(child(a, 1), montanyes, constants);
    }
    else{
      //cout << "entra: " << child(a, 0)->text.c_str() << endl;
      constants[child(a, 0)->text.c_str()] = evaluate(child(a, 1), constants);
    }
    execute (a->right, montanyes, constants);
  }
  else if (a->kind == "if"){
    
  }
  else if (a->kind == "Draw"){
    
  }
  else if ("while"){
    
  }
  else if ("complete"){
    
  }
  else {
    execute (a->right, montanyes, constants);
  }
}

int main() {
  map<string, string> montanyes;
  map<string, int> constants;
  root = NULL;
  ANTLR(mountains(&root), stdin);
  ASTPrint(root);
  execute(root, montanyes, constants);
  for (auto it = constants.begin(); it != constants.end(); ++it){
    cout << it->first << ": " << it->second << endl;
  }
}

void
#ifdef __USE_PROTOS
mountains(AST**_root)
#else
mountains(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    for (;;) {
      if ( !((setwd1[LA(1)]&0x1))) break;
      if ( (LA(1)==ID) ) {
        assign(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {
        if ( (LA(1)==IF) ) {
          condic(zzSTR); zzlink(_root, &_sibling, &_tail);
        }
        else {
          if ( (LA(1)==DRAW) ) {
            draw(zzSTR); zzlink(_root, &_sibling, &_tail);
          }
          else {
            if ( (LA(1)==WHILE) ) {
              iter(zzSTR); zzlink(_root, &_sibling, &_tail);
            }
            else {
              if ( (LA(1)==COMPLETE) ) {
                complete(zzSTR); zzlink(_root, &_sibling, &_tail);
              }
              else break; /* MR6 code for exiting loop "for sure" */
            }
          }
        }
      }
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  (*_root) = createASTlist(_sibling);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x2);
  }
}

void
#ifdef __USE_PROTOS
assign(AST**_root)
#else
assign(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(IS); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  interior(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x4);
  }
}

void
#ifdef __USE_PROTOS
interior(AST**_root)
#else
interior(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  clause(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (LA(1)==CONCAT) ) {
      zzmatch(CONCAT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      clause(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x8);
  }
}

void
#ifdef __USE_PROTOS
clause(AST**_root)
#else
clause(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (setwd1[LA(1)]&0x10) ) {
      expr_m(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (LA(1)==SOST) ) {
        obj(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {
        if ( (LA(1)==PEAK) ) {
          peak(zzSTR); zzlink(_root, &_sibling, &_tail);
        }
        else {
          if ( (LA(1)==VALLEY) ) {
            valley(zzSTR); zzlink(_root, &_sibling, &_tail);
          }
          else {zzFAIL(1,zzerr1,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
      }
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x20);
  }
}

void
#ifdef __USE_PROTOS
peak(AST**_root)
#else
peak(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(PEAK); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(LP);  zzCONSUME;
  expr(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(COMA);  zzCONSUME;
  expr(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(COMA);  zzCONSUME;
  expr(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(RP);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x40);
  }
}

void
#ifdef __USE_PROTOS
valley(AST**_root)
#else
valley(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(VALLEY); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(LP);  zzCONSUME;
  expr(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(COMA);  zzCONSUME;
  expr(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(COMA);  zzCONSUME;
  expr(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(RP);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd1, 0x80);
  }
}

void
#ifdef __USE_PROTOS
obj(AST**_root)
#else
obj(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(SOST);  zzCONSUME;
  zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x1);
  }
}

void
#ifdef __USE_PROTOS
draw(AST**_root)
#else
draw(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(DRAW); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(LP);  zzCONSUME;
  interior(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(RP);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x2);
  }
}

void
#ifdef __USE_PROTOS
complete(AST**_root)
#else
complete(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(COMPLETE); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(LP);  zzCONSUME;
  zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(RP);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x4);
  }
}

void
#ifdef __USE_PROTOS
expr(AST**_root)
#else
expr(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  term(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (setwd2[LA(1)]&0x8) ) {
      {
        zzBLOCK(zztasp3);
        zzMake0;
        {
        if ( (LA(1)==PLUS) ) {
          zzmatch(PLUS); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        }
        else {
          if ( (LA(1)==CIM) ) {
            zzmatch(CIM); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {zzFAIL(1,zzerr2,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
        zzEXIT(zztasp3);
        }
      }
      term(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x10);
  }
}

void
#ifdef __USE_PROTOS
term(AST**_root)
#else
term(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  par(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (setwd2[LA(1)]&0x20) ) {
      {
        zzBLOCK(zztasp3);
        zzMake0;
        {
        if ( (LA(1)==PER) ) {
          zzmatch(PER); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        }
        else {
          if ( (LA(1)==ASC) ) {
            zzmatch(ASC); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {zzFAIL(1,zzerr3,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
        zzEXIT(zztasp3);
        }
      }
      par(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd2, 0x40);
  }
}

void
#ifdef __USE_PROTOS
par(AST**_root)
#else
par(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (LA(1)==LP) ) {
    {
      zzBLOCK(zztasp2);
      zzMake0;
      {
      zzmatch(LP);  zzCONSUME;
      expr(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzmatch(RP);  zzCONSUME;
      zzEXIT(zztasp2);
      }
    }
  }
  else {
    if ( (setwd2[LA(1)]&0x80) ) {
      {
        zzBLOCK(zztasp2);
        zzMake0;
        {
        if ( (LA(1)==NUM) ) {
          zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
        }
        else {
          if ( (LA(1)==ID) ) {
            zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {zzFAIL(1,zzerr4,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
        zzEXIT(zztasp2);
        }
      }
    }
    else {zzFAIL(1,zzerr5,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x1);
  }
}

void
#ifdef __USE_PROTOS
expr_m(AST**_root)
#else
expr_m(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  term_m(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (setwd3[LA(1)]&0x2) ) {
      {
        zzBLOCK(zztasp3);
        zzMake0;
        {
        if ( (LA(1)==PLUS) ) {
          zzmatch(PLUS); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        }
        else {
          if ( (LA(1)==CIM) ) {
            zzmatch(CIM); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {zzFAIL(1,zzerr6,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
        zzEXIT(zztasp3);
        }
      }
      term_m(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x4);
  }
}

void
#ifdef __USE_PROTOS
term_m(AST**_root)
#else
term_m(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  par_m(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (setwd3[LA(1)]&0x8) ) {
      {
        zzBLOCK(zztasp3);
        zzMake0;
        {
        if ( (LA(1)==PER) ) {
          zzmatch(PER); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        }
        else {
          if ( (LA(1)==ASC) ) {
            zzmatch(ASC); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {zzFAIL(1,zzerr7,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
        }
        zzEXIT(zztasp3);
        }
      }
      par_m(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x10);
  }
}

void
#ifdef __USE_PROTOS
par_m(AST**_root)
#else
par_m(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (LA(1)==LP) ) {
    {
      zzBLOCK(zztasp2);
      zzMake0;
      {
      zzmatch(LP);  zzCONSUME;
      expr_m(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzmatch(RP);  zzCONSUME;
      zzEXIT(zztasp2);
      }
    }
  }
  else {
    if ( (setwd3[LA(1)]&0x20) ) {
      {
        zzBLOCK(zztasp2);
        zzMake0;
        {
        if ( (LA(1)==NUM) ) {
          zzmatch(NUM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
        }
        else {
          if ( (LA(1)==ID) ) {
            zzmatch(ID); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {
            if ( (LA(1)==ASC) ) {
              zzmatch(ASC); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
            }
            else {
              if ( (LA(1)==CIM) ) {
                zzmatch(CIM); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
              }
              else {
                if ( (LA(1)==DESC) ) {
                  zzmatch(DESC); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
                }
                else {zzFAIL(1,zzerr8,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
              }
            }
          }
        }
        zzEXIT(zztasp2);
        }
      }
    }
    else {zzFAIL(1,zzerr9,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x40);
  }
}

void
#ifdef __USE_PROTOS
condic(AST**_root)
#else
condic(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(IF); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(LP);  zzCONSUME;
  bool_expr(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(RP);  zzCONSUME;
  mountains(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(ENDIF);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd3, 0x80);
  }
}

void
#ifdef __USE_PROTOS
iter(AST**_root)
#else
iter(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(WHILE); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(LP);  zzCONSUME;
  bool_expr(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(RP);  zzCONSUME;
  mountains(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(ENDWHILE);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd4, 0x1);
  }
}

void
#ifdef __USE_PROTOS
bool_expr(AST**_root)
#else
bool_expr(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  bool_term(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (LA(1)==AND) ) {
      zzmatch(AND); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      bool_term(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd4, 0x2);
  }
}

void
#ifdef __USE_PROTOS
bool_term(AST**_root)
#else
bool_term(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  bool_par(zzSTR); zzlink(_root, &_sibling, &_tail);
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    while ( (LA(1)==OR) ) {
      zzmatch(OR); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      bool_par(zzSTR); zzlink(_root, &_sibling, &_tail);
      zzLOOP(zztasp2);
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd4, 0x4);
  }
}

void
#ifdef __USE_PROTOS
bool_par(AST**_root)
#else
bool_par(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==NOT) ) {
      zzmatch(NOT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (setwd4[LA(1)]&0x8) ) {
      }
      else {zzFAIL(1,zzerr10,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    bool_atom(zzSTR); zzlink(_root, &_sibling, &_tail);
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd4, 0x10);
  }
}

void
#ifdef __USE_PROTOS
bool_atom(AST**_root)
#else
bool_atom(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  if ( (setwd4[LA(1)]&0x20) ) {
    bool_func(zzSTR); zzlink(_root, &_sibling, &_tail);
  }
  else {
    if ( (setwd4[LA(1)]&0x40) ) {
      bool_comp(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {zzFAIL(1,zzerr11,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd4, 0x80);
  }
}

void
#ifdef __USE_PROTOS
bool_comp(AST**_root)
#else
bool_comp(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (setwd5[LA(1)]&0x1) ) {
      expr(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (LA(1)==HEIGHT) ) {
        height(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {zzFAIL(1,zzerr12,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==LESS) ) {
      zzmatch(LESS); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (LA(1)==MORE) ) {
        zzmatch(MORE); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
      }
      else {
        if ( (LA(1)==EQUALS) ) {
          zzmatch(EQUALS); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
        }
        else {
          if ( (LA(1)==LESSOREQUAL) ) {
            zzmatch(LESSOREQUAL); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {
            if ( (LA(1)==MOREOREQUAL) ) {
              zzmatch(MOREOREQUAL); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
            }
            else {zzFAIL(1,zzerr13,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
          }
        }
      }
    }
    zzEXIT(zztasp2);
    }
  }
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (setwd5[LA(1)]&0x2) ) {
      expr(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (LA(1)==HEIGHT) ) {
        height(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {zzFAIL(1,zzerr14,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd5, 0x4);
  }
}

void
#ifdef __USE_PROTOS
bool_simb(AST**_root)
#else
bool_simb(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==LESS) ) {
      zzmatch(LESS); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
    }
    else {
      if ( (LA(1)==MORE) ) {
        zzmatch(MORE); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
      }
      else {
        if ( (LA(1)==EQUALS) ) {
          zzmatch(EQUALS); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
        }
        else {
          if ( (LA(1)==LESSOREQUAL) ) {
            zzmatch(LESSOREQUAL); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
          }
          else {
            if ( (LA(1)==MOREOREQUAL) ) {
              zzmatch(MOREOREQUAL); zzsubchild(_root, &_sibling, &_tail); zzCONSUME;
            }
            else {zzFAIL(1,zzerr15,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
          }
        }
      }
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd5, 0x8);
  }
}

void
#ifdef __USE_PROTOS
bool_func(AST**_root)
#else
bool_func(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  {
    zzBLOCK(zztasp2);
    zzMake0;
    {
    if ( (LA(1)==MATCH) ) {
      match(zzSTR); zzlink(_root, &_sibling, &_tail);
    }
    else {
      if ( (LA(1)==WELLFORMED) ) {
        wellformed(zzSTR); zzlink(_root, &_sibling, &_tail);
      }
      else {zzFAIL(1,zzerr16,&zzMissSet,&zzMissText,&zzBadTok,&zzBadText,&zzErrk); goto fail;}
    }
    zzEXIT(zztasp2);
    }
  }
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd5, 0x10);
  }
}

void
#ifdef __USE_PROTOS
match(AST**_root)
#else
match(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(MATCH); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(LP);  zzCONSUME;
  obj(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(COMA);  zzCONSUME;
  obj(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(RP);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd5, 0x20);
  }
}

void
#ifdef __USE_PROTOS
height(AST**_root)
#else
height(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(HEIGHT); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(LP);  zzCONSUME;
  obj(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(RP);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd5, 0x40);
  }
}

void
#ifdef __USE_PROTOS
wellformed(AST**_root)
#else
wellformed(_root)
AST **_root;
#endif
{
  zzRULE;
  zzBLOCK(zztasp1);
  zzMake0;
  {
  zzmatch(WELLFORMED); zzsubroot(_root, &_sibling, &_tail); zzCONSUME;
  zzmatch(LP);  zzCONSUME;
  interior(zzSTR); zzlink(_root, &_sibling, &_tail);
  zzmatch(RP);  zzCONSUME;
  zzEXIT(zztasp1);
  return;
fail:
  zzEXIT(zztasp1);
  zzsyn(zzMissText, zzBadTok, (ANTLRChar *)"", zzMissSet, zzMissTok, zzErrk, zzBadText);
  zzresynch(setwd5, 0x80);
  }
}
