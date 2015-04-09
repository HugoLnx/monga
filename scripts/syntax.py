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

BUILD = os.path.join(ROOT, "build")
TMP = os.path.join(ROOT, "tmp")
DIST = os.path.join(BUILD, "dist")

SRC = os.path.join(ROOT, "src")
TESTS = os.path.join(ROOT, "tests")

def clear():
  os.system("rm -rf " + BUILD)
  os.system("rm -rf " + TMP)
  os.system("rm -rf " + DIST)
  os.system("mkdir -p " + BUILD)
  os.system("mkdir -p " + TMP)
  os.system("mkdir -p " + DIST)
  return

def run(input_name):
  compile()
  print execute(input_name)
  return

def test(type_test):
  compile()
  error_msg = execute_case(type_test)
  if error_msg:
    print error_msg
  else:
    print "The test case passed."

def test_all():
  compile()
  list_files = []
  fails = 0
  for file in os.listdir(TESTS):
    if file.endswith(".in") or file.endswith(".rejected"):
      list_files.append(file)
      error_msg = execute_case(os.path.join(TESTS,file))
      if error_msg:
        print "Failing test: " + file.split("/")[-1]
        print error_msg
        print
        fails += 1
  print "Result: " + str(len(list_files) - fails) + " of " + str(len(list_files)) + " test cases passed."

def critical_sys(cmd):
  outcode = os.system(cmd)
  if outcode != 0:
    exit()

def compile():
  clear()
  os.system("cp " + os.path.join(SRC, "monga.lex") + " " + BUILD)
  os.system("cp " + os.path.join(SRC, "monga.yacc") + " " + BUILD)
  os.system("cp " + os.path.join(SRC, "main.c") + " " + BUILD)
  os.system("cp " + os.path.join(SRC, "main.h") + " " + BUILD)
  os.system("cp " + os.path.join(SRC, "monga.yacc") + " " + BUILD)
  critical_sys("lex -o " + os.path.join(BUILD, "lex.yy.c") + " " + os.path.join(BUILD, "monga.lex"))
  os.chdir("build")
  critical_sys("yacc -d -i -o 'y.tab.c' " + os.path.join(BUILD, "monga.yacc"))
  os.chdir("..")
  
  critical_sys("gcc " + os.path.join(BUILD, "main.c") + " -o " + os.path.join(BUILD,"main.o") + " -c")
  critical_sys("gcc " + os.path.join(BUILD,"y.tab.c") + " " + os.path.join(BUILD,"lex.yy.c") + " " + os.path.join(BUILD,"main.o") + " -o " + os.path.join(DIST,"main"))
  return

def execute(input_path):
  import commands
  output = commands.getoutput(os.path.join(DIST,"main") + " < " + input_path)
  return output

def execute_content(content):
  tmp_path = os.path.join(TMP, "temp.in")
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