CC 			  = gcc
ARR			  = ar

VESRION_C     = 11

PATH_SRC 	      		= src
PATH_INCLUDE      		= include
PATH_EXAMPLES	  		= example
PATH_SimpleOfuscator 	= ./lib/SimpleOfuscator
PATH_LZ77_c		  		= $(PATH_SimpleOfuscator)/LZ77_c

LINKER_FLAGS  	    = 									\
	-L.							-lGetCommandOutputC		\
	-L$(PATH_SimpleOfuscator)	-lSimpleOfuscator		\
	-L$(PATH_LZ77_c) 			-lLZ77_c				\


INCLUDE_FLAGS = -I. 									\
				-I$(PATH_INCLUDE)						\
				-I$(PATH_SimpleOfuscator)/include		\
				-I$(PATH_LZ77_c)/include
GLOBAL_CFLAGS = -std=c$(VESRION_C) $(INCLUDE_FLAGS) -masm=intel \
				-D_ExceptionHandler -fdiagnostics-color=always $(DEBUG_LINUX)

CFLAGS 		  =  $(GLOBAL_CFLAGS) -O3 -Wno-unused-parameter \
				-Wno-implicit-fallthrough -Wno-type-limits  \
				-Wno-unused-variable -Wno-pointer-sign 		\
				-Wl,--strip-all -Os -s -ffunction-sections 	\
				-Wl,--gc-sections \
				-fno-asynchronous-unwind-tables
				
ARR_FLAGS     = -rc
OBJECTS = 	GetCommandOutputC.o