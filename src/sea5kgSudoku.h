#ifndef SEA5KG_SUDOKU_H
#define SEA5KG_SUDOKU_H

#include <vector>
#include <string>

// 2011-04-22 sea-kg
// 2020-05-10 sea-kg

class sea5kgSudokuType {
    public:
        static std::string SEA5KG_SUDOKU_NONE;
        static std::string SEA5KG_SUDOKU_6x6;
        static std::string SEA5KG_SUDOKU_9x9;
};

//----------------------------------------------------------------------------

class sea5kgSudokuCell {
    public:
        sea5kgSudokuCell(int nPosX, int nPosY, char cValue);
        void setValue(char cValue);
        char getValue();
        const std::vector<char> &getPossibleValues();
        void clear();
        void setPossibleValues(const std::string &sAlphabet);
        void excludePossibleValue(char cValue);
        std::string getOnelinePossibleValues();

    private:
        int m_nPosX;
        int m_nPosY;
        char m_cValue;
        std::vector<char> m_vPossibleValues;
};

//----------------------------------------------------------------------------

class sea5kgSudokuRegion {
    public:
        sea5kgSudokuRegion(std::vector<std::pair<int,int>> &vRegionCells);
        const std::vector<std::pair<int,int>> &getRegionCells() const;
        std::pair<int,int> getMin() const;
        std::pair<int,int> getMax() const;
        bool has(int x, int y);
        std::string getOnelineData();

    private:
        std::vector<std::pair<int,int>> m_vRegionCells;
};

//----------------------------------------------------------------------------

class sea5kgSudoku
{
    public:
        sea5kgSudoku(
            const std::string &sAlphabet,
            const std::string &sSudokuType
        );

        ~sea5kgSudoku();
        void setData(const std::string &sPole);

        std::string printData();
        std::string getOnelineData();
        void coutVariant();
        void clearAll();

        const std::vector<sea5kgSudokuRegion> &getRegions() const;

        bool step();
        void solve();
        
        int getCountOfPossibleValuesInRegion(char cValue, const sea5kgSudokuRegion &region);
        sea5kgSudokuCell &getCell(int x, int y);
        sea5kgSudokuCell &getCell(const std::pair<int,int> &p);
        void updatePossibleValues();
        void findRegions(int x, int y, std::vector<sea5kgSudokuRegion> &foundRegions);

    private:
        std::string TAG;

        void applyClassicRegionsFor6x6();
        void applyClassicRegionsFor9x9();
        void addRegionsRowsAndColumns();

        std::vector<sea5kgSudokuCell *> m_vCells;
        std::vector<sea5kgSudokuRegion> m_vRegions;
        
        std::string m_sAlphabet;
        int m_nLen;
};

//----------------------------------------------------------------------------

#endif // SEA5KG_SUDOKU_H
