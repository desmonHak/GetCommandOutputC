@echo off
rem Cambiar color de la consola
color 0A

rem Mostrar información de red
ipconfig

rem Condicional con variables
if "%ComputerName%"=="S1" (
    echo SharedPath=//blah/blah
) else (
    echo SharedPath=//some/path
)

rem Bucle for para listar archivos .txt
for %%f in (*.txt) do (
    echo Archivo: %%f
    rem Aquí podrías hacer más cosas con %%f
)

rem Comando extra para probar salto de línea dentro de bloque
(
    echo Inicio bloque extra
    dir /b
    echo Fin bloque extra
)
