/****************************************************************************/
/*									    */
/*  PROGRAM: Example.c							    */
/*									    */
/*  PURPOSE: Demonstrates how to use MOXA serial I/O cards under MS Windows */
/*	     Written by Microsoft C 6.0 				    */
/****************************************************************************/

#include <windows.h>
#include "ex1.h"
#include "../driver/msa.h"

void	test1();
void	test2();

HANDLE hInst;

int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg;
    BOOL bMsgAvail;

    if (!hPrevInstance)
	if (!InitApplication(hInstance))
	    return (FALSE);
    if (!InitInstance(hInstance, nCmdShow))
	return (FALSE);
    while (GetMessage(&msg, NULL, NULL, NULL)) {
	TranslateMessage((LPMSG)&msg);
	DispatchMessage((LPMSG)&msg);
    }
    return (msg.wParam);
}

BOOL InitApplication(HANDLE hInstance)
{
    WNDCLASS wc;

    wc.style = NULL;
    wc.lpfnWndProc = MainWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName =  "ExampleMenu";
    wc.lpszClassName = "ExampleClass";
    return (RegisterClass(&wc));
}

BOOL InitInstance(HANDLE hInstance, int nCmdShow)
{
    HWND hWnd;
    int i;
    char tmp[100];

    hInst = hInstance;

    hWnd = CreateWindow(
	"ExampleClass",
	"MOXA C101 Sync Serial I/O Example Application",
	WS_OVERLAPPEDWINDOW,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	CW_USEDEFAULT,
	NULL,
	NULL,
	hInstance,
	NULL
    );
    if (!hWnd)
	return (FALSE);
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    if ((i = MSAInit()) <= 0) { 				//**
	wsprintf(tmp, "MOXA INIT error %d", i);                 //**
	MessageBox(hWnd, tmp, NULL, MB_OK | MB_ICONHAND);	//**
	return(FALSE);						//**
    }								//**

    return (TRUE);
}

long FAR PASCAL MainWndProc(HWND hWnd, unsigned message,
	WORD wParam, LONG lParam)
{
    FARPROC lpProcAbout;
    HMENU   hMenu;

    switch (message) {
	case WM_COMMAND:
	    switch (wParam) {
		case IDM_T1:	test1(hWnd);	break;
		case IDM_ABOUT:
		    lpProcAbout = MakeProcInstance(About, hInst);
		    DialogBox(hInst, "AboutBox", hWnd, lpProcAbout);
		    FreeProcInstance(lpProcAbout);
		    break;
		default:		/* Lets Windows process it	 */
		    return (DefWindowProc(hWnd, message, wParam, lParam));
	    }
	    break;
	case WM_DESTROY:
	    PostQuitMessage(0);
	    MSAEnd();
	    break;
	default:
	    return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return (NULL);
}

BOOL WINAPI _export About(HWND hDlg, unsigned message,
	WORD wParam, LONG lParam)
{
    switch (message) {
	case WM_INITDIALOG:
	    return (TRUE);
	case WM_COMMAND:
	    if (wParam == IDOK
		|| wParam == IDCANCEL) {
		EndDialog(hDlg, TRUE);
		return (TRUE);
	    }
	    break;
    }
    return (FALSE);
}

#define MAX_TEST_PORTS	50

int	total_port;
int	irq_count;
int	port_no[MAX_TEST_PORTS];

/***************************************************************************/
/*									   */
/*  FUNCTION: test1(HWND)						   */
/*									   */
/*  PURPOSE:  get card info						   */
/*									   */
/***************************************************************************/
void	test1(HWND hWnd)
{
	HDC	hDC;
	int	i, j = 1000, j1 = 1000;
	char	buf[1024], tmp[1024];
	long	total = 0;
	unsigned	long	t1, t2;

	ScrollWindow(hWnd, 0, -300, NULL, NULL);
	UpdateWindow(hWnd);
	hDC = GetDC(hWnd);
	buf[0] = j;
	MSAWrite(buf, j--);
	buf[0] = j;
	MSAWrite(buf, j--);
	while (j1 > 100) {
		t1 = GetCurrentTime();
		while ((i = MSARead(buf, 1024)) == 0)
			if (((t2 = GetCurrentTime()) - t1) > 2000)
				break;
		buf[1] = j1;
		if (i != j1 || buf[0] != buf[1])
			break;
		j1--;
		buf[0] = j;
		MSAWrite(buf, j--);
	}
	t1 = MSAIsUnderRun();
	ReleaseDC(hWnd, hDC);
}
