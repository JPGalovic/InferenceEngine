#include <string>
#include <iostream>
#include <algorithm>

#include "TruthTable.h"
#include "TruthStatement.h"

using std::string;
using std::cout;
using std::endl;

int main(int argc, char** argv)
{
	List<string> lList;

	string lTest = "(a<=>(c=>~d))&b&(b=>a)";
	//string lTest = "a<=>(c=>~d)";

	TruthStatement lStatment(lTest);

	cout << "Phased Statements: ";
	lStatment.print();
	cout << endl << endl;

	lStatment.printTruthTable();
	cout << endl << endl;

	system("PAUSE");
}