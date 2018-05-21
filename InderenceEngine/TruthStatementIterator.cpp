#include "TruthStatementIterator.h"

TruthStatementIter::TruthStatementIter(const TruthStatement & aStatement)
{
	fLeftMost = &aStatement;
	while (&fLeftMost->getLeft() != &TruthStatement::NIL)
		fLeftMost = &fLeftMost->getLeft();

	fCurrent = fLeftMost;
	if (fCurrent != &TruthStatement::NIL)
		fState = DATA;
	else
		fState = AFTER;

	fRightMost = &aStatement;
	while (&fRightMost->getRight() != &TruthStatement::NIL)
		fRightMost = &fRightMost->getRight();
}

const TruthStatement & TruthStatementIter::operator*() const
{
	return *fCurrent;
}

TruthStatementIter & TruthStatementIter::operator++()
{
	switch (fState)
	{
	case BEFORE:
		fCurrent = fLeftMost;
		if (fCurrent == &TruthStatement::NIL)
			fState = AFTER;
		else
			fState = DATA;
		break;
	case DATA:
		fCurrent = &fCurrent->getRight();
		if (fCurrent == &TruthStatement::NIL)
			fState = AFTER;
	default:
		break;
	}
	return *this;
}

TruthStatementIter TruthStatementIter::operator++(int)
{
	TruthStatementIter lTemp = *this;
	++(*this);
	return lTemp;
}

TruthStatementIter & TruthStatementIter::operator--()
{
	switch (fState)
	{
	case AFTER:
		fCurrent = fRightMost;
		if (fCurrent == &TruthStatement::NIL)
			fState = BEFORE;
		else
			fState = DATA;
		break;
	case DATA:
		fCurrent = &fCurrent->getLeft();
		if (fCurrent == &TruthStatement::NIL)
			fState = BEFORE;
	default:
		break;
	}
	return *this;
}

TruthStatementIter TruthStatementIter::operator--(int)
{
	TruthStatementIter lTemp = *this;
	--(*this);
	return lTemp;
}

bool TruthStatementIter::operator==(const TruthStatementIter & aOther) const
{
	return fLeftMost == aOther.fLeftMost &&
		fRightMost == aOther.fRightMost &&
		fCurrent == aOther.fCurrent &&
		fState == aOther.fState;;
}

bool TruthStatementIter::operator!=(const TruthStatementIter & aOther) const
{
	return !(*this == aOther);
}

TruthStatementIter TruthStatementIter::leftEnd() const
{
	TruthStatementIter lTemp = *this;
	lTemp.fCurrent = &TruthStatement::NIL;
	lTemp.fState = BEFORE;
	return lTemp;
}

TruthStatementIter TruthStatementIter::first() const
{
	return ++(leftEnd());
}

TruthStatementIter TruthStatementIter::last() const
{
	return --(rightEnd());
}

TruthStatementIter TruthStatementIter::rightEnd() const
{
	TruthStatementIter lTemp = *this;
	lTemp.fCurrent = &TruthStatement::NIL;
	lTemp.fState = AFTER;
	return lTemp;
}
