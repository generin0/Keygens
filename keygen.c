// https://crackmes.one/crackme/691f53d32d267f28f69b7f62
#include <string.h>
#include <stdint.h>
#include <stdio.h>

uint32_t ROL4(uint32_t value, int shift) {
    shift &= 0x1F;
    return (value << shift) | (value >> (32 - shift));
}

int main(void) {
    char name[32];
    uint32_t stateA, stateB;
    uint32_t result;
    int counter;
    int negative_length;
    
    while (1) {
        printf("Enter name: ");
        fgets(name, sizeof(name), stdin);
        
        int length = strlen(name);
        if (length > 0 && name[length - 1] == '\n') { // remove newline character
            name[length - 1] = '\0';
            length--;
        }
        
        if (name != 0) { // check for first character
            negative_length = -length;
            stateA = 0xDEADC0DE;
            stateB = 0x55555555;

            for (counter = 0; counter < length && name[counter] != 0; counter++) { // main hash loop
                uint8_t current_char = name[counter] & 0xFFFFFFFF;
                
                if ((current_char & 1) == 0) {
                    stateB = ROL4(stateB, 12) & 0xFFFFFFFF;
                    stateB ^= current_char & 0xFFFFFFFF;
                    stateB += 0x90F01234 & 0xFFFFFFFF;
                } else {
                    stateB = ROL4(stateB, 29) & 0xFFFFFFFF;
                    stateB += current_char & 0xFFFFFFFF;
                    stateB += 0xE5D4C3B3 & 0xFFFFFFFF;
                }
                
                uint32_t temp = stateA + counter & 0xFFFFFFFF;

                stateA = (temp + negative_length) ^ temp & 0xFFFFFFFF;
                stateB ^= stateA & 0xFFFFFFFF;
            }

        } else {
            stateB = 0x55555555;
            stateA = 0xDEADC0DE;
            counter = 0;
        }

        result = ((255 * counter * counter) ^ (stateA + __ROL4__(stateB, 29))) & 0xFFFFFFFF;
        
        printf("Key: %08X\n", result);
    }
    return 0;
}
