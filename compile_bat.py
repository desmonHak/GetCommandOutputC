from os.path        import join
from platform       import system
from subprocess     import getoutput

import argparse
import re

def batch_to_oneline(batch_script: str) -> str:
    """
    Convierte un script batch multilinea a un único comando línea para cmd /c,
    respetando bloques condicionales y separando comandos con & incluso dentro de paréntesis.
    Además ignora comentarios rem y ::.
    Evita poner & justo después de '(' o justo antes de ')'.
    """
    lines = [line.rstrip() for line in batch_script.splitlines()]
    
    # Filtrar comentarios y líneas vacías
    filtered_lines = []
    for line in lines:
        stripped = line.strip()
        if not stripped or stripped.lower().startswith('rem') or stripped.startswith('::'):
            continue
        filtered_lines.append(line)

    result = []
    block = []
    inside_parens = 0

    for line in filtered_lines:
        inside_parens += line.count('(') - line.count(')')

        block.append(line.strip())

        if inside_parens == 0:
            # Aquí unimos las líneas con & sin poner & justo al principio ni al final
            # Simplemente join normal
            combined_block = " & ".join(block)
            
            # Corregir que no haya & justo después de ( o antes de )
            # Reemplazar "( & " por "(" y " & )" por ")"
            combined_block = combined_block.replace("( & ", "(").replace(" & )", ")")

            result.append(combined_block)
            block = []

    if block:
        combined_block = " & ".join(block)
        combined_block = combined_block.replace("( & ", "(").replace(" & )", ")")
        result.append(combined_block)

    joined = " & ".join(result)

    # Cambiar %% por % en variables for
    joined = re.sub(r'%%(\w)', r'%\1', joined)

    return joined

class SimpleOfuscatorC():

    def __init__(self) -> None:

        self.root = join(".", "lib", "SimpleOfuscator", "code2" + [".elf", ".exe"][system() == "Windows"])

        """
            F:/C/simple_bytecode/lib/GetCommandOutputC/lib/SimpleOfuscator>code2.exe
            Uso:
            code2.exe o <archivo_entrada> <archivo_salida> <password>
            code2.exe d <archivo_entrada> <archivo_salida> <password>
        """

    def compress(self, file_in, file_out, password):
        out = getoutput(self.root + " o {} {} {}".format(file_in, file_out, password))
        code_compress = None
        with open(file_out, "rb") as f:
            code_compress = f.read()

        return [out, code_compress]

    def descompress(self, file_in, file_out, password):
        out = getoutput(self.root + " d {} {} {}".format(file_in, file_out, password))
        code_descompress = None
        with open(file_out, "rb") as f:
            code_descompress = f.read()

        return [out, code_descompress]

def main():
    parser = argparse.ArgumentParser(description="Procesar archivo .bat y convertirlo a comando de una línea para cmd /c.")

    parser.add_argument('--bat-name', type=str, required=True,
                        help='Nombre del archivo .bat a procesar.')

    parser.add_argument('--lz77', action='store_true',
                        help='Activar compresión LZ77 (no usado aquí, solo ejemplo).')

    parser.add_argument('--show-commands', action='store_true',
                        help='Mostrar el comando convertido.')

    parser.add_argument('--gen-exec', action='store_true',
                        help='generar un ejecutable con el bat.')
    
    parser.add_argument('--hidden-console', action='store_false',
                    help='generar un ejecutable con el bat pero que no se muestre por consola.')

    args = parser.parse_args()

    print("Nombre del .bat:", args.bat_name)
    print("Compresión LZ77 activada:", args.lz77)
    print("Mostrar comandos:", args.show_commands)

    # Leer el contenido del archivo .bat
    with open(args.bat_name, 'r', encoding='utf-8') as f:
        batch_code = f.read()

    # Convertir a comando en una línea
    oneline_cmd = batch_to_oneline(batch_code)

    # Preparar comando completo para cmd /c
    full_cmd = f'cmd /c "{oneline_cmd}"'

    # tamaño original del codigo batch file
    size_org_bath_file = len(full_cmd)

    # almacenar el codigo comprimido
    args.bat_name = args.bat_name.replace(".", "_inline.")
    with open(args.bat_name, "w") as f:
        f.write(full_cmd)

    if args.show_commands:
        print("\nComando en una línea para cmd:")
        print(full_cmd)

    # Instancias la clase para ofuscar
    ofus = SimpleOfuscatorC()
    f_output = args.bat_name.replace(".", "_compress.")

    password = "1234"

    # en el indice 0 la salida de la compresion, en el 1 los datos generados
    output = ofus.compress(args.bat_name, f_output, password)
    print(output[0])

    # preparar los datos para generar el codigo C de saldia
    shellcode = []
    for byte in output[1]:
        shellcode.append(f"0x{byte:02x},")


    data_output =   f"""
#include <stdint.h>

#include "GetCommandOutputC.h"

#include "SimpleOfuscator.h"

#define ORG_SIZE {size_org_bath_file}
#define PASSWORD "{password}"
int main(int argc, char const *argv[])"""+" {\n#define ARR(...) { __VA_ARGS__ }" + f"""
    string_cmd output = ARR(0);
    

    uint8_t shellcode[] = ARR({" ".join(shellcode)});
    size_t size_out = 0;
    string_cmd command;

    desofuc(shellcode, sizeof(shellcode), PASSWORD, strlen(PASSWORD), (uint8_t**)&command.data, &command.size_buff);

    command.data = (char*)realloc(command.data, command.size_buff);
    command.data[command.size_buff] = 0;

    GetStdoutFromCommand(&output, command.data);
    printf("%s\\n", output.data);
    return 0;
""" + "}"
    print(data_output)

    with open(args.bat_name.replace(".bat", ".c"), "w") as f:
        f.write(data_output)

    mk_name = args.bat_name.replace(".bat", ".mk")
    print("Archivo destino:", mk_name)

    with open("general.mk", "r") as file_org:
        replaced = file_org.read().replace("code", args.bat_name.split(".")[0])
        replaced = replaced.replace("$(PATH_EXAMPLES)/", "")
        replaced = replaced.replace(".$(EXTENSION)", [".elf", ".exe"][system() == "Windows"])
        replaced = replaced.replace("$(MAKE_NAME)", ["linux.mk", "windows.mk"][system() == "Windows"])

        
        print("Contenido modificado:", repr(replaced))

        with open(mk_name, "w") as file_finally:
            file_finally.write(replaced)

    print("Use: mingw32-make -f example_inline.mk")




if __name__ == "__main__":
    main()
