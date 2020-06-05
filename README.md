# sea5kgSudoku

[![Build Status](https://api.travis-ci.org/sea-kg/sea5kgSudoku.svg?branch=master)](https://travis-ci.org/sea-kg/sea5kgSudoku)

c++ sudoku solver/generator
* sudoku solver
* sudoku generator
* microservice

## Integrate classes to your project

wsjcpp:
```
wsjcpp install https://github.com/sea-kg/sea5kgSudoku:master
```

or include this files:
* src.wsjcpp/wsjcpp_core/wsjcpp_core.h
* src.wsjcpp/wsjcpp_core/wsjcpp_core.cpp
* src.wsjcpp/nlohmann_json/json.hpp
* src/sea5kgSudoku.cpp
* src/sea5kgSudoku.h

## Build && Run

Requirements for build

* g++
* cmake

``` 
$ ./build_simple.sh
```

Example run

```
$ ./sea5kgSudoku solve "123456" "6x6" "--3-1-56-32--542-32-645--12-45-4-1--"
Sudoku (input):
 +---+---+---+---+---+---+
 | - | - | 3 | - | 1 | - | 
 | 5 | 6 | - | 3 | 2 | - | 
 | - | 5 | 4 | 2 | - | 3 | 
 | 2 | - | 6 | 4 | 5 | - | 
 | - | 1 | 2 | - | 4 | 5 | 
 | - | 4 | - | 1 | - | - | 
 +---+---+---+---+---+---+
Sudoku (solved):
 +---+---+---+---+---+---+
 | 4 | 2 | 3 | 5 | 1 | 6 | 
 | 5 | 6 | 1 | 3 | 2 | 4 | 
 | 1 | 5 | 4 | 2 | 6 | 3 | 
 | 2 | 3 | 6 | 4 | 5 | 1 | 
 | 3 | 1 | 2 | 6 | 4 | 5 | 
 | 6 | 4 | 5 | 1 | 3 | 2 | 
 +---+---+---+---+---+---+
Input: --3-1-56-32--542-32-645--12-45-4-1--
Solved: 423516561324154263236451312645645132

$ ./sea5kgSudoku solve "123456789" "9x9" \
    "--7--5--2-86-3-9---5--7------4--8---89-----75---1--3------2--6---8-6-71-3--9--8--"
Sudoku (input):
 +---+---+---+---+---+---+---+---+---+
 | - | - | 7 | - | - | 5 | - | - | 2 | 
 | - | 8 | 6 | - | 3 | - | 9 | - | - | 
 | - | 5 | - | - | 7 | - | - | - | - | 
 | - | - | 4 | - | - | 8 | - | - | - | 
 | 8 | 9 | - | - | - | - | - | 7 | 5 | 
 | - | - | - | 1 | - | - | 3 | - | - | 
 | - | - | - | - | 2 | - | - | 6 | - | 
 | - | - | 8 | - | 6 | - | 7 | 1 | - | 
 | 3 | - | - | 9 | - | - | 8 | - | - | 
 +---+---+---+---+---+---+---+---+---+
Sudoku (solved):
 +---+---+---+---+---+---+---+---+---+
 | 9 | 4 | 7 | 6 | 8 | 5 | 1 | 3 | 2 | 
 | 2 | 8 | 6 | 4 | 3 | 1 | 9 | 5 | 7 | 
 | 1 | 5 | 3 | 2 | 7 | 9 | 4 | 8 | 6 | 
 | 6 | 3 | 4 | 7 | 5 | 8 | 2 | 9 | 1 | 
 | 8 | 9 | 1 | 3 | 4 | 2 | 6 | 7 | 5 | 
 | 5 | 7 | 2 | 1 | 9 | 6 | 3 | 4 | 8 | 
 | 7 | 1 | 9 | 8 | 2 | 4 | 5 | 6 | 3 | 
 | 4 | 2 | 8 | 5 | 6 | 3 | 7 | 1 | 9 | 
 | 3 | 6 | 5 | 9 | 1 | 7 | 8 | 2 | 4 | 
 +---+---+---+---+---+---+---+---+---+
Input: --7--5--2-86-3-9---5--7------4--8---89-----75---1--3------2--6---8-6-71-3--9--8--
Solved: 947685132286431957153279486634758291891342675572196348719824563428563719365917824
```

## Example usage classes for sudoku solve in code

```
#include <sea5kgSudoku.h>

...

sea5kgSudoku sudoku("123456789", SEA5KG_SUDOKU_9x9);
sudoku.setData("--7--5--2-86-3-9---5--7------4--8---89-----75---1--3------2--6---8-6-71-3--9--8--");

sudoku.solve();

// solution maybe incomplete (!)
std::cout << "Solution:" << std::endl << sudoku.printData() << std::endl;

std::cout << sData << std::endl;
std::cout << sudoku.getOnelineData() << std::endl;
std::cout << "\n";

```

### step by step

You can look sudoku step by step:
```

```
