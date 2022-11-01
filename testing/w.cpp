#include <stage0.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iomanip>

using namespace std;

/*NOTES FOR JP nextToken, nextChar, and isSpecialSymbol are done*/

Compiler::Compiler(char **argv) // constructor
{
   sourceFile.open(argv[1]);
   listingFile.open(argv[2]);
   objectFile.open(argv[3]);
}

Compiler::~Compiler()           // destructor
{
   sourceFile.close();
   listingFile.close();
   objectFile.close();
}

void Compiler::createListingHeader()
{
    
}

void Compiler::parser()
{
   int i = 10;
   
   string s = "";
   
   while (i > 0)
   {
      i--;
      
     cout << "eh" << " ";
	  if(i > 5)
      cout <<endl << genInternalName(INTEGER) << endl;
     else
      cout <<endl << genInternalName(BOOLEAN) << endl;
   }
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
    if( c == '=' ||
        c == ':' ||
        c == ',' ||
        c == ';' ||
        c == '.' ||
        c == '+' ||
        c == '-')
    {return true;}
    
    return false;
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

void Compiler::code(string op, string operand1 , string operand2 )
{
    
}

// Emit Functions
void Compiler::emit(string label , string instruction , string operands , string comment )
{
    
}

void Compiler::emitPrologue(string progName, string s)
{
    
}

void Compiler::emitEpilogue(string a, string b)
{
    
}

void Compiler::emitStorage()
{
    
}


// Lexical routines
char Compiler::nextChar() // returns the next character or END_OF_FILE marker
{
    sourceFile.get(ch);
    
    static char preCh = '\n';
    
    if(sourceFile.eof())
    {
       ch = END_OF_FILE;
       return ch;
    }
    
    if(preCh == '\n')
    {
       lineNo++;
       listingFile << setw(5) << lineNo << '|';
    }
    
    listingFile << ch;
    
    preCh = ch;
    //cout << ch << '|';
    return ch;
}

string Compiler::nextToken() // returns the next token or END_OF_FILE marker
{
    token = "";
    
    while(token == "")
    {
       if(ch == '{')
       {
          while(ch != END_OF_FILE && ch != '}')
          {
             nextChar();
          }
          
          if(ch == END_OF_FILE)
          {
             processError("unexpected end of file");
          }
          else
          {
             nextChar();
          }
       }
       else if (ch == '}')
       {
          processError("'}' cannot begin token");
       }
       else if(isspace(ch))
       {
          nextChar();
       }
       else if(isSpecialSymbol(ch))
       {
          token = ch;
          nextChar();
       }
       else if(islower(ch)) 
       {
          
          token = ch;
          nextChar();
          
          while( (islower(ch) || isdigit(ch) || ch == '_') && ch!= END_OF_FILE)
          {
             token += ch;
             nextChar();
          }
          
          if(ch == END_OF_FILE)
          {processError("unexpected end of file");}
       }
       else if(isdigit(ch))
       {
          token = ch;
          nextChar();
          
          while(isdigit(ch) && ch!= END_OF_FILE)
          {
             token += ch;
             nextChar();
          }
          
          if(ch == END_OF_FILE)
          {processError("unexpected end of file");}
       }
       else if(ch == END_OF_FILE)
       {token = ch;}
       else
       {processError("illegal symbol");}
    }
    return token;
}

// Other routines
string Compiler::genInternalName(storeTypes stype) const
{
    static int intCount = 0;
    
    static int boolCount = 0;
    
    string ret;
    
    if(stype == INTEGER)
    {
        
        ret = "I" + to_string(intCount);
        intCount++;
    }
    else if( stype == BOOLEAN )
    {
        ret = "B" + to_string(boolCount);
        boolCount++;
    }
    
    
    return ret;
}
void Compiler::processError(string err)
{
    //Error: Line 7: ":" expected
   //listingFile << "Error: Line " << lineNo << ": " << err << "/n";
  // exit(EXIT_FAILURE);
}