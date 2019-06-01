#include <windows.h>
#include <stdio.h>
#include <string.h>

#include "NETWORK.H"
#include "MAIN.H"
#include "XOR.H"
#include "COMMAND.H"

CMDTYPE GetCmdType( CHAR *Cmd )
{
  if (strncmp(Cmd, "connect", 7) == 0)
    return CMDTYPE_CONNECT;
  if (strncmp(Cmd, "disconnect", 10) == 0)
    return CMDTYPE_DISCONNECT;
  if (strncmp(Cmd, "download", 8) == 0)
    return CMDTYPE_DOWNLOAD;
  if (strncmp(Cmd, "upload", 6) == 0)
    return CMDTYPE_UPLOAD;
  if (strncmp(Cmd, "reboot", 6) == 0)
    return CMDTYPE_REBOOT;
  if (strncmp(Cmd, "cmd", 3) == 0)
    return CMDTYPE_CALLCMD;
  if (strncmp(Cmd, "dh", 2) == 0)
    return CMDTYPE_INIT_DH;
  if (strncmp(Cmd, "name", 4) == 0)
    return CMDTYPE_CHANGE_NAME;
  if (strncmp(Cmd, "debug", 5) == 0)
    return CMDTYPE_SET_DEBUG;
  if (strncmp(Cmd, "clear", 5) == 0)
    return CMDTYPE_CLEAR;
  if (strncmp(Cmd, "rand", 4) == 0)
    return CMDTYPE_RAND;
  return CMDTYPE_UNKNOWN;
}


size_t RunCmd( CMD *Cmd )
{
  MESSAGE Msg = { 0 };
  CHAR Buf[256];
  size_t len = sizeof(Buf);
  CHAR Message[256];

  switch (Cmd->Type)
  {
  case CMDTYPE_CONNECT:
    return NetworkInit(Cmd->Args[0]);
  case CMDTYPE_DISCONNECT:
    if (NetworkInited)
      NetworkDisconnect();
    else
      Print(0, "You are not connected");
    return 1;
  case CMDTYPE_DOWNLOAD:
    Print(0, "Download");
    return 1;
  case CMDTYPE_CALLCMD:
    sprintf(Message, "%s %s", Cmd->Args[0], Cmd->Args[1]);
    Msg.Head.Length = strlen(Message);
    EncryptRN(Message, Msg.Head.Length, Buf, &len);
    Msg.Head.Length += 31;
    Msg.Head.DataType = DATATYPE_CALL_CMD;
    Msg.Data = Buf;
    SendMessage(&Msg);
    return 1;
  case CMDTYPE_UPLOAD:
    Print(0, "Upload");
    return 1;
  case CMDTYPE_INIT_DH:
    Msg.Head.DataType = DATATYPE_DH_INIT;
    Msg.Head.Length = 0;
    SendMessage(&Msg);
    return 1;
  case CMDTYPE_CHANGE_NAME:
    memcpy_s(Name, sizeof(Name), Cmd->Args[0], sizeof(Cmd->Args[0]));
    Print(0, "You are now %s", Name);
    return 1;
  case CMDTYPE_SET_DEBUG:
    DebugLevel = strtoul(Cmd->Args[0], NULL, 10);
    Print(0, "Debug level set to %i", DebugLevel);
    return 1;
  case CMDTYPE_RAND:
    Print(0, "%i", rand());
    return 1;
  case CMDTYPE_REBOOT:
    return system("shutdown -r");
  case CMDTYPE_CLEAR:
    Clear();
    Print(2, "Log cleared");
    return 1;
  }
  return 0;
}

size_t ParseCmd( CHAR *String, CMD *Cmd )
{
  size_t i = 0, j = 0, k = 0;
  static CHAR Buf[256];
  CMDTYPE Type;
  BOOL Flag = FALSE;

  if (String[i++] != '/')
    return 0;

  while (String[i] != ' ' && String[i] != 0)
    Buf[j++] = String[i++];
  Type = GetCmdType(Buf);
  if (Type == CMDTYPE_UNKNOWN)
    return 0;
  Cmd->Type = Type;

  i++;
  j = 0;
  while (String[i] != 0)
  {
    if (String[i] == ' ' && !Flag)
    {
      Cmd->Args[k][j] = 0;
      k++;
      j = 0;
      i++;
      continue;
    }
    if (String[i] == '\"')
    {
      Flag = !Flag;
      i++;
      continue;
    }
    Cmd->Args[k][j++] = String[i++];
  }
  Cmd->Args[k][j] = 0;
  return 1;
}