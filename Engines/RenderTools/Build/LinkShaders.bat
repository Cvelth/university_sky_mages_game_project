REM Create folders for all the outputs, if necessary.
mkdir ..\Release\shaders\ 1>NUL 2>NUL
mkdir ..\Debug\shaders\ 1>NUL 2>NUL
mkdir ..\x64\Release\shaders\ 1>NUL 2>NUL
mkdir ..\x64\Debug\shaders\ 1>NUL 2>NUL

REM Copy files to output folders.
for /f "delims=|" %%f in ('dir /b "shaders\*.glsl"') do (
	copy Shaders\%%f ..\Release\shaders\%%f 1>NUL
	copy Shaders\%%f ..\Debug\shaders\%%f 1>NUL
	copy Shaders\%%f ..\x64\Release\shaders\%%f 1>NUL
	copy Shaders\%%f ..\x64\Debug\shaders\%%f 1>NUL
)

REM Print success to debug console.
echo Shaders were successfully linked.