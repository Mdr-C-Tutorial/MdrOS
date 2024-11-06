set_project("MdrOS")

add_rules("mode.debug", "mode.release")
add_requires("zig", "nasm")

set_optimize("fastest")
set_warnings("all", "extra")

target("kernel")
    set_kind("binary")
    set_languages("c23")
    set_toolchains("@zig", "nasm")

    add_linkdirs("lib")
    add_includedirs("src/include")

    add_links("os_terminal")
    add_files("src/**/*.asm", "src/**/*.c")

    add_asflags("-f", "elf32", {force = true})
    add_cflags("-target x86-freestanding", {force = true})
    add_ldflags("-target x86-freestanding", {force = true})

    add_ldflags("-T", "linker.ld", {force = true})
    add_cflags("-m32", "-mno-80387", "-mno-mmx", "-mno-sse", "-mno-sse2", {force = true})

target("iso")
    set_kind("phony")
    add_deps("kernel")
    set_default(true)

    on_build(function (target)
        import("core.project.project")
        local iso_dir = "$(buildir)/iso"

        if os.exists(iso_dir) then
            os.rmdir(iso_dir)
        end

        os.cp("iso", iso_dir)
        local target = project.target("kernel")
        os.cp(target:targetfile(), iso_dir .. "/cpkrnl.elf")

        local iso_file = "$(buildir)/mdros.iso"
        local xorriso_flags = "-b limine/limine-bios-cd.bin -no-emul-boot -boot-info-table"
        os.run("xorriso -as mkisofs %s %s -o %s", xorriso_flags, iso_dir, iso_file)
        print("ISO image created at: " .. iso_file)
    end)

target("qemu")
    set_kind("phony")
    add_deps("iso")
    set_default(false)

    on_build(function (target)
        local flags = "-device sb16 -m 4096 -no-reboot"
        os.exec("qemu-system-i386 -cdrom $(buildir)/mdros.iso %s", flags)
    end)