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

    GetStdoutFromCommand(&output, "cmd /c \"if \"%ComputerName%\"==\"S1\" (echo SharedPath=//blah/blah) else (echo SharedPath=//some/path) & for %f in (*.txt) do echo Archivo: %f\" 2>&1");
    printf("%s\n", output.data);
    free_string_cmd(&output); /* es necesario librerar
    la memoria cuando ya no se vaya a seguir ejecutando
    comandos.
    */

    return 0;
}