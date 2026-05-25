# hair-brained-hash

**Fast • High quality • 64-bit non-cryptographic hash**

Built around a jumpable PRNG for strong diffusion. Ideal as a statistically robust, modern alternative to FNV-1a.

### Features
- Passes full SMHasher test suite (including avalanche, bit independence, etc.)
- ~2.7 GB/s on modern hardware (single-threaded)
- Very small code size (~100 lines)
- Simple API
- MIT licensed
- 30 Lines of code in the hasher, another 30 lines for a minimal cooperating random number generator.

### Quick Start

```cpp
#include "HBHash.h"

uint64_t seed = 42;
const char* data = "example";
size_t len = 7;

uint64_t hash = HairBrainedHash(seed, (const uint8_t*)data, len);
```

### Ideal Use Case
For anyone currently using std::hash or raw FNV-1a for:

- Custom hash maps (std::unordered_map bucket hashing)
- String-to-ID compile-time hashing
- Basic asset indexing in game engines

HairBrainedHash is a massive safety upgrade to FNV-1a. It protects the application from hash-collision performance degradation (where a hash map accidentally degrades into a slow linked list because of bad distribution) while keeping the code beautifully compact.

### How it works

The core idea is unconventional but effective:

- Process input in 8-byte words.
- Maintain a jumpable PRNG state.
- For each word: `rng.discard( rng() ^ (counter += golden_ratio) ^ input_word )`
  - `rng()` provides **non-linearity**
  - The counter prevents simple patterns/collisions
  - The input word steers the future state of the generator

After processing all chunks, one final `rng()` extracts the hash.

### Need a 128-bit output?

You can run two instances of HairBrainedHash in parallel using two different seeds, combining their outputs to form a 128-bit value. 
- For Performance & Capacity: This is an excellent, fast way to eliminate accidental collisions in massive, distributed datasets exceeding 2^32 elements.
- CRITICAL SECURITY NOTE: This construction is not cryptographically secure. Because both halves are based on the same 64-bit state machine design (just with different seeds), it does not provide the full security strength of a proper 128-bit hash like BLAKE3. An attacker who finds a way to analyze or attack the underlying structure may be able to generate collisions more efficiently than against a well-designed 128-bit cryptographic hash.
 
### Performance & Quality

- Speed: Approximately 2.7 GB/s
- Quality: Passes the complete SMHasher suite. See SMHasher output log for details.

### Security
- Non-cryptographic.
- 64-bit hashes are unsuitable for any security-sensitive application (collision attacks are trivial). Use established cryptographic hashes (BLAKE3, SHA-3, etc.) when security matters.

### Why "Hair-Brained"?
It started as a silly experiment: What if I just abused a good RNG as the mixer? Surprisingly, it works very well.

### Limitations

- 64-bit output only (not suitable for hash tables with > 2³² entries)
- Not designed for incremental hashing (though it could be extended)
- Platform assumes 64-bit with good uint64_t multiplication
- Currently compiles with MSVC. Simple replacement of 128 bit integers possible for gcc.

### License
MIT
