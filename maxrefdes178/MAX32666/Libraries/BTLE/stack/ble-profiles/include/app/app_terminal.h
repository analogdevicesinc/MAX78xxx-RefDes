/*************************************************************************************************/
/*!
 *  \file
 *
 *  \brief      App Terminal handler.
 *
 *  Copyright (c) 2015-2018 Arm Ltd. All Rights Reserved.
 *  ARM Ltd. confidential and proprietary.
 *
 *  IMPORTANT.  Your use of this file is governed by a Software License Agreement
 *  ("Agreement") that must be accepted in order to download or otherwise receive a
 *  copy of this file.  You may not use or copy this file for any purpose other than
 *  as described in the Agreement.  If you do not agree to all of the terms of the
 *  Agreement do not use this file and delete all copies in your possession or control;
 *  if you do not have a copy of the Agreement, you must contact ARM Ltd. prior
 *  to any use, copying or further distribution of this software.
 */
/*************************************************************************************************/

#ifndef APP_TERMINAL_H
#define APP_TERMINAL_H

/*! \addtogroup APP_FRAMEWORK_UI_API
 *  \{ */

/**************************************************************************************************
  Function Prototypes
**************************************************************************************************/

/** \name APP Terminal Functions
 * Open a terminal interface to the application.
 */
/**@{*/

/*************************************************************************************************/
/*!
 *  \brief  Initialize terminal.
 *
 *  \return None.
 */
/*************************************************************************************************/
void AppTerminalInit(void);

/**@}*/

/*! \} */    /*! APP_FRAMEWORK_UI_API */

#endif /* APP_TERMINAL_H */
