#include <iostream>
#include <stdexcept>
#include "token.h"
#include "scanner.h"
#include "exp.h"
#include "parser.h"

using namespace std;

bool Parser::match(Token::Type ttype) {
    if (check(ttype)) {
        advance();
        return true;
    }
    return false;
}

bool Parser::check(Token::Type ttype) {
    if (isAtEnd()) return false;
    return current->type == ttype;
}

bool Parser::advance() {
    if (!isAtEnd()) {
        Token* temp = current;
        if (previous) delete previous;
        current = scanner->nextToken();
        previous = temp;
        if (check(Token::ERR)) {
            cout << "Error de análisis, carácter no reconocido: " << current->text << endl;
            exit(1);
        }
        return true;
    }
    return false;
}

bool Parser::isAtEnd() {
    return (current->type == Token::END);
}

Parser::Parser(Scanner* sc):scanner(sc) {
    previous = NULL;
    current = scanner->nextToken();
    if (current->type == Token::ERR) {
        cout << "Error en el primer token: " << current->text << endl;
        exit(1);
    }
}

VarDec* Parser::parseVarDec() {
    VarDec* vd = NULL;
    if (match(Token::VAR)) { //considera usar diferenciacion para 'val' y 'var'
        list<string> ids;
        if (!match(Token::ID)) {
            cout << "Error: se esperaba un identificador después de 'var'." << endl;
            exit(1);
        }
        ids.push_back(previous->text);
        if (!match(Token::TWODOT)) {
            cout << "Error: se esperaba un ':' después de 'id'." << endl;
            exit(1);
        }
        
        if (!match(Token::ID)) {
            cout << "Error: se esperaba un identificador después de 'var'." << endl;
            exit(1);
        }
        string type = previous->text;
        //if (!match(Token::PC)) {
        //    cout << "Error: se esperaba un ';' al final de la declaración." << endl;
        //    exit(1);
        //}
        if (check(Token::PC)) {
            match(Token::PC); // Consumir ';' si está presente
        }
        vd = new VarDec(type, ids);
    }
    else if (match(Token::VAL)) { //considera usar diferenciacion para 'val' y 'var'
        list<string> ids;
        if (!match(Token::ID)) {
            cout << "Error: se esperaba un identificador después de 'var'." << endl;
            exit(1);
        }
        ids.push_back(previous->text);
        if (!match(Token::TWODOT)) {
            cout << "Error: se esperaba un ':' después de 'id'." << endl;
            exit(1);
        }
        
        if (!match(Token::ID)) {
            cout << "Error: se esperaba un identificador después de 'var'." << endl;
            exit(1);
        }
        string type = previous->text;
        //if (!match(Token::PC)) {
        //    cout << "Error: se esperaba un ';' al final de la declaración." << endl;
        //    exit(1);
        //}
        if (check(Token::PC)) {
            match(Token::PC); // Consumir ';' si está presente
        }
        vd = new VarDec(type, ids);
    }
    return vd;
}

VarDecList* Parser::parseVarDecList() {
    VarDecList* vdl = new VarDecList();
    VarDec* aux;
    aux = parseVarDec();
    while (aux != NULL) {
        vdl->add(aux);
        aux = parseVarDec();
    }
    return vdl;
}

StatementList* Parser::parseStatementList() {
    StatementList* sl = new StatementList();
    //sl->add(parseStatement());
    //while (match(Token::PC)) {
    //    sl->add(parseStatement());
    //}

    if (check(Token::ID) || check(Token::PRINT) || check(Token::FOR) || 
    check(Token::WHILE) || check(Token::DO) || check(Token::IF)) /* u otros tokens relevantes */ {
        // Si hay un statement, lo procesamos y lo agregamos a la lista
        sl->add(parseStatement());

        // Después seguimos parseando si encontramos un ';'
        while (match(Token::PC)) {
            sl->add(parseStatement());
        }
    }

    return sl;
}


Body* Parser::parseBody() {
    VarDecList* vdl = parseVarDecList();
    StatementList* sl = parseStatementList();
    return new Body(vdl, sl);
}

FunDec* Parser::parseFunDec() {
  FunDec* fd = NULL;
  if (match(Token::FUN)) {
    Body* body = NULL;
    if (!match(Token::ID)) {
        cout << "Error: se esperaba un nombre de función después del tipo." << endl;
        exit(1);
    }
    string fname = previous->text;
    bool flagIsMain = fname == "main";
    if (!match(Token::PI)) {
        cout << "Error: se esperaba un '(' después del nombre de la función." << endl;
        exit(1);
    }
    list<string> types;
    list<string> vars;
    string rtype;
    if (!check(Token::PD)) {
        //cambia sintaxis
        if (!match(Token::ID)) {
            cout << "Error: se esperaba un ID después del '('." << endl;
            exit(1);
        }
        vars.push_back(previous->text);
        if (!match(Token::TWODOT)) {
            cout << "Error: se esperaba ':' después del ID." << endl;
            exit(1);
        }
        if (!match(Token::ID)) {
            cout << "Error: se esperaba el tipo de la variable después de ':'." << endl;
            exit(1);
        }
        types.push_back(previous->text);
        while(match(Token::COMA)) {
            if(!match(Token::ID)) {
            cout << "Error: se esperaba un ID después de ','." << endl;
            exit(1);
            }
            vars.push_back(previous->text);
            if (!match(Token::TWODOT)) {
                cout << "Error: se esperaba ':' después del ID." << endl;
                exit(1);
            }
            if (!match(Token::ID)) {
                cout << "Error: se esperaba el tipo de la variable después de ':'." << endl;
                exit(1);
            }
            types.push_back(previous->text);
        }
    }
    if (!match(Token::PD)) {
        cout << "Error: se esperaba un ')' después de la lista de argumentos." << endl;
        exit(1);
    }
    if (fname != "main") {
        if (!match(Token::TWODOT)) {
            cout << "Error: se esperaba un ':' después del paréntesis derecho." << endl;
            exit(1);
        }
        if (!match(Token::ID)) {
            cout << "Error: se esperaba un tipo 'fun'." << endl;
            exit(1);
        }
        rtype = previous->text;
    } 
    else {
        rtype = "Void"; // Asignamos Void como tipo predeterminado para main
    }

    if (!match(Token::LLI)) {
        cout << "Error: se esperaba un '{' después de la lista de argumentos." << endl;
        exit(1);
    }
    body = parseBody();
    if(!flagIsMain){
        if (!match(Token::RETURN)){
            cout << "Error: se esperaba el return de la función." << endl;
            exit(1);
        }
        Exp* e;
        e = parseCExp();
        if (!match(Token::LLD)){
            cout << "Error: se esperaba '}' al final de la declaración." << endl;
            exit(1);
        }
        fd = new FunDec(fname, types, vars, rtype, body,e);
    }
    else{
        if (!match(Token::LLD)){
            cout << "Error: se esperaba '}' al final de la declaración." << endl;
            exit(1);
        }
        fd = new FunDec(fname, types, vars, rtype, body);
    }
    
  }
  return fd;
}

FunDecList* Parser::parseFunDecList() {
    FunDecList* fdl = new FunDecList();
    FunDec* aux;
    aux = parseFunDec();
    while (aux != NULL) {
        fdl->add(aux);
        aux = parseFunDec();
    }
    return fdl;
}

Program* Parser::parseProgram() {
    VarDecList* v = parseVarDecList();
    FunDecList* b = parseFunDecList();
    return new Program(v, b);
}

list<Stm*> Parser::parseStmList() {
    list<Stm*> slist;
    slist.push_back(parseStatement());
    while(match(Token::PC)) {
        slist.push_back(parseStatement());
    }
    return slist;
}

Stm* Parser::parseStatement() {
    Stm* s = NULL;
    Exp* e = NULL;
    Body* tb = NULL; //true case
    Body* fb = NULL; //false case
    if (current == NULL) {
        cout << "Error: Token actual es NULL" << endl;
        exit(1);
    }

    if (match(Token::ID)) {
        string lex = previous->text;

        if (match(Token::ASSIGN)) {
            e = parseCExp();
            s = new AssignStatement(lex, e);
        }
        else if (match(Token::PI)){
            //fcallstmt
            list<Exp*> args;
            if (!check(Token::PD)){
                args.push_back(parseCExp());
                while (match(Token::COMA)){
                    args.push_back(parseCExp());
                }
            }
            if (!match(Token::PD)){
                cout << "Falta paréntesis derecho" << endl;
                exit(0);
            }
            return new FCallStatement(lex, args);
        }
        
    } else if (match(Token::PRINT)) {
        if (!match(Token::PI)) {
            cout << "Error: se esperaba un '(' después de 'print'." << endl;
            exit(1);
        }
        e = parseCExp();
        if (!match(Token::PD)) {
            cout << "Error: se esperaba un ')' después de la expresión." << endl;
            exit(1);
        }
        s = new PrintStatement(e);
    }
    else if (match(Token::IF)) {
        if (!match(Token::PI)) {
            cout << "Error: se esperaba '(' después de la expresión." << endl;
            exit(1);
        }
        e = parseCExp();
        if (!match(Token::PD)) {
            cout << "Error: se esperaba ')' después de la expresión." << endl;
            exit(1);
        }
        if (!match(Token::LLI)) {
            cout << "Error: se esperaba '{' después de la expresión." << endl;
            exit(1);
        }
        
        tb = parseBody();
        if (!match(Token::LLD)) { //ENDIF
            cout << "Error: se esperaba '}' después de la expresión." << endl;
            exit(1);
        }
        if (match(Token::ELSE)) {
            if (!match(Token::LLI)) {
            cout << "Error: se esperaba '{' después de la expresión." << endl;
            exit(1);
            }
            fb = parseBody();
            if (!match(Token::LLD)) {
            cout << "Error: se esperaba '}' después de la expresión." << endl;
            exit(1);
            }
        }
        s = new IfStatement(e, tb, fb);

    }
    else if (match(Token::WHILE)) {
        if (!match(Token::PI)) {
            cout << "Error: se esperaba '(' después de la expresión." << endl;
            exit(1);
        }
        e = parseCExp();
        if (!match(Token::PD)) {
            cout << "Error: se esperaba ')' después de la expresión." << endl;
            exit(1);
        }
        if (!match(Token::LLI)) {
            cout << "Error: se esperaba '{' después de la expresión." << endl;
            exit(1);
        }
        tb = parseBody();
        if (!match(Token::LLD)) {
            cout << "Error: se esperaba '}' después de la expresión." << endl;
            exit(1);
        }
        s = new WhileStatement(e, tb);
    }
    else if (match(Token::DO)){
        if (!match(Token::LLI)) {
            cout << "Error: se esperaba '{' después de la expresión." << endl;
            exit(1);
        }
        tb = parseBody();
        if (!match(Token::LLD)) {
            cout << "Error: se esperaba '}' después de la expresión." << endl;
            exit(1);
        }
        if (!match(Token::WHILE)) {
            cout << "Error: se esperaba un 'while' después de la expresión." << endl;
            exit(1);
        }
        if (!match(Token::PI)) {
            cout << "Error: se esperaba '(' después de la expresión." << endl;
            exit(1);
        }
        e = parseCExp();
        if (!match(Token::PD)) {
            cout << "Error: se esperaba ')' después de la expresión." << endl;
            exit(1);
        }
        s = new DoWhileStatement(e, tb);
    }
    else if(match(Token::FOR)){
        if(!match(Token::PI)){
            cout << "Error: se esperaba '(' después de 'for'." << endl;
            exit(1);
        }
        if (!match(Token::ID)) {
            cout << "Error: se esperaba un identificador después del tipo." << endl;
            exit(1);
        }
        string id = previous->text;
        if (!match(Token::IN)) {
            cout << "Error: se esperaba 'in' después de la expresión." << endl;
            exit(1);
        }
        Exp* start = parseCExp();
        //cout << previous->text;
        if (!match(Token::DOTS)) {
            cout << "Error: se esperaba '..' después de la expresión." << endl;
            exit(1);
        }
        Exp* end = parseCExp();
        if (!match(Token::PD)) {
            cout << "Error: se esperaba ')' después de la expresión." << endl;
            exit(1);
        }
        if (!match(Token::LLI)) {
            cout << "Error: se esperaba '{' después de la expresión." << endl;
            exit(1);
        }
        tb = parseBody();
        if (!match(Token::LLD)) {
            cout << "Error: se esperaba '}' después de la expresión." << endl;
            exit(1);
        }
        s = new ForStatement(id, start, end, tb);
    }
    //else if (match(Token::RETURN)){
    //    e = parseCExp();
    //    s = new ReturnStatement(e); //Si es null, no hay problema
    //}
    else {
        cout << "Error: Se esperaba un identificador o 'print', pero se encontró: " << *current << endl;
        exit(1);
    }
    return s;
}

Exp* Parser::parseCExp(){
    Exp* left = parseExpression();
    if (match(Token::LT) || match(Token::LE) || match(Token::EQ) || match(Token::GT) || match(Token::GE) || match(Token::NEQ)){
        BinaryOp op;
        if (previous->type == Token::LT){
            op = LT_OP;
        }
        else if (previous->type == Token::LE){
            op = LE_OP;
        }
        else if (previous->type == Token::EQ){
            op = EQ_OP;
        }
        else if (previous->type == Token::NEQ){
            op = NEQ_OP;
        }
        else if (previous->type == Token::GT){
            op = GT_OP;
        }
        else if (previous->type == Token::GE){
            op = GE_OP;
        }
        Exp* right = parseExpression();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseExpression() {
    Exp* left = parseTerm();
    while (match(Token::PLUS) || match(Token::MINUS)) {
        BinaryOp op;
        if (previous->type == Token::PLUS){
            op = PLUS_OP;
        }
        else if (previous->type == Token::MINUS){
            op = MINUS_OP;
        }
        Exp* right = parseTerm();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseTerm() {
    Exp* left = parseFactor();
    while (match(Token::MUL) || match(Token::DIV)) {
        BinaryOp op;
        if (previous->type == Token::MUL){
            op = MUL_OP;
        }
        else if (previous->type == Token::DIV){
            op = DIV_OP;
        }
        Exp* right = parseFactor();
        left = new BinaryExp(left, right, op);
    }
    return left;
}

Exp* Parser::parseFactor() {
    Exp* e;
    Exp* e1;
    Exp* e2;
    if (match(Token::TRUE)){
        return new BoolExp(1);
    }else if (match(Token::FALSE)){
        return new BoolExp(0);
    }
    else if (match(Token::NUM)) {
        return new NumberExp(stoi(previous->text));
    }
    else if (match(Token::ID)) {
        string texto = previous->text;
        //Parse FCallExp
        if (match(Token::PI)){
            list<Exp*> args;
            if (!check(Token::PD)){
                args.push_back(parseCExp());
                while (match(Token::COMA)){
                    args.push_back(parseCExp());
                }
            }
            if (!match(Token::PD)){
                cout << "Falta paréntesis derecho" << endl;
                exit(0);
            }
            return new FCallExp(texto, args);
        }
        else{
            return new IdentifierExp(previous->text);
        }
            
    }
    else if (match(Token::PI)){
        e = parseCExp();
        if (!match(Token::PD)){
            cout << "Falta paréntesis derecho" << endl;
            exit(0);
        }
        return e;
    }
    cout << "Error: se esperaba un número o identificador." << endl;
    exit(0);
}
