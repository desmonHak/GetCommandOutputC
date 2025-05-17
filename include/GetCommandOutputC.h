#ifndef GET_COMMAND_OUTPUT_C_H
#define GET_COMMAND_OUTPUT_C_H

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
void GetStdoutFromCommand(string_cmd *output, char* cmd);

/*
 * el programador no debe liberar la memoria sin poner
 * el miembro data a NULL despues de la liberacion.
 */
static inline void free_string_cmd(string_cmd *output) {
    if (output->data != NULL && output->size_buff != 0) {
        free(output->data);
        *output = (string_cmd){0};
    }
}

#endif // GET_COMMAND_OUTPUT_C_H