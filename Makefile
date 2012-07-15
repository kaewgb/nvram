CC					= g++
CFLAGS				=

TARGET				= test
OBJ_FILES			= test.o nvram.o

BUBBLE				= bubble
BUBBLE_OBJ_FILES 	= bubble.o nvram.o

RASUM				= rasum
RASUM_OBJ_FILES		= rasum.o random.o nvram.o

HEADERS		= nvram.h

$(TARGET):	$(OBJ_FILES)
			$(CC) -o $(TARGET) $^

$(BUBBLE):	$(BUBBLE_OBJ_FILES)
			$(CC) -o $@ $^

$(RASUM):	$(RASUM_OBJ_FILES)
			$(CC) -o $@ $^

%.o:		%.cpp $(HEADERS)
			$(CC) $(CFLAGS) -c $< -o $@
clean:
			rm -f *.o $(TARGET) $(BUBBLE) $(RASUM)
