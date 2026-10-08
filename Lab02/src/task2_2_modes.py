# Task 2.2 — Mode of Operation: AES-ECB, AES-CBC và các mode khác
# ============================================================
# So sánh sự khác biệt bảo mật giữa ECB và CBC khi mã hóa
# dữ liệu có tính lặp lại cao.
#
# Cài đặt: pip install pycryptodome
# Chạy: python task2_2_modes.py

from Crypto.Cipher import AES
from Crypto.Util.Padding import pad
import os

KEY = b'1234567890123456'       # 16 bytes = AES-128
IV  = b'abcdefghijklmnop'       # 16 bytes, cố định để dễ so sánh
PLAINTEXT = b"UIT_LAB_UIT_LAB_UIT_LAB_UIT_LAB_"  # 32 bytes (2 khối 16 byte)


def print_blocks(label, ciphertext, block_size=16):
    """In ciphertext theo từng khối để dễ so sánh."""
    print(f"  {label} (hex):")
    for i in range(0, len(ciphertext), block_size):
        block = ciphertext[i:i + block_size]
        print(f"    Khối {i // block_size + 1}: {block.hex()}")


def mode_ecb():
    cipher = AES.new(KEY, AES.MODE_ECB)
    ct = cipher.encrypt(PLAINTEXT)
    return ct


def mode_cbc():
    cipher = AES.new(KEY, AES.MODE_CBC, iv=IV)
    ct = cipher.encrypt(PLAINTEXT)
    return ct


def mode_cfb():
    # CFB dùng segment_size=128 (CFB-128, tương đương CBC về block chaining)
    cipher = AES.new(KEY, AES.MODE_CFB, iv=IV, segment_size=128)
    ct = cipher.encrypt(PLAINTEXT)
    return ct


def mode_ofb():
    cipher = AES.new(KEY, AES.MODE_OFB, iv=IV)
    ct = cipher.encrypt(PLAINTEXT)
    return ct


def mode_ctr():
    # CTR dùng nonce thay vì IV (pycryptodome tự tạo)
    cipher = AES.new(KEY, AES.MODE_CTR, nonce=b'12345678')  # 8-byte nonce
    ct = cipher.encrypt(PLAINTEXT)
    return ct


def main():
    print("=" * 60)
    print("Task 2.2 — AES Mode of Operation")
    print("=" * 60)
    print(f"\nPlaintext: {PLAINTEXT}")
    print(f"  = {PLAINTEXT.hex()} (hex)")
    print(f"\nLưu ý: bản rõ gồm 2 khối 16 byte GIỐNG HỆT nhau:")
    print(f"  Khối 1: {PLAINTEXT[:16].hex()}")
    print(f"  Khối 2: {PLAINTEXT[16:].hex()}")

    ct_ecb = mode_ecb()
    ct_cbc = mode_cbc()
    ct_cfb = mode_cfb()
    ct_ofb = mode_ofb()
    ct_ctr = mode_ctr()

    print("\n" + "-" * 60)
    print("ECB (Electronic Code Book):")
    print_blocks("ECB", ct_ecb)
    block1 = ct_ecb[:16].hex()
    block2 = ct_ecb[16:].hex()
    if block1 == block2:
        print(f"  ⚠️  Khối 1 == Khối 2 (LỖ HỔNG BẢO MẬT: lộ cấu trúc dữ liệu!)")
    else:
        print(f"  ✅ Khối 1 ≠ Khối 2")

    print("\n" + "-" * 60)
    print("CBC (Cipher Block Chaining):")
    print_blocks("CBC", ct_cbc)
    block1 = ct_cbc[:16].hex()
    block2 = ct_cbc[16:].hex()
    if block1 != block2:
        print(f"  ✅ Khối 1 ≠ Khối 2 (bản rõ giống nhau → bản mã KHÁC nhau)")

    print("\n" + "-" * 60)
    print("CFB (Cipher FeedBack):")
    print_blocks("CFB", ct_cfb)

    print("\n" + "-" * 60)
    print("OFB (Output FeedBack):")
    print_blocks("OFB", ct_ofb)

    print("\n" + "-" * 60)
    print("CTR (Counter):")
    print_blocks("CTR", ct_ctr)

    print("\n" + "=" * 60)
    print("Nhận xét tổng hợp")
    print("=" * 60)
    print("""
ECB:
  - Mỗi khối bản rõ mã hóa ĐỘC LẬP bằng cùng khóa.
  - Khối bản rõ GIỐNG NHAU → khối bản mã GIỐNG NHAU.
  - LỘ CẤU TRÚC DỮ LIỆU (ảnh bitmap ECB-encrypted nổi tiếng).
  - Không dùng IV → không có tính ngẫu nhiên.
  - KHÔNG AN TOÀN cho dữ liệu có tính lặp lại.

CBC:
  - Mỗi khối bản rõ XOR với khối bản mã TRƯỚC đó trước khi mã hóa.
  - Khối bản rõ giống nhau → bản mã KHÁC nhau (nhờ chuỗi phụ thuộc).
  - Cần IV ngẫu nhiên cho mỗi lần mã hóa. IV phải duy nhất nhưng
    không cần bí mật (có thể gửi kèm bản mã).
  - Lỗi bit ở khối i ảnh hưởng khối i và i+1 khi giải mã (xem Task 2.4).
  - Không thể mã hóa song song (phụ thuộc tuần tự).

CFB:
  - Biến block cipher thành stream cipher.
  - Lỗi lan truyền tương tự CBC.
  - Có thể mã hóa dữ liệu không đủ 1 khối.

OFB:
  - Tạo keystream từ IV, XOR với bản rõ (giống stream cipher).
  - Lỗi bit ở bản mã CHỈ ảnh hưởng đúng vị trí đó khi giải mã.
  - Lỗi KHÔNG LAN TRUYỀN (ưu điểm trong môi trường có nhiễu).

CTR:
  - Mã hóa song song được (không phụ thuộc tuần tự).
  - Tốc độ nhanh nhất trong các mode.
  - Phải đảm bảo counter không bao giờ lặp lại với cùng một key.
""")


if __name__ == "__main__":
    main()
