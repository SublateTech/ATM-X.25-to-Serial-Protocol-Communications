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

void	test1(HWND hWnd);
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
    	//if (MSAIFrame() > 0 )
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
    wc.lpszMenuName =  "SDLC";
    wc.lpszClassName = "SDLCClass";
    return (RegisterClass(&wc));
}

BOOL InitInstance(HANDLE hInstance, int nCmdShow)
{
    HWND hWnd;
    int i;
    char tmp[100];

    hInst = hInstance;

    hWnd = CreateWindow(
	"SDLCClass",
	"Gateway from SDLC to Serial Port",
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
    PAINTSTRUCT ps;

    switch (message) {
	
    
	
	    
	case WM_COMMAND:
	    switch (wParam) {
	    
		case IDM_T1:	test1(hWnd);	break;
		
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
	NPTTYINFO  npTTYInfo ;
	DCB dcb;
    int err;
    char  szTemp[ TXQUEUE ] ;


   if ((COMDEV( npTTYInfo ) = OpenComm( "COM5", RXQUEUE, TXQUEUE )) < 0)
 	{ 
 	
	   MessageBox(hWnd,"Error:OpenComm","",MB_OK);
       return ( FALSE ) ;
     }
      
   err = BuildCommDCB("COM5:9600,n,8,1", &dcb);
   if (err < 0) {
    	MessageBox(hWnd, "Ok", "BuildCommDCB", MB_OK); 
     	return 0;
		}
    
   memset( szTemp, 0, TXQUEUE) ;

   strcpy(szTemp, "Hola");
   
   //SendMessage( hWnd, WM_COMMAND, (WPARAM) IDM_T1, 	NULL ) ;
   
   
    
   WriteComm(COMDEV( npTTYInfo ),  (LPSTR) szTemp, strlen(szTemp));

    
   CloseComm( COMDEV( npTTYInfo ) ) ;


     

    /*
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
	t1 = MSAIsUnderRun(); */
	
	
	
	
	
	ReleaseDC(hWnd, hDC);
}

void	test2(HWND hWnd)
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
