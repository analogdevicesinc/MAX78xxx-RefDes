openocd -s C:/Maxim/Toolchain/share/openocd/scripts -f interface/cmsis-dapMXIM0000f3f8c34900000000000000000000000097969904.cfg -f target/maxcam.cfg -c  "init;halt;max32xxx mass_erase 0;exit"
#openocd -s C:/Maxim/Toolchain/share/openocd/scripts -f interface/cmsis-dap.cfg -f target/maxcam.cfg -c "program build/maxcam_faceid_max32666.elf 0x0 verify reset exit"
openocd -s C:/Maxim/Toolchain/share/openocd/scripts -f interface/cmsis-dapMXIM0000f3f8c34900000000000000000000000097969904.cfg -f target/maxcam.cfg -c  "program build/maxcam_faceid_max32666.bin verify reset exit 0x10000000"
sleep 5