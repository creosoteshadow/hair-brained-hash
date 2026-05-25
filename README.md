# hair-brained-hash
Simple, high quality non-cryptographic hash function using a jumpable random number generator.

This began with what I thought was a hair-brained idea: use a jumpable random number generator
to create a simple hash function. The input data is processed in 8 byte chunks, and the
generator is jumped with a rng.discard( rng() ^ (counter+=increment) ^ input_word ) pattern.
The rng() provides a non-linear feedback, the counter provides protection against simple
correlations in the data, and the input word provides a steering wheel that drives the
state of the random number generator.

Usage:

uint64_t seed = 42;
std::string s( "My hair-brained hash function." );
uint64_t hashvalue = HairBrainedHash( seed, (const uint8_t*) s.data(), s.size());

Performance: Approximately 2.7 GB/s.

Quality: Passes the full SMHasher suite of tests. See the output log for details.

Security: non-cryptographic.

License: MIT
