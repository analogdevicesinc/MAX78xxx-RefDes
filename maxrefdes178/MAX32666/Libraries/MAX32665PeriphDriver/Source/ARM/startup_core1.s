;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
 ; Copyright (C) 2018 Maxim Integrated Products, Inc., All Rights Reserved.
 ;
 ; Permission is hereby granted, free of charge, to any person obtaining a
 ; copy of this software and associated documentation files (the "Software"),
 ; to deal in the Software without restriction, including without limitation
 ; the rights to use, copy, modify, merge, publish, distribute, sublicense,
 ; and/or sell copies of the Software, and to permit persons to whom the
 ; Software is furnished to do so, subject to the following conditions:
 ;
 ; The above copyright notice and this permission notice shall be included
 ; in all copies or substantial portions of the Software.
 ;
 ; THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 ; OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 ; MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 ; IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 ; OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 ; ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 ; OTHER DEALINGS IN THE SOFTWARE.
 ;
 ; Except as contained in this notice, the name of Maxim Integrated
 ; Products, Inc. shall not be used except as stated in the Maxim Integrated
 ; Products, Inc. Branding Policy.
 ;
 ; The mere transfer of this software does not imply any licenses
 ; of trade secrets, proprietary technology, copyrights, patents,
 ; trademarks, maskwork rights, or any other form of intellectual
 ; property whatsoever. Maxim Integrated Products, Inc. retains all
 ; ownership rights.
 ;
 ; $Date: 2019-04-26 10:42:08 -0500 (Fri, 26 Apr 2019) $
 ; $Revision: 42955 $
 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

; To map FreeRTOS function names to their CMSIS equivalents add following lines to FreeRTOSConfig.h
; #define vPortSVCHandler SVC_Handler
; #define xPortPendSVHandler PendSV_Handler
; #define xPortSysTickHandler SysTick_Handler
; *------- <<< Use Configuration Wizard in Context Menu to Modify Stack Size and Heap Size. >>> ----

; <h> Stack Configuration
;   <o> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Stack_Size1     EQU     0x00000800

; TODO: Create another STACK AREA
                AREA    CORE1_STACK, NOINIT, READWRITE, ALIGN=3
Stack_Mem1      SPACE   Stack_Size1
__initial_sp1                                       ; ARMCC: name is set to work with MicroLib

; <h> Heap Configuration
;   <o>  Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
; </h>

Heap_Size1      EQU     0x00000800

                AREA    HEAP, NOINIT, READWRITE, ALIGN=3
__heap_base1
Heap_Mem1       SPACE   Heap_Size1
__heap_limit1

                PRESERVE8
                THUMB


; Vector Table Mapped to Address 0 at Reset

                AREA    |.text|, CODE, READONLY
                EXPORT  __Vectors1
                EXPORT  __Vectors_End1
                EXPORT  __Vectors_Size1
                EXPORT  __isr_vector_core1
                IMPORT  SysTick_Handler
				IMPORT NMI_Handler
				IMPORT HardFault_Handler
				IMPORT MemManage_Handler
				IMPORT BusFault_Handler
				IMPORT UsageFault_Handler
				IMPORT SVC_Handler
				IMPORT DebugMon_Handler
				IMPORT PendSV_Handler
				IMPORT PF_IRQHandler
				IMPORT WDT0_IRQHandler
				IMPORT USB_IRQHandler
				IMPORT RTC_IRQHandler
				IMPORT TRNG_IRQHandler
				IMPORT TMR0_IRQHandler
				IMPORT TMR1_IRQHandler
				IMPORT TMR2_IRQHandler
				IMPORT TMR3_IRQHandler
				IMPORT TMR4_IRQHandler
				IMPORT TMR5_IRQHandler
				IMPORT RSV11_IRQHandler
				IMPORT RSV12_IRQHandler
				IMPORT I2C0_IRQHandler
				IMPORT UART0_IRQHandler
				IMPORT UART1_IRQHandler
				IMPORT SPI17Y1_IRQHandler
				IMPORT SPI17Y2_IRQHandler
				IMPORT RSV18_IRQHandler
				IMPORT RSV19_IRQHandler
				IMPORT ADC_IRQHandler
				IMPORT RSV21_IRQHandler 
				IMPORT RSV22_IRQHandler
				IMPORT FLC0_IRQHandler
				IMPORT GPIO0_IRQHandler
				IMPORT GPIO1_IRQHandler
				IMPORT RSV26_IRQHandler
				IMPORT TPU_IRQHandler
				IMPORT DMA0_IRQHandler
				IMPORT DMA1_IRQHandler
				IMPORT DMA2_IRQHandler
				IMPORT DMA3_IRQHandler 
				IMPORT RSV32_IRQHandler
				IMPORT RSV33_IRQHandler
				IMPORT UART2_IRQHandler
				IMPORT RSV35_IRQHandler
				IMPORT I2C1_IRQHandler
				IMPORT RSV37_IRQHandler
				IMPORT SPIXFC_IRQHandler
				IMPORT BTLE_TX_DONE_IRQHandler
				IMPORT BTLE_RX_RCVD_IRQHandler
				IMPORT BTLE_RX_ENG_DET_IRQHandler
				IMPORT BTLE_SFD_DET_IRQHandler
				IMPORT BTLE_SFD_TO_IRQHandler
				IMPORT BTLE_GP_EVENT_IRQHandler
				IMPORT BTLE_CFO_IRQHandler
				IMPORT BTLE_SIG_DET_IRQHandler
				IMPORT BTLE_AGC_EVENT_IRQHandler
				IMPORT BTLE_RFFE_SPIM_IRQHandler
                IMPORT BTLE_TX_AES_IRQHandler  
                IMPORT BTLE_RX_AES_IRQHandler      
                IMPORT BTLE_INV_APB_ADDR_IRQHandler  
                IMPORT BTLE_IQ_DATA_VALID_IRQHandler 
                IMPORT WUT_IRQHandler                
                IMPORT GPIOWAKE_IRQHandler           
                IMPORT RSV55_IRQHandler              
                IMPORT SPI17Y0_IRQHandler            
                IMPORT WDT1_IRQHandler               
                IMPORT RSV58_IRQHandler              
                IMPORT PT_IRQHandler                 
                IMPORT SDMA0_IRQHandler               
                IMPORT RSV61_IRQHandler              
                IMPORT I2C2_IRQHandler               
                IMPORT RSV63_IRQHandler              
                IMPORT RSV64_IRQHandler              
                IMPORT RSV65_IRQHandler              
                IMPORT SDHC_IRQHandler               
                IMPORT OWM_IRQHandler                
                IMPORT DMA4_IRQHandler               
                IMPORT DMA5_IRQHandler               
                IMPORT DMA6_IRQHandler              
                IMPORT DMA7_IRQHandler               
                IMPORT DMA8_IRQHandler               
                IMPORT DMA9_IRQHandler               
                IMPORT DMA10_IRQHandler              
                IMPORT DMA11_IRQHandler              
                IMPORT DMA12_IRQHandler              
                IMPORT DMA13_IRQHandler              
                IMPORT DMA14_IRQHandler              
                IMPORT DMA15_IRQHandler              
                IMPORT USBDMA_IRQHandler            
                IMPORT WDT2_IRQHandler               
                IMPORT ECC_IRQHandler                 
                IMPORT DVS_IRQHandler                 
                IMPORT SIMO_IRQHandler                
                IMPORT SCA_IRQHandler                
                IMPORT AUDIO_IRQHandler              
                IMPORT FLC1_IRQHandler               
                IMPORT UART3_IRQHandler              
                IMPORT UART4_IRQHandler             
                IMPORT UART5_IRQHandler              
                IMPORT CameraIF_IRQHandler           
                IMPORT I3C_IRQHandler                
                IMPORT HTMR0_IRQHandler             
                IMPORT HTMR1_IRQHandler              					
					
                ; Core Level - CM4
                                                ; Most names are to help the FreeRTOS port.

__isr_vector_core1   DCD     __initial_sp1           ; Top of Stack
                DCD     Reset_Handler1          ; Reset Handler
                DCD     NMI_Handler             ; NMI Handler
                DCD     HardFault_Handler       ; Hard Fault Handler
                DCD     MemManage_Handler       ; MPU Fault Handler
                DCD     BusFault_Handler        ; Bus Fault Handler
                DCD     UsageFault_Handler      ; Usage Fault Handler
                DCD     0                       ; Reserved
                DCD     0                       ; Reserved
                DCD     0                       ; Reserved
                DCD     0                       ; Reserved
                DCD     SVC_Handler             ; SVCall Handler
                DCD     DebugMon_Handler        ; Debug Monitor Handler
                DCD     0                       ; Reserved
                DCD     PendSV_Handler          ; PendSV Handler
                DCD     SysTick_Handler         ; SysTick Handler

                ; MAX32665 Device-specific Interrupts
                DCD     PF_IRQHandler                 ; 0x10  0x0040  16: Power Fail
                DCD     WDT0_IRQHandler               ; 0x11  0x0044  17: Watchdog 0
                DCD     USB_IRQHandler                ; 0x12  0x0048  18: USB
                DCD     RTC_IRQHandler                ; 0x13  0x004C  19: RTC
                DCD     TRNG_IRQHandler               ; 0x14  0x0050  20: True Random Number Gener
                DCD     TMR0_IRQHandler               ; 0x15  0x0054  21: Timer 0
                DCD     TMR1_IRQHandler               ; 0x16  0x0058  22: Timer 1
                DCD     TMR2_IRQHandler               ; 0x17  0x005C  23: Timer 2
                DCD     TMR3_IRQHandler               ; 0x18  0x0060  24: Timer 3
                DCD     TMR4_IRQHandler               ; 0x19  0x0064  25: Timer 4
                DCD     TMR5_IRQHandler               ; 0x1A  0x0068  26: Timer 5
                DCD     RSV11_IRQHandler              ; 0x1B  0x006C  27: Reserved
                DCD     RSV12_IRQHandler              ; 0x1C  0x0070  28: Reserved
                DCD     I2C0_IRQHandler               ; 0x1D  0x0074  29: I2C0
                DCD     UART0_IRQHandler              ; 0x1E  0x0078  30: UART 0
                DCD     UART1_IRQHandler              ; 0x1F  0x007C  31: UART 1
                DCD     SPI17Y1_IRQHandler            ; 0x20  0x0080  32: SPI17Y1
                DCD     SPI17Y2_IRQHandler            ; 0x21  0x0084  33: SPI17Y2
                DCD     RSV18_IRQHandler              ; 0x22  0x0088  34: Reserved
                DCD     RSV19_IRQHandler              ; 0x23  0x008C  35: Reserved
                DCD     ADC_IRQHandler                ; 0x24  0x0090  36: ADC
                DCD     RSV21_IRQHandler              ; 0x25  0x0094  37: Reserved
                DCD     RSV22_IRQHandler              ; 0x26  0x0098  38: Reserved
                DCD     FLC0_IRQHandler               ; 0x27  0x009C  39: Flash Controller 0
                DCD     GPIO0_IRQHandler              ; 0x28  0x00A0  40: GPIO0
                DCD     GPIO1_IRQHandler              ; 0x29  0x00A4  41: GPIO1
                DCD     RSV26_IRQHandler              ; 0x2A  0x00A8  42: Reserved
                DCD     TPU_IRQHandler                ; 0x2B  0x00AC  43: TPU
                DCD     DMA0_IRQHandler               ; 0x2C  0x00B0  44: DMA0
                DCD     DMA1_IRQHandler               ; 0x2D  0x00B4  45: DMA1
                DCD     DMA2_IRQHandler               ; 0x2E  0x00B8  46: DMA2
                DCD     DMA3_IRQHandler               ; 0x2F  0x00BC  47: DMA3
                DCD     RSV32_IRQHandler              ; 0x30  0x00C0  48: Reserved
                DCD     RSV33_IRQHandler              ; 0x31  0x00C4  49: Reserved
                DCD     UART2_IRQHandler              ; 0x32  0x00C8  50: UART 2
                DCD     RSV35_IRQHandler              ; 0x33  0x00CC  51: Reserved
                DCD     I2C1_IRQHandler               ; 0x34  0x00D0  52: I2C1
                DCD     RSV37_IRQHandler              ; 0x35  0x00D4  53: Reserved
                DCD     SPIXFC_IRQHandler              ; 0x36  0x00D8  54: SPI execute in place
                DCD     BTLE_TX_DONE_IRQHandler       ; 0x37  0x00DC  55: BTLE TX Done
                DCD     BTLE_RX_RCVD_IRQHandler       ; 0x38  0x00E0  56: BTLE RX Recived
                DCD     BTLE_RX_ENG_DET_IRQHandler    ; 0x39  0x00E4  57: BTLE RX Energy Dectected 
                DCD     BTLE_SFD_DET_IRQHandler       ; 0x3A  0x00E8  58: BTLE SFD Detected
                DCD     BTLE_SFD_TO_IRQHandler        ; 0x3B  0x00EC  59: BTLE SFD Timeout
                DCD     BTLE_GP_EVENT_IRQHandler      ; 0x3C  0x00F0  60: BTLE Timestamp
                DCD     BTLE_CFO_IRQHandler           ; 0x3D  0x00F4  61: BTLE CFO Done
                DCD     BTLE_SIG_DET_IRQHandler       ; 0x3E  0x00F8  62: BTLE Signal Detected
                DCD     BTLE_AGC_EVENT_IRQHandler     ; 0x3F  0x00FC  63: BTLE AGC Event
                DCD     BTLE_RFFE_SPIM_IRQHandler     ; 0x40  0x0100  64: BTLE RFFE SPIM Done
                DCD     BTLE_TX_AES_IRQHandler        ; 0x41  0x0104  65: BTLE TX AES Done
                DCD     BTLE_RX_AES_IRQHandler        ; 0x42  0x0108  66: BTLE RX AES Done
                DCD     BTLE_INV_APB_ADDR_IRQHandler  ; 0x43  0x010C  67: BTLE Invalid APB Address
                DCD     BTLE_IQ_DATA_VALID_IRQHandler ; 0x44  0x0110  68: BTLE IQ Data Valid
                DCD     WUT_IRQHandler                ; 0x45  0x0114  69: WUT Wakeup
                DCD     GPIOWAKE_IRQHandler           ; 0x46  0x0118  70: GPIO Wakeup
                DCD     RSV55_IRQHandler              ; 0x47  0x011C  71: Reserved
                DCD     SPI17Y0_IRQHandler            ; 0x48  0x0120  72: SPI17Y0 AHB
                DCD     WDT1_IRQHandler               ; 0x49  0x0124  73: Watchdog 1
                DCD     RSV58_IRQHandler              ; 0x4A  0x0128  74: Reserved
                DCD     PT_IRQHandler                 ; 0x4B  0x012C  75: Pulse train
                DCD     SDMA0_IRQHandler               ; 0x4C  0x0130  76: Smart DMA 0s
                DCD     RSV61_IRQHandler              ; 0x4D  0x0134  77: Reserved
                DCD     I2C2_IRQHandler               ; 0x4E  0x0138  78: I2C 1
                DCD     RSV63_IRQHandler              ; 0x4F  0x013C  79: Reserved
                DCD     RSV64_IRQHandler              ; 0x50  0x0140  80: Reserved
                DCD     RSV65_IRQHandler              ; 0x51  0x0144  81: Reserved
                DCD     SDHC_IRQHandler               ; 0x52  0x0148  82: SDIO/SDHC
                DCD     OWM_IRQHandler                ; 0x53  0x014C  83: One Wire Master
                DCD     DMA4_IRQHandler               ; 0x54  0x0150  84: DMA4
                DCD     DMA5_IRQHandler               ; 0x55  0x0154  85: DMA5
                DCD     DMA6_IRQHandler               ; 0x56  0x0158  86: DMA6
                DCD     DMA7_IRQHandler               ; 0x57  0x015C  87: DMA7
                DCD     DMA8_IRQHandler               ; 0x58  0x0160  88: DMA8
                DCD     DMA9_IRQHandler               ; 0x59  0x0164  89: DMA9
                DCD     DMA10_IRQHandler              ; 0x5A  0x0168  90: DMA10
                DCD     DMA11_IRQHandler              ; 0x5B  0x016C  91: DMA11
                DCD     DMA12_IRQHandler              ; 0x5C  0x0170  92: DMA12
                DCD     DMA13_IRQHandler              ; 0x5D  0x0174  93: DMA13
                DCD     DMA14_IRQHandler              ; 0x5E  0x0178  94: DMA14
                DCD     DMA15_IRQHandler              ; 0x5F  0x017C  95: DMA15
                DCD     USBDMA_IRQHandler             ; 0x60  0x0180  96: USBDMA
                DCD     WDT2_IRQHandler               ; 0x61  0x0184  97: Watchdog Timer 2 
                DCD     ECC_IRQHandler                ; 0x62  0x0188  98: Error Correction 
                DCD     DVS_IRQHandler                ; 0x63  0x018C  99: DVS Controller 
                DCD     SIMO_IRQHandler               ; 0x64  0x0190  100: SIMO Controller 
                DCD     SCA_IRQHandler                ; 0x65  0x0194  101: SCA 
                DCD     AUDIO_IRQHandler              ; 0x66  0x0198  102: Audio subsystem 
                DCD     FLC1_IRQHandler               ; 0x67  0x019C  103: Flash Control 1 
                DCD     UART3_IRQHandler              ; 0x68  0x01A0  104: UART 3 
                DCD     UART4_IRQHandler              ; 0x69  0x01A4  105: UART 4 
                DCD     UART5_IRQHandler              ; 0x6A  0x01A8  106: UART 5 
                DCD     CameraIF_IRQHandler           ; 0x6B  0x01AC  107: Camera IF 
                DCD     I3C_IRQHandler                ; 0x6C  0x01B0  108: I3C 
                DCD     HTMR0_IRQHandler              ; 0x6D  0x01B4  109: HTimer0 
                DCD     HTMR1_IRQHandler              ; 0x6E  0x01B8  110: HTimer1 					

__isr_vector_end1

__isr_vector_Size1      EQU     __isr_vector_end1 - __isr_vector_core1
__Vectors1      EQU     __isr_vector_core1
__Vectors_End1  EQU     __isr_vector_end1
__Vectors_Size1 EQU     __isr_vector_Size1

                AREA    |.text|, CODE, READONLY

Reset_Handler1  PROC
                EXPORT  Reset_Handler1          [WEAK]
                IMPORT  PreInit_Core1
                IMPORT  SystemInit_Core1
                IMPORT  Core1_Main

                LDR     R0, =PreInit_Core1
                BLX     R0

                LDR     R0, =SystemInit_Core1
                BLX     R0

                LDR     R0, =Core1_Main
                BX      R0
__SPIN1
                WFI
                BL __SPIN1
                ENDP
; FT START



; FT END
                ALIGN

;*******************************************************************************
; User Stack and Heap initialization
;*******************************************************************************
        IF      :DEF:__MICROLIB
                EXPORT  __initial_sp1
                EXPORT  __heap_base1
                EXPORT  __heap_limit1

        ELSE    ; Not using Microlib
                IMPORT  __use_two_region_memory
                EXPORT  __user_initial_stackheap

;*******************************************************************************
; Set up the initial stack and heap
;*******************************************************************************
__user_initial_stackheap\
                PROC

                LDR     R0, =  Heap_Mem1
                LDR     R1, = (Stack_Mem1 + Stack_Size1)
                LDR     R2, = (Heap_Mem1 +  Heap_Size1)
                LDR     R3, = Stack_Mem1
                BX      LR
                ENDP

                ALIGN

                ENDIF

                END

;*******************************************************************************
; End of file.
;*******************************************************************************
