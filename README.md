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

```bash
python compile_bat.py --bat-name example.bat --show-commands
```

----