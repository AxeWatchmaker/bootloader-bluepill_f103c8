#ifndef BOOTLOADER_H
#define BOOTLOADER_H

#include "main.h"

void Bootloader_Run(void);
bool Bootloader_CheckButton(void);
bool Bootloader_ReceiveFirmware(void);
void Bootloader_JumpToApp(void);

#define SYNC_BYTE 0xAA
#define CMD_UPDATE 0x01
#define CMD_RUN_APP 0x02

#endif