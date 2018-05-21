# CustomCPP
Custom data types and engine elements for C++ programming.

Developed using knowlege and code developed in Bachelor of Computer Science (BA-CS), undertaken at Swinburne University of Technology.

## Version Tracking
### Semantic Numbering.
Each type and library developed in this package has its version tracked semantically. the version tracking should follow the outline of Semantic Versioning 2.0.0, found [here](https://semver.org/).
file versions are denoted in the file header, @version    v MAJOR . MINOR . PATCH, and where applicable is marked with a build version tag following a dash (-).

MAJOR version number denotes major usually fully incompatable API changes.
MINOR version number denotes added functionality that is backwars compatible.
PATCH version number denotes changes, fixes and improvements made to existing code, without added functionality.

Build version tags can include -ALPHA, -BETA, -RC or independant SHADOW releases usually donoted with the year and week number, eg. -18W21.
Order of release, SHADOW > ALPHA > BETA > RC > X.Y.Z.

### Change Log.
Change logs are kept for each inderviual type and library, changes should be written in an unordered list format, kept in indervidual fies for each specified type and library.

### File Headers
Each file contains a header that has information about file authors, verions (where applicable) and date.
File authors are denoted by a list of contributors to a file following the @author tag.
File versions are denoted, where applicable as described above, following the @version tag.
File date information in the header is denoted in the format \[MONTH\]\[YEAR\] following the @date tag. this contains an indication of the month and year the file was last edited.
Each function/method contained within a file is indervidually dated using full DD\\MM\\YYYY format following the @date tag.

## Package Contents
### Avalible Types
* Doubly Linked List
* Dynamic Stack
* Dynamic Queue
* N-Tree
* Dictionary
### Custom Libraries