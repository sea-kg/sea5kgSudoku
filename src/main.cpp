#include <string.h>
#include <iostream>
#include <algorithm>
#include <wsjcpp_core.h>
#include <sea5kgSudoku.h>

void printHelp(const std::string &sProgramName) {
    std::cout
        << std::endl
        << "Usage examples: "
        << std::endl
        << "  " << sProgramName << " solve \"123456\" \"6x6\" \"--3-1-56-32--542-32-645--12-45-4-1--\""
        << std::endl
        << "  " << sProgramName << " solve \"123456789\" \"9x9\" \"--7--5--2-86-3-9---5--7------4--8---89-----75---1--3------2--6---8-6-71-3--9--8--\""
        << std::endl
        << "  " << sProgramName << " generate \"123456\" \"6x6\""
        << std::endl

        << std::endl;
}

int main(int argc, const char* argv[]) {
    std::string TAG = "MAIN";
    std::string appName = std::string(WSJCPP_APP_NAME);
    std::string appVersion = std::string(WSJCPP_APP_VERSION);
    if (!WsjcppCore::dirExists(".logs")) {
        WsjcppCore::makeDir(".logs");
    }
    WsjcppLog::setPrefixLogFile("sea5kgSudoku");
    WsjcppLog::setLogDirectory(".logs");

    WsjcppCore::initRandom();

    std::string sProgramName(argv[0]);
    std::string sSubCommand = "";
    if (argc > 1) {
        sSubCommand = std::string(argv[1]);
    }

    if (sSubCommand == "solve") {
        if (argc != 5) {
            printHelp(sProgramName);
            return -1;
        }
        std::string sAlphabet(argv[2]);
        std::string sSudokuType(argv[3]);
        std::string sData(argv[4]);
        
        sea5kgSudoku sudoku(sAlphabet, sSudokuType);
        sudoku.setData(sData);
        std::cout << "Sudoku (input):" << std::endl << sudoku.getPrintableData() << std::endl;
        sudoku.solve();
        std::cout << "Sudoku (solved):" << std::endl << sudoku.getPrintableData() << std::endl;
        std::cout << "Input: " << sData << std::endl;
        std::cout << "Solved: " << sudoku.getData() << std::endl;
        return 0;
    } else if (sSubCommand == "generate") {
        if (argc != 4) {
            printHelp(sProgramName);
            return -1;
        }
        std::string sAlphabet(argv[2]);
        std::string sSudokuType(argv[3]);
        
        sea5kgSudoku sudoku(sAlphabet, sSudokuType);
        if (sudoku.generate(100)) {
            std::cout << "Sudoku generated:" << std::endl << sudoku.getPrintableData() << std::endl;
            std::cout << "Data: " << sudoku.getData() << std::endl;
            return 0;
        }
        std::cout << "FAIL: Could not try generate sudoku" << std::endl;
        return -1;
    } else if (sSubCommand == "server") {
        std::cout << "Not implemented yet" << std::endl;
        return -1;
    }
    printHelp(sProgramName);
    return -1;
}

