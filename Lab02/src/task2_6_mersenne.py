import math
import random

SMALL_PRIMES = [2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37, 41, 43, 47, 53, 59, 61, 67, 71]


# ---------- Câu 3: lũy thừa module (square-and-multiply) ----------
def mod_exp(a, x, p):
    """Tính a^x mod p bằng bình phương liên tiếp, độ phức tạp O(log x)."""
    result = 1
    a %= p
    while x > 0:
        if x & 1:                    # bit thấp nhất của x là 1
            result = (result * a) % p
        a = (a * a) % p              # bình phương cơ số
        x >>= 1                      # dịch sang bit kế tiếp
    return result


# ---------- Câu 2: GCD (Euclid) ----------
def gcd(a, b):
    while b:
        a, b = b, a % b
    return abs(a)


# ---------- Câu 1: Miller-Rabin ----------
def is_prime(n, rounds=40):
    """Kiểm tra nguyên tố Miller-Rabin. n < 3.3e24 là tất định với 13 cơ số đầu;
    với n lớn hơn, xác suất sai <= 4^(-rounds)."""
    if n < 2:
        return False
    for p in SMALL_PRIMES:
        if n == p:
            return True
        if n % p == 0:
            return False
    d, s = n - 1, 0                  # n-1 = 2^s * d, d lẻ
    while d % 2 == 0:
        d //= 2
        s += 1
    bases = SMALL_PRIMES[:13] if n < 3_317_044_064_679_887_385_961_981 else \
        [random.randrange(2, n - 1) for _ in range(rounds)]
    for a in bases:
        x = mod_exp(a, d, n)
        if x in (1, n - 1):
            continue
        for _ in range(s - 1):
            x = (x * x) % n
            if x == n - 1:
                break
        else:
            return False             # a là "chứng cứ" n là hợp số
    return True


def random_prime(bits):
    while True:
        n = random.getrandbits(bits) | (1 << (bits - 1)) | 1   # đủ bits, số lẻ
        if is_prime(n):
            return n


def largest_primes_below(limit, count):
    found, n = [], limit - 1
    while len(found) < count:
        if is_prime(n):
            found.append(n)
        n -= 1
    return found


if __name__ == "__main__":
    print("=== 1a. Số nguyên tố ngẫu nhiên ===")
    for b in (8, 16, 64):
        p = random_prime(b)
        print(f"{b:>2} bits: {p}  (bit_length = {p.bit_length()})")

    print("\n=== 1b. 10 số nguyên tố lớn nhất nhỏ hơn M10 = 2^89 - 1 ===")
    exps = [2, 3, 5, 7, 13, 17, 19, 31, 61, 89]
    print("10 số mũ Mersenne đầu tiên:", exps)
    M10 = 2 ** 89 - 1
    print("M10 =", M10, "| là số nguyên tố?", is_prime(M10))
    for i, p in enumerate(largest_primes_below(M10, 10), 1):
        print(f"{i:>2}. {p}")

    print("\n=== 1c. Kiểm tra số bất kỳ < 2^89 - 1 ===")
    tests = [2 ** 61 - 1, 2 ** 61 + 1, 2 ** 31 - 1, 561, 1000000007, 2 ** 88 + 1, M10 - 2]
    for n in tests:
        print(f"{n}: {'NGUYÊN TỐ' if is_prime(n) else 'hợp số'}")

    print("\n=== 2. GCD của hai số rất lớn ===")
    random.seed(2026)
    k = random_prime(512)                        # ước chung được cài sẵn
    a = k * random.getrandbits(512)
    b = k * random.getrandbits(512)
    g = gcd(a, b)
    print("a có", a.bit_length(), "bits; b có", b.bit_length(), "bits")
    print("gcd(a, b) có", g.bit_length(), "bits")
    print("Khớp với math.gcd:", g == math.gcd(a, b), "| chia hết cho k:", g % k == 0)
    print("gcd(48, 18) =", gcd(48, 18), "| gcd(2^89-1, 2^61-1) =", gcd(2 ** 89 - 1, 2 ** 61 - 1))

    print("\n=== 3. Lũy thừa module a^x mod p ===")
    print("7^40 mod 19 =", mod_exp(7, 40, 19), "| pow() =", pow(7, 40, 19))
    cases = [(7, 40, 19), (2, 100, 13), (3, 1000, 1000003), (5, 2 ** 64, 2 ** 89 - 1),
             (123456789, 10 ** 50, 2 ** 89 - 1)]
    for a_, x_, p_ in cases:
        r = mod_exp(a_, x_, p_)
        print(f"{a_}^{x_} mod {p_} = {r}  | khớp pow(): {r == pow(a_, x_, p_)}")