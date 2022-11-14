//CS 4301: 010 && 020
//Stage 0 Compiler
//JohnPaul Flores && Steven Womack
//11/4/2022

/*Womack Edition*/



//Reminder github/winscp converts lf to crlf so we need to call setup for 
//actual data files with proper lf
#include <stage1.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <iomanip>

using namespace std;


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
    listingFile << "STAGE0:  " << "JohnPaul Flores, Steven Womack  " << ctime(&now) << "\n";
    listingFile << "LINE NO." << setw(30) << right << "SOURCE STATEMENT" << left << "\n\n" << right;
    //line numbers and source statements should be aligned under the headings 
}

void Compiler::parser()
{
   

	//cout << "bwhaha" << endl;
   //varibale delimit testing
   //cout << isNonKeyId("a234567890abc4_aa") << " " << isNonKeyId("a234567890abc4aaaaaa") << endl;
   
   //objectFile Testing
   //objectFile << "words";
   //objectFile.close();
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
   /*
    map<string, SymbolTableEntry>::iterator i;
   insert("WORDS,what,why,WHO",INTEGER,VARIABLE,"",YES,1);
   insert("ababa",INTEGER,VARIABLE,"",YES,1);
   insert("huh",BOOLEAN,VARIABLE,"",YES,1);
   insert("Wha",INTEGER,VARIABLE,"",YES,1);
   insert("yyyyY",INTEGER,VARIABLE,"",YES,1);
   
   storeTypes thing = symbolTable.at("huh").getDataType();
   
   cout << thing << " ";
   */
   /*
   for(i= symbolTable.begin(); i != symbolTable.end(); i++)
   {
      cout << i->second.getInternalName() << endl;
   }
   
   insert("yyyyY",INTEGER,VARIABLE,"",YES,1);
   
   for(i= symbolTable.begin(); i != symbolTable.end(); i++)
   {
      cout << i->second.getInternalName() << endl;
   }
   */
   //error testing;
   /*
   processError("WHAT");
   */
   
   //isLiteral testing
   /*
   string s = "12547+32893";
   
   
   
   if(isLiteral("12547+32893"))
       cout << "t" << endl;
   else
       cout << "f" << endl;
   
   if(isLiteral("no"))
       cout << "t" << endl;
   else
       cout << "f" << endl;
   
   if(isLiteral("not false"))
       cout << "t" << endl;
   else
       cout << "f" << endl;
   
   if(isLiteral("_4383832"))
       cout << "t" << endl;
   else
       cout << "f" << endl;
   
   if(isLiteral("383 82"))
       cout << "t" << endl;
   else
       cout << "f" << endl;
   */
   
   nextChar();
   
   if(nextToken() != "program")
      processError("keyword \"program\" expected");
   
   prog();
   
}

void Compiler::createListingTrailer()
{
    if (errorCount == 1)
		listingFile << "\nCOMPILATION TERMINATED" << setw(7) << right << errorCount << " ERROR ENCOUNTERED\n";
	 else
		listingFile << "\nCOMPILATION TERMINATED" << setw(7) << right << errorCount << " ERRORS ENCOUNTERED\n";
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
       processError("data type of token on the right-hand side must be INTEGER or BOOLEAN");
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



bool Compiler::isNonKeyId(string s) const // determines if s is a non_key_id
{
    if(s.length() > 15)
    {s = s.substr(0,15);}
 
    if(isKeyword(s))
       return false;
    
    //cout << "|hi|";
    
    if(s[s.length()-1] == '_')
       return false;
    
	 if(!islower(s[0]) )
    {
       //cout << "F";
	 	 return false;
	 }
    else if(!islower(s[s.length() - 1]) && !isdigit(s[s.length() - 1]))
    {
       return false;
    }
    
    if(s.length() < 2 && islower(s[0]))
    {return true;}
	 //cout << "{str length: " << s.length() << " }";
	 for(uint i = 1; i < s.length() - 2; i++)
    {
	 	 if (s[i] == '_')
       {
	 	 	 if(s[i+1] == '_')
          {
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
	 if(s.substr(0, 3) == "not")
    {
		 if(isBoolean(s.substr(4, s.length() - 4)))
          {return true;}
        
    }
    
    return false;
}

// Action routines
void Compiler::insert(string externalName, storeTypes inType, modes inMode, string inValue, allocation inAlloc, int inUnits)
{
   
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
          
          if(tempName.length() > 15)
          {tempName = tempName.substr(0,15);}
       }
       
       if( i > -1)
       {
          tempName = name.substr(0,i);
          name = name.substr(i+1, name.length() - 1);
          
          if(tempName.length() > 15)
          {tempName = tempName.substr(0,15);}
       }
       
       
       if(symbolTable.find(tempName) != symbolTable.end())
       { 
          string err = "symbol " + tempName + " is multiply defined";
          processError(err); 
          
       }
       else if(isKeyword(tempName))
       { processError("illegal use of keyword");}
       else if(symbolTable.size() > 255)
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
           processError("reference to undefined symbol");
        }
    }
	
	 return dataType;
    
    //return INTEGER;
}

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



// Emit Functions
void Compiler::emit(string label , string instruction , string operands , string comment )
{
   
    objectFile << left;
    objectFile << setw(8) << label;
    objectFile << setw(8) << instruction;
    objectFile << setw(24) << operands;
    objectFile << comment << "\n";
}

void Compiler::emitPrologue(string progName, string s)
{
    time_t now = time (NULL);
    objectFile << "; " << setw(35) << left << "JohnPaul Flores & Steven Womack " << right << ctime(&now);
    
    //includes
    objectFile << "%INCLUDE " << "\"Along32.inc\"" << "\n";
    objectFile << "%INCLUDE " << "\"Macros_Along.inc\"\n" << "\n";
    
    emit("SECTION", ".text");
    //may run into program name length errors?
    
    if(progName.length() > 15)
    {
        progName = progName.substr(0,15);
    }
    emit("global", "_start", "", "; program " + progName);
    objectFile << "\n";
    emit("_start:");
}

void Compiler::emitEpilogue(string a, string b)
{
   emit("","Exit", "{0}");
   objectFile << "\n";
   emitStorage();
}

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
    else if(stype == PROG_NAME)
    {
        ret = "P0";
    }
    
    
    return ret;
}

void Compiler::processError(string err)
{
    errorCount++;
    listingFile << "\nError: Line " << lineNo << ": " << err << "\n";
	 createListingTrailer();
     
     
     
   /*
   cout << endl << "operator stack: ";
   while( !operatorStk.empty())
   {
      cout << operatorStk.top() << " ";
      operatorStk.pop();
   }
   
   cout << endl << "operand stack: ";
    
    while( !operandStk.empty())
   {
      cout << operandStk.top() << " ";
      operandStk.pop();
   }
   
   
   cout << endl;
   
   */
    sourceFile.close();
    listingFile.close();
    objectFile.close();
    
    
    //cout << " near the end";
    exit(EXIT_FAILURE);
}


/*Stage 1 Begins Below*/

/*Changed Stage 0 Functions*/


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
          if(token == ":" || token == "<" || token == ">")
          {
              if(ch == '=')
              {
                  token += ch;
                  nextChar();
              }
              else if( token == "<" && ch == '>')
              {
                  token += ch;
                  nextChar();
              }
          }
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
        s == "not"     ||
        s == "mod"     ||
        s == "div"     ||
        s == "and"     ||
        s == "or"      ||
        s == "read"    ||
        s == "write")
	{return true;}
	
	return false;
}

bool Compiler::isSpecialSymbol(char c) const // determines if c is a special symbol
{
    if( c == '=' ||
        c == ':' ||
        c == ',' ||
        c == ';' ||
        c == '.' ||
        c == '+' ||
        c == '-' ||
        c == '*' ||
        c == '(' ||
        c == ')' ||
        c == '<' ||
        c == '>')
    {return true;}
    
    return false;
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
    else if( op == "read")
    {
       emitReadCode(operand1);
    }
    else if(op == "write")
    {
       emitWriteCode(operand1);
    }
    else if(op == "+") //bindary +
    {
		
       emitAdditionCode(operand1, operand2);
    }
    else if(op == "-") //binary -
    {
       emitSubtractionCode(operand1, operand2);
    }
    else if( op == "neg") //unary -
    {
       emitNegationCode(operand1);
    }
    else if(op == "not")
    {
       emitNotCode(operand1);
    }
    else if(op == "*")
    {
       emitMultiplicationCode(operand1, operand2);
    }
    else if( op == "div") //unary -
    {
       emitDivisionCode(operand1, operand2);
    }
    else if(op == "mod")
    {
       emitModuloCode(operand1, operand2);
    }
    else if(op == "and")
    {
       emitAndCode(operand1, operand2);
    }
    else if( op == "or") //unary -
    {
       emitOrCode(operand1, operand2);
    }
    else if(op == "=")
    {
       emitEqualityCode(operand1, operand2);
    }
    else if(op == ":=")
    {
       emitAssignCode(operand1, operand2);
    }
    else if(op == "<>")
    {
       emitInequalityCode(operand1, operand2);
    }
    else if(op == "<")
    {
       emitLessThanCode(operand1, operand2);
    }
    else if( op == "<=") //unary -
    {
       emitLessThanOrEqualToCode(operand1, operand2);
    }
    else if(op == ">")
    {
       emitGreaterThanCode(operand1, operand2);
    }
    else if(op == ">=")
    {
       emitGreaterThanOrEqualToCode(operand1, operand2);
    }
    else
       processError("compiler error since function code should not be called with illegal arguments");
}

void Compiler::beginEndStmt()   // stage 0, production 5
{
     if (token != "begin")
        processError("keyword \"begin\" expected");
     
     execStmts();
     
     //changing nextToken to token (gonna call next token at end of execStmts
     if (token != "end")
        processError("keyword \"end\" expected");
     
     if (nextToken() != ".")
        processError("period expected");
     
     nextToken();
     //cout << " begin code end" << endl;
     code("end", ".");
}

void Compiler::execStmts()      // stage 1, production 2
{
   nextToken();
   
   
   if(isNonKeyId(token)    || 
      token == "read"      ||
      token == "write")
   {
	   
      execStmt();
 
      execStmts();
   }
   
   
   
}

void Compiler::execStmt()       // stage 1, production 3
{
   if(isNonKeyId(token))
   {
	  
      assignStmt();
   }
   else if(token == "read")
   {
      readStmt();
   }
   else if(token == "write")
   {
      writeStmt();
   }
   else
   {
      processError("invalid token in execStmt");
   }
   
}

/* When Next Token is Called:
Express - on start
Part - on end only
factors - none* 
factor - none
terms - none*
term - none
expresses - none*
express - at the start
*/
void Compiler::assignStmt()     // stage 1, production 4
{
   pushOperand(token);
   
   if(nextToken() != ":=")
   {processError(":= expected");}
   
   pushOperator(":=");
   
   express();
   
   if(token != ";")
   {processError("; expected");}
   
   
   string op = popOperator(), op1 = popOperand(), op2 = popOperand();
   code(op,op1,op2);
   
}

void Compiler::readStmt()       // stage 1, production 5
{
   nextToken();
   
   if(token  == "(")
   {
      if (!isNonKeyId(nextToken())) //psuedo is !isNonKeyId(nextToken())
       processError("non-keyword identifier must follow \"(\"");
    
      string x = ids();
      
      if (token != ")")
       processError("\")\" expected");
      code("read", x);
      
      if(nextToken() != ";")
         processError("\";\" expected");
      
   }
   else
   {
      processError(" ( expected after read");
   }
   
}

void Compiler::writeStmt()      // stage 1, production 7
{
   nextToken();
   
   if(token  == "(")
   {
      if (!isNonKeyId(nextToken())) //psuedo is !isNonKeyId(nextToken())
       processError("non-keyword identifier must follow \"(\"");
    
      string x = ids();
      
      if (token != ")")
       processError("\")\" expected");
      code("write", x);
      
      if(nextToken() != ";")
         processError("\";\" expected");
      
   }
   else
   {
      processError(" ( expected after write");
   }
   
}

void Compiler::express()       // stage 1, production 9
{
   nextToken();
   if(token != "not"    && 
      token != "true"   && 
      token != "false"  &&
      token != "("      &&
      token != "+"      &&
      token != "-"      &&
      !isInteger(token) &&
      !isNonKeyId(token))
   {processError("'not','true','false','(','+','-', INTEGER,NON_KEY_ID is expected");}
   
   term();
   expresses();
   
}

void Compiler::expresses()      // stage 1, production 10
{
   if(token == "<>"  || 
      token == "="   ||
      token == "<="  ||
      token == ">="  ||
      token == "<"   ||
      token == ">")
   {
      pushOperator(token);
      nextToken();
      term();
      string op = popOperator(), op1 = popOperand(), op2 = popOperand();
      code(op,op1,op2);
      expresses();
   }
   
}

void Compiler::term()           // stage 1, production 11
{
   if(token != "not"    && 
      token != "true"   && 
      token != "false"  &&
      token != "("      &&
      token != "+"      &&
      token != "-"      &&
      !isInteger(token) &&
      !isNonKeyId(token))
   {processError("'not','true','false','(','+','-', INTEGER,NON_KEY_ID is expected");}
   factor();
   terms();
   
}

void Compiler::terms()          // stage 1, production 12
{
   if(token == "-"   ||
      token == "+"   ||
      token == "or")
   {
      pushOperator(token);
      nextToken();
      factor();
	  
	   string op = popOperator(), op1 = popOperand(), op2 = popOperand();
      code(op,op1,op2);
      terms();
   }
   
   
}

void Compiler::factor()         // stage 1, production 13
{
   if(token != "not"    && 
      token != "true"   && 
      token != "false"  &&
      token != "("      &&
      token != "+"      &&
      token != "-"      &&
      !isInteger(token) &&
      !isNonKeyId(token))
   {processError("'not','true','false','(','+','-', INTEGER,NON_KEY_ID is expected");}
   part();
   factors();
   
}

/*draft*/
void Compiler::factors()        // stage 1, production 14
{
   if(token == "*"   || 
      token == "div" || 
      token == "mod" || 
      token == "and")
   {
      pushOperator(token);
      nextToken();
      part();
      string op = popOperator(), op1 = popOperand(), op2 = popOperand();
      code(op,op1,op2);
      factors();
   }
}

/*Untested but existing*/
void Compiler::part()           // stage 1, production 15
{
   string x = token, y;
   
   if(x == "not")
   {
      y = nextToken();
      if(y == "(")
      {
         
         
         express();
         
         //need to think about how the token comes out of express
         //current expectation is that at the end of a function it will 
         //call next token so we can look at it
         if(token == ")")
         {
            //x = not
            code(x, popOperand());
         }
         else 
         {
            processError("In Part: Expected ) after express is called");
         }
      }
      else if(isBoolean(y))
      {
         if(y == "true")
         {
            pushOperand("false");
         }
         else
         {
            pushOperand("true");
         }
      }
      else if(isNonKeyId(y))
      {
         //x = not
         code(x,y);
      }
      else 
      {
         processError("In Part: Expected ( , boolean, or  after not");
      }
      
   }
   else if(x == "+")
   {
      y = nextToken();
      if(y == "(")
      {
         
         
         express();
        
         if(token != ")")
         {
            processError("In Part: Expected ) after express is called");
         }
      }
      else if(isInteger(y) || isNonKeyId(y))
      {
         pushOperand(y);
      }
      else
      {
         processError("In Part: Expected (,INTEGER, or NON KEY ID after a + ");
      }
   }
   else if(x == "-")
   {
      y = nextToken();
      if(y == "(")
      {
         
         
         express();
        
         if(token != ")")
         {
            processError("In Part: Expected ) after express is called");
         }
         
         code("neg", popOperand());
      }
      else if(isInteger(y))
      {
         x += y;
         pushOperand(x);
      }
      else if(isNonKeyId(y))
      {
         code("neg",y);
      }
      else
      {
         processError("In Part: Expected (,INTEGER, or NON KEY ID after a - ");
      }
   }
   else if(isInteger(x) || 
           isBoolean(x) || 
           isNonKeyId(x))
   {
      pushOperand(x);
   }
   else if( x == "(")
   {
      express();
        
      if(token != ")")
      {
         processError("In Part: Expected ) after express is called");
      }
   }
   else
   {
      processError("not, +, -, (, INTEGER, true, false, or NON KEY ID expected in Part");
   }
   
   
   nextToken();
}

void Compiler::pushOperator(string op)
{
   operatorStk.push(op);
   
}

void Compiler::pushOperand(string operand)
{
   
   if(isLiteral(operand))
   {
      if(symbolTable.find(operand) == symbolTable.end())
      {
        
         //note need to ask motl if pushing the operand should be constant or variable
         insert(operand,whichType(operand),CONSTANT,whichValue(operand),YES,1);
      }
     
   }
   
   operandStk.push(operand);
      
   
}

string Compiler::popOperator()
{
   if(!operatorStk.empty())
   {
      string s = operatorStk.top();
      operatorStk.pop();
      
      ;
      return s;
   }
   else
   {
      processError("compiler error; operator stack underflow");
   }
   
   return "popOperator if issue";
}

string Compiler::popOperand()
{
   if(!operandStk.empty())
   {
      string s = operandStk.top();
      
      operandStk.pop();
      return s;
   }
   else
   {
      processError("compiler error; operand stack underflow");
   }
   
   return "popOperand if issue";
}

void Compiler::freeTemp()
{
    currentTempNo--;
   
    if(currentTempNo < -1)
       processError("compiler error, currentTempNo should be ≥ –1");
}

string Compiler::getTemp()
{
    string temp;
    currentTempNo++;
    temp = "T" + to_string(currentTempNo);
    
    if(currentTempNo > maxTempNo)
    {
       insert(temp, UNKNOWN, VARIABLE, "", NO, 1);
       maxTempNo++;
    }
    return temp;
}

//need to check with motl wtf this is, this is a guess
string Compiler::getLabel()
{
   static int labelCount = 0;
   
   string ret = "L" + to_string(labelCount);
   labelCount++;
   return ret;
}

//Note may be incomplete, currently views all temps as existing in the symbol table,
//may need to worry about a temp that is not in the symbol table
//also check with motl if this is even correct
bool Compiler::isTemporary(string s) const // determines if s represents a temporary
{
   if(symbolTable.find(s) != symbolTable.end())
   {
      if(s[0] == 'T')
         return true;
   }
   return false;
}

/* Validation will happen in these*/


// operands for read code will be variable names
void Compiler::emitReadCode(string operand, string operand2)
{
    string name = operand;
   
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
           
           if(tempName.length() > 15)
           {tempName = tempName.substr(0,15);}
        }
        
        if( i > -1)
        {
           tempName = name.substr(0,i);
           name = name.substr(i+1, name.length() - 1);
           
           if(tempName.length() > 15)
           {tempName = tempName.substr(0,15);}
        }
        
        //whichType will handle  undefined constant
        if(whichType(tempName) != INTEGER)
           processError("can't read variables of this type");
        
        if(symbolTable.at(tempName).getMode() != VARIABLE)
           processError("attempting to read to a read-only location");
        
        /*emits*/
        emit("","call", "ReadInt", "; read int; value placed in eax");
        emit("","mov", "[" + symbolTable.at(tempName).getInternalName() + "],eax", "; store eax at " + tempName);
        contentsOfAReg = tempName;
        
    }
   
}

//operands for write code will be variable names or constant names
void Compiler::emitWriteCode(string operand, string operand2)
{
    string name = operand;
    
    //static bool definedStorage = false;
   
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
           
           if(tempName.length() > 15)
           {tempName = tempName.substr(0,15);}
        }
        
        if( i > -1)
        {
           tempName = name.substr(0,i);
           name = name.substr(i+1, name.length() - 1);
           
           if(tempName.length() > 15)
           {tempName = tempName.substr(0,15);}
        }
        
        if(symbolTable.find(tempName) == symbolTable.end())
           processError("reference to undefined symbol");
        
        if(symbolTable.at(tempName).getDataType() != INTEGER &&
           symbolTable.at(tempName).getDataType() != BOOLEAN)
           processError("INTEGER or BOOLEAN REQUIRED FOR WRITE EMIT");
           
        if(contentsOfAReg != tempName)
        {
           emit("","mov","eax,[" + symbolTable.at(tempName).getInternalName() + "]", "; load " + tempName + " in eax");
           contentsOfAReg = tempName;
        }
        
        emit("", "call", "WriteInt", "; write int in eax to standard out");
        emit("", "call", "Crlf", "; write \\r\\n to standard out");
        /**/
        
    }
}

void Compiler::emitAssignCode(string operand1, string operand2)         // op2 = op1
{
    if(symbolTable.find(operand1) == symbolTable.end())
    {processError("operand1 reference to undefined symbol");}
        
    if(symbolTable.find(operand2) == symbolTable.end())
    {processError("operand2 reference to undefined symbol");}
        
    if (whichType(operand1) != whichType(operand2)) //(types of operands are not the same)
    {processError("incompatible types");}
   
    if (symbolTable.at(operand2).getMode() != VARIABLE) //storage mode of operand2 is not VARIABLE -> NON_KEY_ID
    {processError("symbol on left-hand side of assignment must have a storage mode of VARIABLE");}
    
    if(operand1 == operand2)
    {return;}
 
    
    if(contentsOfAReg != operand1)
    {
       emit("", "mov", "eax,[" + symbolTable.at(operand1).getInternalName() + "]", "; AReg = " + operand1);
       contentsOfAReg = operand1;
    }
    
    
    emit("", "mov", "[" + symbolTable.at(operand2).getInternalName() + "],eax", "; " +operand2 + " = AReg");
    
    contentsOfAReg = operand2;
    
    
}


/*Note from here on out operands can be INTEGER, BOOLEANS, OR NON KEY IDS*/
void Compiler::emitAdditionCode(string operand1, string operand2)       // op2 +  op1
{
	

   
   if(whichType(operand1) != INTEGER || whichType(operand2) != INTEGER) // type of either operand is not integer
   {processError("illegal type");}
   
   if(isTemporary(contentsOfAReg) && contentsOfAReg !=operand1 && contentsOfAReg != operand2)
   {
      emit("", "mov","[" + symbolTable.at(contentsOfAReg).getInternalName() + "],eax","; deassign AReg");
      symbolTable.at(contentsOfAReg).setAlloc(YES);
      contentsOfAReg = "";
   }
   
   if(contentsOfAReg !=operand1 && contentsOfAReg != operand2)
   {
      contentsOfAReg = operand2;
      emit("", "mov", "eax,[" + symbolTable.at(operand2).getInternalName() + "]","; AReg = " + operand2); 
   }
   
   /*Emit addition*/
   if(contentsOfAReg == operand2)
   {
      emit("","add","eax,[" + symbolTable.at(operand1).getInternalName() +"]", "; AReg = " + operand2 + " + " + operand1);
   }
   else
   {
      emit("","add","eax,[" + symbolTable.at(operand2).getInternalName() +"]", "; AReg = " + operand1 + " + " + operand2);
   }
   /*Free temps*/
   if(isTemporary(operand1))
   {freeTemp();}

   if(isTemporary(operand2))
   {freeTemp();}

   
   contentsOfAReg = getTemp();
   
   
   symbolTable.at(contentsOfAReg).setDataType(INTEGER);
   pushOperand(contentsOfAReg);
   
   
   
   
}

void Compiler::emitSubtractionCode(string operand1, string operand2)    // op2 -  op1
{
   if(whichType(operand1) != INTEGER || whichType(operand2) != INTEGER) // type of either operand is not integer
   {processError("illegal type");}
}

void Compiler::emitMultiplicationCode(string operand1, string operand2) // op2 *  op1
{
   
   if(whichType(operand1) != INTEGER || whichType(operand2) != INTEGER) // type of either operand is not integer
   {processError("illegal type");}
   
   if(isTemporary(contentsOfAReg) && contentsOfAReg != operand1 && contentsOfAReg != operand2)
   {
      emit("", "mov","[" + symbolTable.at(contentsOfAReg).getInternalName() + "],eax","; deassign AReg");
      symbolTable.at(contentsOfAReg).setAlloc(YES);
      contentsOfAReg = "";
   }
   
   if(contentsOfAReg !=operand1 && contentsOfAReg != operand2)
   {
      contentsOfAReg = operand2;
      emit("", "mov", "eax,[" + symbolTable.at(operand2).getInternalName() + "]","; AReg = " + operand2); 
   }
   
   /*emit multiplication*/
   if(contentsOfAReg == operand2)
   {
      emit("","imul","dword [" + symbolTable.at(operand1).getInternalName() +"]", "; AReg = " + operand2 + " * " + operand1);
   }
   else
   {
      emit("","imul","dword [" + symbolTable.at(operand2).getInternalName() +"]", "; AReg = " + operand1 + " * " + operand2);
   }
   /*Free temps*/
   if(isTemporary(operand1))
   {freeTemp();}

   if(isTemporary(operand2))
   {freeTemp();}

   
   contentsOfAReg = getTemp();
   
   
   symbolTable.at(contentsOfAReg).setDataType(INTEGER);
   pushOperand(contentsOfAReg);
   
   
}

void Compiler::emitDivisionCode(string operand1, string operand2)       // op2 /  op1
{
   if(whichType(operand1) != INTEGER || whichType(operand2) != INTEGER) // type of either operand is not integer
   {processError("illegal type");}
}

void Compiler::emitModuloCode(string operand1, string operand2)         // op2 %  op1
{
   if(whichType(operand1) != INTEGER || whichType(operand2) != INTEGER) // type of either operand is not integer
   {processError("illegal type");}
}

void Compiler::emitNegationCode(string operand1, string operand2)           // -op1
{
   if(whichType(operand1) != INTEGER) 
   {processError("illegal type");}
}

void Compiler::emitNotCode(string operand1, string operand2)                // !op1
{
   if(whichType(operand1) != BOOLEAN) 
   {processError("illegal type");}
}

void Compiler::emitAndCode(string operand1, string operand2)            // op2 && op1
{
   if(whichType(operand1) != BOOLEAN || whichType(operand2) != BOOLEAN) 
   {processError("illegal type");}
}

void Compiler::emitOrCode(string operand1, string operand2)             // op2 || op1
{
   if(whichType(operand1) != BOOLEAN || whichType(operand2) != BOOLEAN) 
   {processError("illegal type");}
}

void Compiler::emitEqualityCode(string operand1, string operand2)       // op2 == op1
{
   if(whichType(operand1) != BOOLEAN || whichType(operand2) != BOOLEAN) 
   {processError("illegal type");}
}

void Compiler::emitInequalityCode(string operand1, string operand2)     // op2 != op1
{
   if(whichType(operand1) != BOOLEAN || whichType(operand2) != BOOLEAN) 
   {processError("illegal type");}
}

void Compiler::emitLessThanCode(string operand1, string operand2)       // op2 <  op1
{
   if(whichType(operand1) != BOOLEAN || whichType(operand2) != BOOLEAN) 
   {processError("illegal type");}
}

void Compiler::emitLessThanOrEqualToCode(string operand1, string operand2) // op2 <= op1
{
   if(whichType(operand1) != BOOLEAN || whichType(operand2) != BOOLEAN) 
   {processError("illegal type");}
}

void Compiler::emitGreaterThanCode(string operand1, string operand2)    // op2 >  op1
{
   if(whichType(operand1) != BOOLEAN || whichType(operand2) != BOOLEAN) 
   {processError("illegal type");}
}

void Compiler::emitGreaterThanOrEqualToCode(string operand1, string operand2) // op2 >= op1
{
   if(whichType(operand1) != BOOLEAN || whichType(operand2) != BOOLEAN) 
   {processError("illegal type");}
}

