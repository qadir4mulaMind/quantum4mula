#include <oqs/oqs.h>
#include <iostream>
#include <vector>
#include <cstdio>

void test_kem(const char* alg_name) {
    std::cout << "\n=== " << alg_name << " ===\n";

    OQS_KEM* kem = OQS_KEM_new(alg_name);
    if (kem == nullptr) {
        std::cerr << "KEM init failed!\n";
        return;
    }

    std::cout << "Public key size:  " << kem->length_public_key << " bytes\n";
    std::cout << "Private key size: " << kem->length_secret_key << " bytes\n";
    std::cout << "Ciphertext size:  " << kem->length_ciphertext << " bytes\n";
    std::cout << "Shared secret:    " << kem->length_shared_secret << " bytes\n";

    std::vector<uint8_t> pk(kem->length_public_key);
    std::vector<uint8_t> sk(kem->length_secret_key);
    std::vector<uint8_t> ct(kem->length_ciphertext);
    std::vector<uint8_t> ss_enc(kem->length_shared_secret);
    std::vector<uint8_t> ss_dec(kem->length_shared_secret);

    if (OQS_KEM_keypair(kem, pk.data(), sk.data()) != OQS_SUCCESS) {
        std::cerr << "KeyGen failed!\n";
        OQS_KEM_free(kem);
        return;
    }

    if (OQS_KEM_encaps(kem, ct.data(), ss_enc.data(), pk.data()) != OQS_SUCCESS) {
        std::cerr << "Encaps failed!\n";
        OQS_KEM_free(kem);
        return;
    }

    if (OQS_KEM_decaps(kem, ss_dec.data(), ct.data(), sk.data()) != OQS_SUCCESS) {
        std::cerr << "Decaps failed!\n";
        OQS_KEM_free(kem);
        return;
    }

    if (ss_enc == ss_dec) {
        std::cout << "[OK] Round-trip successful\n";
    } else {
        std::cout << "[FAIL] Secrets don't match!\n";
    }

    OQS_KEM_free(kem);
}

int main() {
    test_kem(OQS_KEM_alg_kyber_512);
    test_kem(OQS_KEM_alg_kyber_768);
    test_kem(OQS_KEM_alg_kyber_1024);
    return 0;
}
