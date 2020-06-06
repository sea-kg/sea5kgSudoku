#include "unit_test_regions9x9.h"
#include <vector>
#include <wsjcpp_core.h>
#include <sea5kgSudoku.h>

REGISTRY_WSJCPP_UNIT_TEST(UnitTestRegions9x9)

UnitTestRegions9x9::UnitTestRegions9x9()
    : WsjcppUnitTestBase("UnitTestRegions9x9") {
}

// ---------------------------------------------------------------------

void UnitTestRegions9x9::init() {
    // nothing
}

// ---------------------------------------------------------------------

bool UnitTestRegions9x9::run() {
    bool bTestSuccess = true;
    sea5kgSudoku sudoku("123456789", "9x9");
    std::vector<sea5kgSudokuRegion> vRegions = sudoku.getRegions();
    compareN(bTestSuccess, "Regions size", vRegions.size(), 27);
    int nRows = 0;
    int nColumns = 0;
    int nSquares = 0;
    for (int i = 0; i < vRegions.size(); i++) {
        std::vector<std::pair<int,int>> vRegionCells = vRegions[i].getRegionCells();
        compareN(bTestSuccess, "Region cells size", vRegionCells.size(), 9);

        std::pair<int,int> minXY = vRegions[i].getMin();
        std::pair<int,int> maxXY = vRegions[i].getMax();
        // column
        if (minXY.first == maxXY.first) {
            compareN(bTestSuccess, "Column " + std::to_string(minXY.first), maxXY.second - minXY.second, 8);
            nColumns++;
        }
        // row
        if (minXY.second == maxXY.second) {
            compareN(bTestSuccess, "Row " + std::to_string(minXY.second), maxXY.first - minXY.first, 8);
            nRows++;
        }

        // squares
        if (maxXY.first - minXY.first == 2 && maxXY.second - minXY.second == 2) {
            nSquares++;
            compareN(bTestSuccess, "Squares x " + std::to_string(minXY.first), minXY.first % 3, 0);
            compareN(bTestSuccess, "Squares y " + std::to_string(minXY.second), minXY.second % 3, 0);
        }

    }
    compareN(bTestSuccess, "Columns ", nColumns, 9);
    compareN(bTestSuccess, "Rows ", nRows, 9);
    compareN(bTestSuccess, "Squares ", nSquares, 9);
    
    return bTestSuccess;
}

