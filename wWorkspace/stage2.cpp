//CS 4301: 010 && 020
//Stage 2 Compiler
//JohnPaul Flores && Steven Womack
//11/4/2022

/*Tired Edition*/


#include <stage2.h>
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
   listingFile << "STAGE2:  " << "JohnPaul Flores, Steven Womack  " << ctime(&now) << "\n";
   listingFile << "LINE NO." << setw(30) << right << "SOURCE STATEMENT" << left << "\n\n" << right;
}

void Compiler::parser()
{  
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
      token = nextToken();
      
      if(symbolTable.find(token) != symbolTable.end())
      {
         token = symbolTable.at(token).getValue();
      }
      
      if (!isBoolean(token))
      {processError("boolean expected after \"not\"");}
      
      if (token == "true")
         y = "false";
      else
         y = "true";
   }
   
   if (nextToken() != ";")
      processError("semicolon expected");
   
   if (whichType(y) != INTEGER && whichType(y) != BOOLEAN)
   {
      processError("data type of token on the right-hand side must be INTEGER or BOOLEAN");
   }

   insert(x,whichType(y),CONSTANT,whichValue(y),YES,1); 
   
   x = nextToken();
   
   if ( x != "begin" && x != "var" && !isNonKeyId(x))
      processError("non-keyword identifier, \"begin\", or \"var\" expected");
   
   if (isNonKeyId(x))
   {
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

   if (!isNonKeyId(token) && token != "begin")
   {
      processError("non-keyword identifier or \"begin\" expected");
   }
   
   if (isNonKeyId(token))
      varStmts();
}

string Compiler::ids()          // stage 0, production 8
{
   string temp, tempString;
	
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
   
   if(s[s.length()-1] == '_')
      return false;
   
	if(!islower(s[0]) )
   {
		 return false;
	}
   else if(!islower(s[s.length() - 1]) && !isdigit(s[s.length() - 1]))
   {
      return false;
   }
   
   if(s.length() < 2 && islower(s[0]))
   {return true;}

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
   for(uint i = 0; i < s.length(); i++)
   {

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
            if(tempName == "FALSE" )
            {
               SymbolTableEntry s(tempName, inType, inMode, inValue, inAlloc, inUnits);
               symbolTable.emplace("false",s);
            }
            else if(tempName == "TRUE")
            {
               SymbolTableEntry s(tempName, inType, inMode, inValue, inAlloc, inUnits);
               symbolTable.emplace("true",s);
            }
            else
            {
               SymbolTableEntry s(tempName, inType, inMode, inValue, inAlloc, inUnits);
               symbolTable.emplace(tempName,s);
            }
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
          processError("reference to undefined symbol " + name);
       }
   }
	
	return dataType;
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
            processError("reference to undefined symbol " + name);
         }
      }
      else
      {
         processError("reference to undefined symbol " + name);
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
      if(x.second.getAlloc() == YES && x.second.getMode() == CONSTANT)
      {
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
      if(x.second.getAlloc() == YES && x.second.getMode() == VARIABLE)
      {
         emit(x.second.getInternalName(), "resd", to_string(x.second.getUnits()), comment); 
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
   
   sourceFile.close();
   listingFile.close();
   objectFile.close();

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
   {processError(":= expected in assignment statement");}
   
   pushOperator(":=");
   
   express();
   
   /*Error statement was decided do due to dataset 113*/
   if(token != ";")
   {processError("one of \"*\", \"and\", \"div\", \"mod\", \")\", \"+\", \"-\", \";\", \"<\", \"<=\", \"<>\", \"=\", \">\", \">=\", or \"or\" expected");}
   
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
       processError("')' expected are non_key_id in \"read\"");
      code("read", x);
      
      if(nextToken() != ";")
         processError("\";\" expected");
      
   }
   else
   {
      processError(" '(' expected after read");
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
       processError("',' or ')' expected after non-keyword identifier");
      code("write", x);
      
      if(nextToken() != ";")
         processError("\";\" expected");
      
   }
   else
   {
      processError(" '(' expected after \"write\"");
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
   {processError("expected non_key_id, integer, \"not\", \"true\", \"false\", '(', '+', or '-'");}
   
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
   {processError("expected non_key_id, integer, \"not\", \"true\", \"false\", '(', '+', or '-'");}
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
   {processError("expected non_key_id, integer, \"not\", \"true\", \"false\", '(', '+', or '-'");}
   part();
   factors();
   
}

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
            code(x, popOperand());
         }
         else 
         {
            processError("')' expected");
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
         code(x,y);
      }
      else 
      {
         processError("expected '(', boolean, or non-keyword id");
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
            processError("')' expected");
         }
      }
      else if(isInteger(y) || isNonKeyId(y))
      {
         pushOperand(y);
      }
      else
      {
         processError("expected '(', integer, or non-keyword id; found +");
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
            processError("')' expected");
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
         processError("expected '(', integer, or non_key_id");
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
         processError("')' expected");
      }
   }
   else
   {
      processError("not, +, -, (, INTEGER, true, false, or NON KEY ID expected");
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
      
	   if(operand.length() > 15)
		  operand = operand.substr(0,15);
      
      if(symbolTable.find(operand) == symbolTable.end())
      {
        
         if(operand == "true")
         {
            insert("TRUE", BOOLEAN, CONSTANT, "-1", YES, 1);
         }
         else if(operand == "false")
         {
            insert("FALSE", BOOLEAN, CONSTANT, "0", YES, 1);
         }
         else
         {
            bool constExist = false;
            for(auto const& x : symbolTable) 
            {
               if(x.second.getValue() == operand && x.second.getMode() == CONSTANT)
               {
                  operand = x.first;
                  constExist = true;
                  break;
               }
            }
            //note need to ask motl if pushing the operand should be constant or variable
            if(!constExist)
            {
             insert(operand,whichType(operand),CONSTANT,whichValue(operand),YES,1);
            }
         }
         
         
      }
     
   }
   else if(isNonKeyId(operand))
   {
      if(operand.length() > 15)
      { operand = operand.substr(0,15);}
      
   }
   
   operandStk.push(operand);
      
   
}

string Compiler::popOperator()
{
   if(!operatorStk.empty())
   {
      string s = operatorStk.top();
      operatorStk.pop();

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

string Compiler::getLabel()
{
   static int labelCount = -1;
   labelCount++;
      
   string ret = ".L" + to_string(labelCount);
   
   return ret;
}

bool Compiler::isTemporary(string s) const // determines if s represents a temporary
{
   if(symbolTable.find(s) != symbolTable.end())
   {
      if(s[0] == 'T')
         return true;
   }
   
   return false;
}

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
         processError("reading in of read-only location " + tempName );
      
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
         processError("reference to undefined symbol " + tempName);
      
      if(symbolTable.at(tempName).getDataType() != INTEGER &&
         symbolTable.at(tempName).getDataType() != BOOLEAN)
         processError("INTEGER or BOOLEAN REQUIRED FOR WRITE EMIT");
         
      if(contentsOfAReg != tempName)
      {
         emit("","mov","eax,[" + symbolTable.at(tempName).getInternalName() + "]", "; load " + tempName + " in eax");
         contentsOfAReg = tempName;
      }
      
      //refrencing data set 15 for this
      //if(symbolTable.at(tempName).getDataType() == INTEGER)
      //{
         emit("", "call", "WriteInt", "; write int in eax to standard out");
         emit("", "call", "Crlf", "; write \\r\\n to standard out");
      //}
      /*
      else
      {
         string labelOne = getLabel(), labelTwo = getLabel();
         emit("", "cmp", "eax,0", "; compare to 0");
         emit("", "je", labelOne, "; jump if equal to print FALSE");
         emit("", "mov", "edx,TRUELIT", "; load address of TRUE literal in edx");
         emit("", "jmp", labelTwo, "; unconditionally jump to " + labelTwo);
         
         emit(labelOne + ":", "", "", "");
         emit("", "mov", "edx,FALSLIT", "; load address of FALSE literal in edx");
         
         emit(labelTwo + ":", "", "", "");
         emit("", "call", "WriteString", "; write string to standard out");
         
         if(!definedStorage)
         {
            emit("\nSECTION ", ".data");
            
            emit("TRUELIT", "db", "'TRUE',0", "; literal string TRUE");
            emit("FALSLIT", "db", "'FALSE',0", "; literal string FALSE");
            
            emit("\nSECTION ", ".text");
            
            definedStorage = true;
            
         }
         emit("", "call", "Crlf", "; write \\r\\n to standard out");
      }
      */
      
   }
}

void Compiler::emitAssignCode(string operand1, string operand2)         // op2 = op1
{
   if(symbolTable.find(operand1) == symbolTable.end())
   {processError("reference to undefined symbol " + operand1);}
       
   if(symbolTable.find(operand2) == symbolTable.end())
   {processError("reference to undefined symbol " + operand2);}
       
   if (whichType(operand1) != whichType(operand2)) //(types of operands are not the same)
   {processError("incompatible types for operator ':='");}
   
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
   
   if(isTemporary(operand1))
   {freeTemp();}
}

/*Note from here on out operands can be INTEGER, BOOLEANS, OR NON KEY IDS*/
void Compiler::emitAdditionCode(string operand1, string operand2)       // op2 +  op1
{
	
   if(whichType(operand1) != INTEGER || whichType(operand2) != INTEGER) // type of either operand is not integer
   {processError("binary '+' requires integer operands");}
   
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
   {processError("binary '-' requires integer operands");}
   
   if(isTemporary(contentsOfAReg) && contentsOfAReg != operand2)
   {
      emit("", "mov","[" + symbolTable.at(contentsOfAReg).getInternalName() + "],eax","; deassign AReg");
      symbolTable.at(contentsOfAReg).setAlloc(YES);
      contentsOfAReg = "";
   }
   
   if(contentsOfAReg != operand2)
   {
      contentsOfAReg = operand2;
      emit("", "mov", "eax,[" + symbolTable.at(operand2).getInternalName() + "]","; AReg = " + operand2); 
   }
   
   emit("", "sub", "eax,[" + symbolTable.at(operand1).getInternalName() + "]", "; AReg = " + operand2 + " - " + operand1);
   
   if(isTemporary(operand1))
   {freeTemp();}

   if(isTemporary(operand2))
   {freeTemp();}

   contentsOfAReg = getTemp();

   symbolTable.at(contentsOfAReg).setDataType(INTEGER);
   pushOperand(contentsOfAReg);
}

void Compiler::emitMultiplicationCode(string operand1, string operand2) // op2 *  op1
{
   
   if(whichType(operand1) != INTEGER || whichType(operand2) != INTEGER) // type of either operand is not integer
   {processError("binary '*' requires integer operands");}
   
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
   {processError("binary 'div' requires integer operands");}
   
   if(isTemporary(contentsOfAReg) && contentsOfAReg != operand2)
   {
      emit("", "mov","[" + symbolTable.at(contentsOfAReg).getInternalName() + "],eax","; deassign AReg");
      symbolTable.at(contentsOfAReg).setAlloc(YES);
      contentsOfAReg = "";
   }
   
   if(contentsOfAReg != operand2)
   {
      contentsOfAReg = operand2;
      emit("", "mov", "eax,[" + symbolTable.at(operand2).getInternalName() + "]","; AReg = " + operand2); 
   }
   
   emit("", "cdq", "", "; sign extend dividend from eax to edx:eax");
   emit("", "idiv", "dword [" + symbolTable.at(operand1).getInternalName() + "]", "; AReg = " + operand2 + " div " + operand1);
   
   if(isTemporary(operand1))
   {freeTemp();}

   if(isTemporary(operand2))
   {freeTemp();}

   contentsOfAReg = getTemp();

   symbolTable.at(contentsOfAReg).setDataType(INTEGER);
   pushOperand(contentsOfAReg);
}

void Compiler::emitModuloCode(string operand1, string operand2)         // op2 %  op1
{
   if(whichType(operand1) != INTEGER || whichType(operand2) != INTEGER) // type of either operand is not integer
   {processError("binary 'mod' requires integer operands");}
   
   if(isTemporary(contentsOfAReg) && contentsOfAReg != operand2)
   {
      emit("", "mov","[" + symbolTable.at(contentsOfAReg).getInternalName() + "],eax","; deassign AReg");
      symbolTable.at(contentsOfAReg).setAlloc(YES);
      contentsOfAReg = "";
   }
   
   if(contentsOfAReg != operand2)
   {
      contentsOfAReg = operand2;
      emit("", "mov", "eax,[" + symbolTable.at(operand2).getInternalName() + "]","; AReg = " + operand2); 
   }
   
   emit("", "cdq", "", "; sign extend dividend from eax to edx:eax");
   emit("", "idiv", "dword [" + symbolTable.at(operand1).getInternalName() + "]", "; AReg = " + operand2 + " div " + operand1);
   emit("", "xchg", "eax,edx", "; exchange quotient and remainder");
   
   if(isTemporary(operand1))
   {freeTemp();}

   if(isTemporary(operand2))
   {freeTemp();}

   contentsOfAReg = getTemp();

   symbolTable.at(contentsOfAReg).setDataType(INTEGER);
   pushOperand(contentsOfAReg);
   
}

void Compiler::emitNegationCode(string operand1, string operand2)           // -op1
{
   if(whichType(operand1) != INTEGER) // type of either operand is not integer
   {processError("unary '-' requires an integer operand");}
   
   if(isTemporary(contentsOfAReg) && contentsOfAReg !=operand1)
   {
      emit("", "mov","[" + symbolTable.at(contentsOfAReg).getInternalName() + "],eax","; deassign AReg");
      symbolTable.at(contentsOfAReg).setAlloc(YES);
      contentsOfAReg = "";
   }
   
   if(contentsOfAReg !=operand1 )
   {
      contentsOfAReg = operand1;
      emit("", "mov", "eax,[" + symbolTable.at(operand1).getInternalName() + "]","; AReg = " + operand1); 
   }
   
   /*Emit negation*/
   emit("", "neg", "eax", "; AReg = -AReg");
   
   /*Free temps*/
   if(isTemporary(operand1))
   {freeTemp();}
   
   contentsOfAReg = getTemp();

   symbolTable.at(contentsOfAReg).setDataType(INTEGER);
   pushOperand(contentsOfAReg);
}

void Compiler::emitNotCode(string operand1, string operand2)                // !op1
{
   if(whichType(operand1) != BOOLEAN) // type of either operand is not integer
   {processError("unary 'not' requires a boolean operand");}
   
   if(isTemporary(contentsOfAReg) && contentsOfAReg !=operand1)
   {
      emit("", "mov","[" + symbolTable.at(contentsOfAReg).getInternalName() + "],eax","; deassign AReg");
      symbolTable.at(contentsOfAReg).setAlloc(YES);
      contentsOfAReg = "";
   }
   
   if(contentsOfAReg !=operand1 )
   {
      contentsOfAReg = operand1;
      emit("", "mov", "eax,[" + symbolTable.at(operand1).getInternalName() + "]","; AReg = " + operand1); 
   }
   
   /*Emit negation*/
   emit("", "not", "eax", "; AReg = !AReg");
   
   /*Free temps*/
   if(isTemporary(operand1))
   {freeTemp();}
   
   contentsOfAReg = getTemp();

   symbolTable.at(contentsOfAReg).setDataType(BOOLEAN);
   pushOperand(contentsOfAReg);
}

void Compiler::emitAndCode(string operand1, string operand2)            // op2 && op1
{
   if(whichType(operand1) != BOOLEAN || whichType(operand2) != BOOLEAN) // type of either operand is not integer
   {processError("binary 'and' requires boolean operands");}
   
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
   
   /*Emit and*/
   if(contentsOfAReg == operand2)
   {
      emit("","and","eax,[" + symbolTable.at(operand1).getInternalName() +"]", "; AReg = " + operand2 + " and " + operand1);
   }
   else
   {
      emit("","and","eax,[" + symbolTable.at(operand2).getInternalName() +"]", "; AReg = " + operand1 + " and " + operand2);
   }
   
   /*Free temps*/
   if(isTemporary(operand1))
   {freeTemp();}

   if(isTemporary(operand2))
   {freeTemp();}

   contentsOfAReg = getTemp();

   symbolTable.at(contentsOfAReg).setDataType(BOOLEAN);
   pushOperand(contentsOfAReg);
}

void Compiler::emitOrCode(string operand1, string operand2)             // op2 || op1
{
   if(whichType(operand1) != BOOLEAN || whichType(operand2) != BOOLEAN) // type of either operand is not integer
   {processError("logical 'or' requires boolean operands");}
   
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
   
   /*Emit or*/
   if(contentsOfAReg == operand2)
   {
      emit("","or","eax,[" + symbolTable.at(operand1).getInternalName() +"]", "; AReg = " + operand2 + " or " + operand1);
   }
   else
   {
      emit("","or","eax,[" + symbolTable.at(operand2).getInternalName() +"]", "; AReg = " + operand1 + " or " + operand2);
   }
   
   /*Free temps*/
   if(isTemporary(operand1))
   {freeTemp();}

   if(isTemporary(operand2))
   {freeTemp();}

   contentsOfAReg = getTemp();

   symbolTable.at(contentsOfAReg).setDataType(BOOLEAN);
   pushOperand(contentsOfAReg);
}

void Compiler::emitEqualityCode(string operand1, string operand2)       // op2 == op1
{
   if(whichType(operand1) != whichType(operand2)) // type of either operand is not integer
   {processError("binary '=' requires operands of the same type");}
   
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

   string labelOne = getLabel(), labelTwo = getLabel();
   if(contentsOfAReg == operand2)
   {
      emit("", "cmp", "eax,[" + symbolTable.at(operand1).getInternalName() + "]", "; compare " +operand2 + " and " + operand1);
      emit("", "je", labelOne, "; if " + operand2 + " = " + operand1 + " then jump to set eax to TRUE");
      emit("", "mov", "eax,[FALSE]", "; else set eax to FALSE");
      emit("", "jmp", labelTwo, "; unconditionally jump");
      
      emit(labelOne + ":", "", "", "");
      emit("", "mov", "eax,[TRUE]", "; set eax to TRUE");
      
      emit(labelTwo + ":", "", "", "");
   }
   else
   {
      emit("", "cmp", "eax,[" + symbolTable.at(operand2).getInternalName() + "]", "; compare " +operand1 + " and " + operand2);
      emit("", "je", labelOne, "; if " + operand2 + " = " + operand1 + " then jump to set eax to TRUE");
      emit("", "mov", "eax,[FALSE]", "; else set eax to FALSE");
      emit("", "jmp", labelTwo, "; unconditionally jump");
      
      emit(labelOne + ":", "", "", "");
      emit("", "mov", "eax,[TRUE]", "; set eax to TRUE");
      
      emit(labelTwo + ":", "", "", "");
   }
   
   if(symbolTable.find("true") == symbolTable.end())
      insert("TRUE", BOOLEAN, CONSTANT, "-1", YES, 1);

   if(symbolTable.find("false") == symbolTable.end())
      insert("FALSE", BOOLEAN, CONSTANT, "0", YES, 1);
  
   /*Free temps*/
   if(isTemporary(operand1))
   {freeTemp();}

   if(isTemporary(operand2))
   {freeTemp();}

   contentsOfAReg = getTemp();

   symbolTable.at(contentsOfAReg).setDataType(BOOLEAN);
   pushOperand(contentsOfAReg);
}

void Compiler::emitInequalityCode(string operand1, string operand2)     // op2 != op1
{
   if(whichType(operand1) != whichType(operand2)) // type of either operand is not integer
   {processError("binary '<>' requires operands of the same type");}
   
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

   string labelOne = getLabel(), labelTwo = getLabel();
   if(contentsOfAReg == operand2)
   {
      emit("", "cmp", "eax,[" + symbolTable.at(operand1).getInternalName() + "]", "; compare " +operand2 + " and " + operand1);
      emit("", "jne", labelOne, "; if " + operand2 + " <> " + operand1 + " then jump to set eax to TRUE");
      emit("", "mov", "eax,[FALSE]", "; else set eax to FALSE");
      emit("", "jmp", labelTwo, "; unconditionally jump");
      
      emit(labelOne + ":", "", "", "");
      emit("", "mov", "eax,[TRUE]", "; set eax to TRUE");
      
      emit(labelTwo + ":", "", "", "");
   }
   else
   {
      emit("", "cmp", "eax,[" + symbolTable.at(operand2).getInternalName() + "]", "; compare " +operand1 + " and " + operand2);
      emit("", "jne", labelOne, "; if " + operand2 + " <> " + operand1 + " then jump to set eax to TRUE");
      emit("", "mov", "eax,[FALSE]", "; else set eax to FALSE");
      emit("", "jmp", labelTwo, "; unconditionally jump");
      
      emit(labelOne + ":", "", "", "");
      emit("", "mov", "eax,[TRUE]", "; set eax to TRUE");
      
      emit(labelTwo + ":", "", "", "");
   }
   
   if(symbolTable.find("true") == symbolTable.end())
      insert("TRUE", BOOLEAN, CONSTANT, "-1", YES, 1);

   if(symbolTable.find("false") == symbolTable.end())
      insert("FALSE", BOOLEAN, CONSTANT, "0", YES, 1);
  
   /*Free temps*/
   if(isTemporary(operand1))
   {freeTemp();}

   if(isTemporary(operand2))
   {freeTemp();}

   contentsOfAReg = getTemp();

   symbolTable.at(contentsOfAReg).setDataType(BOOLEAN);
   pushOperand(contentsOfAReg);
}

void Compiler::emitLessThanCode(string operand1, string operand2)       // op2 <  op1
{
   if(whichType(operand1) != INTEGER || whichType(operand2) != INTEGER) // type of either operand is not integer
   {processError("binary '<' requires integer operands");}
   
   if(isTemporary(contentsOfAReg) && contentsOfAReg != operand2)
   {
      emit("", "mov","[" + symbolTable.at(contentsOfAReg).getInternalName() + "],eax","; deassign AReg");
      symbolTable.at(contentsOfAReg).setAlloc(YES);
      contentsOfAReg = "";
   }
   
   if(contentsOfAReg != operand2)
   {
      contentsOfAReg = operand2;
      emit("", "mov", "eax,[" + symbolTable.at(operand2).getInternalName() + "]","; AReg = " + operand2); 
   }
   
   string labelOne = getLabel(), labelTwo = getLabel();
   emit("", "cmp", "eax,[" + symbolTable.at(operand1).getInternalName() + "]", "; compare " +operand2 + " and " + operand1);
   emit("", "jl", labelOne, "; if " + operand2 + " < " + operand1 + " then jump to set eax to TRUE");
   emit("", "mov", "eax,[FALSE]", "; else set eax to FALSE");
   emit("", "jmp", labelTwo, "; unconditionally jump");
   
   emit(labelOne + ":", "", "", "");
   emit("", "mov", "eax,[TRUE]", "; set eax to TRUE");
   
   emit(labelTwo + ":", "", "", "");
   
   if(symbolTable.find("true") == symbolTable.end())
      insert("TRUE", BOOLEAN, CONSTANT, "-1", YES, 1);

   if(symbolTable.find("false") == symbolTable.end())
      insert("FALSE", BOOLEAN, CONSTANT, "0", YES, 1);
   
   if(isTemporary(operand1))
   {freeTemp();}

   if(isTemporary(operand2))
   {freeTemp();}

   contentsOfAReg = getTemp();

   symbolTable.at(contentsOfAReg).setDataType(BOOLEAN);
   pushOperand(contentsOfAReg);
}

void Compiler::emitLessThanOrEqualToCode(string operand1, string operand2) // op2 <= op1
{
   if(whichType(operand1) != INTEGER || whichType(operand2) != INTEGER) // type of either operand is not integer
   {processError("binary '<=' requires integer operands");}
   
   if(isTemporary(contentsOfAReg) && contentsOfAReg != operand2)
   {
      emit("", "mov","[" + symbolTable.at(contentsOfAReg).getInternalName() + "],eax","; deassign AReg");
      symbolTable.at(contentsOfAReg).setAlloc(YES);
      contentsOfAReg = "";
   }
   
   if(contentsOfAReg != operand2)
   {
      contentsOfAReg = operand2;
      emit("", "mov", "eax,[" + symbolTable.at(operand2).getInternalName() + "]","; AReg = " + operand2); 
   }
   
   string labelOne = getLabel(), labelTwo = getLabel();
   emit("", "cmp", "eax,[" + symbolTable.at(operand1).getInternalName() + "]", "; compare " +operand2 + " and " + operand1);
   emit("", "jle", labelOne, "; if " + operand2 + " <= " + operand1 + " then jump to set eax to TRUE");
   emit("", "mov", "eax,[FALSE]", "; else set eax to FALSE");
   emit("", "jmp", labelTwo, "; unconditionally jump");
   
   emit(labelOne + ":", "", "", "");
   emit("", "mov", "eax,[TRUE]", "; set eax to TRUE");
   
   emit(labelTwo + ":", "", "", "");
   
   if(symbolTable.find("true") == symbolTable.end())
      insert("TRUE", BOOLEAN, CONSTANT, "-1", YES, 1);

   if(symbolTable.find("false") == symbolTable.end())
      insert("FALSE", BOOLEAN, CONSTANT, "0", YES, 1);
   
   if(isTemporary(operand1))
   {freeTemp();}

   if(isTemporary(operand2))
   {freeTemp();}

   contentsOfAReg = getTemp();

   symbolTable.at(contentsOfAReg).setDataType(BOOLEAN);
   pushOperand(contentsOfAReg);
}

void Compiler::emitGreaterThanCode(string operand1, string operand2)    // op2 >  op1
{
   if(whichType(operand1) != INTEGER || whichType(operand2) != INTEGER) // type of either operand is not integer
   {processError("binary '>' requires integer operands");}
   
   if(isTemporary(contentsOfAReg) && contentsOfAReg != operand2)
   {
      emit("", "mov","[" + symbolTable.at(contentsOfAReg).getInternalName() + "],eax","; deassign AReg");
      symbolTable.at(contentsOfAReg).setAlloc(YES);
      contentsOfAReg = "";
   }
   
   if(contentsOfAReg != operand2)
   {
      contentsOfAReg = operand2;
      emit("", "mov", "eax,[" + symbolTable.at(operand2).getInternalName() + "]","; AReg = " + operand2); 
   }
   
   string labelOne = getLabel(), labelTwo = getLabel();
   emit("", "cmp", "eax,[" + symbolTable.at(operand1).getInternalName() + "]", "; compare " +operand2 + " and " + operand1);
   emit("", "jg", labelOne, "; if " + operand2 + " > " + operand1 + " then jump to set eax to TRUE");
   emit("", "mov", "eax,[FALSE]", "; else set eax to FALSE");
   emit("", "jmp", labelTwo, "; unconditionally jump");
   
   emit(labelOne + ":", "", "", "");
   emit("", "mov", "eax,[TRUE]", "; set eax to TRUE");
   
   emit(labelTwo + ":", "", "", "");
   
   if(symbolTable.find("true") == symbolTable.end())
      insert("TRUE", BOOLEAN, CONSTANT, "-1", YES, 1);

   if(symbolTable.find("false") == symbolTable.end())
      insert("FALSE", BOOLEAN, CONSTANT, "0", YES, 1);
   
   if(isTemporary(operand1))
   {freeTemp();}

   if(isTemporary(operand2))
   {freeTemp();}

   contentsOfAReg = getTemp();

   symbolTable.at(contentsOfAReg).setDataType(BOOLEAN);
   pushOperand(contentsOfAReg);

}

void Compiler::emitGreaterThanOrEqualToCode(string operand1, string operand2) // op2 >= op1
{
   if(whichType(operand1) != INTEGER || whichType(operand2) != INTEGER) // type of either operand is not integer
   {processError("binary '>=' requires integer operands");}
   
   if(isTemporary(contentsOfAReg) && contentsOfAReg != operand2)
   {
      emit("", "mov","[" + symbolTable.at(contentsOfAReg).getInternalName() + "],eax","; deassign AReg");
      symbolTable.at(contentsOfAReg).setAlloc(YES);
      contentsOfAReg = "";
   }
   
   if(contentsOfAReg != operand2)
   {
      contentsOfAReg = operand2;
      emit("", "mov", "eax,[" + symbolTable.at(operand2).getInternalName() + "]","; AReg = " + operand2); 
   }
   
   string labelOne = getLabel(), labelTwo = getLabel();
   emit("", "cmp", "eax,[" + symbolTable.at(operand1).getInternalName() + "]", "; compare " +operand2 + " and " + operand1);
   emit("", "jge", labelOne, "; if " + operand2 + " >= " + operand1 + " then jump to set eax to TRUE");
   emit("", "mov", "eax,[FALSE]", "; else set eax to FALSE");
   emit("", "jmp", labelTwo, "; unconditionally jump");
   
   emit(labelOne + ":", "", "", "");
   emit("", "mov", "eax,[TRUE]", "; set eax to TRUE");
   
   emit(labelTwo + ":", "", "", "");
   
   if(symbolTable.find("true") == symbolTable.end())
      insert("TRUE", BOOLEAN, CONSTANT, "-1", YES, 1);

   if(symbolTable.find("false") == symbolTable.end())
      insert("FALSE", BOOLEAN, CONSTANT, "0", YES, 1);
   
   if(isTemporary(operand1))
   {freeTemp();}

   if(isTemporary(operand2))
   {freeTemp();}

   contentsOfAReg = getTemp();

   symbolTable.at(contentsOfAReg).setDataType(BOOLEAN);
   pushOperand(contentsOfAReg);
}

/* Stage 2*/
void Compiler::code(string op, string operand1 , string operand2 )
{
   if (op == "program")
   {
      emitPrologue(operand1);
   }
   else if (op == "end")
   {
      //cout << " made it to emit \n";
      if(operand1 == ".")
      {  
         if (token != "$")
           processError("no text may follow \"end.\"");
         
         emitEpilogue();
         
      }
      else if(operand1 == ";")
      {//currently do nothing may need to think on this further}
      }
      else
      {processError("neither . or ; as operand1");}
      
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
   else if(op == "then")
   {
      emitThenCode(operand1, operand2);
   }
   else if(op == "else")
   {
      emitElseCode(operand1, operand2);
   }
   else if(op == "post if")
   {
      emitPostIfCode(operand1, operand2);
   }
   else if(op == "while")
   {
      emitWhileCode(operand1, operand2);
   }
   else if(op == "do")
   {
      emitDoCode(operand1, operand2);
   }
   else if(op == "post while")
   {
      emitPostWhileCode(operand1, operand2);
   }
   else if(op == "repeat")
   {
      emitRepeatCode(operand1,operand2);
   }
   else if(op == "until")
   {
      emitUntilCode(operand1,operand2);
   }
   else
   {
      processError("compiler error since function code should not be called with illegal arguments");
   }
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
       s == "write"   ||
       s == "if"      ||
       s == "then"    ||
       s == "else"    ||
       s == "while"   ||
       s == "do"      ||
       s == "repeat"  ||
       s == "until")
	{return true;}
	
	return false;
}

void Compiler::beginEndStmt()   // stage 0, production 5
{
   if (token != "begin")
      processError("keyword \"begin\" expected");
   
   nextToken();
   execStmts();
   
   //changing nextToken to token (gonna call next token at end of execStmts
   if (token != "end")
      processError("one of \";\", \"begin\", \"if\", \"read\", \"repeat\", \"while\", \"write\", \"end\", or \"until\" expected");
   
   nextToken();
   if (token != "." && token != ";")
      processError("'.' or ';' expected");
   
   string x = token;
   nextToken();

   code("end", x);
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
   else if(token == "if")
   {
      ifStmt();
   }
   else if(token == "while")
   {
      whileStmt();
   }
   else if(token == "repeat")
   {
      repeatStmt();
   }
   else if(token == ";")
   {
      nullStmt();
   }
   else if(token == "begin")
   {
      beginEndStmt();
   }
   else
   {
      processError("executable statement expected");
   }
   
}

void Compiler::execStmts()      // stage 1, production 2
{
   if(isNonKeyId(token)    || 
      token == "read"      ||
      token == "write"     ||
      token == "if"        ||
      token == "while"     ||
      token == "repeat"    ||
      token == ";"         ||
      token == "begin")
   {
      execStmt();
      
      if(token == ";")
      {
         nextToken();
      }
      execStmts();
   }

}


//token is on if
void Compiler::ifStmt()         // stage 2, production 3
{
   if(token != "if")
      processError("if expected");
   
   express();
   
   //token on then
   if(token != "then")
      processError("then expected after if check");
   
   string op1 = popOperand();
   code("then", op1);
   
   nextToken();

   execStmt();
   
   //need this cause we could end at the end of a assign/read/write/
   if(token == ";")
      nextToken();
   
   elsePt();
   
}

void Compiler::elsePt()         // stage 2, production 4
{
   if(token == "else")
   {
      string op1 = popOperand();
      code("else", op1);
      
      nextToken();
      execStmt();
      
      //token could be on ;
      op1 = popOperand();
      code("post if", op1);
   }
   else if( 
      token == "end"    ||
      token == ";"      ||
      isNonKeyId(token) ||
      token == "until"  ||
      token == "begin"  ||
      token == "while"  ||
      token == "if"     ||
      token == "repeat" ||
      token == "read"   ||
      token == "write")
   {
      string op1 = popOperand();
      code("post if", op1);
   }
   else
   {
      processError("invalid token in elsePt");
   }
}

void Compiler::whileStmt()      // stage 2, production 5
{
   if(token != "while")
   {
      processError("while expected");
   }
   
   code("while");
   express();
   
   if(token != "do")
   {
      processError("'do' expected");
   }
   
   string op1 = popOperand(), op2;
   
   code("do", op1);
   
   nextToken();
   execStmt();
   
   op1 = popOperand();
   op2 = popOperand();
   
   code("post while", op1, op2);
}

void Compiler::repeatStmt()     // stage 2, production 6
{
   if(token != "repeat")
   {
      processError("repeat expected");
   }
   
   code("repeat");
   
   nextToken();
   execStmts();

   if(token != "until")
   {
      processError("until expected");
   }
   
   express();
   
   string op1 = popOperand(), op2 = popOperand();
   
   code("until", op1, op2);
   
   if(token != ";")
   {
      processError("; expected at end of repeat expression");
   }
   
}

void Compiler::nullStmt()       // stage 2, production 7
{
   if(token != ";")
      processError("null statement did not recieve a ;");
}

// Emit functions for Stage 2

// emit code which follows 'then' and statement predicate
void Compiler::emitThenCode(string operand1, string operand2)
{
	string tempLabel;
   
   if(whichType(operand1) != BOOLEAN)
   {
      processError("predicate of if must be boolean valued");
   }
   
   tempLabel = getLabel();
   
   if(contentsOfAReg != operand1)
   {
      contentsOfAReg = operand1;
      emit("", "mov", "eax,[" + symbolTable.at(operand1).getInternalName() + "]","; AReg = " + operand1); 
   }
   
   emit("", "cmp", "eax,0", "; compare eax to 0");
   emit("", "je", tempLabel, "; if " + operand1 + " is false then jump to end of if");
   pushOperand(tempLabel);
   
   if(isTemporary(operand1))
   {freeTemp();}

   contentsOfAReg = "";
}

// emit code which follows 'else' clause of 'if' statement
void Compiler::emitElseCode(string operand1, string operand2)
{
	string tempLabel = getLabel();
   
   emit("", "jmp", tempLabel, "; jump to end if");
   emit(operand1 + ":", "", "", "; else");
   
   pushOperand(tempLabel);
   
   contentsOfAReg = "";
}

// emit code which follows end of 'if' statement
void Compiler::emitPostIfCode(string operand1, string operand2)
{
	emit(operand1 + ":", "", "", "; end if");
   contentsOfAReg = "";
}

// emit code following 'while'
void Compiler::emitWhileCode(string operand1, string operand2)
{
	string tempLabel = getLabel();
   emit(tempLabel + ":", "", "", "; while");
   pushOperand(tempLabel);
   contentsOfAReg = "";
}

// emit code following 'do'
void Compiler::emitDoCode(string operand1, string operand2)
{
	string tempLabel;
   
   if(whichType(operand1) != BOOLEAN)
   {
      processError("predicate of while must be of type boolean");
   }
   
   tempLabel = getLabel();
   
   if(contentsOfAReg != operand1)
   {
      contentsOfAReg = operand1;
      emit("", "mov", "eax,[" + symbolTable.at(operand1).getInternalName() + "]","; AReg = " + operand1); 
   }
   
   emit("", "cmp", "eax,0", "; compare eax to 0");
   emit("", "je", tempLabel, "; if " + operand1 + " is false then jump to end while");
   pushOperand(tempLabel);
   
   if(isTemporary(operand1))
   {freeTemp();}

   contentsOfAReg = "";
   
}

// emit code at end of 'while' loop;
// operand2 is the label of the beginning of the loop
// operand1 is the label which should follow the end of the loop
void Compiler::emitPostWhileCode(string operand1, string operand2)
{
	emit("", "jmp", operand2, "; end while");
   emit(operand1 + ":", "", "", "");
   contentsOfAReg = "";
}

// emit code which follows 'repeat'
void Compiler::emitRepeatCode(string operand1, string operand2)
{
	string tempLabel = getLabel();
   //cout << endl<< "in emit" <<endl;
   emit(tempLabel + ":", "", "","; repeat");
   pushOperand(tempLabel);
   contentsOfAReg = "";
}

// emit code which follows 'until' and the predicate of loop
// operand1 is the value of the predicate
// operand2 is the label which points to the beginning of the loop
void Compiler::emitUntilCode(string operand1, string operand2)
{
	if(whichType(operand1) != BOOLEAN)
   {
      processError("predicate of until must be of type boolean");
   }
   
   if(contentsOfAReg != operand1)
   {
      contentsOfAReg = operand1;
      emit("", "mov", "eax,[" + symbolTable.at(operand1).getInternalName() + "]","; AReg = " + operand1); 
   }
   
   emit("", "cmp", "eax,0", "; compare eax to 0");
   emit("", "je", operand2, "; until "+ operand1+" is true");
   
   if(isTemporary(operand1))
   {freeTemp();}

   contentsOfAReg = "";
}

bool Compiler::isLabel(string s) const     // determines if s represents a label
{
	if(s.length() > 2 && s[0] == '.' && s[1] == 'L')
   {return true;}

   return false;
}
