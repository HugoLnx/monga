#!/usr/bin/python
import os
import sys
import re
import signal
import subprocess
from pprint import pprint
 
# Get the total number of args passed to the demo.py
total = len(sys.argv)
# Get the arguments list 
cmdargs = sys.argv

ROOT = os.path.abspath('.')
SRC = os.path.join(ROOT, "src")

LEX_ROOT = os.path.join(ROOT, "lexical")

LEX_BUILD = os.path.join(LEX_ROOT, "build")
LEX_TMP = os.path.join(LEX_ROOT, "tmp")
LEX_DIST = os.path.join(LEX_ROOT, "dist")

LEX_SRC = os.path.join(LEX_ROOT, "src")
LEX_TESTS = os.path.join(LEX_ROOT, "tests")

REJECT_CASE_TEMPLATE = "92 ::test-case:: 29"


def clear():
  os.system("rm -rf " + LEX_BUILD)
  os.system("rm -rf " + LEX_TMP)
  os.system("rm -rf " + LEX_DIST)
  os.system("mkdir -p " + LEX_BUILD)
  os.system("mkdir -p " + LEX_TMP)
  os.system("mkdir -p " + LEX_DIST)
  return

def run(input_name):
  lex_test_compile()
  print execute(input_name)

def test(type_test):
  lex_test_compile()
  error_msg = execute_case(type_test)
  if error_msg:
    print error_msg
  else:
    print "The test case passed."

def test_all():
  lex_test_compile()
  list_files = []
  fails = 0
  for file in os.listdir(LEX_TESTS):
    if file.endswith(".in") or file.endswith(".rejected"):
      list_files.append(file)
      error_msg = execute_case(os.path.join(LEX_TESTS,file))
      if error_msg:
        print "Failing test: " + file.split("/")[-1]
        print error_msg
        print
        fails += 1
  print "Result: " + str(len(list_files) - fails) + " of " + str(len(list_files)) + " test cases passed."

def lex_test_compile():
  clear()
  os.system("cp " + os.path.join(SRC, "monga.lex") + " " + LEX_BUILD)
  os.system("cp " + os.path.join(LEX_SRC, "y.tab.h") + " " + LEX_BUILD)
  os.system("cp " + os.path.join(LEX_SRC, "main.c") + " " + LEX_BUILD)
  os.system("cp " + os.path.join(LEX_SRC, "main.h") + " " + LEX_BUILD)
  os.system("cp " + os.path.join(SRC, "utils.c") + " " + LEX_BUILD)
  os.system("cp " + os.path.join(SRC, "utils.h") + " " + LEX_BUILD)
  os.system("lex --header-file=" + os.path.join(LEX_BUILD, 'lex.yy.h') + " -o " + os.path.join(LEX_BUILD, "lex.yy.c") + " " + os.path.join(LEX_BUILD, "monga.lex"))
  os.system("gcc -ll " + os.path.join(LEX_BUILD,"lex.yy.c") + " -o " + os.path.join(LEX_BUILD,"lex.yy.o") + " -c")
  
  os.system("gcc " + os.path.join(LEX_BUILD, "main.c") + " -o " + os.path.join(LEX_BUILD,"main.o") + " -c")
  os.system("gcc " + os.path.join(LEX_BUILD, "utils.c") + " -o " + os.path.join(LEX_BUILD,"utils.o") + " -c")
  os.system("gcc " + os.path.join(LEX_BUILD,"utils.o") + " " + os.path.join(LEX_BUILD,"main.o") + " " + os.path.join(LEX_BUILD,"lex.yy.o") + " -o " + os.path.join(LEX_DIST,"main"))
  return

def execute(input_path):
  import commands
  output = commands.getoutput(os.path.join(LEX_DIST,"main") + " < " + input_path)
  return output

def execute_content(content):
  tmp_path = os.path.join(LEX_TMP, "temp.in")
  f = open(tmp_path, "w")
  f.write(content)
  f.close()
  return execute(tmp_path)

def execute_case(test_path):
  if ".in" in test_path:
    return execute_normal_test(test_path)
  else:
    return execute_reject_test(test_path)

def execute_normal_test(test_path):
  test_output = execute(test_path)
  expected_path = test_path[:-3] + ".expected"
  expected = (open(expected_path, 'r') if os.path.exists(expected_path) else None)
  return failing_msg(test_output, expected)

def execute_reject_test(test_path):
  msgs = []
  file_test = open(test_path, 'r')
  for line in file_test:
    content = REJECT_CASE_TEMPLATE.replace("::test-case::", line.strip())
    test_output = execute_content(content)
    if not is_rejected(test_output):
      msgs.append(content + " should be rejected but the output was: \n" + test_output)
  if len(msgs) == 0:
    return None
  else:
    return "\n".join(msgs)


def failing_msg(output, expected):
  outlines = output.split("\n")
  if expected:
    for index,line in enumerate(expected):
      if line.strip() != outlines[index].strip():
        return "Difference on line " + str(index) + ": \
          \n  Expected: " + (line.strip() if line else "<no-line>") + "\
          \n  Output: " + str(outlines[index].strip())
    return None
  else:
    if is_rejected(output):
      return None
    else:
      return "Expected to reject but the output was: " + output + "".strip()

def is_rejected(output):
  matching = re.search('INVALID,.*$', output)
  return matching


if (len(cmdargs) > 1):
  if (str(cmdargs[1]) == "clear"):
    clear()
  elif (str(cmdargs[1]) == "compile"):
    compile()
  elif (str(cmdargs[1]) == "run"):
    if len(cmdargs) > 2:
      run(cmdargs[2])
    else:
      print 'Missing <input-name>.'
  elif (str(cmdargs[1]) == "test"):
    if len(cmdargs) > 2:
      test(cmdargs[2])
    else:
      print 'Missing <test-path>.'
  elif (str(cmdargs[1]) == "test_all"):
    test_all()
  else:
    print 'No command found.\nclear - Clear build files\ncompile - Compile all files\nrun <input-name> - Run with one input\ntest <test-path> - Run with one test\ntest_all - Run all tests'
else: 
  print 'No command found.\nclear - Clear build files\ncompile - Compile all files\nrun <input-name> - Run with one input\ntest <test-path> - Run with one test\ntest_all - Run all tests'
