#include <openssl/sha.h>
#include <stdio.h>

int main (int argc, char** argv)
{
    if (argc < 2 || argc > 2) {
        printf("Error not enough arguments passed.\n");
        return 1;
    }
    char* data = argv[1];
    unsigned char encodedStr[SHA512_DIGEST_LENGTH];
    unsigned char encoded[SHA512_DIGEST_LENGTH];
    int i = 0;

    SHA512(data, sizeof(data), encoded);
    for (; i < SHA512_DIGEST_LENGTH; ++i) {
        sprintf(&encodedStr[i], "%0x", encoded[i]);
    }
    printf("%s\n", encodedStr);
    
    return 0;
}
