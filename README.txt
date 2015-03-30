Directory Structure

src/ - Source files that we are developing.
build/ - Temporary files that are automatically created.
build/dist - Distributions files. (The executable in our case)
build/test - Simulated distributions files for testing. (The executable in our case)
tests/ - Where the tests are.


Tasks
The tasks were writen in python, if you execute the scrypt without parameters it will appear some tasks:
clear - Clear build files
compile - Compile all files
run <input-name> - Run with one input
test <test-path> - Run with one test
test_all - Run all tests

There are three types of test:
 	- Files with extension ".in": 
 		- If exists a file witch has same name and extension ".expected", the program run and compare the output with the ".expected"
 		- If not, out program try run it and output errors.
 	- Files with extension ".rejected":
 		- These files have errors, and it is expected. If these test pass, our script will be display an error.
 	