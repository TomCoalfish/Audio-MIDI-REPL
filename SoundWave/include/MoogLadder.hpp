
//------------------------------------------------------------------------------
// This file was generated using the Faust compiler (https://faust.grame.fr),
// and the Faust post-processor (https://github.com/jpcima/faustpp).
//
// Source: moogladder.dsp
// Name: MoogLadder
// Author: Christopher Arndt
// Copyright: 
// License: MIT-style STK-4.3 license
// Version: 
//------------------------------------------------------------------------------






#pragma once
#ifndef MoogLadder_Faust_pp_Gen_HPP_
#define MoogLadder_Faust_pp_Gen_HPP_

#include <memory>

class MoogLadder {
public:
    MoogLadder();
    ~MoogLadder();

    void init(float sample_rate);
    void clear() noexcept;

    void process(
        const float *in0,
        float *out0,
        unsigned count) noexcept;

    enum { NumInputs = 1 };
    enum { NumOutputs = 1 };
    enum { NumActives = 2 };
    enum { NumPassives = 0 };
    enum { NumParameters = 2 };

    enum Parameter {
        p_cutoff,
        p_q,
        
    };

    struct ParameterRange {
        float init;
        float min;
        float max;
    };

    static const char *parameter_label(unsigned index) noexcept;
    static const char *parameter_short_label(unsigned index) noexcept;
    static const char *parameter_symbol(unsigned index) noexcept;
    static const char *parameter_unit(unsigned index) noexcept;
    static const ParameterRange *parameter_range(unsigned index) noexcept;
    static bool parameter_is_trigger(unsigned index) noexcept;
    static bool parameter_is_boolean(unsigned index) noexcept;
    static bool parameter_is_integer(unsigned index) noexcept;
    static bool parameter_is_logarithmic(unsigned index) noexcept;

    float get_parameter(unsigned index) const noexcept;
    void set_parameter(unsigned index, float value) noexcept;

    
    float get_cutoff() const noexcept;
    
    float get_q() const noexcept;
    
    
    void set_cutoff(float value) noexcept;
    
    void set_q(float value) noexcept;
    
    float Tick(float in) {
        float r = 0.0;
        process(&in,&r,1);
        return r;
    }
    void Process(size_t n, float * input, float * output) {
        process(input,output,n);
    }
    void Process(float * samples, size_t n) {
        process(samples,samples,n);
    }
public:
    class BasicDsp;

private:
    std::unique_ptr<BasicDsp> fDsp;



};




#endif // MoogLadder_Faust_pp_Gen_HPP_