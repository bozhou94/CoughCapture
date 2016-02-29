# Read in audio signal
# Split into 32ms frames with half overlap
# Use mfcc to perform feature extraction on frame
import sys
import numpy as np
import time
import os
from features import mfcc

SAMPLING_RATE = 44100 # 44100 Hz capture
FRAME_DURATION = 0.032 # 32ms length frames
FRAME_SIZE = int(SAMPLING_RATE * FRAME_DURATION) # Samples in one frame

FILE_LOCATION = "mfccs.csv"

# Writes the MFCC for a frame to a csv file
def writeMfcc(frame, csv):
        mfcc_feat = mfcc(frame, samplerate=SAMPLING_RATE, winlen=FRAME_DURATION,appendEnergy=True)
        row = ""
        for index in range(0,12):
                row += str(mfcc_feat[0,index]) + ", "
        row += "\n"
        print(row)
        csv.write(row)
        csv.flush()
        os.fsync(csv)

if __name__ == '__main__':
        time.sleep(1)
        frame1 = np.zeros(FRAME_SIZE) # First frame
        frame2 = np.zeros(FRAME_SIZE) # Second starting from half-point of first frame
        counter1 = 0 # Current index of frame 1
        counter2 = int(-FRAME_SIZE / 2) # Current index of frame 2

        # CSV File holding MFCCs
        csv= open(FILE_LOCATION,'w')

        # Read signal value
        for line in sys.stdin:
                try:
                        value = float(line)
                        # Place signal value into frame 1
                        frame1[counter1] = value
                        # Place signal value into frame 2
                        if counter2 >= 0:
                                frame2[counter2] = value
                        # Reached end of frame, classify, output and restart
                        if counter1 == FRAME_SIZE - 1:
                                writeMfcc(frame1, csv)
                                counter1 = 0
                        else:
                                counter1 += 1
                        if counter2 == FRAME_SIZE - 1:
                                writeMfcc(frame2, csv)
                                counter2 = 0
                        else:
                                counter2 += 1
                except ValueError:
                        print("Reading strings")
