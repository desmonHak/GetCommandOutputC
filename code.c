#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * @brief Estructura para la creacion de un string dinamico
 */
typedef struct string_cmd {
    size_t use_size_buff; /* tamaño del buffer usado. */
    size_t size_buff;     /* tamaño real del buffer.  */
    char* data;           /* buffer a los datos       */
} string_cmd;

/**
 * @brief Permite inicializar una estructura de tipo `string_cmd`
 *      con la memoria de data a 0.
 */
#define init_string_cmd(size_init) (string_cmd){            \
        .use_size_buff = 0,                                 \
        .size_buff = size_init,                             \
        .data = (char*)calloc(size_init, sizeof(char))      \
    };

/**
 * @brief Obtener salida de un commando del cmd o shell
 * 
 * @param output puntero donde almacenar la salida del comando. 
 *      El miembro `data` contiene la salida del comando, que tiene un tamaño
 *      `use_size_buff`, pero que puede albergar hasta una cantidad de 
 *      `size_buff` caracteres
 * @param cmd cadena con el comando a ejecutar.
 */
void GetStdoutFromCommand(string_cmd *output, char* cmd) {
    /**
     * @brief tamaño incial de la salida del comando. Deberia ser multiplo de 8
     * en arch de 64bits.
     */
    *output = init_string_cmd(16); 

    // añadir " 2>&1" al comando final para obtener el stderr.
    char *cmd_ = (char*)calloc(
        strlen(cmd) + sizeof(" 2>&1") + 1, 
        sizeof(char)
    );

    // puntero para el stream del comando ejecutado
    FILE * stream; 

    // comando final, concatenar el comando con el buffer.:
    memcpy(cmd_, cmd, strlen(cmd));
    memcpy(cmd_ + strlen(cmd), " 2>&1", sizeof(" 2>&1"));

    printf("command : %s\n", cmd);

    stream = popen(cmd_, "r");
    if (stream) {

        // mientras no se encuentre el final del stream:
        while (!feof(stream)){
            // si el buffer usado +1(por el carcter nulo) tiene mayor tamaño
            // que el buffer real, redimensionar el buffer real a el nuevo size.
            if (output->use_size_buff  + 1> output->size_buff){
                // almacenar el antiguo tamaño del string
                size_t old_size = output->size_buff;

                // el nuevo buffer tendra el doble de tamaño
                output->size_buff *= 2;
                // reobtener el nuevo buffer.
                output->data = (char*)realloc(output->data, output->size_buff);
                
                /* 
                 * poner la memoria nueva a 0, se suma la direccion base, mas
                 * el antiguo tamaño.
                 * la cantidad de bytes a poner a 0 son:
                 *      (sizeof(nuevo_buffer) - sizeof(antiguo_buffer))
                 * 
                 * memoria antes del realloc:
                 * |-----------------------|
                 * |-----------------------+++++++++++++++++++++++|
                 * 
                 * la antigua memoria(-) contiene parte de la la salida actual.
                 * la nueva memoria(+) contiene valores aleatorios que deben
                 * ponerse a 0 para no causar conflictos en las cadenas 
                 * terminadas en nulo(\0).
                 */
                memset(output->data + old_size, 0, output->size_buff - old_size);
            }
            // almacenar el nuevo caracter y sumar uno al buffer.
            output->data[output->use_size_buff] = fgetc(stream);
            output->use_size_buff += 1;
        }
        pclose(stream);
    }

    // liberar comando final
    free(cmd_);
}
int main(int argc, char const *argv[])
{   
    string_cmd output;
    
    GetStdoutFromCommand(&output, "dir");
    printf("%s\n", output.data);
    free(output.data); // es necesario librerar la memoria en cada ejecucion

    GetStdoutFromCommand(&output, "ipconfig");
    printf("%s\n", output.data);
    free(output.data); // es necesario librerar la memoria en cada ejecucion

    return 0;
}
