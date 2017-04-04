## CRACKER SECUENCIAL
``` shell
# Compilación
$ gcc crack.c -lssl -lcrypto -o crack

# Ejecución
$ ./crack -m MIN_LENGTH -M MAX_LENGTH -a ALPHABET -H HASH

# Por ejemplo
$ ./crack -m 1 -M 4 -a "0123456789" -H "6ad275d26c200e81534d9996183c8748ddfabc7b0a011a90f46301626d709923474703cacab0ff8b67cd846b6cb55b23a39b03fbdfb5218eec3373cf7010a166"
```

## CRACKER PARALELIZADO
``` shell
# Compilación
$ gcc crack-openmp.c -lssl -lcrypto -fopenmp -o crack-openmp

# Ejecución
$ ./crack-openmp -m MIN_LENGTH -M MAX_LENGTH -a ALPHABET -p PROCESSES -H HASH

# Por ejemplo
$ ./crack-openmp  -m 1 -M 4 -a "0123456789" -p 3 -H "6ad275d26c200e81534d9996183c8748ddfabc7b0a011a90f46301626d709923474703cacab0ff8b67cd846b6cb55b23a39b03fbdfb5218eec3373cf7010a166"
```

## ESTADÍSTICAS DE RENDIMIENTO
Los resultados en las pruebas con más procesos se obtienen antes que en la secuencial, pero es necesario esperar a que
terminen los procesos para finalizar el programa. Por eso los resultados con más procesos tardan más tiempo. Se han ejecutado
sobre una máquina virtual, no pudiendo configurar más de dos núcleos; por lo que los resultados de 3 y 4 núcleos están
sujetos a la eficiencia del sistema. Según se va aumentando la longitud del problema, los procesos se hacen notar más,
en las pruebas a partir de longitud 5 se notaba que desde el cracker secuencial 5 procesos la mejora era del doble. Es
decir, tarda la mitad respecto la ejecución en secuencial.

### HASH 1
fd48579f6f8ee8a78dcbbd53bda1c24f6bb310ecf5f48bc468e78d32fa6cd44ceb0753cb4f611c28e91f49f451eefcbd1d90caf1f940486c8274d1d151a8a8ca
Longitud: (2,4).
Alfabeto: abcdefghijklmnopqrsuvwxyz

#### Cracker secuencial
Tiempo min: 0.061
Tiempo max: 0.111
Tiempo medio: 0.077

#### Cracker OpenMP 2 procesos
Tiempo min: 0.093
Tiempo max: 0.125
Tiempo medio: 0.109

#### Cracker OpenMP 3 procesos
Tiempo min: 0.079
Tiempo max: 0.221
Tiempo medio: 0.123

#### Cracker OpenMP 4 procesos
Tiempo min: 0.085
Tiempo max: 0.135
Tiempo medio: 0.106

### HASH 2:
7e0c36816c25d3350e16b1332984ac7550123261503d0a9c8c6c6e43424b343f5e71faf2c82229538c933d2ca04543f0f6a5f8d4b57028d40685e1596ee78c32
Longitud: (2, 4).
Alfabeto: abcdefghijklmnopqrsuvwxyz01234567890

#### Cracker secuencial
Tiempo min: 0.213
Tiempo max: 0.247
Tiempo medio: 0.225

#### Cracker OpenMP 2 procesos
Tiempo min: 0.235
Tiempo max: 0.277
Tiempo medio: 0.256

#### Cracker OpenMP 3 procesos
Tiempo min: 0.24
Tiempo max: 0.451
Tiempo medio: 0.275

#### Cracker OpenMP 4 procesos
Tiempo min: 0.241
Tiempo max: 0.282
Tiempo medio: 0.258
