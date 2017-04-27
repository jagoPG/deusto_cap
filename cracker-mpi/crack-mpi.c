/*
 * Cracker de claves SHA512 por fuerta bruta paralelizado.
 *
 * @author Jorge Sarabia <georgesarabia@opendeusto.es>
 * @author Jagoba Pérez <jagobaperez92@opendeusto.es>
 */
#include <stdio.h>
#include <mpi.h>
#include <openssl/sha.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * Genera un hash a partir de una clave.
 *
 * @param plain clave en texto plano
 * @param encodedStr hash generado
 */
void hashing(unsigned char plain[], unsigned char encodedStr[])
{
    static unsigned char encoded[SHA512_DIGEST_LENGTH*2];
    int i;

    SHA512(plain, strlen(plain), encoded);
    for (i = 0; i < SHA512_DIGEST_LENGTH; i++) {
        sprintf(&encodedStr[i*2], "%02x", encoded[i]);
    }
}

/**
 * Calcula un número elevado a una potencia.
 *
 * @param number Número
 * @param power Potencia
 */
long long int power(int number, int power)
{
    long long int total = 1;
    while (power > 0) {
        total = number * total;
        power--;
    }

    return total;
}

/**
 * Convierte un número en una clave compuesta por el alfabeto que se pasa
 * como parámetro.
 *
 * @param n número de clave a ser convertida en clave
 * @param size anchura de la clave
 * @param alpha  alfabeto empleado
 * @param candidato clave candidata generada por la función
 */
int get_key(long long int n, long long int size, unsigned char *alpha, unsigned char *candidato)
{
    int len = strlen(alpha);
    int num = n;

    if (n > power(strlen(alpha), size)) {
        return -1;
    } else {
        int i = 0;

        // cálculo de n que está en base 10 a strlen(alpha)
        while (num > 0) {
            candidato[size - i - 1] = alpha[num % len];
            num = num / len;
            i++;
        }

        // rellenar con alpha[0] los huecos que faltan
        while (i < size) {
            candidato[size - i - 1] = alpha[0];
            i++;
        }

        candidato[size] = '\0';
    }

    return strlen(candidato);
}

/**
 * Obtiene el tamaño de una clave a partir de un tamaño mínimo, hasta un tamaño máxmo.
 *
 * @param n Clave candidata indexada
 * @param min Tamaño minimo
 * @param max Tamaño máximo
 * @param alpha Alfabeto
 * @param size Tamaño resultante
 * @return -1 si hay error, clave corregida
 */
int get_corrected_key(long long int n, int min, int max, unsigned char *alpha, int *size)
{
    int i;
    long long int limit;

    for (i = min; i <= max; i++) {
        limit = power(strlen(alpha), i);
        if (n < limit) {
            *size = i;

            return n;
        } else {
            n -= limit;
        }
    }

    return -1;
}

/**
 * Calcular espacio de claves
 * @param min tamaño minimo
 * @param max tamaño máximo
 * @param alpha_length tamaño del alfabeto
 * @return espacio total de claves
 */
long long int calculate_key_space(int min, int max, int alpha_length)
{
    long long int total = 0;

    while (min <= max) {
        total += power(alpha_length, min);
        min++;
    }

    return total;
}

/**
 * Enviar a los nodos hijos la cantidad de claves que tienen que analizar
 *
 * @return El número de claves que tiene que analizar el nodo padre
 */
long long int send_amount_work(long long int keyspace, int amount_processes) {
    int index;
    long long int amount_work = keyspace / amount_processes;

    for (index = 0; index < amount_processes; index++) {
        MPI_Send(&amount_work, 1, MPI_LONG_LONG_INT, index, 0, MPI_COMM_WORLD);
    }

    return amount_work + (keyspace % amount_processes);
}

/**
 * Recibir cantidad de trabajo
 *
 * @return Cantidad de trabajo
 */
int get_amount_work() {
    long long int amount_work;
    MPI_Status status;

    MPI_Recv(&amount_work, 1, MPI_LONG_LONG_INT, 0, 0, MPI_COMM_WORLD, &status);

    return amount_work;
}

/**
 * El programa de poder recibir un alfabeto, un tamaño de la clave y el hash
 * que se quiere crackear.
 * @param argc
 * @param argv
 * @return 0 si no ha habido errores.
 */
int main (int argc, char *argv[])
{
    unsigned char secretHashed[SHA512_DIGEST_LENGTH*2];
    int min, max, rank, opt, size, found = 0;
    unsigned char *alpha;

    while ((opt = getopt(argc, argv, "m:M:a:H:")) != -1) {
        switch (opt) {
            case 'm':
                min = atoi(optarg);
                break;
            case 'M':
                max = atoi(optarg);
                break;
            case 'a':
                alpha = (char *) calloc(strlen(optarg), sizeof(char));
                sprintf(alpha, "%s", optarg);
                break;
            case 'H':
                sprintf(secretHashed, "%s", optarg);
                break;
            default:
                printf("./crack-openmp -m MIN -M MAX -a ALPHABET -H HASH -p PROCESSES\n");
                return 0;
        }
    }

    long long int lenkeyspace =  calculate_key_space(min, max, strlen(alpha));
    long long int i;
    long long int corrected_index;
    unsigned char hash[SHA512_DIGEST_LENGTH*2];
    unsigned char *candidate;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Repartir la cantidad de trabajo entre el padre y los hijos
    long long int amount_work;
    long long int index;
    long long int begining;
    int tmp;

    if (rank == 0) {
        amount_work = send_amount_work(lenkeyspace, size);
    } else {
        amount_work = get_amount_work();
    }
    begining = (amount_work * (size - rank - 1));

    // Búsqueda de la clave
    for (i = 0; i < lenkeyspace && !found; i++) {
        // Calcular la clave a procesar
        index = i + begining;

        // Obtener el tamaño de la clave
        corrected_index = get_corrected_key(index, min, max, alpha, &size);

        if (corrected_index != -1) {
            // Obtener el candidato y hashearlo
            candidate = (char *) calloc(size, sizeof(char));
            get_key(corrected_index, size, alpha, candidate);
            hashing(candidate, hash);

            // Comprobar si se ha encontrado la clave
            if (!strncmp(hash, secretHashed, SHA512_DIGEST_LENGTH * 2)) {
                found = 1;
                printf("%s = hash(\"%s\")\n", hash, candidate);
            }
            free(candidate);
        }

        // Comprobar algún otro nodo ha encontrado la clave
        for (tmp = 0; tmp < size; tmp++) {
            if (tmp != rank) {
                MPI_Bcast(&found, 1, MPI_INT, 3, MPI_COMM_WORLD);
                if (found) {
                    break;
                }
            }
        }
    }
    if (!found) {
        printf("No se ha encontrado la clave, pruebe con otro alfabeto u otro rango de longitud.\n");
    }
    MPI_Finalize();


    return 0;
}
