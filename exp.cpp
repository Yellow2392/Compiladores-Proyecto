#include <iostream>
#include "exp.h"
using namespace std;
IFExp::IFExp(Exp* cond,Exp* l, Exp* r): cond(cond),left(l),right(r){}
BinaryExp::BinaryExp(Exp* l, Exp* r, BinaryOp op):left(l),right(r),op(op) {}
NumberExp::NumberExp(int v):value(v) {}
BoolExp::BoolExp(bool v):value(v) {}
IdentifierExp::IdentifierExp(const string& n):name(n) {}
FCallExp::FCallExp(string fname, list<Exp*> args):fname(fname),args(args) {}
Exp::~Exp() {}
BinaryExp::~BinaryExp() { delete left; delete right; }
IFExp::~IFExp() {delete cond, delete left; delete right; }
NumberExp::~NumberExp() { }
BoolExp::~BoolExp() { }
IdentifierExp::~IdentifierExp() { }
FCallExp::~FCallExp() { 
    while (!args.empty()) {
        delete args.front();
        args.pop_front();
    }
}
FCallStatement::FCallStatement(string fname, list<Exp*> args): fname(fname), args(args) {}
FCallStatement::~FCallStatement() {
    while (!args.empty()) {
        delete args.front();
        args.pop_front();
    }
}
AssignStatement::AssignStatement(string id, Exp* e): id(id), rhs(e) {}
AssignStatement::~AssignStatement() {
    delete rhs;
}
PrintStatement::PrintStatement(Exp* e): e(e) {}
PrintStatement::~PrintStatement() {
    delete e;
}

IfStatement::IfStatement(Exp* c, Body* t, Body* e): condition(c), then(t), els(e) {}
IfStatement::~IfStatement() {
    delete condition;
    delete then;
    delete els;
}
WhileStatement::WhileStatement(Exp* c, Body* t): condition(c), b(t) {}
WhileStatement::~WhileStatement() {
    delete condition;
    delete b;
}

DoWhileStatement::DoWhileStatement(Exp* c, Body* t): condition(c), b(t) {}
DoWhileStatement::~DoWhileStatement() {
    delete condition;
    delete b;
}

ForStatement::ForStatement(std::string id, Exp* s, Exp* end, Body* b): id(id), start(s), end(end), b(b) {}
ForStatement::~ForStatement() {
    delete start;
    delete end;
    delete b;
}

VarDec::VarDec(string type, list<string> vars): type(type), vars(vars) {}
VarDec::~VarDec() {}

VarDecList::VarDecList(): vardecs() {}
void VarDecList::add(VarDec* v) {
    vardecs.push_back(v);
}
VarDecList::~VarDecList() {
    for (auto v: vardecs) {
        delete v;
    }
}

StatementList::StatementList(): stms() {}
void StatementList::add(Stm* s) {
    stms.push_back(s);
}
StatementList::~StatementList() {
    for (auto s: stms) {
        delete s;
    }
}

Body::Body(VarDecList* v, StatementList* s): vardecs(v), slist(s) {}
Body::~Body() {
    delete vardecs;
    delete slist;
}

FunDec::FunDec(string fname, list<string> types, list<string> vars, string rtype, Body* b): fname(fname), types(types), vars(vars), rtype(rtype), body(b) {}
//FunDec::FunDec(string fname, list<string> types, list<string> vars, string rtype, Body* b, Exp* e): fname(fname), types(types), vars(vars), rtype(rtype), body(b), e(e) {}

FunDec::~FunDec() {
    delete body;
}

FunDecList::FunDecList(): flist() {}
void FunDecList::add(FunDec* f) {
    flist.push_back(f);
}
FunDecList::~FunDecList() {
    for (auto f: flist) {
        delete f;
    }
}

ReturnStatement::ReturnStatement(Exp* e): e(e) {}
ReturnStatement::~ReturnStatement() {
    delete e;
}

Program::Program(VarDecList* v, FunDecList* f): vardecs(v), fundecs(f) {}

Program::~Program() {
    delete vardecs;
    delete fundecs;
}
Stm::~Stm() {}
string Exp::binopToChar(BinaryOp op) {
    string  c;
    switch(op) {
        case PLUS_OP: c = "+"; break;
        case MINUS_OP: c = "-"; break;
        case MUL_OP: c = "*"; break;
        case DIV_OP: c = "/"; break;
        case LT_OP: c = "<"; break;
        case LE_OP: c = "<="; break;
        case EQ_OP: c = "=="; break;
        case GT_OP: c = ">"; break;
        case GE_OP: c = ">="; break;
        case NEQ_OP: c = "!="; break;
        default: c = "$";
    }
    return c;
}