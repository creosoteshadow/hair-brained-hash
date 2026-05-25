#pragma once

#include <cstdint>
#include <intrin.h> // _umul128
#include <string> // memcpy

namespace HBHash {
	// Simple random number generator -- closely related to wyrand
	class RNG {
		// 64 bit state
		uint64_t state;

	public:

		RNG(uint64_t seed = 0) :state(seed) {}

		uint64_t operator()() {
			state += 0x2d358dccaa6c78a5ull;
			return wymix(state, state ^ 0x8bb84b93962eacc9ull);
		}

		void discard(uint64_t jump) {
			state += jump * 0x2d358dccaa6c78a5ull;
		}

	private:

		uint64_t wymix(uint64_t a, uint64_t b) {
			uint64_t lo, hi;
			lo = _umul128(a, b, &hi);
			/*
			* Note: in wyhash they use lo ^ hi. But here if one of a and b is zero, I wanted to allow
			* the result contain the entropy in the other. So, I mixed in the a ^ b term.
			*/
			return lo ^ hi ^ a ^ b;
		}
	};

	uint64_t HairBrainedHash(uint64_t seed, const uint8_t* input, size_t size) {
		size_t length = size;
		RNG gen(seed);
		uint64_t counter = gen();
		uint64_t buffer;

		while (size >= 8) {
			memcpy(&buffer, input, 8);
			uint64_t rng_draw = gen();
			gen.discard(buffer ^ counter ^ rng_draw);

			counter += 0x9e3779b97f4a7c15ull;
			input += 8;
			size -= 8;
		}

		// handle tail
		// size is now in [0,7]
		uint8_t chars[8] = { 0 };
		if (size > 0)
			memcpy(chars, input, size); // tail
		chars[size] = 0x80; // domain separation
		memcpy(&buffer, chars, 8); // copy to integer so the ^ operation works right
		uint64_t rng_draw = gen();
		gen.discard(buffer ^ counter ^ rng_draw);
		counter += 0x9e3779b97f4a7c15ull;

		// Length
		rng_draw = gen();
		gen.discard(length ^ counter ^ rng_draw);

		return gen();
	}

}
