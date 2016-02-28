# CoughCapture

## Installing PortAudio (Linux or Windows MinGW)

Download at http://www.portaudio.com/download.html
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

## Installing HTK 
For Windows MinGW users, go to `htk/HTKLib/esignal.c` and replace all instances of `ARCH` with `"darwin"`.

Additionally, after calling `./configure` go to `HTKTools/Makefile` and remove the `-lX11`
```
cd htk
./configure
make all
make install
```

## Compilation and Running

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
