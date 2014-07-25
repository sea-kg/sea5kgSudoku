
#include <iostream>
#include <string>

#include "seakgExecSudoku.h"

using namespace std;


int main(int argc, char *argv[])
{
	string pole = "";
	
	if( argc == 2 )
	{
		cout << "\nSource data:";
		pole = string(argv[1]);
	}
	else
	{
		cout << "Example:";
		pole = "--7--5--2-86-3-9---5--7------4--8---89-----75---1--3------2--6---8-6-71-3--9--8--";
	};
	
	
	
	if( pole.length() != 81 )
	{
		cout << "The number of characters to be 81!\n\n";
		return -1; 
	};
	
	seakgExecSudoku *sudoku = new seakgExecSudoku();
	sudoku->clearAll();
	sudoku->setAlphabet("-123456789");
	sudoku->setPole( pole );
	sudoku->coutPole();
	sudoku->setOblasty9x9();
	
	//while fulfilling performed
	while( sudoku->shag() == true ) { };
	
	//decision may be incomplete (!)
	cout << "Solution:";
	sudoku->coutPole();
	
	cout << pole << "\n";
	sudoku->coutPoleSimple();
	
	sudoku->~seakgExecSudoku();
	cout << "\n";
	return 0;
};
