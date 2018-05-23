#include "Agent.h"

#include <iostream>
#include <algorithm>

using std::cout;
using std::cerr;
using std::endl;

List<string> Agent::getLiterals()
{
	List<string> lResult;

	for (int i = 0; i < fStatements.size(); i++)
	{
		List<string> lLiterals = fStatements[i].getLiterals();
		for (int j = 0; j < lLiterals.size(); j++)
			if (!lResult.contains(lLiterals[j]))
				lResult.add(lLiterals[j]);
	}

	if (lResult.empty())
		throw std::runtime_error("Agent has no literals.");

	return lResult;
}

Agent::Agent(string aQuery, List<TruthStatement> aStatements)
{
	fQuery = aQuery;
	fStatements = aStatements;
}

int Agent::TT()
{
	// Get list of literals
	try
	{
		List<std::string> lLiterals = getLiterals();
		// Check if queiry is in literals?

		std::string lOutput;

		if (lLiterals.contains(fQuery))
		{
			lOutput = "YES";

			for (int i = 0; i < fStatements.size(); i++)
			{
				if (fStatements[i].getLiterals().contains(fQuery))
				{
					string lStatementName = fStatements[i].getFullStatement();
					std::string::iterator lEnd_Pos = std::remove(lStatementName.begin(), lStatementName.end(), ' ');
					lStatementName.erase(lEnd_Pos, lStatementName.end());

					if (fStatements[i].getOperand() == TruthRunner::NEGATION)
						lStatementName = "~" + lStatementName;

					TruthStatement lTruthStatement = fStatements[i];
					TruthTable lTruthTable = lTruthStatement.getTruthTable();
					TruthColoumn lTruthColomn = lTruthTable.getColoumn(lStatementName);

					int lTrue = 0;
					int lFalse = 0;

					for (int j = 0; j < lTruthColomn.size(); j++)
					{
						if (lTruthColomn.getTruth(j))
							lTrue++;
						else
							lFalse++;
					}

					cout << lOutput << ": " << lTrue << endl;
					break;
				}
			}

			system("PAUSE");
			return 0;
		}
		else
		{
			cout << "NO;" << endl;
			system("PAUSE");
			return 1001;
		}
	}
	catch (const std::runtime_error& e)
	{
		cerr << "Error: " << e.what() << endl;
		system("PAUSE");
		return 1000;
	}
}

int Agent::FC()
{
	try
	{
		List<string> lOpenList;

		List<string> lClosedList;

		// Get list of literals from Statement list
		for (List<TruthStatement>::Iter lIter = fStatements.getIter(); lIter != lIter.rightEnd(); lIter++)
		{
			TruthStatement lStatement = *lIter;
			if (lStatement.getOperand() == TruthRunner::LITERAL)
				lOpenList.add(lStatement.getStatement());
		}

		bool lFound = false;

		while (!lOpenList.empty())
		{
			std::string lChecking = lOpenList[0];
			lOpenList.removeAt(0);
			lClosedList.add(lChecking);

			List<std::string> lTempClosed;
			List<std::string> lTempOpen;

			lTempOpen.add(lChecking);

			while (!lTempOpen.empty())
			{
				std::string lTempChecking = lTempOpen[0];
				lTempOpen.removeAt(0);
				lTempClosed.add(lTempChecking);

				std::cout << lTempOpen << ", " << lTempClosed << std::endl;

				for (int i = 0; i < fStatements.size(); i++)
				{
					fStatements[i].print();
					// Eliminate statement if it is a Literal or Negation only.
					if (fStatements[i].getOperand() != TruthRunner::LITERAL || fStatements[i].getOperand() != TruthRunner::NEGATION)
					{
						// Check if statement relates to Check Statement.
						if (fStatements[i].getLeft().getLiterals().contains(lTempChecking))
						{
							List<std::string> lValid = fStatements[i].getRight().getLiterals();
							for (int j = 0; j < lValid.size(); j++)
								if (!lTempOpen.contains(lValid[j]))
									lTempOpen.add(lValid[j]);
						}
					}
				}
			}

			std::cout << std::endl << std::endl << lOpenList << ", " << lClosedList << std::endl << std::endl;

			// Check if temp closed list contains query
			if (lTempClosed.contains(fQuery))
			{
				for (int i = 1; i < lTempClosed.size(); i++)
					if (!lClosedList.contains(lTempClosed[i]))
						lClosedList.add(lTempClosed[i]);
				lFound = true;
			}
		}

		// Print results
		if (lFound)
		{
			std::cout << "YES: ";
			bool lFirst = true;
			for (List<string>::Iter lIter = lClosedList.getIter(); lIter != lIter.rightEnd(); lIter++)
				if (lFirst)
				{
					std::cout << *lIter;
					lFirst = false;
				}
				else
					std::cout << ", " << *lIter;

			std::cout << std::endl;
			system("PAUSE");
			return 0;
		}
		else
		{
			std::cout << "NO: ";
			bool lFirst = true;
			for (List<string>::Iter lIter = lClosedList.getIter(); lIter != lIter.rightEnd(); lIter++)
				if (lFirst)
				{
					std::cout << *lIter;
					lFirst = false;
				}
				else
					std::cout << ", " << *lIter;

			std::cout << std::endl;
			system("PAUSE");
			return 1001;
		}
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		system("PAUSE");
		return 1000;
	}
}

int Agent::BC()
{
	try
	{
		List<std::string> lOpenList;
		List<std::string> lClosedList;

		lOpenList.add(fQuery);

		while (!lOpenList.empty())
		{
			std::string lChecking = lOpenList[0];
			lOpenList.removeAt(0);
			lClosedList.add(lChecking);

			for (int i = 0; i < fStatements.size(); i++)
			{
				// Eliminate statement if it is a Literal or Negation only.
				if (fStatements[i].getOperand() != TruthRunner::LITERAL)
				{
					// Check if statement relates to Check Statement.
					if (fStatements[i].getRight().getLiterals().contains(lChecking))
					{
						List<std::string> lValid = fStatements[i].getLeft().getLiterals();
						for (int j = 0; j < lValid.size(); j++)
							if (!lOpenList.contains(lValid[j]))
								lOpenList.add(lValid[j]);
					}
				}
			}
		}

		// Print results
		if (getLiterals().contains(fQuery))
		{
			std::cout << "YES: ";
			bool lFirst = true;
			for (List<string>::Iter lIter = lClosedList.getIter(); lIter != lIter.rightEnd(); lIter++)
				if (lFirst)
				{
					std::cout << *lIter;
					lFirst = false;
				}
				else
					std::cout << ", " << *lIter;

			std::cout << std::endl;
			system("PAUSE");
			return 0;
		}
		else
		{
			std::cout << "NO: ";
			bool lFirst = true;
			for (List<string>::Iter lIter = lClosedList.getIter(); lIter != lIter.rightEnd(); lIter++)
				if (lFirst)
				{
					std::cout << *lIter;
					lFirst = false;
				}
				else
					std::cout << ", " << *lIter;

			std::cout << std::endl;
			system("PAUSE");
			return 1001;
		}
	}
	catch (const std::runtime_error& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		system("PAUSE");
		return 1000;
	}
}
