Students
 * Hugo Roque  - 1311162
 * Rômulo Dusi - 1213094

Fourth Delivery: Syntax tree
  Runner
		* The output of the program is the tree printed or an error message in case of
		  an undefined variable have been used or an type mismatch in the assignment.

	Tests
	There are three types of test:
		* Files named as "testname.in" and its pair "testname.expected":
		  The "testname.expeted" file have the rules that should be in the output.
			So the "testname.expected" may have not all the rules of output, but all the
			rules in "testname.expected" must be in output respecting the expected order.
		* Files named as "testname.rejected":
		  Each block between "-----" represents a test case that should be rejected.
	
	Changelog:
		* Module created: ast_traversing - Encapsulates the logic of traversing the AST.
		* Module modified: ast_print - Now uses the ast_traversing
		* Module created: stack - ADT representing an stack of variable scopes.
		* Module created: ast_variables - Traverse the AST referencing each variable to its
		  declaration (if any) and output a report of success or error.
		* Module created: ast_types - Traverse the AST verifying verifying the types of the
		  variables in assignments.

Third Delivery: Syntax tree
  Directory Structure
	  * src/ - Source files that we are developing.
	  * build/ - Temporary files that are automatically created.
	  * build/dist - Distributions files. (The executable in our case)
	  * tests/ - Where the tests are.
	
	Tasks
	  * The syntax tasks were writen in python. To list all the commands run the script
		  "scripts/syntax.py" with no parameters.
		* To run the tests of all the projects execute the script "scripts/run_tests.sh".

  Runner
		* The output of the program is the tree printed. Commands to create or modify 
		  nodes of tree were inserted in monga.yacc file.

	Tests
	There are three types of test:
		* Files named as "testname.in" and its pair "testname.expected":
		  The "testname.expeted" file have the rules that should be in the output.
			So the "testname.expected" may have not all the rules of output, but all the
			rules in "testname.expected" must be in output respecting the expected order.
		* Files named as "testname.rejected":
		  Each block between "-----" represents a test case that should be rejected.
		* Files named as "testname.accepted":
		  Each block between "-----" represents a test case that should be accepted.
	
	Changes in Syntax validation:
		* 

Second Delivery: Syntax validation
  Directory Structure
	  * src/ - Source files that we are developing.
	  * build/ - Temporary files that are automatically created.
	  * build/dist - Distributions files. (The executable in our case)
	  * tests/ - Where the tests are.
		* scripts/ - Where are the scripts that build, run and test the projects.
		* lexical/ - The files of the lexical project (Still buildable)
	
	Tasks
	  * The syntax tasks were writen in python. To list all the commands run the script
		  "scripts/syntax.py" with no parameters.
		* To run the tests of all the projects execute the script "scripts/run_tests.sh".

  Runner
		* The output of the program is the rules reducted. To do this, the runner
		  create a debugging version of "monga.yacc" where it have printf to each rule.

	Tests
	There are three types of test:
		* Files named as "testname.in" and its pair "testname.expected":
		  The "testname.expeted" file have the rules that should be in the output.
			So the "testname.expected" may have not all the rules of output, but all the
			rules in "testname.expected" must be in output respecting the expected order.
		* Files named as "testname.rejected":
		  Each block between "-----" represents a test case that should be rejected.
		* Files named as "testname.accepted":
		  Each block between "-----" represents a test case that should be accepted.
	
	Changes in Lexical:
		* Doesn't include the minus signal in numbers tokens, so the expression "10-5"
		  can be interpreted as three tokens, instead of two.

First Delivery: Lexical
	Directory Structure
	 * lexical/src/ - Source files to run the lexical tests.
	 * lexical/build/ - Temporary files necessary to build the testing executable.
	 * lexical/build/dist - Distributions files. (The executable in our case)
	 * lexical/tests/ - Where the lexical tests are.


	Tasks
	The tasks were writen in python. To list all the commands run the script "scripts/lexical.py" with no parameters.

	Tests
	There are three types of test:
		* Files named as "testname.in": 
			* If exists a file named "testname.expected", the runner compare its content with the program output.
			* If not, our runner expects that the program rejects the input.
		* Files named as "testname.rejected":
			* Each line represents a test case that should be rejected.

Technologies used
  * yacc 1.9
  * lex 2.5.37
  * python 2.7
