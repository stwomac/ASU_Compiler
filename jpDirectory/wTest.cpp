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
   time_t now = time (NULL);
   listingFile << "STAGE0:" << "JohnPaul Flores, Steven Womack " << ctime(&now) << endl;
   listingFile << "LINE NO." << setw(30) << right << "SOURCE STATEMENT " << "\n\n";
   //line numbers and source statements should be aligned under the headings 
}

void Compiler::parser()
{
   
   nextChar();
   
   if(nextToken() != "program")
      processError("keyword \"program\" expected");
   
   prog();
}


void Compiler::createListingTrailer()                                               //DONE
{
	if (errorCount == 1)
		listingFile << "\nCOMPILATION TERMINATED" << setw(7) << right << errorCount << " ERRORS ENCOUNTERED";
	else
		listingFile << "\nCOMPILATION TERMINATED" << setw(7) << right << errorCount << " ERRORS ENCOUNTERED";
}

// Methods implementing the grammar productions
void Compiler::prog()           // stage 0, production 1
{
   
    //cout <<" in prog ";
    if (token != "program")
       processError("keyword \"program\" expected");
    
    progStmt();
    
    if (token == "const")
       consts();
    
    if (token == "var")
       vars();
    
    if (token != "begin")
       processError("keyword \"begin\" expected");
    
    beginEndStmt();
   
    if (token != "$")
       processError("no text may follow \"end\"");
    
}

void Compiler::progStmt()       // stage 0, production 2
{
   //cout << "in progStmt " << token << " ";
    string x;
    
    if (token != "program")
       processError("keyword \"program\" expected");
    
    x = nextToken();
    
    //cout << token <<  " ";
    
    if (!isNonKeyId(token))
       processError("program name expected");
    
    if (nextToken() != ";")			//psuedo is nextToken()
       processError("semicolon expected");
    
    nextToken();
    
    code("program", x);
    
    insert(x,PROG_NAME,CONSTANT,x,NO,0); //fix
}

/*DONE BUT UNTESTED*/
void Compiler::consts()         // stage 0, production 3
{
   if (token != "const")
      processError("keyword \"const\" expected");
   
   if (!isNonKeyId(nextToken())) 		//pseudo is !isNonKeyId(nextToken())
      processError("non-keyword identifier must follow \"const\"");
   
   constStmts();
}

void Compiler::vars()           // stage 0, production 4
{
    if (token != "var")
       processError("keyword \"var\" expected");
    
    if (!isNonKeyId(nextToken())) //psuedo is !isNonKeyId(nextToken())
       processError("non-keyword identifier must follow \"var\"");
    
    varStmts();
}

void Compiler::beginEndStmt()   // stage 0, production 5
{
    if (token != "begin")
       processError("keyword \"begin\" expected");
    
    if (nextToken() != "end")
       processError("keyword \"end\" expected");
    
    if (nextToken() != ".")
       processError("period expected");
    
    nextToken();
    //cout << " begin code end" << endl;
    code("end", ".");
}

/*DONE BUT UNTESTED*/
void Compiler::constStmts()     // stage 0, production 6
{
    string x,y;
    
    if(!isNonKeyId(token))
       processError("non-keyword identifier expected");
    
    x = token;
    
    if (nextToken() != "=")
       processError("\"=\" expected");
   
    y = nextToken();
    
    if ( y != "+"       &&
         y != "-"       &&
         y != "not"     &&
         !isNonKeyId(y) &&
         y != "true"    &&
         y != "false"   &&
         !isInteger(y)  ) 
    {processError("token to right of \"=\" illegal");}
    
    if (y == "+" || y == "-") 
    {
       if (!isInteger(nextToken())) 
          processError("integer expected after sign");
       
       y = y + token;
    }
    
    if (y == "not")
    {
       //cout << endl << token;
       token = nextToken();
       
       //cout << " " << token;
       if(symbolTable.find(token) != symbolTable.end())
       {
          token = symbolTable.at(token).getValue();
       }
       //cout << " " << token << endl;
       
       if (!isBoolean(token))
       {processError("boolean expected after \"not\"");}
       
       if (token == "true")
          y = "false";
       else
          y = "true";
    }
    
    if (nextToken() != ";")
       processError("semicolon expected");
    
    //cout << endl << y << " " ;
    
    if (whichType(y) != INTEGER && whichType(y) != BOOLEAN)
    {
       //cout  << y << endl;
       processError("data type of token on the right-hand side must be INTEGER or BOOLEAN, womack");
    }
    //cout << x << " " << whichType(y) << endl;;
    insert(x,whichType(y),CONSTANT,whichValue(y),YES,1); 
    
    x = nextToken();
    
    if ( x != "begin" && x != "var" && !isNonKeyId(x))
       processError("non-keyword identifier, \"begin\", or \"var\" expected");
    
    if (isNonKeyId(x))
    {
       //cout << x << " ";
       constStmts();
    }
}

void Compiler::varStmts()       // stage 0, production 7
{
    string x,y;
    
    if (!isNonKeyId(token)) //fix
       processError("non-keyword identifier expected");
   
    x = ids();
   
    if (token != ":")
       processError("\":\" expected");
   
    token = nextToken();
     
       
    
    y = token;
    storeTypes dataY;
    if(y == "integer")
    {
       dataY = INTEGER;
    }
    else if(y == "boolean")
    {
       dataY = BOOLEAN;
    }
    else
    {
       processError("illegal type follows \":\"");
    }
    
    if (nextToken() != ";")
       processError("semicolon expected");
    
    insert(x,dataY,VARIABLE,"",YES,1);
    
    token = nextToken();																	
    //can not really do nextToken() multiple times in one if, basically look at the same nextToken
    if (!isNonKeyId(token) && token != "begin")
    {
       //please look into if we need ||'s or &&'s
       processError("non-keyword identifier or \"begin\" expected");
    }
    
    if (isNonKeyId(token))
       varStmts();
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

/*DONE*/
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
   if(s.length() > 15)
   {s = s.substr(0,15);}
   if(isKeyword(s))
      return false;
   //cout << "|hi|";
   if(s[s.length()-1] == '_')
      return false;
   
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
    for(uint i = 0; i < s.length(); i++){
		//this code is for catching + or - unsure if necessary
      //if(i == 0 && (s[i] == '+' || s[i] == '-'))
         //continue;
      
      if (!isdigit(s[i]))
			return false;
		
	}
    return true;
}

bool Compiler::isBoolean(string s) const  // determines if s is a boolean
{
    if (s == "true" || s == "false")
		return true;
	else
		return false;
}

/*DONE AND TESTED*/
bool Compiler::isLiteral(string s) const  // determines if s is a literal
{
    // if s[0] == + or -, check +[possibilities] to be integer
	if(s[0] == '+' || s[0] == '-')
    {			
        
		if(isInteger(s.substr(1, s.length() - 1)))
        {return true;}
    
    }
	
	// if straight bool or int then your fine
	if(isBoolean(s) || isInteger(s))
    {
        //cout << " in here ";
		return true;
	}
	// if it starts with not, the rest should be a bool
	if(s.substr(0, 3) == "not"){
		if(isBoolean(s.substr(4, s.length() - 4)))
        {return true;}
        
    }
    
    return false;
}

/*DONE AND TESTED*/
// Action routines
void Compiler::insert(string externalName, storeTypes inType, modes inMode, string inValue, allocation inAlloc, int inUnits)
{
   
   string name = externalName;
   
   if(name.length() > 15)
   {name = name.substr(0,15);}
   
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

/*DONE AND TESTED*/
storeTypes Compiler::whichType(string name) // tells which data type a name has
{
    storeTypes dataType;
    
    if(isLiteral(name))
    {
        if(name == "true" || name == "false")
        {
            dataType = BOOLEAN;
        }
        else
        {
            dataType = INTEGER;
        }
    }
    else
    {
        if(symbolTable.find(name) != symbolTable.end())
        {
            dataType = symbolTable.at(name).getDataType();
        }
		else
		{
         //cout << endl << name  << endl;
			processError("reference to undefined constant");
		}
    }
	
	return dataType;
    
    //return INTEGER;
}

/*DONE and NOT TESTED*/
string Compiler::whichValue(string name) // tells which value a name has
{
    string value = "value never set";
	
	 if(isLiteral(name))
	 {value = name;}
	 else
    {
       if(symbolTable.find(name) != symbolTable.end() )
       {
          if(symbolTable.at(name).getValue() != "")
          {
             value = symbolTable.at(name).getValue();
          }
          else
          {
             //cout << endl << name  << endl;
             processError("reference to undefined constant");
          }
       }
       else
       {
          //cout << endl << name  << endl;
          processError("reference to undefined constant");
       }
    }
    
    return value;
}

void Compiler::code(string op, string operand1 , string operand2 )
{
   
    if (op == "program")
    {
       emitPrologue(operand1);
    }
    else if (op == "end")
    {
       //cout << " made it to emit \n";
       emitEpilogue();
       
    }
    else
       processError("compiler error since function code should not be called with illegal arguments");
}

// Emit Functions dont know why there is an if in here for w.cpp
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

//changed to store one way but output the correctway
void Compiler::emitStorage()
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
         string val;
         if(x.second.getValue() == "true")
         {
            emit(x.second.getInternalName(), "dd", "-1", comment);
         }
         else if(x.second.getValue() == "false")
         {
            emit(x.second.getInternalName(), "dd", "0", comment);
         }
         else
         {
            emit(x.second.getInternalName(), "dd", x.second.getValue(), comment); //the line?
         }
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
         emit(x.second.getInternalName(), "resd", to_string(x.second.getUnits()), comment); //the line?
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

/*DONE AND TESTED*/
void Compiler::processError(string err)
{
   //cout << "what?" << endl;
    //Error: Line 7: ":" expected
    listingFile << "Error: Line " << lineNo << ": " << err << "\n";
    
    sourceFile.close();
    listingFile.close();
    objectFile.close();
    
    
    //cout << " near the end";
    exit(EXIT_FAILURE);
}