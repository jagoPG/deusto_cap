# Proyectos Computación de Altas Prestaciones
## SHA 512 Hasher
``` shell
# Instalar dependencias
$ sudo apt-get install libssl-dev

# Compilación
$ cd hasher
$ gcc sha1.c -lssl -lcrypto  -o hasher

# Ejecución
$ ./hasher "Hola mundo"
```
