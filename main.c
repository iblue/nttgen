#include <stdio.h>
#include <stdint.h>

// Calculcates a^n % mod
uint64_t modexp(uint64_t a, uint64_t exp, uint64_t modulus) {
  uint64_t result = 1;

  // Slow, but needed to prevent overflow.
  __uint128_t base = a%modulus;

  while (exp > 0) {
    if (exp & 1) {
      result = (result * base) % modulus;
    }

    base = (base * base) % modulus;
    exp >>= 1;
  }

  return result;
}

// Deterministic Miller-Rabin primality test for values up to 2^64.
int is_prime(const uint64_t n) {
  // Jim Sinclair SPRP for n <= 2^64
  const uint64_t sprps[] = {2, 325, 9375, 28178, 450775, 9780504, 1795265022};

  if (n == 2) {
    return 1;
  }

  if (n < 2 || n % 2 == 0) {
    return 0;
  }

  uint64_t d = n - 1;
  uint64_t s = 0;

  while (d % 2 == 0) {
    d /= 2;
    s++;
  }

  for (size_t i = 0; i < sizeof(sprps); i++) {
    uint64_t a = sprps[i];

    if(a > n) {
      return 1;
    }

    uint64_t x = modexp(a, d, n);

    if (x == 1 || x == n-1) {
      continue;
    }

    for (uint64_t j = 1; j < s; j++) {
      x = modexp(x, 2, n);
      if (x == 1) {
        return 0;
      }

      if (x == n-1) {
        goto next;
      }
    }

    return 0;

    next: continue;
  }

  return 1;
}

// x86 magic for lazy people
static inline uint32_t intlog2(const uint64_t x){return (63 - __builtin_clz (x));}

// Finds the biggest prime where l*2^k+1 < max with maximum k.
uint64_t prime(void) {
  uint64_t max_k = 63;

  for(uint64_t k = max_k; k > 0; k--) {
    uint64_t l_max = 1ULL << (63-k);
    for(uint64_t l = l_max-1; l > 1; l-=2) {
      uint64_t p = l*(1ULL << k)+1;

      if(is_prime(p)) {
        printf("Found %ju = %ju*2^%ju + 1 to be prime\n", p, l, k);

        // Now search roots of unity
        uint64_t r = 1;
        while(r++) {
          if(!is_prime(r)) {
            continue;
          }

          if(modexp(r, (p-1)/2, p) != 1 && modexp(r, (p-1)/l, p) != 1) {
            printf("  Primitive root of unity: %ju\n", r);
            uint64_t rp = modexp(r, l, p);
            printf("  Root of unity for transformations up to 2^%ju: %ju\n", k, rp);
            for(int j=k;j > 1; j--) {
              rp = modexp(rp, 2, p);
              printf("  Root of unity for transformations up to 2^%d: %ju\n", j, rp);
            }
            printf("  Scaling factor: %ju\n", p/2+1);
            break;
          }
        }
      }
    }
  }
}

int main(void) {
  prime();

  return 0;
}
