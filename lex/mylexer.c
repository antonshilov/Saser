%{
#if defined _WIN32
#include <io.h>              // ??? isatty
#elif defined _WIN64
#include <io.h>              // ??? isatty
#endif

#ifdef MSVC
#define  isatty _isatty	      // ? VC isatty ?????? _isatty
#endif

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <map>
#include <vector>
#include <tuple>
#include <string>

using namespace std;
enum class TokenClass {
    ASSIGN,
    INTCONST,
    SEMICOLON,
    COMMA,
    DOT,
    OPEN_PAREN,
    CLOSE_PAREN,
    OPEN_BRACE,
    CLOSE_BRACE,
    IDENTIFIER,
    CHAR,
    STRING_LITERAL,
    TRUE,
    FALSE,
    FLOAT,
    INT,
    RETURN,
    FOR,
    IF,
    ELSE,
    DO,
    GOTO,
    BREAK,
    CONTINUE,
    WHILE,
    EXCLAMATION,
    PERCENT,
    MULT,
    MINUS,
    PLUS,
    SQ_BR_OP,
    SQ_BR_CL,
    INV_COMMA,
    SLASH,
    QUESTION,
    LE,
    GE,
    COLON,
    LE_OP,
    GE_OP,
    EQ_OP,
    VOID,
    NE_OP
};

static std::map <TokenClass, string> TokenClassNames =
        {
                {TokenClass::ASSIGN,         "ASSIGN"},
                {TokenClass::INTCONST,       "INTCONST"},
                {TokenClass::SEMICOLON,      "SEMICOLON"},
                {TokenClass::COMMA,          "COMMA"},
                {TokenClass::DOT,            "DOT"},
                {TokenClass::OPEN_PAREN,     "OPEN_PAREN"},
                {TokenClass::CLOSE_PAREN,    "CLOSE_PAREN"},
                {TokenClass::OPEN_BRACE,     "OPEN_BRACE"},
                {TokenClass::CLOSE_BRACE,    "CLOSE_BRACE"},
                {TokenClass::IDENTIFIER,     "IDENTIFIER"},
                {TokenClass::CHAR,           "CHAR"},
                {TokenClass::STRING_LITERAL, "STRING_LITERAL"},
                {TokenClass::TRUE,           "TRUE"},
                {TokenClass::FALSE,          "FALSE"},
                {TokenClass::FLOAT,          "FLOAT"},
                {TokenClass::INT,            "INT"},
                {TokenClass::RETURN,         "RETURN"},
                {TokenClass::FOR,            "FOR"},
                {TokenClass::IF,             "IF"},
                {TokenClass::ELSE,           "ELSE"},
                {TokenClass::DO,             "DO"},
                {TokenClass::GOTO,           "GOTO"},
                {TokenClass::BREAK,          "BREAK"},
                {TokenClass::CONTINUE,       "CONTINUE"},
                {TokenClass::WHILE,          "WHILE"},
                {TokenClass::EXCLAMATION,    "EXCLAMATION"},
                {TokenClass::PERCENT,        "PERCENT"},
                {TokenClass::MULT,           "MULT"},
                {TokenClass::MINUS,          "MINUS"},
                {TokenClass::PLUS,           "PLUS"},
                {TokenClass::SQ_BR_OP,       "SQ_BR_OP"},
                {TokenClass::SQ_BR_CL,       "SQ_BR_CL"},
                {TokenClass::INV_COMMA,      "INV_COMMA"},
                {TokenClass::SLASH,          "SLASH"},
                {TokenClass::QUESTION,       "QUESTION"},
                {TokenClass::LE,             "LE"},
                {TokenClass::GE,             "GE"},
                {TokenClass::COLON,          "COLON"},
                {TokenClass::LE_OP,          "LE_OP"},
                {TokenClass::GE_OP,          "GE_OP"},
                {TokenClass::EQ_OP,          "EQ_OP"},
                {TokenClass::VOID,           "VOID"},
                {TokenClass::NE_OP,          "NE_OP"}
        };

typedef std::tuple<TokenClass, string, int> TokenTableRow;

std::vector <TokenTableRow> TokenTable;

void count();

int g_offset = 0;
bool failrue_flag = false;

%}

%option nodefault noyywrap nounistd
%option yylineno
%option c++

stringch      [^"\n]
D			[0-9]
L			[a-zA-Z_]
%%

"/!"[^\n]* {}

"="			{ count();  return TokenClass::ASSIGN; }
"break"			{ count();  return TokenClass::BREAK; }
"continue"		{ count();  return TokenClass::CONTINUE; }
"do"			{ count();  return TokenClass::DO; }
"else"			{ count();  return TokenClass::ELSE; }
"float"			{ count();  return TokenClass::FLOAT; }
"for"			{ count();  return TokenClass::FOR; }
"goto"			{ count();  return TokenClass::GOTO;}
"if"			{ count();  return TokenClass::IF;}
"int"			{ count();  return TokenClass::INT;}
"return"		{ count();  return TokenClass::RETURN; }
"void"			{ count();  return TokenClass::VOID;}
"while"			{ count();  return TokenClass::WHILE; }
"!="			{ count();  return TokenClass::NE_OP;}
"=="			{ count();  return TokenClass::EQ_OP;}
">="			{ count();  return TokenClass::GE_OP;}
"<="			{ count();  return TokenClass::LE_OP;}
":"			{ count();  return TokenClass::COLON;}
">"			{ count();  return TokenClass::GE;}
"<"			{ count();  return TokenClass::LE;}
"?"			{ count();  return TokenClass::QUESTION;}
"/"			{ count();  return TokenClass::SLASH;}
"'"			{ count();  return TokenClass::INV_COMMA;}
"["			{ count();  return TokenClass::SQ_BR_OP;}
"]"			{ count();  return TokenClass::SQ_BR_CL;}
"+"			{ count();  return TokenClass::PLUS;}
"-"			{ count();  return TokenClass::MINUS;}
"*"			{ count();  return TokenClass::MULT;}
"%"			{ count();  return TokenClass::PERCENT;}
"!"			{ count();  return TokenClass::EXCLAMATION;}
"true"			{ count();  return TokenClass::TRUE;}
"false"			{ count();  return TokenClass::FALSE;}
"}"			{ count();  return TokenClass::CLOSE_BRACE;}
"{"			{ count();  return TokenClass::OPEN_BRACE;}
")"			{ count();  return TokenClass::CLOSE_PAREN;}
"("			{ count();  return TokenClass::OPEN_PAREN;}
"."			{ count();  return TokenClass::DOT;}
","			{ count();  return TokenClass::COMMA;}
";"			{ count();  return TokenClass::SEMICOLON;}
"char"			{ count();  return TokenClass::CHAR;}

\n++ {g_offset = 0;}

0|[1-9][0-9]* 			{ count();  return TokenClass::INTCONST;}
\"(\\.|[^\\"])*\"	{ count();  return TokenClass::STRING_LITERAL;}
({L}|[_])({L}|{D})*	{ count();  return TokenClass::IDENTIFIER;}
[ \t\v\f]		{ count();}

\"{stringch}*$  {
cerr << "ERROR: " << YYText() << " - Unfinished string literal. Line " << lineno() <<"___" << g_offset<< ".\n";
failrue_flag = true;
}

.       {
cerr << "ERROR: " << YYText() << " - unrecognized token. Line " << lineno() << "--" << g_offset<< ".\n";
failrue_flag = true;
}
%%


void count() {
    g_offset++;
}

int yywrap(void) {
    return 1;
}

static void DumpTokenTable(string firstColName, string secondColName, string thirdColName) {
    cout << left;
    cout.width(20);
    cout << firstColName;
    cout.width(20);
    cout << secondColName;
    cout.width(20);
    cout << thirdColName << "\n";
    for (TokenTableRow ttr : TokenTable) {
        cout.width(20);
        cout << TokenClassNames[std::get<0>(ttr)];
        cout.width(20);
        cout << get<1>(ttr) << get<2>(ttr) << "\n";
    }
}

int main(int argc, const char *argv[]) {

    if (argc < 2) {
        cerr << "Too few parameters in command line.\n";
        return EXIT_FAILURE;
    } else if (argc > 2) {
        cerr << "Too much parameters in command line.\n";
        return EXIT_FAILURE;
    }

    ifstream infile;
    infile.open(argv[1]);
    if (infile.fail()) {
        cerr << "Cannot open file " << argv[1] << "\n";
        return EXIT_FAILURE;
    }

    yyFlexLexer lexer(&infile);
    lexer.yylex();

    infile.close();

    cout << "Dump token table.\n\n";
    if (0 < TokenTable.size()) {
        DumpTokenTable("Token class", "Token value", "Line number");
    } else {
        cout << "No tokens." << "\n";
    }
    cout << "\nDump finished.\n\n";
    if (failrue_flag) {
        return EXIT_FAILURE;
    } else {
        return EXIT_SUCCESS;
    }
}