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
void Sleep(int Time);                 
char *IfFirst(char *pStr);
char *IfLast(char *pStr);

                     	
	
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
    int nTimer, pTimer, lTrans; 
    int lFirst;
	

int PASCAL WinMain(HANDLE hInstance, HANDLE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
    MSG msg;
    

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
    wc.hIcon = LoadIcon( hInstance, MAKEINTRESOURCE( ICONO_BBVA ) ); 
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
	WS_SYSMENU | WS_MINIMIZEBOX,
	200,
	100,
	290,
	50,
	NULL,
	NULL,
	hInstance,
	NULL
    );
    if (!hWnd)
	return (FALSE);
    
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);
    
    
        

    if ((i = MSAInit()) <= 0) { 				
	wsprintf(tmp, "MOXA INIT error %d", i);     
	MessageBox(hWnd, tmp, NULL, MB_OK | MB_ICONHAND);	
	//	return(FALSE);						
    }		
    
       if ((COMDEV( npTTYInfo ) = OpenComm( "COM5", RXQUEUE, TXQUEUE )) < 0)
 	{ 
 	
	   MessageBox(hWnd,"Error:OpenComm","",MB_OK);
       return ( FALSE ) ;
     }
      
   err = BuildCommDCB("COM5:19200,n,8,1", &dcb);
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
	         
    pTimer = 500;
    lpfnMyTimerProc = (TIMERPROC) MakeProcInstance(MyTimerProc, hInstance);
	SetTimer(hWnd, ID_MYTIMER, pTimer, lpfnMyTimerProc);
    
    nTimer = 0;
    lTrans = 0;
             
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




void CALLBACK MyTimerProc(HWND hWnd, UINT msg, UINT idTimer, DWORD dwTime)

{
	unsigned	long	t1;
   
   	int i,n=0, nFlg = 0, Index=0; 
   	HDC	hDC;
   	char 	*pStr;
   	
   
   
   
   	if (nTimer > 0 ) 
    	
    	if (nTimer > 10000)
    	  	{
    		nTimer = 0;
    		WriteComm(COMDEV( npTTYInfo ),  (LPSTR) "FIN", 3);
    		}
    	else 
    		{
    		nTimer += pTimer;
    		    		
    		}
    		
   
      
   	  hDC = GetDC(hWnd);
      memset( szTemp, 0, TXQUEUE) ;
      if  ((i = MSARead(szTemp, TXQUEUE)) > 0)
         {
    		if ( i > 4  ) //(strcmp(Control, szTemp))
    		{
    		
    		 memset( szBuf, 0, TXQUEUE) ;
    		 
    		 for(n=0;i>n;n++)
    			{
    			if (!szTemp[n]) szTemp[n]=' ';
    		    //if (szTemp[n]== 12) { szTemp[n++]=0; nFlg = 1; break; }
    		    }
    		 
    		 /* Buscando el Inicio de la Transaccion*/
    		 
    		 if (IfFirst(szTemp) != NULL)
    		 	{
    		 	lTrans = 1;
    		 	WriteComm(COMDEV( npTTYInfo ),  (LPSTR) "INICIO", 6);
    		 	strcpy(szBuf, szTemp + 37);
    		 	strcpy(szTemp, szBuf);
    		 	nTimer = pTimer;
    		 	}
    		 
    		 if ((pStr= IfLast(szTemp)) != NULL)
    		 	{
    		 	Index = pStr-szTemp;
    			szTemp[Index] = '\0';
    		 	strcpy(szBuf, szTemp + 5);
    		 	
    		 	}
    		 
    		  if ((IfFirst(szTemp) == NULL) &&  (IfLast(szTemp) == NULL))
    		  		strcpy(szBuf, szTemp + 5);
    		  
    		  if (lTrans)
    		  	{
    		  	WriteComm(COMDEV( npTTYInfo ),  (LPSTR) szBuf, strlen(szBuf));
 			 // 	_lwrite(hfTempFile, (LPSTR) szBuf, strlen(szBuf) );
 			  	}
 			  	
 			  	if (pStr=strchr(szTemp, 12) != NULL)
    		 	{
    		 	lTrans = 0;
    		 	//WriteComm(COMDEV( npTTYInfo ),  (LPSTR) "FIN", 3);
    		 	}
 			  	
    		      		  
    		  			//MessageBox(hWnd, szBuf   ,"ok", MB_OK);    		  
    					//sprintf(szBuf, "%s --> %d ==>%d,%d,%d ---->i=%d\n", szTemp, strlen(szTemp),szTemp[0], szTemp[1], szTemp[2],i);
    					//TextOut(hDC, 10, y, (LPSTR) szBuf, strlen(szBuf));
    					
    					//TextOut(hDC, 10, y, (LPSTR) "                            ", 28);
    					//y+=15;
    	    		 
    		 }   			
    	}    
        
    t1 = MSAIsUnderRun();  
    ReleaseDC(hWnd, hDC);
    

}


char *IfFirst(char *pStr)
{
 pStr=strstr(pStr, "BBVA");
 return pStr;
}

char *IfLast(char *pStr)
{
 pStr=strchr(pStr, 12);
 return pStr;
}

