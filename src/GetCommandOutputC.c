#ifndef GET_COMMAND_OUTPUT_C_C
#define GET_COMMAND_OUTPUT_C_C

#include "GetCommandOutputC.h"

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

    /*
     * el programador no debe liberar la memoria sin poner
     * el miembro data a NULL despues de la liberacion.
     */
    free_string_cmd(output);

    /**
     * @brief tamaño incial de la salida del comando. Deberia ser multiplo de 8
     * en arch de 64bits.
     */
    *output = init_string_cmd(sizeof(size_t) * 2);
    if (output->data == NULL) {
        printf("(GetStdoutFromCommand) calloc error in output->data\n");
        return;
    }

    // añadir " 2>&1" al comando final para obtener el stderr.
    char *cmd_ = (char*)calloc(
        strlen(cmd) + sizeof(" 2>&1") + 1, 
        sizeof(char)
    );
    if (cmd_ == NULL) {
        printf("(GetStdoutFromCommand) calloc error in cmd_\n");
        return;
    }

    // puntero para el stream del comando ejecutado

    // comando final, concatenar el comando con el buffer.:
    memcpy(cmd_, cmd, strlen(cmd));
    memcpy(cmd_ + strlen(cmd), " 2>&1", sizeof(" 2>&1"));

    //printf("command : %s\n", cmd_);

    FILE *stream = popen(cmd_, "r");
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
                char *temp = (char*)realloc(output->data, output->size_buff);
                if (temp == NULL) {
                    printf("(GetStdoutFromCommand) realloc error\n");
                    free(cmd_);
                    return;
                } else output->data = temp;

                
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
            int c = fgetc(stream);
            if (c == EOF) break;
            output->data[output->use_size_buff] = (char)c;

            output->use_size_buff += 1;
        }
        pclose(stream);
    }

    // liberar comando final
    free(cmd_);
}

#endif // GET_COMMAND_OUTPUT_C_C