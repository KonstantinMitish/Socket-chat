#include <winsock2.h>
#include <windows.h>
#include <WS2tcpip.h>

#include <stdio.h>

#include "main.h"
//#include "xor.h"

#pragma comment(lib, "Ws2_32.lib")

SOCKET ServerSocket;

#define PORT 30239

USER Users[MAX_USERS];

void NetworkInit(void)
{
  WSADATA WsaData;
  struct sockaddr_in Address;
  int res;

   WSAStartup(0x202, &WsaData);

   ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

   Address.sin_addr.s_addr = INADDR_ANY;
   Address.sin_family = AF_INET;
   Address.sin_port = htons(PORT);
  memset(&Address.sin_zero, 0, sizeof(Address.sin_zero));

   res = bind(ServerSocket, (struct sockaddr *)&Address, sizeof(Address));

   res = listen(ServerSocket, 0xF);
}

USER *UserReg( USER User )
{
  int i;

  for (i = 0; i < MAX_USERS; i++)
    if (!Users[i].Exist)
    {
      Users[i] = User;
      return &Users[i];
    }

  return NULL;
}

void SendOne(USER *User, MSGHEAD MsgHead, char *Data)
{
  send(User->Socket, (char *)&MsgHead, sizeof(MSGHEAD), 0);
  send(User->Socket, Data, MsgHead.Length, 0);
}


void SendNext(USER *User, MSGHEAD MsgHead, char *Data)
{
  USER *end = &Users[MAX_USERS];
  do
  {
    User++; 
    if (User == end)
      User = Users;
  } while (User->Exist == 0);

  send(User->Socket, (char *)&MsgHead, sizeof(MSGHEAD), 0);
  send(User->Socket, Data, MsgHead.Length, 0);
}

void SendAll( USER *User, MSGHEAD MsgHead, char *Data )
{
  int i;

  for (i = 0; i < MAX_USERS; i++)
    if (Users[i].Exist)
      SendOne(&Users[i], MsgHead, Data);

  //free(NewData);
}

void ExecuteMessage( USER *User, MSGHEAD *MsgHead, char *Data )
{
  if (MsgHead->DataType == CLOSE_CLIENT)
    User->Exist = 0;
}

int HandleMessage( USER *User )
{
  MSGHEAD MsgHead;
  char Data[1024];
  int res;

  if (!User->Exist)
    return 0;

  res = recv(User->Socket, (char *)&MsgHead, sizeof(MSGHEAD), 0);


  if (res == -1 || MsgHead.DataType == CLOSE_CLIENT)
  {
    User->Exist = 0;
    return 1;
  }

  recv(User->Socket, Data, MsgHead.Length, 0);

  //DecryptRN(Data, MsgHead.Length - 31, Data);
  switch (MsgHead.DataType & 0xFF000000)
  {
    case EXECUTE:
      ExecuteMessage(User, &MsgHead, Data);
      break;
    case SEND_TO_ALL:
      SendAll(User, MsgHead, Data);
      break;
    case SEND_TO_NEXT:
      SendNext(User, MsgHead, Data);
      break;
  }

  return 1;
}

DWORD WINAPI UserHandle( void *Param )
{
  ACCEPTDATA param = *(ACCEPTDATA *)Param;
  free(Param);
  USER User;
  USER *UserRegistered;

  User.Address = param.Address;
  User.Socket = param.Socket;
  User.Exist = 1;

  UserRegistered = UserReg(User);

  while (HandleMessage(UserRegistered))
    ;

  return 0;
}


void AddUser(void)
{
  struct sockaddr_in ClientAddr;
  int AddrLen = sizeof(ClientAddr);
  ACCEPTDATA *Param = malloc(sizeof(ACCEPTDATA));

  Param->Socket = accept(ServerSocket, (struct sockaddr *)&Param->Address, &AddrLen);

  CreateThread(NULL, 1000, UserHandle, Param, 0, NULL);
}

void main(void)
{
  NetworkInit();

  while (1)
    AddUser();
}