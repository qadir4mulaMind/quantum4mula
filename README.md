# QuantumLite

Lightweight Post-Quantum Cryptography for IoT Devices.

## Why?

Quantum computers will break RSA and ECC in the next decade. NIST has standardized Post-Quantum Cryptography (PQC) — but current PQC implementations are **too heavy for IoT devices** (ESP32, Arduino, Raspberry Pi).

**QuantumLite** provides lightweight, optimized PQC for resource-constrained devices.

## Status

- [x] Kyber-512/768/1024 working in C++
- [x] Benchmarks on Apple M3
- [ ] Benchmarks on Raspberry Pi
- [ ] ARM assembly optimization
- [ ] ESP32 port
- [ ] Dilithium (signatures)
- [ ] AI-based anomaly detection integration

## Benchmarks — Apple M3

**System:** Apple M3, macOS
**Compiler:** AppleClang 21.0.0
**Build:** Release (-O3)
**Iterations:** 10,000

| Variant | KeyGen | Encaps | Decaps | Total |
|---|---|---|---|---|
| Kyber-512  | 7.3 µs | 9.25 µs | 7.73 µs | 24.2 µs |
| Kyber-768  | 10.9 µs | 14.5 µs | 12.2 µs | 37.6 µs |
| Kyber-1024 | 15.4 µs | 19.8 µs | 17.4 µs | 52.5 µs |

### Key Sizes

| Variant | Public Key | Private Key | Ciphertext | Shared Secret |
|---|---|---|---|---|
| Kyber-512  | 800 B  | 1632 B | 768 B  | 32 B |
| Kyber-768  | 1184 B | 2400 B | 1088 B | 32 B |
| Kyber-1024 | 1568 B | 3168 B | 1568 B | 32 B |

### Observations

- **Kyber-512** is fastest and smallest — ideal for IoT
- Each variant is ~1.5x slower than the previous
- M3 ARM crypto extensions give excellent performance
- Expected 10x slowdown on Raspberry Pi (est. 150-250 µs)

## Build

### Prerequisites (macOS)

```bash
brew install cmake ninja openssl@3 git