cmd_/working/kenvi/uydang/Test/prac/CLibrary/thread/duplicate_ir_buffer_full/irkernel/kernelir.mod.o := mips-linux-gnu-gcc -Wp,-MD,/working/kenvi/uydang/Test/prac/CLibrary/thread/duplicate_ir_buffer_full/irkernel/.kernelir.mod.o.d  -nostdinc -isystem /working/kenvi/SDK/DTV/867x/8670/common/mips-4.3/bin/../lib/gcc/mips-linux-gnu/4.3.2/include -Iinclude  -I/working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include -include include/linux/autoconf.h -D__KERNEL__ -D"VMLINUX_LOAD_ADDRESS=0x84000000" -D"DATAOFFSET=0" -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -Werror-implicit-function-declaration -Wno-format-security -fno-delete-null-pointer-checks -O2 -ffunction-sections -mno-check-zero-division -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -ffreestanding -EL -UMIPSEB -U_MIPSEB -U__MIPSEB -U__MIPSEB__ -UMIPSEL -U_MIPSEL -U__MIPSEL -U__MIPSEL__ -DMIPSEL -D_MIPSEL -D__MIPSEL -D__MIPSEL__ -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -D__linux__ -I/working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/mach-tango3 -DEM86XX_CHIP=EM86XX_CHIPID_TANGO3 -DEM86XX_REVISION=3 -DCPU_REMAP_SPACE=0x4000000UL -mtune=24kf -DRMCHIP_ID=RMCHIP_ID_SMP8674 -DRMCHIP_REVISION=1 -I/working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/mach-generic -fno-stack-protector -fomit-frame-pointer -Wdeclaration-after-statement -Wno-pointer-sign -fno-strict-overflow -DVIXS  -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(kernelir.mod)"  -D"KBUILD_MODNAME=KBUILD_STR(kernelir)"  -DMODULE -mlong-calls -c -o /working/kenvi/uydang/Test/prac/CLibrary/thread/duplicate_ir_buffer_full/irkernel/kernelir.mod.o /working/kenvi/uydang/Test/prac/CLibrary/thread/duplicate_ir_buffer_full/irkernel/kernelir.mod.c

deps_/working/kenvi/uydang/Test/prac/CLibrary/thread/duplicate_ir_buffer_full/irkernel/kernelir.mod.o := \
  /working/kenvi/uydang/Test/prac/CLibrary/thread/duplicate_ir_buffer_full/irkernel/kernelir.mod.c \
    $(wildcard include/config/module/unload.h) \
  include/linux/module.h \
    $(wildcard include/config/modules.h) \
    $(wildcard include/config/modversions.h) \
    $(wildcard include/config/unused/symbols.h) \
    $(wildcard include/config/generic/bug.h) \
    $(wildcard include/config/kallsyms.h) \
    $(wildcard include/config/tracepoints.h) \
    $(wildcard include/config/tracing.h) \
    $(wildcard include/config/event/tracing.h) \
    $(wildcard include/config/ftrace/mcount/record.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/constructors.h) \
    $(wildcard include/config/sysfs.h) \
  include/linux/list.h \
    $(wildcard include/config/debug/list.h) \
  include/linux/stddef.h \
  include/linux/compiler.h \
    $(wildcard include/config/trace/branch/profiling.h) \
    $(wildcard include/config/profile/all/branches.h) \
    $(wildcard include/config/enable/must/check.h) \
    $(wildcard include/config/enable/warn/deprecated.h) \
  include/linux/compiler-gcc.h \
    $(wildcard include/config/arch/supports/optimized/inlining.h) \
    $(wildcard include/config/optimize/inlining.h) \
  include/linux/compiler-gcc4.h \
  include/linux/poison.h \
    $(wildcard include/config/illegal/pointer/value.h) \
  include/linux/prefetch.h \
  include/linux/types.h \
    $(wildcard include/config/uid16.h) \
    $(wildcard include/config/lbdaf.h) \
    $(wildcard include/config/phys/addr/t/64bit.h) \
    $(wildcard include/config/64bit.h) \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/types.h \
    $(wildcard include/config/highmem.h) \
    $(wildcard include/config/64bit/phys/addr.h) \
  include/asm-generic/int-ll64.h \
  include/asm-generic/bitsperlong.h \
  include/linux/posix_types.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/posix_types.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/sgidefs.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/processor.h \
    $(wildcard include/config/32bit.h) \
    $(wildcard include/config/cpu/cavium/octeon.h) \
    $(wildcard include/config/cavium/octeon/cvmseg/size.h) \
    $(wildcard include/config/mips/mt/fpaff.h) \
    $(wildcard include/config/cpu/has/prefetch.h) \
  include/linux/cpumask.h \
    $(wildcard include/config/cpumask/offstack.h) \
    $(wildcard include/config/hotplug/cpu.h) \
    $(wildcard include/config/debug/per/cpu/maps.h) \
    $(wildcard include/config/disable/obsolete/cpumask/functions.h) \
  include/linux/kernel.h \
    $(wildcard include/config/preempt/voluntary.h) \
    $(wildcard include/config/debug/spinlock/sleep.h) \
    $(wildcard include/config/prove/locking.h) \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/dynamic/debug.h) \
    $(wildcard include/config/ring/buffer.h) \
    $(wildcard include/config/numa.h) \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/stdarg.h \
  include/linux/linkage.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/linkage.h \
  include/linux/bitops.h \
    $(wildcard include/config/generic/find/first/bit.h) \
    $(wildcard include/config/generic/find/last/bit.h) \
    $(wildcard include/config/generic/find/next/bit.h) \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/bitops.h \
    $(wildcard include/config/cpu/mipsr2.h) \
  include/linux/irqflags.h \
    $(wildcard include/config/trace/irqflags.h) \
    $(wildcard include/config/irqsoff/tracer.h) \
    $(wildcard include/config/preempt/tracer.h) \
    $(wildcard include/config/trace/irqflags/support.h) \
    $(wildcard include/config/x86.h) \
  include/linux/typecheck.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/irqflags.h \
    $(wildcard include/config/mips/mt/smtc.h) \
    $(wildcard include/config/irq/cpu.h) \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/hazards.h \
    $(wildcard include/config/cpu/mipsr1.h) \
    $(wildcard include/config/mach/alchemy.h) \
    $(wildcard include/config/cpu/loongson2.h) \
    $(wildcard include/config/cpu/r10000.h) \
    $(wildcard include/config/cpu/r5500.h) \
    $(wildcard include/config/cpu/rm9000.h) \
    $(wildcard include/config/cpu/sb1.h) \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/cpu-features.h \
    $(wildcard include/config/mips/tlb/smpid/errorepc.h) \
    $(wildcard include/config/mips/tlb/pgd/errorepc.h) \
    $(wildcard include/config/mips/sead3.h) \
    $(wildcard include/config/cpu/mipsr2/irq/vi.h) \
    $(wildcard include/config/cpu/mipsr2/irq/ei.h) \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/cpu.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/cpu-info.h \
    $(wildcard include/config/mips/mt/smp.h) \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/cache.h \
    $(wildcard include/config/mips/l1/cache/shift.h) \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/mach-generic/kmalloc.h \
    $(wildcard include/config/dma/coherent.h) \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/mach-generic/cpu-feature-overrides.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/barrier.h \
    $(wildcard include/config/cpu/has/sync.h) \
    $(wildcard include/config/tango2.h) \
    $(wildcard include/config/tango3.h) \
    $(wildcard include/config/tango4.h) \
    $(wildcard include/config/sgi/ip28.h) \
    $(wildcard include/config/cpu/has/wb.h) \
    $(wildcard include/config/weak/ordering.h) \
    $(wildcard include/config/weak/reordering/beyond/llsc.h) \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/bug.h \
    $(wildcard include/config/bug.h) \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/break.h \
    $(wildcard include/config/cpu/micromips.h) \
  include/asm-generic/bug.h \
    $(wildcard include/config/generic/bug/relative/pointers.h) \
    $(wildcard include/config/debug/bugverbose.h) \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/byteorder.h \
  include/linux/byteorder/little_endian.h \
  include/linux/swab.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/swab.h \
  include/linux/byteorder/generic.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/war.h \
    $(wildcard include/config/cpu/r4000/workarounds.h) \
    $(wildcard include/config/cpu/r4400/workarounds.h) \
    $(wildcard include/config/cpu/daddi/workarounds.h) \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/mach-tango3/war.h \
  include/asm-generic/bitops/non-atomic.h \
  include/asm-generic/bitops/fls64.h \
  include/asm-generic/bitops/ffz.h \
  include/asm-generic/bitops/find.h \
  include/asm-generic/bitops/sched.h \
  include/asm-generic/bitops/hweight.h \
  include/asm-generic/bitops/ext2-non-atomic.h \
  include/asm-generic/bitops/le.h \
  include/asm-generic/bitops/ext2-atomic.h \
  include/asm-generic/bitops/minix.h \
  include/linux/log2.h \
    $(wildcard include/config/arch/has/ilog2/u32.h) \
    $(wildcard include/config/arch/has/ilog2/u64.h) \
  include/linux/ratelimit.h \
  include/linux/param.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/param.h \
    $(wildcard include/config/hz.h) \
  include/linux/dynamic_debug.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/div64.h \
  include/asm-generic/div64.h \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/linux/bitmap.h \
  include/linux/string.h \
    $(wildcard include/config/binary/printf.h) \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/string.h \
    $(wildcard include/config/cpu/r3000.h) \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/cachectl.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/mipsregs.h \
    $(wildcard include/config/cpu/vr41xx.h) \
    $(wildcard include/config/page/size/4kb.h) \
    $(wildcard include/config/page/size/8kb.h) \
    $(wildcard include/config/page/size/16kb.h) \
    $(wildcard include/config/page/size/32kb.h) \
    $(wildcard include/config/page/size/64kb.h) \
    $(wildcard include/config/hugetlb/page.h) \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/prefetch.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/system.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/addrspace.h \
    $(wildcard include/config/cpu/r8000.h) \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/mach-tango3/spaces.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/cmpxchg.h \
  include/asm-generic/cmpxchg-local.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/dsp.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/watch.h \
    $(wildcard include/config/hardware/watchpoints.h) \
  include/linux/stat.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/stat.h \
  include/linux/time.h \
    $(wildcard include/config/arch/uses/gettimeoffset.h) \
  include/linux/cache.h \
    $(wildcard include/config/arch/has/cache/line/size.h) \
  include/linux/seqlock.h \
  include/linux/spinlock.h \
    $(wildcard include/config/debug/spinlock.h) \
    $(wildcard include/config/generic/lockbreak.h) \
    $(wildcard include/config/preempt.h) \
    $(wildcard include/config/debug/lock/alloc.h) \
  include/linux/preempt.h \
    $(wildcard include/config/debug/preempt.h) \
    $(wildcard include/config/preempt/notifiers.h) \
  include/linux/thread_info.h \
    $(wildcard include/config/compat.h) \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/thread_info.h \
    $(wildcard include/config/debug/stack/usage.h) \
    $(wildcard include/config/mips32/o32.h) \
    $(wildcard include/config/mips32/n32.h) \
  include/linux/stringify.h \
  include/linux/bottom_half.h \
  include/linux/spinlock_types.h \
  include/linux/spinlock_types_up.h \
  include/linux/lockdep.h \
    $(wildcard include/config/lockdep.h) \
    $(wildcard include/config/lock/stat.h) \
    $(wildcard include/config/generic/hardirqs.h) \
  include/linux/spinlock_up.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/atomic.h \
  include/asm-generic/atomic-long.h \
  include/linux/spinlock_api_up.h \
  include/linux/math64.h \
  include/linux/kmod.h \
  include/linux/gfp.h \
    $(wildcard include/config/kmemcheck.h) \
    $(wildcard include/config/zone/dma.h) \
    $(wildcard include/config/zone/dma32.h) \
    $(wildcard include/config/debug/vm.h) \
  include/linux/mmzone.h \
    $(wildcard include/config/force/max/zoneorder.h) \
    $(wildcard include/config/memory/hotplug.h) \
    $(wildcard include/config/sparsemem.h) \
    $(wildcard include/config/arch/populates/node/map.h) \
    $(wildcard include/config/discontigmem.h) \
    $(wildcard include/config/flat/node/mem/map.h) \
    $(wildcard include/config/cgroup/mem/res/ctlr.h) \
    $(wildcard include/config/have/memory/present.h) \
    $(wildcard include/config/need/node/memmap/size.h) \
    $(wildcard include/config/need/multiple/nodes.h) \
    $(wildcard include/config/have/arch/early/pfn/to/nid.h) \
    $(wildcard include/config/flatmem.h) \
    $(wildcard include/config/sparsemem/extreme.h) \
    $(wildcard include/config/nodes/span/other/nodes.h) \
    $(wildcard include/config/holes/in/zone.h) \
    $(wildcard include/config/arch/has/holes/memorymodel.h) \
  include/linux/wait.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/current.h \
  include/linux/numa.h \
    $(wildcard include/config/nodes/shift.h) \
  include/linux/init.h \
    $(wildcard include/config/hotplug.h) \
  include/linux/nodemask.h \
  include/linux/pageblock-flags.h \
    $(wildcard include/config/hugetlb/page/size/variable.h) \
  include/linux/bounds.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/page.h \
    $(wildcard include/config/cpu/mips32.h) \
  include/linux/const.h \
  include/linux/pfn.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/io.h \
    $(wildcard include/config/dma/noncoherent.h) \
  include/asm-generic/iomap.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/pgtable-bits.h \
    $(wildcard include/config/cpu/tx39xx.h) \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/mach-generic/ioremap.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/mach-generic/mangle-port.h \
    $(wildcard include/config/swap/io/space.h) \
  include/asm-generic/memory_model.h \
    $(wildcard include/config/sparsemem/vmemmap.h) \
  include/asm-generic/getorder.h \
  include/linux/memory_hotplug.h \
    $(wildcard include/config/have/arch/nodedata/extension.h) \
    $(wildcard include/config/memory/hotremove.h) \
  include/linux/notifier.h \
  include/linux/errno.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/errno.h \
  include/asm-generic/errno-base.h \
  include/linux/mutex.h \
    $(wildcard include/config/debug/mutexes.h) \
  include/linux/rwsem.h \
    $(wildcard include/config/rwsem/generic/spinlock.h) \
  include/linux/rwsem-spinlock.h \
  include/linux/srcu.h \
  include/linux/topology.h \
    $(wildcard include/config/sched/smt.h) \
    $(wildcard include/config/sched/mc.h) \
  include/linux/smp.h \
    $(wildcard include/config/use/generic/smp/helpers.h) \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/topology.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/mach-generic/topology.h \
  include/asm-generic/topology.h \
  include/linux/mmdebug.h \
    $(wildcard include/config/debug/virtual.h) \
  include/linux/elf.h \
  include/linux/elf-em.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/elf.h \
    $(wildcard include/config/mips32/compat.h) \
  include/linux/kobject.h \
  include/linux/sysfs.h \
  include/linux/kref.h \
  include/linux/moduleparam.h \
    $(wildcard include/config/alpha.h) \
    $(wildcard include/config/ia64.h) \
    $(wildcard include/config/ppc64.h) \
  include/linux/tracepoint.h \
  include/linux/rcupdate.h \
    $(wildcard include/config/tree/preempt/rcu.h) \
    $(wildcard include/config/tree/rcu.h) \
  include/linux/completion.h \
  include/linux/rcutree.h \
    $(wildcard include/config/no/hz.h) \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/local.h \
  include/linux/percpu.h \
    $(wildcard include/config/have/legacy/per/cpu/area.h) \
    $(wildcard include/config/need/per/cpu/embed/first/chunk.h) \
    $(wildcard include/config/need/per/cpu/page/first/chunk.h) \
    $(wildcard include/config/debug/kmemleak.h) \
    $(wildcard include/config/have/setup/per/cpu/area.h) \
  include/linux/slab.h \
    $(wildcard include/config/slab/debug.h) \
    $(wildcard include/config/debug/objects.h) \
    $(wildcard include/config/slub.h) \
    $(wildcard include/config/slob.h) \
    $(wildcard include/config/debug/slab.h) \
  include/linux/slab_def.h \
    $(wildcard include/config/kmemtrace.h) \
  include/linux/kmemtrace.h \
  include/trace/events/kmem.h \
  include/trace/define_trace.h \
  include/linux/kmalloc_sizes.h \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/percpu.h \
  include/asm-generic/percpu.h \
  include/linux/percpu-defs.h \
    $(wildcard include/config/debug/force/weak/per/cpu.h) \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/module.h \
    $(wildcard include/config/cpu/mips32/r1.h) \
    $(wildcard include/config/cpu/mips32/r2.h) \
    $(wildcard include/config/cpu/mips64/r1.h) \
    $(wildcard include/config/cpu/mips64/r2.h) \
    $(wildcard include/config/cpu/r4300.h) \
    $(wildcard include/config/cpu/r4x00.h) \
    $(wildcard include/config/cpu/tx49xx.h) \
    $(wildcard include/config/cpu/r5000.h) \
    $(wildcard include/config/cpu/r5432.h) \
    $(wildcard include/config/cpu/r6000.h) \
    $(wildcard include/config/cpu/nevada.h) \
    $(wildcard include/config/cpu/rm7000.h) \
  /working/kenvi/SDK/DTV/867x/8674/4.0.0/smp86xx_kernel_source_R2.6.32-24/linux-2.6.32/arch/mips/include/asm/uaccess.h \
    $(wildcard include/config/tangox.h) \
  include/trace/events/module.h \
  include/linux/vermagic.h \
  include/linux/utsrelease.h \

/working/kenvi/uydang/Test/prac/CLibrary/thread/duplicate_ir_buffer_full/irkernel/kernelir.mod.o: $(deps_/working/kenvi/uydang/Test/prac/CLibrary/thread/duplicate_ir_buffer_full/irkernel/kernelir.mod.o)

$(deps_/working/kenvi/uydang/Test/prac/CLibrary/thread/duplicate_ir_buffer_full/irkernel/kernelir.mod.o):
