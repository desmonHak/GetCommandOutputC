# GetCommandOutputC

----

Esta pequeña libreria contiene lo necesario para ejecutar comandos de forma multiplataforma en C/C++ y obtener la salida del comando ejecutado. 
La salida se obtiene en un string dinamico, por lo que el usuario solo debe preocuparse de liberar la memoria al final de su uso, utilizando la funcion `free_string_cmd` para la tarea.

----

Codigo de ejemplo en C:

```c
#include "GetCommandOutputC.h"

int main(int argc, char const *argv[])
{   
    string_cmd output = {0};
    
    GetStdoutFromCommand(&output, "dir");
    printf("%s\n", output.data);

    /* no es necesario liberar la memoria en cada 
    ejecuccion, la funcion `GetStdoutFromCommand` lo 
    hara internamente por nosotros.
    */

    GetStdoutFromCommand(&output, "ipconfig");
    printf("%s\n", output.data);
    free_string_cmd(&output); /* es necesario librerar 
    la memoria cuando ya no se vaya a seguir ejecutando 
    comandos.
    */

    return 0;
}
```

puede compilar el codigo con:
```bash
gcc example/code.c src/GetCommandOutputC.c -Iinclude -o code.exe
```

o usando el make:
```
mingw32-make -f windows.mk
```
```
make -f linux.mk
```
----

# Generar ejecutable desde un .bat

> Para poder generar un ejecutable desde un .bat necesita tener la utilidad ``make`` o `mingw32-make` del compilador C/C++. Puede usar ``mingw32`` o alternativas como ``TDM`` para esto.

> Adicionalmente, se necesita tener python y git instalado.


1. Compilar el proyecto con:
```bash
mingw32-make -f windows.mk
```

si necesita limpiar, puede hacerlo con lo siguiente:
```bash
mingw32-make -f windows.mk cleanall
```

2. generar el codigo fuente con el .bat:

```c
F:\C\simple_bytecode\lib\GetCommandOutputC>python compile_bat.py -h
usage: compile_bat.py [-h] --bat-name BAT_NAME [--lz77] [--show-commands] [--gen-exec] [--hidden-console] [--hidden-output]

Generar un ejecutable desde un .bat.

options:
  -h, --help           show this help message and exit
  --bat-name BAT_NAME  Nombre del archivo .bat a procesar.
  --lz77               Activar compresión LZ77
  --show-commands      Mostrar el comando convertido.
  --gen-exec           generar un ejecutable con el bat.
  --hidden-console     generar un ejecutable con el bat pero que no se muestre la consola.
  --hidden-output      permite indicar que no se quiere ver la salida de la ejecuccion
```
> Si se usa la flag `--lz77` se activara la compresion LZ77, esto nos permite comprimir los comandos en tiempo de compilacion y poder ahorrar algo de espacio y realizar la descompresion en tiempop de ejecucion.

Generar codigo de ejecutable, mostrando los comandos ejecutados
```bash
python compile_bat.py --bat-name example.bat --show-commands
```

Generar ejecutable sin mostrar consola
```bash
python compile_bat.py --bat-name example.bat --show-commands  --hidden-console
```

3. Al generar el codigo fuente de C, se generar varios archivos mas con este, se debera usar el nuevo `.mk` generado. Si su .bat es `example.bat`, el archivo .mk generado se llamara `example_inline.mk`, debera usar `mingw32-make -f example_inline.mk` para compilarlo:
```c
F:\C\simple_bytecode\lib\GetCommandOutputC>python compile_bat.py --bat-name example.bat --show-commands  --lz77
hidden_console: False
Nombre del .bat: example.bat
Compresión LZ77 activada: True
Mostrar comandos: True

Comando en una línea para cmd:
cmd /c "@echo off & color 0A & ipconfig & if "%ComputerName%"=="S1" (echo SharedPath=//blah/blah) else (echo SharedPath=//some/path) & for %f in (*.txt) do (echo Archivo: %f) & (echo Inicio bloque extra & dir /b & echo Fin bloque extra)"
".\lib\SimpleOfuscator\code2.exe" no se reconoce como un comando interno o externo,
programa o archivo por lotes ejecutable.
#define COMPESS_LZ77
#define SHOW_COMMANDS_EXEC

#include <stdint.h>

#include "GetCommandOutputC.h"

#include "SimpleOfuscator.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#ifdef _WIN32
#   include <windows.h>
#   define _WIN32_WINNT 0x0500
#endif

#define ORG_SIZE 237
#define PASSWORD "1234"
#if defined(_WIN32) && defined(HIDDEN_CMD)
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#else
int main(int argc, char const *argv[])
#endif
 {
#define ARR(...) { __VA_ARGS__ }
    #ifdef _WIN32
    #   ifdef HIDDEN_CMD
            HWND hWnd = GetConsoleWindow();
            ShowWindow( hWnd, SW_MINIMIZE );
            ShowWindow( hWnd, SW_HIDE );
    #   endif
    #endif

    string_cmd output = ARR(0);

    #ifdef COMPESS_LZ77
        uint8_t shellcode[] = ARR(0x7c, 0x04, 0xa9, 0xa1, 0xc1, 0x07, 0xbb, 0x77, 0xf1, 0xde, 0xb9, 0x1b, 0xe2, 0x2e, 0x08, 0x8d, 0x77, 0xec, 0xde, 0xc9, 0xb8, 0x0f, 0x06, 0x80, 0x0f, 0x1a, 0xf5, 0x76, 0xc8, 0x80, 0x71, 0xa3, 0x81, 0x0f, 0x1a, 0x38, 0x6a, 0xd4, 0x70, 0xb1, 0x43, 0xc7, 0xbb, 0x7d, 0x34, 0xd8, 0xdd, 0xa0, 0x31, 0x44, 0x46, 0x0f, 0x1c, 0xf1, 0x1e, 0xf8, 0xa0, 0xd7, 0x63, 0x47, 0x8c, 0x1d, 0x3f, 0x62, 0x84, 0x68, 0x21, 0x63, 0xc6, 0x0c, 0x0e, 0x0a, 0xdc, 0x09, 0x49, 0xf0, 0xce, 0x5d, 0xbf, 0x1d, 0x30, 0x7a, 0xcc, 0xe0, 0x01, 0xc2, 0x9d, 0x90, 0x7a, 0x32, 0xe8, 0xd1, 0x93, 0x11, 0x24, 0x06, 0x90, 0x78, 0xfc, 0x6e, 0xb8, 0xd3, 0x51, 0xc3, 0x9d, 0x3d, 0x7d, 0x2e, 0xe0, 0x19, 0xa9, 0x27, 0x43, 0x86, 0x0c, 0x76, 0xed, 0xfc, 0xcd, 0xa0, 0x51, 0xe3, 0x81, 0x3b, 0x1f, 0x35, 0x7c, 0xd8, 0x10, 0x12, 0xa3, 0x06, 0x82, 0x78, 0x35, 0x64, 0xdc, 0x93, 0x07, 0x24, 0x88, 0xbd, 0x17, 0xfc, 0xde, 0xf5, 0xa8, 0x50, 0xe3, 0xc7, 0x8d, 0x1a, 0xed, 0x6a, 0xb8, 0xc3, 0xd1, 0xae, 0xde, 0x8f, 0x18, 0x36, 0xde, 0xcd, 0x00, 0x72, 0xcf, 0xc7, 0x0b, 0x79, 0x41, 0x86, 0xc8, 0xf0, 0xc1, 0x03, 0x46, 0x3f, 0x76, 0x30, 0xe2, 0xb9, 0x7b, 0xe1, 0x40, 0x44, 0x0f, 0x09, 0x36, 0xee, 0xf1, 0x70, 0xd1, 0xce, 0x47, 0x8f, 0x20, 0xed, 0x60, 0xb8, 0x23, 0x22, 0xe3, 0x03, 0x90, 0x7a, 0xed, 0x18, 0xec, 0x98, 0x01, 0x13, 0x10, 0x81, 0x17, 0x31, 0xda, 0x2d, 0x10, 0x82, 0x0f, 0xc6, 0xbf, 0x22, 0x42, 0x78, 0xcc, 0x68, 0x47, 0x63, 0x9e, 0x3b, 0x1b, 0xef, 0x72, 0x00, 0x79, 0x11, 0xa4, 0xdf, 0x0b, 0x19, 0x30, 0xda, 0xf5, 0x6b, 0xe7, 0x8f, 0xc7, 0x8b, 0x76, 0x3c, 0x68, 0xe8, 0xf0, 0x01, 0xc3, 0x86, 0x3b, 0x19, 0xef, 0xe4, 0xcd, 0x70, 0xf9, 0x00, 0x00,);
        size_t size_out = 0;
        string_cmd command;

        desofuc(shellcode, sizeof(shellcode), PASSWORD, strlen(PASSWORD), (uint8_t**)&command.data, &command.size_buff);

        command.data = (char*)realloc(command.data, command.size_buff);
        command.data[command.size_buff] = 0;

    #   ifdef SHOW_COMMANDS_EXEC
        printf("Commands: %s\n", command.data);
    #   endif

        GetStdoutFromCommand(&output, command.data);
    #else
        char* command = "@echo off & color 0A & ipconfig & if \"%ComputerName%\"==\"S1\" (echo SharedPath=//blah/blah) else (echo SharedPath=//some/path) & for %f in (*.txt) do (echo Archivo: %f) & (echo Inicio bloque extra & dir /b & echo Fin bloque extra)";

    #   ifdef SHOW_COMMANDS_EXEC
        printf("Commands: %s\n", command);
    #   endif

        GetStdoutFromCommand(&output, command);
    #endif

    #ifndef HIDDEN_OUTPUT_COMMANDS
        printf("%s\n", output.data);
    #endif

    return 0;
}
Archivo destino: example_inline.mk
Contenido modificado: 'include config.mk\n\nall: $(TARGET).a libSimpleOfuscator.a\n\tar -t $^\n\tgcc $(CFLAGS) $(INCLUDE_FLAGS) example_inline.c $(LINKER_FLAGS) -o example_inline.exe\n\n$(TARGET).a: $(OBJECTS)\n\t$(ARR) $(ARR_FLAGS) $@ $^\n\tranlib $@\n\nlibSimpleOfuscator.a:\n\t$(MAKE) -C ./$(PATH_SimpleOfuscator) -f windows.mk\n\nGetCommandOutputC.o: $(PATH_SRC)/GetCommandOutputC.c\n\t$(CC) $(CFLAGS) -c $^ -o $@\n\ncleanobj:\n\t$(RM) $(RMFLAGS) $(OBJECTS)\n\t$(MAKE) -C ./$(PATH_SimpleOfuscator) -f windows.mk cleanobj\n\ncleanall: cleanobj\n\t$(RM) $(RMFLAGS) *.o $(TARGET).a \\\n\t$(TARGET_DEBUG).a *.exe \n\t$(MAKE) -C ./$(PATH_SimpleOfuscator) -f windows.mk cleanall\n\n.SILENT: clean cleanobj cleanall\n.IGNORE: cleanobj cleanall\n.PHONY:  cleanobj cleanall'
Use: mingw32-make -f example_inline.mk

F:\C\simple_bytecode\lib\GetCommandOutputC>

```

----

Compilacion final
```c
F:\C\simple_bytecode\lib\GetCommandOutputC>mingw32-make -f example_inline.mk
mingw32-make -C ././lib/SimpleOfuscator -f windows.mk
mingw32-make[1]: Entering directory 'F:/C/simple_bytecode/lib/GetCommandOutputC/lib/SimpleOfuscator'
mingw32-make -C ././LZ77_c -f windows.mk
mingw32-make[2]: Entering directory 'F:/C/simple_bytecode/lib/GetCommandOutputC/lib/SimpleOfuscator/LZ77_c'
ar -t libLZ77_c.a
lz77.o
gcc -std=c11 -I. -Iinclude -masm=intel -D_ExceptionHandler -fdiagnostics-color=always  -O3 -Wno-unused-parameter -Wno-implicit-fallthrough -Wno-type-limits -Wno-unused-variable -Wno-pointer-sign -I. -Iinclude example/code.c -L. -lLZ77_c -o code.exe
mingw32-make[2]: Leaving directory 'F:/C/simple_bytecode/lib/GetCommandOutputC/lib/SimpleOfuscator/LZ77_c'
ar -t libSimpleOfuscator.a libLZ77_c.a
no entry libLZ77_c.a in archive
gcc -std=c11 -I. -Iinclude -I./LZ77_c/include -masm=intel -D_ExceptionHandler -fdiagnostics-color=always  -O3 -Wno-unused-parameter -Wno-implicit-fallthrough -Wno-type-limits -Wno-unused-variable -Wno-pointer-sign -I. -Iinclude -I./LZ77_c/include example/code2.c -L.                                                   -lSimpleOfuscator -L./LZ77_c                     -lLZ77_c  -o code2.exe
mingw32-make[1]: Leaving directory 'F:/C/simple_bytecode/lib/GetCommandOutputC/lib/SimpleOfuscator'
ar -t .a libSimpleOfuscator.a
no entry libSimpleOfuscator.a in archive
gcc -std=c11 -I. -Iinclude -I./lib/SimpleOfuscator/include -I./lib/SimpleOfuscator/LZ77_c/include -masm=intel -D_ExceptionHandler -fdiagnostics-color=always  -O3 -Wno-unused-parameter -Wno-implicit-fallthrough -Wno-type-limits -Wno-unused-variable -Wno-pointer-sign -Wl,--strip-all -Os -s -ffunction-sections -Wl,--gc-sections -fno-asynchronous-unwind-tables -I. -Iinclude -I./lib/SimpleOfuscator/include -I./lib/SimpleOfuscator/LZ77_c/include example_inline.c -L.            -lGetCommandOutputC -L./lib/SimpleOfuscator      -lSimpleOfuscator -L./lib/SimpleOfuscator/LZ77_c                        -lLZ77_c  -o example_inline.exe
example_inline.c:14: warning: "_WIN32_WINNT" redefined
   14 | #   define _WIN32_WINNT 0x0500
      |
In file included from C:/TDM-GCC-64/x86_64-w64-mingw32/include/corecrt.h:10,
                 from C:/TDM-GCC-64/x86_64-w64-mingw32/include/crtdefs.h:10,
                 from C:/TDM-GCC-64/x86_64-w64-mingw32/include/stdint.h:28,
                 from C:/TDM-GCC-64/lib/gcc/x86_64-w64-mingw32/10.3.0/include/stdint.h:9,
                 from example_inline.c:4:
C:/TDM-GCC-64/x86_64-w64-mingw32/include/_mingw.h:233: note: this is the location of the previous definition
  233 | #define _WIN32_WINNT 0x502
      |

F:\C\simple_bytecode\lib\GetCommandOutputC>
```

----