Smart Campus Navigation — Milestone 1

PURPOSE
Load campus buildings, connections, facilities, and departments from
text files. Support facility ID lookup, campus graph display, BFS
reachability, and department hierarchy operations.

BUILD ON CELL
Run from the project root:
g++ -std=c++11 -Wall -Wextra -pedantic -Iinclude main.cpp src/CampusGraph.cpp src/Hashtable.cpp src/DepartmentTree.cpp -o campus

RUN
Default instructor data:
./campus

Alternative data directory:
./campus testdata/validation

The selected directory must contain:
buildings.txt, connections.txt, facilities.txt, departments.txt

Files load in that order. Missing files produce errors and zero-count
summaries; invalid records produce filename/line-number warnings.

MENU
1. View facilities
2. Search facility by ID
3. View graph
4. Display graph counts and degrees
5. BFS reachability
6. Add department
7. Search department
8. Display department hierarchy
9. Preorder traversal
10. Exit
Option 0 also exits.

Names accept spaces. Comparisons ignore capitalization and outer
whitespace. Facility IDs are normalized to uppercase before hashing.
Department additions are stored in memory only.

LAYOUT
main.cpp                 Loading utilities and menu coordination
include/                 Class and record declarations
src/                     Hash table, graph, and tree implementations
data/                    Original instructor input files
testdata/                Separate validation and collision datasets
tests/                   Source-level tests
CONTRIBUTIONS.txt        Team contribution statements
design_document.txt      Data structure design
test_results.txt         Recorded test evidence
.gitignore               Excludes compiled outputs and temporary files

TESTING
The program was compiled and exercised on cell02-cse.
See test_results.txt for inputs, expected results, and actual results.
Do not include the compiled campus executable in the submission ZIP.
