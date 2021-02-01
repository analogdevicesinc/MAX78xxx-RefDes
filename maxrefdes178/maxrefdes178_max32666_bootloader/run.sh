openocd -s ${TOOLCHAIN_PATH}/OpenOCD/scripts -f interface/cmsis-dap.cfg -f target/maxrefdes178.cfg -c "init;halt;max32xxx mass_erase 0;exit"
#openocd -s ${TOOLCHAIN_PATH}/OpenOCD/scripts -f interface/cmsis-dap.cfg -f target/maxrefdes178.cfg -c "program build/maxrefdes178_max32666_bootloader.elf 0x0 verify reset exit"
openocd -s ${TOOLCHAIN_PATH}/OpenOCD/scripts -f interface/cmsis-dap.cfg -f target/maxrefdes178.cfg -c "program build/maxrefdes178_max32666_bootloader.bin verify reset exit 0x10000000"
