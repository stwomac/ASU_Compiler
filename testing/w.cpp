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
   /* IDS testing
   nextChar();
   
   
   while (ch != END_OF_FILE)
   {
     nextToken();
	 
	 //cout << endl << token;
    //if(token == 'var')
    cout << token <<  " ";
    if(token == "var")
    {continue;}
    if(isNonKeyId(token))   
      cout << endl << ids() << endl << endl;
    //cout << "Next?" << endl;  
     
   }
   */
   
   
   
   //insert testing
    map<string, SymbolTableEntry>::iterator i;
   insert("WORDS",INTEGER,VARIABLE,"",YES,1);
   insert("ababa",INTEGER,VARIABLE,"",YES,1);
   insert("huh",INTEGER,VARIABLE,"",YES,1);
   insert("Wha",INTEGER,VARIABLE,"",YES,1);
   insert("yyyyY",INTEGER,VARIABLE,"",YES,1);
   
   
   
   for(i= symbolTable.begin(); i != symbolTable.end(); i++)
   {
      cout << i->second.getInternalName() << endl;
   }
   
   insert("yyyyY",INTEGER,VARIABLE,"",YES,1);
   
   for(i= symbolTable.begin(); i != symbolTable.end(); i++)
   {
      cout << i->second.getInternalName() << endl;
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

/*DONE AND TESTED*/
string Compiler::ids()          // stage 0, production 8
{
    string temp, tempString;
	
	static int depth = 0;
	//cout  << '|' << depth << '|' << token << "|";
    depth++;
    if(!isNonKeyId(token))
    {
       processError("non-keyword identifier expected");
    }
    
    tempString = token;
    temp = token;
    
   
    if( nextToken() == ",")
    {
       
       if(!isNonKeyId(nextToken()))
       {
          processError("non-keyword identifier expected");
       }
       
       tempString = temp + "," + ids();
    }
    
    return tempString;
}

// Helper functions for the Pascallite lexicon
bool Compiler::isKeyword(string s) const  // determines if s is a keyword
{
    if( s == "program" ||
        s == "begin"   ||
        s == "end"     ||
        s == "var"     ||
        s == "const"   ||
        s == "integer" ||
        s == "boolean" ||
        s == "true"    ||
        s == "false"   ||
        s == "not")
	{return true;}
	
	return false;
}

/*DONE AND TESTED*/
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

/*DONE AND TESTED*/
bool Compiler::isNonKeyId(string s) const // determines if s is a non_key_id
{
   //cout << "|hi|";
	if(!islower(s[0]) ){
      //cout << "F";
		return false;
	}
   else if(!islower(s[s.length() - 1]) && !isdigit(s[s.length() - 1]))
   {
      return false;
   }
	//cout << "{str length: " << s.length() << " }";
	for(uint i = 1; i < s.length() - 2; i++){
		if (s[i] == '_'){
			if(s[i+1] == '_'){
				return false;
			}
		}
	}
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
   /*
    
   //create symbol table entry for each identifier in list of external names
   //Multiply inserted names are illegal
   {
   string name ( name == external name)
   while (name broken from list of external names and put into name != "")
   {
      if (symbolTable[name] is defined)
         processError(multiple name definition)
      else if (name is a keyword)
         processError(illegal use of keyword)
      else //create table entry
      {
      if (name begins with uppercase)
         symbolTable[name]=(name,inType,inMode,inValue,inAlloc,inUnits)
      else
         symbolTable[name]=(genInternalName(inType),inType,inMode,inValue,
      inAlloc,inUnits)
      }
   }   */
   
   
   string name = externalName;
   
   while(name != "")
   {
      string tempName;
      int i = name.find(",");
      
      if(name.length() <= 0)
      {
         name = "";
         continue;
      }
      
      if(i < 0 && name.length() > 0)
      {
         tempName = name;
         name = "";
      }
      
      if( i > -1)
      {
         tempName = name.substr(0,i);
         name = name.substr(i+1, name.length() - 1);
      }
      
      
      if(symbolTable.find(tempName) != symbolTable.end())
      { processError("multiple name definition"); }
      else if(isKeyword(tempName))
      { processError("illegal use of keyword");}
      else if(symbolTable.size() > 256)
      { processError("symbolTable is over maximum size");}
      else
      {
         if(isupper(tempName[0]))
         {
            SymbolTableEntry s(tempName, inType, inMode, inValue, inAlloc, inUnits);
            symbolTable.emplace(tempName,s);
         }
         else
         {
            SymbolTableEntry x(genInternalName(inType), inType, inMode, inValue, inAlloc, inUnits);
            symbolTable.emplace(tempName,x);
         }
         
      }
      
   }
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

/*DONE AND TESTED*/
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

/*DONE AND TESTED*/
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

/*DONE AND TESTED*/
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
    listingFile << "Error: Line " << lineNo << ": " << err << "/n";
    exit(0);
}