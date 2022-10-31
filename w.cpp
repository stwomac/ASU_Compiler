#include <stage0.h>

using namespace std;


Compiler::Compiler(char **argv) // constructor
{
    
}

Compiler::~Compiler()           // destructor
{
    
}

void Compiler::createListingHeader()
{
    
}

void Compiler::parser()
{
    
}

void Compiler::createListingTrailer()
{
    
}

// Methods implementing the grammar productions
void Compiler::prog()           // stage 0, production 1
{
    
}

void Compiler::progStmt()       // stage 0, production 2
{
    
}

void Compiler::consts()         // stage 0, production 3
{
    
}

void Compiler::vars()           // stage 0, production 4
{
    
}

void Compiler::beginEndStmt()   // stage 0, production 5
{
    
}

void Compiler::constStmts()     // stage 0, production 6
{
    
}

void Compiler::varStmts()       // stage 0, production 7
{
    
}

string Compiler::ids()          // stage 0, production 8
{
    return "";
}

// Helper functions for the Pascallite lexicon
bool Compiler::isKeyword(string s) const  // determines if s is a keyword
{
    return true;
}

bool Compiler::isSpecialSymbol(char c) const // determines if c is a special symbol
{
    return true;
}

bool Compiler::isNonKeyId(string s) const // determines if s is a non_key_id
{
    return true;
}

bool Compiler::isInteger(string s) const  // determines if s is an integer
{
    return true;
}

bool Compiler::isBoolean(string s) const  // determines if s is a boolean
{
    return true;
}

bool Compiler::isLiteral(string s) const  // determines if s is a literal
{
    return true;
}

// Action routines
void Compiler::insert(string externalName, storeTypes inType, modes inMode, string inValue, allocation inAlloc, int inUnits)
{
        
}

storeTypes Compiler::whichType(string name) // tells which data type a name has
{
    return INTEGER;
}
string Compiler::whichValue(string name) // tells which value a name has
{
    return "";
}

void Compiler::code(string op, string operand1 = "", string operand2 = "")
{
    
}

// Emit Functions
void Compiler::emit(string label = "", string instruction = "", string operands = "", string comment = "")
{
    
}

void Compiler::emitPrologue(string progName, string = "")
{
    
}

void Compiler::emitEpilogue(string = "", string = "")
{
    
}

void Compiler::emitStorage()
{
    
}


// Lexical routines
char Compiler::nextChar() // returns the next character or END_OF_FILE marker
{
    return 'a';
}

string Compiler::nextToken() // returns the next token or END_OF_FILE marker
{
    return "";
}

// Other routines
string Compiler::genInternalName(storeTypes stype) const
{
    return "";
}
void Compiler::processError(string err)
{
    
}