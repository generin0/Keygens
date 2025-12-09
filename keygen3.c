// https://crackmes.one/crackme/685d2284aadb6eeafb398ce4
#include <stdio.h>
#include <inttypes.h>
#include <ctype.h>

int64_t hash(char *str) {
    int v2;
    unsigned int i = 0;
    char *v1;
    while ( *str )
    {
        v1 = str++;
        v2 = tolower(*v1);
        i = ((1025 * (v2 + i)) >> 6) ^ (1025 * (v2 + i));
    }
    return 32769 * (((9 * i) >> 11) ^ (9 * i));
}

int main(void) {
    char buf[99];
    int64_t result;
    printf("Enter username: ");
    scanf("%99s", &buf);
    result = hash(buf);
    printf("Hash: %llX\n", result);
    return 0;
}
