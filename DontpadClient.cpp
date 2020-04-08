// DontpadClient.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "DontpadClient.h"
#include "Connectioon.h"
#include <Windows.h>
#include <commctrl.h>
#include <Richedit.h>
#define MAX_LOADSTRING 100
#pragma comment(lib, "comctl32.lib")
// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
WSADATA wsaData;
HWND TextBox;
HWND GoButton;
HWND EditFile;
HWND RefreshButton;
HWND SaveButton;
HWND PasswordLockButton;
HWND LockPassword;
HWND backButton;
HWND RemoveLock;

wchar_t text[1024];
wchar_t password[1024];
wchar_t *data;
void init();
Connection *conn;
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    LoadLibrary(TEXT("Msftedit.dll"));
    InitCommonControls();
    init();
    data = new wchar_t[1024 * 1024];
    conn = new Connection("127.0.0.1", "9090");
    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DONTPADCLIENT, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);
 
    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DONTPADCLIENT));

    MSG msg;
    
    conn->write("test");
    auto resp = conn->read();
    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    WSACleanup();
    return (int) msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DONTPADCLIENT));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DONTPADCLIENT);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX ),
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
           
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case 1: {
                int gwstat;
                memset(password, 0, sizeof(password));
                memset(data, 0, sizeof(data));
                gwstat = GetWindowText(TextBox, text, 1024);
                EnableWindow(TextBox, FALSE);
                EnableWindow(GoButton, FALSE);
                ShowWindow(TextBox, FALSE);
                ShowWindow(GoButton, FALSE);
                std::wstring aux(text);
                std::string command = std::string("get ") + std::string("\"") + std::string(aux.begin(),aux.end()) + "\"";
                conn->write(command);
                EditFile = CreateWindowEx(0, MSFTEDIT_CLASS, TEXT(""), ES_MULTILINE | WS_VISIBLE | WS_CHILD | WS_BORDER | WS_TABSTOP|WS_VSCROLL, 1, 40, 800, 800, hWnd, NULL, hInst, NULL);
                std::string resp = conn->read();
                std::wstring out_text(resp.begin(), resp.end());
                out_text.erase(out_text.begin(), out_text.begin() + 3);
                SetWindowTextW(EditFile, out_text.c_str());
                ::SetWindowPos(hWnd,0,0,0,820,900, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
                RefreshButton=CreateWindow(L"BUTTON", L"Refresh", WS_VISIBLE | WS_CHILD | WS_BORDER, 115, 5, 100, 30, hWnd, (HMENU)3, NULL, NULL);
                SaveButton = CreateWindow(L"BUTTON", L"Save", WS_VISIBLE | WS_CHILD | WS_BORDER, 220, 5, 100, 30, hWnd, (HMENU)4, NULL, NULL);
                PasswordLockButton = CreateWindow(L"BUTTON", L"Lock", WS_VISIBLE | WS_CHILD | WS_BORDER, 325, 5, 100, 30,hWnd, (HMENU)5, NULL, NULL);
                LockPassword = CreateWindow(L"EDIT", L"", WS_BORDER | WS_CHILD | WS_VISIBLE |ES_PASSWORD, 430, 5, 200, 30, hWnd, NULL, NULL, NULL);
                backButton= CreateWindow(L"BUTTON", L"Back", WS_VISIBLE | WS_CHILD | WS_BORDER, 10, 5, 100, 30, hWnd, (HMENU)6, NULL, NULL);
                RemoveLock = CreateWindow(L"BUTTON", L"Unlock", WS_VISIBLE | WS_CHILD | WS_BORDER, 635, 5, 100, 30, hWnd, (HMENU)7, NULL, NULL);
                break;
            }
            case 3: {
                memset(password, 0, sizeof(password));
                memset(data, 0, sizeof(data));
                std::wstring aux(text);
                std::string command = std::string("get ") + std::string("\"") + std::string(aux.begin(), aux.end()) + "\"";
                conn->write(command);
                std::string resp = conn->read();
                std::wstring out_text(resp.begin(), resp.end());
                out_text.erase(out_text.begin(), out_text.begin() + 3);
                SetWindowTextW(EditFile, out_text.c_str());
                break;
            }
            case 4: {
                memset(password, 0, sizeof(password));
                memset(data, 0, sizeof(data));
                GetWindowText(LockPassword, password, 1024);
                GetWindowText(EditFile, data, 1024 * 1024);
                std::wstring aux(text);
                std::wstring aux2(password);
                std::wstring aux3(data);
                std::string command = "write " + std::string("\"") + std::string(aux.begin(), aux.end()) + std::string("\" \"") + std::string(aux2.begin(), aux2.end()) + std::string("\" \"")+std::string(aux3.begin(),aux3.end())+std::string("\"");
                conn->write(command);
                std::string resp = conn->read();
                if (resp == "Private Data" || resp == "Wrong Password")
                    ::MessageBox(hWnd, L"Can't Perform operatration", (LPCWSTR)(std::wstring(resp.begin(), resp.end()).c_str()), MB_OK);
                break;
            }
            case 5: {
                memset(password, 0, sizeof(password));
                memset(data, 0, sizeof(data));
                GetWindowText(LockPassword, password, 1024);
                std::wstring aux(text);
                std::wstring aux2(password);
                std::string command = "lock " + std::string("\"") + std::string(aux.begin(), aux.end()) + std::string("\" \"") + std::string(aux2.begin(), aux2.end()) + std::string("\"");
                conn->write(command);
                std::string resp = conn->read();
               if(resp=="already locked")
                   ::MessageBox(hWnd, L"Can't Perform operatration", (LPCWSTR)(std::wstring(resp.begin(), resp.end()).c_str()), MB_OK);
                break;
            }
            case 6: {
                EnableWindow(TextBox, TRUE);
                EnableWindow(GoButton, TRUE);
                ShowWindow(TextBox, TRUE);
                ShowWindow(GoButton, TRUE);
                EnableWindow(RefreshButton, FALSE);
                EnableWindow(EditFile, FALSE);
                ShowWindow(RefreshButton, FALSE);
                ShowWindow(EditFile, FALSE);
                EnableWindow(PasswordLockButton, FALSE);
                EnableWindow(SaveButton, FALSE);
                ShowWindow(SaveButton, FALSE);
                ShowWindow(PasswordLockButton, FALSE);
                EnableWindow(backButton, FALSE);
                EnableWindow(LockPassword, FALSE);
                ShowWindow(backButton, FALSE);
                ShowWindow(LockPassword , FALSE);
                ShowWindow(RemoveLock, FALSE);
                EnableWindow(RemoveLock, FALSE);
                ::SetWindowPos(hWnd, 0, 0, 0, 400, 125, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
                break;
            }
            case 7: {
                memset(password, 0, sizeof(password));
                memset(data, 0, sizeof(data));
                GetWindowText(LockPassword, password, 1024);
                std::wstring aux(text);
                std::wstring aux2(password);
                std::string command = "removelock " + std::string("\"") + std::string(aux.begin(), aux.end()) + std::string("\" \"") + std::string(aux2.begin(), aux2.end()) + std::string("\"");
                conn->write(command);
                std::string resp = conn->read();
                if(resp=="wrong password")
                    ::MessageBox(hWnd, L"Can't Perform operatration", (LPCWSTR)(std::wstring(resp.begin(), resp.end()).c_str()), MB_OK);
                break;
            }
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CREATE:
        ::SetWindowPos(hWnd, 0, 0,0, 400,125,SWP_NOMOVE|SWP_NOOWNERZORDER|SWP_NOZORDER);
        TextBox = CreateWindow(L"EDIT", L"", WS_BORDER | WS_CHILD | WS_VISIBLE, 20, 20, 300, 20,hWnd,NULL,NULL,NULL);
        GoButton = CreateWindow(L"BUTTON", L"Go", WS_VISIBLE | WS_CHILD | WS_BORDER, 340, 20, 30, 20, hWnd, (HMENU)1, NULL, NULL);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}


void init() {
    int iResult = WSAStartup(MAKEWORD(2, 2),&wsaData);
    if (iResult) {
        printf("WSAStartup failed %d\n", iResult);
        exit(1);
    }
}

