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

The project also requires numpy and scipy, which can be installed as follows

```
sudo pip3.5 install numpy
sudo pip3.5 install scipy
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

## Running Classification

To run classification from the captured data run:
```
./classify | python3.5 python/classify.py
```