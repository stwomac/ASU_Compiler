#include <stage0.h>
#include <iostream>
#include <fstream>
#include <string>
#include <map>

using namespace std;


Compiler::Compiler(char **argv) // constructor                                   //DONE
{
   if (argc >= 3){
      sourceFile = open(argv[1]);
      listingFile = open(argv[2]);
      objectFile = open(argv[3]);
   }
}

Compiler::~Compiler()           // destructor                                    //DONE
{
   if(soureFile.is_open()){
      sourceFile.close();
   }
   if(listingFile.is_open()){
      listingFile.close();
   }
   if(objectFile.is_open()){
      objectFile.close();
   }
}

void Compiler::createListingHeader()                                             //DONE
{
   cout << "STAGE0:" << "JohnPaul Flores, Steven Womack" << ctime << endl;
   print "LINE " << lineNo << ":" << "SOURCE STATEMENT" << endl;
   //line numbers and source statements should be aligned under the headings 
}
void Compiler::parser()                                                          //possibly done? come back to
{
   nextChar();
   //ch must be initialized to the first character of the source file
   if (nextToken() != "program")
      processError("keyword \"program\" expected");
      //a call to nextToken() has two effects
      // (1) the variable, token, is assigned the value of the next token
      // (2) the next token is read from the source file in order to make
      // the assignment. The value returned by nextToken() is also
      // the next token.
   prog();
   //parser implements the grammar rules, calling first rule
}

void Compiler::createListingTrailer()                                               //DONE
{
   cout << "COMPILATION TERMINATED" << errorCount << " ERRORS ENCOUNTERED";
}

// Methods implementing the grammar productions
void Compiler::prog()           // stage 0, production 1                            //DONE   //check " if (token() != "$" "
{
   if (nextToken() != "program")
      processError("keyword \"program\" expected")
   progStmt()
   if (nextToken() == "const")
      consts()
   if (nextToken() == "var")
      vars()
   if (nextToken() != "begin")
      processError("keyword \"begin\" expected")
   beginEndStmt()
   if (token() != "$")
      processError("no text may follow \"end\"")
}

void Compiler::progStmt()       // stage 0, production 2                               //ask about PROG_NAME, CONSTANT, X, NO
{
   string x
   if (token != "program")
      processError("keyword \"program\" expected")
   x = NextToken()
   if (!isNonKeyId(token))
      processError("program name expected")
   if (token != ";")
      processError("semicolon expected")
   nextToken();
   code("program", x);
   insert(x,PROG_NAME,CONSTANT,x,NO,0); //fix
}

void Compiler::consts()         // stage 0, production 3                                  //DONE
{
   if (token != "const")
      processError("keyword \"const\" expected")
   if (!isNonKeyId(token)) 
      processError("non-keyword identifier must follow \"const\"")
   constStmts()
}

void Compiler::vars()           // stage 0, production 4                                  //DONE
{
   if (token != "var")
      processError("keyword \"var\" expected")
   if (!isNonKeyId(token))
      processError("non-keyword identifier must follow \"var\"")
   varStmts()
}

void Compiler::beginEndStmt()   // stage 0, production 5                                  //DONE
{
   if (token != "begin")
      procesError("keyword \"begin\" expected")
   if (nextToken() != "end")
      processError("keyword \"end\" expected")
   if (nextToken() != ".")
      processError("period expected")
   nextToken()
   code("end", ".")
}

void Compiler::constStmts()     // stage 0, production 6                                  //partially DONE, review line after "y = nextToken()" and insert after "if(!isInteger(y) || !isBoolean(y))
{
   string x,y;
   if (!isNonKeyId(token))
      processError("non-keyword identifier expected")
   x = token;
   if (token != "=")
      processError("\"=\" expected")
   y = nextToken();
   if (y is not one of "+","-","not",NON_KEY_ID,"true","false",INTEGER) //fix get clarification
      processError("token to right of \"=\" illegal")
   if (y == "+" || y =="-") 
   {
      if (!isInteger(nextToken())) 
         processError("integer expected after sign")
      y = y + token;
   }
   if (y == "not")
   {
      if (!isBoolean(nextToken())
         processError("boolean expected after \"not\"")
      if (token == "true");
         y = "false"
      else
         y = "true";
   }
   if (nextToken() != ";")
      processError("semicolon expected")
   if (!isInteger(y) || !isBoolean(y))
      processError("data type of token on the right-hand side must be INTEGER or BOOLEAN");
   insert(x,whichType(y),CONSTANT,whichValue(y),YES,1) //review
   x = nextToken();
   if (!isNonKeyId(x) || x != "begin" || x != "var")
      processError("non-keyword identifier, \"begin\", or \"var\" expected");
   if (isNonKeyId(x))
      constStmts();
}

void Compiler::varStmts()       // stage 0, production 7                                  //reveiw x, y, VARIABLE, YES. partially done, check for any || before this as of
                                                                                          //10-31-2022
{
   string x,y
   if (!isNonKeyId(token)) //fix
      processError("non-keyword identifier expected")
   x = ids();
   if (token != ":")
      processError(":" expected)
   if (!isBoolean(nextToken()) && !isInteger(nextToken()))
      processError("illegal type follows \":\"")
   y = token
   if (nextToken() != ";")
      processError("semicolon expected")
   insert(x,y,VARIABLE,"",YES,1)
   if (!isNonKeyId(nextToken()) && nextToken() != "begin")
      processError("non-keyword identifier or \"begin\" expected")
   if (!isNonKeyId(token))
      varStmts()
}

string Compiler::ids()          // stage 0, production 8
{
   string temp,tempString
   if (token is not a NON_KEY_ID)
      processError(non-keyword identifier expected)
   tempString = token
   temp = token
   if (nextToken() == ",")
   {
      if (nextToken() is not a NON_KEY_ID)
         processError(non-keyword identifier expected)
      tempString = temp + "," + ids()
   }
   return tempString
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
   //create symbol table entry for each identifier in list of external names
   //Multiply inserted names are illegal
   string name
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
            symbolTable[name]=(genInternalName(inType),inType,inMode,inValue,inAlloc,inUnits)
      }
   }
}

storeTypes Compiler::whichType(string name) // tells which data type a name has
{
   if (name is a literal)
      if (name is a boolean literal)
         data type = BOOLEAN
      else
         data type = INTEGER
   else //name is an identifier and hopefully a constant
      if (symbolTable[name] is defined)
         data type = type of symbolTable[name]
      else
         processError(reference to undefined constant)
   return data type
}
string Compiler::whichValue(string name) // tells which value a name has
{
   if (name is a literal)
      value = name
   else //name is an identifier and hopefully a constant
   if (symbolTable[name] is defined and has a value)
      value = value of symbolTable[name]
   else
      processError(reference to undefined constant)
   return value
}

void Compiler::code(string op, string operand1 , string operand2 )
{
   if (op == "program")
      emitPrologue(operand1)
   else if (op == "end")
      emitEpilogue()
   else
      processError(compiler error since function code should not be called with illegal arguments)
}

// Emit Functions
void Compiler::emit(string label , string instruction , string operands , string comment )
{
   Turn on left justification in objectFile
   Output label in a field of width 8
   Output instruction in a field of width 8
   Output the operands in a field of width 24
   Output the comment
}

void Compiler::emitPrologue(string progName, string s)
{
   Output identifying comments at beginning of objectFile
   Output the %INCLUDE directives
   emit("SECTION", ".text")
   emit("global", "_start", "", "; program" + progName)
   emit("_start:")
}

void Compiler::emitEpilogue(string a, string b)
{
   emit("","Exit", "{0}");
   emitStorage();
}

void Compiler::emitStorage()
{
   emit("SECTION", ".data")
   for those entries in the symbolTable that have an allocation of YES and a storage mode of CONSTANT
   { 
      call emit to output a line to objectFile 
   }
   emit("SECTION", ".bss")
   for those entries in the symbolTable that have an allocation of YES and a storage mode of VARIABLE
   { 
      call emit to output a line to objectFile
   }
}


// Lexical routines
char Compiler::nextChar() // returns the next character or END_OF_FILE marker
{
   read in next character
   if end of file
      ch = END_OF_FILE //use a special character to designate end of file
   else
      ch = next character
   print to listing file (starting new line if necessary)
   return ch;
}

string Compiler::nextToken() // returns the next token or END_OF_FILE marker
{
   token = "";
   while (token == "")
   {
      switch(ch)
      {
         case '{' : //process comment
                  while (nextChar() is not one of END_OF_FILE, '}'
                  { //empty body 
                  }
                  if (ch == END_OF_FILE)
                     processError(unexpected end of file)
                  else
                     nextChar()
         case '}' : processError('}' cannot begin token)
         case isspace(ch) : nextChar()
         case isSpecialSymbol(ch): token = ch;
                                   nextChar()
         case islower(ch) : token = ch;
                           while (nextChar() is one of letter, digit, or '_' but not END_OF_FILE)
                           {
                                 token+=ch
                           }
                           if (ch is END_OF_FILE)
                              processError(unexpected end of file)
         case isdigit(ch) : token = ch;
         while (nextChar() is digit but not END_OF_FILE)
         {
            token+=ch
         }
         if (ch is END_OF_FILE)
            processError(unexpected end of file)
         case END_OF_FILE : token = ch
         default : processError(illegal symbol)
      }
   }
   return token;
}

// Other routines
string Compiler::genInternalName(storeTypes stype) const
{
    return "";
}
void Compiler::processError(string err)
{
    
}