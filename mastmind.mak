# Microsoft Developer Studio Generated NMAKE File, Format Version 4.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=mastmind - Win32 Debug
!MESSAGE No configuration specified.  Defaulting to mastmind - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "mastmind - Win32 Release" && "$(CFG)" !=\
 "mastmind - Win32 Debug"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "mastmind.mak" CFG="mastmind - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "mastmind - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "mastmind - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe

!IF  "$(CFG)" == "mastmind - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
OUTDIR=.\Release
INTDIR=.\Release

ALL : "$(OUTDIR)\mastmind.exe"

CLEAN : 
	-@erase ".\Release\mastmind.exe"
	-@erase ".\Release\Mm.obj"
	-@erase ".\Release\Mm.res"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/mastmind.pch" /YX /Fo"$(INTDIR)/" /c 
CPP_OBJS=.\Release/
CPP_SBRS=
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
MTL_PROJ=/nologo /D "NDEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Mm.res" /d "NDEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/mastmind.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/mastmind.pdb" /machine:I386 /out:"$(OUTDIR)/mastmind.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Mm.obj" \
	"$(INTDIR)/Mm.res"

"$(OUTDIR)\mastmind.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "mastmind - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
OUTDIR=.\Debug
INTDIR=.\Debug

ALL : "$(OUTDIR)\mastmind.exe"

CLEAN : 
	-@erase ".\Debug\mastmind.exe"
	-@erase ".\Debug\Mm.obj"
	-@erase ".\Debug\Mm.res"
	-@erase ".\Debug\mastmind.ilk"
	-@erase ".\Debug\mastmind.pdb"
	-@erase ".\Debug\vc40.pdb"
	-@erase ".\Debug\vc40.idb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /c
CPP_PROJ=/nologo /MLd /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS"\
 /Fp"$(INTDIR)/mastmind.pch" /YX /Fo"$(INTDIR)/" /Fd"$(INTDIR)/" /c 
CPP_OBJS=.\Debug/
CPP_SBRS=
# ADD BASE MTL /nologo /D "_DEBUG" /win32
# ADD MTL /nologo /D "_DEBUG" /win32
MTL_PROJ=/nologo /D "_DEBUG" /win32 
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
RSC_PROJ=/l 0x409 /fo"$(INTDIR)/Mm.res" /d "_DEBUG" 
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/mastmind.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib\
 odbccp32.lib /nologo /subsystem:windows /incremental:yes\
 /pdb:"$(OUTDIR)/mastmind.pdb" /debug /machine:I386\
 /out:"$(OUTDIR)/mastmind.exe" 
LINK32_OBJS= \
	"$(INTDIR)/Mm.obj" \
	"$(INTDIR)/Mm.res"

"$(OUTDIR)\mastmind.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_OBJS)}.obj:
   $(CPP) $(CPP_PROJ) $<  

.c{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx{$(CPP_SBRS)}.sbr:
   $(CPP) $(CPP_PROJ) $<  

################################################################################
# Begin Target

# Name "mastmind - Win32 Release"
# Name "mastmind - Win32 Debug"

!IF  "$(CFG)" == "mastmind - Win32 Release"

!ELSEIF  "$(CFG)" == "mastmind - Win32 Debug"

!ENDIF 

################################################################################
# Begin Source File

SOURCE=.\Mm.c
DEP_CPP_MM_C0=\
	".\mm.h"\
	

"$(INTDIR)\Mm.obj" : $(SOURCE) $(DEP_CPP_MM_C0) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mm.dlg

!IF  "$(CFG)" == "mastmind - Win32 Release"

!ELSEIF  "$(CFG)" == "mastmind - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Pegover.cur

!IF  "$(CFG)" == "mastmind - Win32 Release"

!ELSEIF  "$(CFG)" == "mastmind - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Peg.cur

!IF  "$(CFG)" == "mastmind - Win32 Release"

!ELSEIF  "$(CFG)" == "mastmind - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Overwell.cur

!IF  "$(CFG)" == "mastmind - Win32 Release"

!ELSEIF  "$(CFG)" == "mastmind - Win32 Debug"

!ENDIF 

# End Source File
################################################################################
# Begin Source File

SOURCE=.\Mm.rc
DEP_RSC_MM_RC=\
	".\mm.ico"\
	".\Overwell.cur"\
	".\Peg.cur"\
	".\Pegover.cur"\
	".\mm.h"\
	

"$(INTDIR)\Mm.res" : $(SOURCE) $(DEP_RSC_MM_RC) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
# End Target
# End Project
################################################################################
