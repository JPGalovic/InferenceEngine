#pragma once

#include "TruthTable.h"
#include "TruthRunner.h"

#include "Pair.h"

#include <string>

using std::string;
using Container::Pair;
using Container::DLNode;

class TruthStatement
{
private:
	string fStatement;
	bool fTruth;

	TruthStatement* fLeft;
	TruthStatement* fRight;
	TruthStatement* fChild;
	TruthStatement* fParent;

	TruthRunner::Symbol fOperand;

	// Construction and Destruction.
	TruthStatement(string aStatement, TruthRunner::Symbol lTruthType);


	// Getters
	const List<string> sort(List<Pair<size_t, string>> & aList);

	void bubble(List<Pair<size_t, string>> & aList);
	DLNode<Pair<size_t, string>>* partition(DLNode<Pair<size_t, string>>* aL, DLNode<Pair<size_t, string>>* aH);
	void quicksort(DLNode<Pair<size_t, string>>* aL, DLNode<Pair<size_t, string>>* aH);

	// Setters
	void init();

public:
	// Construction and Destruction
	static TruthStatement NIL;
	TruthStatement();
	TruthStatement(string aStatement);
	

	// Getters
	bool run();

	const TruthStatement & getLeft() const;
	const TruthStatement & getRight() const;
	const TruthStatement & getChild() const;
	const TruthStatement & getParent() const;

	string getStatement() const;
	string getFullStatement() const;

	bool operator==(const TruthStatement & aOther) const;
	bool operator!=(const TruthStatement & aOther) const;

	bool hasLeft() const;
	bool hasRight() const;
	bool hasChild() const;
	bool hasParent() const;

	const TruthRunner::Symbol& getOperand() const;

	List<string> getLiterals() const;
	List<TruthStatement> getSubStatements() const;
	TruthTable getTruthTable();
	
	// Setters
	void setTruth(string aLiteral, bool aTruth);
	
	void setLeft(TruthStatement & aOther);
	void setRight(TruthStatement & aOther);
	void setChild(TruthStatement & aOther);
	void setParent(TruthStatement & aOther);

	// Printers
	void print() const;
	void printTruthTable();
};