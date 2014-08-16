BASIC_DIR=libUsinglib
INCLUDE =	\
	-I$(BASIC_DIR)
LDFLAG = -Wl,-rpath-link -Wl,$(BASIC_DIR)	\
		-L$(BASIC_DIR) -lbasic
all: libadvance.so
libadvance.so:
	g++ -o libadvance.so $(INCLUDE) $(LDFLAG) advancelib.cpp -shared -fPIC
clean:
	rm -rf libadvance.so
	