#include <oqs/oqs.h>
#include <iostream>
#include <vector>
#include <chrono>

using namespace std::chrono;

int main() {
    const char* alg = OQS_KEM_alg_kyber_512;
    OQS_KEM* kem = OQS_KEM_new(alg);
    
    std::vector<uint8_t> pk(kem->length_public_key);
    std::vector<uint8_t> sk(kem->length_secret_key);
    std::vector<uint8_t> ct(kem->length_ciphertext);
    std::vector<uint8_t> ss_enc(kem->length_shared_secret);
    std::vector<uint8_t> ss_dec(kem->length_shared_secret);
    
    const int ITERATIONS = 10000;
    
    auto start = high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; i++) {
        OQS_KEM_keypair(kem, pk.data(), sk.data());
    }
    auto end = high_resolution_clock::now();
    auto keygen_ns = duration_cast<nanoseconds>(end - start).count() / ITERATIONS;
    
    OQS_KEM_keypair(kem, pk.data(), sk.data());
    
    start = high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; i++) {
        OQS_KEM_encaps(kem, ct.data(), ss_enc.data(), pk.data());
    }
    end = high_resolution_clock::now();
    auto encaps_ns = duration_cast<nanoseconds>(end - start).count() / ITERATIONS;
    
    start = high_resolution_clock::now();
    for (int i = 0; i < ITERATIONS; i++) {
        OQS_KEM_decaps(kem, ss_dec.data(), ct.data(), sk.data());
    }
    end = high_resolution_clock::now();
    auto decaps_ns = duration_cast<nanoseconds>(end - start).count() / ITERATIONS;
    
    std::cout << "=== Kyber-512 Benchmark (" << ITERATIONS << " iters) ===\n";
    std::cout << "KeyGen: " << keygen_ns << " ns  (" << keygen_ns/1000.0 << " us)\n";
    std::cout << "Encaps: " << encaps_ns << " ns  (" << encaps_ns/1000.0 << " us)\n";
    std::cout << "Decaps: " << decaps_ns << " ns  (" << decaps_ns/1000.0 << " us)\n";
    std::cout << "Total:  " << (keygen_ns+encaps_ns+decaps_ns)/1000.0 << " us\n";
    
    OQS_KEM_free(kem);
    return 0;
}
