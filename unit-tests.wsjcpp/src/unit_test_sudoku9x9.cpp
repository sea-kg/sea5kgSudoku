#include "unit_test_sudoku9x9.h"
#include <vector>
#include <wsjcpp_core.h>

REGISTRY_WSJCPP_UNIT_TEST(UnitTestSudoku9x9)

UnitTestSudoku9x9::UnitTestSudoku9x9()
    : WsjcppUnitTestBase("UnitTestSudoku9x9") {
}

// ---------------------------------------------------------------------

void UnitTestSudoku9x9::init() {
    // nothing
}

// ---------------------------------------------------------------------

bool UnitTestSudoku9x9::run() {
    bool bTestSuccess = true;
    compareB(bTestSuccess, "Not implemented", true, false);

    struct LTest {
        LTest(std::string sSrc, std::string sOut) {
            this->sSrc = sSrc;
            this->sOut = sOut;
        }
        std::string sSrc;
        std::string sOut;
    };

    std::vector<LTest> tests;

    // 17/100:
    tests.push_back(LTest(
        "--7--5--2-86-3-9---5--7------4--8---89-----75---1--3------2--6---8-6-71-3--9--8--",
        "947685132286431957153279486634758291891342675572196348719824563428563719365917824"
    ));

    // 18/100:
    tests.push_back(LTest(
        "--------2----96--47---253-6-5--74-9-----------6-13--5-2-854---76--78----9--------",
        "536417982182396574794825316851674293327958461469132758218549637645783129973261845"
    ));

    // 19/100:
    tests.push_back(LTest(
        "9---3--4-812-7----------5---5----279---5-9---491----8---4----------1-496-6--2---1",
        "975632148812475963346891527658143279237589614491267385184956732523718496769324851"
    ));

    seakgExecSudoku *sudoku = new seakgExecSudoku();
    for (int i = 0; i < tests.size(); i++) {
        LTest test = tests[i];
        
        sudoku->clearAll();
        sudoku->setAlphabet("-123456789");
        sudoku->setPole(test.sSrc);
        sudoku->coutPole();
        sudoku->setOblasty9x9();

        // while fulfilling performed
        while (sudoku->shag() == true) {
            // nothing
        };

        // sudoku->
    }
    
    return bTestSuccess;
}

