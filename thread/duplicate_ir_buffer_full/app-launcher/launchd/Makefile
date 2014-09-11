#!make
ifeq (,$(FLASH_DST))
	FLASH_DST=$(DCCHD_DIR)/../clients/flashlite
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

include ../launch.defs

SRCS=launchd.cpp FullScreenLayout.cpp app.cpp media.cpp


OBJS=$(addsuffix .o, $(basename $(SRCS)))
DEPS=$(addsuffix .d, $(basename $(SRCS)))

TARGET=launchd

# Override LDFLAGS from smpapi.defs since that is more intended for applications and
# not building the library itself.
#LDFLAGS = -L$(RUA_DIR)/lib -lrua -lllad -lgbus -lrmcw -lrmcore -lrmpthreadw
#LDFLAGS += -lruaoutput -lrmoutput -lrmhdmi -lrmedid -lrmi2c -lrmsha1 -lrmcec -lrmhsi -lrmvideoout

# These are for closed caption rendering.
#LDFLAGS += -lrmfontrender -lrmscc -lrmrtk86 -ldcc 
#LDFLAGS += -L$(SMP86XX_ROOTFS_PATH)/cross_rootfs/lib -lfreetype

CXXFLAGS += -fvisibility=hidden -Wall -Wextra -Werror 

CXXFLAGS +=     \
	-D_FILE_OFFSET_BITS=64 \
	-DLLAD_DIRECT \
	-DALLOW_OS_CODE \
	-D_REENTRANT \
	-DWITH_RM_FILE_JPEG \
	-DRMVDEMUX_LOCAL_RMOBJECT=1 \
	-DBASIC_MPEG4_FRAMEWORK=1 \
	-DBASIC_DETECTOR_FRAMEWORK=1 \
	-DWITH_THREADS \
	-DWITH_MONO=1 \
	-DNO_RUA_DEBUG

ifdef DO_DMALLOC
	RMCFLAGS += -DDO_DMALLOC=1
endif

CXXFLAGS += \
	-I$(DCCHD_DIR)/core -I$(DCCHD_DIR)/dcchd -I$(DCCHD_DIR)/brd -I$(DCCHD_DIR)/mono -I$(DCCHD_DIR)/dvdvr -I$(DCCHD_DIR)/curacao -I$(DCCHD_DIR)/dtv \
	-I$(DCCHD_INSTALL_DIR)/include -I$(DCCHD_INSTALL_DIR)/include/freetype2 \
	-I$(DCCHD_INSTALL_DIR)/include/directfb -I$(DCCHD_INSTALL_DIR)/include/directfb-internal -I$(DCCHD_INSTALL_DIR)/include/sawman -I$(DCCHD_INSTALL_DIR)/include/divine \
	-I$(FUSION_DIR)/linux/include -I$(RUA_DIR) \
	-I$(SMP86XX_ROOTFS_PATH)/cross_rootfs/include \
	-I$(LAUNCH_DIR)/include \
	-I$(FLASH_DST)/include
	


LIBS = -lpthread -ldl
ifdef DO_DMALLOC
	LIBS += -ldmallocthcxx
endif

CXXFLAGS += -D_GNU_SOURCE -Wall -fPIC -ffast-math -pipe -fno-builtin-cos -fno-builtin-sin \
	-D_REENTRANT -DFONTNAME=\"decker.ttf\"

ifneq '$(filter release,$(COMPILKIND))' ''
  CXXFLAGS += -O2 -g
else
  CXXFLAGS += -O0 -g3
endif

LDFLAGS += -lm -lz $(FLASH_DST)/lib/libAELoader.a $(FLASH_DST)/lib/libAEModule.a

LDFLAGS += \
	-L$(DCCHD_INSTALL_DIR)/lib \
	-Wl,-rpath-link -Wl,$(DCCHD_INSTALL_DIR)/lib \
	-Wl,-rpath-link -Wl,$(RUA_DIR)/lib

ifneq '$(filter codesourcery,$(COMPILKIND))' ''			# CS G++
  ifneq '$(filter -DXBOOT2_SMP865X=1,$(RMCFLAGS))' ''
    # This is SMP8652 or SMP8654 with 24kf MIPS processor
    CXXFLAGS += -EL -march=24kf -mtune=24kf -Wa,-mips32r2
  else
    # This is SMP8644 with 74kf MIPS processor
    CXXFLAGS += -EL -march=74kf2_1 -mtune=74kf2_1 -mdspr2 -Wa,-mips32r2
  endif
  LDFLAGS  += -EL -L$(SMP86XX_ROOTFS_PATH)/cross_rootfs/lib  -Wl,-rpath-link -Wl,$(SMP86XX_ROOTFS_PATH)/cross_rootfs/lib
else
  CXXFLAGS += -mips32r2 -Wa,-mips32r2
endif
.PHONY: all
all: install

.PHONY: install
install: $(TARGET) 
	mkdir -p $(LAUNCH_BINDIR)
	cp $(TARGET) $(LAUNCH_BINDIR)

include ../launch.rules

$(TARGET): $(OBJS)
	$(QUIET)echo "  [link]    $@"
	$(Q)$(CC) -o $@ $^ -ldirectfb -lfusion -ldirect -lsawman -ldivine -ltinyxml $(LDFLAGS) $(LIBS)
#	$(Q)$(CC) -o $@ $(CFLAGS) $^ $(LDFLAGS)

.PHONY: doc
doc:
	$(QUIET)echo "  [gen docs]  "
	$(Q)doxygen dox/doxyfile > /dev/null

.PHONY: doc.internal
doc.internal:
	$(QUIET)echo "  [gen docs.internal]  "
	$(Q)doxygen dox/doxyfile.internal > /dev/null

-include $(DEPS)
