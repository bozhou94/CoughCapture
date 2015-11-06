#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "portaudio.h"

#define SAMPLE_RATE 44100
#define NUM_CHANNELS 1
#define BUFFER_LENGTH 256
#define SUCCESS 1
#define FAILURE 0

PaStream *stream;
PaError err;

int looping; // Boolean to dictate whether or not we should contiue looping

// For data collection purposes
time_t lastCheckpoint;
int sampleNumber;
FILE* dataFile; // holds the data accumulated until now
// FILE* gnuplotPipe; // Pipes commands to gnuplot

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
  
  int index;
  for (index = 0; index < framesPerBuffer; index++) {
    if (inData == NULL) {
      break;
    } else {
      float entry = *inData++;
      fwrite(&entry,1,sizeof(entry),dataFile);
      // printf("[DEBUG] Float is %f\n", entry);
    }
  }
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
			      NUM_CHANNELS, // stereo input
			      NUM_CHANNELS, // stereo output
			      paFloat32, // 32 bit floating point output
			      SAMPLE_RATE,
			      BUFFER_LENGTH, // frames per buffer
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

/**
 * Obtains the next file name based on the set checkpoint
 */
char* getNextFileName() {
  // Obtain time string
  char* timeString = asctime(localtime(&lastCheckpoint));
  // Remove unwanted characters
  char* i = timeString;
  while(*i != '\n') {
    if (*i == ' ' || *i == ':' || *i == '?')
      *i = '_';
    *i++;
  }
  *i = 0;
  char* fileName = malloc(strlen(timeString)+8);
  strcpy(fileName,"bin/");
  strcat(fileName,timeString);
  strcat(fileName,".bin");
  return fileName;
}

int main() {
  // Initialize structures
  time(&lastCheckpoint);
  dataFile = fopen(getNextFileName(), "wb");
  // gnuplotPipe = popen("gnuplot -persistent", "w");	
  // sampleNumber = 0;

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
