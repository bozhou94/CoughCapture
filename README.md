# CoughCapture

## Installing PortAudio (Linux or Windows MinGW)

[Download](http://www.portaudio.com/download.html)
First install the ALSA dev library
```
sudo apt-get install libasound-dev
```

Then place the extracted portaudio directory into the project root and do the following:
```
cd portaudio
./configure
make all
make install
```

## Installing Python

Note that the classification part of the project runs on Python 3.5. Directions on how to get this version for Raspbian can be found [here](http://bohdan-danishevsky.blogspot.com/2015/10/building-python-35-on-raspberry-pi-2.html).

The project also requires numpy, scipy, and scikit-learn which can be installed as follows

```
sudo pip3.5 install numpy
sudo pip3.5 install scipy
sudo pip3.5 install -U scikit-learn
```

For Windows, an alternative that comes with everything is [Anaconda](https://www.continuum.io/downloads)


## Compiling and Running Capture

### Linux
From the project root, execute the following:
```
cp portaudio/include/portaudio.h .
cp portaudio/lib/.libs/libportaudio.a
gcc -o capture capture_lite.c libportaudio.a -lrt -lasound -lpthread -lm
./capture
```
### Windows (MinGW)
From the project root, execute the following:
```
cp portaudio/include/portaudio.h .
cp portaudio/lib/.libs/libportaudio.dll.a
gcc -o capture capture_lite.c libportaudio.dll.a
./capture.exe
```

## Training

To obtain raw training samples run in terminal:
```
arecord -f cd -D plughw:1,0 <path>.wav
```
This will record audio from the device at plughw:1,0 at 16 bits, 44100Hz and put it into a wav file at <path>.wav. You can exit the recording by pressing Ctrl-C.

To annotate the recorded audio, use [Praat](http://www.fon.hum.uva.nl/praat/). Note that areas that are coughs should be annotated as 'Cough'.

Before running the feature extraction, you'll need the [TextGrid parser package](https://github.com/kylebgorman/textgrid).

To extract features from the collected audio, run:
```
python3.5 python/extract_features.py <wav file> <TextGrid file> <output file>
```

Finally, to fit the collected data to a model and save it, run:
```
python3.5 python/fit.py <features file> <output pkl file>
```

## Running Classification

To run classification from the captured data run:
```
./capture | python3.5 python/classify.py <model pkl file> <output>
```
This will classify data as it comes in and output to the file in CSV format.
