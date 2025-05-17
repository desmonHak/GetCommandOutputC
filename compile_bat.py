import argparse
import re

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

    if args.show_commands:
        print("\nComando en una línea para cmd:")
        print(full_cmd)


if __name__ == "__main__":
    main()
