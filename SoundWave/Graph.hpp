#include <vector>
#include <list>
#include <map>
#include <memory>
#include <iostream>
#include <string>

using std::map;
using std::vector;
using std::list;
using std::shared_ptr;
using std::string;
using std::cout;
using std::endl;

struct StereoSample
{
    float sample[2] = {0.0f,0.0f};

    StereoSample() = default;
    StereoSample(float left, float right) {
        sample[0] = left;
        sample[1] = right;
    }

    StereoSample& operator = (const StereoSample& sample) {
        this->sample[0] = samples.sample[0];
        this->sample[1] = samples.sample[1];
        return *this;
    }

    float operator[](uint32_t channel) { return sample[channel]; }
};

struct StereoSamples
{
    std::vector<float> samples[2];
    bool isMono = false;
    size_t len;

    StereoSamples() = default;
    StereoSamples(size_t len, bool mono = false) {
        samples[0].resize(len);
        samples[1].resize(len);
        isMono = mono;
        this->len = len;
    }

    StereoSamples& operator = (const StereoSamples & s) {
        samples = s.samples;
        return *this;
    }
    std::vector<float>& get_channel(uint32_t c) { return samples[c]; }
    std::vector<float>& get_left_channel() { return samples[0]; }
    std::vector<float>& get_right_channel() { return samples[1]; }

    float& operator()(uint32_t c, uint32_t pos) { return samples[c][pos]; }
    std::vector<float>& operator[](uint32_t channel) { return samples[channel]; }

    size_t size() const { return len; }
    bool   isStereo() const { return isMono == false; }
};

struct ControlValue
{
    float value = 0.0f;

    ControlValue() = default;

    void SetValue(const float v) { value = v; }
    float GetValue() { return value; }

};

struct PortConnection
{    
    StereoSamples samples;
    
    PortConnect(size_t len) : samples(len) {}

    PortConnection& operator = (const PortConnection & c) {
        samples = c.samples;
        return *this;
    }

    std::vector<float>& get_channel(uint32_t c) { return samples[c]; }
    std::vector<float>& get_left_channel() { return samples[0]; }
    std::vector<float>& get_right_channel() { return samples[1]; }

    float& operator()(uint32_t c, uint32_t pos) { return samples[c][pos]; }
    std::vector<float>& operator[](uint32_t channel) { return samples[channel]; }   

    void SetSamples(StereoSamples & s) { samples = s; }

    void CopyLeft(std::vector<T> & dst) { dst = samples[0]; }
    void CopyRight(std::vector<T> & dst) { dst = samples[1]; }
};

using PortPtr = shared_ptr<PortConnection>;
using ControlPtr = shared_ptr<ControlValue>;

template<typename Key, typename Value>
bool contains(std::map<Key,Value> & m, Key & k) {
    typename std::map<Key,Value>::iterator i = m.find(k);
    return i != m.end();
}

struct DspNode;
struct DSP {

    virtual void Run(DspNode * node, size_t n, StereoSamples &in, StereoSamples &out)=0;
};

struct Connection
{
    PortConnection * from, * to;    
};

using ConnectionPtr = shared_ptr<Connection>;

struct DspNode
{
    bool processed = false;
    vector<PortPtr> port_ptrs;
    vector<ControlValue> control_ptrs;    
    list<ConnectionPtr> connections;
    map<string,PortConnection*> ports;
    map<string,ControlValue*>   controls;

    
    DspNode() = default;

    void AddPort(const string &name, PortConnection * p) {
        ports[name] = p;
        port_ptrs.push_back( PortPtr( p, [](PortConnection * p) { delete p; }) );        
    }
    void AddControl(const string &name, ControlValue * p) {
        ports[name] = p;
        port_ptrs.push_back( ControlPtr( p, [](ControlValue * p) { delete p; }) );        
    }

    void SetPortData(const string & name, StereoSamples & s) {
        if(contains(ports,name)) {
            ports[name]->SetSamples(s);
        }
    }
    void SetControlValue(const string & name, float value) {
        if(contains(controls,name)) {
            controls[name]->SetValue(value);
        }
    }
    void Connect(Connection * p) {
        connections.push_back( ConnectionPtr(p, [](Connection *c) { delete c; }));
    }
    
    void Reset() { process = false; }
    void Run(size_t n, StereoSamples & in, StereoSamples & out) {
        for(size_t i = 0; i < inputs.size(); i++) {
            if(!inputs[i].processed) inputs[i]->Run(n,in,out);
        }
        dsp->Run(this,n,in,out);
        for(typename std::list<PortConnection*>::iterator i = connections.begin(); i != connections.end(); i++)
            (*i)->to->SetSamples((*i)->from->samples);
        processed=true;
    }
};

using DspNodePtr = shared_ptr<DspNode>;

struct Graph
{
    vector<DspNodePtr> node_ptrs;
    list<DspNode*> nodes;

    Graph() = default;

    void AddNode(DspNode * n) {
        nodes.push_back(n);
    }
}