# MdrOS Project

项目结构说明, 方便开发者移植或~~魔改~~ `CP_Kernel` 内核,
请在开发前确保您拥有这些文件夹,如不存在则需要您自己创建以保证编译通过

<hr>

* `build.py` CPOS废弃的一键构建脚本
* `xmake.lua` xmake 构建脚本
* `linker.ld` 内核源码的链接脚本

## src

`CP_Kernel` 内核源码文件夹

* 详细说明 [src about](KrnlSrc.md)

## iso

光盘映像目录, 最终的OS ISO文件打包区域, 
这里可以添加一些内核或应用程序要用的文件, 以及移植的第三方应用程序

### limine
Limine引导器文件夹

`grub.cfg` GRUB引导配置文件

### sys

MdrOS系统文件夹

`cpkrnl.elf` 内核本体

## lib
OS第三方静态库

`libos_terminal.a` os_terminal静态库

`libelf_parse.a` elf_parse静态库

## apps

OS外置应用程序以及libc实现

## target

> 仅存在于使用 `build.py` 构建脚本

内核临时目标文件夹, 用于链接源文件与外部静态库 <br>
目前来看你可以不用创建该文件夹, xmake有自己的一套输出目录


