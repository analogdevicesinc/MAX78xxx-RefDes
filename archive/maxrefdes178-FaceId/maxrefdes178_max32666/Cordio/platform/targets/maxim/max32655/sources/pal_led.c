/*************************************************************************************************/
/*!
 * \file
 *
 * \brief      LED driver implementation.
 *
 * Copyright (c) 2019-2020 Packetcraft, Inc.  All rights reserved.
 * Packetcraft, Inc. confidential and proprietary.
 *
 * IMPORTANT.  Your use of this file is governed by a Software License Agreement
 * ("Agreement") that must be accepted in order to download or otherwise receive a
 * copy of this file.  You may not use or copy this file for any purpose other than
 * as described in the Agreement.  If you do not agree to all of the terms of the
 * Agreement do not use this file and delete all copies in your possession or control;
 * if you do not have a copy of the Agreement, you must contact Packetcraft, Inc. prior
 * to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/

#include "pal_led.h"
#include "led.h"
#include "board.h"

/**************************************************************************************************
  Functions: Initialization
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Initialize LEDs.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalLedInit(void)
{
  LED_Init();
}

/**************************************************************************************************
  Functions: Status and Control
**************************************************************************************************/

/*************************************************************************************************/
/*!
 *  \brief      Set LED on.
 *
 *  \param      ledId           LED ID.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalLedOn(uint8_t ledId)
{
  switch (ledId) {
    case PAL_LED_ID_CPU_ACTIVE:
      LED_On(1);    /* D2: green */
      break;
    case PAL_LED_ID_ERROR:
      LED_On(0);    /* D1: red */
      break;
    default:
      break;
  }
}

/*************************************************************************************************/
/*!
 *  \brief      Set LED off.
 *
 *  \param      ledId           LED ID.
 *
 *  \return     None.
 */
/*************************************************************************************************/
void PalLedOff(uint8_t ledId)
{
  switch (ledId) {
    case PAL_LED_ID_CPU_ACTIVE:
      LED_Off(1);   /* D2: green */
      break;
    case PAL_LED_ID_ERROR:
      LED_Off(0);   /* D1: red */
      break;
    default:
      break;
  }
}
