CC        = g++-4.8
INCLUDES  = -I. -I/opt/local/include -I/usr/local/include
LIBS      = -L. -L/opt/local/lib -L/usr/local/lib 
CFLAGS    = -std=c++11 -g -O0
LDFLAGS   = -lopencv_core -lopencv_highgui -lopencv_video -lopencv_imgproc -lopencv_features2d
LDFLAGS  += -lboost_system -lboost_filesystem
TARGET    = libcvhelper.a
SRCS      = cvhelper.cpp
OBJS      = $(SRCS:.cpp=.o)

.SUFFIXES: .cpp .o

all: $(TARGET)

$(TARGET): $(OBJS)
	ar rv $(TARGET) $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

cvhelper.o: cvhelper.hpp imagemat.hpp

