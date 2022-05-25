swig -lua -c++ -Iinclude src/adsr.i
gcc -Iinclude -O2 -fPIC -march=native -mavx2 -fPIC -shared -o adsr.so src/adsr_wrap.cxx src/ADSR.cpp -lstdc++ -lm -lluajit
