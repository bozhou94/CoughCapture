# Read in training sets
# Fit to a classification model
# Save the model to disk
import sys
import numpy as np
from sklearn import svm
from sklearn import ensemble
from sklearn.externals import joblib

if __name__ == '__main__':

    # Read in CSV data
    data = np.genfromtxt(sys.argv[1], delimiter=',')

    # Split into data; labels are last column
    labels = data[:,data.shape[1] - 1]
    samples = data[:,0:data.shape[1] - 1]

    # Train data
    model = svm.SVC()
    # model = ensemble.RandomForestClassifier()
    model.fit(samples, labels)

    # Dump model
    joblib.dump(model, sys.argv[2])
