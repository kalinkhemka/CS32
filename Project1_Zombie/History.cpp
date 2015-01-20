#include <iostream>
#include "History.h"
#include "Arena.h"
#include "globals.h"
using namespace std;

History::History (int nRows, int nCols)
{
	m_rows = nRows;
	m_cols = nCols;
	for (int r = 1; r <= m_rows; r++)
        for (int c = 1; c <= m_cols; c++)
			m_grid[r-1][c-1] = 0;
}

//The record function is to be called to notify the History object that a zombie has died at a grid point. 
//The function returns false if row r, column c is not within bounds; otherwise, it returns true after recording 
//what it needs to. This function expects its parameters to be expressed in the same coordinate system as the 
//arena (e.g., row 1, column 1 is the upper-left-most position).

bool History::record(int r, int c)
{
	if ( !(r>=1) || (r>m_rows))
		return false;
	if ( !(c>=1) || (r>m_cols))
		return false;
	m_grid[r-1][c-1]++;
	return true;	
}

//The display function clears the screen and displays the history grid as the posted programs do. 
//This function does clear the screen and write an empty line after the history grid; it does 
//not print the Press enter to continue. after the display.
void  History::display() const
{
	clearScreen();
	for (int r = 1; r <= m_rows; r++)
    {
        for (int c = 1; c <= m_cols; c++)
			if (m_grid[r-1][c-1] > 0)
			{
				if (m_grid[r-1][c-1] <= 26)
					cout << static_cast<char>('A'+(m_grid[r-1][c-1]-1));
				else
					cout << 'Z';
			} else
				cout << '.';
        cout << endl;
    }

}

