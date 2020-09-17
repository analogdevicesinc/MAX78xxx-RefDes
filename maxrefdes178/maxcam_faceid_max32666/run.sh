#openocd -s C:/Maxim/Toolchain/share/openocd/scripts -f interface/cmsis-dap.cfg -f target/max32665.cfg -c "program build/maxcam_faceid_max32666.elf 0x0 verify reset exit"
openocd -s C:/Maxim/Toolchain/share/openocd/scripts -f interface/cmsis-dap.cfg -f target/max32665.cfg -c "program build/maxcam_faceid_max32666.bin verify reset exit 0x10000000"
