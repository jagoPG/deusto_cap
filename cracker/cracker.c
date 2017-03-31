#include <stdio.h>
#include <omp.h>
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
int get_key(int n, int size, unsigned char *alpha, unsigned char *candidato)
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

int get_corrected_key(int n, int min, int max, unsigned char *alpha, unsigned char *candidato, int *size)
{
    int i;
    int limit;

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

long long int calculate_key_space(int min, int max, int alpha_length)
{
    long long int total = 1;

    while (min <= max) {
        total += power(alpha_length, min);
        min++;
    }

    return total;
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
    int opt;
    unsigned char secretHashed[SHA512_DIGEST_LENGTH*2] = "ddaf35a193617abacc417349ae20413112e6fa4e89a97ea20a9eeee64b55d39a2192992a274fc1a836ba3c23a3feebbd454d4423643ce80e2a9ac94fa54ca49f";
    int min = 2;
    int max = 4;
    unsigned char alpha[] = "abcdefghijklmnopqrstuvwxyz";

    while ((opt = getopt(argc, argv, "m:M:a:h:")) != -1) {
        switch (opt) {
            case 'm':
                min = atoi(optarg);
                break;
            case 'M':
                max = atoi(optarg);
                break;
            case 'a':
                sprintf(alpha, "%s", optarg);
                break;
            case 'h':
                sprintf(secretHashed, "%s", optarg);
                break;
            default:
                printf("./cracker -m MIN -M MAX -a ALPHABET -h HASH\n");
        }
    }
    printf("%d %d %s %s\n", min, max ,alpha, secretHashed);

    long long int lenkeyspace =  calculate_key_space(min, max, strlen(alpha));
    int i;
    int corrected_index;
    int size;
    int found = 0;
    unsigned char hash[SHA512_DIGEST_LENGTH*2];
    unsigned char *candidate;

    printf("Buscar: %s\n", secretHashed);

    #pragma omp parallel for private(corrected_index, candidate, size, hash) shared(found)
    for (i = 0; i < lenkeyspace; i++) {
        if (found) {
            continue;
        }

        // Obtener el tamaño de la clave
        corrected_index = get_corrected_key(i, min, max, alpha, candidate, &size);

        if (corrected_index == -1) {
            printf("La clave es demasiado grande, aumente la longitud máxima permitida.\n");
        } else {
            // Obtener el candidato y hashearlo
            candidate = (char *) calloc(size, sizeof(char));
            get_key(corrected_index, size, alpha, candidate);
            hashing(candidate, hash);

            // Comprobar si se ha encontrado la clave
            if (!strncmp(hash, secretHashed, SHA512_DIGEST_LENGTH * 2)) {
                found = 1;
                printf("Encontrado, %s = %s", candidate, hash);
            }
            printf("%s ", candidate);
            free(candidate);
        }
    }

    return 0;
}
