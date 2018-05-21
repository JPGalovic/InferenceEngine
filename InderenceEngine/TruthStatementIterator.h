#pragma once

#include "TruthStatement.h"

class TruthStatementIter
{
private:
	enum State {BEFORE, DATA, AFTER};
	State fState;

	const TruthStatement* fLeftMost;
	const TruthStatement* fRightMost;
	const TruthStatement* fCurrent;

public:
	// Construction and Destriction.
	TruthStatementIter(const TruthStatement & aStatement);

	// Getters.
	const TruthStatement & operator*() const;

	TruthStatementIter & operator++();
	TruthStatementIter operator++(int);
	TruthStatementIter & operator--();
	TruthStatementIter operator--(int);

	bool operator== (const TruthStatementIter& aOther) const;
	bool operator!= (const TruthStatementIter& aOther) const;

	TruthStatementIter leftEnd() const;
	TruthStatementIter first() const;
	TruthStatementIter last() const;
	TruthStatementIter rightEnd() const;
};