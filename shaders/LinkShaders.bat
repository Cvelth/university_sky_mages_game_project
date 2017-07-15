mkdir ..\Release\shaders\
mkdir ..\Debug\shaders\

copy CircleShader.vk.vert.spv ..\Release\shaders\CircleShader.vk.vert.spv
copy CircleShader.vk.vert.spv ..\Debug\shaders\CircleShader.vk.vert.spv

copy CoordinateColorShader.vk.frag.spv ..\Release\shaders\CoordinateColorShader.vk.frag.spv
copy CoordinateColorShader.vk.frag.spv ..\Debug\shaders\CoordinateColorShader.vk.frag.spv