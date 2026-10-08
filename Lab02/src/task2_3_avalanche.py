# Task 2.3 — Avalanche Effect với DES
# ============================================================
# Mã hóa "STAYHOME" và "STAYHOMA" bằng DES, đo Hamming Distance.
# Thay đổi key theo MSSV từng thành viên nhóm để so sánh.
#
# Cài đặt: pip install pycryptodome
# Chạy: python task2_3_avalanche.py

from Crypto.Cipher import DES


def hamming_distance_bytes(b1: bytes, b2: bytes) -> int:
    """Đếm tổng số bit khác nhau giữa 2 chuỗi bytes cùng độ dài."""
    total = 0
    for x, y in zip(b1, b2):
        total += bin(x ^ y).count('1')
    return total


def avalanche_test(key: bytes, p1: bytes, p2: bytes):
    """
    Mã hóa p1 và p2 bằng DES-ECB với key đã cho.
    In kết quả và tỷ lệ bit thay đổi.
    """
    assert len(key) == 8, "DES key phải đúng 8 bytes"
    assert len(p1) == 8 and len(p2) == 8, "DES block phải đúng 8 bytes"

    cipher1 = DES.new(key, DES.MODE_ECB)
    cipher2 = DES.new(key, DES.MODE_ECB)
    c1 = cipher1.encrypt(p1)
    c2 = cipher2.encrypt(p2)

    # Đếm bit khác nhau ở bản rõ
    input_diff = hamming_distance_bytes(p1, p2)
    # Đếm bit khác nhau ở bản mã
    output_diff = hamming_distance_bytes(c1, c2)
    total_bits = len(c1) * 8  # 64 bit

    pct = output_diff / total_bits * 100

    print(f"  Key     : {key}  ({key.hex()})")
    print(f"  P1      : {p1}  → C1: {c1.hex()}")
    print(f"  P2      : {p2}  → C2: {c2.hex()}")
    print(f"  Bản rõ khác nhau   : {input_diff}/{total_bits} bit ({input_diff/total_bits*100:.1f}%)")
    print(f"  Bản mã khác nhau   : {output_diff}/{total_bits} bit ({pct:.1f}%)")
    if pct >= 45:
        print(f"  → Hiệu ứng thác đổ TỐT (lý tưởng ≈ 50%)")
    else:
        print(f"  → Hiệu ứng thác đổ chưa đạt 50%")

    return output_diff, pct


def main():
    p1 = b'STAYHOME'
    p2 = b'STAYHOMA'   # Chỉ khác 1 ký tự cuối (E → A = 4 bit khác nhau)

    print("=" * 60)
    print("Task 2.3 — Avalanche Effect với DES")
    print("=" * 60)
    print(f"\nP1 = {p1}  | P2 = {p2}")
    diff_bits = hamming_distance_bytes(p1, p2)
    print(f"P1 và P2 khác nhau: {diff_bits} bit\n")

    # Danh sách key thử nghiệm: key mặc định + MSSV từng thành viên
    # TODO: điền MSSV thật của nhóm vào đây (8 ký tự = 8 bytes)
    keys = [
        (b'87654321', "Key mặc định (đề bài)"),
        (b'24520324', "MSSV Nguyễn Trung Đức"),
        (b'24520341', "MSSV Nguyễn Tiến Dũng"),
    ]

    results = []
    for key_bytes, label in keys:
        print(f"--- {label} ---")
        diff, pct = avalanche_test(key_bytes, p1, p2)
        results.append((label, diff, pct))
        print()

    print("=" * 60)
    print("Tổng hợp kết quả")
    print("=" * 60)
    print(f"{'Key':<35} {'Bit khác':>9} {'Tỷ lệ %':>9}")
    print("-" * 55)
    for label, diff, pct in results:
        print(f"{label:<35} {diff:>9}/64 {pct:>8.1f}%")

    print("""
Nhận xét:
- DES có 16 vòng xử lý → thay đổi 1 bit đầu vào gây ra
  ~50% bit bản mã thay đổi (xấp xỉ lý tưởng).
- Tỷ lệ khác nhau thay đổi theo key nhưng luôn gần 50%,
  cho thấy DES đạt hiệu ứng thác đổ tốt.
- Đây là tính chất QUAN TRỌNG của mật mã an toàn: kẻ tấn
  công không thể suy ra quan hệ giữa bản rõ và bản mã.
""")


if __name__ == "__main__":
    main()
