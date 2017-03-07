#include <openssl/sha.h>
#include <stdio.h>

int main (int argc, char** argv)
{
    if (argc < 2 || argc > 2) {
        printf("Error not enough arguments passed.\n");
        return 1;
    }
    char* data = argv[1];
    unsigned char hash[SHA512_DIGEST_LENGTH];

    SHA512(data, sizeof(data), hash);

    printf("%s\n", hash);
}
