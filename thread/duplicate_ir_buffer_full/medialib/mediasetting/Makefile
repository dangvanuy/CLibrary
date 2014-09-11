PWD:= $(shell pwd)
TOP = $(PWD)

TARGET_TYPE = SHARED_LIBRARY
TARGET = libmediasetting.so
# include build.flags.env
include ../build.flags.env

#Mediasetting lib
#MEDIASETTING_LIB_DIR	= .
MEDIASETTING_API_DIR	= mediasettingapi
MEDIASETTING_LIB_DIR	= mediasettinglib
MEDIASETTING_LIB_SRC_DIR	= $(MEDIASETTING_LIB_DIR)/src
MEDIASETTING_LIB_INC_DIR	= $(MEDIASETTING_LIB_DIR)/include
#for Zeno
ZENOLIB_LIB_DIR                 = ../zenolib
ZENOLIB_LIB_DIR_INCLUDE			= $(ZENOLIB_LIB_DIR)/include
#########################################################
ifdef DIRECTFB_INSTALL_DIR
	DFB_DIR = $(DIRECTFB_INSTALL_DIR)
else
ifneq '$(findstring -DSDK_4_0_0,$(RMCFLAGS))' ''
	DFB_DIR = $(DCCHD_INSTALL_DIR)
else
	DFB_DIR = $(DIRECTFB_DIR)
endif
endif

DATADIR = $(DFB_DIR)/share/directfb-examples
#############################################################
#For player
SRC = \
		$(MEDIASETTING_LIB_SRC_DIR)/mediaconfig_dump.c \
		$(MEDIASETTING_LIB_SRC_DIR)/mediaplaylist.c \
		$(MEDIASETTING_LIB_SRC_DIR)/iruser.c \
		$(MEDIASETTING_LIB_SRC_DIR)/media.c	\
        $(MEDIASETTING_LIB_SRC_DIR)/hotplug_handler.c	\
		$(ZENOLIB_LIB_DIR)/zenotimer/src/zenotimer.cpp \
		$(ZENOLIB_LIB_DIR)/zenogpio/src/zenogpio.cpp

RMCFLAGS +=	\
	-I$(QT_INSTALL_DIR)/include \
	-I$(QT_INSTALL_DIR)/include/Qt \
	-I$(QT_INSTALL_DIR)/include/QtCore \
	-I$(DTV_DIR)/include \
	-I$(CORE_DIR) \
	-I$(CORE_DIR)/include \
	-I./include/npapi


#######################################################################

#COMPILKIND  += 'withoutdbg withthreads'
USE_STD_LIB := 1
LINKER      := g++

ifneq '$(findstring -DEM86XX_MODE=EM86XX_MODEID_STANDALONE,$(RMCFLAGS))' ''

ifneq '$(findstring -DEM86XX_CHIP=EM86XX_CHIPID_TANGO2,$(RMCFLAGS))' ''

CROSS      := mipsel-linux-
RMPLATFORM := RMPLATFORMID_TANGO2
RMCFLAGS   += -mips32r2 -Wa,-mips32r2
COMPILKIND += withdl
lib_ext    := so
LDFLAGS =-g -O

else
ifneq '$(findstring -DEM86XX_CHIP=EM86XX_CHIPID_TANGO3,$(RMCFLAGS))' ''

CROSS      := mipsel-linux-
RMPLATFORM := RMPLATFORMID_TANGO3
RMCFLAGS   += -mips32r2 -Wa,-mips32r2
COMPILKIND += withdl
lib_ext    := so
LDFLAGS =-g -O

else # EM86XX_CHIPID_TANGO2

CROSS      := arm-elf-
RMPLATFORM := RMPLATFORMID_JASPERMAMBO
RMCFLAGS   += -DLLAD_DIRECT -D__arm__
LDFLAGS    += -Wl,-elf2flt="-s32768"
lib_ext    := a

ifeq  '$(findstring static,$(COMPILKIND))' ''
COMPILKIND += static
endif

COMPILKIND += withthreads

endif # EM86XX_CHIPID_TANGO3
endif # EM86XX_CHIPID_TANGO2

else # EM86XX_MODEID_STANDALONE
ifneq '$(findstring -DEM86XX_MODE=EM86XX_MODEID_WITHHOST,$(RMCFLAGS))' ''

COMPILKIND += withdl
#COMPILKIND += release#withdl
lib_ext := so

else # EM86XX_MODEID_WITHHOST

$(error EM86XX_MODE is not set in RMCFLAGS: refer to emhwlib/include/emhwlib_modes.h)

endif # EM86XX_MODEID_WITHHOST
endif # EM86XX_MODEID_STANDALONE


LDFLAGS += \
	-L$(DESTDIR)/$(DFB_DIR)/lib \
	-Wl,-rpath-link -Wl,$(RUA_DIR)/lib \
	-Wl,-rpath-link -Wl,$(DCCHD_DIR)/core \
	-Wl,-rpath-link -Wl,$(DCCHD_DIR)/mono \
	-Wl,-rpath-link -Wl,$(DCCHD_DIR)/dvdvr \
	-Wl,-rpath-link -Wl,$(DCCHD_DIR)/brd \
	-Wl,-rpath-link -Wl,$(DCCHD_DIR)/curacao \
	-Wl,-rpath-link -Wl,$(DCCHD_DIR)/curacao/lib \
	-Wl,-rpath-link -Wl,$(DCCHD_DIR)/dtv \
	-Wl,-rpath-link -Wl,$(DCCHD_DIR)/dtv/tuner \
	-Wl,-rpath-link -Wl,$(DCCHD_DIR)/dtv/capture \
	-Wl,-rpath-link -Wl,$(DCCHD_DIR)/dcchd \
	-Wl,-rpath-link -Wl,$(DESTDIR)/$(DFB_DIR)/lib

RMCFLAGS +=     \
    -D_FILE_OFFSET_BITS=64 \
	-DLLAD_DIRECT \
	-DALLOW_OS_CODE \
	-D_REENTRANT \
	-DWITH_RM_FILE_JPEG \
	-DRMVDEMUX_LOCAL_RMOBJECT=1 \
	-DBASIC_MPEG4_FRAMEWORK=1 \
	-DBASIC_DETECTOR_FRAMEWORK=1 \
	-DWITH_THREADS \
	-DWITH_MONO=1

INCLUDES += \
		-I$(MEDIASETTING_LIB_DIR) \
		-I$(MEDIASETTING_API_DIR) \
		-I$(MEDIASETTING_LIB_INC_DIR) \
		-I$(ZENOLIB_LIB_DIR_INCLUDE)      \
		-I$(ZENOLIB_LIB_DIR)/zenoapp/include     \
		-I$(ZENOLIB_LIB_DIR)/zenocommand/include     \
		-I$(ZENOLIB_LIB_DIR)/zenodfb/include     \
		-I$(ZENOLIB_LIB_DIR)/zenogpio/include     \
		-I$(ZENOLIB_LIB_DIR)/zenotimer/include     \
		-I$(MEDIASETTING_LIB_INC_DIR)/dtv	\
		-I$(QT_DIR)/src/3rdparty/webkit/Source/WebCore/bridge \
		-I$(QT_DIR)/src/3rdparty/webkit/WebCore/bridge

JAVASCRIPTCORE_PATH=$(QT_DIR)/src/3rdparty/webkit/Source/JavaScriptCore
INCLUDES += \
    -I$(JAVASCRIPTCORE_PATH) \
    -I$(JAVASCRIPTCORE_PATH)/.. \
    -I$(JAVASCRIPTCORE_PATH)/assembler \
    -I$(JAVASCRIPTCORE_PATH)/bytecode \
    -I$(JAVASCRIPTCORE_PATH)/bytecompiler \
    -I$(JAVASCRIPTCORE_PATH)/debugger \
    -I$(JAVASCRIPTCORE_PATH)/interpreter \
    -I$(JAVASCRIPTCORE_PATH)/jit \
    -I$(JAVASCRIPTCORE_PATH)/parser \
    -I$(JAVASCRIPTCORE_PATH)/pcre \
    -I$(JAVASCRIPTCORE_PATH)/profiler \
    -I$(JAVASCRIPTCORE_PATH)/runtime \
    -I$(JAVASCRIPTCORE_PATH)/wtf \
    -I$(JAVASCRIPTCORE_PATH)/wtf/symbian \
    -I$(JAVASCRIPTCORE_PATH)/wtf/unicode \
    -I$(JAVASCRIPTCORE_PATH)/yarr \
    -I$(JAVASCRIPTCORE_PATH)/API \
    -I$(JAVASCRIPTCORE_PATH)/ForwardingHeaders \
    -I$(JAVASCRIPTCORE_PATH)/generated

#directFB
INCLUDES += \
	-I$(DFB_DIR)/include -I$(DFB_DIR)/include/freetype2 -I$(DFB_DIR)/include/directfb -I$(DFB_DIR)/include/directfb-internal	\
	-I$(CURACAO_DIR) -I$(RUA_DIR) \
	-I$(DCCHD_DIR)/core -I$(DCCHD_DIR)/brd -I$(DCCHD_DIR)/mono -I$(DCCHD_DIR)/dvdvr -I$(DCCHD_DIR)/curacao\
	-I$(RUA_DIR) -I$(DCCHD_DIR) -I$(DCCHD_DIR)/dcchd \
	-I$(DCCHD_DIR)/dtv -I$(DCCHD_DIR)/dtv/tuner/src
RMCFLAGS +=	\
	$(INCLUDES) \
	-D_GNU_SOURCE -Wall -O3 -fPIC -ffast-math -pipe -g3 -fno-builtin-cos -fno-builtin-sin \
	-D_REENTRANT -DDATADIR=\"$(DATADIR)\" -DFONT=\"$(DATADIR)/fonts/decker.ttf\"

ifdef DO_DMALLOC
	RMCFLAGS += -DDO_DMALLOC=1
endif

#LDFLAGS += -lpthread -lssl -ldl -lm -lz -ldirectfb -lfusion -ldirect -lexpat -lssl -lcrypto -lcurl -lrt
LDFLAGS +=-lm -lz -ldirectfb -lfusion -ldirect -lpthread -ldl
ifdef DO_DMALLOC
	LDFLAGS += -ldmallocthcxx
endif
###########################################
export RMPLATFORM
export RMCFLAGS
MAKEFILE_NAME = Makefile
include $(RUA_DIR)/scripts/inc.Makefile

COPY_LIBRARY:
	@echo "***********Copy libmediasetting.so into" $(QT_INSTALL_DIR)/lib
	@if [ "`find $(TOP)/ -name *.so`" ] ; then \
        cp -f $(TOP)/*.so $(QT_INSTALL_DIR)/lib ;\
	fi ;\

install: COPY_LIBRARY


