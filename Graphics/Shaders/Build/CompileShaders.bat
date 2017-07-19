mkdir Output\ 1>NUL 2>NUL

glslangValidator.exe -V Source\Circle.vert -o Output\Circle.vk.vert.spv
glslangValidator.exe -V Source\CoordinateColor.frag -o Output\CoordinateColor.vk.frag.spv

glslangValidator.exe -V Source\Triangle.vert -o Output\Triangle.vk.vert.spv
glslangValidator.exe -V Source\Triangle.frag -o Output\Triangle.vk.frag.spv