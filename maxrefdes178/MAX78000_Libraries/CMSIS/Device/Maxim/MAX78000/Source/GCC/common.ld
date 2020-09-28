/* Shared RISC-V and ARM memory map */
_FlashLength_ =  __FlashLength;
MEMORY {
    ROM     (rx)  : ORIGIN = 0x00000000, LENGTH = 0x00010000 /* 64kB - ROM */
    
    FLASH   (rx)  : ORIGIN = __FlashStart, LENGTH = _FlashLength_ /* 256kB - Flash - can be used by RV */

    SRAM0   (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00008000 /* 32 kB - ARM Data */

    SRAM1   (rwx) : ORIGIN = 0x20008000, LENGTH = 0x00008000 /* 32 kB - ARM Stack */
    /* SRAM2 Instance 0 */
    SRAM2   (rwx) : ORIGIN = 0x20010000, LENGTH = 0x00008000 /* 26kB SRAM2 - RV Data */
    /* SRAM2 Instance 1 */
    SRAM21  (rwx) : ORIGIN = 0x20018000, LENGTH = 0x00004000 /* 16kB SRAM2_1 - ARM Data */
    /* SRAM 3 */
    SRAM3   (rwx) : ORIGIN = 0x2001C000, LENGTH = 0x00004000 /* 16kB SRAM3 - RV Code */
}

