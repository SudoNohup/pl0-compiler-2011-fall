#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Lexer.h"
#include "SymbolTable.h"
#include "Parser.h"
#include "MiddleCode.h"
#include "ASMGenerator.h"
#include "ErrorHandler.h"
#include "Kuikong.h"
#include "DAG.h"
#include "BaseBlock.h"
#include "OptASMGenerator.h"
#include "ConflictGraph.h"


#define KEYWORD_RESERVE_NUM 20
#define IDENTIFIER_NUM 21

static string KEYWORD[50]={"", "const", "int", "char", "var", "array", "of", "integer", "procedure", "function", "if", "then", "case", "for", "to", "downto", "do", "begin", "end", "read", "write", "identifier","number","character","string","(", ")", "[", "]", ",", ";", "+", "-", "*", "/", "<", "<=", ">", ">=", "=", "<>", ":=", ":", ".", "\'", "\""};
static Lexer myLexer;
static Parser myParser;
static SymbolTable mySymbolTable;
static MiddleCode myMiddleCode;
static ASMGenerator myASMGenerator;
static ErrorHandler myErrorHandler;
static char sourcefileName[500];
static char objectfileName[500];
static char optobjectfileName[500];
static string sourceFile;
static string objectFile;
static Kuikong myKuikong;
static DAG myDAG;
static BaseBlock myBaseBlock;

static OptASMGenerator myOptASMGenerator;

static ConflictGraph myConflictGraph;
