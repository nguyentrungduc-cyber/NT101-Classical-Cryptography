# Task 2.4 — SEED Lab: Sự lan truyền lỗi (Error Propagation)
# ============================================================
# Tạo 1000 byte, mã hóa AES-128, làm hỏng 1 bit tại byte thứ 26,
# giải mã và quan sát bao nhiêu khối bị ảnh hưởng theo từng mode.
#
# Cài đặt: pip install pycryptodome
# Chạy: python task2_4_error_propagation.py

from Crypto.Cipher import AES
from Crypto.Util.Padding import pad, unpad

KEY   = b'1234567890123456'   # AES-128
IV    = b'abcdefghijklmnop'   # 16 bytes

# Dữ liệu 1000 byte cố định để dễ so sánh giữa các mode
DATA  = bytes(range(256)) * 3 + bytes(range(232))  # 1000 bytes


def flip_bit(data: bytes, byte_index: int, bit_position: int = 0) -> bytes:
    """Đảo 1 bit tại vị trí byte_index trong data."""
    arr = bytearray(data)
    arr[byte_index] ^= (1 << bit_position)
    return bytes(arr)


def count_different_bytes(original: bytes, corrupted: bytes) -> int:
    return sum(1 for a, b in zip(original, corrupted) if a != b)


def analyze_mode(mode_name: str, encrypt_fn, decrypt_fn, data: bytes):
    """
    Mã hóa data, làm hỏng 1 bit ở byte 26 của bản mã,
    giải mã và báo cáo số khối bị ảnh hưởng.
    """
    # Mã hóa
    ct = encrypt_fn(data)

    # Làm hỏng 1 bit tại byte thứ 26 (0-indexed)
    ct_corrupted = flip_bit(ct, byte_index=26, bit_position=0)

    # Giải mã bản mã bị lỗi
    try:
        pt_corrupted = decrypt_fn(ct_corrupted)
    except Exception as e:
        pt_corrupted = b'\x00' * len(data)  # unpad có thể fail với ECB/CBC

    # So sánh byte bị hỏng (chỉ so sánh trong vùng mã hóa được)
    min_len = min(len(data), len(pt_corrupted))
    diff_bytes = count_different_bytes(data[:min_len], pt_corrupted[:min_len])
    block_size = 16

    # Byte 26 nằm trong khối nào? (0-indexed)
    corrupted_block = 26 // block_size  # = khối 1 (khối thứ 2, 0-indexed)

    # Đếm số block bị ảnh hưởng
    affected_blocks = set()
    for i in range(min_len):
        if i < len(pt_corrupted) and data[i] != pt_corrupted[i]:
            affected_blocks.add(i // block_size)

    print(f"  Mode: {mode_name}")
    print(f"  Byte bị flip: #26 → thuộc khối {corrupted_block} (khối thứ {corrupted_block+1})")
    print(f"  Số byte bản rõ bị sai: {diff_bytes}")
    print(f"  Khối bị ảnh hưởng: {sorted(affected_blocks)} ({len(affected_blocks)} khối)")
    print()

    return sorted(affected_blocks)


# --- ECB ---
def encrypt_ecb(data):
    cipher = AES.new(KEY, AES.MODE_ECB)
    return cipher.encrypt(pad(data, 16))

def decrypt_ecb(ct):
    cipher = AES.new(KEY, AES.MODE_ECB)
    try:
        return unpad(cipher.decrypt(ct), 16)
    except:
        return cipher.decrypt(ct)


# --- CBC ---
def encrypt_cbc(data):
    cipher = AES.new(KEY, AES.MODE_CBC, iv=IV)
    return cipher.encrypt(pad(data, 16))

def decrypt_cbc(ct):
    cipher = AES.new(KEY, AES.MODE_CBC, iv=IV)
    try:
        return unpad(cipher.decrypt(ct), 16)
    except:
        return cipher.decrypt(ct)


# --- CFB ---
def encrypt_cfb(data):
    cipher = AES.new(KEY, AES.MODE_CFB, iv=IV, segment_size=128)
    return cipher.encrypt(data)

def decrypt_cfb(ct):
    cipher = AES.new(KEY, AES.MODE_CFB, iv=IV, segment_size=128)
    return cipher.decrypt(ct)


# --- OFB ---
def encrypt_ofb(data):
    cipher = AES.new(KEY, AES.MODE_OFB, iv=IV)
    return cipher.encrypt(data)

def decrypt_ofb(ct):
    cipher = AES.new(KEY, AES.MODE_OFB, iv=IV)
    return cipher.decrypt(ct)


def main():
    print("=" * 60)
    print("Task 2.4 — Sự lan truyền lỗi theo Mode of Operation")
    print("=" * 60)
    print(f"\nDữ liệu: {len(DATA)} bytes")
    print(f"AES-128, key={KEY.hex()}")
    print(f"Lỗi: flip bit 0 của byte #26 trong bản mã")
    print(f"Byte #26 nằm trong khối 1 (khối thứ 2, byte 16-31)\n")

    modes = [
        ("ECB", encrypt_ecb, decrypt_ecb),
        ("CBC", encrypt_cbc, decrypt_cbc),
        ("CFB", encrypt_cfb, decrypt_cfb),
        ("OFB", encrypt_ofb, decrypt_ofb),
    ]

    results = {}
    for name, enc, dec in modes:
        affected = analyze_mode(name, enc, dec, DATA)
        results[name] = affected

    print("=" * 60)
    print("Nhận xét tổng hợp")
    print("=" * 60)
    print("""
ECB (Electronic Code Book):
  - Mỗi khối mã hóa ĐỘC LẬP → lỗi ở khối i CHỈ ảnh hưởng khối i
  - Lỗi 1 bit ở bản mã làm hỏng đúng 1 khối bản rõ tương ứng

CBC (Cipher Block Chaining):
  - Khối i phụ thuộc vào bản mã của khối i-1 khi giải mã
  - Lỗi ở bản mã khối i ảnh hưởng: khối i (hoàn toàn hỏng) +
    khối i+1 (hỏng đúng bit tương ứng)
  - Tổng: 2 khối bị ảnh hưởng

CFB (128-bit):
  - Tương tự CBC: lỗi lan truyền sang khối kế tiếp
  - Ảnh hưởng 2 khối

OFB (Output FeedBack):
  - Keystream tạo ra ĐỘC LẬP với bản mã → lỗi KHÔNG LAN TRUYỀN
  - Lỗi 1 bit ở bản mã → CHỈ sai đúng 1 bit tương ứng ở bản rõ
  - Ưu điểm trong môi trường truyền thông có nhiễu (satellite, radio)
""")


if __name__ == "__main__":
    main()
