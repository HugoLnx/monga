Students
 * Hugo Roque  - 1311162
 * Rômulo Dusi - 1213094

Final Delivery: Code generator
  Tests Runner
		The script "scripts/run_tests.sh" execute all five test suites. (lexical, syntax, ast-tree, ids-and-types, monga).

	Runner
	  The script "scripts/monga.py" have the following commands:
			* run: Generate the assembly code to a monga code.
			* exec: Generate, compile and execute the assembly code, from the monga code.
			* test_all: Run all test cases.
			* test: Run an single test case.

		Examples:
		  * ./scripts/monga.py run tests/simple.in
		  * ./scripts/monga.py exec tests/simple.in
		  * ./scripts/monga.py test_all
		  * ./scripts/monga.py test tests/simple.in
	
	Overview
	  Abstract Syntax Tree creation
		  The module ast_tree have all the structures to the tree and also the functions to create it,
			that functions are used by yacc to generate the AST.

		Ids and types - Identification binding
			After the AST are created, the module ast_ids traverse it in pre-order to bind variables
			and function calls to its declarations. It also validate some problems in the tree, like
			variables that wasn't declared.

			In that phase, we use the scopes stack module, to search the variables declaration in the
			current or above scopes.

		Ids and types - Types resolving
		  After all variables and functions are referencing to its declarations, we pass to the phase of
			resolving the types of var and expressions.

		  To do that the ast_types module traverse the AST in post-order, resolving the types from
			the "bottom" of the tree. The benefit of this strategy is that when visiting an node, the types
			of its children were already defined, so we can easily define the type of the current node.

			Besides resolving types, we do some verification about the types like, if the expression type
			its compatible with the variable assigned, or if only integers are used as array indexes.

		Monga - Code generator
		  Given that the types are also resolved, we start the phase of code generation. In order to do that
			we traverse the AST in pre-order generating the assembly code.

	
	Changelog:
		* Module created: ast_post_traversing - Encapsulates the logic of traversing the AST in post-order.
		* Feature: Linking function to its declaration.
		* Feature: Validating if two variables have the same name in the same context.
		* Feature: Validating if two functions have the same name.
		* Feature: Validating function parameters.
		* Feature: Expression type are defined based on its children.
		* Feature: Assembly code generation.

	Tests
	There are three types of test:
		* Files named as "testname.in" and its pair "testname.expected":
		  The "testname.expeted" file have the rules that should be in the output.
			So the "testname.expected" may have not all the rules of output, but all the
			rules in "testname.expected" must be in output respecting the expected order.
		* Files named as "testname.rejected":
		  Each block between "-----" represents a test case that should be rejected.

	Could not implement in time:
	  * Code generation support for floats.
	  * Function call expression as pointer to array.



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
