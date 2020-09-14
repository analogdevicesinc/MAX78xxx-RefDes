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

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <stdio.h>
#include <string.h>
#include "MAXCAM_Definitions.h"
#include "MAXCAM_Structure.h"
#include "MAXCAM_Packet.h"
#include "MAXCAM_PacketHelper.h"


//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#define S_MODULE_NAME       "PacketHelper"


//-----------------------------------------------------------------------------
// Typedefs
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
static uint8_t mySequenceNumber = 1;
static uint8_t peerSequenceNumber = 1;
static SendNotificationFPtr SendNotification;
static GetCurrentMtuFPtr GetCurrentMtu;
static PayloadReceivedFPtr PayloadReceived;


//-----------------------------------------------------------------------------
// Local function declarations
//-----------------------------------------------------------------------------
//static void PacketHelper_RequestRetransmission(const uint8_t requestedSequenceNumber);


//-----------------------------------------------------------------------------
// Function definitions
//-----------------------------------------------------------------------------
void PacketHelper_RegisterSendNotification(SendNotificationFPtr fptr)
{
    if(fptr != NULL) {
        SendNotification = fptr;
    }
}

void PacketHelper_RegisterPayloadReceived(PayloadReceivedFPtr fptr)
{
    if(fptr != NULL) {
        PayloadReceived = fptr;
    }
}

void PacketHelper_RegisterGetCurrentMtu(GetCurrentMtuFPtr fptr)
{
    if(fptr != NULL) {
        GetCurrentMtu = fptr;
    }
}

void PacketHelper_SendFile(const char *fileName, const char *fileContent, const uint32_t fileLength)
{
    uint8_t packet[MAXCAM_BLE_MAX_PACKET_SIZE] = {0};
    uint8_t payload[MAXCAM_BLE_MAX_PACKET_SIZE - sizeof(tsCommandPackage)] = {0};
    uint16_t packetSize = 0;
    uint16_t payloadSize = 0;
    uint16_t chunkSize = 0;
    uint32_t fileSize = 0;
    const uint8_t *filePtr = NULL;

    // Get file size and chunk size
    chunkSize = GetCurrentMtu() - sizeof(tsDataPackage);
    fileSize = fileLength;
    filePtr = fileContent;

    // Send LFO request command
    payloadSize = Packet_FormLFOArgs(MAXCAM_LFO_BLE, MAXCAM_LFO_SDCARD,
                                     fileSize, strlen((const char *) fileName) + 1, fileName,
                                     payload);
    packetSize = Packet_FormCommandPacket(MAXCAM_ANDROID, MAXCAM_BLE,
                                          MAXCAM_ME14, MAXCAM_SDCARD, mySequenceNumber++, 0,
                                          eLFOFunctionHandler, payloadSize, payload, packet);

    if (SendNotification) {
        SendNotification(packetSize, packet);
    }

    // Send file data chunk by chunk
    while (fileSize > chunkSize) {
        packetSize = Packet_FormDataPacket(MAXCAM_ANDROID, MAXCAM_BLE,
                                           MAXCAM_ME14, MAXCAM_SDCARD, mySequenceNumber++,
                                           chunkSize,
                                           filePtr, packet);

        if (SendNotification) {
            SendNotification(packetSize, packet);
        }

        filePtr += chunkSize;
        fileSize -= chunkSize;
    }
    if (fileSize) {
        packetSize = Packet_FormDataPacket(MAXCAM_ANDROID, MAXCAM_BLE,
                                           MAXCAM_ME14, MAXCAM_SDCARD, mySequenceNumber++, fileSize,
                                           filePtr, packet);

        if (SendNotification) {
            SendNotification(packetSize, packet);
        }
    }
}

void PacketHelper_Receive(const uint16_t packetLen, const uint8_t *packet)
{
    uint8_t *payload = NULL;
    uint16_t payloadLen = 0;

//    printf("Receive len: %d, data:\n", packetLen);
//    for (uint16_t i = 0; i < packetLen; i++) {
//      printf("0x%02hhX ", packet[i]);
//    }
//    printf("\n");

    if (packetLen < sizeof(tsPacketHeader)) {
        // Error illegal packet size
        return;
    }

    if (packetLen != Packet_ParsePacketSize(packet)) {
        // Error packet size and packet header size are different
        return;
    }

    if (peerSequenceNumber != Packet_ParsePacketSequenceNumber(packet)) {
        // Error incorrect sequence number

        // Request retransmission
//        PacketHelper_RequestRetransmission(peerSequenceNumber);
//        return;
    }
    peerSequenceNumber++;

    if (Packet_ParsePacketType(packet) == MAXCAM_PACKET_TYPE_REQUEST) {
        if (packetLen < sizeof(tsCommandPackage)) {
            // Error illegal packet size
            return;
        }

        payloadLen = packetLen - sizeof(tsCommandPackage);
        payload = Packet_ParseCommandPacketPayload(packet);

        // Handle commands
        if (Packet_ParseCommandPacketCommand(packet) == MAXCAM_BLE_RETRANSMISSION) {
            // TODO handle retransmission
        } else if (Packet_ParseCommandPacketCommand(packet) == MAXCAM_SD_LS_RESPONSE) {
            uint32_t lsFileSize = *((uint32_t*)(payload));
            // TODO handle lsFileSize
        } else {
            // Error unknown command
            return;
        }

    } else if (Packet_ParsePacketType(packet) == MAXCAM_PACKET_TYPE_DATA) {
        if (packetLen < sizeof(tsDataPackage)) {
            // Error illegal packet size
            return;
        }

        payloadLen = packetLen - sizeof(tsDataPackage);
        payload = Packet_ParseDataPacketData(packet);

        // use data
        if(PayloadReceived) {
            PayloadReceived(payloadLen, payload);
        }
    }
}

//static void PacketHelper_RequestRetransmission(const uint8_t requestedSequenceNumber)
//{
//    uint8_t packet[MAXCAM_BLE_MAX_PACKET_SIZE] = {0};
//    uint8_t payload[1] = {requestedSequenceNumber};
//    uint16_t packetSize = 0;
//
//    packetSize = Packet_FormCommandPacket(MAXCAM_ANDROID, MAXCAM_BLE,
//                                          MAXCAM_ME14, MAXCAM_BLE, mySequenceNumber++,
//                                          MAXCAM_BLE_RETRANSMISSION, 0, sizeof(payload), payload,
//                                          packet);
//
//    if (SendNotification) {
//        SendNotification(packetSize, packet);
//    }
//}

void PacketHelper_RequestGetDir(void)
{
    uint8_t packet[MAXCAM_BLE_MAX_PACKET_SIZE] = {0};
    uint16_t packetSize = 0;

    packetSize = Packet_FormCommandPacket(MAXCAM_ANDROID, MAXCAM_BLE,
                                          MAXCAM_ME14, MAXCAM_SDCARD, mySequenceNumber++,
                                          MAXCAM_SD_LS, eSDCardFunctionHandler, 0, NULL,
                                          packet);

    if (SendNotification) {
        SendNotification(packetSize, packet);
    }
}

void PacketHelper_RequestGetFile(const char *fileName)
{
    uint8_t packet[MAXCAM_BLE_MAX_PACKET_SIZE] = {0};
    uint8_t payload[MAXCAM_BLE_MAX_PACKET_SIZE - sizeof(tsCommandPackage)] = {0};
    uint16_t packetSize = 0;
    uint16_t payloadSize = 0;

    // Send LFO request command
    payloadSize = Packet_FormLFOArgs(MAXCAM_LFO_SDCARD, MAXCAM_LFO_BLE,
                                     0, strlen((const char *) fileName) + 1, fileName,
                                     payload);
    packetSize = Packet_FormCommandPacket(MAXCAM_ANDROID, MAXCAM_BLE,
                                          MAXCAM_ME14, MAXCAM_SDCARD, mySequenceNumber++, MAXCAM_SD_READ,
                                          eLFOFunctionHandler, payloadSize, payload, packet);

    if (SendNotification) {
        SendNotification(packetSize, packet);
    }
}

void PacketHelper_ResetPacketNumber()
{
    mySequenceNumber = 1;
    peerSequenceNumber = 1;
}

void PacketHelper_EnterDEMO(void)
{

    uint8_t packet[MAXCAM_BLE_MAX_PACKET_SIZE] = {0};
    uint8_t payload[MAXCAM_BLE_MAX_PACKET_SIZE - sizeof(tsCommandPackage)] = {0};
    uint16_t packetSize = 0;
    uint16_t payloadSize = 2;

    *((uint8_t *)payload) = MAXCAM_AI85_MODE_DEMO;
    *((uint8_t *)(payload+1)) = MAXCAM_AI85_DEMO_FACEID_v01;

    packetSize = Packet_FormCommandPacket(MAXCAM_ANDROID, MAXCAM_BLE, MAXCAM_AI85_IMAGE, MAXCAM_AI85_IMAGE,
                                          mySequenceNumber++, MAXCAM_AI85_SET_MODE,
                                          eAI85FunctionHandler, payloadSize, payload, packet);

    if (SendNotification) {
        SendNotification(packetSize, packet);
    }
}

void PacketHelper_CaptureImage(const uint32_t fileLength)
{
    uint8_t packet[MAXCAM_BLE_MAX_PACKET_SIZE] = {0};
    uint8_t payload[MAXCAM_BLE_MAX_PACKET_SIZE - sizeof(tsCommandPackage)] = {0};
    uint16_t packetSize = 0;
    uint16_t payloadSize = 0;

    payloadSize = sizeof(uint32_t) + sizeof(uint16_t);

    *((uint32_t *)payload) = fileLength;
    /* data packet size for qspi communication */
    *((uint16_t *)(payload+sizeof(uint32_t))) = GetCurrentMtu() - sizeof(tsDataPackage);

    packetSize = Packet_FormCommandPacket(MAXCAM_ANDROID, MAXCAM_BLE,
                                          MAXCAM_ME14, MAXCAM_SDCARD, mySequenceNumber++, MAXCAM_AI85_CAPTURE_IMAGE,
                                          eAI85FunctionHandler, payloadSize, payload, packet);

    if (SendNotification) {
        SendNotification(packetSize, packet);
    }

}

void PacketHelper_SendImage(const char *fileName, const char *fileContent, const uint32_t fileLength)
{
    uint8_t packet[MAXCAM_BLE_MAX_PACKET_SIZE] = {0};
    uint8_t payload[MAXCAM_BLE_MAX_PACKET_SIZE - sizeof(tsCommandPackage)] = {0};
    uint16_t packetSize = 0;
    uint16_t payloadSize = 0;
    uint16_t chunkSize = 0;
    uint32_t fileSize = 0;
    const uint8_t *filePtr = NULL;

    // Get file size and chunk size
    chunkSize = GetCurrentMtu() - sizeof(tsDataPackage);
    fileSize = fileLength;
    filePtr = fileContent;

    // Send LFO request command
    payloadSize = Packet_FormLFOArgs(MAXCAM_LFO_BLE, MAXCAM_LFO_AI85_IMAGE,
                                     fileSize, strlen((const char *) fileName) + 1, fileName,
                                     payload);
    packetSize = Packet_FormCommandPacket(MAXCAM_ANDROID, MAXCAM_BLE,
                                          MAXCAM_ME14, MAXCAM_SDCARD, mySequenceNumber++, 0,
                                          eLFOFunctionHandler, payloadSize, payload, packet);

    if (SendNotification) {
        SendNotification(packetSize, packet);
    }

    // Send file data chunk by chunk
    while (fileSize > chunkSize) {
        packetSize = Packet_FormDataPacket(MAXCAM_ANDROID, MAXCAM_BLE,
                                           MAXCAM_ME14, MAXCAM_SDCARD, mySequenceNumber++,
                                           chunkSize,
                                           filePtr, packet);

        if (SendNotification) {
            SendNotification(packetSize, packet);
        }

        filePtr += chunkSize;
        fileSize -= chunkSize;
    }
    if (fileSize) {
        packetSize = Packet_FormDataPacket(MAXCAM_ANDROID, MAXCAM_BLE,
                                           MAXCAM_ME14, MAXCAM_SDCARD, mySequenceNumber++, fileSize,
                                           filePtr, packet);

        if (SendNotification) {
            SendNotification(packetSize, packet);
        }
    }

}

void PacketHelper_LoadImage(const char *fileName)
{
    uint8_t packet[MAXCAM_BLE_MAX_PACKET_SIZE] = {0};
    uint8_t payload[MAXCAM_BLE_MAX_PACKET_SIZE - sizeof(tsCommandPackage)] = {0};
    uint16_t packetSize = 0;
    uint16_t payloadSize = 0;


    // Send LFO request command
    payloadSize = Packet_FormLFOArgs(MAXCAM_LFO_SDCARD, MAXCAM_LFO_AI85_IMAGE,
                                     0, strlen((const char *) fileName) + 1, fileName,
                                     payload);
    packetSize = Packet_FormCommandPacket(MAXCAM_ANDROID, MAXCAM_BLE,
                                          MAXCAM_ME14, MAXCAM_SDCARD, mySequenceNumber++, 0,
                                          eLFOFunctionHandler, payloadSize, payload, packet);

    if (SendNotification) {
        SendNotification(packetSize, packet);
    }

}


