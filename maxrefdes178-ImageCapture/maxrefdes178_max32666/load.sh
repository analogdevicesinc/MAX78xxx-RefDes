platform="$(uname -s)"
echo "$platform"

TOOLCHAIN_PATH="/C/MaximSDK/Tools"
export PATH=$TOOLCHAIN_PATH/GNUTools/10.3/bin:$TOOLCHAIN_PATH/xPack/riscv-none-embed-gcc/10.2.0-1.2/bin:$TOOLCHAIN_PATH/MSYS2/usr/bin:$TOOLCHAIN_PATH/OpenOCD:$PATH

OPENOCD_SCIPTS="$TOOLCHAIN_PATH/OpenOCD/scripts"
TARGET="${OPENOCD_SCIPTS}/target/max32665_nsrst.cfg"
INTERFACE="${OPENOCD_SCIPTS}/interface/cmsis-dap.cfg"

openocd -s "$OPENOCD_SCIPTS" -f $INTERFACE -f $TARGET -c "init;halt;max32xxx mass_erase 0;exit"
#openocd -s "$OPENOCD_SCIPTS" -f $INTERFACE -f $TARGET -c "program build/maxrefdes178_max32666.elf 0x0 verify reset exit"
openocd -s "$OPENOCD_SCIPTS" -f $INTERFACE -f $TARGET -c "program build/maxrefdes178_max32666.bin verify reset exit 0x10000000"
