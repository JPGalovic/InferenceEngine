#include "TruthColoumn.h"

#include <string>
#include <stdexcept>

TruthColoumn::TruthColoumn()
{
	fTitle = string();
}

TruthColoumn::TruthColoumn(string aTitle)
{
	fTitle = aTitle;
}

TruthColoumn & TruthColoumn::operator=(const TruthColoumn & aOther)
{
	if (&aOther != this)
	{
		fTitle = aOther.fTitle;
		fData = aOther.fData;
	}
	return *this;
}

string TruthColoumn::getTitle() const
{
	return fTitle;
}

bool TruthColoumn::getTruth(size_t aIndex) const
{
	try
	{
		return fData[aIndex];
	}
	catch (const std::range_error& e)
	{
		throw e;
	}
}

bool TruthColoumn::areYou(string aTitle) const
{
	return aTitle == fTitle;
}

int TruthColoumn::size() const
{
	return fData.size();
}

void TruthColoumn::setTitle(string aTitle)
{
	fTitle = aTitle;
}

void TruthColoumn::addTruth(bool aTruth)
{
	fData.add(aTruth);
}
