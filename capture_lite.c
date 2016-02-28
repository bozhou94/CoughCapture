// Version of capture.c with no file writing, no plotting
// Output is delivered to stdout

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

#include "portaudio.h"

// Audio capture parameters
#define SAMPLE_RATE 44100
#define NUM_CHANNELS 1
#define FRAMES_PER_BUFFER 1024

PaStream *stream;
PaError err;

typedef struct {
  int frameIndex;
  int maxFrameIndex;
  char* recordedSamples;
} testData;

// Flow control
#define SUCCESS 1
#define FAILURE 0

int looping; // Boolean to dictate whether or not we should contiue looping

/**
 * Callback function for the stream
 */
static int captureCallback(const void *inputBuffer, void *outputBuffer,
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
  
  double rms = 0;
  int index;
  for (index = 0; index < framesPerBuffer; index++) {
    if (inData == NULL) {
      break;
    } else {
      float entry = *inData++;
      printf("%f\n", entry);
    }
  }
  return paContinue;
}

/**
 * Initializes PortAudio and an audio stream
 */
int initCapture() {

  PaStreamParameters inputParameters;
  testData data;
  int i;
  int totalFrames;
  
  err = Pa_Initialize();
  if( err != paNoError ) { 
    printf( "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    return FAILURE; 
  };

  // Set up stream
  data.maxFrameIndex = totalFrames = SAMPLE_RATE;
  data.frameIndex = 0;
  data.recordedSamples = (char*)malloc(totalFrames * sizeof(char));
  for(i = 0; i < totalFrames;i++) data.recordedSamples[i]= 0;

  inputParameters.device = Pa_GetDefaultInputDevice();
  inputParameters.channelCount = 1;
  inputParameters.sampleFormat = paFloat32;
  inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
  inputParameters.hostApiSpecificStreamInfo = NULL;

  
  err = Pa_OpenStream( &stream,
		       &inputParameters,
		       NULL, 
		       SAMPLE_RATE,
		       FRAMES_PER_BUFFER,
		       paClipOff,
		       captureCallback,
		       &data ); // pointer that will be passed to your callback
  
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

  // Initialize structures
  err = paNoError;

  // Initialize and start the capture
  if (initCapture()) {
    if (startCapture()) {
      
      // Looping the capture
      // looping = SUCCESS;
      while (1) {}
      
      // Stop and kill the capture
      /*
      if (stopCapture()) {
	if (killCapture()) {
	  fclose(dataFile);
	  printf("[INFO] Capture Finished\n");
	  return SUCCESS;
	}
	}*/
    }
  }
  //printf("[INFO] Capture Stalled\n");
  return FAILURE;
}
