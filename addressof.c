#include <stdio.h>
#include <stddef.h>
#include "portaudio.h"

PaStream * stream;
PaStreamParameters outputParameters;

PaStream*  get_stream() { return stream; }
PaStream** get_stream_address() { return &stream; }

void RunLoop() {
    while(1) {
        sleep(1);
    }
}

int StreamCallback(
    const void *input, void *output,
    unsigned long frameCount,
    const PaStreamCallbackTimeInfo* timeInfo,
    PaStreamCallbackFlags statusFlags,
    void *userData )
	{
		printf("%d\n",frameCount);
		return paContinue;
	}


void test()
{
	outputParameters.device = Pa_GetDefaultOutputDevice(); /* default output device */
    if (outputParameters.device == paNoDevice) {
        fprintf(stderr,"Error: No default output device.\n");        
    }
    outputParameters.channelCount = 2;       /* stereo output */
    outputParameters.sampleFormat = paFloat32; /* 32 bit floating point output */
    outputParameters.suggestedLatency = Pa_GetDeviceInfo( outputParameters.device )->defaultLowOutputLatency;
    outputParameters.hostApiSpecificStreamInfo = NULL;

    int err = Pa_OpenStream(
              &stream,
              NULL, /* no input */
              &outputParameters,
              44100,
              256,
              paClipOff,      /* we won't output out of range samples so don't bother clipping them */
              StreamCallback,
              NULL );

	Pa_StartStream(stream);
}

