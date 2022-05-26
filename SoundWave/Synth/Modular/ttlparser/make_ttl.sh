swig -lua -c++ -Iinclude turtle_parser.i
gcc -Iinclude -O2 -fPIC -march=native -mavx2 -shared -o turtle_parser.so turtle_parser_wrap.cxx lib/libttlparser.a -lstdc++ -lm -lluajit
