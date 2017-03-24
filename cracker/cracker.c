#include <stdio.h>
#include <omp.h>
#include <openssl/sha.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

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
    printf("%s\n", encodedStr);
}

/**
 * Convierte un número en una clave compuesta por el alfabeto que se pasa
 * como parámetro.
 *
 * @param n número de clave a ser convertida en clave
 * @param size anchura de la clave
 * @param alpha  alfabeto empleado
 * @param candidato clave candidata generada
 */
int getKey(int n, int size, unsigned char *alpha, unsigned char *candidato)
{
    unsigned char *key[size];
    int len = strlen(alpha);
    int num = n;

    if (n > ipow(strlen(alpha), size)) {
        return 0;
    } else {
        int i = 0;

        // cálculo de n que está en base 10 a strlen(alpha)
        while (num > 0) {
            key[size - i - 1] = alpha[num % len];
            num = num / len;
            i++;
        }

        // rellenar con alpha[0] los huecos que faltan
        while (i < size) {
            key[size - i] = alpha[0];
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
    unsigned char secret[] = "hola";
    unsigned int size = 4;
    unsigned char alpha[] = "abcdefghijklmnopqrstuvwxyz";
    int lenkeyspace = 4 * strlen(alpha);
    int i;

    // comprobar si los argumentos son correctos
    unsigned char *hash;
    unsigned char *candidate;

    for (i = 0; i < lenkeyspace; i++) {
        // generar clave candidata y hashearla
        hash = (char*) malloc(SHA512_DIGEST_LENGTH * sizeof(char));
        candidate = (char *) malloc((size + 1) *sizeof(char));
        getKey(i, size, alpha, candidate);
        hashing(candidate, hash);

        // comprobar si se ha encontrado la clave
        if (!strcmp(hash, secret)) {
            printf("Encontrado, %s = %s", candidate, secret);
            break;
        }

        free(hash);
        free(candidate);
    }

    return 0;
}

