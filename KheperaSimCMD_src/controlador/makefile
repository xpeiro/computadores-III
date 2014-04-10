CFLAGS = -I../remoteApi -I../include -DNON_MATLAB_PARSING -DMAX_EXT_API_CONNECTIONS=255
LDFLAGS = -lpthread
OBJS = ../remoteApi/extApi.o ../remoteApi/extApiPlatform.o controlador.o
OUTPUT = controlador

OS = $(shell uname -s)


ifeq ($(OS), Linux)
	CFLAGS += -D__linux
else
	CFLAGS += -D__APPLE__
endif

all: controlador clean

controlador: $(OBJS) 
			$(CXX) -o $(OUTPUT) $^  $(LDFLAGS)

controlador.o: controlador.cpp control.o demo.o
				$(CXX) -c $(CFLAGS) $< 

control.o: control.cpp i_control.h
			$(CXX) -c $(CFLAGS) $<

demo.o: demo.cpp i_control.h
		$(CXX) -c $(CFLAGS) $<

clean:
		rm *.o

delete: 
		rm controlador