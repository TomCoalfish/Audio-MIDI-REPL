#include "Differentiator.h"

dsp::Differentiator::Differentiator(Type type)
        : Transformer(type, type)
        , gate(std::make_shared<Input>(Type::BOOLEAN))
        , reset(std::make_shared<Input>(Type::BOOLEAN)) {
    getInputs().push_back(gate);
    getInputs().push_back(reset);
}

std::shared_ptr<dsp::Input> dsp::Differentiator::getReset() const {
    return reset;
}

std::shared_ptr<dsp::Input> dsp::Differentiator::getGate() const {
    return gate;
}

void dsp::Differentiator::setNumOutputChannelsNoLock(size_t numChannels) {
    Node::setNumOutputChannelsNoLock(numChannels);
    state.resize(numChannels, 0.0);
    memory.resize(numChannels, 0.0);
}

void dsp::Differentiator::processNoLock() {
    for (size_t channel = 0; channel < getNumChannels(); ++channel) {
        Sample *inputChannel = getInput()->getWrapper().getChannelPointer(channel);
        Sample *gateChannel = getGate()->getWrapper().getChannelPointer(channel);
        Sample *resetChannel = getReset()->getWrapper().getChannelPointer(channel);
        Sample *outputChannel = getOutput()->getWrapper().getChannelPointer(channel);
        Sample channelValue = getOutput()->getChannelValue(channel);
        for (size_t sample = 0; sample < getNumSamples(); ++sample) {
            if (resetChannel[sample]) {
                state[channel] = 0.0;
                memory[channel] = 0.0;
            }
            if (gateChannel[sample]) {
                if (std::isnan(memory[channel])) {
                    memory[channel] = 0.0;
                }
                state[channel] = inputChannel[sample] - memory[channel];
            }
            if (resetChannel[sample] || gateChannel[sample]) {
                memory[channel] = inputChannel[sample];
            }
            outputChannel[sample] = state[channel];
        }
    }
}
