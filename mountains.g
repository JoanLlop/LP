
#header
<<
#include <string>
#include <iostream>
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
>>

<<
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


int main() {
  root = NULL;
  ANTLR(mountains(&root), stdin);
  ASTPrint(root);
}
>>


#lexclass START
#token IS "is"
#token IF "if"
#token AND "AND"
#token OR "OR"
#token NOT "NOT"
#token ENDIF "endif"
#token LESS "\<"
#token MORE "\>"
#token EQUALS "\=\="
#token LESSOREQUAL "\<\="
#token MOREOREQUAL "\>\="
#token ASC "\/"
#token CIM "\-"
#token DESC "\\"
#token SOST "\#"
#token PER "\*"
#token COMA "\,"
#token PLUS "\+"
#token MINUS "\-"
#token DIV "\/"
#token CONCAT "\;"
#token LP "\("
#token RP "\)"
#token DRAW "Draw"
#token PEAK "Peak"
#token VALLEY "Valley"
#token COMPLETE "Complete"
#token MATCH "Match"
#token HEIGHT "Height"
#token WELLFORMED "Wellformed"
#token NUM "[1-9][0-9]*"
#token ID "[a-zA-Z]+[0-9]*"
#token SPACE "[\ \t \n]" << zzskip(); >>


mountains: (assign /*| condic*/ | draw | /*iter |*/ complete)* << #0 = createASTlist(_sibling); >>;

assign: ID IS^ interior;
interior: clause (CONCAT^ clause)*;
clause: (lit | obj | peak | valley);                     
peak: PEAK^ LP! expr COMA! expr COMA! expr RP!;
valley: VALLEY^ LP! expr COMA! expr COMA! expr RP!;
lit: (NUM | ID | LP! expr RP!) ((PER^ (ASC | DESC | CIM))|);
obj: SOST! ID;

draw: DRAW^ LP! interior RP!;

complete: COMPLETE^ LP! interior RP!;

expr: term ((PLUS^ | MINUS^) term)*;
term: par ((PER^ | DIV^) par)*;
par: (LP! expr RP!) | (NUM | ID);

condic: IF^ LP! bool_expr RP! mountains ENDIF;

bool_expr: bool_term (AND^ bool_term)*;
bool_term: bool_par (OR^ bool_par)*;
bool_par:  (NOT^ | ) (bool_atom /*| (LP! bool_expr RP!)*/);

bool_atom: bool_func | bool_comp;
bool_comp: (expr | height) bool_simb (expr | height); 
bool_simb: (LESS^ | MORE^ | EQUALS^ | LESSOREQUAL^ | MOREOREQUAL^); 
bool_func: (match | wellformed);
match: MATCH^ LP! obj COMA! obj RP!;
height: HEIGHT^ LP! obj RP!;
wellformed: WELLFORMED^ LP! interior RP!;
