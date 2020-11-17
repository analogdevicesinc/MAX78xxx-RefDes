#openocd -s C:/Maxim/Toolchain/share/openocd/scripts -f interface/cmsis-dap.cfg -f target/ai85.cfg -f target/ai85Base.cfg -c "program build/maxcam_max78000_video.elf 0x0 verify reset exit"
openocd -s C:/Maxim/Toolchain/share/openocd/scripts -f interface/cmsis-dap.cfg -f target/ai85.cfg -f target/ai85Base.cfg -c "program build/maxcam_max78000_video.bin verify reset exit 0x10000000"
