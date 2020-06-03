#include <string.h>
#include <iostream>
#include <algorithm>
#include <wsjcpp_core.h>
#include <sea5kgSudoku.h>

int main(int argc, const char* argv[]) {
    std::string TAG = "MAIN";
    std::string appName = std::string(WSJCPP_APP_NAME);
    std::string appVersion = std::string(WSJCPP_APP_VERSION);
    if (!WsjcppCore::dirExists(".logs")) {
        WsjcppCore::makeDir(".logs");
    }
    WsjcppLog::setPrefixLogFile("wsjcpp");
    WsjcppLog::setLogDirectory(".logs");


    std::string pole = "";
    
    if (argc == 2) {
        std::cout << "\nSource data:";
        pole = std::string(argv[1]);
    } else {
        std::cout << "Example:";
        pole = "--7--5--2-86-3-9---5--7------4--8---89-----75---1--3------2--6---8-6-71-3--9--8--";
    };

    if (pole.length() != 81) {
        std::cout << "The number of characters to be 81!\n\n";
        return -1; 
    };
    
    sea5kgSudoku sudoku("123456789");
    sudoku.setData( pole );
    
    std::cout << "Source data: " << std::endl << sudoku.printData() << std::endl;
    sudoku.applyClassicRegionsFor9x9();
    
    //while fulfilling performed
    // sudoku.step();
    int nStep = 0;
    while (sudoku.step()) {
        // nothing
        std::cout << "Step " << nStep << ":";
        nStep++;
        sudoku.coutPoleSimple();
    };

    sudoku.step();
    sudoku.coutPoleSimple();
    
    //decision may be incomplete (!)
    std::cout << "Solution:" << std::endl << sudoku.printData() << std::endl;
    
    std::cout << pole << "\n";
    sudoku.coutPoleSimple();
    std::cout
        << "Expected" << std::endl
        << "947685132286431957153279486634758291891342675572196348719824563428563719365917824" << std::endl
    ;
    std::cout << "\n";
    return 0;
}

