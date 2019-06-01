#include <windows.h>
#include <stdio.h> 
#include <time.h>

#include "NETWORK.H"
#include "COMMAND.H"
#include "NOTIFICATIONS.H"

CHAR Name[256] = "faggot";

LRESULT CALLBACK WinProcMain(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProcTop(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK WinProcBottom(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

HWND hWnd_main;

HINSTANCE HInstance;

INT DebugLevel = 0;

#define MAX_MESSAGES 45

CHAR Messages[MAX_MESSAGES][1024];
size_t MessageNum;
size_t top_repaint_needed = 1;

VOID Clear()
{
  memset(Messages, 0, sizeof(Messages));
  MessageNum = 0;
  top_repaint_needed = 1;
}

CHAR *ByteToHexStr(BYTE *Buffer, SIZE_T Size)
{
  static CHAR buf[1024];
  int i, j;
  for (i = 0, j = 0; i < Size && j < sizeof(buf) - 2; i++, j += 2)
    sprintf(&buf[j], "%X%X", (Buffer[i] >> 4) & 0xF, Buffer[i] & 0xF);
  buf[j] = 0;
  return buf;
}

VOID Print(INT DebugLevelMin, const CHAR *Format, ...)
{
  if (DebugLevelMin > DebugLevel)
    return;
  if (MessageNum == MAX_MESSAGES - 1)
  {
    size_t i;

    MessageNum--;

    for (i = 0; i < MAX_MESSAGES - 1; i++)
      strcpy(Messages[i], Messages[i + 1]);
  }
  va_list argptr;
  va_start(argptr, Format);
  vsprintf_s(Messages[MessageNum++], sizeof(Messages[0]), Format, argptr);
  va_end(argptr);
  top_repaint_needed = 1;
}

VOID MessageIn( CHAR *Message )
{ 
  Notify("sokEt chat", Message);
  Print(-1, "%s", Message);
}

VOID MessageOut(CHAR *Message)
{
  CMD Cmd;
  CHAR Buf[256];

  if (ParseCmd(Message, &Cmd) != 0)
  {
    if (RunCmd(&Cmd) == 0)
      MessageIn("Failed\n");
  }
  else
  {
    MESSAGE Msg;
    sprintf_s(Buf, sizeof(Buf), "%s: %s", Name, Message);

    Msg.Head.Length = strlen(Buf) + 1;
    Msg.Head.DataType = DATATYPE_PLAIN_TEXT;
     

    Msg.Data = Buf;
    SendMessage(&Msg);
  }
}

VOID CreateStyles( HINSTANCE hInstance)
{
  WNDCLASS main, top, bottom;
  
  main.cbClsExtra = 0;
  main.cbWndExtra = 0;
  main.hbrBackground = GetStockObject(GRAY_BRUSH);
  main.hCursor = LoadCursor(hInstance, IDC_ARROW);
  main.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
  main.hInstance = hInstance;
  main.lpfnWndProc = WinProcMain;
  main.lpszClassName = "Chat main";
  main.lpszMenuName = NULL;
  main.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

  RegisterClass(&main);

  top.cbClsExtra = 0;
  top.cbWndExtra = 0;
  top.hbrBackground = GetStockObject(GRAY_BRUSH);
  top.hCursor = LoadCursor(hInstance, IDC_ARROW);
  top.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
  top.hInstance = hInstance;
  top.lpfnWndProc = WinProcTop;
  top.lpszClassName = "Chat top";
  top.lpszMenuName = NULL;
  top.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

  RegisterClass(&top);

  bottom.cbClsExtra = 0;
  bottom.cbWndExtra = 0;
  bottom.hbrBackground = GetStockObject(GRAY_BRUSH);
  bottom.hCursor = LoadCursor(hInstance, IDC_ARROW);
  bottom.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
  bottom.hInstance = hInstance;
  bottom.lpfnWndProc = WinProcBottom;
  bottom.lpszClassName = "Chat bottom";
  bottom.lpszMenuName = NULL;
  bottom.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;

  RegisterClass(&bottom);
}

INT WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, BOOL nCmdShow )
{
  srand(_time32(NULL));
  //rand();
  CreateStyles(hInstance);
  HInstance = hInstance;

  hWnd_main = CreateWindow("Chat main", "sockEt chat", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 640, MAX_MESSAGES * 20 + 100, NULL, NULL, hInstance, NULL);
  ShowWindow(hWnd_main, SW_SHOWNORMAL);

  MSG msg;

  while (GetMessage(&msg, NULL, 0, 0))
  {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

LRESULT CALLBACK WinProcMain(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  static HWND hWnd_top, hWnd_bottom;
  PAINTSTRUCT ps;

  switch(msg)
  {
  case WM_CREATE:
    hWnd_top = CreateWindow("Chat top", "sockEt chat top", WS_VISIBLE, 0, 0, 640, MAX_MESSAGES * 20, NULL, NULL, HInstance, NULL);
    SetWindowLong(hWnd_top, GWL_STYLE, WS_POPUP);
    SetWindowLong(hWnd_top, GWL_EXSTYLE, WS_EX_TOPMOST);
    ShowWindow(hWnd_top, SW_SHOWMAXIMIZED);

    hWnd_bottom = CreateWindow("Chat bottom", "sockEt chat bottom", WS_VISIBLE, 0, MAX_MESSAGES * 20, 640, 100, NULL, NULL, HInstance, NULL);
    SetWindowLong(hWnd_bottom, GWL_STYLE, WS_POPUP);
    SetWindowLong(hWnd_bottom, GWL_EXSTYLE, WS_EX_TOPMOST);
    ShowWindow(hWnd_bottom, SW_SHOWMAXIMIZED);

    SetParent(hWnd_top, hWnd);
    SetParent(hWnd_bottom, hWnd);
    break;
  case WM_CLOSE:
    NetworkClose();
    PostQuitMessage(0);
    return 0;
    break;
  case WM_PAINT:
    BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    TextOut(GetDC(hWnd), 0, 0, "It is main window", 17);
    return 0;
  default:
    return DefWindowProc(hWnd, msg, wParam, lParam);
  }
  return 0;
}

LRESULT CALLBACK WinProcTop(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  PAINTSTRUCT ps;
  switch(msg)
  {
  case WM_CREATE:
    SetTimer(hWnd, 30, 10, NULL);
    break;
  case WM_TIMER:
    if (top_repaint_needed)
    {
      RECT rect;
      size_t i;

      rect.bottom = MAX_MESSAGES * 20;
      rect.top = 0;
      rect.left = 0;
      rect.right = 640;

      FillRect(GetDC(hWnd), &rect, GetStockObject(GRAY_BRUSH));
      FrameRect(GetDC(hWnd), &rect, GetStockObject(BLACK_BRUSH));
      for (i = 0; i < MessageNum; i++)
        TextOut(GetDC(hWnd), 10, 20 * i, Messages[i], strlen(Messages[i]));
      top_repaint_needed = 0;
    }
    break;
  case WM_PAINT:
    BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    return 0;
  default:
    return DefWindowProc(hWnd, msg, wParam, lParam);
  }
  return 0;
}

LRESULT CALLBACK WinProcBottom(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
  static CHAR Msg[200] = "";
  CHAR tmp[200];
  static size_t repaint_needed = 1;
  PAINTSTRUCT ps;
  size_t i;

  switch(msg)
  {
  case WM_CREATE:
    CreateCaret(hWnd, NULL, 10, 20);
    SetTimer(hWnd, 30, 10, NULL);
    break;
  case WM_TIMER:
    if (repaint_needed)
    {
      RECT rect;

      rect.bottom = 100;
      rect.top = 4;
      rect.left = 4;
      rect.right = 620;

      FillRect(GetDC(hWnd), &rect, GetStockObject(GRAY_BRUSH));
      FrameRect(GetDC(hWnd), &rect, GetStockObject(BLACK_BRUSH));
      TextOut(GetDC(hWnd), 15, 15, Msg, strlen(Msg));
      repaint_needed = 0;
    }
    break;
  case WM_CHAR:
    if (wParam == '\r')
    {
      MessageOut(Msg);
      strcpy(Msg, "");
    }
    else
      if (wParam == '\b')
      {
        i = strlen(Msg);
        if (i > 0)
          Msg[i - 1] = '\0';
      }
      else
      {
        i = strlen(Msg);
        if (i < 190)
        {
          sprintf(tmp, "%s%c", Msg, (char)wParam);
          strcpy(Msg, tmp);
        }
      }
    repaint_needed = 1;
    break;
  case WM_PAINT:
    BeginPaint(hWnd, &ps);
    EndPaint(hWnd, &ps);
    return 0;
  case WM_ERASEBKGND:
    return 0;
  default:
    return DefWindowProc(hWnd, msg, wParam, lParam);
  }
  return 0;
}