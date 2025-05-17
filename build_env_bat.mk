ifeq ($(OS),Windows_NT)
    OS_NAME := windows
else
    UNAME_S := $(shell uname -s)

    ifeq ($(UNAME_S),Linux)
        OS_NAME := linux
    endif

    ifeq ($(UNAME_S),Darwin)
        OS_NAME := macos
    endif
endif

ifeq ($(OS_NAME),windows)
	CHECK_PROGRAM 	= where $(1) > NUL 2>&1
    DIR				=	dir
    RM				=	del /Q
	PRINT			=	echo 
	END_PRINT		=
	GET_ERROR_CODE	=	%errorlevel%
	SEND_TO_NULL	=	> NUL
	IF				= 	if $(1) ( $(2) )
	END				=
	EQUALS			=	==
	NOT_EQUALS		=	NEQ 
	EXIT_MAKE		= 	exit /b $(1)
	P				= 	\\
	AND_COMMAND		= 	&
	EXTENSION		= 	exe
else
	CHECK_PROGRAM 	= 	command -v $(1) > /dev/null 2>&1
    DIR				=	ls
    RM 				= 	rm -f
	PRINT			=	echo "
	END_PRINT		=	"
	GET_ERROR_CODE	=	$$?
	SEND_TO_NULL	=	>/dev/null 2>&1
	IF				= 	if [ $(1) ]; then $(2) ; fi
	EQUALS			=	-eq
	NOT_EQUALS		=	!=
	EXIT_MAKE		= 	exit $(1)
	P				= 	/
	AND_COMMAND		= 	&
	EXTENSION		= 	elf
endif


# $(call IF, $(GET_ERROR_CODE) $(NOT_EQUALS) 0, $(PRINT)git no esta instalado$(END_PRINT) $(AND_COMMAND) $(call EXIT_MAKE 1))
#
# $(call IF, $(GET_ERROR_CODE) $(NOT_EQUALS) 0
# se llama a IF comprobando si el codigo de error no es igual a 0
#  
# en caso de ser diferente de 0, se ejecuta:
# $(PRINT)git no esta instalado$(END_PRINT); $(call QUIT 1);


SimpleOfuscator = .$(P)lib$(P)SimpleOfuscator

CC = gcc -O2

all:

	@$(PRINT)Sistema operativo detectado: $(OS_NAME) $(END_PRINT)

	@git -h $(SEND_TO_NULL)
	@$(call IF, $(GET_ERROR_CODE) $(NOT_EQUALS) 0, $(PRINT)git no esta instalado. descargelo$(END_PRINT) $(AND_COMMAND) $(call EXIT_MAKE, 1) )

	@$(DIR) $(SimpleOfuscator) $(SEND_TO_NULL)
	@$(call IF, $(GET_ERROR_CODE) $(NOT_EQUALS) 0, git submodule update --init --recursive )
	
	@$(call CHECK_PROGRAM, python) || ($(PRINT)python no esta instalado, descargelo o configure en linux python con \"cp /bin/python3 /bin/python\" $(END_PRINT) $(AND_COMMAND) $(call EXIT_MAKE, 1))

	$(MAKE) -C $(SimpleOfuscator) -f $(OS_NAME).mk 
