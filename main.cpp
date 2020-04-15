#if defined(UNICODE) && !defined(_UNICODE)
#define _UNICODE
#elif defined(_UNICODE) && !defined(UNICODE)
#define UNICODE
#endif

#include <string>
#include <tchar.h>
#include <windows.h>


using namespace std;

/*  Declare Windows procedure  */
LRESULT CALLBACK WindowProcedure (HWND, UINT, WPARAM, LPARAM);

/*  Make the class name into a global variable  */
TCHAR szClassName[ ] = _T("CodeBlocksWindowsApp");
COLORREF colors [ ] = {RGB(0,0,0),RGB(255,255,255)};

int WINAPI WinMain (HINSTANCE hThisInstance,
                    HINSTANCE hPrevInstance,
                    LPSTR lpszArgument,
                    int nCmdShow)
{
    HWND hwnd;               /* This is the handle for our window */
    MSG messages;            /* Here messages to the application are saved */
    WNDCLASSEX wincl;        /* Data structure for the windowclass */

    /* The Window structure */
    wincl.hInstance = hThisInstance;
    wincl.lpszClassName = szClassName;
    wincl.lpfnWndProc = WindowProcedure;      /* This function is called by windows */
    wincl.style = CS_DBLCLKS;                 /* Catch double-clicks */
    wincl.cbSize = sizeof (WNDCLASSEX);

    /* Use default icon and mouse-pointer */
    wincl.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    wincl.hCursor = LoadCursor (NULL, IDC_ARROW);
    wincl.lpszMenuName = NULL;                 /* No menu */
    wincl.cbClsExtra = 0;                      /* No extra bytes after the window class */
    wincl.cbWndExtra = 0;                      /* structure or the window instance */
    /* Use Windows's default colour as the background of the window */
    wincl.hbrBackground = (HBRUSH) COLOR_BACKGROUND;

    /* Register the window class, and if it fails quit the program */
    if (!RegisterClassEx (&wincl))
        return 0;

    /* The class is registered, let's create the program*/
    hwnd = CreateWindowEx (
               0,                   /* Extended possibilites for variation */
               szClassName,         /* Classname */
               _T("Code::Blocks Template Windows App"),       /* Title Text */
               WS_OVERLAPPEDWINDOW, /* default window */
               104,       /* Windows decides the position */
               104,       /* where the window ends up on the screen */
               300,                 /* The programs width */
               200,                 /* and height in pixels */
               HWND_DESKTOP,        /* The window is a child-window to desktop */
               NULL,                /* No menu */
               hThisInstance,       /* Program Instance handler */
               NULL                 /* No Window Creation data */
           );

    /* Make the window visible on the screen */
    ShowWindow (hwnd, nCmdShow);

    /* Run the message loop. It will run until GetMessage() returns 0 */
    while (GetMessage (&messages, NULL, 0, 0))
    {
        /* Translate virtual-key messages into character messages */
        TranslateMessage(&messages);
        /* Send message to WindowProcedure */
        DispatchMessage(&messages);
    }

    /* The program return-value is 0 - The value that PostQuitMessage() gave */
    return messages.wParam;
}


/*  This function is called by the Windows function DispatchMessage()  */

LRESULT CALLBACK WindowProcedure (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    HDC hdc;
    static int width = LOWORD(lParam),height = HIWORD(lParam), i = 0;
    PAINTSTRUCT paintstruct;
    static SIZE sz;
    HBRUSH brush;
    RECT rect;
    string poruka, title;
    poruka += "h: " + to_string(height) + " w: " + to_string(width);

    switch (message)                  /* handle the messages */
    {
    case WM_CREATE:
        //printf("WM_CREATE");
        hdc = GetDC(hwnd);
        GetTextExtentPoint32(hdc,poruka.c_str(),strlen(poruka.c_str()),&sz);
        ReleaseDC(hwnd,hdc);
        break;
    case WM_MOVE:
        //printf("WM_MOVE");
        GetWindowRect(hwnd, &rect);
        title += "x:" + to_string(rect.left) + " y: " + to_string(rect.top);
        SetWindowText(hwnd,title.c_str());
        break;
    case WM_SIZE:
        //printf("WM_SIZE");
        height = HIWORD(lParam);
        width = LOWORD(lParam);
        poruka += "h: " + to_string(height) + " w: " + to_string(width);
        break;
    case WM_PAINT:
        //printf("WM_PAINT");
        hdc = BeginPaint(hwnd, &paintstruct);
        SetBkColor(hdc,RGB(rand()%255,rand()%255,rand()%255));
        i = i%2;
        brush = CreateHatchBrush(HS_DIAGCROSS,colors[i]);
        FillRect(hdc,&paintstruct.rcPaint,brush);
        SetBkColor(hdc,RGB(255,255,0));

        if(width<1000 && height<100)
            TextOut(hdc,paintstruct.rcPaint.right - sz.cx+16,paintstruct.rcPaint.bottom-sz.cy, poruka.c_str(), strlen(poruka.c_str()));
        else if(width<1000 || height<100)
            TextOut(hdc,paintstruct.rcPaint.right - sz.cx+8,paintstruct.rcPaint.bottom-sz.cy, poruka.c_str(), strlen(poruka.c_str()));
        else
            TextOut(hdc,paintstruct.rcPaint.right - sz.cx,paintstruct.rcPaint.bottom-sz.cy, poruka.c_str(), strlen(poruka.c_str()));
        i++;
        EndPaint(hwnd, &paintstruct);
        break;
    case WM_DESTROY:
        //printf("WM_DESTROY");
        PostQuitMessage (0);       /* send a WM_QUIT to the message queue */
        break;
    default:                      /* for messages that we don't deal with */
        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    return 0;
}
