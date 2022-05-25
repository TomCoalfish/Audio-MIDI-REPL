swig -lua -c++ thread.i
gcc -fmax-errors=1  -O2 -fPIC -shared -o thread.so thread_wrap.cxx -lstdc++ -lluajit -lpthread
