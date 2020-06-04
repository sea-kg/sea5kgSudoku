#include "sea5kgSudoku.h"
#include <wsjcpp_core.h>
#include <math.h>
#include <algorithm>

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
    m_vPossibleValues.clear();
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

sea5kgSudoku::sea5kgSudoku(const std::string &sAlphabet) {
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
    m_nLen = m_sAlphabet.length();

    for (int x = 0; x < m_nLen; x++) {
        for (int y = 0; y < m_nLen; y++) {
            m_vCells.push_back(new sea5kgSudokuCell(x,y,'-'));
        }
    };

    // prepare fields
}

//-----------------------------------------------------------------------------

sea5kgSudoku::~sea5kgSudoku() {
    this->clearAll();
};

//-----------------------------------------------------------------------------

void sea5kgSudoku::setData(const std::string &sPole) {
    int nLen = m_sAlphabet.length();
    if (sPole.length() != (nLen*nLen)) {
        WsjcppLog::throw_err(TAG, "Wrong size of data");
    }

    // m_vCells.clear();
    for (int x = 0; x < nLen; x++) {
        for (int y = 0; y < nLen; y++) {
            int i = x + y*nLen;
            m_vCells[i]->setValue(sPole[i]);
        }
    }
}

//-----------------------------------------------------------------------------

std::string sea5kgSudoku::printData() {
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

std::string sea5kgSudoku::getOnelineData() {
    std::string sRet = "";
    for (unsigned int i = 0; i < m_vCells.size(); i++) {
        sRet += m_vCells[i]->getValue();
    };
    return sRet;
}

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

void sea5kgSudoku::applyClassicRegionsFor9x9() {
    if (m_sAlphabet.length() != 9) {
        WsjcppLog::throw_err(TAG, "Alphabet must have size 9");
    }
    // depending fill
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
                    if (x == 2 && y == 2) {
                        std::cout << "Possible values 0,0: " << getCell(0,0).getOnelinePossibleValues() << std::endl;
                        std::cout << "Possible values 0,1: " << getCell(0,1).getOnelinePossibleValues() << std::endl;
                        std::cout << "Possible values 0,2: " << getCell(0,2).getOnelinePossibleValues() << std::endl;
                        std::cout << "Possible values 1,0: " << getCell(1,0).getOnelinePossibleValues() << std::endl;
                        std::cout << "Possible values 2,2: " << getCell(2,2).getOnelinePossibleValues() << std::endl;
                        std::cout << "Value 2,2: " << getCell(2,2).getValue() << std::endl;

                        for (int r = 0; r < vFoundRegions.size(); r++) {
                            std::cout << vFoundRegions[r].getOnelineData() << std::endl;
                        }
                    }

                    for (int p = 0; p < vPossibleValues.size(); p++) {
                        char cValue = vPossibleValues[p];
                        for (int r = 0; r < vFoundRegions.size(); r++) {
                            sea5kgSudokuRegion region = vFoundRegions[r];
                            if (x == 2 && y == 2) {
                                std::cout << ">>>> Found value! " << cValue << std::endl;
                                std::cout << "Found vFoundRegions[r].getOnelineData()! " << vFoundRegions[r].getOnelineData() << std::endl;
                            }
                            if (getCountOfPossibleValuesInRegion(cValue, region) == 1) {
                                getCell(x,y).setValue(cValue);
                                nSet++;
                            }
                            if (x == 2 && y == 2) {
                                std::cout << "<<< Found value! " << cValue << std::endl;
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

void sea5kgSudoku::applyClassicRegionsFor6x6() {
    if (m_sAlphabet.length() != 6) {
        WsjcppLog::throw_err(TAG, "Alphabet must have size 6");
    }

    m_vRegions.clear();
    addRegionsRowsAndColumns();

    // TODO add 6 rectangules
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