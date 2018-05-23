#pragma once

#include "TruthStatement.h"

class Agent
{
private:
	string fQuery;
	List<TruthStatement> fStatements;

	List<string> getLiterals();

public:
	Agent(string aQuery, List<TruthStatement> aStatements);

	int TT();
	int FC();
	int BC();
};