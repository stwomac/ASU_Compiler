#include <fstream>
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
  ifstream sourceFile;
  
  char ch;
  if (argc >=2)         // Check to see if pgm was invoked correctly
  {
	  sourceFile.open(argv[1]);
	  
	  while(!sourceFile.eof())
	  {
		  sourceFile.get(ch);
		  
		  if(ch !='\n' && ch!='\t')
		  cout << ch;
		  else
		  cout << '@';

	
	  }
  }
  else
  {cout << "U FUCKED";
	return 0;}
  
  

  return 0;
}