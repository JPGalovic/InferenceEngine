#pragma once

#include "TruthColoumn.h"

class TruthTable
{
private:
	List<TruthColoumn> fTable;
	bool fCharDisplay;

public:
	TruthTable();
	TruthTable(bool aFlag);

	// Getters
	const TruthColoumn& getColoumn(size_t aIndex);
	const TruthColoumn& getColoumn(string aTitle);

	bool getTruth(string aTitle, size_t aIndex);

	// Setters
	void addColoumn(const TruthColoumn & aColoumn);

	// Printers
	void print() const;
};