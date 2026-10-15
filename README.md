# Quantum4mula

> Post-Quantum Cryptography for IoT — the formula for quantum-safe devices.

Lightweight PQC implementation for resource-constrained devices.

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![C++17](https://img.shields.io/badge/C%2B%2B-17-blue.svg)](https://isocpp.org/)
[![Platform: macOS](https://img.shields.io/badge/Platform-macOS-lightgrey.svg)]()

## Why Quantum4mula?

Quantum computers will break RSA and ECC in the next decade. NIST has standardized Post-Quantum Cryptography (PQC) — but current implementations are **too heavy for IoT devices** (ESP32, Arduino, Raspberry Pi).

**Quantum4mula** provides lightweight, optimized PQC for resource-constrained devices.

## Features

- ✅ **Kyber-512/768/1024** — all three NIST-standardized variants
- ✅ **Zero-copy API** — uses caller-provided buffers
- ✅ **ARM-optimized** — leverages ARMv8.5 crypto extensions
- ✅ **Build-clean** — CMake + Ninja, no warnings
- ⏳ **ARM assembly** — custom optimizations (in progress)
- ⏳ **ESP32 port** — sub-50KB memory target (planned)

## Benchmarks — Apple M3

**System:** Apple M3, macOS  
**Compiler:** AppleClang 21.0.0  
**Build:** Release (`-O3`)  
**Iterations:** 10,000

| Variant | KeyGen | Encaps | Decaps | **Total** |
|---|---|---|---|---|
| **Kyber-512**  | 7.3 µs | 9.25 µs | 7.73 µs | **24.2 µs** |
| **Kyber-768**  | 10.9 µs | 14.5 µs | 12.2 µs | **37.6 µs** |
| **Kyber-1024** | 15.4 µs | 19.8 µs | 17.4 µs | **52.5 µs** |

### Key Sizes

| Variant | Public Key | Private Key | Ciphertext | Shared Secret |
|---|---|---|---|---|
| Kyber-512  | 800 B  | 1632 B | 768 B  | 32 B |
| Kyber-768  | 1184 B | 2400 B | 1088 B | 32 B |
| Kyber-1024 | 1568 B | 3168 B | 1568 B | 32 B |

### Observations

- **Kyber-512** is the fastest and smallest — ideal for IoT devices
- Each variant is roughly **1.5× slower** than the previous
- Apple M3 ARM crypto extensions give excellent performance
- Expected **~10× slowdown on Raspberry Pi** (est. 150-250 µs)
- **ESP32 target:** < 500 µs per operation (feasibility being tested)

## Quick Start

### Prerequisites (macOS)

```bash
brew install cmake ninja openssl@3 git