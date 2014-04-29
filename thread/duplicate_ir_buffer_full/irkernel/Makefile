# Ensure all is the default goal.
all:

# KERNELRELEASE is a flag for a single run of the first condition
# exported by KERNELSRCDIR/Makefile
ifndef KERNELRELEASE


#KERNELRELEASE := $(shell uname -r)
#KERNELSRCDIR   ?= /lib/modules/$(KERNELRELEASE)/build

KERNELSRCDIR ?= $(UCLINUX_KERNEL)
KERNELBUILDDIR ?= $(KERNELSRCDIR)

ifneq ($(KERNELBUILDDIR),$(KERNELSRCDIR))
	KBUILDO=O=$(KERNELBUILDDIR)
endif

all:
# call to the Makefile in KERNELSRCDIR, which will return here
	$(MAKE) -C $(KERNELSRCDIR) $(KBUILDO) M=$(CURDIR) modules

clean:
	$(MAKE) -C $(KERNELSRCDIR) $(KBUILDO) M=$(CURDIR) clean

modules_install:
	$(MAKE) -C $(KERNELSRCDIR) $(KBUILDO) M=$(CURDIR)
	depmod -ae

else
# this branch is executed when called from KERNELSRCDIR Makefile



# VIXS
#ifeq ($(TARGET), mips)
   EXTRA_CFLAGS += -DVIXS
   CC = mips-linux-gnu-gcc
   LD = mips-linux-gnu-ld
#endif

#i386
ifeq ($(TARGET), i386)
endif


# declaration of the actual files used for the irkernel module
MOD_SRCS := irkernel.c
# two variables exported for use in the second invocation
MOD_OBJS := ${MOD_SRCS:.c=.o}
# final kernel module name
MOD_NAME := kernelir


# the following variables trigger actions in scripts/Makefile.build
$(MOD_NAME)-objs += $(MOD_OBJS)
obj-m += $(MOD_NAME).o

endif
