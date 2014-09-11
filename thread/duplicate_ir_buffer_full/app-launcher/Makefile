#!make

ifeq (,$(FLASH_DST))
	FLASH_DST=.
endif
ifneq (, $(DIRECTFB_INSTALL_DIR))
	# DirectFB 1.0
	DFB_DIR = $(DIRECTFB_INSTALL_DIR)
	DFB_LIBS = -ldirectfb -lfusion -ldirect
else
ifneq (, $(DCCHD_INSTALL_DIR))
	# DirectFB 1.4
	DFB_DIR = $(DCCHD_INSTALL_DIR)
	DFB_LIBS = -ldirectfb -lfusion -ldirect -lsawman
else
	$(error Neither DIRECTFB_INSTALL_DIR nor DCCHD_INSTALL_DIR is set)
endif
endif

CC= mipsel-linux-gcc
CXX=mipsel-linux-g++
OPTIMIZER= -g -fPIC
RMCFLAGS += \
	-I$(DCCHD_DIR)/core -I$(DCCHD_DIR)/dcchd -I$(DCCHD_DIR)/brd -I$(DCCHD_DIR)/mono -I$(DCCHD_DIR)/dvdvr -I$(DCCHD_DIR)/curacao -I$(DCCHD_DIR)/dtv \
	-I$(DCCHD_INSTALL_DIR)/include -I$(DCCHD_INSTALL_DIR)/include/freetype2 \
	-I$(DCCHD_INSTALL_DIR)/include/directfb -I$(DCCHD_INSTALL_DIR)/include/directfb-internal -I$(DCCHD_INSTALL_DIR)/include/sawman -I$(DCCHD_INSTALL_DIR)/include/divine \
	-I$(FUSION_DIR)/linux/include -I$(RUA_DIR) \
	-I$(SMP86XX_ROOTFS_PATH)/cross_rootfs/include \
	-I./include \

CFLAGS= $(RMCFLAGS) $(OPTIMIZER) -I. -I$(RUA_DIR) -I$(DCCHD_DIR)/core -I$(DCCHD_DIR)/dcchd -I$(DCCHD_DIR)/mono -I$(DFB_DIR)/include/directfb-internal -I$(DFB_DIR)/include/directfb -I../../include

CFLAGS += -D_GNU_SOURCE -Wall -fPIC -ffast-math -pipe -fno-builtin-cos -fno-builtin-sin \
	-D_REENTRANT -DFONTNAME=\"decker.ttf\"

LDFLAGS += -lm -lz

LDFLAGS += \
	-L$(DFB_DIR)/lib $(DFB_LIBS) \
	-Wl,-rpath-link -Wl,$(DFB_DIR)/lib \
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
	-Wl,-rpath-link -Wl,$(DCCHD_DIR)/dtv/network \
	-Wl,-rpath-link -Wl,$(DCCHD_DIR)/dtv/acap \
	-Wl,-rpath-link -Wl,$(DCCHD_DIR)/dcchd

ifneq '$(filter codesourcery,$(COMPILKIND))' ''			# CS G++
  ifneq '$(filter anytango3,$(COMPILKIND))' '' # generic Tango3 compilation
		CFLAGS	+= -mips32r2 -Wa,-mips32r2
  else
	  ifneq '$(filter -DXBOOT2_SMP865X=1,$(RMCFLAGS))' ''
	    # This is SMP8652 or SMP8654 with 24kf MIPS processor
	    CFLAGS += -march=24kf -mtune=24kf -Wa,-mips32r2
	  else
	    # This is SMP8644 with 74kf MIPS processor
	    CFLAGS += -march=74kf2_1 -mtune=74kf2_1 -mdspr2 -Wa,-mips32r2
	  endif
  endif
  LDFLAGS  += -L$(SMP86XX_ROOTFS_PATH)/cross_rootfs/lib  -Wl,-rpath-link -Wl,$(SMP86XX_ROOTFS_PATH)/cross_rootfs/lib
else
  CFLAGS += -mips32r2 -Wa,-mips32r2
endif


SRC= launchd/launchd.cpp launchd/FullScreenLayout.cpp launchd/app.cpp launchd/media.cpp

#TARGET= $(FLASH_DST)/bin/app_launcher
TARGET= ../app_launcher

all: app_launcher

app_launcher:
	$(CXX) -o $(TARGET) $(CFLAGS) $(SRC) $(LDFLAGS)


clean:
	$(RM) $(OBJ) $(TARGET)
