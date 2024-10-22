# MdrOS Project

项目结构说明, 方便开发者移植或~~魔改~~ `CP_Kernel` 内核,
请在开发前确保您拥有这些文件夹,如不存在则需要您自己创建以保证编译通过

<hr>

* `build.py` CPOS默认一键构建脚本
* `linker.ld` 内核源码的链接脚本

## src

`CP_Kernel` 内核源码文件夹

* 详细说明 [src about](KrnlSrc.md)

## iso_img

光盘映像目录, 最终的OS ISO文件打包区域, 
这里可以添加一些内核或应用程序要用的文件, 以及移植的第三方应用程序

### boot/grub
GRUB引导器文件夹

`grub.cfg` GRUB引导配置文件

### sys
MdrOS系统文件夹

`cposkrnl.elf` 内核本体

## os_terminal

`libos_terminal.a` os_terminal静态库

## target

内核临时目标文件夹, 用于链接源文件与外部静态库

