REM Create folders for all the outputs, if necessary.
mkdir ..\Release\ 1>NUL 2>NUL
mkdir ..\Debug\ 1>NUL 2>NUL
mkdir ..\x64\Release\ 1>NUL 2>NUL
mkdir ..\x64\Debug\ 1>NUL 2>NUL

REM Copy files to output folders.
for /f "delims=|" %%f in ('dir /b "*.ini"') do (
	copy %%f ..\Release\%%f 1>NUL
	copy %%f ..\Debug\%%f 1>NUL
	copy %%f ..\x64\Release\%%f 1>NUL
	copy %%f ..\x64\Debug\%%f 1>NUL
)

REM Print success to debug console.
echo Settings were successfully linked.