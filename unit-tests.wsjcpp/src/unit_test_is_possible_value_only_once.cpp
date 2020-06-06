#include "unit_test_is_possible_value_only_once.h"
#include <vector>
#include <wsjcpp_core.h>
#include <sea5kgSudoku.h>

REGISTRY_WSJCPP_UNIT_TEST(UnitTestIsPossibleValueOnlyOnce)

UnitTestIsPossibleValueOnlyOnce::UnitTestIsPossibleValueOnlyOnce()
    : WsjcppUnitTestBase("UnitTestIsPossibleValueOnlyOnce") {
}

// ---------------------------------------------------------------------

void UnitTestIsPossibleValueOnlyOnce::init() {
    // nothing
}

// ---------------------------------------------------------------------

bool UnitTestIsPossibleValueOnlyOnce::run() {
    bool bTestSuccess = true;
    sea5kgSudoku sudoku("123456789", "9x9");
    sudoku.setData("--7-85-32-86-3-957-5--7--8---4758-9-89--4--755721963487--82-56---856-71-365917824");
    std::cout << sudoku.getData() << std::endl;
    std::cout << sudoku.getPrintableData() << std::endl;

    std::vector<std::pair<int,int>> vRegionCells;
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            vRegionCells.push_back(std::pair<int,int>(x,y));
        }
    }
    sea5kgSudokuRegion region(vRegionCells);
    char cValue = '3';
    sudoku.updatePossibleValues();
    int nCount = sudoku.getCountOfPossibleValuesInRegion(cValue, region);
    compareN(bTestSuccess, "Not implemented", nCount, 1);
    return bTestSuccess;
}

