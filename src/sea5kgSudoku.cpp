#include "sea5kgSudoku.h"
#include <wsjcpp_core.h>
#include <math.h>
#include <algorithm>

//----------------------------------------------------------------------------
// sea5kgSudokuType

std::string sea5kgSudokuType::ST_NONE = "none";
std::string sea5kgSudokuType::ST_5x5 = "5x5";
std::string sea5kgSudokuType::ST_6x6 = "6x6";
std::string sea5kgSudokuType::ST_9x9 = "9x9";

//----------------------------------------------------------------------------
// sea5kgSudokuCell

sea5kgSudokuCell::sea5kgSudokuCell(int nPosX, int nPosY, char cValue) {
    m_nPosX = nPosX;
    m_nPosY = nPosY;
    m_cValue = cValue;
}

//----------------------------------------------------------------------------

void sea5kgSudokuCell::setValue(char cValue) {
    m_cValue = cValue;
}

//----------------------------------------------------------------------------

bool sea5kgSudokuCell::setRandomlyValueFromPossible() {
    if (m_cValue != '-') {
        return true;
    }
    if (m_vPossibleValues.size() == 0) {
        return false;
    }
    int nIndex = rand() % m_vPossibleValues.size();
    m_cValue = m_vPossibleValues[nIndex];
    return true;
}

//----------------------------------------------------------------------------

char sea5kgSudokuCell::getValue() {
    return m_cValue;
}

//----------------------------------------------------------------------------

const std::vector<char> &sea5kgSudokuCell::getPossibleValues() {
    return m_vPossibleValues;
}

//----------------------------------------------------------------------------

void sea5kgSudokuCell::clear() {
    m_vPossibleValues.clear();
    m_cValue = '-';
}

//----------------------------------------------------------------------------

void sea5kgSudokuCell::setPossibleValues(const std::string &sAlphabet) {
    m_vPossibleValues.clear();
    for (int i = 0; i < sAlphabet.length(); i++) {
        m_vPossibleValues.push_back(sAlphabet[i]);
    }
}

//----------------------------------------------------------------------------

void sea5kgSudokuCell::excludePossibleValue(char cValue) {
    std::vector<char>::iterator it;
    it = std::find(m_vPossibleValues.begin(), m_vPossibleValues.end(), cValue);
    if (it != m_vPossibleValues.end()) {
        m_vPossibleValues.erase(it);
    }
}

//----------------------------------------------------------------------------

std::string sea5kgSudokuCell::getOnelinePossibleValues() {
    std::string sRet = "";
    for (int i = 0; i < m_vPossibleValues.size(); i++) {
        sRet += m_vPossibleValues[i];
    }
    return sRet;
}

//----------------------------------------------------------------------------
// sea5kgSudokuRegion

sea5kgSudokuRegion::sea5kgSudokuRegion(std::vector<std::pair<int,int>> &vRegionCells) {
    m_vRegionCells = vRegionCells;
}

//----------------------------------------------------------------------------

const std::vector<std::pair<int,int>> &sea5kgSudokuRegion::getRegionCells() const {
    return m_vRegionCells;
}

//----------------------------------------------------------------------------

std::pair<int,int> sea5kgSudokuRegion::getMin() const {
    std::pair<int,int> ret = m_vRegionCells[0];
    for (int i = 0; i < m_vRegionCells.size(); i++) {
        if (m_vRegionCells[i].first < ret.first || m_vRegionCells[i].second < ret.second) {
            ret = m_vRegionCells[i];
        }
    }
    return ret;
}

//----------------------------------------------------------------------------

std::pair<int,int> sea5kgSudokuRegion::getMax() const {
    std::pair<int,int> ret = m_vRegionCells[0];
    for (int i = 0; i < m_vRegionCells.size(); i++) {
        if (m_vRegionCells[i].first > ret.first || m_vRegionCells[i].second > ret.second) {
            ret = m_vRegionCells[i];
        }
    }
    return ret;
}

//----------------------------------------------------------------------------

bool sea5kgSudokuRegion::has(int x, int y) {
    for (int i = 0; i < m_vRegionCells.size(); i++) {
        if (m_vRegionCells[i].first == x && m_vRegionCells[i].second == y) {
            return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------

std::string sea5kgSudokuRegion::getOnelineData() {
    std::string sRet;
    for (int i = 0; i < m_vRegionCells.size(); i++) {
        sRet += "{" + std::to_string(m_vRegionCells[i].first) + ","
            + std::to_string(m_vRegionCells[i].second) + "}";
    }
    return sRet;
}

// -----------------------------------------------------------------------------
// sea5kgSudoku

sea5kgSudoku::sea5kgSudoku(
    const std::string &sAlphabet,
    const std::string &sSudokuType
) {
    TAG = "sea5kgSudoku";
    int nLength = sAlphabet.length();
    
    if (nLength < 2) {
        WsjcppLog::throw_err(TAG, "Sudoku alphabet size must be more then 1");
    }

    // check reserved char
    for (int i = 0; i < nLength; i++) {
        if (sAlphabet[i] == '-') {
            WsjcppLog::throw_err(TAG, "Sudoku alphabet could not contains '-'");
        }
    }

    // check unique values
    for (int x = 0; x < nLength; x++) {
        for (int y = 0; y < nLength; y++) {
            if (x != y && sAlphabet[x] == sAlphabet[y]) {
                std::string sMessage = "Sudoku alphabet could not contains dublicates '";
                sMessage += sAlphabet[x];
                sMessage += "'";
                WsjcppLog::throw_err(TAG, sMessage);
            }
        }
    }  

    m_sAlphabet = sAlphabet;
    m_sType = sSudokuType;
    m_nLen = m_sAlphabet.length();

    for (int x = 0; x < m_nLen; x++) {
        for (int y = 0; y < m_nLen; y++) {
            m_vCells.push_back(new sea5kgSudokuCell(x, y, '-'));
        }
    };

    if (sSudokuType == sea5kgSudokuType::ST_NONE) {
        // just skip
    } else if (sSudokuType == sea5kgSudokuType::ST_5x5) {
        applyClassicRegionsFor5x5();
    } else if (sSudokuType == sea5kgSudokuType::ST_6x6) {
        applyClassicRegionsFor6x6();
    } else if (sSudokuType == sea5kgSudokuType::ST_9x9) {
        applyClassicRegionsFor9x9();
    } else {
        WsjcppLog::throw_err(TAG, "Unknown sudoku type");
    }
}

// -----------------------------------------------------------------------------

sea5kgSudoku::~sea5kgSudoku() {
    this->clearAll();
};

// -----------------------------------------------------------------------------

void sea5kgSudoku::setData(const std::string &sData) {
    int nExpectedLen = m_nLen * m_nLen;
    if (sData.length() != nExpectedLen) {
        WsjcppLog::throw_err(TAG, "Wrong size of data! Expected "
            + std::to_string(nExpectedLen) + ", but got '"
            + std::to_string(sData.length()) + "'");
    }
    for (int i = 0; i < sData.length(); i++) {
        m_vCells[i]->setValue(sData[i]);
    }
}

//-----------------------------------------------------------------------------

std::string sea5kgSudoku::getData() {
    std::string sRet = "";
    for (unsigned int i = 0; i < m_vCells.size(); i++) {
        sRet += m_vCells[i]->getValue();
    };
    return sRet;
}

//-----------------------------------------------------------------------------

void sea5kgSudoku::setEmptyData() {
    for (unsigned int i = 0; i < m_vCells.size(); i++) {
        m_vCells[i]->setValue('-');
    };
}

//-----------------------------------------------------------------------------

std::string sea5kgSudoku::getPrintableData() {

    if (m_sType == sea5kgSudokuType::ST_5x5) {
        return getPrintableDataFor5x5();
    } else if (m_sType == sea5kgSudokuType::ST_6x6) {
        return getPrintableDataFor6x6();
    } else if (m_sType == sea5kgSudokuType::ST_9x9) {
        return getPrintableDataFor9x9();
    }

    int sch = 1;
    std::string sData = " +";
    for (int i = 0; i < m_nLen; i++) {
        sData += "---+";
    }
    for (unsigned int i = 0; i < m_vCells.size(); i++) {
        char cValue = m_vCells[i]->getValue();
        if (i % m_nLen == 0)  {
            sData += "\n | ";
            sch++;
        }
        sData += cValue;
        sData += " | ";
    };
    sData += "\n +";
    for (int i = 0; i < m_nLen; i++) {
        sData += "---+";
    }
    return sData;
};

//-----------------------------------------------------------------------------

void sea5kgSudoku::clearAll() {
    //clear cells
    for (unsigned int i = 0; i < m_vCells.size(); i++) {
        m_vCells[i]->clear();
    };
    m_vRegions.clear();
};

//-----------------------------------------------------------------------------

void sea5kgSudoku::coutVariant() {
    for (unsigned int i = 0; i < m_vCells.size(); i++) {
        // m_vCells[i]->updatePossibleValues(m_sAlphabet);
        
        std::vector<char> vPossibleValues = m_vCells[i]->getPossibleValues();

        std::cout << "Cell "<< i << ": \n";
        std::cout << "\tthe number of cases = " << vPossibleValues.size() << ";\n"; 
        std::cout << "\tcases: ";
        for (unsigned int t = 0; t < vPossibleValues.size(); t++) {
            std::cout << " " << vPossibleValues[t] << ", ";
        };
        // std::cout << "\n\tNumber of areas which include cell: " << m_vCells[i]->oblasty.size() << "\n"; 
    };
};

//-----------------------------------------------------------------------------

void sea5kgSudoku::updatePossibleValues() {
    for (int x = 0; x < m_nLen; x++) {
        for (int y = 0; y < m_nLen; y++) {
            if (getCell(x,y).getValue() != '-') {
                getCell(x,y).setPossibleValues("");
                continue;
            }
            std::vector<sea5kgSudokuRegion> vFoundRegions;
            findRegions(x, y, vFoundRegions);
            getCell(x,y).setPossibleValues(m_sAlphabet);
            for (int i = 0; i < vFoundRegions.size(); i++) {
                sea5kgSudokuRegion region = vFoundRegions[i];
                for (int r = 0; r < region.getRegionCells().size(); r++) {
                    std::pair<int,int> p = region.getRegionCells()[r];
                    char c = getCell(p).getValue();
                    if (c != '-') {
                        getCell(x,y).excludePossibleValue(c);
                    }
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------

void sea5kgSudoku::findRegions(int x, int y, std::vector<sea5kgSudokuRegion> &foundRegions) {
    foundRegions.clear();
    for (int i = 0; i < m_vRegions.size(); i++) {
        sea5kgSudokuRegion region = m_vRegions[i];
        if (region.has(x,y)) {
            foundRegions.push_back(region);
        }
    }
}

//----------------------------------------------------------------------------

const std::vector<sea5kgSudokuRegion> &sea5kgSudoku::getRegions() const {
    return m_vRegions;
}

//-----------------------------------------------------------------------------

bool sea5kgSudoku::step() {
    updatePossibleValues();
    
    int nSet = 0;
    for (int x = 0; x < m_nLen; x++) {
        for (int y = 0; y < m_nLen; y++) {
            if (getCell(x,y).getValue() == '-') {
                std::vector<char> vPossibleValues = getCell(x,y).getPossibleValues();
                if (vPossibleValues.size() == 1) {
                    char cValue = getCell(x,y).getPossibleValues()[0];
                    getCell(x,y).setValue(cValue);
                    nSet++;
                } else {
                    std::vector<sea5kgSudokuRegion> vFoundRegions;
                    findRegions(x, y, vFoundRegions);
                    for (int p = 0; p < vPossibleValues.size(); p++) {
                        char cValue = vPossibleValues[p];
                        for (int r = 0; r < vFoundRegions.size(); r++) {
                            sea5kgSudokuRegion region = vFoundRegions[r];
                            if (getCountOfPossibleValuesInRegion(cValue, region) == 1) {
                                getCell(x,y).setValue(cValue);
                                nSet++;
                            }
                        }
                    }
                }
            }
        }
    }
    return nSet > 0;
};

// ----------------------------------------------------------------------------

void sea5kgSudoku::solve() {
    while (step()) {
        // next step
    }
}

// ----------------------------------------------------------------------------

bool sea5kgSudoku::generate(int nMaxTries) {

    // first step fill the sudoku
    int nTries = 0;
    while (nTries < nMaxTries) {
        nTries++;
        // TODO some how improve
        if (!tryFillCellsRandomly()) {
            std::cout << "Failed: " << std::endl << getPrintableData() << std::endl;
            setEmptyData();
            continue;
        }
        if (isSolved()) {
            break;
        }
    }
    if (!isSolved()) {
        return false;
    }
    std::cout << "Found: " << std::endl << getPrintableData() << std::endl;

    // second step erase cells from sudoku
    std::string sData = getData();

    nTries = 0;
    while (nTries < nMaxTries) {
        // std::cout << sData << std::endl;
        setData(sData);

        std::vector<std::pair<int,int>> vPositions;
        for (int x = 0; x < m_nLen; x++) {
            for (int y = 0; y < m_nLen; y++) {
                if (getCell(x,y).getValue() != '-') {
                    vPositions.push_back(std::pair<int,int>(x,y));
                }
            }
        }

        int i = rand() % vPositions.size();
        std::pair<int,int> p = vPositions[i];
        getCell(p).setValue('-');
        std::string sNewData = getData();
        solve();
        if (isSolved()) {
            sData = sNewData;
        } else {
            nTries++;
        }
    }
    setData(sData);
    return true;
}

// ----------------------------------------------------------------------------

bool sea5kgSudoku::isSolved() {
    for (int x = 0; x < m_nLen; x++) {
        for (int y = 0; y < m_nLen; y++) {
            if (getCell(x,y).getValue() == '-') {
                return false;
            }
        }
    }
    return true;
}

// ----------------------------------------------------------------------------

int sea5kgSudoku::getCountOfPossibleValuesInRegion(char cValue, const sea5kgSudokuRegion &region) {
    int nCount = 0;
    std::vector<std::pair<int,int>> vRegionCells = region.getRegionCells();
    for (int rc = 0; rc < vRegionCells.size(); rc++) {
        std::vector<char> vPossibleValues = getCell(vRegionCells[rc]).getPossibleValues();
        for (int y = 0; y < vPossibleValues.size(); y++) {
            if (vPossibleValues[y] == cValue) {
                nCount++;
            }
        }
    }
    return nCount;
}

//----------------------------------------------------------------------------

sea5kgSudokuCell &sea5kgSudoku::getCell(int x, int y) {
    return *m_vCells[x + y * m_nLen];
}

//----------------------------------------------------------------------------

sea5kgSudokuCell &sea5kgSudoku::getCell(const std::pair<int,int> &p) {
    return *m_vCells[p.first + p.second * m_nLen];
}

// ----------------------------------------------------------------------------

void sea5kgSudoku::applyClassicRegionsFor5x5() {
    if (m_sAlphabet.length() != 5) {
        WsjcppLog::throw_err(TAG, "Alphabet must have size 5");
    }
    /*
    +-----------+-------+
    | -   -   2 | -   - |
    |       +---+       |
    | -   4 | - | -   - |
    +---+---+   +---+   |
    | 1 | -   4   - | - |
    |   +---+   +---+---+
    | 5   - | - | -   2 |
    |       +---+       |
    | -   2 | -   -   3 |
    +---+---+---+---+---+
    */
    m_vRegions.clear();
    addRegionsRowsAndColumns();
    std::vector<std::pair<int,int>> vRegion1;
    vRegion1.push_back(std::pair<int,int>(0,0));
    vRegion1.push_back(std::pair<int,int>(1,0));
    vRegion1.push_back(std::pair<int,int>(2,0));
    vRegion1.push_back(std::pair<int,int>(0,1));
    vRegion1.push_back(std::pair<int,int>(1,1));
    m_vRegions.push_back(sea5kgSudokuRegion(vRegion1));

    std::vector<std::pair<int,int>> vRegion2;
    vRegion2.push_back(std::pair<int,int>(3,0));
    vRegion2.push_back(std::pair<int,int>(4,0));
    vRegion2.push_back(std::pair<int,int>(3,1));
    vRegion2.push_back(std::pair<int,int>(4,1));
    vRegion2.push_back(std::pair<int,int>(4,2));
    m_vRegions.push_back(sea5kgSudokuRegion(vRegion2));

    std::vector<std::pair<int,int>> vRegion3;
    vRegion3.push_back(std::pair<int,int>(0,2));
    vRegion3.push_back(std::pair<int,int>(0,3));
    vRegion3.push_back(std::pair<int,int>(1,3));
    vRegion3.push_back(std::pair<int,int>(0,4));
    vRegion3.push_back(std::pair<int,int>(1,4));
    m_vRegions.push_back(sea5kgSudokuRegion(vRegion3));

    std::vector<std::pair<int,int>> vRegion4;
    vRegion4.push_back(std::pair<int,int>(3,3));
    vRegion4.push_back(std::pair<int,int>(4,3));
    vRegion4.push_back(std::pair<int,int>(2,4));
    vRegion4.push_back(std::pair<int,int>(3,4));
    vRegion4.push_back(std::pair<int,int>(4,4));
    m_vRegions.push_back(sea5kgSudokuRegion(vRegion4));

    std::vector<std::pair<int,int>> vRegion5;
    vRegion5.push_back(std::pair<int,int>(2,1));
    vRegion5.push_back(std::pair<int,int>(1,2));
    vRegion5.push_back(std::pair<int,int>(2,2));
    vRegion5.push_back(std::pair<int,int>(3,2));
    vRegion5.push_back(std::pair<int,int>(2,3));
    m_vRegions.push_back(sea5kgSudokuRegion(vRegion5));
}

// ----------------------------------------------------------------------------

void sea5kgSudoku::applyClassicRegionsFor6x6() {
    if (m_sAlphabet.length() != 6) {
        WsjcppLog::throw_err(TAG, "Alphabet must have size 6");
    }

    m_vRegions.clear();
    addRegionsRowsAndColumns();

    // add 6 other regions
    for (int x = 0; x < 2; x++) {
        for (int y = 0; y < 3; y++) {
            std::vector<std::pair<int,int>> vCells;
            int x0 = x*3;
            int y0 = y*2;
            vCells.push_back(std::pair<int,int>(x0,y0));
            vCells.push_back(std::pair<int,int>(x0,y0+1));
            vCells.push_back(std::pair<int,int>(x0+1,y0));
            vCells.push_back(std::pair<int,int>(x0+1,y0+1));
            vCells.push_back(std::pair<int,int>(x0+2,y0+0));
            vCells.push_back(std::pair<int,int>(x0+2,y0+1));
            m_vRegions.push_back(sea5kgSudokuRegion(vCells));
        }
    }
}

//----------------------------------------------------------------------------

void sea5kgSudoku::applyClassicRegionsFor9x9() {
    if (m_sAlphabet.length() != 9) {
        WsjcppLog::throw_err(TAG, "Alphabet must have size 9");
    }
    m_vRegions.clear();
    addRegionsRowsAndColumns();

    // add 9 squares
    for (int x = 0; x < 3; x++) {
        for (int y = 0; y < 3; y++) {
            std::vector<std::pair<int,int>> vCells;
            int x0 = x*3;
            int y0 = y*3;
            vCells.push_back(std::pair<int,int>(x0,y0));
            vCells.push_back(std::pair<int,int>(x0,y0+1));
            vCells.push_back(std::pair<int,int>(x0,y0+2));
            vCells.push_back(std::pair<int,int>(x0+1,y0));
            vCells.push_back(std::pair<int,int>(x0+1,y0+1));
            vCells.push_back(std::pair<int,int>(x0+1,y0+2));
            vCells.push_back(std::pair<int,int>(x0+2,y0));
            vCells.push_back(std::pair<int,int>(x0+2,y0+1));
            vCells.push_back(std::pair<int,int>(x0+2,y0+2));
            m_vRegions.push_back(sea5kgSudokuRegion(vCells));
        }
    }
}

//----------------------------------------------------------------------------

void sea5kgSudoku::addRegionsRowsAndColumns() {
    int nLen = m_sAlphabet.length();
    // rows && columns
    for (int x = 0; x < nLen; x++) {
        std::vector<std::pair<int,int>> vRowCells;
        std::vector<std::pair<int,int>> vColumnCells;
        for( int y = 0; y < nLen; y++ ) {
            vRowCells.push_back(std::pair<int,int>(x,y));
            vColumnCells.push_back(std::pair<int,int>(y,x));
        }
        m_vRegions.push_back(sea5kgSudokuRegion(vRowCells));
        m_vRegions.push_back(sea5kgSudokuRegion(vColumnCells));
    }
}

//----------------------------------------------------------------------------

bool sea5kgSudoku::tryFillCellsRandomly() {
    int y = 0;
    int nTries = 0;
    while (y < m_nLen) {
        if (!tryFillRowRandomly(y)) {
            bool bLastResult = false;
            for (int i = y; i >= 0; i--) {
                for (int y0 = y; y0 <= y; y0++) {
                    clearRow(y0);
                }
                for (int y0 = i; y0 <= y; y0++) {
                    bLastResult = tryFillRowRandomly(y0);
                    if (!bLastResult) {
                        break;
                    }
                }
            }
            if (!bLastResult) {
                return false;
            }
        }
        y++;
        continue;
    }
    return true;
}

//----------------------------------------------------------------------------

void sea5kgSudoku::clearRow(int y) {
    for (int x = 0; x < m_nLen; x++) {
        getCell(x,y).setValue('-');
    }
}

//----------------------------------------------------------------------------

bool sea5kgSudoku::tryFillRowRandomly(int y) {
    updatePossibleValues();
    for (int x = 0; x < m_nLen; x++) {
        if (getCell(x, y).getValue() == '-') {
            if (!getCell(x, y).setRandomlyValueFromPossible()) {
                return false;
            }
            updatePossibleValues();
        }
    }
    return true;
}

//----------------------------------------------------------------------------

std::string sea5kgSudoku::getPrintableDataFor5x5() {
    std::string sRet = "+-----------+-------+\n";
    sRet += "| "; 
    sRet += getCell(0,0).getValue();
    sRet += "   "; 
    sRet += getCell(1,0).getValue();
    sRet += "   "; 
    sRet += getCell(2,0).getValue();
    sRet += " | "; 
    sRet += getCell(3,0).getValue();
    sRet += "   "; 
    sRet += getCell(4,0).getValue();
    sRet += " |\n";
    sRet += "|       +---+       |\n";
    sRet += "| "; 
    sRet += getCell(0,1).getValue();
    sRet += "   "; 
    sRet += getCell(1,1).getValue();
    sRet += " | "; 
    sRet += getCell(2,1).getValue();
    sRet += " | "; 
    sRet += getCell(3,1).getValue();
    sRet += "   "; 
    sRet += getCell(4,1).getValue();
    sRet += " |\n"; 
    sRet += "+---+---+   +---+   |\n";
    sRet += "| "; 
    sRet += getCell(0,2).getValue();
    sRet += " | "; 
    sRet += getCell(1,2).getValue();
    sRet += "   "; 
    sRet += getCell(2,2).getValue();
    sRet += "   "; 
    sRet += getCell(3,2).getValue();
    sRet += " | "; 
    sRet += getCell(4,2).getValue();
    sRet += " |\n"; 
    sRet += "|   +---+   +---+---+\n";
    sRet += "| "; 
    sRet += getCell(0,3).getValue();
    sRet += "   "; 
    sRet += getCell(1,3).getValue();
    sRet += " | "; 
    sRet += getCell(2,3).getValue();
    sRet += " | "; 
    sRet += getCell(3,3).getValue();
    sRet += "   "; 
    sRet += getCell(4,3).getValue();
    sRet += " |\n";
    sRet += "|       +---+       |\n"; 
    sRet += "| "; 
    sRet += getCell(0,4).getValue();
    sRet += "   "; 
    sRet += getCell(1,4).getValue();
    sRet += " | "; 
    sRet += getCell(2,4).getValue();
    sRet += "   "; 
    sRet += getCell(3,4).getValue();
    sRet += "   ";
    sRet += getCell(4,4).getValue();
    sRet += " |\n"; 
    sRet += "+---+---+---+---+---+";
    return sRet;
}

//----------------------------------------------------------------------------

std::string sea5kgSudoku::getPrintableDataFor6x6() {
    std::string sRet = "";
    for (int y = 0; y < 6; y++) {
        if (y % 2 == 0) {
            sRet += "+---+---+---++---+---+---+\n";
        }
        for (int x = 0; x < 6; x++) {
            if (x > 0 && x % 3 == 0) sRet += "|";
            sRet += "| ";
            sRet += getCell(x,y).getValue();
            sRet += " ";
        }
        sRet += "|\n";
    }
    sRet += "+---+---+---++---+---+---+";
    return sRet;
}

//----------------------------------------------------------------------------

std::string sea5kgSudoku::getPrintableDataFor9x9() {
    std::string sRet = "";
    for (int y = 0; y < 9; y++) {
        if (y % 3 == 0) {
            sRet += "+---+---+---++---+---+---++---+---+---+\n";
        }
        for (int x = 0; x < 9; x++) {
            if (x > 0 && x % 3 == 0) sRet += "|";
            sRet += "| ";
            sRet += getCell(x,y).getValue();
            sRet += " ";
        }
        sRet += "|\n";
    }
    sRet += "+---+---+---++---+---+---++---+---+---+";
    return sRet;
}