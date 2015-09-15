# CoughCapture

## Installing PortAudio (Linux or Windows MinGW)
```
cd portaudio
./configure
make
make install
```

## Installing HTK 
For Windows users, after calling `./configure` go to `HTKTools/Makefile` and remove the `-lX11`
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
gcc -o capture main.c libportaudio.a -lrt -lasound -lpthread -lm
./capture
```
### Windows (MinGW)
From the project root, execute the following:
```
cp portaudio/include/portaudio.h .
cp portaudio/lib/.libs/libportaudio.dll.a
gcc -o capture main.c libportaudio.dll.a
./capture.exe
```
