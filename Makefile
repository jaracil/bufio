LIBRARY = bufio
STATIC_LIB = lib${LIBRARY}.a
SHARED_LIB = lib${LIBRARY}.so

INCLUDE_DIRS = -I include
CFLAGS = -g -fPIC

LIB_OBJS := bufio.o

BUFIO_LIBS = ${SHARED_LIB} $(STATIC_LIB)

ifdef TEST_DYN
	LD_EXTRA_FLAGS = 
else
	LD_EXTRA_FLAGS = -static
endif

bufiotest: ${BUFIO_LIBS} bufiotest.o
	$(CC) ${CFLAGS} ${LD_EXTRA_FLAGS} bufiotest.o -L. -lbufio -o $@ 

ifdef USE_OLD_API
CFLAGS += -DUSE_OLD_API
endif

%.o : %.c
	$(CC) -c ${CFLAGS} ${INCLUDE_DIRS} $< -o $@ 

${SHARED_LIB}: ${LIB_OBJS}
	$(CC) -shared $^ ${CFLAGS} ${LDFLAGS} -o $@ ${LDLIBS}

$(STATIC_LIB): ${LIB_OBJS}
	$(AR) rcs $@ $^

install: ${BUFIO_LIBS}
	mkdir -p ${PREFIX}/include
	mkdir -p ${PREFIX}/lib
	cp include/*.h ${PREFIX}/include
	cp $(SHARED_LIB) ${PREFIX}/lib
	cp $(STATIC_LIB) ${PREFIX}/lib

clean:
	rm -f ${LIB_OBJS} *.so *.a *.o bufiotest