Software TFG
============

En este directorio se encuentra todo el código fuente de la implementación del algoritmo AKS como de los tests probabilísticos, además del código para generar las gráficas utilizadas en la memoria.

Para compilar el proyecto serán necesarios los siguientes programas. Versiones inferiores que las descritas pueden funcionar:

- GCC 11.1.0
- CMake 3.21.3
- Conan 1.41.0
- NTL 6.2.1
- (Opcional) Ccache
- (Opcional) CppCheck
- (Opcional) Clang-Tidy
- (Opcional) Gnuplot

Para compilar el proyecto, simplemente ejecutar los siguientes comandos en la terminal dentro del directorio Software:

```console
$> mkdir build
$> cd build
$> cmake ..
$> make
```

Algunas opciones que se le pueden pasar a CMake son las siguiente:

- ENABLE_LTO: Activar las optimizaciones en tiempo de enlazado. Por defecto es ON.
- ENABLE_CPPCHECK: Activa el análisis estático con CppCheck. Por defecto es OFF.
- ENABLE_CLANG_TIDY: Activa el análisis estático con Clang-Tidy. Por defecto es OFF.
- BUILD_EXAMPLES: Compila los programas de ejemplo. Por defecto es OFF.
- BUILD_TEST: Compilas los programas para testear. Por defecto es OFF.

El software no tiene un comando para ser instalado.
