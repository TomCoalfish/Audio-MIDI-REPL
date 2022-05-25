#include <audiosystem.h>
#include <cstdio>
#include <cstring>
#include <string>
#include <cassert>
#include <signal.h>
#include <SoundWave.h>
#include <chorus.h>
#include <map>
#include <functional>
#include <memory>
#include <functional>
//#include <luajit.h>

using std::string;
using std::map;
using std::function;

using namespace SoundWave;

constexpr int max_voices = 16;

struct LadsaPlugin
{

};

struct Lv2Plugin
{

};

struct PluginList
{

    // insertFront
    // insertBack
    // insertBeforePlugin
    // insertAfterPlugin
};

struct DSP
{
    map<std::string,float> controls;
    map<std::string,function<void (float)>> control_functions;

    std::vector<float> in,out;
    size_t channels;
    float  sample_rate;

    void  setControl(string & name, float value);
    float getControl(string & name);
};

struct DSPList
{
    // insertFront
    // insertBack
    // insertBeforePlugin
    // insertAfterPlugin
};


class OnePole {
public:
    OnePole() {a0 = 1.0; b1 = 0.0; z1 = 0.0;};
    OnePole(double Fc) {z1 = 0.0; setFc(Fc);};
    ~OnePole() = default;

    void setFc(double Fc);
    void setHighPass(double Fc);
    float process(float in);

protected:
    double a0, b1, z1;
};

// low pass
inline void OnePole::setFc(double Fc) {
    b1 = exp(-2.0 * M_PI * Fc);
    a0 = 1.0 - b1;
}

inline void OnePole::setHighPass(double Fc) {
    b1 = -exp(-2.0 * M_PI * (0.5 - Fc));
    a0 = 1.0 + b1;
}

inline float OnePole::process(float in) {
    return z1 = in * a0 + z1 * b1;
}

template<typename T>
void zero(size_t n, std::vector<T> & v)
{
    for(size_t i = 0; i < n; i++) v[i] = T(0);
}

template<typename T>
inline std::vector<T> linear_convolution(size_t n, T * h, T * x) {
    std::vector<T> out(n);
    zero(n,out);
    for(size_t i = 0; i < n; i++)
        for(size_t j = 0; j < n; j++)
            out[i] += h[j]*x[i];
    return out;
}

template<typename T>
inline void inplace(std::vector<T> & vec, std::function<T (T)> func)
{
    for(size_t i = 0; i < vec.size(); i++) vec[i] = func(vec[i]);
}
template<typename T>
inline void inplace_multiply(size_t n, std::vector<T> & vec, std::function<T (T)> func)
{
    for(size_t i = 0; i < vec.size(); i++) vec[i] *= func(vec[i]);
}
template<typename T>
inline void inplace_add(size_t n, std::vector<T> & vec, std::function<T (T)> func)
{
    for(size_t i = 0; i < vec.size(); i++) vec[i] += func(vec[i]);
}
template<typename T>
inline void inplace_subtract(size_t n, std::vector<T> & vec, std::function<T (T)> func)
{
    for(size_t i = 0; i < vec.size(); i++) vec[i] -= func(vec[i]);
}
template<typename T>
inline void inplace_divide(size_t n, std::vector<T> & vec, std::function<T (T)> func)
{
    for(size_t i = 0; i < vec.size(); i++) vec[i] /= func(vec[i]);
}

template<typename T>
inline void inplace(size_t n, T * vec, std::function<T (T)> func)
{
    for(size_t i = 0; i < n; i++) vec[i] = func(vec[i]);
}
template<typename T>
inline void inplace_multiply(size_t n, T * vec, std::function<T (T)> func)
{
    for(size_t i = 0; i < n; i++) vec[i] *= func(vec[i]);
}
template<typename T>
inline void inplace_add(size_t n, T * vec, std::function<T (T)> func)
{
    for(size_t i = 0; i < n; i++) vec[i] += func(vec[i]);
}
template<typename T>
inline void inplace_subtract(size_t n, T * vec, std::function<T (T)> func)
{
    for(size_t i = 0; i < n; i++) vec[i] -= func(vec[i]);
}
template<typename T>
inline void inplace_divide(size_t n, T * vec, std::function<T (T)> func)
{
    for(size_t i = 0; i < n; i++) vec[i] /= func(vec[i]);
}

template<typename T>
inline void to_stereo(size_t n, const T * in, T * out)
{
    for(size_t i = 0; i < n; i++)
    {
        float r = in[i];
        *out++ = r;
        *out++ = r;
    }
}


struct SimpleSynth
{
    BandlimitedOscillator *osc1,*osc2;
    MoogLadderFilter      *filter;
    LFO                   *lfo;
    ADSR                  *env1,*env2;
    PinkNoiseGenerator    *noise;
    Chorus                *chorus;
    OnePole                filter_cutoff_smooth;
    OnePole                filter_resonance_smooth;
    float                  osc1_tune,osc2_tune;
    float                  velocity;
    float                  cv;
    float                  osc_mix;
    float                  noise_amount;
    float                  output_gain;
    bool                   chorus_enable;
    float                  lfo_amount;

    SimpleSynth(float sr, BandlimitedOscillator::Waveform waveform = BandlimitedOscillator::Waveform::SINE, float freq=440.0f) {
        osc1 = new BandlimitedOscillator(sr,waveform,freq);
        osc2 = new BandlimitedOscillator(sr,waveform,freq);
        osc1_tune = 0;
        osc2_tune = 0;
        cv = 0;
        filter = new MoogLadderFilter(FINN_MOOG,sr);
        output_gain = 1.0;
        noise = new PinkNoiseGenerator();
        //lfo   = new LFO(4096,LFO::WaveType::SINE, 0.5, sr);
        env1  = new ADSR();
        env2  = new ADSR();
        env1->setAttackRate(0.1*sr);
        env1->setDecayRate(0.1*sr);
        env1->setSustainLevel(1.0);
        env1->setReleaseRate((0.1*sr));
        env2->setAttackRate(0.1*sr);
        env2->setDecayRate(0.1*sr);
        env2->setSustainLevel(1.0);
        env2->setReleaseRate((0.1*sr));
        chorus = new Chorus(sr);
        filter->SetCutoff(0.1f);
        filter->SetResonance(0.01f);
        chorus->setCurrentDepth(1.0);
        chorus->setCurrentWidth(20.0);
        chorus->setCurrentDelay(30.0);
        chorus->setNumVoices(5);
        chorus->setFrequency(1.5);
        chorus->setInterpolation(Chorus::interpolationLinear);
    }
    ~SimpleSynth() {
        if(osc1) delete osc1;
        if(osc2) delete osc2;
    }

    void setLfoFrequency(float f) {
        lfo->setFrequency(f);
    }
    void setLfoCv(float cv) {
        lfo->setFrequency( std::pow(2.0, cv));
    }
    void setOsc1TuneCv(float cv) {
        osc1_tune = cv;
    }
    void setOsc2TuneCv(float cv) {
        osc1_tune = cv;
    }
    void setFilterCutoff(float cutoff) {
        filter->SetCutoff(cutoff);
    }
    void setFilterResonance(float resonance) {
        filter->SetResonance(resonance);
    }
    void setFilterCutoffCv(float cv) {
        filter->SetCutoff( std::pow(2.0,cv) );
    }
    void setFilterResonanceCv(float cv) {
        filter->SetResonance(std::pow(2.0,cv));
    }

    void setCV(float voltage) {
        cv = voltage;
        osc1->setFrequency( std::pow(2, cv + osc1_tune));
        osc2->setFrequency( std::pow(2, cv + osc2_tune));
    }
    void noteOn(float freq) {
        cv = log2(freq);
        osc1->setFrequency(freq + freq*std::pow(2,osc1_tune));
        osc2->setFrequency(freq + freq*std::pow(2,osc2_tune));
        filter->SetCutoff(freq);
        env1->gate(true);
        env2->gate(true);
    }
    void noteOff(float f) {
        env1->gate(false);
        env2->gate(false);
    }
    float Tick() {
        float o =  (osc1->Tick() + osc2->Tick());
        filter->Process(&o,1);
        return env1->Tick() * o * env2->Tick() * output_gain;
    }
    void Process(size_t n, float * samples)
    {
        std::vector<float> temp(n);
        std::vector<float> stereo(n*2);
        zero(n*2,stereo);
        inplace(n,temp.data(),std::function<float (float)>([&](float x) { return osc1->Tick() + osc2->Tick(); }));
        filter->Process(temp.data(),n);
        inplace_multiply(n,temp.data(),std::function<float(float)>([&](float x) { return env2->Tick() * output_gain; }));
        to_stereo(n,temp.data(),stereo.data());
        chorus->Process(n,stereo.data(),samples);
    }
};

struct Synthesizer
{
    SimpleSynth * voices[max_voices];
    bool          voice_map[max_voices];
    float         voice_freq[max_voices];

    Synthesizer(float sr) {
        for(size_t i = 0; i < max_voices; i++)
        {
            voices[max_voices] = new SimpleSynth(sr);
            voice_map[max_voices] = false;
        }
    }
    ~Synthesizer() {
        for(size_t i = 0; i < max_voices; i++) if(voices[i]) delete voices[i];
    }

    void setCutoff(float c) {
        for(size_t i = 0; i < max_voices; i++) voices[i]->setFilterCutoff(c);
    }
    void setResonance(float r) {
       for(size_t i = 0; i < max_voices; i++) voices[i]->setFilterResonance(r);
    }
    void noteOn(float freq) {
        bool setVoice = false;
        for(size_t i = 0; i < max_voices; i++)
            if(voice_map[i] == false) {
                setVoice = true;
                voice_map[i] = true;
                voice_freq[i] =freq;
                voices[i]->noteOn(freq);
            }
        if(!setVoice) {
            voices[max_voices-1]->noteOn(freq);
            voice_freq[max_voices-1] = freq;
        }
    }
    void noteOff(float freq) {
        for(size_t i = 0; i < 16; i++)
        {
            if( std::fabs((freq - voice_freq[i])) < 1e-6 && voice_map[i] == true)
            {
                voice_freq[i] = 0;
                voice_map[i] = false;
                voices[i]->noteOff(freq);
            }
        }
    }
};
SimpleSynth *  synth;

void set_osc1_tune(float tune) {
    synth->osc1_tune = tune;
}
void set_osc2_tune(float tune) {
    synth->osc2_tune = tune;
}
void noteOn(float freq) {
    synth->noteOn(freq);
}
void noteOff(float freq) {
    synth->noteOff(freq);
}
void setCv(float v) {
    synth->setCV(v);
}
void setLfoFreq(float f) {
    synth->setLfoFrequency(f);
}

int note_on(MidiMsg * msg) {
    float f = std::pow(2.0, (((double)msg->data1)-69.0)/12.0)*440.0f;
    synth->noteOn(f);
    synth->velocity = ((float)msg->data2)/127.0f;
    return 0;
}
int note_off(MidiMsg * msg) {
    float f = std::pow(2.0, (((double)msg->data1)-69.0)/12.0)*440.0f;
    synth->noteOff(f);
    return 0;
}

int audio_callback(const void *inputBuffer, void *outputBuffer, unsigned long framesPerBuffer,
                    const PaStreamCallbackTimeInfo* timeInfo, PaStreamCallbackFlags statusFlags, void *userData)
{
    SimpleSynth * synth = (SimpleSynth*)userData;
    float * in = (float*)inputBuffer;
    float * out= (float*)outputBuffer;
    synth->Process(framesPerBuffer,out);
    return paContinue;
}

// luajit
// load_plugin
// unload_plugin
// insert_plugin(FRONT,BACK)
// insert_plugin_after
// insert_plugin_before
// set_filter_cutoff
// set_filter_resonance
// set_env1_attack
// set_amplifier_gain

int main(int argc, char * argv[])
{
    Init();
    Pm_Initialize();

    set_note_on_callback(note_on);
    set_note_off_callback(note_off);

    int num_midi_device = GetNumMidiDevices();
    for(size_t i = 0; i < num_midi_device; i++)
        printf("Device #%lu : %s\n",i,GetMidiDeviceName(i));

    set_audio_callback(audio_callback);
    int device=14;

    Pa_Initialize();
    int num_audio_device = GetNumAudioDevices();
    for(size_t i = 0; i < num_audio_device; i++)
    {
        string s = GetAudioDeviceName(i);
        if(s == "pulse") device = i;
        printf("Audio Device#%lu: %s\n",i,s.c_str());
    }

    synth = new SimpleSynth(44100);
    //synth->noteOn(220.0f);
    InitMidiDevice(1,3,3);
    InitAudioDevice(device,-1,2,44100,256,synth);
    RunAudio();
    StopAudio();

}