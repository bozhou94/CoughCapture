#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "portaudio.h"

#define SAMPLE_RATE (44100)

PaStream *stream;
PaError err;

void *data;

/**
 * Callback function for the stream
 */
static int captureCallback( 
	const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData) {

	return 0;
}

/**
 * Initializes PortAudio and an audio stream
 * Returns 0 if success, -1 otherwise
 */
int initCapture() {
	err = Pa_Initialize();
	if( err != paNoError ) { 
		printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
		return -1; 
	};

	err = Pa_OpenDefaultStream( &stream,
		0, // no input channels 
		2, // stereo output
		paFloat32, // 32 bit floating point output
		SAMPLE_RATE,
		256, // frames per buffer
		captureCallback,
		&data ); // pointer that will be passed to your callback

	if( err != paNoError ) { 
		printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
		return -1; 
	};

	return 0;
}

/**
 * Begins the capture on the stream
 * Returns 0 if success, -1 otherwise
 */
int startCapture() {
	err = Pa_StartStream( stream );
	if( err != paNoError ) { 
		printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
		return -1;
	};
	return 0;
}

/**
 * Halts the capture on the stream
 * Returns 0 if success, -1 otherwise
 */
int stopCapture() {
	err = Pa_StopStream( stream );
	if( err != paNoError ) { 
		printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
		return -1;
	};
	return 0;
}

/**
 * Terminates the stream and PortAudio
 * Returns 0 if success, -1 otherwise
 */
int killCapture() {
	err = Pa_Terminate();
	if( err != paNoError ) { 
		printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
		return -1; 
	};
	return 0;	
}

int main() {
	int status;
	status = initCapture();
	if (status == 0) {
		return -1;
	}
	status = killCapture();
	return status;
}
