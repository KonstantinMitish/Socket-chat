#pragma warning(disable: 4267)

#define MAX_NAME 40

typedef struct tagACCEPTDATA
{
  SOCKET Socket;
  struct sockaddr_in Address;
} ACCEPTDATA;

typedef struct tagUSER
{
  size_t Exist;
  SOCKET Socket;
  struct sockaddr_in Address;
} USER;


typedef enum
{
  EXECUTE      = 0x01000000,
  SEND_TO_ALL  = 0x02000000,
  SEND_TO_NEXT = 0x04000000,

  CLOSE_CLIENT = 0x01000003,
  CLOSE_SERVER = 0x01000004,
} DATATYPE;

#pragma pack(push, 1)

typedef struct tagMSGHEAD
{
  size_t Length;
  DATATYPE DataType;
} MSGHEAD;

#pragma pack(pop)

#define MAX_USERS 30