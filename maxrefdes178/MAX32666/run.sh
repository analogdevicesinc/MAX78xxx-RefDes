openocd -s C:/Maxim/Toolchain/share/openocd/scripts -f interface/cmsis-dap.cfg -f target/max32665.cfg -c "program build/max32666.elf 0x0 verify reset exit"
