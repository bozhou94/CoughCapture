# Read in audio signal
# Split into 32ms frames with half overlap
# Use mfcc to perform feature extraction on frame
import sys
import numpy as np
from features import mfcc

SAMPLING_RATE = 44100 # 44100 Hz capture
FRAME_DURATION = 0.032 # 32ms length frames
FRAME_SIZE = int(SAMPLING_RATE * FRAME_DURATION) # Samples in one frame

if __name__ == '__main__':
	frame1 = np.zeros(FRAME_SIZE) # First frame
	frame2 = np.zeros(FRAME_SIZE) # Second starting from half-point of first frame
	counter1 = 0 # Current index of frame 1
	counter2 = int(-FRAME_SIZE / 2) # Current index of frame 2

	# Read signal value
	for line in sys.stdin:
		value = float(line)
		# Place signal value into frame 1
		frame1[counter1] = value
		# Place signal value into frame 2
		if counter2 >= 0:
			frame2[counter2] = value
		# Reached end of frame, classify and restart
		if counter1 == FRAME_SIZE - 1:
			mfcc_feat1 = mfcc(frame1, SAMPLING_RATE, FRAME_DURATION)
			print(mfcc_feat1)
			counter1 = 0
		else:
			counter1 += 1
		if counter2 == FRAME_SIZE - 1:
			mfcc_feat2 = mfcc(frame2, SAMPLING_RATE, FRAME_DURATION)
			counter2 = 0
		else:
			counter2 += 1