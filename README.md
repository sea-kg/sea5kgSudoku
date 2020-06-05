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

## Basic example usage sudoku solver

```
#include <sea5kgSudoku.h>

...
std::string sData = "--7--5--2-86-3-9---5--7------4--8---89-----75---1--3------2--6---8-6-71-3--9--8--";
sea5kgSudoku sudoku("123456789");
sudoku.setData(sData);
sudoku.applyClassicRegionsFor9x9();

while (sudoku.step()) {
    // step by step
};

// solution maybe incomplete (!)
std::cout << "Solution:" << std::endl << sudoku.printData() << std::endl;

std::cout << sData << std::endl;
std::cout << sudoku.getOnelineData() << std::endl;
std::cout << "\n";

```


