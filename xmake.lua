set_project("MdrOS")

add_rules("mode.debug", "mode.release")
add_requires("zig")
add_requires("nasm")

set_languages("c23")
set_arch("x86")

-- set_optimize("none")
-- set_policy("build.optimization.lto", true)

target("asm")
    set_kind("object")
    set_toolchains("nasm")

    add_files("src/**/*.asm")
    add_asflags("-f", "elf32", {force = true})

target("kernel")
    set_kind("binary")
    add_deps("asm")
    set_toolchains("@zig")

    add_linkdirs("lib")
    add_includedirs("src/include")

    add_links("os_terminal")
    add_files("src/**/*.c")

    add_cflags("-target x86-freestanding", {force = true})
    add_ldflags("-target x86-freestanding", {force = true})

    add_ldflags("-T", "linker.ld", {force = true})
    add_cflags("-Wno-macro-redefined", "-Wno-int-conversion", {force = true})

target("iso")
    set_kind("phony")
    add_deps("kernel")
    set_default(true)

    on_build(function (target)
        import("core.project.project")

        local iso_dir = "$(buildir)/iso"
        local target = project.target("kernel")
        os.cp(target:targetfile(), iso_dir .. "/kernel.elf")

        os.mkdir(iso_dir .. "/boot/grub")
        os.cp("grub.cfg", iso_dir .. "/boot/grub")

        local iso_file = "$(buildir)/mdros.iso"
        os.run("grub-mkrescue -o " .. iso_file .. " " .. iso_dir)
        print("ISO image created at: " .. iso_file)
    end)

target("qemu")
    set_kind("phony")
    add_deps("iso")
    set_default(false)

    on_build(function (target)
        local flags = "-serial stdio -device sb16 -m 4096"
        os.run("qemu-system-i386 -cdrom $(buildir)/mdros.iso " .. flags)
    end)