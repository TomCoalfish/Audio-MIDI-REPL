swig -lua -c++ -Iinclude src/chorus.i
gcc -Iinclude -O2 -fPIC -march=native -mavx2 -shared -o chorus.so src/chorus_wrap.cxx -lstdc++ -lm -lluajit
