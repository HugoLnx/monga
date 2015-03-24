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

desc "rake test[<test-name>] - Run with one test"
task(:test, %w{test_name} => [:test_compile]) do |_, args|
  test_name = args[:test_name]
  test_output = %x[#{File.join(TEST_DIST,"main")} < #{File.join(TESTS, test_name)}.in]
  expected = File.read(File.join(TESTS, test_name) + ".out")
  if test_output == expected
    puts "Passou!"
  else
    puts "Não passou!"
  end
end

# PRIVATE

task :test_compile => [:compile] do
  system(%Q[
    gcc #{File.join(TESTS, "main.c")} -o #{File.join(TMP_COMPILED,"main.o")} -c
    gcc #{File.join(TMP_COMPILED,"main.o")} #{File.join(TMP_COMPILED,"lex.yy.o")} -o #{File.join(TEST_DIST,"main")}
  ])
end
