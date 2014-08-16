LIB_DIR=../lib
INCLUDE =	\
	-I$(LIB_DIR)
LDFLAG = \
		-Wl,-rpath-link -Wl,$(LIB_DIR)	\
		-Wl,-rpath-link -Wl,$(libUsinglib)	\
		-L$(LIB_DIR) -ladvance
all: TestApp
TestApp:
	g++ -o TestApp $(INCLUDE) $(LDFLAG) TestApp.cpp
clean:
	rm -rf TestApp
	