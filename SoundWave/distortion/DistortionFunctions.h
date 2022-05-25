#pragma once 
namespace DistortionFunctions {
    float distortionOne(float input);
    float distortionTwo(float input);
    
    float overdrive(float input, float drive);
    
    // Functions from MusicDSP: http://musicdsp.org
    float softSaturation(float input, float param);
    float clipper(float input, float drive);
    
}