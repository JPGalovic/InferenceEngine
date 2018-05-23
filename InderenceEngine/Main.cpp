/**
 * Inderence Engine.
 * Code developed for COS30019 - Introduction to Artificial Intelegence.
 * @author  J.P.Galovic
 * @version v1.0.0
 * @date    MAY18
 */

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "TruthStatement.h"
#include "Agent.h"

using std::string;
using std::fstream;
using std::ios;

using std::getline;
using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char** argv)
{
	// Check for correct number of arguments.
	if (argc < 3)
	{
		cerr << "Error: Incorrect number of aguments set" << endl << "Usage: <FILENAME> <TT|FC|BC METHOD>" << endl;
		system("PAUSE");
		return 1;
	}

	// Check that method is set correctly
	string lMethod = argv[2];

	if (lMethod != "TT" && lMethod != "FC" && lMethod != "BC")
	{
		cerr << "Error: This program only accepts set methods." << endl << "Usage: Truth Table (TT), Forward Chaining (FC) and Backward Chaining (BC)." << endl;
		system("PAUSE");
		return 2;
	}

	// Open input file
	string lFileName = argv[1];
	fstream lInputFile;

	lInputFile.open(lFileName, ios::in);

	if (!lInputFile)
	{
		cerr << "Error: Unable to open file " << lFileName << endl;
		system("PAUSE");
		return 3;
	}

	// Read file.
	bool lTell = false;
	bool lAsk = false;

	string lQuery;
	string lTellTable;

	List<TruthStatement> lStatements;

	for (string lLine; getline(lInputFile, lLine);)
	{
		cout << "Read Line: " << lLine << endl;

		if (lLine == "TELL")
		{
			lTell = true;
			lAsk = false;
			continue;
		}

		if (lLine == "ASK")
		{
			lTell = false;
			lAsk = true;
			continue;
		}

		if (lTell)
		{
			// Remove spaces from string
			std::stringstream lData(lLine);

			Container::List<std::string> lTellList;

			// Get each statement (seperated by ;)
			while (lData.good())
			{
				std::string lTellValue;
				std::getline(lData, lTellValue, ';');
				lTellList.add(lTellValue);
			}

			lTellList.removeAt(lTellList.size() - 1);

			for (int i = 0; i < lTellList.size(); i++)
				lStatements.add(TruthStatement(lTellList[i]));

			lTell = false;
		}

		if (lAsk)
		{
			lQuery = lLine;
			lAsk = false;
		}
	}

	if (lStatements.empty())
	{
		cerr << "Error: No statements read." << endl;
		system("PAUSE");
		return 4;
	}

	cout << "Statements Read: ";
	bool lFirst = true;
	for (int i = 0; i < lStatements.size(); i++)
	{
		if (!lFirst)
			cout << ", ";
		else
			lFirst = false;
		lStatements[i].print();
		
	}
		

	cout << endl << "Query Read: " << lQuery << endl << endl;

	system("PAUSE");
	system("CLS");

	Agent lAgent(lQuery, lStatements);

	if (lMethod == "TT")
		return lAgent.TT();
	else if (lMethod == "FC")
		return lAgent.FC();
	else if (lMethod == "BC")
		return lAgent.BC();
	else
		return 4;

	return 0;
}