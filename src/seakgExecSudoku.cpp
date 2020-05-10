#include "seakgExecSudoku.h"

//-----------------------------------------------------------------------------

seakgExecSudoku::seakgExecSudoku() {
	pole.clear();
	return;
};

//-----------------------------------------------------------------------------

seakgExecSudoku::~seakgExecSudoku() {
	this->clearAll();
};

//-----------------------------------------------------------------------------

void seakgExecSudoku::setAlphabet(std::string sAlphabet) {
	alphabet = sAlphabet;
};

//-----------------------------------------------------------------------------

int seakgExecSudoku::getCharToIntAlphabet( char ch ) {
	for (unsigned int i = 0; i < alphabet.length(); i++) {
		if( ch == alphabet[i] ) return i;
	}
	return 0;
};

//-----------------------------------------------------------------------------

char seakgExecSudoku::getIntToCharAlphabet( int i ) {
	i = i % alphabet.length();
	return alphabet[i];
};

//-----------------------------------------------------------------------------

void seakgExecSudoku::setPole(std::string Pole) {
	pole.clear();
	
	for (unsigned int i = 0; i < Pole.length(); i++) {
		int ch = getCharToIntAlphabet( Pole[i] );
		seakgExecSudokuKletka *kl = new seakgExecSudokuKletka();
		kl->value = ch;
		kl->sudoku = this;
		pole.push_back( kl );
	};
	return;
};

//-----------------------------------------------------------------------------

void seakgExecSudoku::coutPole() {
	int lenstr = sqrt( pole.size() );
	
	int sch = 1;
	std::cout << "\n" << sch << ") ";
	
	for(unsigned int i = 0; i < pole.size(); i++ )
	{
		int value = pole[i]->value;
		
		std::cout << " | "<< getIntToCharAlphabet( value );
		if( (int)(i % lenstr) == ( lenstr - 1 ) ) 
		{
			std::cout << " |";
			sch++;
			if( i != pole.size() - 1 )
			std::cout << "\n" << sch << ") ";
		}
	};
	std::cout << "\n\n";
	return;
};

//-----------------------------------------------------------------------------

void seakgExecSudoku::coutPoleSimple() {
	for (unsigned int i = 0; i < pole.size(); i++) {
		int value = pole[i]->value;
		std::cout << getIntToCharAlphabet( value );
	};
	std::cout << "\n";
	return;
};

//-----------------------------------------------------------------------------

void seakgExecSudoku::clearAll() {
	//clear cells
	for (unsigned int i = 0; i < pole.size(); i++) {
		pole[i]->variants.clear();
		pole[i]->oblasty.clear();
	};
	pole.clear();
	
	//clear areas
	for (unsigned int i = 0; i < oblasty.size(); i++) {
		oblasty[i]->kletki.clear();
	};
	oblasty.clear();
};

//-----------------------------------------------------------------------------

void seakgExecSudoku::coutVariant() {
	for (unsigned int i = 0; i < pole.size(); i++) {
		pole[i]->getVariants();
		
		std::cout << "Cell "<< i << ": \n";
		std::cout << "\tthe number of cases = " << pole[i]->variants.size() << ";\n"; 
		std::cout << "\tcases: ";
		for (unsigned int t = 0; t < pole[i]->variants.size(); t++) {
			std::cout << " " << pole[i]->variants[t] << ", ";
		};
		std::cout << "\n\tNumber of areas which include cell: " << pole[i]->oblasty.size() << "\n"; 
	};
};

//-----------------------------------------------------------------------------

void seakgExecSudoku::addOblasty( int mas[], int count )
{
	seakgExecSudokuOblast * obl = new seakgExecSudokuOblast();
	obl->kletki.clear();
	obl->sudoku = this;
	//cout << "oblast: ";
	for(int i = 0; i < count; i++ )
	{
		seakgExecSudokuKletka *kl = pole[ mas[i] ];
		obl->kletki.push_back( kl );
		kl->oblasty.push_back( obl );
		//cout << mas[i] << ", ";
	};
	oblasty.push_back(obl);
	//cout << "\n";
	return;
};
//-----------------------------------------------------------------------------
int seakgExecSudoku::getCountVariants()
{
	int len = 0;
	if( pole.size() > 0 ) len = sqrt( pole.size() );
	return len;
};
//-----------------------------------------------------------------------------
int seakgExecSudoku::getValueKletka(int ind)
{
	return pole[ind]->value;
};
//-----------------------------------------------------------------------------
int seakgExecSudoku::findKletka( seakgExecSudokuKletka * kl )
{
	for( unsigned int i = 0; i < pole.size(); i++ )
	{
		if( kl == pole[i] ) return i;
	};
	
	return -1; 
};
//-----------------------------------------------------------------------------
bool seakgExecSudoku::shag()
{
	for( unsigned int i = 0; i < pole.size(); i++ )
	{
		pole[i]->getVariants();
	};
	
	for( unsigned int i = 0; i < oblasty.size(); i++ )
	{
		int max = this->getCountVariants() + 1;
		for( int x = 1; x < max; x++ )
		{
			int ind_kl = -1;
			int sch = oblasty[i]->schValueVariants( x, ind_kl );
			
			if( sch == 1 )
			{
				pole[ind_kl]->value = x;
				//cout << "kletka " << ind_kl << " set new value " << x << "; \n";
				return true;
			};
		};
		
	};
	return false;
};
//----------------------------------------------------------------------------
void seakgExecSudoku::setOblasty9x9()
{
	//depending fill
	int obla[9];
	for( int i = 0; i < 9; i++ )
	{
		//rows
		for(int x = 0; x < 9; x++)
		{
			obla[x] = i*9 + x;
		};
		//cout << "line: ";
		this->addOblasty( obla , 9 );
		
		//cols
		int sch = 0;
		for(int x = 0; x < 9; x++)
		{
			obla[x] = i + sch;
			sch += 9;
		};
		//cout << "Col: ";
		this->addOblasty( obla , 9 );
		
		
		if( i % 3 == 0 )
		{
			int sch3 = 0;
			for(int x1 = 0; x1 < 3; x1++ )
			{
				sch = 0;
				int sch2 = 0;
				for(int y = 0; y < 3; y++ )
				{
					for( int x = 0; x < 3 ; x++ ) obla[ x + sch2 ] = i + sch3 + sch + x;
					sch += 9;
					sch2 += 3;
				};
				//cout << "square: ";
				this->addOblasty( obla , 9 );
				sch3 += 27;
			};
		};
	};
}
//----------------------------------------------------------------------------
void seakgExecSudokuKletka::getVariants()
{
	variants.clear();
	if( value != 0 ) return;
	int max = (sudoku->getCountVariants() + 1);
	for( int value1 = 1; value1 < max ; value1++)
	{
		int sch = 0;
		for( unsigned int i = 0; i < oblasty.size(); i++)
		{
			if( oblasty[i]->findValue( value1 ) == true )
			{
				sch++;
			};
		};
		if( sch == 0 ) variants.push_back( value1 );
	};
};
//----------------------------------------------------------------------------
bool seakgExecSudokuKletka::trueVariant( int true_value )
{
	// if the cell has a specific value that is
	// the desired option can not be
	if( value != 0 ) return false;
	
	// look for this case
	for( unsigned int i = 0; i < variants.size(); i++)
	{
		if( variants[i] == true_value ) return true;
	};
	
	//have not found anything
	return false;
};
//----------------------------------------------------------------------------
bool seakgExecSudokuOblast::findValue( int value )
{
	for( unsigned int i = 0; i < kletki.size(); i++)
	{
		if( kletki[i]->value == value )
		{
			return true; 
		};
	};
	return false;
};
//----------------------------------------------------------------------------
int seakgExecSudokuOblast::schValueVariants( int value , int &ind_kl)
{
	int sch = 0;
	for( unsigned int i = 0; i < kletki.size(); i++)
	{
		if( kletki[i]->trueVariant( value ) == true )
		{
			sch++; 
			seakgExecSudokuKletka * kl = kletki[i];
			ind_kl = sudoku->findKletka(kl); 
		};
	};
	return sch;
};
//----------------------------------------------------------------------------
