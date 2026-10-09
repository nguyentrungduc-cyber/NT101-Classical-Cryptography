# Task 2.1 — Feistel Cipher & Sự lan truyền thay đổi (Avalanche Effect)
# Triển khai Feistel cipher 4 vòng để quan sát cách thay đổi bit

#Khai báo hàm
def F(right, subkey):
    """Hàm nội bộ Feistel: kết hợp XOR và dịch bit."""
    return (right ^ subkey) & 0x0F


def feistel_round(L_in, R_in, subkey):
    """
    1 vòng Feistel:
        LE_i = RE_{i-1}
        RE_i = LE_{i-1} XOR F(RE_{i-1}, K_i)
    """
    L_out = R_in
    R_out = L_in ^ F(R_in, subkey)
    return L_out, R_out


def track_avalanche(msg, key, label=""):
    """
    Mã hóa msg qua 4 vòng Feistel và in giá trị (L, R) từng vòng.
    msg: số nguyên 8-bit (chia thành L=4 bit cao, R=4 bit thấp)
    key: số nguyên 8-bit (tạo 4 subkey)
    """
    L = (msg >> 4) & 0x0F   # 4 bit cao
    R = msg & 0x0F            # 4 bit thấp

    subkeys = [
        key & 0x0F,
        (key >> 4) & 0x0F,
        (key + 1) & 0xFF & 0x0F,
        (key + 2) & 0xFF & 0x0F,
    ]

    print(f"  Khởi tạo: L={format(L, '04b')}  R={format(R, '04b')}  (msg=0x{msg:02X})")
    for i in range(4):
        L, R = feistel_round(L, R, subkeys[i])
        print(f"  Vòng {i+1}:   L={format(L, '04b')}  R={format(R, '04b')}")

    cipher = (L << 4) | R
    print(f"  Bản mã:  0x{cipher:02X}  ({format(cipher, '08b')})")
    return cipher


def hamming_distance(a, b):
    """Đếm số bit khác nhau giữa 2 số nguyên."""
    return bin(a ^ b).count('1')


def main():
    key = 0x12
    M1 = 171
    M2 = 0xAC

    print("=" * 55)
    print("Task 2.1 — Feistel Cipher (4 vòng, 8-bit)")
    print("=" * 55)
    print(f"\nKey = 0x{key:02X} | M1 = 0x{M1:02X} | M2 = 0x{M2:02X}")
    print(f"M1 vs M2 khác nhau: {hamming_distance(M1, M2)} bit (bit cuối)\n")

    print("--- Mã hóa M1 ---")
    c1 = track_avalanche(M1, key)

    print("\n--- Mã hóa M2 ---")
    c2 = track_avalanche(M2, key)

    diff = hamming_distance(c1, c2)
    total_bits = 8
    print(f"\n{'=' * 55}")
    print(f"Bản mã M1 (C1): 0x{c1:02X}  ({format(c1, '08b')})")
    print(f"Bản mã M2 (C2): 0x{c2:02X}  ({format(c2, '08b')})")
    print(f"Số bit khác nhau giữa C1 và C2: {diff}/{total_bits} bit ({diff/total_bits*100:.1f}%)")

    # So sánh từng vòng để thấy sự lan truyền
    print(f"\n--- Nhận xét ---")
    print(f"Đầu vào M1 và M2 khác nhau 1 bit ({hamming_distance(M1, M2)}/{total_bits} = {hamming_distance(M1, M2)/total_bits*100:.1f}%)")
    print(f"Đầu ra C1 và C2 khác nhau {diff} bit ({diff}/{total_bits} = {diff/total_bits*100:.1f}%)")
    if diff > total_bits // 2:
        print("→ Hiệu ứng thác đổ tốt: thay đổi nhỏ ở đầu vào gây ra")
        print("  thay đổi lớn ở đầu ra sau 4 vòng Feistel.")
    else:
        print("→ Hiệu ứng thác đổ chưa mạnh (Feistel 4 vòng đơn giản)")


if __name__ == "__main__":
    main()
