
# find the GLEW GL Extension Wrangler Library
# bit more robust interface to opengl

FIND_PATH(GLEW_INCLUDE_DIR glew.h
	PATH_SUFFIXES include include/GL
	PATHS
	/usr/local/include)

FIND_LIBRARY(GLEW_LIBRARY
	NAMES glew32 glew
	PATH_SUFFIXES lib64 lib
	PATHS
	/usr/local)
