#include <oqs/oqs.h>
#include <iostream>
#include <vector>
#include <cstring>
#include <cstdio>

int main() {
    const char* alg_name = OQS_KEM_alg_kyber_512;
    
    OQS_KEM* kem = OQS_KEM_new(alg_name);
    if (kem == nullptr) {
        std::cerr << "KEM init failed!\n";
        return 1;
    }
    
    std::cout << "Algorithm: " << kem->method_name << "\n";
    std::cout << "Public key size:  " << kem->length_public_key << " bytes\n";
    std::cout << "Private key size: " << kem->length_secret_key << " bytes\n";
    std::cout << "Ciphertext size:  " << kem->length_ciphertext << " bytes\n";
    std::cout << "Shared secret:    " << kem->length_shared_secret << " bytes\n";
    
    std::vector<uint8_t> public_key(kem->length_public_key);
    std::vector<uint8_t> private_key(kem->length_secret_key);
    
    OQS_STATUS rc = OQS_KEM_keypair(kem, public_key.data(), private_key.data());
    if (rc != OQS_SUCCESS) {
        std::cerr << "KeyGen failed!\n";
        return 1;
    }
    std::cout << "\n[OK] Key pair generated\n";
    
    std::vector<uint8_t> ciphertext(kem->length_ciphertext);
    std::vector<uint8_t> shared_secret_enc(kem->length_shared_secret);
    
    rc = OQS_KEM_encaps(kem, ciphertext.data(), shared_secret_enc.data(), public_key.data());
    if (rc != OQS_SUCCESS) {
        std::cerr << "Encaps failed!\n";
        return 1;
    }
    std::cout << "[OK] Encapsulation done\n";
    
    std::vector<uint8_t> shared_secret_dec(kem->length_shared_secret);
    
    rc = OQS_KEM_decaps(kem, shared_secret_dec.data(), ciphertext.data(), private_key.data());
    if (rc != OQS_SUCCESS) {
        std::cerr << "Decaps failed!\n";
        return 1;
    }
    std::cout << "[OK] Decapsulation done\n";
    
    if (shared_secret_enc == shared_secret_dec) {
        std::cout << "\n[SUCCESS] Shared secrets match!\n";
    } else {
        std::cout << "\n[FAIL] Secrets don't match!\n";
    }
    
    std::cout << "Shared secret (first 8 bytes): ";
    for (int i = 0; i < 8; i++) {
        printf("%02x ", shared_secret_enc[i]);
    }
    std::cout << "\n";
    
    OQS_KEM_free(kem);
    return 0;
}
