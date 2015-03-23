ROOT = File.dirname(__FILE__)

BUILD = File.join(ROOT, "build")
TMP_SRC = BUILD
TMP_COMPILED = BUILD
DIST = File.join(BUILD, "dist")

SRC = File.join(ROOT, "src")
TESTS = File.join(ROOT, "tests")

desc "rake clear - Clear build files"
task :clear do
  system(%Q[
    sudo rm -rf #{File.join(BUILD, "*")}
    mkdir -p #{TMP_SRC}
    mkdir -p #{TMP_COMPILED}
    mkdir -p #{DIST}
  ])
end

desc "rake compile - Compile all files"
task :compile => [:clear] do
  system(%Q[
    lex --header-file=#{File.join(TMP_SRC, 'lex.yy.h')} -o #{File.join(TMP_SRC, "lex.yy.c")} #{File.join(SRC, "monga.lex")}
    gcc -ll #{File.join(TMP_SRC,"lex.yy.c")} -o #{File.join(TMP_COMPILED,"lex.yy.o")} -c
    gcc #{File.join(SRC, "main.c")} -o #{File.join(TMP_COMPILED,"main.o")} -c
    gcc #{File.join(TMP_COMPILED,"main.o")} #{File.join(TMP_COMPILED,"lex.yy.o")} -o #{File.join(DIST,"main")}
  ])
end

desc "rake run <input-file> - Run with one input"
task :run => [:compile] do
  system(%Q[
    #{File.join(DIST,"main")} < #{ARGV[1]}
  ])
end
