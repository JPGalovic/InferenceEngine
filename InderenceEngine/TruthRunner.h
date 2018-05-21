#pragma once

static class TruthRunner {
public:
	static enum Symbol {
		IMPLICATION,	// =>
		EQUIVILANCE,	// <=>
		CONJUNCTION,	// &
		DISJUNCTION,	// \/
		NEGATION,		// ~

		PARENTHESIS,	// ( )
		LITERAL,		// any other character

		NONE
	};

	static bool implication(bool aP, bool aQ);
	static bool equivilance(bool aP, bool aQ);
	static bool conjunction(bool aP, bool aQ);
	static bool disjunction(bool aP, bool aQ);
	static bool negation(bool aP);
};
