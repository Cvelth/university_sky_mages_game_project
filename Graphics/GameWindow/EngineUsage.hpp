#pragma once

#ifdef USE_DEFAULT_ENGINE 
	#ifdef USE_VULKAN_ENGINE	
		#ifdef USE_OPENGL_ENGINE
			#define MULTIPLE_ENGINES_USED	
		#else
			#define MULTIPLE_ENGINES_USED		
		#endif	
	#else	
		#ifdef USE_OPENGL_ENGINE
			#define MULTIPLE_ENGINES_USED		
		#else
			#define DEFAULT_ENGINE_USED		
		#endif
	#endif
#else
	#ifdef USE_VULKAN_ENGINE	
		#ifdef USE_OPENGL_ENGINE
			#define MULTIPLE_ENGINES_USED		
		#else
			#define VULKAN_ENGINE_USED	
		#endif	
	#else	
		#ifdef USE_OPENGL_ENGINE
			#define OPENGL_ENGINE_USED	
		#else
			#define NO_ENGINE_USED	
		#endif
	#endif
#endif

#ifdef NO_ENGINE_USED
	#error You should choose graphics engine to use. Define USE_*ENGINE_NAME*_ENGINE.
#endif

#ifdef MULTIPLE_ENGINES_USED
	#error To many engines were choosen. You should choose only one of them.	
#endif

#ifdef DEFAULT_ENGINE_USED
	#define OPENGL_ENGINE_USED
#endif