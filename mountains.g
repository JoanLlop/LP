
#header
<<
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
  else error ("Valor numeric incorrecte");
}


string convertir_en_muntanya (AST *a, map<string, string>& montanyes, map<string, int>& constants){
  if (a == NULL) return "";
  else if (a->kind == ";"){
    return convertir_en_muntanya(child(a, 0), montanyes, constants) + convertir_en_muntanya(child(a, 1), montanyes, constants);
  }
  else if (a->kind == "*"){
    if (not(child(a, 1)->kind == "/" or child(a, 1)->kind == "-" or child(a, 1)->kind == "\\")) error("S'han de posar parentesis per posar expressions a la definicio de muntanyes");
    int n = evaluate(child(a, 0), constants);
    string r = "";
    for (int i = 0; i < n; ++i){
      if (child(a, 1)->kind == "/" or child(a, 1)->kind == "-" or child(a, 1)->kind == "\\")r += child(a, 1)->kind;
      else error("hi ha numeros i montanyes barrejats!");
    } 
    return r;
  }
  else if (a->kind == "id"){
    if (montanyes.find(a->text.c_str()) != montanyes.end()) return montanyes[a->text.c_str()];
    else error("montanya no definida: " + string(a->text.c_str()));
  }
  else if (a->kind == "Peak"){
    int d, b, c;
    d = evaluate(child(a, 0), constants);
    b = evaluate(child(a, 1), constants);
    c = evaluate(child(a, 2), constants);
    string r = "";
    for (int i = 0; i < d; ++i) r += "/";
    for (int i = 0; i < b; ++i) r += "-";
    for (int i = 0; i < c; ++i) r += "\\";
    return r;
  }
  else if (a->kind == "Valley"){
    int d, b, c;
    d = evaluate(child(a, 0), constants);
    b = evaluate(child(a, 1), constants);
    c = evaluate(child(a, 2), constants);
    string r = "";
    for (int i = 0; i < d; ++i) r += "\\";
    for (int i = 0; i < b; ++i) r += "-";
    for (int i = 0; i < c; ++i) r += "/";
    return r;
  }
  else {
    cout << a->kind;
    error ("Montanya incorrecte, recorda que s'han de posar parentesis per posar expressions a la definicio de muntanyes");
  }
}

int altura (string M){
  int minim = 0;
  int maxim = 0;
  int act   = 0;
  for (int i = 0; i < M.size(); ++i){
    if (M[i] == '/'){
      ++act;
      if (act > maxim) maxim = act;
      if (act < minim) minim = act;
    }
    else if (M[i] == '\\'){
      --act;
      if (act > maxim) maxim = act;
      if (act < minim) minim = act;
    }
    else if (M[i] != '-') error("montanya mal definida: " + M);
  }
  return maxim - minim;
}

bool Match (string M1, string M2){
  return altura(M1) == altura(M2);
}

bool wellformed (string M){
  if (M.size() <= 0) return false;
  int i = 0;
  while (i < M.size()){
    if (M[i] == '/') {
      while (M[i] == '/'){
        ++i;
        if (i >= M.size()) return false;
      }
      if (M[i] != '-') return false;
      while (M[i] == '-'){
        ++i;
        if (i >= M.size()) return false;
      }
      if (M[i] != '\\') return false;
      while (M[i] == '\\'){
        ++i;
        if (i >= M.size()) return false;
      }
    }
    else if (M[i] == '\\'){
      while (M[i] == '\\'){
        ++i;
        if (i >= M.size()) return false;
      }
      if (M[i] != '-') return false;
      while (M[i] == '-'){
        ++i;
        if (i >= M.size()) return false;
      }
      if (M[i] != '/') return false;
      while (M[i] == '/'){
        ++i;
        if (i >= M.size()) return false;
      }
    }
    else return false;
  }
  return true;
}

bool es_true(AST *a, map<string, string>& montanyes, map<string, int>& constants){
  if (a->kind == "OR") {
    return es_true(child(a, 0), montanyes, constants) || es_true(child(a, 1), montanyes, constants);
  }
  else if (a->kind == "AND"){
    return es_true(child(a, 0), montanyes, constants) && es_true(child(a, 1), montanyes, constants);
  }
  else if (a->kind == "NOT"){
    return not (es_true(child(a, 0), montanyes, constants));
  }
  else if (a->kind == "<" or a->kind == ">" or a->kind=="=="){
    int p, q;
    if (child(a, 0)->kind == "Height"){
      if (es_montanya(child(child(a, 0), 0), montanyes)){
        p = altura (convertir_en_muntanya(child(child(a, 0), 0), montanyes, constants));
      }
      else error ("Height: " + string(child(child(a, 0), 0)->text.c_str())+" no es una montanya o no esta definida");
    }
    else {
      p = evaluate(child(a, 0), constants);
    }

    if (child(a, 1)->kind == "Height"){
      if (es_montanya(child(child(a, 1), 0), montanyes)){
        q = altura (convertir_en_muntanya(child(child(a, 1), 0), montanyes, constants));
      }
      else error ("Height: " + string(child(child(a, 1), 0)->text.c_str())+" no es una montanya o no esta definida");
    }
    else q = evaluate(child(a, 1), constants);


    if (a->kind == "<") return p < q;
    else if (a->kind == ">") return p > q;
    else if (a->kind == "==") return p == q;
  }
  else if (a->kind == "Match"){
    if (es_montanya(child(a, 0), montanyes) and es_montanya(child(a, 1), montanyes)){
      return (altura(convertir_en_muntanya(child(a, 0), montanyes, constants)) == altura(convertir_en_muntanya(child(a, 1), montanyes, constants)));
    }
    else error("no es una montanya");
  }
  else if (a->kind == "Wellformed"){
    if (es_montanya(child(a, 0), montanyes)){
      return wellformed(convertir_en_muntanya(child(a, 0), montanyes, constants));
    }
    else error("Wellformed: montanya no definida.");
  }
  else {
    error("condició no booleana");
  }
}

void complete(AST *a, map<string, string>& montanyes, map<string, int>& constants) {
  if (montanyes.find(a->text.c_str()) != montanyes.end()){
    string s = montanyes[a->text.c_str()];
    int i = 0;
    if (s[i] == '/'){
      while (s[i] == '/') {
        ++i;
        if (i >= s.size()){
          s += "-\\";
          montanyes[a->text.c_str()] = s;
          return;
        }
      }
      if (s[i] != '-') error ("Completar no es possible");
      while (s[i] == '-') {
        ++i;
        if (i >= s.size()){
          s += "\\";
          montanyes[a->text.c_str()] = s;
          return;
        }
      }
      if (s[i] != '\\') error ("Completar no es possible");
      while (s[i] == '\\') {
        ++i;
        if (i >= s.size()){
          montanyes[a->text.c_str()] = s;
          return;
        }
      }
    }
    else if (s[i] == '\\'){
      while (s[i] == '\\') {
        ++i;
        if (i >= s.size()){
          s += "-/";
          montanyes[a->text.c_str()] = s;
          return;
        }
      }
      if (s[i] != '-') error ("Completar no es possible");
      while (s[i] == '-') {
        ++i;
        if (i >= s.size()){
          s += "/";
          montanyes[a->text.c_str()] = s;
          return;
        }
      }
      if (s[i] != '/') error ("Completar no es possible");
      while (s[i] == '/') {
        ++i;
        if (i >= s.size()){
          montanyes[a->text.c_str()] = s;
          return;
        }
      }
    }
    else error ("Completar no es possible");
  }
  else error("No s'ha trobat la montanya per a completar");
}


void execute(AST *a, map<string, string>& montanyes, map<string, int>& constants) {
  if (a == NULL) return;
  else if (a->kind == "list"){
    execute (a->down, montanyes, constants);
  }
  else if (a->kind == "is"){
    if (es_montanya(child(a, 1), montanyes)){
      montanyes[child(a, 0)->text.c_str()] = convertir_en_muntanya(child(a, 1), montanyes, constants);
    }
    else{
      constants[child(a, 0)->text.c_str()] = evaluate(child(a, 1), constants);
    }
    execute (a->right, montanyes, constants);
  }
  else if (a->kind == "if"){
    if (es_true(child(a, 0), montanyes, constants)){
      execute(child(a, 1), montanyes, constants);
    }
    execute (a->right, montanyes, constants);
  }
  else if (a->kind == "Draw"){
    if (es_montanya(child(a, 0), montanyes)){
      cout << convertir_en_muntanya(child(a, 0), montanyes, constants) << endl;
      execute (a->right, montanyes, constants);

    }
    else error("Draw no conté cap muntanya");
  }
  else if (a->kind == "while"){
    while (es_true(child(a, 0), montanyes, constants)){
      execute(child(a, 1), montanyes, constants);
    }
    execute (a->right, montanyes, constants);
  }
  else if (a->kind == "Complete"){
    complete (child(a, 0), montanyes, constants);
    execute (a->right, montanyes, constants);
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
  cout << "----------------------------------------" << endl;
  for (auto it = montanyes.begin(); it != montanyes.end(); ++it){
    cout << "l'altitud final de " << it->first << " es: " << altura(it->second) << endl;
  }
}
>>


#lexclass START
#token IS "is"
#token IF "if"
#token WHILE "While"
#token AND "AND"
#token OR "OR"
#token NOT "NOT"
#token ENDIF "endif"
#token ENDWHILE "endwhile"
#token LESS "\<"
#token MORE "\>"
#token EQUALS "\=\="
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
#token ASC "\/"
#token CIM "\-"
#token DESC "\\"
#token NUM "[0-9]*"
#token ID "[A-Z]+[0-9]*"
#token SPACE "[\ \t \n]" << zzskip(); >>


mountains: (assign | condic | draw | iter | complete)* << #0 = createASTlist(_sibling); >>;

assign: ID IS^ interior;
interior: clause (CONCAT^ clause)*;
clause: ( expr_m | obj | peak | valley);                     
peak: PEAK^ LP! expr COMA! expr COMA! expr RP!;
valley: VALLEY^ LP! expr COMA! expr COMA! expr RP!;
obj: SOST! ID;


draw: DRAW^ LP! interior RP!;

complete: COMPLETE^ LP! ID RP!;

expr: term ((PLUS^ | MINUS^) term)*;
term: par ((PER^ | DIV^) par)*;
par: (LP! expr RP!) | (NUM | ID);

expr_m: term_m ((PLUS^ | MINUS^) term_m)*;
term_m: par_m ((PER^ | DIV^) par_m)*;
par_m: (LP! expr_m RP!) | (NUM | ID | ASC | CIM | DESC);

condic: IF^ LP! bool_expr RP! mountains ENDIF!;

iter: WHILE^ LP! bool_expr RP! mountains ENDWHILE!;

bool_expr: bool_term (AND^ bool_term)*; 
bool_term: bool_par (OR^ bool_par)*;
bool_par:  (NOT^ | ) bool_atom;//per incompatibilitat amb expr no puc posar gerarquia de paréntesis. Que s'afegiria algo així al final: | LP! bool_expr RP!.

bool_atom: (match | wellformed | bool_comp);
bool_comp: (expr | height) (LESS^ | MORE^ | EQUALS^) (expr | height);

match: MATCH^ LP! obj COMA! obj RP!;
height: HEIGHT^ LP! obj RP!;
wellformed: WELLFORMED^ LP! interior RP!;
