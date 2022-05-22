%module thread
%{
#include <pthread.h>
#include <unistd.h>
%}
%include "lua_fnptr.i"
%inline %{
    struct Mutex {
        pthread_mutex_t mutex;

        Mutex() {
            pthread_mutex_init(&mutex,NULL);
        }
        ~Mutex() {
            pthread_mutex_destroy(&mutex);
        }

        void lock() {
            pthread_mutex_lock(&mutex);
        }
        void unlock() {
            pthread_mutex_unlock(&mutex);
        }
    };

    Mutex mutex;

    volatile int run = 0;
    volatile int end_thread = 0;
    
    void *run_thread(void * ptr);

    struct Thread
    {
        SWIGLUA_REF ref;
        pthread_t thread;

        Thread(SWIGLUA_REF cb) { 
            pthread_create(&thread,NULL,*run_thread,(void*)this);
            ref = cb; 
        }
        ~Thread() {
            end_thread = 1;
            pthread_join(thread,NULL);            
        }
        void Start() {
            run = 1;
        }
        void Stop() {
            run = 0;
        }
    };


    void *run_thread(void * ptr) {
        Thread * t = (Thread*)ptr;
        while(!end_thread) {
            if(run == 1) {
                mutex.lock();
                swiglua_ref_get(&t->ref);
                if(lua_pcall(t->ref.L,0,0,0) != 0) 
                    printf("%s\n",lua_tostring(t->ref.L,-1));
                mutex.unlock();
            }
            usleep(10);
        }
        return ptr;
    }
%}