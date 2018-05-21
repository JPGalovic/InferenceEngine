#pragma once

#include "List.h"

using Container::List;
using std::string;

class TruthColoumn
{
private:
	string fTitle;
	List<bool> fData;

public:
	// Construction and Destruction
	TruthColoumn();
	TruthColoumn(string aTitle);

	TruthColoumn& operator=(const TruthColoumn& aOther);

	// Getters
	string getTitle() const;
	bool getTruth(size_t aIndex) const;

	bool areYou(string aTitle) const;
	int size() const;

	// Setters
	void setTitle(string aTitle);
	void addTruth(bool aTruth);
};