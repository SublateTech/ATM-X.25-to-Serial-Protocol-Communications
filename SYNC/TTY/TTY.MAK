# Microsoft Visual C++ generated build script - Do not modify

PROJ = TTY
DEBUG = 1
PROGTYPE = 0
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = 
R_RCDEFINES = 
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = C:\BBVA\SYNC\TTY\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC = TTY.C       
FIRSTCPP =             
RC = rc
CFLAGS_D_WEXE = /nologo /YX /G2 /Zp1 /W3 /AM /Od /D "_DEBUG" /D "_WINDOWS" /FR /GA /Zi
CFLAGS_R_WEXE = /nologo /YX /G2 /Zp1 /W3 /AM /Ox /D "NDEBUG" /D "_WINDOWS" /FR /GA /Gs
LFLAGS_D_WEXE = /NOLOGO /NOD /STACK:4096 /ALIGN:16 /ONERROR:NOEXE /CO 
LFLAGS_R_WEXE = /NOLOGO /NOD /STACK:4096 /ALIGN:16 /ONERROR:NOEXE
LIBS_D_WEXE = oldnames libw commdlg shell olecli olesvr mlibcew commdlg.lib  libw
LIBS_R_WEXE = oldnames libw commdlg shell olecli olesvr mlibcew commdlg.lib  libw
RCFLAGS = /NOLOGO  
RESFLAGS = /NOLOGO  -t
RUNFLAGS = 
DEFFILE = TTY.DEF
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_WEXE)
LFLAGS = $(LFLAGS_D_WEXE)
LIBS = $(LIBS_D_WEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_WEXE)
LFLAGS = $(LFLAGS_R_WEXE)
LIBS = $(LIBS_R_WEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = TTY.SBR


TTY_DEP = c:\bbva\sync\tty\tty.h \
	c:\bbva\sync\tty\version.h


TTY_RCDEP = c:\bbva\sync\tty\tty.ico


all:	$(PROJ).EXE $(PROJ).BSC

TTY.OBJ:	TTY.C $(TTY_DEP)
	$(CC) $(CFLAGS) $(CCREATEPCHFLAG) /c TTY.C

TTY.RES:	TTY.RC $(TTY_RCDEP)
	$(RC) $(RCFLAGS) $(RCDEFINES) -r TTY.RC


$(PROJ).EXE::	TTY.RES

$(PROJ).EXE::	TTY.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
TTY.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
c:\msvc\lib\+
c:\msvc\mfc\lib\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF
	$(RC) $(RESFLAGS) TTY.RES $@
	@copy $(PROJ).CRF MSVC.BND

$(PROJ).EXE::	TTY.RES
	if not exist MSVC.BND 	$(RC) $(RESFLAGS) TTY.RES $@

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
