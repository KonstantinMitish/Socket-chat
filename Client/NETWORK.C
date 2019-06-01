#include <winsock2.h> 
#include <time.h>
#include <Windows.h>
#include <WS2tcpip.h>

#include "NETWORK.H"
#include "DIFFIE_HELLMAN.H"
#include "MAIN.H"
#include "XOR.H"

#pragma comment(lib, "Ws2_32.lib")

SOCKET Socket;
size_t NetworkInited = 0;

CHAR Key[KEY_LEN] = STATIC_KEY;


INT ProcessMessage( MESSAGE *Message )
{
  BYTE Buf[1024];
  switch (Message->Head.DataType)
  {
  case DATATYPE_CLOSE_CLIENT:
  case DATATYPE_CLOSE_SERVER:
    NetworkInited = 0;
    Print(2, "Disconnect message");
    Print(0, "Disconnected");
    return 1;
  case DATATYPE_PLAIN_TEXT:
    Print(2, "Text message");
    MessageIn(Message->Data);
    break;
  case DATATYPE_DH_INIT:
    Print(2, "Diffie-Hellman initialisation request");
    Message->Head.DataType = DATATYPE_DH_NEXT;
    Message->Head.Length = sizeof(Buf);
    Message->Data = Buf;
    DiffieHellmanInit(DIFFIE_HELLMAN_KEY_LEN, Buf, &Message->Head.Length);
    Print(2, "Generated Diffie-Hellman packet");
    Print(3, "Id (%i bytes): %s", DIFFIE_HELLMAN_ID_LEN, ByteToHexStr(DiffieHellmanId, DIFFIE_HELLMAN_ID_LEN));
    Print(3, "Secret (%i bytes): %s", DiffieHellmanSecretLen, ByteToHexStr(DiffieHellmanSecret, DiffieHellmanSecretLen));
    Print(3, "Key (%i bytes): %s", DiffieHellmanKeyLen, ByteToHexStr(DiffieHellmanKey, DiffieHellmanKeyLen));
    Print(3, "Message (%i bytes): %s", Message->Head.Length, ByteToHexStr(Buf, Message->Head.Length));
    SendMessage(Message);
    break;
  case DATATYPE_DH_NEXT:
    Print(2, "Diffie-Hellman iteration request");
    switch (DiffieHellmanStep(Message->Data, Message->Head.Length))
    {
    case 1:
      Print(2, "Iteration done");
      Print(3, "Key (%i bytes): %s", DiffieHellmanKeyLen, ByteToHexStr(DiffieHellmanKey, DiffieHellmanKeyLen));
      Print(3, "Message (%i bytes): %s", Message->Head.Length, ByteToHexStr(Buf, Message->Head.Length));
      SendMessage(Message);
      break;
    case 2:
      Print(2, "Diffie-Hellman protocol realized");
      Print(3, "Key (%i bytes): %s", DiffieHellmanKeyLen, ByteToHexStr(DiffieHellmanKey, DiffieHellmanKeyLen));
      memcpy_s(Key, KEY_LEN, DiffieHellmanKey, DiffieHellmanKeyLen);
      break;
    }
    break;
  }
  return 0;
}

DWORD WINAPI GetMessages( VOID *Param )
{
  srand(_time32(NULL));
  while (1)
  {
    MESSAGE Msg;
    CHAR Buf[1024];
    CHAR Buf2[1024];
    size_t i;

    Msg.Data = Buf;
    i = recv(Socket, (CHAR *)&(Msg.Head), sizeof(MSGHEAD), 0);
    if (i == -1)
    {
      Print(1, "Socket failture");
      NetworkInited = 0;
      Print(0, "Disconnected");
      return 0;
    }
    i = recv(Socket, Msg.Data, Msg.Head.Length, 0);
    if (i == -1)
    {
      Print(1, "Socket failture");
      NetworkInited = 0;
      Print(0, "Disconnected");
      return 0;
    }
    i = sizeof(Buf2);
    DecryptRNX(Msg.Data, Msg.Head.Length, Key, KEY_LEN, Buf2, &i);
    Print(2, "Recieved message. Length: %i. Decrypted: %i", Msg.Head.Length, i);
    Print(3, "Original message: %s", ByteToHexStr(Buf, Msg.Head.Length));
    Print(3, "Decrypted message: %s", ByteToHexStr(Buf2, i));
    Print(2, "Recieved %i bytes", sizeof(MSGHEAD) + Msg.Head.Length);
    Msg.Data = Buf2;
    if (ProcessMessage(&Msg))
      return 0;
  }
}

size_t NetworkInit( CHAR *Command )
{
  WSADATA WsaData;
  struct sockaddr_in Addr;
  size_t res;

  Print(1, "Connecting to %s:%i", Command, PORT);
  WSAStartup(0x202, &WsaData);

  Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  inet_pton(AF_INET, Command, &Addr.sin_addr);
  Addr.sin_family = AF_INET;
  Addr.sin_port = htons(PORT);
  memset(Addr.sin_zero, 0, sizeof(Addr.sin_zero));

  res = connect(Socket, (struct sockaddr *)&Addr, sizeof(Addr));

  if (res == -1)
  {
    Print(0, "Connection failed");
    closesocket(Socket);
    WSACleanup();
    return 0;
  }
  Print(0, "Successful connection");
  CreateThread(NULL, 1000, GetMessages, NULL, 0, NULL);
  NetworkInited = 1;
  return 1;
}

VOID NetworkDisconnect( VOID )
{
  MSGHEAD Head;

  Head.DataType = DATATYPE_CLOSE_CLIENT;
  send(Socket, (CHAR *)&Head, sizeof(MSGHEAD), 0);
  Print(0, "Disconnected");
}

VOID NetworkClose( VOID )
{
  NetworkDisconnect();

  closesocket(Socket);
  WSACleanup();
}

VOID SendMessage( MESSAGE *Message )
{
  MSGHEAD MsgHead;
  CHAR Buf[1024];
  size_t len = sizeof(Buf);

  if(NetworkInited)
  {
    int i;
    MsgHead.Length = Message->Head.Length;
    EncryptRNX(Message->Data, MsgHead.Length, Key, KEY_LEN, Buf, &len);
    MsgHead.DataType = Message->Head.DataType;
    Print(2, "Sending message. Length: %i. Encrypted: %i", MsgHead.Length, len);
    Print(3, "Original message: %s", ByteToHexStr(Message->Data, MsgHead.Length));
    Print(3, "Encrypted message: %s", ByteToHexStr(Buf, len));
    MsgHead.Length = len;
    i = send(Socket, (CHAR *)&MsgHead, sizeof(MsgHead), 0);
    i += send(Socket, Buf, MsgHead.Length, 0);
    Print(2, "Send %i bytes", i);
  }
  else
  {
    Print(0, "Error. You are not connected.\n");
  }
}
