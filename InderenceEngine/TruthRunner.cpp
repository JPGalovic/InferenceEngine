#include "TruthRunner.h"

bool TruthRunner::implication(bool aP, bool aQ)
{
	if (aP)
		return aQ;
	else
		return true;
}

bool TruthRunner::equivilance(bool aP, bool aQ)
{
	return aP == aQ;
}

bool TruthRunner::conjunction(bool aP, bool aQ)
{
	return aP && aQ;
}

bool TruthRunner::disjunction(bool aP, bool aQ)
{
	return aP || aQ;
}

bool TruthRunner::negation(bool aP)
{
	return !(aP);
}
