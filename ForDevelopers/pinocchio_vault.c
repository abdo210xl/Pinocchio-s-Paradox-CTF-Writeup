#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/sha.h>

void decrypt_data(unsigned char *data, size_t len, char key) {
    for (size_t i = 0; i < len; i++) {
        data[i] ^= key;
    }
}

int main() {
    char user_input[100];
    unsigned char stored_hash[SHA256_DIGEST_LENGTH];
    unsigned char user_hash[SHA256_DIGEST_LENGTH];
    char key = 0xAA;

    FILE *vault_file = fopen(".vault.dat", "rb");

    // Read the stored hash (first 32 bytes)
    fread(stored_hash, 1, SHA256_DIGEST_LENGTH, vault_file);
    
    // Read sizes and encrypted data
    long prompt_size, success_size, decoy_size;
    
    fread(&prompt_size, sizeof(long), 1, vault_file);
    char *encrypted_prompt = malloc(prompt_size);
    fread(encrypted_prompt, 1, prompt_size, vault_file);
    
    fread(&success_size, sizeof(long), 1, vault_file);
    char *encrypted_success = malloc(success_size);
    fread(encrypted_success, 1, success_size, vault_file);
    
    fread(&decoy_size, sizeof(long), 1, vault_file);
    char *encrypted_decoy = malloc(decoy_size);
    fread(encrypted_decoy, 1, decoy_size, vault_file);
    
    fclose(vault_file);

    // Decrypt and display prompt
    decrypt_data(encrypted_prompt, prompt_size, key);
    printf("%s", encrypted_prompt);
    scanf("%99s", user_input);

    // Hash the user's input
    SHA256((unsigned char *)user_input, strlen(user_input), user_hash);

    if (memcmp(user_hash, stored_hash, SHA256_DIGEST_LENGTH) == 0) {
        // CORRECT PASSWORD: Decrypt and show complete success message
        decrypt_data(encrypted_success, success_size, key);
        printf("%s", encrypted_success);
    } else {
        // WRONG PASSWORD: Show decoy message
        decrypt_data(encrypted_decoy, decoy_size, key);
        printf("%s", encrypted_decoy);
    }

    free(encrypted_prompt);
    free(encrypted_success);
    free(encrypted_decoy);
    return 0;
}
