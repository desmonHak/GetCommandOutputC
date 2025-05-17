#include "GetCommandOutputC.h"

int main(int argc, char const *argv[])
{   
    string_cmd output = {0};
    
    GetStdoutFromCommand(&output, "dir");
    printf("%s\n", output.data);

    GetStdoutFromCommand(&output, "ipconfig");
    printf("%s\n", output.data);
    free_string_cmd(&output); // es necesario librerar la memoria cuando ya no se vaya a seguir ejecutando comandos.

    return 0;
}
