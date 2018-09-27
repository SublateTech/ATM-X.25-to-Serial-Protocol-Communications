/****************************************************************************/
/*																		    */
/*  PROGRAMA : BBVA.c							    						*/
/*									    									*/
/*  PROPOSITO: Puente de SDLC a Puerto Serial  								*/
/*	     	   Escrito por Alvaro Medina de PERURIMPEX S.A. 				*/
/****************************************************************************/

#include <windows.h>
#include "bbva.h"
#include "msa.h"

void GateWay(char wParam);

int y=0;

HANDLE hInst;

	HDC	hDC;
	NPTTYINFO  npTTYInfo ;
	DCB dcb;
    int err;
    char  szTemp[ TXQUEUE ] ;
	HFILE hfTempFile;
	char szBuf[TXQUEUE];
    TIMERPROC lpfnMyTimerProc;
	

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
    wc.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE( ICONO_BBVA ) ); //LoadIcon(hInstance, ID_BBVA);
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
	 WS_POPUP | WS_BORDER,			//WS_OVERLAPPED | WS_MINIMIZEBOX,
	350,
	530,
	400,
	20,
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
    
       if ((COMDEV( npTTYInfo ) = OpenComm( "COM5", RXQUEUE, TXQUEUE )) < 0)
 	{ 
 	
	   MessageBox(hWnd,"Error:OpenComm","",MB_OK);
       return ( FALSE ) ;
     }
      
   err = BuildCommDCB("COM5:2400,n,8,1", &dcb);
   if (err < 0) {
    	MessageBox(hWnd, "Ok", "BuildCommDCB", MB_OK); 
     	return 0;
		}              
	err = SetCommState(&dcb);
	if (err < 0) {
    	MessageBox(hWnd, "Ok", "SetCommState", MB_OK); 
    	return 0;
	}
    
    strcpy(szBuf, "c:\\SDLC.TXT");
	hfTempFile = _lcreat(szBuf, 0);

	if (hfTempFile == HFILE_ERROR) {
    	                                               
		MessageBox(hWnd, "Ok", "OpenFile", MB_OK); 
		return ( FALSE);    	                                               
	}
	         
    lpfnMyTimerProc = (TIMERPROC) MakeProcInstance(MyTimerProc, hInstance);
	SetTimer(hWnd, ID_MYTIMER, 300, lpfnMyTimerProc);

         
    SendMessage( hWnd, WM_SYSCOMMAND, (WPARAM) SC_MINIMIZE, 	NULL ) ;
    return (TRUE);
}

long FAR PASCAL MainWndProc(HWND hWnd, unsigned message,
	WORD wParam, LONG lParam)
{
    FARPROC lpProcAbout;
    HMENU   hMenu;
    PAINTSTRUCT ps;

    switch (message) {
	
	case WM_CHAR:
		 GateWay(wParam);
	case WM_SYSCOMMAND:
		switch (wParam) {
	    
		//case SC_MINIMIZE:
		     //MessageBox(hWnd, "Ok", "BuildCommDCB", MB_OK); 
		//     break;
			
		default:		/* Lets Windows process it	 */
		    return (DefWindowProc(hWnd, message, wParam, lParam));
	    }
	    break;
		
	case WM_COMMAND:
	    switch (wParam) {
	    
		default:		/* Lets Windows process it	 */
		    return (DefWindowProc(hWnd, message, wParam, lParam));
	    }
	    break;
	case WM_DESTROY:
	    PostQuitMessage(0);
	    MSAEnd();
	    CloseComm( COMDEV( npTTYInfo ) ) ;
	    _lclose(hfTempFile);
	    break;
	    
	default:
	   // GateWay(wParam, hWnd);
	    return (DefWindowProc(hWnd, message, wParam, lParam));
    }
    return (NULL);
}


#define MAX_TEST_PORTS	50

int	total_port;
int	irq_count;
int	port_no[MAX_TEST_PORTS];


void CALLBACK MyTimerProc(HWND hWnd, UINT msg, UINT idTimer, DWORD dwTime)

{
	unsigned	long	t1;
   char Control[] = {3, -79, 108, 97,0};
   
   int i; 
   HDC	hDC;
    	
//	MessageBox(hWnd, "ok" ,"ok", MB_OK); 
	//ScrollWindow(hWnd, 0, -300, NULL, NULL);
	//UpdateWindow(hWnd);
	

      
      memset( szTemp, 0, TXQUEUE) ;
      if  ((i = MSARead(szTemp, TXQUEUE)) != 0)
         {
    		if (strlen(szTemp)> 10  ) //(strcmp(Control, szTemp))
    		{
    		memset( szBuf, 0, TXQUEUE) ;
    		strcpy(szBuf, "INICIO");
    		strcat(szBuf, szTemp + 34);
    		strcat(szBuf,"FIN");
    		
    		
    		
    		WriteComm(COMDEV( npTTYInfo ),  (LPSTR) szBuf, strlen(szBuf));
 			_lwrite(hfTempFile, (LPSTR) szBuf, strlen(szBuf) );
    		
    		hDC = GetDC(hWnd);
    		TextOut(hDC, 10, y, (LPSTR) szBuf, strlen(szBuf));
    		ReleaseDC(hWnd, hDC);
    		//y+=20;
    		
    		}
    	}    
        
    t1 = MSAIsUnderRun();  
    
    

}

void GateWay(char wParam)
{

   memset( szTemp, 0, TXQUEUE) ;
   *szTemp = wParam;
   //strcpy(szTemp, "Hola");
   WriteComm(COMDEV( npTTYInfo ),  (LPSTR) szTemp, strlen(szTemp));
}
