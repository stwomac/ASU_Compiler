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
   emitPrologue("program.asm", "string?");
   
   
   sourceFile.close();
   listingFile.close();
   objectFile.close();
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
    return true;
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
void Compiler::emit(string label , string instruction , string operands , string comment ) //DONE?
{
   objectFile << left;
   objectFile << setw(8) << label;
   objectFile << setw(8) << instruction;
   objectFile << setw(24) << operands;
   objectFile << comment << endl;
   /*
   Turn on left justification in objectFile
   Output label in a field of width 8
   Output instruction in a field of width 8
   Output the operands in a field of width 24
   Output the comment
   */
}

void Compiler::emitPrologue(string progName, string s) // uhhh done? I would really look into this one
{
   /*
   Output identifying comments at beginning of objectFile
   Output the %INCLUDE directives
   */
   //time and nams
   time_t now = time (NULL);
   objectFile << "; " << setw(35) << left << "JohnPaul Flores & Steven Womack " << right << ctime(&now);
   
   //includes
   objectFile << "%INCLUDE " << "\"Along32.inc\"" << endl;
   objectFile << "%INCLUDE " << "\"Macros_Along.inc\"" << endl;
   
   emit("SECTION", ".text");
   //may run into program name length errors?
   emit("global", "_start", "", "; program " + progName);
   objectFile << "\n";
   emit("_start:");
}

void Compiler::emitEpilogue(string a, string b) //why arent the parameters used?
{
   emit("","Exit", "{0}");
   objectFile << "\n";
   emitStorage();
}

void Compiler::emitStorage()  //first stable version
{
   //showing structure of symbolTable. basically a dictionary of lists
   //map<string, SymbolTableEntry> symbolTable;
   //StmbolTableEntry[InternalName, dataType, Mode, Value, Allocation, Unit]
   //[externalName, [InternalName, dataType, Mode, Value, Allocation, Unit]
   
   //emmiting the .data section
   emit("SECTION", ".data");
   
   //for those entries in the symbolTable that have an allocation of YES and a storage mode of CONSTANT
   for(auto const& x : symbolTable) 
   { 
      string comment = "; ";
      comment += x.first;
      //see sample output - basically printing the int name, then dataType, etc
      if(x.second.getAlloc() == YES && x.second.getMode() == CONSTANT){
         //may need to look at booleans
         emit(x.second.getInternalName(), to_string(x.second.getDataType()), x.second.getValue(), comment); //the line?
	  }
   }
   
   //emitting the .bss section on a newline
   objectFile << "\n";
   emit("SECTION", ".bss");
   
   //for those entries in the symbolTable that have an allocation of YES and a storage mode of VARIABLE
   for(auto const& x : symbolTable) 
   { 
      string comment = "; ";
      comment += x.first;
      //see sample output - basically printing the int name, then dataType, etc
      if(x.second.getAlloc() == YES && x.second.getMode() == VARIABLE){
         emit(x.second.getInternalName(), to_string(x.second.getDataType()), to_string(x.second.getUnits()), comment); //the line?
      }
   }
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
    exit(EXIT_FAILURE);
}