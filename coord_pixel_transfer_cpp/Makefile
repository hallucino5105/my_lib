CC        = g++
LD        = ar
INCLUDES  = -I/opt/local/include
INCLUDES += -I/Users/hoshino/ym0_program/mylib/str2num
INCLUDES += -I/home/hoshino/ym0_program/mylib/str2num
LIBS      = 
CFLAGS    = -std=c++11 -O3 -Wall
LDFLAGS   = r
TARGET    = libcpt.a
SRCS      = coord_pixel_transfer.cpp
OBJS      = $(SRCS:.cpp=.o)


.SUFFIXES:
.SUFFIXES: .cpp .o

main: $(TARGET)

$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) $(LIBS) $@ $(OBJS)

clean:
	rm -f $(OBJS) $(TARGET)

.cpp.o:
	$(CC) $(CFLAGS) $(INCLUDES) -c $<

coord_pixel_transfer.o: coord_pixel_transfer.h

