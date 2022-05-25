swig -lua -c++ -Iinclude src/sample.i
gcc -Iinclude -O2 -fPIC -march=native -mavx2 -shared -o sample.so src/sample_wrap.cxx -lstdc++ -lm -lluajit
