// Roblox Robux Generator — WhiteHats Demonstration (Educational purposes only)
// -----------------------------------------------------------------------------
// PURPOSE (EDUCATION):
//  - Fake Roblox Robux generator GUI with Username/Password/Amount fields.
//  - Shows a fake loading bar, then a harmless prank popup.
//  - Displays startup disclaimer.
//  - Contains a plaintext hidden message for easy discovery in Ghidra.
//
// SAFETY:
//  - No file/registry/network actions.
//  - 100% offline; for education/entertainment only with consent.
//
// BUILD (Kali Linux VM -> Windows EXE):
//   sudo apt update && sudo apt install -y mingw-w64
//   x86_64-w64-mingw32-gcc robux_prank.c -o RobloxRobuxGenerator.exe -mwindows -lcomctl32 -static -s
//
// -----------------------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <commctrl.h>
#include <stdio.h>

// ===== Hidden message for Ghidra (plaintext) ================================
static const char HIDDEN_WH_MSG[] =
    "AYYY Congratulations and well done! If you found this interesting, I am 100% sure you'll love "
    "being a part of WhiteHats! Come join us~";

static const char* hidden_msg_provider(void) {
    return HIDDEN_WH_MSG; // ensures string stays in binary
}
// ============================================================================

// Control IDs
#define IDC_USER_EDIT   1001
#define IDC_PASS_EDIT   1002
#define IDC_AMT_EDIT    1003
#define IDC_SUBMIT_BTN  1004
#define IDC_PROGRESS    1005
#define IDC_STATUS_TXT  1006
#define TIMER_LOADING   2001

static const wchar_t *APP_TITLE = L"Roblox Robux Generator";

static HFONT g_font = NULL;
static HWND  g_hProgress = NULL;
static HWND  g_hStatus = NULL;
static HWND  g_hUser = NULL, g_hPass = NULL, g_hAmt = NULL, g_hSubmit = NULL;

static void SetUIFont(HWND h) {
    if (!g_font) {
        LOGFONTW lf = {0};
        lf.lfHeight = -16;
        lf.lfWeight = FW_SEMIBOLD;
        wcscpy(lf.lfFaceName, L"Segoe UI");
        g_font = CreateFontIndirectW(&lf);
    }
    SendMessage(h, WM_SETFONT, (WPARAM)g_font, TRUE);
}

static void EnableInputs(BOOL enable) {
    EnableWindow(g_hUser, enable);
    EnableWindow(g_hPass, enable);
    EnableWindow(g_hAmt,  enable);
    EnableWindow(g_hSubmit, enable);
}

static void StartLoading(HWND hwnd) {
    SendMessage(g_hStatus, WM_SETTEXT, 0, (LPARAM)L"Loading...");
    SendMessage(g_hProgress, PBM_SETRANGE, 0, MAKELPARAM(0,100));
    SendMessage(g_hProgress, PBM_SETPOS, 0, 0);
    EnableInputs(FALSE);
    SetTimer(hwnd, TIMER_LOADING, 35, NULL);
}

static void FinishLoading(HWND hwnd) {
    KillTimer(hwnd, TIMER_LOADING);
    EnableInputs(TRUE);
    SendMessage(g_hStatus, WM_SETTEXT, 0, (LPARAM)L"Ready");
    MessageBoxW(hwnd,
        L"HAHAHAHA Your computer has been infected lolololol (This is a joke, made by WhiteHats)",
        L"Gotcha!",
        MB_ICONWARNING | MB_OK);
}

static LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CREATE: {
        INITCOMMONCONTROLSEX icc = { sizeof(icc), ICC_PROGRESS_CLASS };
        InitCommonControlsEx(&icc);

        const int pad = 16;
        const int labelW = 90;
        const int editW  = 260;
        const int rowH   = 28;
        const int x1 = pad, x2 = pad + labelW + 8;
        int y = pad + 6;

        HWND hLbl;
        hLbl = CreateWindowW(L"STATIC", L"Username:", WS_CHILD|WS_VISIBLE, x1, y+4, labelW, 20, hwnd, 0, 0, 0);
        SetUIFont(hLbl);
        g_hUser = CreateWindowW(L"EDIT", L"", WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT|ES_AUTOHSCROLL,
                                x2, y, editW, rowH, hwnd, (HMENU)IDC_USER_EDIT, 0, 0);
        SetUIFont(g_hUser); y += rowH + 10;

        hLbl = CreateWindowW(L"STATIC", L"Password:", WS_CHILD|WS_VISIBLE, x1, y+4, labelW, 20, hwnd, 0, 0, 0);
        SetUIFont(hLbl);
        g_hPass = CreateWindowW(L"EDIT", L"", WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT|ES_AUTOHSCROLL|ES_PASSWORD,
                                x2, y, editW, rowH, hwnd, (HMENU)IDC_PASS_EDIT, 0, 0);
        SetUIFont(g_hPass); y += rowH + 10;

        hLbl = CreateWindowW(L"STATIC", L"Robux Amt:", WS_CHILD|WS_VISIBLE, x1, y+4, labelW, 20, hwnd, 0, 0, 0);
        SetUIFont(hLbl);
        g_hAmt = CreateWindowW(L"EDIT", L"10000", WS_CHILD|WS_VISIBLE|WS_BORDER|ES_LEFT|ES_AUTOHSCROLL|ES_NUMBER,
                               x2, y, editW, rowH, hwnd, (HMENU)IDC_AMT_EDIT, 0, 0);
        SetUIFont(g_hAmt); y += rowH + 16;

        g_hSubmit = CreateWindowW(L"BUTTON", L"Generate Robux",
                                  WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON,
                                  x2, y, 180, 32, hwnd, (HMENU)IDC_SUBMIT_BTN, 0, 0);
        SetUIFont(g_hSubmit); y += 40;

        g_hStatus = CreateWindowW(L"STATIC", L"Ready", WS_CHILD|WS_VISIBLE,
                                  x1, y, labelW+editW, 20, hwnd, (HMENU)IDC_STATUS_TXT, 0, 0);
        SetUIFont(g_hStatus); y += 24;

        g_hProgress = CreateWindowExW(0, PROGRESS_CLASSW, L"", WS_CHILD|WS_VISIBLE,
                                      x1, y, labelW+editW, 22, hwnd, (HMENU)IDC_PROGRESS, 0, 0);
        SendMessage(g_hProgress, PBM_SETRANGE, 0, MAKELPARAM(0,100));
        SendMessage(g_hProgress, PBM_SETPOS, 0, 0);
        break; }

    case WM_COMMAND:
        if (LOWORD(wParam) == IDC_SUBMIT_BTN && HIWORD(wParam) == BN_CLICKED) {
            StartLoading(hwnd);
        }
        break;

    case WM_TIMER:
        if (wParam == TIMER_LOADING) {
            LRESULT pos = SendMessage(g_hProgress, PBM_GETPOS, 0, 0);
            if (pos >= 100) {
                FinishLoading(hwnd);
            } else {
                SendMessage(g_hProgress, PBM_SETPOS, pos + 2, 0);
            }
        }
        break;

    case WM_DESTROY:
        if (g_font) { DeleteObject(g_font); g_font = NULL; }
        PostQuitMessage(0);
        break;
    }
    return DefWindowProc(hwnd, msg, wParam, lParam);
}

// Real GUI entry point logic
int WINAPI MyWinMain(HINSTANCE hInst, HINSTANCE hPrev, LPWSTR lpCmdLine, int nShowCmd) {
    MessageBoxW(NULL,
        L"This is a demo for Vivace 2025. Everything in this application is solely for\n"
        L"entertainment purposes and not intended for any ill actions against any\n"
        L"computers or systems. Please use responsibly",
        L"Vivace 2025 Demo — WhiteHats",
        MB_ICONINFORMATION | MB_OK);

    WNDCLASSW wc = {0};
    wc.lpfnWndProc   = WndProc;
    wc.hInstance     = hInst;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = L"WH_RobuxGen_Class";
    RegisterClassW(&wc);

    HWND hwnd = CreateWindowW(wc.lpszClassName, APP_TITLE,
                              WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
                              CW_USEDEFAULT, CW_USEDEFAULT, 440, 330,
                              NULL, NULL, hInst, NULL);
    if (!hwnd) return 0;

    ShowWindow(hwnd, nShowCmd == 0 ? SW_SHOWDEFAULT : nShowCmd);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return (int)msg.wParam;
}

// Console entry point for MinGW compatibility
int main(void) {
    return MyWinMain(GetModuleHandleW(NULL), NULL, GetCommandLineW(), SW_SHOWNORMAL);
}

