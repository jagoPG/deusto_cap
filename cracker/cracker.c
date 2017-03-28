#include <stdio.h>
//#include <omp.h>
#include <openssl/sha.h>
#include <string.h>
#include <stdlib.h>

/**
 * Genera un hash a partir de una clave.
 *
 * @param plain clave en texto plano
 * @param encodedStr hash generado
 */
void hashing(unsigned char *plain, unsigned char *encodedStr)
{
    unsigned char encoded[SHA512_DIGEST_LENGTH];
    int i;

    SHA512(plain, strlen(plain), encoded);
    for (i = 0; i < SHA512_DIGEST_LENGTH; ++i) {
        sprintf(&encodedStr[i*2], "%02x", encoded[i]);
    }
}

/**
 * Calcula un número elevado a una potencia.
 *
 * @param number Número
 * @param power Potencia
 */
long long power(int number, int power)
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
int getKey(int n, int size, unsigned char *alpha, unsigned char *candidato)
{
    int len = strlen(alpha);
    int num = n;

    if (n > power(strlen(alpha), size)) {
        return 0;
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
    }

    return strlen(candidato);
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
    unsigned char *secretHashed = (unsigned char *) malloc(SHA512_DIGEST_LENGTH * sizeof(char));
    unsigned char *secret = "bbbc";
    hashing(secret, secretHashed);

    int size = 4;
    unsigned char alpha[] = "abcdefghijklmnopqrstuvwxyz";
    int lenkeyspace = size * strlen(alpha);
    int i;
    int errcode;

    // !TODO comprobar si los argumentos son correctos
    unsigned char *hash;
    unsigned char *candidate;
    unsigned char *tmp;

    printf("Buscar %s\n", secretHashed);
    for (i = 0; i < lenkeyspace; i++) {
        // generar clave candidata y hashearla
        hash = (char *) malloc(SHA512_DIGEST_LENGTH * sizeof(char));
        candidate = (char *) malloc((size + 1) * sizeof(char));

        getKey(i, size, alpha, candidate);
        hashing(tmp, hash);

        // comprobar si se ha encontrado la clave
        if (!strcmp(hash, secretHashed)) {
            printf("Encontrado, %s = %s", candidate, secret);
            break;
        }

        free(hash);
        free(candidate);
    }
    free(secretHashed);

    return 0;
}
