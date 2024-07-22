# Proyecto de Compiladores: Analizador Léxico y Sintáctico

## Descripción

Este proyecto consiste en la creación de un analizador léxico y sintáctico, junto con un graficador, para desarrollar una herramienta de aprendizaje en C. Se enfoca en la sintaxis Arden.

## Configuración del Entorno de Desarrollo

Para configurar el entorno de desarrollo en Linux, asegúrate de tener las siguientes bibliotecas y herramientas instaladas:

- GTK 3.0
- GLib 2.0
- Pango 1.0
- HarfBuzz
- Cairo
- Pixman
- GDK Pixbuf 2.0
- ATK 1.0

## Configuraciones del Compilador

A continuación se presentan las configuraciones utilizadas para el compilador:

```json
{
    "configurations": [
        {
            "name": "Linux",
            "includePath": [
                "${workspaceFolder}/**",
                "/usr/include/gtk-3.0",
                "/usr/include/glib-2.0",
                "/usr/lib/x86_64-linux-gnu/glib-2.0/include",
                "/usr/include/pango-1.0",
                "/usr/include/harfbuzz",
                "/usr/include/cairo",
                "/usr/include/pixman-1",
                "/usr/include/gdk-pixbuf-2.0",
                "/usr/include/atk-1.0"
            ],
            "defines": [],
            "compilerPath": "/usr/bin/gcc",
            "cStandard": "c17",
            "cppStandard": "gnu++17",
            "intelliSenseMode": "linux-gcc-x64"
        }
    ],
    "version": 4
}
