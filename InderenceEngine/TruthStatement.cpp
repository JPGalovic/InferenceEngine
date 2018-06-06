#include "TruthStatement.h"
#include "Dictionary.h"

#include <algorithm>
#include <iostream>
#include <Windows.h>

TruthStatement TruthStatement::NIL;

using std::cout;
using std::endl;
using std::remove_if;

using Container::Dictionary;
using Container::Pair;

TruthStatement::TruthStatement(string aStatement, TruthRunner::Symbol lTruthType)
{
	init();

	// Load Statement and Operand
	fStatement = aStatement;
	fStatement.erase(remove_if(fStatement.begin(), fStatement.end(), isspace));
	fOperand = lTruthType;

	// Set child for parenthesis, negation
	if (lTruthType == TruthRunner::PARENTHESIS || lTruthType == TruthRunner::NEGATION)
	{
		fChild = new TruthStatement(aStatement);
		fChild->fParent = this;
	}
}

const List<string> TruthStatement::sort(List<Pair<size_t, string>>& aList)
{
	// Sort the list
	bubble(aList);

	// Create output from sorted list.
	List<string> lResult;
	for (int i = 0; i < aList.size(); i++)
	{
		Pair<size_t, string> lPair = aList[i];
		lResult.add(lPair.getB());
	}

	return lResult;
}

void TruthStatement::bubble(List<Pair<size_t, string>>& aList)
{
	size_t lN = aList.size();
	for (int i = 0; i < lN -1; i++)
		for (int j = 0; j < lN - i - 1; j++)
		{
			Pair<size_t, string> lA = aList[j];
			Pair<size_t, string> lB = aList[j + 1];
			if (lA.getA() > lB.getA())
				aList.swap(j, j + 1);
		}
}

DLNode<Pair<size_t, string>>* TruthStatement::partition(DLNode<Pair<size_t, string>>* aL, DLNode<Pair<size_t, string>>* aH)
{
	Pair<size_t, string> lData = aH->getValue();
	size_t lX = lData.getA();

	DLNode<Pair<size_t, string>>* lI = (DLNode<Pair<size_t, string>>*)&aL->getPrevious();

	for (DLNode<Pair<size_t, string>>* j = aL; j != aH; j = (DLNode<Pair<size_t, string>>*)&j->getNext())
	{
		Pair<size_t, string> lJData = j->getValue();
		size_t lY = lJData.getA();
		if (lY <= lX)
		{
			lI = (lI == &DLNode<Pair<size_t, string>>::NIL) ? aL : (DLNode<Pair<size_t, string>>*)&lI->getNext();
			DLNode<Pair<size_t, string>>::swap(*lI, *j);
		}
	}
	lI = (lI == &DLNode<Pair<size_t, string>>::NIL) ? aL : (DLNode<Pair<size_t, string>>*)&lI->getNext();
	lI->swap(*lI, *aH);
	return lI;
}

void TruthStatement::quicksort(DLNode<Pair<size_t, string>>* aL, DLNode<Pair<size_t, string>>* aH)
{
	if (aL != nullptr && aL != aH && aL != aH)
	{
		DLNode<Pair<size_t, string>>* lP = partition(aL, aH);
		quicksort(aL, (DLNode<Pair<size_t, string>>*)&lP->getPrevious());
		quicksort((DLNode<Pair<size_t, string>>*)&lP->getNext(), aH);
	}
}

void TruthStatement::init()
{
	fLeft = &NIL;
	fRight = &NIL;
	fChild = &NIL;
	fParent = &NIL;

	fOperand = TruthRunner::NONE;

	fTruth = false;
}

TruthStatement::TruthStatement()
{
	init();
}

/**
 * Statement string is initally loaded here, constructor seperates into concerns to be read left to right, seperating by first level parenthesis
 * and other batch statements, eg. (a <=> (c => ~d)) & b & (b => a) should be split into the following segments
 * [a <=> (c => ~d)] & [b] & [b => a], these statements are then seperated into child statements to be run prior to left and right statements.
 *
 * STATEMENT		[(a <=> (c => ~d)) & b & (b => a)]: [a <=> (c => ~d)] & [b] & [b => a]
 *
 * CHILD STATEMENT	[a <=> (c => ~d)]:					[a] <=> [c => ~d]
 * CHILD STATEMENT	[c => ~d]:							[c] => [~d]
 * CHILD STATEMENT	[~d]:								~ [d]
 *
 * CHILD STATEMENT	[b]:								[b]
 *
 * CHILD STATEMENT	[b => a]							[b] => [a]
 */

TruthStatement::TruthStatement(string aStatement)
{
	init();

	Dictionary<TruthRunner::Symbol, string> lSymbols;
	Dictionary<string, TruthRunner::Symbol> lStrings;
	List<string> lLookup;

	lSymbols.addItem(TruthRunner::NEGATION, "~");
	lSymbols.addItem(TruthRunner::EQUIVILANCE, "<=>");
	lSymbols.addItem(TruthRunner::IMPLICATION, "=>");
	lSymbols.addItem(TruthRunner::CONJUNCTION, "&");
	lSymbols.addItem(TruthRunner::DISJUNCTION, "\/");

	lStrings.addItem("~", TruthRunner::NEGATION);
	lStrings.addItem("<=>", TruthRunner::EQUIVILANCE);
	lStrings.addItem("=>", TruthRunner::IMPLICATION);
	lStrings.addItem("&", TruthRunner::CONJUNCTION);
	lStrings.addItem("\/", TruthRunner::DISJUNCTION);

	lLookup.add("<=>");
	lLookup.add("=>");
	lLookup.add("&");
	lLookup.add("\/");
	lLookup.add("~");

	std::string::iterator lEnd_Pos = std::remove(aStatement.begin(), aStatement.end(), ' ');
	aStatement.erase(lEnd_Pos, aStatement.end());

	fStatement = aStatement;

	// Check for Parenthesis
	// Check for Brackets
	List<size_t> lOpenPos;
	List<size_t> lClosePos;
	size_t lPos = 0;
	string lData = aStatement;

	// Find (
	while ((lPos = lData.find_last_of('(')) != string::npos)
	{
		lOpenPos.prepend(lPos);
		lData.erase(lPos, lData.size());
	}

	// Find )
	lPos = 0;
	lData = aStatement;
	while ((lPos = lData.find_last_of(')')) != string::npos)
	{
		lClosePos.prepend(lPos);
		lData.erase(lPos, lData.size());
	}

	// Check Statment Validitity
	if (lOpenPos.size() != lClosePos.size())
		throw std::runtime_error("Invalid Statement, open and close parentesis are not matching: " + aStatement);

	// Create Statements List seperating by parenthesis.
	List<string> lInitalStatements;
	List<bool> lParenthesis;

	// If not brackets found, then whole statement is used.
	if (lOpenPos.empty())
	{
		lInitalStatements.add(aStatement);
		lParenthesis.add(false);
	}
	// Else statements are generated from each part of the overall statment, broken between brackets, eg A ( B ) C ( D ) would create four statements to be read left to right.
	else
	{
		// Get pre first bracket
		if (lOpenPos[0] > 0)
		{
			lInitalStatements.add(aStatement.substr(0, lOpenPos[0]));
			lParenthesis.add(false);
		}

		// Get remaining Statements
		int lCount = 0;
		for (int i = 0; i < lClosePos.size(); i = lCount)
		{
			// determine number of ( before the ith )
			for (int j = i; j < lOpenPos.size(); j++)
			{
				if (lOpenPos[j] < lClosePos[i])
				{
					++lCount;
				}
			}

			lInitalStatements.add(aStatement.substr(lOpenPos[i] + 1, (lClosePos[lCount - 1] - lOpenPos[i]) - 1));
			lParenthesis.add(true);

			if (lCount < lClosePos.size())
				if (lClosePos[lCount - 1] - (lOpenPos[lCount] + 1) > 0)
				{
					lInitalStatements.add(aStatement.substr(lClosePos[lCount - 1] + 1, (lOpenPos[lCount] - lClosePos[lCount - 1]) - 1));
					lParenthesis.add(false);
				}
		}
		// Get after last bracket
		if ((lClosePos[lClosePos.size() - 1] + 1) < aStatement.size())
		{
			lInitalStatements.add(aStatement.substr(lClosePos[lClosePos.size() - 1] + 1, aStatement.size()));
			lParenthesis.add(false);
		}
	}

	// Create binding statements, statements thate are parenthesis are treated like literals
	List<string> lFinalStatements;
	List<TruthRunner::Symbol> lTypes;

	for (int i = 0; i < lInitalStatements.size(); i++)
	{
		if (lParenthesis[i]) // add it to the list
		{
			lFinalStatements.add(lInitalStatements[i]);
			lTypes.add(TruthRunner::PARENTHESIS);
		}
		else // seperate into descrete statements, eg: & b & would be seperated into [&] -> [b] -> [&]
		{
			// Pharse Sub-Statement
			List<Pair<size_t, string>> lOpen;
			List<Pair<size_t, string>> lClosed;

			lOpen.add(Pair<size_t, string>(0, lInitalStatements[i]));

			while (!lOpen.empty())
			{
				bool lLiteral = true;

				Pair<size_t, string> lData = lOpen[0];
				lOpen.removeAt(0);

				string lCheck = lData.getB();
				size_t lInitalPos = lData.getA();

				// seperate out characters
				for (int i = 0; i < lLookup.size(); i++)
				{
					size_t lPos = 0;

					if ((lPos = lData.getB().find(lLookup[i])) != string::npos)
					{
						lLiteral = false;
						string lPre = lCheck.substr(0, lPos);
						string lChar = lCheck.substr(lPos, lLookup[i].length());
						string lPost = lCheck.substr(lPos + lLookup[i].length(), lCheck.size());

						if (lChar == "~")
						{
							Pair<size_t, string> lClosePair(lInitalPos + lPos, lChar + lPost);
							if(!lClosed.contains(lClosePair))
								lClosed.add(lClosePair);
						}
						else
						{
							if (lPost.size() > 0)
								lOpen.add(Pair<size_t, string>(lInitalPos + lPos + lLookup[i].length(), lPost));

							Pair<size_t, string> lClosePair(lInitalPos + lPos, lChar);
							if (!lClosed.contains(lClosePair))
								lClosed.add(lClosePair);
						}

						if(lPre.size() > 0)
							lOpen.add(Pair<size_t, string>(lInitalPos, lPre));
						break;
					}
				}
				if (lLiteral)
					if (!lClosed.contains(lData))
						lClosed.add(lData);
			}

			// Add sorted result to output.
			List<string> lSorted = sort(lClosed);

			for (int j = 0; j < lSorted.size(); j++)
			{
				string lKey = lSorted[j];
				if (lStrings.hasKey(lSorted[j]))
				{
					lFinalStatements.add(lSorted[j]);
					TruthRunner::Symbol lType = lStrings.getValue(lSorted[j]);
					lTypes.add(lType);
				}
				else if ((lPos = lSorted[j].find("~")) == 0)
				{
					lFinalStatements.add(lSorted[j].substr(1, lSorted[i].size()));
					lTypes.add(TruthRunner::NEGATION);
				}
				else
				{
					lFinalStatements.add(lSorted[j]);
					lTypes.add(TruthRunner::LITERAL);
				}
			}
		}
	}

	// Load segments
	List<TruthStatement*> lSegments;
	for (int i = 0; i < lFinalStatements.count(); i++)
		lSegments.add(new TruthStatement(lFinalStatements[i], lTypes[i]));

	// Attach Statements
	for (int i = 0; i < lFinalStatements.count() - 1; i++)
		lSegments[i]->setRight(*lSegments[i + 1]);

	bool lSet = false;
	for (int i = 0; i < lFinalStatements.count(); i++)
		if (lSegments[i]->fOperand != TruthRunner::LITERAL && lSegments[i]->fOperand != TruthRunner::PARENTHESIS && lSegments[i]->fOperand != TruthRunner::NONE)
		{
			fChild = lSegments[i];
			lSet = true;
			break;
		}
	if (!lSet)
		fChild = lSegments[0];
	fChild->fParent = this;
		
}

bool TruthStatement::run()
{
	switch (fOperand)
	{
	case TruthRunner::IMPLICATION:
		return TruthRunner::implication(fLeft->run(), fRight->run());
	case TruthRunner::EQUIVILANCE:
		return TruthRunner::equivilance(fLeft->run(), fRight->run());
	case TruthRunner::CONJUNCTION:
		return TruthRunner::conjunction(fLeft->run(), fRight->run());
	case TruthRunner::DISJUNCTION:
		return TruthRunner::disjunction(fLeft->run(), fRight->run());
	case TruthRunner::NEGATION:
		if (hasChild())
			return TruthRunner::negation(fChild->run());
		throw std::runtime_error("Statement not correctly initalised.");
	case TruthRunner::LITERAL:
		return fTruth;
	case TruthRunner::PARENTHESIS:
	case TruthRunner::NONE:
		if (hasChild())
			return fChild->run();
	default:
		throw std::runtime_error("Statement not correctly initalised.");
	}
}

const TruthStatement & TruthStatement::getLeft() const
{
	return *fLeft;
}

const TruthStatement & TruthStatement::getRight() const
{
	return *fRight;
}

const TruthStatement & TruthStatement::getChild() const
{
	return *fChild;
}

const TruthStatement & TruthStatement::getParent() const
{
	return *fParent;
}

string TruthStatement::getStatement() const
{
	string lResult;

	if (hasChild())
	{
		TruthStatement* lChild = fChild;
		while (lChild->fLeft != &TruthStatement::NIL)
			lChild = lChild->fLeft;

		lResult += lChild->getStatement();

		while (lChild->fRight != &TruthStatement::NIL)
		{
			lChild = lChild->fRight;
			lResult += lChild->getStatement();
		}
	}
	
	switch (fOperand)
	{
	case TruthRunner::IMPLICATION:
		return " => ";
	case TruthRunner::EQUIVILANCE:
		return " <=> ";
	case TruthRunner::CONJUNCTION:
		return " & ";
	case TruthRunner::DISJUNCTION:
		return " \\/ ";
	case TruthRunner::NEGATION:
		return " ~ " + lResult;
	case TruthRunner::PARENTHESIS:
		return " (" + lResult + ") ";
	case TruthRunner::LITERAL:
		return fStatement;
	case TruthRunner::NONE:
		return lResult;
	default:
		return string();
	}
}

string TruthStatement::getFullStatement() const
{
	string lResult;

	if (hasChild())
	{
		TruthStatement* lChild = fChild;
		while (lChild->fLeft != &TruthStatement::NIL)
			lChild = lChild->fLeft;

		lResult += lChild->getStatement();

		while (lChild->fRight != &TruthStatement::NIL)
		{
			lChild = lChild->fRight;
			lResult += lChild->getStatement();
		}
	}

	switch (fOperand)
	{
	case TruthRunner::IMPLICATION:
		return fLeft->getStatement() + " => " + fRight->getStatement();
	case TruthRunner::EQUIVILANCE:
		return fLeft->getStatement() + " <=> " + fRight->getStatement();
	case TruthRunner::CONJUNCTION:
		return fLeft->getStatement() + " & " + fRight->getStatement();
	case TruthRunner::DISJUNCTION:
		return fLeft->getStatement() + " \\/ " + fRight->getStatement();
	case TruthRunner::NEGATION:
		return " ~ " + lResult;
	case TruthRunner::PARENTHESIS:
		return " (" + lResult + ") ";
	case TruthRunner::LITERAL:
		return fStatement;
	case TruthRunner::NONE:
		return lResult;
	default:
		return string();
	}
}

bool TruthStatement::operator==(const TruthStatement & aOther) const
{
	return fStatement == aOther.fStatement;
}

bool TruthStatement::operator!=(const TruthStatement & aOther) const
{
	return !(*this == aOther);
}

bool TruthStatement::hasLeft() const
{
	return fLeft != &NIL;
}

bool TruthStatement::hasRight() const
{
	return fRight != &NIL;
}

bool TruthStatement::hasChild() const
{
	return fChild != &NIL;
}

bool TruthStatement::hasParent() const
{
	return fParent != &NIL;
}

const TruthRunner::Symbol & TruthStatement::getOperand() const
{
	return fOperand;
}

List<string> TruthStatement::getLiterals() const
{
	List<string> lResult;
	if (fOperand == TruthRunner::LITERAL)
	{
		lResult.add(fStatement);
		return lResult;
	}
	else if (fChild != &TruthStatement::NIL)
	{
		TruthStatement* lChild = fChild;
		while (lChild->fLeft != &TruthStatement::NIL)
			lChild = lChild->fLeft;

		List<string> lCheck = lChild->getLiterals();
		for (int i = 0; i < lCheck.size(); i++)
			if (!lResult.contains(lCheck[i]))
				lResult.add(lCheck[i]);

		while (lChild->fRight != &TruthStatement::NIL)
		{
			lChild = lChild->fRight;
				
			List<string> lCheck = lChild->getLiterals();
			for (int i = 0; i < lCheck.size(); i++)
				if (!lResult.contains(lCheck[i]))
					lResult.add(lCheck[i]);
		}
	}
	return lResult;
}

List<TruthStatement> TruthStatement::getSubStatements() const
{
	List<TruthStatement> lResult;

	if (fChild != &TruthStatement::NIL)
	{
		TruthStatement* lChild = fChild;
		while (lChild->fLeft != &TruthStatement::NIL)
			lChild = lChild->fLeft;

		List<TruthStatement> lCheck = lChild->getSubStatements();
		for (int i = 0; i < lCheck.size(); i++)
			if (!lResult.contains(lCheck[i]))
				lResult.add(lCheck[i]);

		while (lChild->fRight != &TruthStatement::NIL)
		{
			lChild = lChild->fRight;

			List<TruthStatement> lCheck = lChild->getSubStatements();
			for (int i = 0; i < lCheck.size(); i++)
				if (!lResult.contains(lCheck[i]))
					lResult.add(lCheck[i]);
		}
	}

	if (fOperand != TruthRunner::LITERAL && fOperand != TruthRunner::NONE)
		lResult.add(*this);

	return lResult;
}

TruthTable TruthStatement::getTruthTable()
{
	TruthTable lResult;
	List<string> lLiterals = getLiterals();
	List<List<bool>> lTruthSet;

	// construct truth set.

	size_t lN = lLiterals.size();
	size_t lM = exp2(lN);

	for (size_t i = 0; i < lM; i++)
	{
		List<bool> lTruthSetRow;
		size_t lO = i;
		for (size_t j = 0; j < lN; j++)
		{
			lTruthSetRow.add(lO % 2 == 0 ? false : true);
			lO = lO / 2;
		}
		lTruthSet.add(lTruthSetRow);
	}

	// add literals to truth table.
	for (size_t j = 0; j < lN; j++)
	{
		TruthColoumn lTruth(lLiterals[j]);
		for (size_t i = 0; i < lM; i++)
			lTruth.addTruth(lTruthSet[i][j]);
		lResult.addColoumn(lTruth);
	}

	// calculate the remaining truths from ruleset.
	List<TruthStatement> lRules = getSubStatements();
	lRules.add(*this); // Add self to rules, as getSubStatements() phases out base rules.
	for (int k = 0; k < lRules.size(); k++) // for each rule
	{

		string lStatement = lRules[k].getFullStatement();
		std::string::iterator lEnd_Pos = std::remove(lStatement.begin(), lStatement.end(), ' ');
		lStatement.erase(lEnd_Pos, lStatement.end());

		if (lRules[k].fOperand == TruthRunner::PARENTHESIS) // remove opening and closing parenthesis from statment.
		{
			lStatement.erase(lStatement.begin(), lStatement.begin() + 1);
			lStatement.erase(lStatement.end() - 1, lStatement.end());
		}

		// check if duplicate rule in truthtable
		if (!lResult.hasColoumn(lStatement))
		{
			TruthColoumn lTruth(lStatement);
			TruthStatement lTestRunner(lStatement);

			if (lRules[k].getOperand() == TruthRunner::NEGATION)
			{
				TruthStatement lTestRunner("~ " + lStatement);
				TruthColoumn lTruth("~" + lStatement);
			}

			for (int i = 0; i < lM; i++) // for each truth
			{
				for (int j = 0; j < lLiterals.count(); j++) // fore each literal
					lTestRunner.setTruth(lLiterals[j], lResult.getTruth(lLiterals[j], i));
				lTruth.addTruth(lTestRunner.run());
			}
			lResult.addColoumn(lTruth);
		}
	}

	return lResult;
}

void TruthStatement::setTruth(string aLiteral, bool aTruth)
{
	if (fOperand == TruthRunner::LITERAL)
		if (fStatement == aLiteral)
			fTruth = aTruth;

	if (fChild != &TruthStatement::NIL)
	{
		TruthStatement* lChild = fChild;
		while (lChild->fLeft != &TruthStatement::NIL)
			lChild = lChild->fLeft;

		lChild->setTruth(aLiteral, aTruth);

		while (lChild->fRight != &TruthStatement::NIL)
		{
			lChild = lChild->fRight;
			lChild->setTruth(aLiteral, aTruth);
		}
	}
}

void TruthStatement::setLeft(TruthStatement & aOther)
{
	aOther.fRight = this;

	if (hasLeft())
	{
		aOther.fLeft = fLeft;
		fLeft->fRight = &aOther;
	}

	fLeft = &aOther;
}

void TruthStatement::setRight(TruthStatement & aOther)
{
	aOther.fLeft = this;

	if (hasRight())
	{
		aOther.fRight = fRight;
		fRight->fLeft = &aOther;
	}

	fRight = &aOther;
}

void TruthStatement::setChild(TruthStatement & aOther)
{
	fChild = &aOther;
	aOther.fParent = this;
}

void TruthStatement::setParent(TruthStatement & aOther)
{
	fParent = &aOther;
	aOther.fChild = this;
}

void TruthStatement::print() const
{
	cout << getStatement();
}

void TruthStatement::printTruthTable() 
{
	cout << "Truth Table for: " << getStatement() << endl;
	getTruthTable().print();
}
