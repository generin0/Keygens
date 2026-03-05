/*https://crackmes.one/crackme/69a3f47818b5d7ee47093563*/
#include <stdio.h>
#include <string.h>
#include <stdint.h>

static const uint8_t magic[16] = {
    0x20, 0x1B, 0x08, 0x4C, 0x5F, 0x7F, 0x32, 0x06,
    0x58, 0x1A, 0x58, 0x45, 0x59, 0x52, 0x52
};

static void xor(uint8_t *output, const char *username, size_t length) {
    size_t ulen = strlen(username);
    size_t mod  = ulen + 1;
    for (size_t i = 0; i < length; i++) {
        output[i] = (uint8_t)username[i % mod] ^ magic[i];
    }
    output[length] = 0;
}

static void encode(char *out, const uint8_t *in, size_t len) {
    for (size_t i = 0; i < len; i++) {
        uint8_t b = in[i];
        if (b & 1) {
            out[i] = 'K';
        } else if (b % 3 == 0) {
            if (b & 3)
                out[i] = 'f';
            else
                out[i] = 'V';
        } else {
            out[i] = 'b';
        }
    }
    out[len] = '\0';
}

int main(void) {
    char username[17] = {0};
    uint8_t xored[17] = {0};
    char password[128] = {0};

    printf("Enter username: ");
    if (!fgets(username, sizeof(username), stdin)) return 1;

    size_t ulen = strlen(username);
    if (ulen > 0 && username[ulen - 1] == '\n') username[--ulen] = '\0';

    if (ulen == 0) {
        return 1;
    }
    if (ulen > 16) {
        return 1;
    }

    xor(xored, username, 16);

    size_t xlen = strlen((char *)xored);

    encode(password, xored, xlen);

    printf("Username : %s\n", username);
    printf("Password : %s\n", password);

    return 0;
}
