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
}

//----------------------------------------------------------------------------

void sea5kgSudokuCell::setPossibleValues(const std::string &sAlphabet) {
    m_vPossibleValues.clear();
    for (int i = 0; i < sAlphabet.length(); i++) {
        m_vPossibleValues.push_back(sAlphabet[i]);
    }
}

//----------------------------------------------------------------------------

void sea5kgSudokuCell::excludeValue(char cValue) {
    std::vector<char>::iterator it;
    it = std::find(m_vPossibleValues.begin(), m_vPossibleValues.end(), cValue);
    if (it != m_vPossibleValues.end()) {
        m_vPossibleValues.erase(it);
    }
}

//----------------------------------------------------------------------------

bool sea5kgSudokuCell::trueVariant( int true_value ) {
    // if the cell has a specific value that is
    // the desired option can not be
    if (m_cValue != '-') {
        return false;
    }
    
    // look for this case
    for (unsigned int i = 0; i < m_vPossibleValues.size(); i++) {
        if (m_vPossibleValues[i] == true_value) {
            return true;
        }
    }
    
    //have not found anything
    return false;
};

//----------------------------------------------------------------------------
// sea5kgSudokuRegion

sea5kgSudokuRegion::sea5kgSudokuRegion(std::vector<std::pair<int,int>> &vRegionCells) {
    m_vRegionCells = vRegionCells;
}

//----------------------------------------------------------------------------

const std::vector<std::pair<int,int>> &sea5kgSudokuRegion::getRegionCells() {
    return m_vRegionCells;
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

int sea5kgSudoku::getCharToIntAlphabet( char ch ) {
    for (unsigned int i = 0; i < m_sAlphabet.length(); i++) {
        if (ch == m_sAlphabet[i]) return i;
    }
    return 0;
};

//-----------------------------------------------------------------------------

char sea5kgSudoku::getIntToCharAlphabet(int i) {
    i = i % m_sAlphabet.length();
    return m_sAlphabet[i];
}

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

void sea5kgSudoku::coutPoleSimple() {
    for (unsigned int i = 0; i < m_vCells.size(); i++) {
        std::cout << m_vCells[i]->getValue();
    };
    std::cout << "\n";
    return;
};

//-----------------------------------------------------------------------------

void sea5kgSudoku::clearAll() {
    //clear cells
    for (unsigned int i = 0; i < m_vCells.size(); i++) {
        m_vCells[i]->clear();
    };
    m_vCells.clear();
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

int sea5kgSudoku::findKletka( sea5kgSudokuCell * kl ) {
    for( unsigned int i = 0; i < m_vCells.size(); i++ )
    {
        if( kl == m_vCells[i] ) return i;
    };
    
    return -1; 
};

//-----------------------------------------------------------------------------

void sea5kgSudoku::updatePossibleValues(int x, int y) {
    int i = x + y * m_nLen;
    sea5kgSudokuCell *pCell = m_vCells[i];
    std::vector<sea5kgSudokuRegion> vFoundRegions;
    findRegions(x, y, vFoundRegions);
    pCell->setPossibleValues(m_sAlphabet);
    for (int i = 0; i < vFoundRegions.size(); i++) {
        sea5kgSudokuRegion region = vFoundRegions[i];
        for (int r = 0; r < region.getRegionCells().size(); r++) {
            std::pair<int,int> p = region.getRegionCells()[r];
            char c = m_vCells[p.first + p.second * m_nLen]->getValue();
            pCell->excludeValue(c);
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

//-----------------------------------------------------------------------------

bool sea5kgSudoku::step() {
    for (int x = 0; x < m_nLen; x++) {
        for (int y = 0; y < m_nLen; y++) {
            updatePossibleValues(x, y);
        }
    }
    int nSet = 0;
    for (int x = 0; x < m_nLen; x++) {
        for (int y = 0; y < m_nLen; y++) {
            int i = x + y * m_nLen;
            if (m_vCells[i]->getValue() == '-') {
                if (m_vCells[i]->getPossibleValues().size() == 1) {
                    m_vCells[i]->setValue(m_vCells[i]->getPossibleValues()[0]);
                    std::cout << m_vCells[i]->getPossibleValues()[0] << std::endl;
                    nSet++;
                }
            }
        }
    }
    return nSet > 0;
};

//----------------------------------------------------------------------------

void sea5kgSudoku::applyClassicRegionsFor6x6() {
    if (m_sAlphabet.length() != 6) {
        WsjcppLog::throw_err(TAG, "Alphabet must have size 6");
    }

    m_vRegions.clear();
    addRegionsRowsAndColumns();

    // TODO add 6 rectangules
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
            vCells.push_back(std::pair<int,int>(x0+1,y0));
            vCells.push_back(std::pair<int,int>(x0+2,y0));
            vCells.push_back(std::pair<int,int>(x0,y0+1));
            vCells.push_back(std::pair<int,int>(x0+1,y0+1));
            vCells.push_back(std::pair<int,int>(x0+2,y0+1));
            vCells.push_back(std::pair<int,int>(x0,y0+2));
            vCells.push_back(std::pair<int,int>(x0+1,y0+2));
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
