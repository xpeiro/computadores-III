CC = g++
CFLAGS = -I../remoteApi -I../include -DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255
LDFLAGS = -lpthread
OBJS = ../remoteApi/extApi.o ../remoteApi/extApiPlatform.o controlador.o
OUTPUT = controlador

all: controlador clean

controlador: $(OBJS) 
			$(CC) -o $(OUTPUT) $^  $(LDFLAGS)

controlador.o: controlador.cpp control.o demo.o
				$(CC) -c $(CFLAGS) $< 

control.o: control.cpp i_control.h
			$(CC) -c $(CFLAGS) $<

demo.o: demo.cpp i_control.h
		$(CC) -c $(CFLAGS) $<

clean:
		rm *.o

delete: 
		rm controlador