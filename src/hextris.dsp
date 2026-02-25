# Microsoft Developer Studio Project File - Name="hextris" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** 編集しないでください **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=hextris - Win32 Debug
!MESSAGE これは有効なﾒｲｸﾌｧｲﾙではありません。 このﾌﾟﾛｼﾞｪｸﾄをﾋﾞﾙﾄﾞするためには NMAKE を使用してください。
!MESSAGE [ﾒｲｸﾌｧｲﾙのｴｸｽﾎﾟｰﾄ] ｺﾏﾝﾄﾞを使用して実行してください
!MESSAGE 
!MESSAGE NMAKE /f "hextris.mak".
!MESSAGE 
!MESSAGE NMAKE の実行時に構成を指定できます
!MESSAGE ｺﾏﾝﾄﾞ ﾗｲﾝ上でﾏｸﾛの設定を定義します。例:
!MESSAGE 
!MESSAGE NMAKE /f "hextris.mak" CFG="hextris - Win32 Debug"
!MESSAGE 
!MESSAGE 選択可能なﾋﾞﾙﾄﾞ ﾓｰﾄﾞ:
!MESSAGE 
!MESSAGE "hextris - Win32 Release" ("Win32 (x86) Application" 用)
!MESSAGE "hextris - Win32 Debug" ("Win32 (x86) Application" 用)
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "hextris - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "NDEBUG"
# ADD RSC /l 0x411 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"LIBCMT"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "hextris - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x411 /d "_DEBUG"
# ADD RSC /l 0x411 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "hextris - Win32 Release"
# Name "hextris - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Group "ゲーム基底部"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BackGroundImageCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DataFileLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\debug.cpp
# End Source File
# Begin Source File

SOURCE=.\DxGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\Joypad.cpp
# End Source File
# Begin Source File

SOURCE=.\random.cpp
# End Source File
# End Group
# Begin Group "ゲーム管理部"

# PROP Default_Filter ""
# Begin Group "各モード"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\Logo.cpp
# End Source File
# Begin Source File

SOURCE=.\Mode.cpp
# End Source File
# Begin Source File

SOURCE=.\MultiPlay.cpp
# End Source File
# Begin Source File

SOURCE=.\Option.cpp
# End Source File
# Begin Source File

SOURCE=.\ScoreDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\SoloPlay.cpp
# End Source File
# Begin Source File

SOURCE=.\Title.cpp
# End Source File
# End Group
# Begin Source File

SOURCE=.\EraseData.cpp
# End Source File
# Begin Source File

SOURCE=.\GameCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\HextrisCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ScoreManager.cpp
# End Source File
# End Group
# Begin Group "メイン"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\main.cpp
# End Source File
# End Group
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Group "定義ヘッダ"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\common.h
# End Source File
# Begin Source File

SOURCE=.\HexField.h
# End Source File
# Begin Source File

SOURCE=.\keydefine.h
# End Source File
# End Group
# Begin Group "ゲーム基底部ヘッダ"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\BackGroundImageCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DataFileLoader.h
# End Source File
# Begin Source File

SOURCE=.\debug.h
# End Source File
# Begin Source File

SOURCE=.\DxGraph.h
# End Source File
# Begin Source File

SOURCE=.\Joypad.h
# End Source File
# Begin Source File

SOURCE=.\main.h
# End Source File
# Begin Source File

SOURCE=.\random.h
# End Source File
# End Group
# Begin Group "ゲーム管理部ヘッダ"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\EraseData.h
# End Source File
# Begin Source File

SOURCE=.\GameCtrl.h
# End Source File
# Begin Source File

SOURCE=.\HextrisCtrl.h
# End Source File
# Begin Source File

SOURCE=.\logo.h
# End Source File
# Begin Source File

SOURCE=.\Mode.h
# End Source File
# Begin Source File

SOURCE=.\MultiPlay.h
# End Source File
# Begin Source File

SOURCE=.\Option.h
# End Source File
# Begin Source File

SOURCE=.\ScoreDisplay.h
# End Source File
# Begin Source File

SOURCE=.\ScoreManager.h
# End Source File
# Begin Source File

SOURCE=.\soloplay.h
# End Source File
# Begin Source File

SOURCE=.\title.h
# End Source File
# End Group
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Res\hextris.ico
# End Source File
# Begin Source File

SOURCE=.\Res\Resource.rc
# End Source File
# End Group
# Begin Group "StdAfx"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\StdAfx.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# End Target
# End Project
