# Task 2.6 — Mersenne Prime, GCD, Lũy thừa Modulo
# ============================================================
# Yêu cầu:
#   1. Tạo số nguyên tố ngẫu nhiên 8, 16, 64 bit
#      Xác định 10 số nguyên tố lớn nhất < Mersenne thứ 10
#      Kiểm tra số nguyên tố < 2^89 - 1
#   2. Tính GCD của 2 số nguyên lớn
#   3. Tính lũy thừa modulo ax mod p với số mũ lớn
#
# Cài đặt: pip install sympy
# Chạy: python task2_6_mersenne.py

import random
import sympy


# ============================================================
# Phần 1: Số nguyên tố
# ============================================================

def gen_random_prime(bits: int) -> int:
    """Sinh số nguyên tố ngẫu nhiên n bit bằng sympy."""
    return sympy.nextprime(random.getrandbits(bits))


def miller_rabin(n: int, k: int = 20) -> bool:
    """
    Kiểm tra nguyên tố bằng Miller-Rabin probabilistic test.
    k lần lặp → xác suất sai < 4^(-k).
    """
    if n < 2:
        return False
    if n == 2 or n == 3:
        return True
    if n % 2 == 0:
        return False

    # Viết n-1 = 2^r * d
    r, d = 0, n - 1
    while d % 2 == 0:
        r += 1
        d //= 2

    for _ in range(k):
        a = random.randrange(2, n - 1)
        x = pow(a, d, n)  # a^d mod n (Python tự tối ưu fast exponentiation)
        if x == 1 or x == n - 1:
            continue
        for _ in range(r - 1):
            x = pow(x, 2, n)
            if x == n - 1:
                break
        else:
            return False
    return True


# Số nguyên tố Mersenne: M_p = 2^p - 1
# Danh sách các p (số mũ) cho 10 số nguyên tố Mersenne đầu tiên
MERSENNE_EXPONENTS = [2, 3, 5, 7, 13, 17, 19, 31, 61, 89]

def mersenne(k: int) -> int:
    """Trả về số nguyên tố Mersenne thứ k (1-indexed): M_p = 2^p - 1."""
    p = MERSENNE_EXPONENTS[k - 1]
    return 2**p - 1


def ten_largest_primes_below(limit: int, n: int = 10) -> list:
    """
    Tìm n số nguyên tố lớn nhất nhỏ hơn limit bằng prevprime của sympy.
    Dùng sympy vì limit có thể rất lớn (hàng chục chữ số).
    """
    primes = []
    p = sympy.prevprime(limit)
    for _ in range(n):
        primes.append(p)
        p = sympy.prevprime(p)
    return primes


# ============================================================
# Phần 2: Ước chung lớn nhất (GCD)
# ============================================================

def gcd_euclid(a: int, b: int) -> int:
    """Thuật toán Euclid: gcd(a, b) = gcd(b, a mod b) đến khi b = 0."""
    while b:
        a, b = b, a % b
    return a


# ============================================================
# Phần 3: Lũy thừa modulo
# ============================================================

def modular_exp(base: int, exp: int, mod: int) -> int:
    """
    Tính base^exp mod n bằng fast exponentiation (square-and-multiply).
    Dùng tính chất: (a*b) mod n = ((a mod n) * (b mod n)) mod n
    → Tránh tràn số với số mũ lớn.
    """
    result = 1
    base = base % mod
    while exp > 0:
        if exp % 2 == 1:
            result = (result * base) % mod
        exp //= 2
        base = (base * base) % mod
    return result


# ============================================================
# Main
# ============================================================

def main():
    print("=" * 65)
    print("Task 2.6 — Mersenne Prime, GCD, Lũy thừa Modulo")
    print("=" * 65)

    # ---- Phần 1a: Sinh số nguyên tố ngẫu nhiên ----
    print("\n--- 1a. Số nguyên tố ngẫu nhiên ---")
    for bits in [8, 16, 64]:
        p = gen_random_prime(bits)
        print(f"  {bits:>2} bit: {p}  (kiểm tra: {miller_rabin(p)})")

    # ---- Phần 1b: 10 số nguyên tố Mersenne ----
    print("\n--- 1b. 10 số nguyên tố Mersenne đầu tiên ---")
    for i, exp in enumerate(MERSENNE_EXPONENTS, 1):
        m = 2**exp - 1
        print(f"  M{i:>2} = 2^{exp:<3} - 1 = {m}")

    m10 = mersenne(10)
    print(f"\n  Mersenne thứ 10: M10 = 2^89 - 1 = {m10}")

    print(f"\n--- 10 số nguyên tố lớn nhất < M10 ---")
    top10 = ten_largest_primes_below(m10)
    for i, p in enumerate(top10, 1):
        print(f"  {i:>2}. {p}")

    # ---- Phần 1c: Kiểm tra số nguyên tố tùy ý < 2^89 - 1 ----
    print(f"\n--- 1c. Kiểm tra số nguyên tố bằng Miller-Rabin ---")
    test_numbers = [
        2**61 - 1,          # Mersenne thứ 9 (là nguyên tố)
        2**67 - 1,          # 2^67 - 1 = 147573952589676412927 (KHÔNG nguyên tố)
        999999999999999877, # số nguyên tố lớn
        999999999999999878, # không nguyên tố (chẵn)
    ]
    for n in test_numbers:
        is_prime_mr = miller_rabin(n)
        is_prime_gt = sympy.isprime(n)  # ground truth để verify
        print(f"  {n}")
        print(f"    Miller-Rabin: {'NGUYÊN TỐ' if is_prime_mr else 'KHÔNG nguyên tố'}"
              f"  (sympy verify: {is_prime_gt})")

    # ---- Phần 2: GCD ----
    print("\n--- 2. Ước chung lớn nhất (GCD - thuật toán Euclid) ---")
    pairs = [
        (270, 192),
        (1234567890123456789, 9876543210987654321),
        (2**256 - 1, 2**128 - 1),   # Hai số cực lớn
    ]
    for a, b in pairs:
        g = gcd_euclid(a, b)
        g_verify = sympy.gcd(a, b)
        print(f"  gcd({a}, {b})")
        print(f"    = {g}  (verify: {g_verify == g})")

    # ---- Phần 3: Lũy thừa modulo ----
    print("\n--- 3. Lũy thừa modulo (fast exponentiation) ---")
    examples = [
        (7, 40, 19),                             # 7^40 mod 19 (đề bài)
        (2, 100, 1000000007),                    # 2^100 mod 10^9+7
        (3, 2**64, 10**18 + 9),                  # số mũ 2^64 (cực lớn)
    ]
    for base, exp, mod in examples:
        result = modular_exp(base, exp, mod)
        result_verify = pow(base, exp, mod)   # Python built-in (cùng thuật toán)
        print(f"  {base}^{exp} mod {mod}")
        print(f"    = {result}  (verify với pow(): {result == result_verify})")

    print(f"\n  Ghi chú: pow(a, x, p) của Python cũng dùng fast exponentiation,")
    print(f"  tự động xử lý số mũ lớn tùy ý. Hàm modular_exp() ở trên minh họa")
    print(f"  cách thuật toán hoạt động từng bước (square-and-multiply).")


if __name__ == "__main__":
    main()
