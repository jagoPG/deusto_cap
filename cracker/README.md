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
### Cracker secuencial
Tiempo min:
Tiempo max:
Tiempo medio:

### Cracker OpenMP 2 procesos
Tiempo min:
Tiempo max:
Tiempo medio:

### Cracker OpenMP 3 procesos
Tiempo min:
Tiempo max:
Tiempo medio:

### Cracker OpenMP 4 procesos
Tiempo min:
Tiempo max:
Tiempo medio:
