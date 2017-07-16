mkdir Output\ 1>NUL 2>NUL

glslangValidator.exe -V Source\CircleShader.vert -o Output\CircleShader.vk.vert.spv
glslangValidator.exe -V Source\CoordinateColorShader.frag -o Output\CoordinateColorShader.vk.frag.spv