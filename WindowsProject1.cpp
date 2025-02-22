// compile with: /D_UNICODE /DUNICODE /DWIN32 /D_WINDOWS /c

#include <windows.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <iostream>
#include <cmath>
#include <stdio.h>
#include <CommCtrl.h>
#include <string>
#include <locale>

#pragma comment(lib, "comctl32.lib")

using namespace std;

// Global variables

// The main window class name.
static TCHAR szWindowClass[] = _T("Калькулятор Rev");

// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("Калькулятор Rev");

// Stored instance handle for use in Win32 API calls such as FindResource
HINSTANCE hInst;

// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("Калькулятор"),
            NULL);

        return 1;
    }

    // Store instance handle in our global variable
    hInst = hInstance;

    // The parameters to CreateWindowEx explained:
    // WS_EX_OVERLAPPEDWINDOW : An optional extended window style.
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClass,
        szTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("Калькулятор"),
            NULL);

        return 1;
    }

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc, hCompatibleDc;
    HANDLE hbitmap, holdbitmap;
    BITMAP bitmap;
    RECT rect;
    HINSTANCE hInst;


    HFONT hFont;
    HFONT hFontOld;

    static HWND hNeedEdit, hHaveEdit, hPriceEdit, hResultEdit;
    static HWND hCalcButton;

    int y = 0;
    int spacing = 25;
    int height = 0;
    int lvl = 1;

    TCHAR Str1[20];
    int a = 0;
    int b = 0;
    int Len = 0;

    TCHAR text1[] = "Введите уровень";
    TCHAR text2[] = "необходимого камня:";
    TCHAR text3[] = "Введите цену на ауке:";



    switch (message)
    {
    case WM_CREATE:

        hNeedEdit = CreateWindowEx(WS_EX_CLIENTEDGE,
            "EDIT",
            "",
            WS_VISIBLE | WS_CHILD | WS_BORDER,
            180,
            50,
            100,
            20,
            hWnd,
            (HMENU)1,
            NULL, NULL);

        hHaveEdit = CreateWindowEx(WS_EX_CLIENTEDGE,
            "EDIT",
            "",
            WS_VISIBLE | WS_CHILD | WS_BORDER,
            180,
            150,
            100,
            20,
            hWnd,
            (HMENU)2,
            NULL, NULL);

        hPriceEdit = CreateWindowEx(WS_EX_CLIENTEDGE,
            "EDIT",
            "",
            WS_VISIBLE | WS_CHILD | WS_BORDER,
            180,
            100,
            100,
            20,
            hWnd,
            (HMENU)3,
            NULL, NULL);

        hResultEdit = CreateWindowEx(WS_EX_CLIENTEDGE,
            "EDIT",
            "",
            WS_VISIBLE | WS_CHILD | WS_BORDER | ES_READONLY,
            500,
            54,
            100,
            20,
            hWnd,
            (HMENU)4,
            NULL, NULL);

        hCalcButton = CreateWindowEx(
            0,
            "BUTTON",
            "CALC!",
            WS_VISIBLE | WS_CHILD | WS_BORDER,
            35,
            200,
            250,
            50,
            hWnd,
            (HMENU)5,
            NULL, NULL);

        break;

    case WM_COMMAND:
        if (LOWORD(wParam) == 5) {

            char buffer[256];
            long long need, have, price;
            long long finalAmount = 0;
            try {
                GetWindowText(hNeedEdit, buffer, 256);
                need = stoll(buffer);
                GetWindowText(hHaveEdit, buffer, 256);
                have = stoll(buffer);
                GetWindowText(hPriceEdit, buffer, 256);
                price = stoll(buffer);

                //Improved Error Handling
                if (need < 0 || have < 0 || price < 0 || need == 0) {
                    SetWindowText(hResultEdit, "Invalid Input.");
                    break;
                }
                if (need > 0) {
                    for (int i = 1; i < need; i++) {
                        finalAmount = (long long)(3 * pow(3, i - 1)) - have;
                    }
                }
            }
            catch (const invalid_argument& e) {
                SetWindowText(hResultEdit, "Invalid Input (Not a number).");
                break;
            }
            catch (const out_of_range& e) {
                SetWindowText(hResultEdit, "Invalid Input (Number too large).");
                break;
            }

            char resultBuffer[50];
            int charsWritten = snprintf(resultBuffer, sizeof(resultBuffer), "%lld", finalAmount);
            if (charsWritten < 0 || charsWritten >= sizeof(resultBuffer)) {
                SetWindowText(hResultEdit, "Result too large!");
            }
            else {
                SetWindowText(hResultEdit, resultBuffer);
            }
            break;
        }

    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        hbitmap = LoadImage(NULL, "mg.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
        GetObject(hbitmap, sizeof(BITMAP), &bitmap);
        hCompatibleDc = CreateCompatibleDC(hdc);
        holdbitmap = SelectObject(hCompatibleDc, hbitmap);
        GetClientRect(hWnd, &rect);
        StretchBlt(hdc, 0, 0, rect.right, rect.bottom, hCompatibleDc, 0, 0, bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);
        SelectObject(hCompatibleDc, holdbitmap);
        DeleteObject(hbitmap);
        DeleteDC(hCompatibleDc);

        hFontOld = (HFONT)SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
        hFont = CreateFont(16, 0, 0, 0, 0, 0, 0, 0,
            DEFAULT_CHARSET,
            0, 0, 0, VARIABLE_PITCH,
            "Arial Bold");
        if (hFont == NULL) {
            // Обработка ошибки
            EndPaint(hWnd, &ps);
            return 0;
        }

        SelectObject(hdc, hFont);
        SetTextColor(hdc, RGB(255, 255, 0));
        SetBkMode(hdc, TRANSPARENT);
        TextOut(hdc, 35, 46, text1, ARRAYSIZE(text1));
        TextOut(hdc, 35, 62, text2, ARRAYSIZE(text2));
        TextOut(hdc, 35, 100, text3, ARRAYSIZE(text3));
        SelectObject(hdc, hFontOld);
        DeleteObject(hFont);
        y = 10;
        hFont = CreateFont(18, 0, 0, 0, FW_BOLD, FALSE, FALSE, FALSE,
            DEFAULT_CHARSET,
            0, 0, 0, VARIABLE_PITCH,
            "Arial");
        SelectObject(hdc, hFont);
        for (lvl = 1; lvl <= 8; lvl++) {
            char txt[32] = { 0 };
            snprintf(txt, 31, "камней уровня %i", lvl);
            TextOut(hdc, 605, y += 44, txt, strlen(txt));
        }

        SetBkMode(hdc, OPAQUE);
        SelectObject(hdc, hFontOld);
        DeleteObject(hFont);
        // Here your application is laid out.
        // For this introduction, we just print out "Hello, Windows desktop!"
        // in the top left corner.
        // End application-specific layout section.

        EndPaint(hWnd, &ps);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}