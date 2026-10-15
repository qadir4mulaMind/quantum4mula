#include <oqs/oqs.h>
#include <iostream>
#include <vector>
#include <chrono>

using namespace std::chrono;

void bench_kem(const char* alg_name, int iters) {
    OQS_KEM* kem = OQS_KEM_new(alg_name);
    if (!kem) return;
    
    std::vector<uint8_t> pk(kem->length_public_key);
    std::vector<uint8_t> sk(kem->length_secret_key);
    std::vector<uint8_t> ct(kem->length_ciphertext);
    std::vector<uint8_t> ss_enc(kem->length_shared_secret);
    std::vector<uint8_t> ss_dec(kem->length_shared_secret);
    
    // Warm-up
    for (int i = 0; i < 100; i++) {
        OQS_KEM_keypair(kem, pk.data(), sk.data());
    }
    OQS_KEM_keypair(kem, pk.data(), sk.data());
    
    // KeyGen
    auto start = high_resolution_clock::now();
    for (int i = 0; i < iters; i++) {
        OQS_KEM_keypair(kem, pk.data(), sk.data());
    }
    auto kg = duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() / iters;
    
    OQS_KEM_keypair(kem, pk.data(), sk.data());
    
    // Encaps
    start = high_resolution_clock::now();
    for (int i = 0; i < iters; i++) {
        OQS_KEM_encaps(kem, ct.data(), ss_enc.data(), pk.data());
    }
    auto en = duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() / iters;
    
    // Decaps
    start = high_resolution_clock::now();
    for (int i = 0; i < iters; i++) {
        OQS_KEM_decaps(kem, ss_dec.data(), ct.data(), sk.data());
    }
    auto de = duration_cast<nanoseconds>(high_resolution_clock::now() - start).count() / iters;
    
    std::cout << alg_name << ":\n";
    std::cout << "  KeyGen: " << kg << " ns (" << kg/1000.0 << " us)\n";
    std::cout << "  Encaps: " << en << " ns (" << en/1000.0 << " us)\n";
    std::cout << "  Decaps: " << de << " ns (" << de/1000.0 << " us)\n";
    std::cout << "  Total:  " << (kg+en+de)/1000.0 << " us\n\n";
    
    OQS_KEM_free(kem);
}

int main() {
    const int ITERS = 10000;
    std::cout << "=== Kyber Benchmark (" << ITERS << " iters) ===\n\n";
    
    bench_kem(OQS_KEM_alg_kyber_512, ITERS);
    bench_kem(OQS_KEM_alg_kyber_768, ITERS);
    bench_kem(OQS_KEM_alg_kyber_1024, ITERS);
    
    return 0;
}
