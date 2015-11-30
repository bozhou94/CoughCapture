#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include <fftw.h>
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

// File parameters
#define FILE_TTL 600 // Time before we switch to a new file in seconds

time_t lastCheckpoint, newCheckpoint;
FILE* dataFile; // holds the data accumulated until now
FILE* nextFile;

// Plotting parameters
unsigned int sampleNumber;
FILE* rmsPlotFile; // holds the rms data for plotting
FILE* gnuplotPipe;

// Flow control
#define SUCCESS 1
#define FAILURE 0

int looping; // Boolean to dictate whether or not we should contiue looping

char* getNextFileName();

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

  // Check if we should create a new checkpoint
  time(&newCheckpoint);
  double timeDiff = difftime(newCheckpoint,lastCheckpoint);
  // printf("[DEBUG] Time difference is %f\n", timeDiff);

  if ((int) timeDiff > FILE_TTL) {
    lastCheckpoint = newCheckpoint;
    dataFile = freopen(getNextFileName(), "wb", dataFile);
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
	  rms += entry * entry;
      fwrite(&entry,1,sizeof(entry),dataFile);
      // printf("[DEBUG] Float is %f\n", entry);
    }
  }
  rms = sqrt(rms / index);
  
  // Write data and graph
  fprintf(rmsPlotFile, "%u %f \n", sampleNumber++, rms);
  fprintf(gnuplotPipe, "set title \"Audio RMS\" \n");
  fprintf(gnuplotPipe, "plot 'rmsData.temp'");
  
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
  sampleNumber = 0;
  rmsPlotFile = fopen("rmsData.temp", "w");
  gnuplotPipe = popen("gnuplot -persistent", "w");
  
  time(&lastCheckpoint);
  dataFile = fopen(getNextFileName(), "wb");
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
  fclose(dataFile);
  //printf("[INFO] Capture Stalled\n");
  return FAILURE;
}
