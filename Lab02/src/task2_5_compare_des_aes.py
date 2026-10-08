# Task 2.5 — So sánh DES, Triple-DES (3DES), AES và tại sao không dùng 2DES
# ============================================================
# Phân tích lý thuyết + demo thực tế bằng Python.
#
# Cài đặt: pip install pycryptodome
# Chạy: python task2_5_compare_des_aes.py

import time
from Crypto.Cipher import DES, DES3, AES
from Crypto.Random import get_random_bytes

PLAINTEXT = b'ABCDEFGH' * 1000   # 8000 bytes để đo tốc độ rõ ràng


def benchmark(name, encrypt_fn, iterations=100):
    """Đo thời gian mã hóa trung bình."""
    start = time.perf_counter()
    for _ in range(iterations):
        encrypt_fn()
    elapsed = (time.perf_counter() - start) / iterations * 1000
    return elapsed


def demo_des():
    key = get_random_bytes(8)
    def enc():
        cipher = DES.new(key, DES.MODE_ECB)
        return cipher.encrypt(PLAINTEXT)
    return benchmark("DES", enc)


def demo_3des():
    key = get_random_bytes(24)   # 3×8 = 24 bytes cho 3DES-EDE
    def enc():
        cipher = DES3.new(key, DES3.MODE_ECB)
        return cipher.encrypt(PLAINTEXT)
    return benchmark("3DES", enc)


def demo_aes128():
    key = get_random_bytes(16)
    def enc():
        cipher = AES.new(key, AES.MODE_ECB)
        return cipher.encrypt(PLAINTEXT)
    return benchmark("AES-128", enc)


def demo_aes256():
    key = get_random_bytes(32)
    def enc():
        cipher = AES.new(key, AES.MODE_ECB)
        return cipher.encrypt(PLAINTEXT)
    return benchmark("AES-256", enc)


def main():
    print("=" * 65)
    print("Task 2.5 — So sánh DES, 3DES, AES và tại sao không dùng 2DES")
    print("=" * 65)

    print("""
┌─────────────────────────────────────────────────────────────┐
│              BẢNG SO SÁNH DES / 3DES / AES                  │
├────────────┬────────────┬────────────┬────────────┬──────────┤
│ Tiêu chí   │    DES     │    3DES    │  AES-128   │  AES-256 │
├────────────┼────────────┼────────────┼────────────┼──────────┤
│ Năm ra đời │    1977    │    1999    │    2001    │   2001   │
│ Khóa (bit) │     56     │  112/168   │    128     │   256    │
│ Khối (bit) │     64     │     64     │    128     │   128    │
│ Số vòng    │     16     │  48 (3×16) │   10/12/14 │   14     │
│ Tốc độ     │   Nhanh    │   Chậm     │   Nhanh    │  Nhanh   │
│ Bảo mật    │  Yếu (*)   │  Trung bình│   Tốt      │  Tốt     │
│ NIST       │  Đã bỏ     │  Bỏ 2023  │  Chuẩn HT  │ Chuẩn HT │
└────────────┴────────────┴────────────┴────────────┴──────────┘

(*) DES 56-bit: đã bị phá bằng brute-force năm 1998 (22.5 giờ,
    phần cứng trị giá 250.000 USD). Không an toàn từ 1999.
""")

    print("-" * 65)
    print("Tại sao KHÔNG dùng 2DES (Double-DES)?")
    print("-" * 65)
    print("""
2DES mã hóa 2 lần: C = DES_K2(DES_K1(P))
→ Độ dài khóa hiệu quả: 56 + 56 = 112 bit
→ Tưởng rằng không gian khóa tăng từ 2^56 lên 2^112

NHƯNG: dễ bị tấn công Meet-in-the-Middle (MITM):

  Ý tưởng:
    - Mã hóa P với tất cả 2^56 khóa K1 → lưu vào bảng T
    - Giải mã C với tất cả 2^56 khóa K2 → kiểm tra bảng T
    - Nếu DES_K1(P) == DES^-1_K2(C) → tìm được (K1, K2)

  Chi phí thực tế: 2^57 phép toán + 2^56 bộ nhớ
  → Độ bảo mật THỰC TẾ chỉ ~57 bit (gần bằng DES gốc!)
  → Không đáng so với việc dùng khóa 112 bit

3DES khắc phục bằng cách dùng K1 ≠ K3:
  C = DES_K3(DES^-1_K2(DES_K1(P)))  (EDE — Encrypt-Decrypt-Encrypt)
  → MITM cần 2^112 phép toán (an toàn hơn 2DES rất nhiều)
  → Nhưng chậm 3× so với DES, và AES an toàn hơn lại nhanh hơn
""")

    print("-" * 65)
    print("Benchmark thực tế (8000 bytes, trung bình 100 lần):")
    print("-" * 65)
    t_des    = demo_des()
    t_3des   = demo_3des()
    t_aes128 = demo_aes128()
    t_aes256 = demo_aes256()

    print(f"  DES      : {t_des:.3f} ms/lần")
    print(f"  3DES     : {t_3des:.3f} ms/lần  (~{t_3des/t_des:.1f}× chậm hơn DES)")
    print(f"  AES-128  : {t_aes128:.3f} ms/lần  (~{t_aes128/t_des:.1f}× so với DES)")
    print(f"  AES-256  : {t_aes256:.3f} ms/lần  (~{t_aes256/t_des:.1f}× so với DES)")

    print("""
Kết luận:
  - DES: KHÔNG dùng nữa (đã bị phá vỡ từ 1998).
  - 2DES: KHÔNG dùng vì dễ tấn công Meet-in-the-Middle.
  - 3DES: Vẫn an toàn nhưng chậm, NIST đã loại bỏ năm 2023.
  - AES: Chuẩn mực hiện đại, nhanh, an toàn, được dùng rộng rãi.
  → Lựa chọn đúng cho hệ thống mới: AES-128 (thông thường)
    hoặc AES-256 (yêu cầu bảo mật cao / dài hạn).
""")


if __name__ == "__main__":
    main()
