# Read in audio signal
# Split into 32ms frames with half overlap
# Use mfcc to perform feature extraction on frame
import sys
import numpy as np
import scipy.io.wavfile as wav
from features import mfcc
import textgrid as tg

SAMPLING_RATE      = 44100 # 44100 Hz capture
LO_FRAME_DURATION  = 0.032 # 32ms length frames for low-level feature extraction
LO_FRAME_STEP      = 0.016 # 16ms steps for LLFE
LO_FRAME_SIZE      = int(SAMPLING_RATE * LO_FRAME_DURATION) # Samples in one frame for LLFE
HI_FRAME_DURATION  = 1 # 1s length frame for high-level feature extraction
HI_FRAME_STEP      = 0.5 # 0.5s steps for HLFE
HI_FRAME_SIZE      = int(HI_FRAME_DURATION / LO_FRAME_STEP) - 1 # Number of LLFE rows in one frame for HLFE
HI_STEP_SIZE       = int(HI_FRAME_STEP / LO_FRAME_STEP) - 1 # Number of LLFE rows in one step for HLFE

# Labels for classification
COUGH              = 1
NO_COUGH           = 0

# Calculates the low level features via MFCC
def extractLow(signal):
        return mfcc(signal,
                    samplerate = SAMPLING_RATE,
                    winlen = LO_FRAME_DURATION,
                    winstep = LO_FRAME_STEP,
                    appendEnergy = True)

# Creates the annotation array for the low-level features
def annotate(low_feat, fileName):
        # Parse the TextFile for intervals
        parsed = tg.TextGrid()
        parsed.read(fileName)
        intervals = parsed.getList('Cough')[0].intervals

        # Walk through the intervals as we annotate cough MFCCs
        num_rows = low_feat.shape[0]
        interval_index = 0
        annotations = np.zeros(num_rows)
        for row in range(0,num_rows):
                time_start = row * LO_FRAME_STEP
                time_end = time_start + LO_FRAME_DURATION

                # Update until we're in correct interval and check
                while interval_index < len(intervals) and time_start > intervals[interval_index].maxTime:
                        interval_index += 1
                if interval_index < len(intervals) and intervals[interval_index].mark == 'Cough':
                        annotations[row] = COUGH
        return annotations

# Calculates the high level features via mean and std
def extractHigh(low_feat, annotations):
        num_rows = low_feat.shape[0]
        num_high_feat_rows = num_rows 
        # Not enough rows for high level features
        if num_rows < HI_FRAME_SIZE:
                return np.array([])
        # Otherwise, examine section by section
        else:
                high_feat = extractHighRow(low_feat, annotations, 0, HI_FRAME_SIZE)
                for start in range(HI_STEP_SIZE, num_rows, HI_STEP_SIZE):
                        row = extractHighRow(low_feat, annotations, start, start + min(num_rows,HI_FRAME_SIZE))
                        high_feat = np.vstack([high_feat, row])
                return high_feat

# Calculates a row of high level features from a start (inclusive) to an end (exclusive) row in the low level features
def extractHighRow(low_feat, annotations, start, end):
        num_cols = low_feat.shape[1]
        row = np.zeros(2 * num_cols + 1)
        for col in range(0,num_cols):
                row[2 * col] = columnMean(low_feat, start, end, col)
                row[2 * col + 1] = columnStd(low_feat, start, end, col)
        row[2 * num_cols] = label(annotations, start, end)
        return row

### HIGH LEVEL FEATURE EXTRACTION FUNCTIONS ###

# Gives a label (0 for no cough, 1 for cough)
def label(annotations, start, end):
        if sum(annotations[start:end]) > 0: # There exists at least one cough
                return COUGH
        else: # There exists no coughs
                return NO_COUGH

# Calculates the mean of a column
def columnMean(low_feat, start, end, col):
        return np.mean(low_feat[start:end,col])

# Calculates the std of a column
def columnStd(low_feat, start, end, col):
        return np.std(low_feat[start:end,col])


### MAIN FUNCTION ###
# Takes in arguments <wav file> <annotation file> <output csv>
if __name__ == '__main__':
        
        # Read in WAV file
        (rate, sig) = wav.read(str(sys.argv[1]))

        # Extract low-level features via MFCC
        low_feat = extractLow(sig)
                
        # Annotate low-level features
        annotations = annotate(low_feat, str(sys.argv[2]))

        # Combine into high-level features 
        high_feat = extractHigh(low_feat, annotations)

        # Write to file
        csv = open(str(sys.argv[3]), 'w')
        output = ""
        for row in range(0, high_feat.shape[0]):
                for col in range(0, high_feat.shape[1]):
                        output += str(high_feat[row, col]) + ","
                output += "\n"
        csv.write(output)
        csv.close()
