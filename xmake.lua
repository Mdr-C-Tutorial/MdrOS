set_project("MdrOS")

add_rules("mode.debug", "mode.release")
add_requires("zig", "nasm")

set_optimize("none")

target("asm")
    set_kind("object")
    set_toolchains("nasm")

    add_files("src/**/*.asm")
    add_asflags("-f", "elf32", {force = true})

target("kernel")
    set_kind("binary")
    add_deps("asm")
    set_languages("c23")
    set_toolchains("zig")

    add_links("os_terminal")
    add_files("src/**/*.c")

    add_cflags("-target x86-freestanding", {force = true})
    add_ldflags("-target x86-freestanding", {force = true})

    add_ldflags("-T", "linker.ld", {force = true})
    add_cflags("-Wno-macro-redefined", "-Wno-int-conversion", {force = true})
    add_cflags("-Wno-incompatible-pointer-types", {force = true})
    add_cflags("-O0", {force = true})

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
        os.cp(target:targetfile(), iso_dir .. "/sys/cpkrnl.elf")

        local iso_file = "$(buildir)/mdros.iso"
        os.run("grub-mkrescue -o " .. iso_file .. " " .. iso_dir)
        print("ISO image created at: " .. iso_file)
    end)

target("qemu")
    set_kind("phony")
    add_deps("iso")
    set_default(false)

    on_build(function (target)
        local flags = "-device sb16 -m 4096"
        os.run("qemu-system-i386 -cdrom $(buildir)/mdros.iso " .. flags)
    end)

target("qemudebug")
    set_kind("phony")
        add_deps("iso")
        set_default(false)

        on_build(function (target)
            local flags = "-device sb16 -m 4096 -S -s"
            os.run("qemu-system-i386 -cdrom $(buildir)/mdros.iso " .. flags)
        end)