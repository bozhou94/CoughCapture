#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "portaudio.h"

#define SAMPLE_RATE 44100
#define SUCCESS 1
#define FAILURE 0

PaStream *stream;
PaError err;

int looping; // Boolean to dictate whether or not we should contiue looping

/**
 * Callback function for the stream
 */
static int captureCallback( 
	const void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer,
	const PaStreamCallbackTimeInfo* timeInfo,
	PaStreamCallbackFlags statusFlags,
	void *userData) {

	// Received no input, just ignore
	if (inputBuffer == NULL) {
		return paContinue;
	}

	// Loop through the input to obtain samples
	float* inData = (float*) inputBuffer;

	int index;
	for (index = 0; index < framesPerBuffer; index++) {
		if (inData == NULL) {
			break;
		} else {
			printf("[DEBUG] Float is %f\n", *inData++);
		}
	}

	looping = FAILURE;
	return paContinue;
}

/**
 * Initializes PortAudio and an audio stream
 */
int initCapture() {
	err = Pa_Initialize();
	if( err != paNoError ) { 
		printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
		return FAILURE; 
	};

	err = Pa_OpenDefaultStream( &stream,
		2, // stereo input
		2, // stereo output
		paFloat32, // 32 bit floating point output
		SAMPLE_RATE,
		256, // frames per buffer
		captureCallback,
		NULL ); // pointer that will be passed to your callback

	if( err != paNoError ) { 
		printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
		return FAILURE; 
	};

	return SUCCESS;
}

/**
 * Begins the capture on the stream
 */
int startCapture() {
	err = Pa_StartStream( stream );
	if( err != paNoError ) { 
		printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
		return FAILURE;
	};
	return SUCCESS;
}

/**
 * Halts the capture on the stream
 */
int stopCapture() {
	err = Pa_StopStream( stream );
	if( err != paNoError ) { 
		printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
		return FAILURE;
	};
	return SUCCESS;
}

/**
 * Terminates the stream and PortAudio
 */
int killCapture() {
	err = Pa_Terminate();
	if( err != paNoError ) { 
		printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
		return FAILURE;
	};
	return SUCCESS;
}

int main() {
	// Initialize and start the capture
	if (initCapture()) {
		if (startCapture()) {

			// Looping the capture
			looping = SUCCESS;
			while (looping) {}

			// Stop and kill the capture
			if (stopCapture()) {
				if (killCapture()) {
					printf("[INFO] Capture Finished\n");
					return SUCCESS;
				}
			}
		}
	}
	return FAILURE;
}
