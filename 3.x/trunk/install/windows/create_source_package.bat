@echo off
::**************************************************************************
:: File:           create_source_package.bat
:: Version:        1.03
:: Name:           RJP Computing 
:: Date:           03/15/2007
:: Description:    Creates a source directory so that the installer can
::                 include only the needed files.
::**************************************************************************
set APP_VERSION=1.05
set APP_TITLE=Create Source Package

echo ----------------------------------------
echo       %APP_TITLE% v%APP_VERSION%
echo                   By
echo             RJP Computing
echo.
echo Creates a source directory so that the
echo installer can include only the needed
echo files.
echo.
echo             Copyright (c) 2007  
echo ----------------------------------------
echo.

:: Cleanup old source.
if exist source rmdir /S /Q source

:: Check to see if 'source' directory exists.
if exist source goto BEGIN_SVN_EXPORT

mkdir source
goto BEGIN_SVN_EXPORT

:BEGIN_SVN_EXPORT
:: Add Subversion install directory to the path.
set SVN_ROOT=C:\Program Files\Subversion
set SVN_EXPORT="%SVN_ROOT%\bin\svn.exe" export --force
set SVN_REPOS=https://wxformbuilder.svn.sourceforge.net/svnroot/wxformbuilder/3.x/trunk

echo Using Subversion with command :
echo     %SVN_EXPORT%
echo.
echo Exporting wxFormBuilder v3.x source from :
echo     %SVN_REPOS%
echo.

echo [svn] Exporting workspace and premake scripts.
%SVN_EXPORT% --non-recursive %SVN_REPOS% source\

echo [svn] Exporting 'output' directory to 'source\output'
%SVN_EXPORT% %SVN_REPOS%/output source\output

echo [svn] Exporting 'plugins' directory to 'source\plugins'
%SVN_EXPORT% %SVN_REPOS%/plugins source\plugins

::echo [svn] Exporting 'premake' directory to 'source\premake'
::%SVN_EXPORT% %SVN_REPOS%/premake source\premake

echo [svn] Exporting 'sdk' directory to 'source\sdk'
%SVN_EXPORT% %SVN_REPOS%/sdk source\sdk

echo [svn] Exporting 'src' directory to 'source\src'
%SVN_EXPORT% %SVN_REPOS%/src source\src


goto END

:END
echo.
echo Finished creating source package...

:: Cleanup environment.
set APP_VERSION=
set APP_TITLE=
set SVN_ROOT=
set SVN_EXPORT=
set SVN_REPOS=

