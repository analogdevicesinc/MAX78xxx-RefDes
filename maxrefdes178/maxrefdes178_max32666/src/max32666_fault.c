/*******************************************************************************
 * Copyright (C) Maxim Integrated Products, Inc., All rights Reserved.
 *
 * This software is protected by copyright laws of the United States and
 * of foreign countries. This material may also be protected by patent laws
 * and technology transfer regulations of the United States and of foreign
 * countries. This software is furnished under a license agreement and/or a
 * nondisclosure agreement and may only be used or reproduced in accordance
 * with the terms of those agreements. Dissemination of this information to
 * any party or parties not specified in the license agreement and/or
 * nondisclosure agreement is expressly prohibited.
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL MAXIM INTEGRATED BE LIABLE FOR ANY CLAIM, DAMAGES
 * OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of Maxim Integrated
 * Products, Inc. shall not be used except as stated in the Maxim Integrated
 * Products, Inc. Branding Policy.
 *
 * The mere transfer of this software does not imply any licenses
 * of trade secrets, proprietary technology, copyrights, patents,
 * trademarks, maskwork rights, or any other form of intellectual
 * property whatsoever. Maxim Integrated Products, Inc. retains all
 * ownership rights.
 *******************************************************************************
 */
// https://github.com/ferenc-nemeth/arm-hard-fault-handler

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <mxc_sys.h>

#include "max32666_debug.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME   "fault"

/* Bit masking. */
#define CHECK_BIT(REG, POS) ((REG) & (1u << (POS)))

/* Hard Fault Status Register. */
#define FORCED              ((uint8_t)30u)
#define VECTTBL             ((uint8_t)1u)

/* MemManage Fault Status Register (MMFSR; 0-7 bits in CFSR). */
#define MMARVALID           ((uint8_t)7u)
#define MLSPERR             ((uint8_t)5u)   /**< Only on ARM Cortex-M4F. */
#define MSTKERR             ((uint8_t)4u)
#define MUNSTKERR           ((uint8_t)3u)
#define DACCVIOL            ((uint8_t)1u)
#define IACCVIOL            ((uint8_t)0u)

/* Bus Fault Status Register (BFSR; 8-15 bits in CFSR). */
#define BFARVALID           ((uint8_t)15u)
#define LSPERR              ((uint8_t)13u)  /**< Only on ARM Cortex-M4F. */
#define STKERR              ((uint8_t)12u)
#define UNSTKERR            ((uint8_t)11u)
#define IMPRECISERR         ((uint8_t)10u)
#define PRECISERR           ((uint8_t)9u)
#define IBUSERR             ((uint8_t)8u)

/* Usage Fault Status Register (BFSR; 16-25 bits in CFSR). */
#define DIVBYZERO           ((uint8_t)25u)  /**< Has to be enabled in CCR. */
#define UNALIGNED           ((uint8_t)24u)  /**< Has to be enabled in CCR. */
#define NOCP                ((uint8_t)19u)
#define INVPC               ((uint8_t)18u)
#define INVSTATE            ((uint8_t)17u)
#define UNDEFINSTR          ((uint8_t)16u)


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
// Create a hard fault with writing data to the 'External Device' part (0xA0000000-0xDFFFFFFF).
// (*((volatile uint32_t *)(0xAAAAAAAA))) = 0x1234567;
// arm-none-eabi-objdump -S .elf
void ReportHardFault(uint32_t *stack_frame, uint32_t exc)
{
    uint32_t r0   = stack_frame[0];
    uint32_t r1   = stack_frame[1];
    uint32_t r2   = stack_frame[2];
    uint32_t r3   = stack_frame[3];
    uint32_t r12  = stack_frame[4];
    uint32_t lr   = stack_frame[5];
    uint32_t pc   = stack_frame[6];
    uint32_t psr  = stack_frame[7];
    uint32_t hfsr = SCB->HFSR;
    uint32_t cfsr = SCB->CFSR;
    uint32_t mmfar = SCB->MMFAR;
    uint32_t bfar = SCB->BFAR;
    uint32_t afsr = SCB->AFSR;

    printf("\n\n\n\n!!!!!\nCore %d Hard Fault\n!!!!!\n\n\n\n", (SCB->VTOR == (unsigned long)&__isr_vector_core1));

    printf("\nStack frame:\n");
    printf("R0 :        0x%08lX\n", r0);
    printf("R1 :        0x%08lX\n", r1);
    printf("R2 :        0x%08lX\n", r2);
    printf("R3 :        0x%08lX\n", r3);
    printf("R12:        0x%08lX\n", r12);
    printf("LR :        0x%08lX\n", lr);
    printf("PC :        0x%08lX\n", pc);
    printf("PSR:        0x%08lX\n", psr);

    printf("\nFault status:\n");
    printf("HFSR:       0x%08lX\n", hfsr);
    printf("CFSR:       0x%08lX\n", cfsr);
    printf("MMFAR:      0x%08lX\n", mmfar);
    printf("BFAR:       0x%08lX\n", bfar);
    printf("AFSR:       0x%08lX\n", afsr);

    printf("\nOther:\n");
    printf("EXC_RETURN: 0x%08lX\n", exc);

    printf("\nDetails of the fault status:\n");
    printf("Hard fault status:\n");
    if (CHECK_BIT(hfsr, FORCED))
    {
        printf(" - Forced Hard fault.\n");
    }
    if (CHECK_BIT(hfsr, VECTTBL))
    {
        printf(" - Bus fault on vector table read.\n");
    }
    printf("MemManage fault status:\n");
    if (CHECK_BIT(cfsr, MMARVALID))
    {
        printf(" - MMFAR holds a valid address.\n");
    }
    else
    {
        printf(" - MMFAR holds an invalid address.\n");
    }
    if (CHECK_BIT(cfsr, MLSPERR))
    {
        printf(" - Fault occurred during floating-point lazy state preservation.\n");
    }
    if (CHECK_BIT(cfsr, MSTKERR))
    {
        printf(" - Stacking has caused an access violation.\n");
    }
    if (CHECK_BIT(cfsr, MUNSTKERR))
    {
        printf(" - Unstacking has caused an access violation.\n");
    }
    if (CHECK_BIT(cfsr, DACCVIOL))
    {
        printf(" - Load or store at a location that does not permit the operation.\n");
    }
    if (CHECK_BIT(cfsr, IACCVIOL))
    {
        printf(" - Instruction fetch from a location that does not permit execution.\n");
    }
    printf("Bus fault status:\n");
    if (CHECK_BIT(cfsr, BFARVALID))
    {
        printf(" - BFAR holds a valid address.\n");
    }
    else
    {
        printf(" - BFAR holds an invalid address.\n");
    }
    if (CHECK_BIT(cfsr, LSPERR))
    {
        printf(" - Fault occurred during floating-point lazy state preservation.\n");
    }
    if (CHECK_BIT(cfsr, STKERR))
    {
        printf(" - Stacking has caused a Bus fault.\n");
    }
    if (CHECK_BIT(cfsr, UNSTKERR))
    {
        printf(" - Unstacking has caused a Bus fault.\n");
    }
    if (CHECK_BIT(cfsr, IMPRECISERR))
    {
        printf(" - Data bus error has occurred, but the return address in the stack is not related to the fault.\n");
    }
    if (CHECK_BIT(cfsr, PRECISERR))
    {
        printf(" - Data bus error has occurred, and the return address points to the instruction that caused the fault.\n");
    }
    if (CHECK_BIT(cfsr, IBUSERR))
    {
        printf(" - Instruction bus error.\n");
    }
    printf("Usage fault status:\n");
    if (CHECK_BIT(cfsr, DIVBYZERO))
    {
        printf(" - The processor has executed an SDIV or UDIV instruction with a divisor of 0.\n");
    }
    if (CHECK_BIT(cfsr, UNALIGNED))
    {
        printf(" - The processor has made an unaligned memory access.\n");
    }
    if (CHECK_BIT(cfsr, NOCP))
    {
        printf(" - Attempted to access a coprocessor.\n");
    }
    if (CHECK_BIT(cfsr, INVPC))
    {
        printf(" - Illegal attempt to load of EXC_RETURN to the PC.\n");
    }
    if (CHECK_BIT(cfsr, INVSTATE))
    {
        printf(" - Attempted to execute an instruction that makes illegal use of the EPSR.\n");
    }
    if (CHECK_BIT(cfsr, INVSTATE))
    {
        printf(" - The processor has attempted to execute an undefined instruction.\n");
    }

    printf("\n\n\n");

    // Wait a little before reset to avoid reset loop
    for (unsigned int i = 0; i < 30000000; i++) {}
    MXC_GCR->rstr0 = 0xffffffff;
}

void HardFault_Handler(void)
{
    //    __asm__("BKPT");

    __asm volatile
    (
        "TST    LR, #0b0100;      "
        "ITE    EQ;               "
        "MRSEQ  R0, MSP;          "
        "MRSNE  R0, PSP;          "
        "MOV    R1, LR;           "
        "B      ReportHardFault;  "
    );
}
