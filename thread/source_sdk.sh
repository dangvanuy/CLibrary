SDK_DIR=/working/kenvi/SDK/DTV/867x/8670/common
SDK_DIR_SOURCE=/working/kenvi/SDK/DTV/867x/8674/4.0.0

export PATH=$PATH:$SDK_DIR/mips/bin
	
cd $SDK_DIR_SOURCE/CPU
source CPU_KEYS.env

cd $SDK_DIR_SOURCE/rootfs

#SCROOT_LOCAL=/working/kenvi/uydang/867x/8674/4.0.0/sdk
#cd $SCROOT_LOCAL/rootfs
#make clean;make
export COMPILKIND="glibc hardfloat codesourcery"
source rootfs-path.env 


export LINUX_KERNEL=$SDK_DIR_SOURCE/kernel/linux
export UCLINUX_KERNEL=$SDK_DIR_SOURCE/kernel/linux



sdk=/working/kenvi/uydang/867x/8674/4.0.0/TestForChaitu/sdk

cd $sdk/mrua
source build.env 

cd $sdk/dcchd
source DCCHD.env black

code=/working/kenvi/uydang/Test/prac/CLibrary/Thread
cd $code/duplicate_ir_buffer_full



