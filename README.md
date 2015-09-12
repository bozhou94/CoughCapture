# CoughCapture

To compile, run: 
	On Linux: gcc -o capture main.c libportaudio.a -lrt -lasound -lpthread -lm
	On Windows (MSYS Shell): gcc -o capture main.c libportaudio.dll.a
Then to run the file, execute:
	On Linux: ./capture
	On Windows: ./capture.exe