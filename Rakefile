ROOT = File.dirname(__FILE__)

BUILD = File.join(ROOT, "build")
TMP_SRC = BUILD
TMP_COMPILED = BUILD
DIST = File.join(BUILD, "dist")
TEST_DIST = File.join(BUILD, "test")

SRC = File.join(ROOT, "src")
TESTS = File.join(ROOT, "tests")

desc "rake clear - Clear build files"
task :clear do
  system(%Q[
    sudo rm -rf #{File.join(BUILD, "*")}
    mkdir -p #{TMP_SRC}
    mkdir -p #{TMP_COMPILED}
    mkdir -p #{TEST_DIST}
    mkdir -p #{DIST}
  ])
end

desc "rake compile - Compile all files"
task :compile => [:clear] do
  system(%Q[
    lex --header-file=#{File.join(TMP_SRC, 'lex.yy.h')} -o #{File.join(TMP_SRC, "lex.yy.c")} #{File.join(SRC, "monga.lex")}
    gcc -ll #{File.join(TMP_SRC,"lex.yy.c")} -o #{File.join(TMP_COMPILED,"lex.yy.o")} -c
  ])
end

desc "rake run <input-name> - Run with one input"
task(:run => [:test_compile]) do |_, args|
  puts execute ARGV[1]
end

desc "rake test <test-path> - Run with one test"
task(:test => [:test_compile]) do
  error_msg = execute_case ARGV[1]
  if error_msg.nil?
    puts "The test case passed."
  else
    puts error_msg
  end
end

desc "rake test_all - Run all tests"
task(:test_all => [:test_compile]) do
  test_cases = Dir["./tests/*.in"]
  fails = 0
  test_cases.each do |filepath|
    error_msg = execute_case filepath
    if error_msg
      test_name = File.basename filepath, ".in"
      puts "Failing test: '#{test_name}'"
      puts error_msg
      puts
      fails += 1
    end
  end
  passing_tests = test_cases.size - fails
  puts "Result: #{passing_tests} of #{test_cases.size} test cases passed."
end

# PRIVATE

task :test_compile => [:compile] do
  system(%Q[
    gcc #{File.join(TESTS, "main.c")} -o #{File.join(TMP_COMPILED,"main.o")} -c
    gcc #{File.join(TMP_COMPILED,"main.o")} #{File.join(TMP_COMPILED,"lex.yy.o")} -o #{File.join(TEST_DIST,"main")}
  ])
end

def execute(input_path)
  %x[#{File.join(TEST_DIST,"main")} < #{input_path}]
end

def execute_case(test_path)
  test_output = execute test_path
  expected_path = test_path.gsub(/\.in$/, ".expected")
  expected = File.exist?(expected_path) ? File.read(expected_path) : nil
  failing_msg(test_output, expected)
end

def failing_msg(output, expected)
  if expected
    expected = expected.lines
    output.lines.each_with_index do |line, i|
      if line != expected[i]
        return %Q{
Difference on line #{i+1}:
  Expected: #{expected[i].strip}
  Output:   #{line.strip}
        }.strip
      end
    end
    return nil 
  else
    matching = output.match /^INVALID,(.*)\Z/
    if matching.nil?
      return %Q[
Expected to reject but the output was:
#{output}
      ].strip
    else
      return nil
    end
  end
end
