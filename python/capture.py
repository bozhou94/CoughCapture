# Captures audio using ALSA library using single-channel 44100 Hz
import pyaudio
import numpy as np

SAMPLING_RATE = 44100 # 44100 Hz capture
FRAME_DURATION = 0.032 # 32ms length frames
FRAME_SIZE = int(SAMPLING_RATE * FRAME_DURATION) # Samples in one frame
NUM_CHANNELS = 1 # single-channel input

def callback(in_data, frame_count, time_info, status):
	# Print obtained data
	if in_data:
		temp_data=np.fromstring(in_data, dtype=np.float32)
		for point in np.nditer(a):
			print(point)
	return in_data, pyaudio.paContinue

if __name__ == '__main__':
	# Start stream
	pa = pyaudio.PyAudio()
	stream = pa.open(format=pyaudio.paFloat32,
					 channels=NUM_CHANNELS,
					 rate=SAMPLING_RATE,
					 input=True,
					 frames_per_buffer=FRAME_SIZE,
					 stream_callback=callback)