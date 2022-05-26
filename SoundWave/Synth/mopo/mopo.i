%module mopo
%{
#include "mopo.h"
using namespace mopo;
%}

%ignore cr;

%include "processor.h"
%include "processor_router.h"
%include "value.h"
%include "utils.h"
%include "note_handler.h"
%include "alias.h"
%include "arpeggiator.h"
%include "bit_crush.h"
%include "biquad_filter.h"
%include "bypass_router.h"
%include "circular_queue.h"
%include "common.h"
%include "delay.h"
%include "distortion.h"
%include "envelope.h"
%include "feedback.h"
%include "formant_manager.h"
%include "linear_slope.h"
%include "magnitude_lookup.h"
%include "memory.h"
%include "midi_lookup.h"
%include "mono_panner.h"
%include "operators.h"
%include "oscillator.h"
%include "portamento_slope.h"
%include "resonance_lookup.h"
%include "reverb.h"
%include "reverb_all_pass.h"
%include "reverb_comb.h"
%include "simple_delay.h"
%include "smooth_filter.h"
%include "smooth_value.h"
%include "state_variable_filter.h"
%include "step_generator.h"
%include "stutter.h"
%include "tick_router.h"
%include "trigger_operators.h"
%include "voice_handler.h"
%include "wave.h"

%inline %{
    void setSampleRate(mopo::Processor* p, int sample_rate) { p->setSampleRate(sample_rate); }
    void setBufferSize(mopo::Processor* p, int buffer_size) { p->setBufferSize(buffer_size); }
    void setControlRate(mopo::Processor* p, bool control_rate) { p->setControlRate(control_rate); }
    bool isEnabled(mopo::Processor *p) { return p->enabled(); }
    void enable(mopo::Processor * p, bool e) { p->enable(e); }
    int getSampleRate(mopo::Processor * p) { return p->getSampleRate(); }
    int getBufferSize(mopo::Processor * p) { return p->getBufferSize(); }
    int getSamplesToProcess(mopo::Processor * p) { return p->getSamplesToProcess(); }
    bool isControlRate(mopo::Processor * p) { return p->isControlRate(); }
    bool inputMatchesBufferSize(mopo::Processor * p, int input=0) { return p->inputMatchesBufferSize(input); }
    bool isPolyphonic(mopo::Processor * p) { return p->isPolyphonic(); }
    void plug(mopo::Processor * p, mopo::Output * o) { p->plug(o); }
    void plug(mopo::Processor * p, mopo::Output * o, unsigned int index) { p->plug(o,index); }
    void plug(mopo::Processor * p, mopo::Processor * o) { p->plug(o); }
    void plug(mopo::Processor * p, mopo::Processor * o, unsigned int index) { p->plug(o,index); }
    void plugNext(mopo::Processor * p, mopo::Output * o) { p->plugNext(o); }
    void plugNext(mopo::Processor * p, mopo::Processor * o) { p->plugNext(o); }
    int  getConnectedInputs(mopo::Processor * p) { return p->connectedInputs(); }
    void unplugIndex(mopo::Processor * p, unsigned index) { p->unplugIndex(index); }
    void unplug(mopo::Processor * p, mopo::Output * o) { p->unplug(o); }
    void unplug(mopo::Processor * p, mopo::Processor * o) { p->unplug(o); }
    mopo::ProcessorRouter* getRouter(mopo::Processor * p) { return p->router(); }
    void setRouter(mopo::Processor * p, mopo::ProcessorRouter* r) { p->router(r); }
    mopo::ProcessorRouter* getTopLevelRouter(mopo::Processor * p) { return p->getTopLevelRouter(); }
    void registerInput(mopo::Processor * p, mopo::Input * i, int index) { p->registerInput(i,index); }
    mopo::Output* registerOutput(mopo::Processor * p, mopo::Output * output, int index) {return p->registerOutput(output, index); }
    void registerInput(mopo::Processor * p, mopo::Input * i) { p->registerInput(i); }
    mopo::Output* registerOutput(mopo::Processor * p, mopo::Output * output) {return p->registerOutput(output); }
    int getNumInputs(mopo::Processor * p) { return p->numInputs(); }
    int getNumOutputs(mopo::Processor * p) { return p->numOutputs(); }
    float getInputSample(mopo::Processor * p, int input, int sample) { return p->getInputSample(input,sample); }
    void  writeOutputSample(mopo::Processor * p, int output, int sample, float value) { return p->writeOutputSample(output,sample,value); }
    mopo::Input* getInput(mopo::Processor * p,unsigned int index=0) { return p->input(index); }
    mopo::Output* getOutput(mopo::Processor * p,unsigned int index=0) { return p->output(index); }
%}