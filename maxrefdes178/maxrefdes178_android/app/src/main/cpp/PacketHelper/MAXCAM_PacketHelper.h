/******************************************************************************
* Copyright (C) 2020 Maxim Integrated Products, Inc., All rights Reserved.
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

#ifndef MAXCAM_PACKET_HELPER_H_
#define MAXCAM_PACKET_HELPER_H_

#ifdef __cplusplus
extern "C" {
#endif
//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
/*
#include "MAXCAM_Definitions.h"
#include "MAXCAM_Structure.h"
#include "MAXCAM_Packet.h"
 */
#include "maxrefdes178_definitions.h"
#include "maxrefdes178_ble_packet.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------
typedef void (*SendNotificationFPtr)(const uint16_t dataLen, const uint8_t *data);
typedef void (*PayloadReceivedFPtr)(const uint16_t dataLen, const uint8_t *data);
typedef uint16_t (*GetCurrentMtuFPtr)(void);

//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Function declarations
//-----------------------------------------------------------------------------
/*
void PacketHelper_SendFile(const char *fileName, const char *fileContent, const uint32_t fileLength);
void PacketHelper_Receive(const uint16_t packetLen, const uint8_t *packet);
void PacketHelper_RegisterSendNotification(SendNotificationFPtr fptr);
void PacketHelper_RegisterPayloadReceived(PayloadReceivedFPtr fptr);
void PacketHelper_RegisterGetCurrentMtu(GetCurrentMtuFPtr fptr);
void PacketHelper_RequestGetDir(void);
void PacketHelper_RequestGetFile(const char *fileName);
void PacketHelper_ResetPacketNumber();
void PacketHelper_EnterDEMO(void);
void PacketHelper_CaptureImage(const uint32_t fileLength);
void PacketHelper_SendImage(const char *fileName, const char *fileContent, const uint32_t fileLength);
void PacketHelper_LoadImage(const char *fileName);
*/
//MAXREFDES178_COMMON
/*
void PacketHelper_RequestVersion(void);
 */

#ifdef __cplusplus
}
#endif

#endif /* MAXCAM_PACKET_HELPER_H_ */
