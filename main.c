#include <stdio.h>
#include <stdint.h>

// Calculcates a^n % mod
uint64_t modexp(uint64_t a, uint64_t n, uint64_t mod) {
  size_t power = a;
  size_t result = 1;

  while (n) {
    if (n & 1) {
      result = (result * power) % mod;
    }

    power = (power * power) % mod;
    n >>= 1;
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

// Finds the biggest prime where l*2^k+1 < max
uint64_t prime(uint64_t max) {

}

void print_prime(uint64_t c) {
  if(is_prime(c)) {
    printf("%ld is prime\n", c);
  } else {
    printf("%ld is not prime\n", c);
  }
}

int main(void) {
  uint64_t p = prime(UINT64_MAX);

  for(uint64_t i=0;i<10000000;i++) {
    print_prime(i);
  }


  return 0;
}
