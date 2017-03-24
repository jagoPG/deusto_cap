#include <stdio.h>
#include <omp.h>
#include <openssl/sha.h>

/**
 * Genera un hash a partir de una clave.
 *
 * @param plain clave en texto plano
 * @param encodedStr hash generado
 */
void hashing(char *plain, char *encodedStr)
{
    unsigned char *data = (unsigned char *) argv[1];
    unsigned char encoded[SHA512_DIGEST_LENGTH];
    int i;

    SHA512(data, strlen(data), encoded);
    for (i = 0; i < SHA512_DIGEST_LENGTH; ++i) {
        sprintf(&encodedStr[i*2], "%02x", encoded[i]);
    }
    printf("%s\n", encodedStr);

    return encodedStr;
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
void getKey(int n, int size, char *alpha, char *candidato)
{
    unsigned char *key[size];
    int len = strlen(alpha);
    int num = n;

    if (n > ipow(strlen(alpha), size)) {
        return '\0';
    } else {
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
}
/**
 * El programa de poder recibir un alfabeto, un tamaño de la clave y el hash
 * que se quiere crackear.
 * @param argc
 * @param argv
 * @return 0 si no ha habido errores.
 */
int main (int argc, char[] *argv)
{
    unsigned int SIZE = 4;
    unsigned char alpha[] =  "abcdefghijklmnopqrstuvwxyz";
    int i;

    // comprobar si los argumentos son correctos
    unsigned char *encodedStr;
    unsigned char *candidato;s

    for (i = 0; i < lenkeyspace; i++) {
        // generar clave candidata y hashearla
        encodedStr = (char*) malloc(SHA512_DIGEST_LENGTH * sizeof(char));
        candidato = (char *) malloc((size + 1) *sizeof(char));
        getKey(i, SIZE, alpha, candidate);
        hash = hashing(candidate);

        // comprobar si se ha encontrado la clave
        if (!strcmp(hash, secret)) {
            printf("Encontrado, %s = %s", candidate, secret);
            break;
        }

        free(encodedStr);
        free(candidato);
    }

    return 0;
}
