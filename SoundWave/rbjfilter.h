#pragma once

#include <stddef.h>
#include <math.h>

typedef struct 
{
    double a0,a1,a2;
    double b0,b1,b2;    
    double x0,x1;
    double y0,y1;
}
RBJFilter;

#ifdef __cplusplus
extern "C" {
#endif

double tick(RBJFilter * filter, double in);
double compute_w0(double f, double sr);
void lpf(RBJFilter * filter, double w0, double Q);
void hpf(RBJFilter * filter, double w0, double Q);
void bpf_skirt(RBJFilter * filter, double w0, double Q);
void bpf(RBJFilter * filter, double w0, double Q);
void notch(RBJFilter * filter, double w0, double Q);
void apf(RBJFilter * filter, double w0, double Q);
void peak(RBJFilter * filter, double w0, double Q, double gain);
void lowshelf(RBJFilter * filter, double w0, double Q, double gain);
void highshelf(RBJFilter * filter, double w0, double Q, double gain);
void process(RBJFilter * p, size_t n, float * in, float * out);

#ifdef __cplusplus
}
#endif
