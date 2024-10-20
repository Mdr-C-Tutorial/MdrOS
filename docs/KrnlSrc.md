# CP_Kernel Source Code directory

`CP_Kernel` 源码文件夹, 存储内核的所有源码文件 

~~也是你魔改要操作的主要文件夹~~

## boot

汇编 / 引导源文件的文件夹

* `asmfunc.asm` 常规的内核汇编函数部分
* `boot.asm` Multiboot头各种标记的存储, 以及引导`kernel_main`函数
* `interrupt.asm` 内核中断处理程序的汇编部分
* `io.asm` 内核I/O操作函数部分

## core

内核的核心功能源文件

* `gdt.c` 全局描述符表配置源文件, 以及TSS任务状态段配置与操作
* `idt.c` 内核中断处理程序C源码部分
* `main.c` 内核初始化
* `timer.c` RTC时钟中断
* `vdisk.c` 存储设备驱动的抽象层接口
* `vfs.c` 虚拟文件系统实现

### memory

重构版本的内存管理系统

* `kmalloc.c` 内核堆实现
* `page.c` 分页系统的实现

### task

多任务系统实现

* `pcb.c` 进程源码部分
* `scheduler.c` 调度器实现

## driver

内核内置驱动程序部分

* `acpi.c` ACPI驱动程序
* `serial.c` 串口驱动程序
* `tty.c` TTY设备管理

### pci

PCI设备驱动部分

* `pci.c` PCI设备管理
* `ide.c` IDE控制器驱动程序

## video

视频驱动程序部分

* `vga.c` BIOS VGA视频驱动程序
* `vbe.c` VBE高分辨率显示模式

## fs

内核内置文件系统实现

* `devfs.c` Device File System

## util

内核工具型源码部分

* `default_terminal.c` 默认的虚拟终端环境(废弃)
* `fifo8.c` FIFO缓冲队列
* `klog.c` 内核日志实现
* `krlibc.c` 内核常用的C标准库实现, 以及内核内存分布定义(在其对应的头文件)

### data

一些数据结构算法实现

* `list.c` 双向链表实现
* `rbtree-strptr.c` 红黑树实现
* `slist_strptr.c` 单向链表实现

## include

内核源文件对应的头文件部分