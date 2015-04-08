Students
 * Hugo Roque  - 1311162
 * Rômulo Dusi - 1213094

First Delivery: Lexical
	Directory Structure
	 * lexical/src/ - Source files to run the lexical tests.
	 * lexical/build/ - Temporary files necessary to build the testing executable.
	 * lexical/build/dist - Distributions files. (The executable in our case)
	 * lexical/tests/ - Where the lexical tests are.


	Tasks
	The tasks were writen in python. To list all the commands run the script with no parameters:

	Tests
	There are three types of test:
		* Files named as "testname.in": 
			* If exists a file named "testname.expected", the runner compare its content with the program output.
			* If not, our runner expects that the program rejects the input.
		* Files named as "testname.rejected":
			* Each line represents a test case that should be rejected.

Second Delivery: Syntax validation
 * src/ - Source files that we are developing.
 * build/ - Temporary files that are automatically created.
 * build/dist - Distributions files. (The executable in our case)
 * tests/ - Where the tests are.

Technologies used
  * lex 2.5.37
  * python 2.7
