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
IDS_ROOT = os.path.join(ROOT, "ids-and-types")

BUILD = os.path.join(IDS_ROOT, "build")
TMP = os.path.join(IDS_ROOT, "tmp")
DIST = os.path.join(BUILD, "dist")

SRC = os.path.join(ROOT, "src")
IDS_SRC = os.path.join(IDS_ROOT, "src")
TREE_SRC = os.path.join(ROOT, "ast-tree", "src")
TESTS = os.path.join(IDS_ROOT, "tests")

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
    if file.endswith(".in") or file.endswith(".rejected") or file.endswith(".accepted"):
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
  os.system("cp " + os.path.join(IDS_SRC, "main.c") + " " + BUILD)
  os.system("cp " + os.path.join(IDS_SRC, "main.h") + " " + BUILD)
  os.system("cp " + os.path.join(SRC, "ast_tree.h") + " " + BUILD)
  os.system("cp " + os.path.join(SRC, "ast_tree.c") + " " + BUILD)
  os.system("cp " + os.path.join(SRC, "list.h") + " " + BUILD)
  os.system("cp " + os.path.join(SRC, "list.c") + " " + BUILD)
  os.system("cp " + os.path.join(SRC, "utils.h") + " " + BUILD)
  os.system("cp " + os.path.join(SRC, "utils.c") + " " + BUILD)
  os.system("cp " + os.path.join(SRC, "ast_pre_traversing.h") + " " + BUILD)
  os.system("cp " + os.path.join(SRC, "ast_pre_traversing.c") + " " + BUILD)
  os.system("cp " + os.path.join(SRC, "ast_post_traversing.h") + " " + BUILD)
  os.system("cp " + os.path.join(SRC, "ast_post_traversing.c") + " " + BUILD)
  os.system("cp " + os.path.join(TREE_SRC, "ast_print.h") + " " + BUILD)
  os.system("cp " + os.path.join(TREE_SRC, "ast_print.c") + " " + BUILD)
  os.system("cp " + os.path.join(SRC, "ast_ids.h") + " " + BUILD)
  os.system("cp " + os.path.join(SRC, "ast_ids.c") + " " + BUILD)
  os.system("cp " + os.path.join(SRC, "ast_types.h") + " " + BUILD)
  os.system("cp " + os.path.join(SRC, "ast_types.c") + " " + BUILD)
  os.system("cp " + os.path.join(SRC, "stack.c") + " " + BUILD)
  os.system("cp " + os.path.join(SRC, "stack.h") + " " + BUILD)
  os.system("cp " + os.path.join(SRC, "error_reporter.c") + " " + BUILD)
  os.system("cp " + os.path.join(SRC, "error_reporter.h") + " " + BUILD)
  critical_sys("lex -o " + os.path.join(BUILD, "lex.yy.c") + " " + os.path.join(BUILD, "monga.lex"))
  current_dir = os.getcwd()
  os.chdir(BUILD)
  critical_sys("yacc -d -i -v -o 'y.tab.c' " + os.path.join(BUILD, "monga.yacc"))
  os.chdir(current_dir)
  
  compile_main(["main", "list", "stack", "utils", "ast_tree", "ast_pre_traversing", "ast_post_traversing", "ast_print", "ast_ids", "ast_types", "error_reporter"])
  return

def compile_main(artefacts):
  compiling_paths = []
  compiling_paths.append(os.path.join(BUILD,"y.tab.c"))
  compiling_paths.append(os.path.join(BUILD,"lex.yy.c"))
  for artefact in artefacts:
    source_path = os.path.join(BUILD, artefact + ".c")
    object_path = os.path.join(BUILD, artefact + ".o")
    compiling_paths.append(object_path)
    critical_sys("gcc " + source_path + " -o " + object_path + " -c")
  critical_sys("gcc " + " ".join(compiling_paths) + " -o " + os.path.join(DIST,"main"))

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
  if re.search(r'.in$', test_path):
    return execute_normal_test(test_path)
  elif re.search(r'.rejected$', test_path):
    return execute_reject_test(test_path)
  elif re.search(r'.accepted$', test_path):
    return execute_accepted_test(test_path)

def execute_normal_test(test_path):
  test_output = execute(test_path)
  expected_path = test_path[:-3] + ".expected"
  expected = (open(expected_path, 'r') if os.path.exists(expected_path) else None)
  return failing_msg(test_output, expected)

def is_rejected(output):
  matching = re.search(r'^([eE]rror:|Segment)', output)
  return matching

def is_not_rejected(output):
  matching = re.search(r'^Error:', output)
  return not matching

def execute_reject_test(test_path):
  return execute_multiline_case(test_path, is_not_rejected, "should be rejected but the output was")

def execute_accepted_test(test_path):
  return execute_multiline_case(test_path, is_rejected, "should be accepted but the output was")

def failing_msg(output, expected):
  outlines = output.split("\n")
  if expected:
    for index,line in enumerate(expected):
      if line.strip() != outlines[index].strip():
        return "Difference on line " + str(index+1) + ": \
          \n  Expected: " + (line.strip() if line else "<no-line>") + "\
          \n  Output: " + str(outlines[index].strip())
    return None

def execute_multiline_case(test_path, rejection_condition, middle_msg):
  msgs = []
  file_test = open(test_path, 'r')
  file_content = file_test.read()
  file_test.close()
  for content in file_content.split("-----"):
    content = content.strip()
    test_output = execute_content(content)
    if rejection_condition(test_output):
      msgs.append("Failing test: " + test_path + "\n>>>\n" + content + "\n" + middle_msg + "\n" + test_output + "\n<<<\n")
  if len(msgs) == 0:
    return None
  else:
    return "\n".join(msgs)


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
