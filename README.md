SolvingSudoku
=============

c++ Solving Sudoku

An algorithm for solving Sudoku 
Class seakgExecSudoku to solve SUDOKU


1. Alphabet Set
	you must set the alphabet, where the first character - is an unknown state
	using the void seakgExecSudoku::setAlphabet (string alphabet);
	example:
		sudoku->setAlphabet("-123456789ABC"); - for sudoku 12x12 
		sudoku->setAlphabet("-123456789"); - for sudoku 9x9
		sudoku->setAlphabet("-123456"); - for sudoku 6 на 6
		
2. Set the initial values ​​of cells
	using the void seakgExecSudoku::setPole( string pole );
	it is clear that for Sudoku 9x9 pole length should be 81 characters, but for 6x6 - 36 characters for 12x12 - 144 characters

	for example Sudoku size 9x9:
		sudoku->setPole( "--7--5--2-86-3-9---5--7------4--8---89-----75---1--3------2--6---8-6-71-3--9--8--" ); 

3. Install dependencies area of ​​cells
	in cases of 9x9 Sudoku has a special method: void seakgExecSudoku :: setOblasty9x9 ();
	best finish his method or some universal for forming areas

	area - is:
		- Lines that should not be repeated values
		- Lines that should not be repeated too values
		- Hmm ... how to call them 'squares'
	can manually set the field by the method (only 1 and 2 point should be made mandatory):

		void seakgExecSudoku::addOblasty( int mas[], int count );
		
	principle simply pass an array of int and its size in the array must be given all the indices of cells included in the current scope (cell indices are numbered from zero, left to right from top to bottom - the one-dimensional array)

	for example
		Sudoku for 6x6 array to be a long one area 6, and the number of regions 6 * 3 = 18
		Sudoku for 9x9 array for one area to be a long nine, and the number of areas 9 * 3 = 27
		for Sudoku 12x12 array for one area will be long 12, and the number of regions 12 * 3 = 36
		and so on

4. And only then start the calculation like this:
	while (sudoku-> shag () == true) {}; - One shag () - is one filled cell


Example program (compiled by gcc under linux)
```
seakg@seakg:~/developing/sudoku/exec_sudoku$ ./exec_sudoku 9---3--4-812-7----------5---5----279---5-9---491----8---4----------1-496-6--2---1

Source data:
1)  | 9 | - | - | - | 3 | - | - | 4 | - |
2)  | 8 | 1 | 2 | - | 7 | - | - | - | - |
3)  | - | - | - | - | - | - | 5 | - | - |
4)  | - | 5 | - | - | - | - | 2 | 7 | 9 |
5)  | - | - | - | 5 | - | 9 | - | - | - |
6)  | 4 | 9 | 1 | - | - | - | - | 8 | - |
7)  | - | - | 4 | - | - | - | - | - | - |
8)  | - | - | - | - | 1 | - | 4 | 9 | 6 |
9)  | - | 6 | - | - | 2 | - | - | - | 1 |

Solution:
1)  | 9 | 7 | 5 | 6 | 3 | 2 | 1 | 4 | 8 |
2)  | 8 | 1 | 2 | 4 | 7 | 5 | 9 | 6 | 3 |
3)  | 3 | 4 | 6 | 8 | 9 | 1 | 5 | 2 | 7 |
4)  | 6 | 5 | 8 | 1 | 4 | 3 | 2 | 7 | 9 |
5)  | 2 | 3 | 7 | 5 | 8 | 9 | 6 | 1 | 4 |
6)  | 4 | 9 | 1 | 2 | 6 | 7 | 3 | 8 | 5 |
7)  | 1 | 8 | 4 | 9 | 5 | 6 | 7 | 3 | 2 |
8)  | 5 | 2 | 3 | 7 | 1 | 8 | 4 | 9 | 6 |
9)  | 7 | 6 | 9 | 3 | 2 | 4 | 8 | 5 | 1 |

9---3--4-812-7----------5---5----279---5-9---491----8---4----------1-496-6--2---1
975632148812475963346891527658143279237589614491267385184956732523718496769324851
```

tests:
```
 17/100:
	--7--5--2-86-3-9---5--7------4--8---89-----75---1--3------2--6---8-6-71-3--9--8--
	947685132286431957153279486634758291891342675572196348719824563428563719365917824
 18/100:
	--------2----96--47---253-6-5--74-9-----------6-13--5-2-854---76--78----9--------
	536417982182396574794825316851674293327958461469132758218549637645783129973261845
 19/100:
	9---3--4-812-7----------5---5----279---5-9---491----8---4----------1-496-6--2---1
	975632148812475963346891527658143279237589614491267385184956732523718496769324851
```


