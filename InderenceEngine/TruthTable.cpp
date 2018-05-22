#include "TruthTable.h"

#include <iostream>

using std::cout;
using std::endl;

TruthTable::TruthTable()
{
	fCharDisplay = true;
}

TruthTable::TruthTable(bool aFlag)
{
	fCharDisplay = aFlag;
}

const TruthColoumn & TruthTable::getColoumn(size_t aIndex)
{
	try
	{
		return fTable[aIndex];
	}
	catch (const std::range_error& e)
	{
		throw e;
	}
}

const TruthColoumn & TruthTable::getColoumn(string aTitle)
{
	for (int i = 0; i < fTable.size(); i++)
		if (fTable[i].areYou(aTitle))
			return fTable[i];
	throw std::runtime_error("Truth table does not have the coloumn: " + aTitle);
}

bool TruthTable::getTruth(string aTitle, size_t aIndex)
{
	try
	{
		return getColoumn(aTitle).getTruth(aIndex);
	}
	catch (const std::range_error& e)
	{
		throw e;
	}
	catch (const std::runtime_error& e)
	{
		throw e;
	}
}

bool TruthTable::hasColoumn(string aTitle)
{
	try
	{
		getColoumn(aTitle);
		return true;
	}
	catch (const std::runtime_error& e)
	{
		return false;
	}
}

void TruthTable::addColoumn(const TruthColoumn & aColoumn)
{
	fTable.add(aColoumn);
}

void TruthTable::print() const
{
	HANDLE lConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO lConsoleInfo;
	WORD lConsoleColor;

	GetConsoleScreenBufferInfo(lConsole, &lConsoleInfo);
	lConsoleColor = lConsoleInfo.wAttributes;

	
	if (!fTable.empty())
	{
		// Print Headers
		List<size_t> lLenghts;
		size_t lSize = fTable[0].size();
		bool lFirst = true;

		SetConsoleTextAttribute(lConsole, 14);
		for (int i = 0; i < fTable.size(); i++)
		{
			if (!lFirst)
				cout << " | ";
			else
				lFirst = false;

			cout << fTable[i].getTitle().c_str();
			lLenghts.add(fTable[i].getTitle().length());
		}
		SetConsoleTextAttribute(lConsole, lConsoleColor);
		cout << endl;

		// Print Truths
		for (int i = 0; i < lSize; i++)
		{
			lFirst = true;

			for (int j = 0; j < fTable.size(); j++)
			{
				if (!lFirst)
					cout << " | ";
				else
					lFirst = false;

				if (fTable[j].getTruth(i))
				{
					SetConsoleTextAttribute(lConsole, 10);
					if (fCharDisplay)
						cout << "T";
					else
						cout << true;
					SetConsoleTextAttribute(lConsole, lConsoleColor);
				}
				else
				{
					SetConsoleTextAttribute(lConsole, 12);
					if (fCharDisplay)
						cout << "F";
					else
						cout << false;
					SetConsoleTextAttribute(lConsole, lConsoleColor);
				}

				if (lLenghts[j] > 1)
					for (int k = 1; k < lLenghts[j]; k++)
						cout << " ";
			}
			cout << endl;
		}
	}
	SetConsoleTextAttribute(lConsole, lConsoleColor);
}
