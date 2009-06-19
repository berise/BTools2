@echo off

set WORKING_PATH=C:\devel\btools2
set WORKING_VERSION=%WORKING_PATH%\btools_version.h
rem Get the svn revision number
svnversion %WORKING_PATH% > %WORKING_VERSION%
set /p _revision=< %WORKING_VERSION%
rem if x%_revision%==xexported set _revision=0
if x%_revision%==xÀÍ½ºÆ÷Æ®µÊ set _revision=0
if x%_revision%==x set _revision=0

rem Create new version string
echo extern const TCHAR* BTOOLS_SVN_VERSION = _T("%_revision%"); > %WORKING_VERSION%

rem Read and compare the old and the new version string.
rem If they are different replace the old one by the new string
rem else keep the old one to prevent an extra rebuild.
rem set /p _oldrevision=< %WORKING_VERSION%
rem set _oldrevision=x%_oldrevision%
rem set /p _newrevision=< %WORKING_VERSION%
rem set _oldrevision=%_oldrevision:"=%
rem set _newrevision=%_newrevision:"=%
rem if "%_oldrevision%"=="x%_newrevision%" (
rem 	echo "del %WORKING_VERSION%"
rem 	del %WORKING_VERSION%
rem ) else (
rem 	echo "move %WORKING_VERSION% %WORKING_VERSION%"
rem 	move %WORKING_VERSION% %WORKING_VERSION%
rem )

rem Remove the used environment variables.
set _revision=
set _newrevision=
set _oldrevision=
