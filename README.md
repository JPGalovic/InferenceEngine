Inference Engine
J.P. Galovic 6819710

Features:
Program implements Phaser that reads a file that is in the form:

TELL
statements; separated; by; semicolons;
ASK
query

Phaser can read the following statement types:
implication, equivalence, conjunction, disjunction, negation and literals.
'=>'       , '<=>'      , '&'        , '\/'       , '~'        , x (any other char).
Phaser ignores spaces ' '.

Phaser produces knowledge base with functions for producing truth tables.
Truth Tables are built on standard rules:
┌───┬───┬────────┬─────────┬───────┬────────┬────┐
│ A │ B │ A => B │ A <=> B │ A & B │ A \/ B │ ~B │
├───┼───┼────────┼─────────┼───────┼────────┼────┤
│ T │ T │ T      │ T       │ T     │ T      │ F  │
│ T │ F │ F      │ F       │ F     │ T      │ T  │
│ F │ T │ T      │ F       │ F     │ T      │    │
│ F │ F │ T      │ T       │ F     │ F      │    │
└───┴───┴────────┴─────────┴───────┴────────┴────┘

Agent contains three algorithms that can be called:
TT : Truth Table Lookup, determines if rule exists for query, then prints number of true elements in found rule.
FC : Chains forward from defined literals, showing steps until query is found.
BC : Chains backward from query, showing all literals that lead to query.

Test cases:
Two main KBs were used in testing, the assignment provided KB:

p2 => p3; p3 => p1; c => e; b & e => f; f & g => h; p1 => d; p1 & p3 => c; a; b; p2;

and an aditional KB containing more complex statements:

a => b; a <=> c; c \/ b ; ~a & b => d; a => b => c; a; b;

in both cases, the rules were checked against hand written truth tables to determine code validity.

Acknowledgments / Resources:

to develop this program I used previously developed C++ templates and classes (done by myself in other units)

https://en.wikipedia.org/wiki/List_of_logic_symbols
used to aid development of truth table definitions (listed above)

Notes:
before the selected algorithm is run, a series of truth tables for each statement will be shown.
press any key to continue to Agent execution.

the program (.exe) requires two arguments be set, firstly the dir address for the file to be read, and the selected algorithm, TT, FC or BC. errors will be displayed then upon exit one of the following will be returned:

Code : Description
0    : Program exited on success (meaning no errors encounter)
1    : Program exited on error, Incorrect number of argument set, usage: <FILENAME> <TT|FC|BC METHOD>
2    : Program exited on error, Method not in accepted format, usage: Truth Table (TT), Forward Chaining (FC) and Backward Chaining (BC).
3    : Program exited on error, File cannot be opened or file not found.
4    : Program exited on unknown error.
1000 : Runtime Error, see error text displayed
1001 : KB does not contain literal with name used in query.

Summary Report:
All work done in this assignment was done by James P. Galovic, J.P. Galovic, 6819710
Intro to AI COS30019.
Source code: https://github.com/JPGalovicSoftware/inference_engine