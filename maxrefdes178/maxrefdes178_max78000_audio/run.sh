#openocd -s ${TOOLCHAIN_PATH}/OpenOCD/scripts -f interface/cmsis-dap.cfg -f target/max78000.cfg -c "program build/maxrefdes178_max78000_audio.elf 0x0 verify reset exit"
openocd -s ${TOOLCHAIN_PATH}/OpenOCD/scripts -f interface/cmsis-dap.cfg -f target/max78000.cfg -c "program build/maxrefdes178_max78000_audio.bin verify reset exit 0x10000000"
