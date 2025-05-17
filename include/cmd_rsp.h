#ifndef _CMD_RSP_H_
#define _CMD_RSP_H_

// https://codereview.stackexchange.com/questions/162546/send-command-and-get-response-from-windows-cmd-prompt-silently

/**
 * @file cmd_rsp.h
 * @version 0.1
 * @date 2025-05-14
 * 
 * @copyright Copyright (c) 2025
 * 
 */

/**
La necesidad:

En pocas palabras: _popen() para Windows

Necesitaba un método para enviar comandos programáticamente 
al Símbolo del sistema de Windows 7 (y versiones posteriores), 
también conocido como consola CMD, y devolver la respuesta a un 
búfer sin ver una ventana emergente. El diseño debía facilitar la 
implementación en aplicaciones C que necesitaran esta funcionalidad.

El diseño:

El entorno de desarrollo, además del sistema operativo Windows 7, es un 
compilador ANSI C (C99) de National Instruments y el Kit de controladores 
de Microsoft Windows para Windows 8.1. Uno de los objetivos del diseño era 
presentar una API muy pequeña, con instrucciones de uso bien documentadas y 
sencillas. El resultado es una única función exportada, con un prototipo de 
3 argumentos. En su forma original, está diseñada para compilarse como una 
DLL. Los únicos archivos de encabezado que usé en mi entorno fueron windows.h 
y stdlib.h.

Para su revisión:

El código publicado está completo y lo he probado, pero soy nuevo en el uso 
de tuberías a la entrada estándar y la salida estándar, así como en el uso de 
métodos de Windows para CreateProcess(...). Además, dado que los requisitos 
de tamaño del búfer de respuesta no se pueden conocer en tiempo de 
compilación, el código también permite aumentarlo según sea necesario 
durante la ejecución. Por ejemplo, durante las pruebas, leo directorios 
recursivamente usando dir /s desde varias ubicaciones 
(excluyendo el directorio raíz c:\). Por ejemplo:

cd c:\dev && dir /s // Se devuelve un búfer de aproximadamente 1,8
Mbytes en mi sistema.

Me interesa especialmente recibir comentarios sobre lo siguiente:

Creación y uso de tuberías(pipe)
Uso de CreateProcess
Método utilizado para el crecimiento dinámico del búfer de respuesta
*/

#include <windows.h>
#include <stdlib.h>

//Exported Functions

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Prototype:      int cmd_rsp(char *command, char **chunk, size_t chunk_size)  
//
//  Description:    Executes any command that can be executed in a Windows cmd prompt and returns
//                  the response via auto-resizing buffer.
//
//  Inputs:         const char *command - string containing complete command to be sent
//                  char **chunk - initialized pointer to char array to return results
//                  size_t chunk_size - Initial memory chunk_size in bytes char **chunk was initialized to.
//
//  Return:         0 for success
//                 -1 for failure
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int __declspec(dllexport) cmd_rsp(const char *command, char **chunk, unsigned int chunk_size);
#endif //_CMD_RSP_H 