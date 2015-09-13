# CoughCapture

## Linux
First, install PortAudio on your machine
```
cd portaudio
./configure
make
make install
```
Next, compile the main file 
```
cd ..
gcc -o capture main.c libportaudio.a -lrt -lasound -lpthread -lm
```
Finally, execute the binary
```
./capture
```

## Windows (MinGW)
First, execute the MSYS shell. In it, install PortAudio
```
cd portaudio
./configure
make
make install
```
Next, compile the main file
```
cd ..
gcc -o capture main.c libportaudio.dll.a
```
Finally, execute the binary
```
./capture.exe
```
