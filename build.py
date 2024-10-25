"""
CoolPotOS build.py
2024/4/9 - 2024/10/26
* 原生Python构建脚本, 已退役(现由xmake取代)
"""
import os
from threading import Thread
import time
from time import sleep

source_dir = "src"
include_dir = "src/include"

cc = "gcc"
asm = "nasm"
casm = "as"
ld = "ld"

c_flags = "-m32 -nostdlib -ffreestanding -c -Wincompatible-pointer-types -Wall -Wextra -I"+include_dir
asm_flags = "-f elf32"

build_command = "grub-mkrescue -o mdros.iso iso"

def clean():
    for file in os.listdir("target"):  # 遍历指定文件夹下所有文件
        os.remove("target/" + file)
    return 0

def get_files(directory, end):
    file_list = []
    for root, dirs, files in os.walk(directory):
        for file in files:
            if file.split(".")[1] == end:
                file_list.append(os.path.join(root, file.split(".")[0]))
    return file_list


def compile_c():
    for file in get_files(source_dir, "c"):
        if os.system(cc + " " + c_flags + " " + file+".c" + " -o " + "target/"+os.path.basename(file).split('/')[-1]+".o") != 0:
            exit(-1)


def compile_asm():
    for file in get_files(source_dir, "asm"):
        if os.system(asm + " " + asm_flags + " " + file+".asm" + " -o " + "target/"+os.path.basename(file).split('/')[-1]+".o") != 0:
            exit(-1)

def ld_obj():
    files = " "
    for file in os.listdir("target"):
        files += "target/"+file+" "
    str = ld + " -m elf_i386 -nostdlib -T linker.ld" + " -o iso/sys/cpkrnl.elf " + files
    print(str)
    if os.system(str) != 0:
        exit(-1)

#if os.system(casm + "src/boot/boot.asm -o target/boot.o") != 0:
#    exit(-1)

compile_c()
compile_asm()

os.system("cp os_terminal/libos_terminal.a target/os_terminal.o")

ld_obj()

print("Building iso...")
if os.system(build_command) != 0:
    exit(-1)

#os.system("qemu-system-i386 -monitor stdio -cdrom mdros.iso -device sb16 -net nic,model=pcnet -m 4096 -s -S")

os.system("qemu-system-i386 -cdrom mdros.iso -serial stdio -device sb16 -net nic,model=pcnet -m 4096")