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
    WsjcppLog::setPrefixLogFile("sea5kgSudoku");
    WsjcppLog::setLogDirectory(".logs");


    std::string sData = "";
    
    if (argc == 2) {
        std::cout << "\nSource data:";
        sData = std::string(argv[1]);
    } else {
        std::cout << "Example:";
        sData = "--7--5--2-86-3-9---5--7------4--8---89-----75---1--3------2--6---8-6-71-3--9--8--";
    };

    if (sData.length() != 81) {
        std::cout << "The number of characters to be 81!\n\n";
        return -1; 
    };
    
    sea5kgSudoku sudoku("123456789");
    sudoku.setData(sData);
    
    std::cout << "Source data: " << std::endl << sudoku.printData() << std::endl;
    sudoku.applyClassicRegionsFor9x9();
    
    // while fulfilling performed
    int nStep = 0;
    std::cout << "Step " << nStep << ": " << sData << std::endl;
    while (sudoku.step()) {
        // nothing
        std::cout << "Step " << nStep << ": " << sudoku.getOnelineData() << std::endl;
        nStep++;
    };
    
    // decision may be incomplete (!)
    std::cout << "Solution:" << std::endl << sudoku.printData() << std::endl;
    
    std::cout << sData << std::endl;
    std::cout << sudoku.getOnelineData() << std::endl;
    std::cout << "\n";
    return 0;
}

