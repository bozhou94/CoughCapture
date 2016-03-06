# Read in audio signal
# Split into 32ms frames with half overlap
# Use mfcc to perform feature extraction on frame
import sys
import numpy as np
import scipy.io.wavfile as wav
from features import mfcc

SAMPLING_RATE = 44100 # 44100 Hz capture
FRAME_DURATION = 0.032 # 32ms length frames
FRAME_SIZE = int(SAMPLING_RATE * FRAME_DURATION) # Samples in one frame

if __name__ == '__main__':
        
        # Read in WAV file
        (rate, sig) = wav.read(str(sys.argv[1]))

        # Extract low-level features via MFCC
        mfcc_feat = mfcc(sig, samplerate = SAMPLING_RATE, winlen = FRAME_DURATION, winstep = FRAME_DURATION / 2, appendEnergy = True)
        print(mfcc_feat)
