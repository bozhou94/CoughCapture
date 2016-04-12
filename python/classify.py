# Read in audio signal
# Split into 32ms frames with half overlap
# Use mfcc to perform feature extraction on frame
import sys
import numpy as np
import time
import os
import extract_features as ef
from features import mfcc
from sklearn.externals import joblib

# Predicts and writes to file the classification of a window of
# low feature frames
def predictAndWrite(low_feat, model, csv):
        high_feat = ef.extractHigh(low_feat)
        # Reshape to 2D and predict
        high_feat = np.array(high_feat).reshape(1, len(high_feat))
        label = model.predict(high_feat)
        row = str(label[0]) + "\n"
        print(row)
        csv.write(row)
        csv.flush()
        os.fsync(csv)

if __name__ == '__main__':

        # Load trained model from file
        model = joblib.load(sys.argv[1])
        # CSV holding classifications
        csv= open(str(sys.argv[2]),'w')
        # Sleep for a bit to let capture start up
        time.sleep(1)

        # Raw signal frames switching every half frame and indices
        sig_frame1 = np.zeros(ef.LO_FRAME_SIZE)
        sig_frame2 = np.zeros(ef.LO_FRAME_SIZE)
        sig_counter1 = 0
        sig_counter2 = int(-ef.LO_FRAME_SIZE / 2)

        # Low feature frames switching every half frame and indices
        low_feat1 = np.zeros((ef.HI_FRAME_SIZE, ef.NUM_CEPTRUM))
        low_feat2 = np.zeros((ef.HI_FRAME_SIZE, ef.NUM_CEPTRUM))
        low_counter1 = 0
        low_counter2 = -ef.HI_STEP_SIZE

        # Read signal value
        for line in sys.stdin:
                try:
                        value = float(line)
                        # Place signal value into signal frames
                        sig_frame1[sig_counter1] = value
                        if sig_counter2 >= 0:
                                sig_frame2[sig_counter2] = value
                        # Reached end of frame, extract and restart
                        if sig_counter1 == ef.LO_FRAME_SIZE - 1:
                                # Calculate mfcc and put it in low frame
                                low_row = ef.extractLow(sig_frame1)
                                low_feat1[low_counter1] = low_row
                                if low_counter2 >= 0:
                                        low_feat2[low_counter2] = low_row
                                # Reached end of low features, extract and restart
                                if low_counter1 == ef.HI_FRAME_SIZE - 1:
                                        predictAndWrite(low_feat1, model, csv)
                                        low_counter1 = 0
                                else:
                                        low_counter1 += 1
                                if low_counter2 == ef.HI_FRAME_SIZE - 1:
                                        predictAndWrite(low_feat2, model, csv)
                                        low_counter2 = 0
                                else:
                                        low_counter2 += 1
                                        
                                sig_counter1 = 0
                        else:
                                sig_counter1 += 1
                                
                        if sig_counter2 == ef.LO_FRAME_SIZE - 1:
                                # Calculate mfcc and put it in low frame
                                low_row = ef.extractLow(sig_frame2)
                                low_feat1[low_counter1] = low_row
                                if low_counter2 >= 0:
                                        low_feat2[low_counter2] = low_row
                                # Reached end of low features, extract and restart
                                if low_counter1 == ef.HI_FRAME_SIZE - 1:
                                        predictAndWrite(low_feat1, model, csv)
                                        low_counter1 = 0
                                else:
                                        low_counter1 += 1
                                if low_counter2 == ef.HI_FRAME_SIZE - 1:
                                        predictAndWrite(low_feat2, model, csv)
                                        low_counter2 = 0
                                else:
                                        low_counter2 += 1

                                sig_counter2 = 0
                        else:
                                sig_counter2 += 1
                except ValueError:
                        print("Reading strings")
