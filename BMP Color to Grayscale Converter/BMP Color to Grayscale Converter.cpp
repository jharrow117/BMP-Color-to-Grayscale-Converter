// BMP Color to Grayscale Converter.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "BMP Color to Grayscale Converter.h"
#include "GrayscaleConversion.h"

#include <windows.h>
#include <shobjidl.h> // Required for IFileDialog
#include <shlwapi.h>
#include <string>
#include <stringapiset.h>
#include <chrono>

#include <opencv2/imgcodecs.hpp>

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hEditSource;
HWND hEditDest;
HWND hLogStatus;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
std::wstring BrowseFolderOrFile(HWND hwndOwner, bool bFolderOnly);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BMPCOLORTOGRAYSCALECONVERTER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BMPCOLORTOGRAYSCALECONVERTER));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BMPCOLORTOGRAYSCALECONVERTER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BMPCOLORTOGRAYSCALECONVERTER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, 800, 400, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   HWND hSourceLabel = CreateWindowEx(
       0,
       L"STATIC",                       // Window class for text labels
       L"Source BMP file:",
       WS_CHILD | WS_VISIBLE | SS_LEFT,
       20, 20, 120, 20,                 // X, Y position and Width, Height
       hWnd,                            // Handle to the parent window
       (HMENU)IDC_SOURCE_LABEL,         // Unique identifier ID
       hInstance,
       NULL
   );

   hEditSource = CreateWindowEx(
       WS_EX_CLIENTEDGE,                // Extended style: gives it a sunken 3D border
       L"EDIT",                         // Window class for edit boxes
       L"",
       WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, // Window & Edit styles
       20, 50, 500, 25,                 // X, Y positions and Width, Height (pixels)
       hWnd,                            // Parent window handle
       (HMENU)IDC_SOURCE_FILE_EDITBOX,  // Unique child window identifier (ID)
       (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
       NULL
   );

   HWND hSourceButton = CreateWindowEx(
       0,
       L"BUTTON",                                               // Window class for buttons
       L"Browse...",
       WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,   // Button styles
       530, 50, 120, 25,                                        // X, Y position and Width, Height
       hWnd,                                                    // Handle to the parent window
       (HMENU)IDC_BTN_SOURCE_BROWSE,                            // Unique identifier ID
       hInstance,
       NULL
   );

   HWND hDestLabel = CreateWindowEx(
       0,
       L"STATIC",                       // Window class for static text
       L"Destination Folder:",
       WS_CHILD | WS_VISIBLE | SS_LEFT,
       20, 80, 130, 25,                 // X, Y position and Width, Height
       hWnd,                            // Handle to the parent window
       (HMENU)IDC_DEST_LABEL,           // Unique identifier ID
       hInstance,
       NULL
   );

   hEditDest = CreateWindowEx(
       WS_EX_CLIENTEDGE,                // Extended style: gives it a sunken 3D border
       L"EDIT",                         // Window class for edit boxes
       L"",
       WS_CHILD | WS_VISIBLE | WS_TABSTOP | ES_AUTOHSCROLL, // Window & Edit styles
       20, 100, 500, 25,                // X, Y positions and Width, Height (pixels)
       hWnd,                            // Parent window handle
       (HMENU)IDC_DEST_FILE_EDITBOX,    // Unique identifier ID
       (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE),
       NULL
   );

   HWND hDestButton = CreateWindowEx(
       0,
       L"BUTTON",                     // Window class for text labels
       L"Browse...",           // The actual text displayed
       WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Button styles
       530, 100, 120, 25,               // X, Y position and Width, Height
       hWnd,                    // Handle to the parent window
       (HMENU)IDC_BTN_DEST_BROWSE,           // Unique identifier ID
       hInstance,
       NULL
   );

   HWND hConvertButton = CreateWindowEx(
       0,
       L"BUTTON",                     // Window class for text labels
       L"Convert",           // The actual text displayed
       WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, // Button styles
       530, 150, 120, 25,               // X, Y position and Width, Height
       hWnd,                    // Handle to the parent window
       (HMENU)IDC_BTN_CONVERT,           // Unique identifier ID
       hInstance,
       NULL
   );

   hLogStatus = CreateWindowEx(
       0,
       L"EDIT",     // Window class for edit boxes
       L"",
       WS_CHILD | WS_VISIBLE | SS_LEFT | ES_MULTILINE | ES_READONLY,
       20, 150, 500, 150,               // X, Y position and Width, Height
       hWnd,                    // Handle to the parent window
       (HMENU)IDC_TEXT_STATUS,           // Unique identifier ID
       hInstance,
       NULL
   );

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            case IDC_BTN_SOURCE_BROWSE: // User clicked Source Browse button
            {
                // TODO: only allow bmp files to be selected
                std::wstring srcFile = BrowseFolderOrFile(hWnd, false);
                if (!srcFile.empty()) {
                    // Set the text of your Source Edit Box
                    SetDlgItemText(hWnd, IDC_SOURCE_FILE_EDITBOX, srcFile.c_str());
                }
            }
            break;
            case IDC_BTN_DEST_BROWSE: // User clicked Destination Browse button
            {
                std::wstring destFolder = BrowseFolderOrFile(hWnd, true);
                if (!destFolder.empty()) {
                    // Set the text of your Destination Edit Box
                    SetDlgItemText(hWnd, IDC_DEST_FILE_EDITBOX, destFolder.c_str());
                }
            }
            break;
            case IDC_BTN_CONVERT:
            {
                int textLengthSource = GetWindowTextLength(hEditSource);
                std::string strFullSourcePath;
                if (textLengthSource > 0) 
                {
                    // Display some logging showing that the file is being processed
                    std::wstring wstrBuffer;
                    wstrBuffer.resize(textLengthSource + 1);
                    GetWindowText(hEditSource, &wstrBuffer[0], wstrBuffer.size());
                    wstrBuffer.resize(textLengthSource);
                    std::wstring wstrSourceFile(wstrBuffer);
                    wstrSourceFile.insert(0, L"Converting ");
                    wstrSourceFile.append(L" to grayscale...\r\n");
                    SetWindowText(hLogStatus, wstrSourceFile.c_str());

                    // Get a string for the source path
                    std::string strSourceBuffer;
                    strSourceBuffer.resize(textLengthSource + 1);
                    GetWindowTextA(hEditSource, &strSourceBuffer[0], strSourceBuffer.size());
                    strSourceBuffer.resize(textLengthSource);
                    std::string strFullSourcePathCopy(strSourceBuffer);
                    strFullSourcePath = strFullSourcePathCopy;
                }

                else
                {
                    SetWindowText(hLogStatus, L"Source path is empty!");
                    break;
                }

                // Get a string for the destination path 
                int textLengthDest = GetWindowTextLength(hEditDest);
                std::string strDestPath;
                if (textLengthDest)
                {
                    std::string strDestBuffer;
                    strDestBuffer.resize(textLengthDest + 1);
                    GetWindowTextA(hEditDest, &strDestBuffer[0], strDestBuffer.size());
                    strDestBuffer.resize(textLengthDest);
                    std::string strDestPathCopy(strDestBuffer);
                    strDestPath = strDestPathCopy;
                }

                else
                {
                    SetWindowText(hLogStatus, L"Destination path is empty!");
                    break;
                }

                // Process the file
                GrayscaleConverter fileConverter;
                fileConverter.ReadFile(strFullSourcePath, strDestPath);

                if (!fileConverter.ConvertToGrayscale())
                {
                    SetWindowText(hLogStatus, L"Grayscale conversion failed...\r\nCheck the validity of the file paths.");
                }
                else
                {
                    std::string str = fileConverter.GetFullDestPath();
                    std::wstring wstrFullDestPath(str.begin(), str.end());
                    wstrFullDestPath.insert(0, L"Grayscale conversion succeeded in " + fileConverter.GetTimeElapsedWString_ms() + L"ms!\r\nThe new file can be found at ");
                    LPWSTR lpwstr = wstrFullDestPath.data();                        
                    SetWindowText(hLogStatus, lpwstr);
                }
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
            BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code here...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
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

// Helper function to launch the Windows Folder Browse dialog
std::wstring BrowseFolderOrFile(HWND hwndOwner, bool bFolderOnly) {
    std::wstring selectedPath = L"";

    IFileOpenDialog* pFileOpen;
    // Create the File Open Dialog object
    HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileOpen));

    if (SUCCEEDED(hr)) {
        // Set the dialog options to allow folder picking
        FILEOPENDIALOGOPTIONS fos;
        if (SUCCEEDED(pFileOpen->GetOptions(&fos))) {
            if (bFolderOnly)
                pFileOpen->SetOptions(fos | FOS_PICKFOLDERS);
            else
                pFileOpen->SetOptions(fos | FOS_FILEMUSTEXIST);
        }

        // Show the Open dialog box
        hr = pFileOpen->Show(hwndOwner);

        if (SUCCEEDED(hr)) {
            IShellItem* pItem;
            hr = pFileOpen->GetResult(&pItem);
            if (SUCCEEDED(hr)) {
                PWSTR pszFilePath;
                hr = pItem->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &pszFilePath);
                if (SUCCEEDED(hr)) {
                    selectedPath = pszFilePath;
                    CoTaskMemFree(pszFilePath);
                }
                pItem->Release();
            }
        }
        pFileOpen->Release();
    }
    return selectedPath;
}