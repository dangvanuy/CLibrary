setxenv y.ipaddr 10.60.1.193;setxenv y.subnetmask 255.255.255.0;setxenv y.gateway 10.60.1.1
$commit


net init;net up;load -b tftp://10.60.1.248/uydang/867x/vmlinux_2.6.32-24.bin 0x84000000;go
net init;net up;load -b tftp://10.60.1.248/uydang/867x/8674/vmlinux_sqlite.bin 0x84000000;go
net init;net up;load -b tftp://10.60.1.248/uydang/867x/8674/vmlinux_sql_metadata.bin 0x84000000;go


mknod /dev/mum0 c 126 0
mknod /dev/em8xxx0 c 127 0
mknod /dev/ir c 253 0

ifconfig eth0 10.60.1.193 netmask 255.255.255.0
#Set dns1 and dns2 
echo "nameserver 10.60.1.21> /etc/resolv.conf
echo "nameserver 10.10.9.28>> /etc/resolv.conf 
#Set gateway
dgw=`route | grep default | awk '{print $2}'`
if [ $dgw ]; then route delete default gw $dgw dev eth0; fi
route add default gw 10.60.1.1 dev eth0
echo "nameserver 8.8.8.8" > /etc/resolv.conf

mkdir /working
mount -o nolock 10.60.1.248:/working /working

export LLAD_PARAMS="max_dmapool_memory_size=0x800000 max_dmabuffer_log2_size=19"
export MRUA_PACKAGE_DIR="/working/kenvi/uydang/867x/8674/4.0.0/dvbs2/sdk/mrua"
export MRUAFW_PACKAGE_DIR="/working/kenvi/uydang/867x/8674/4.0.0/dvbs2/sdk/mruafw"



export sdk=/working/kenvi/uydang/867x/8674/4.0.0/TestForChaitu/sdk

cd $sdk/mrua
source run.env
unload_imat.bash
fw_reload

#modprobe irkernel

cd $sdk/dcchd
source trun.env

code=/working/kenvi/uydang/Test/prac/CLibrary/Thread
cd $code


cd $code/duplicate_ir_buffer_full
rmmod kernelir
insmod irkernel/kernelir.ko wait_period=300

./MainApp&
