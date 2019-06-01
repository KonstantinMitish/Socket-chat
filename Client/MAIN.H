#ifndef __MAIN_H_
#define __MAIN_H_

#pragma warning(disable: 4267)

#include <windows.h>

#pragma pack(push, 1)

extern HWND hWnd_main;

extern INT DebugLevel;

extern CHAR Name[256];

VOID Clear();
CHAR *ByteToHexStr(BYTE *Buffer, SIZE_T Size);
VOID Print(INT DebugLevelMin, const CHAR *Format, ...);
VOID MessageIn(CHAR *Message);
VOID MessageOut(CHAR *Message);

#endif /* __MAIN_H_ */