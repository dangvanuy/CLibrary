LIB_DIR=../lib
INCLUDE =	\
	-I$(LIB_DIR)
LDFLAG = -L$(LIB_DIR) -ladvance -L$(LIB_DIR)/libUsinglib -lbasic 
all: TestApp
TestApp:
	g++ -o TestApp $(INCLUDE) $(LDFLAG) TestApp.cpp
clean:
	rm -rf TestApp
	