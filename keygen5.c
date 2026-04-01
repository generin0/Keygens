/* https://crackmes.one/upload/solution/69c53f51f2d49d8512f64b7a */
#include <stdio.h>
#include <string.h>
#include <windows.h>
#include <ctype.h>

/* reading machin guid from the registry */
int get_machine_guid(char *guid, int size) {
    HKEY hKey;
    DWORD type = REG_SZ;
    DWORD dataSize = size;
    LONG result;
    
    result = RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Cryptography", 0, KEY_READ, &hKey);
    if (result != ERROR_SUCCESS) {
        printf("Failed to open registry key!\n");
        return 0;
    }
    // getting guid -> &guid
    result = RegQueryValueExA(hKey, "MachineGuid", NULL, &type, (LPBYTE)guid, &dataSize);
    RegCloseKey(hKey);
    
    if (result != ERROR_SUCCESS) {
        printf("Failed to read MachineGuid!\n");
        return 0;
    }
    
    // deleting '-'
    int write_pos = 0;
    for (int i = 0; guid[i] != '\0'; i++) {
        if (guid[i] != '-') {
            guid[write_pos++] = tolower(guid[i]);
        }
    }
    guid[write_pos] = '\0';
    
    return write_pos == 32;
}

/* basically just deleting all letter from the guid and changing to their equals */
void stage1_hex_to_decimal(const char *machine_guid, char *output) {
    for (int i = 0; i < 32; i++) {
        char c = tolower(machine_guid[i]);
        
        if (c >= '0' && c <= '9') {
            output[i] = c;
        } 
        else if (c >= 'a' && c <= 'f') {
            output[i] = '0' + (c - 'a' + 1);
        }
        else {
            output[i] = '0';
        }
    }
    output[32] = '\0';
}

/* (stage1 % 5) */
void stage2_divide_by_5(const char *input, char *output) {
    int remainder = 0;
    
    for (int i = 0; i < 32; i++) {
        int current = remainder * 10 + (input[i] - '0');
        output[i] = '0' + (current / 5);
        remainder = current % 5;
    }
    output[32] = '\0';
}

/* multiplying by 2 with carry */
void stage3_multiply_by_2(const char *input, char *output) {
    int carry = 0;
    
    for (int i = 31; i >= 0; i--) {
        int digit = input[i] - '0';
        int current = digit * 2 + carry;
        output[i] = '0' + (current % 10);
        carry = current / 10;
    }
    output[32] = '\0';
}

/* converting digits to letters */
void stage4_digits_to_password(const char *digits, char *password) {
    for (int i = 0; i < 32; i++) {
        int digit = digits[i] - '0';
        
        if (digit == 0) {
            password[i] = 'x';
        } else {
            password[i] = 'a' + (digit - 1);
        }
    }
    password[32] = '\0';
}

int generate_password(char *password) {
    char machine_guid[256];
    char stage1[33];
    char stage2[33];
    char stage3[33];
    
    if (!get_machine_guid(machine_guid, sizeof(machine_guid))) {
        printf("failed to get MachineGuid from registry.\n");
        return 0;
    }
    
    printf("machine GUID: %s\n", machine_guid);
    
    stage1_hex_to_decimal(machine_guid, stage1);
    printf("stage 1 (Hex->Decimal): %s\n", stage1);
    
    stage2_divide_by_5(stage1, stage2);
    printf("stage 2 (div 5): %s\n", stage2);
    
    stage3_multiply_by_2(stage2, stage3);
    printf("stage 3 (mul 2): %s\n", stage3);
    
    stage4_digits_to_password(stage3, password);
    printf("stage 4 (Password): %s\n", password);
    
    return 1;
}

int main() {
    char password[33];

    if (generate_password(password)) {
        printf("verification code: %s\n", password);
    } else {
        printf("\nfailed to generate verification code.\n");
        return 1;
    }
    
    system("pause");
    return 0;
}
