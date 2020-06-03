#include "sea5kgSudoku.h"
#include <wsjcpp_core.h>
#include <math.h>

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

void sea5kgSudokuCell::updatePossibleValues(const std::string &sAlphabet) {
    // update variants

    m_vPossibleValues.clear();

    if (m_cValue != '-') return;
    int max = sAlphabet.length() + 1;
    for( int value1 = 1; value1 < max ; value1++) {
        int sch = 0;
        for( unsigned int i = 0; i < oblasty.size(); i++) {
            if( oblasty[i]->findValue( value1 ) == true ) {
                sch++;
            };
        };
        if( sch == 0 ) m_vPossibleValues.push_back( value1 );
    };
};

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

sea5kgSudokuRegion::sea5kgSudokuRegion() {
    // TODO remove
}

sea5kgSudokuRegion::sea5kgSudokuRegion(std::vector<std::pair<int,int>> &vCells) {
    m_vCells = vCells;
}

//----------------------------------------------------------------------------

bool sea5kgSudokuRegion::findValue(char cValue) {
    for (unsigned int i = 0; i < kletki.size(); i++) {
        if (kletki[i]->getValue() == cValue) {
            return true; 
        };
    };
    return false;
};

//----------------------------------------------------------------------------

int sea5kgSudokuRegion::schValueVariants( int value , int &ind_kl)
{
    int sch = 0;
    for( unsigned int i = 0; i < kletki.size(); i++)
    {
        if( kletki[i]->trueVariant( value ) == true )
        {
            sch++; 
            sea5kgSudokuCell * kl = kletki[i];
            ind_kl = sudoku->findKletka(kl); 
        };
    };
    return sch;
};

//----------------------------------------------------------------------------

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
    int nLenCells = nLength * nLength;

    for (int x = 0; x < nLength; x++) {
        for (int y = 0; y < nLength; y++) {
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
        if( ch == m_sAlphabet[i] ) return i;
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

void sea5kgSudoku::printData() {
    int sch = 1;
    int nLength = m_sAlphabet.length();
    for (unsigned int i = 0; i < m_vCells.size(); i++) {
        char cValue = m_vCells[i]->getValue();
        if (i % nLength == 0)  {
            std::cout << "\n" << sch << ") | ";
            sch++;
        }
        std::cout << cValue << " | ";
    };
    std::cout << "\n\n";
    return;
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
        m_vCells[i]->oblasty.clear();
    };
    m_vCells.clear();
    m_vRegions.clear();
};

//-----------------------------------------------------------------------------

void sea5kgSudoku::coutVariant() {
    for (unsigned int i = 0; i < m_vCells.size(); i++) {
        m_vCells[i]->updatePossibleValues(m_sAlphabet);
        
        std::vector<char> vPossibleValues = m_vCells[i]->getPossibleValues();

        std::cout << "Cell "<< i << ": \n";
        std::cout << "\tthe number of cases = " << vPossibleValues.size() << ";\n"; 
        std::cout << "\tcases: ";
        for (unsigned int t = 0; t < vPossibleValues.size(); t++) {
            std::cout << " " << vPossibleValues[t] << ", ";
        };
        std::cout << "\n\tNumber of areas which include cell: " << m_vCells[i]->oblasty.size() << "\n"; 
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

bool sea5kgSudoku::step() {
    for( unsigned int i = 0; i < m_vCells.size(); i++ ) {
        if (m_vCells[i]->getValue() != '-') {
            // TODO check the Region and apply variants
            m_vCells[i]->updatePossibleValues(m_sAlphabet);
        }
    }
    
    for (unsigned int i = 0; i < m_vRegions.size(); i++) {
        int max = this->m_sAlphabet.length() + 1;
        for( int x = 1; x < max; x++ )
        {
            int ind_kl = -1;
            int sch = m_vRegions[i].schValueVariants( x, ind_kl );
            
            if( sch == 1 )
            {
                m_vCells[ind_kl]->setValue(x);
                //cout << "kletka " << ind_kl << " set new value " << x << "; \n";
                return true;
            };
        };
        
    };
    return false;
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
