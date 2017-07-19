REM Create output folder.
mkdir Output\ 1>NUL 2>NUL

REM Compile all the vertex shaders in the source folder.
for /f "delims=|" %%f in ('dir /b "Source\*.vert"') do (
	glslangValidator.exe -V Source\%%f -o Output\%%f.vk.spv
)

REM Compile all the fragment shaders in the source folder.
for /f "delims=|" %%f in ('dir /b "Source\*.frag"') do (
	glslangValidator.exe -V Source\%%f -o Output\%%f.vk.spv
)