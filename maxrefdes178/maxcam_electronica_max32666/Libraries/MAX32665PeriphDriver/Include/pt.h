/**
 * @file  pt.h
 * @brief Pulse Train data types, definitions and function prototypes.
 */

/* *****************************************************************************
 * Copyright (C) 2016 Maxim Integrated Products, Inc., All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
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
 *
 * $Date: 2018-11-05 22:40:08 +0300 (Mon, 05 Nov 2018) $
 * $Revision: 38949 $
 *
 ***************************************************************************** */

/* Define to prevent redundant inclusion */
#ifndef _PT_H_
#define _PT_H_

/* **** Includes **** */
#include "mxc_config.h"
#include "ptg_regs.h"
#include "pt_regs.h"
#include "mxc_assert.h"
#include "mxc_sys.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @ingroup periphlibs
 * @defgroup pt Pulse Train Engine (PT)
 * @brief This is the high level API for the pulse train engine.
 * @{
 */

/** @brief Pulse Train ENable enumeration.  */
typedef enum {
    ENABLE_PT0       = MXC_F_PTG_ENABLE_PT0,         /**< Enable PT0 */
    ENABLE_PT1       = MXC_F_PTG_ENABLE_PT1,         /**< Enable PT1 */
    ENABLE_PT2       = MXC_F_PTG_ENABLE_PT2,         /**< Enable PT2 */
    ENABLE_PT3       = MXC_F_PTG_ENABLE_PT3,         /**< Enable PT3 */
    ENABLE_PT4       = MXC_F_PTG_ENABLE_PT4,         /**< Enable PT4 */
    ENABLE_PT5       = MXC_F_PTG_ENABLE_PT5,         /**< Enable PT5 */
    ENABLE_PT6       = MXC_F_PTG_ENABLE_PT6,         /**< Enable PT6 */
    ENABLE_PT7       = MXC_F_PTG_ENABLE_PT7,         /**< Enable PT7 */
    ENABLE_PT8       = MXC_F_PTG_ENABLE_PT8,         /**< Enable PT8 */
    ENABLE_PT9       = MXC_F_PTG_ENABLE_PT9,         /**< Enable PT9 */
    ENABLE_PT10      = MXC_F_PTG_ENABLE_PT10,        /**< Enable PT10 */
    ENABLE_PT11      = MXC_F_PTG_ENABLE_PT11,        /**< Enable PT11 */
    ENABLE_PT12      = MXC_F_PTG_ENABLE_PT12,        /**< Enable PT12 */
    ENABLE_PT13      = MXC_F_PTG_ENABLE_PT13,        /**< Enable PT13 */
    ENABLE_PT14      = MXC_F_PTG_ENABLE_PT14,        /**< Enable PT14 */
    ENABLE_PT15      = MXC_F_PTG_ENABLE_PT15,        /**< Enable PT15 */
} sys_pt_en_t;

/** @brief Pulse Train Resync enumeration.  */
typedef enum {
    RESYNC_PT0       = MXC_F_PTG_RESYNC_PT0,         /**< Resync PT0 */
    RESYNC_PT1       = MXC_F_PTG_RESYNC_PT1,         /**< Resync PT1 */
    RESYNC_PT2       = MXC_F_PTG_RESYNC_PT2,         /**< Resync PT2 */
    RESYNC_PT3       = MXC_F_PTG_RESYNC_PT3,         /**< Resync PT3 */
    RESYNC_PT4       = MXC_F_PTG_RESYNC_PT4,         /**< Resync PT4 */
    RESYNC_PT5       = MXC_F_PTG_RESYNC_PT5,         /**< Resync PT5 */
    RESYNC_PT6       = MXC_F_PTG_RESYNC_PT6,         /**< Resync PT6 */
    RESYNC_PT7       = MXC_F_PTG_RESYNC_PT7,         /**< Resync PT7 */
    RESYNC_PT8       = MXC_F_PTG_RESYNC_PT8,         /**< Resync PT8 */
    RESYNC_PT9       = MXC_F_PTG_RESYNC_PT9,         /**< Resync PT9 */
    RESYNC_PT10      = MXC_F_PTG_RESYNC_PT10,        /**< Resync PT10 */
    RESYNC_PT11      = MXC_F_PTG_RESYNC_PT11,        /**< Resync PT11 */
    RESYNC_PT12      = MXC_F_PTG_RESYNC_PT12,        /**< Resync PT12 */
    RESYNC_PT13      = MXC_F_PTG_RESYNC_PT13,        /**< Resync PT13 */
    RESYNC_PT14      = MXC_F_PTG_RESYNC_PT14,        /**< Resync PT14 */
    RESYNC_PT15      = MXC_F_PTG_RESYNC_PT15,        /**< Resync PT15 */
} sys_pt_resync_t;
/**
 * Structure type for pulse train mode configuration.
 * @note       Do not use for square wave
 */
typedef struct {
    uint32_t bps;           /**< pulse train bit rate */
    uint32_t pattern;       /**< Output pattern to shift out, starts at LSB */
    uint8_t ptLength;       /**< Number of bits in pulse train, 0 = 32bits, 1 = non valid , 2 = 2 bits, ... */
    uint16_t loop;          /**< Number of times to repeat the train, 0 = continuous */
    uint16_t loopDelay;     /**< Delay between loops specified in multiples of bps */
} pt_pt_cfg_t;

/**
 * @brief This function initializes the pulse trains to a known stopped state
 * and sets the global PT clock scale.
 * @param      ptg Pulse train global registers based on bus
 * @param      sys_cfg System configuration object
 */
void PT_Init(mxc_ptg_regs_t *ptg,const sys_cfg_ptg_t* sys_cfg);

/**
 * @brief      Configures the pulse train in the specified mode.
 * @details    The parameters in the config structure must be set before calling
 *             this function. This function should be used for configuring pulse
 *             train mode only.
 * @note       The pulse train cannot be running when this function is called.
 *
 * @param      pt      Pulse train to operate on.
 * @param      cfg     Pointer to pulse train configuration.
 * @param      sysCfg  Pointer to pulse train system GPIO configuration.
 *
 * @return     #E_NO_ERROR if everything is successful, @ref MXC_Error_Codes
 *             "error" if unsuccessful.
 */
int PT_PTConfig(mxc_pt_regs_t *pt, pt_pt_cfg_t *cfg, const sys_cfg_pt_t *sysCfg);

/**
 * @brief   Configures the pulse train in the square wave mode.
 * @details This function should be used for configuring square wave mode only.
 * @note    The pulse train cannot be running when this function is called
 *
 * @param   pt      pulse train to operate on
 * @param   freq    square wave output frequency in Hz
 * @param   sysCfg  pointer to pulse train system GPIO configuration
 *
 * @returns #E_NO_ERROR if everything is successful, @ref MXC_Error_Codes "error" if unsuccessful.
 */
int PT_SqrWaveConfig(mxc_pt_regs_t *pt, uint32_t freq, const sys_cfg_pt_t *sysCfg);

/**
 * @brief Starts the pulse train specified.
 * @param      pt Pulse train to operate on.
 */
void PT_Start(mxc_pt_regs_t *pt);

/**
 * @brief Start multiple pulse train modules together.
 * @param      ptg Pulse train global registers based on bus
 * @param      pts Set the bits of pulse trains to start Bit0-\>pt0,
 *             Bit1-\>pt1... etc.
 */
void PT_StartMulti(mxc_ptg_regs_t *ptg, uint32_t pts);

/**
 * @brief Stops a pulse train.
 * @param      pt Pulse train to operate on.
 */
void PT_Stop(mxc_pt_regs_t *pt);

/**
 * @brief Stop multiple pulse trains together
 * @param      ptg Pulse train global registers based on bus
 * @param      pts Set the bits of pulse trains to stop Bit0-\>pt0,
 *             Bit1-\>pt1... etc.
 */
void PT_StopMulti(mxc_ptg_regs_t *ptg, uint32_t pts);

/**
 * @brief Determines if the pulse train is running.
 * @param      pt Pulse train to operate on.
 * @return 0       Pulse train is off.
 * @return @>0     Pulse train is on.
 */
uint32_t PT_IsActive(mxc_pt_regs_t *pt);

/**
 * @brief Determines if the pulse trains selected are running
 * @param      ptg Pulse train global registers based on bus
 * @param      pts Set the bits of pulse trains to check Bit0-\>pt0,
 *             Bit1-\>pt1... etc.
 * @return 0            All pulse trains are off.
 * @return @>0          At least one pulse train is on.
 */
uint32_t PT_IsActiveMulti(mxc_ptg_regs_t *ptg, uint32_t pts);

/**
 * @brief   Sets the pattern of the pulse train
 *
 * @param   pt      Pointer to pulse train to operate on
 * @param   pattern Output pattern.
 *
 */
void PT_SetPattern(mxc_pt_regs_t *pt, uint32_t pattern);

/**
 * @brief   Sets the pattern of the pulse train
 *
 * @param   pt  Pointer to pulse train to operate on
 * @return      Pulsetrain rate
 *
 */
float PT_GetBPS(mxc_pt_regs_t *pt);

/**
 * @brief      Enable pulse train interrupt.
 *
 * @param      pt    Pointer to pulse train to operate on.
 */
void PT_EnableINT(mxc_pt_regs_t *pt);

/**
 * @brief Enable interrupts for the pulse trains selected.
 * @param      ptg Pulse train global registers based on bus
 * @param      pts Bit mask of which pulse trains to enable. Set the bit
 *             position of each pulse train to enable it. Bit0-\>pt0,
 *             Bit1-\>pt1... etc, 1 will enable the interrupt, 0 to leave a PT
 *             channel in its current state.
 */
void PT_EnableINTMulti(mxc_ptg_regs_t *ptg, uint32_t pts);

/**
 * @brief      Disable pulse train interrupt.
 *
 * @param      pt    pulse train to operate on.
 */
void PT_DisableINT(mxc_pt_regs_t *pt);

/**
 * @brief Disable interrupts for the pulse trains selected.
 * @param      ptg Pulse train global registers based on bus
 * @param      pts Bit mask of what pulse trains to disable. Set the bit
 *             position of each pulse train to disable it. Bit0-\>pt0,
 *             Bit1-\>pt1... etc, 1 will disable the interrupt, 0 to leave a PT
 *             channel in its current state.
 */
void PT_DisableINTMulti(mxc_ptg_regs_t *ptg, uint32_t pts);

/**
 * @brief Gets the pulse trains' interrupt flags.
 * @param      ptg Pulse train global registers based on bus
 * @return The Pulse Train Interrupt Flags, \c PT_INTFL Register for
 * details.
 */
uint32_t PT_GetFlags(mxc_ptg_regs_t *ptg);

/**
 * @brief Clears the pulse trains' interrupt flag.
 * @param      ptg Pulse train global registers based on bus
 * @param      mask bits to clear, see \c PT_INTFL Register for
 *             details.
 */
void PT_ClearFlags(mxc_ptg_regs_t *ptg, uint32_t mask);

/**
 * @brief      Setup and enables a pulse train to restart after another pulse
 *             train has exited its loop. Each pulse train can have up to two
 *             restart triggers.
 *
 * @param      ptToRestart   pulse train to restart after @c ptStop ends.
 * @param      ptStop        pulse train that stops and triggers @p ptToRestart
 *                           to begin.
 * @param      restartIndex  selects which restart trigger to set (0 or 1).
 */
void PT_SetRestart(mxc_pt_regs_t *ptToRestart, mxc_pt_regs_t *ptStop, uint8_t restartIndex);

/**
 * @brief      Disable the restart for the specified pulse train
 *
 * @param      ptToRestart   pulse train to disable the restart
 * @param      restartIndex  selects which restart trigger to disable (0 or 1)
 */
void PT_RestartDisable(mxc_pt_regs_t *ptToRestart, uint8_t restartIndex);

/**
 * @brief Resynchronize individual pulse trains together. Resync will stop those
 * resync_pts; others will be still running
 * @param      ptg Pulse train global registers based on bus
 * @param      resyncPts pulse train modules that need to be re-synced by bit
 *             number. Bit0-\>pt0, Bit1-\>pt1... etc.
 */
void PT_Resync(mxc_ptg_regs_t *ptg, uint32_t resyncPts);

/**@} end of group pulsetrains*/

#ifdef __cplusplus
}
#endif

#endif /* _PT_H_ */
