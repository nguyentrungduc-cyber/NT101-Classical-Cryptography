"""
Nhiệm vụ 2.4 – Xác minh thủ công chứng chỉ X.509 bằng RSA
NT101 – An toàn Mạng máy tính

Quy trình:
  1. Lấy chứng chỉ máy chủ (c0.pem) và chứng chỉ CA (c1.pem)
  2. Trích xuất public key (e, n) từ c1.pem
  3. Trích xuất chữ ký từ c0.pem
  4. Trích xuất phần thân TBS và tính SHA-256
  5. Xác minh: RSA_decrypt(sig, e, n) → bỏ padding → so sánh hash

Chuẩn bị (chạy trong terminal trước khi chạy script):
  openssl asn1parse -i -in data/task2_4/c0.pem -strparse 4 \
      -out data/task2_4/c0_body.bin -noout
"""

import hashlib
import re
import subprocess


DATA_DIR = "data/task2_4"
C0_PEM   = f"{DATA_DIR}/c0.pem"
C1_PEM   = f"{DATA_DIR}/c1.pem"
C0_BODY  = f"{DATA_DIR}/c0_body.bin"

# DigestInfo prefix cố định cho SHA-256 (RFC 3447)
SHA256_DIGEST_INFO = bytes.fromhex("3031300d060960864801650304020105000420")


# ── Bước 2: Trích xuất public key (e, n) từ chứng chỉ CA ─────────────────────

def extract_ca_public_key(c1_pem: str) -> tuple[int, int]:
    # Lấy modulus n
    out = subprocess.check_output(
        ["openssl", "x509", "-in", c1_pem, "-noout", "-modulus"],
        text=True
    )
    n_hex = out.strip().removeprefix("Modulus=")
    n = int(n_hex, 16)

    # Lấy exponent e
    text = subprocess.check_output(
        ["openssl", "x509", "-in", c1_pem, "-text", "-noout"],
        text=True
    )
    match = re.search(r"Exponent:\s*(\d+)", text)
    e = int(match.group(1))

    return e, n


# ── Bước 3: Trích xuất chữ ký từ c0.pem ──────────────────────────────────────

def extract_signature(c0_pem: str) -> int:
    text = subprocess.check_output(
        ["openssl", "x509", "-in", c0_pem, "-text", "-noout"],
        text=True
    )

    # Lấy phần sau "Signature Value:"
    sig_block = text.split("Signature Value:")[1]

    # Ghép tất cả hex bytes (dạng xx:xx:xx...)
    hex_str = re.sub(r"[^0-9a-fA-F]", "", sig_block)
    return int(hex_str, 16)


# ── Bước 4: Tính SHA-256 của phần thân TBS ────────────────────────────────────

def extract_tbs_hash(c0_body_bin: str) -> bytes:
    with open(c0_body_bin, "rb") as f:
        data = f.read()
    return hashlib.sha256(data).digest()


# ── Bước 5: Xác minh chữ ký RSA + PKCS#1 v1.5 ───────────────────────────────

def rsa_verify(signature: int, e: int, n: int, tbs_hash: bytes) -> bool:
    key_len = (n.bit_length() + 7) // 8  # số bytes của modulus

    # RSA decrypt: EM = sig^e mod n
    em_int   = pow(signature, e, n)
    em_bytes = em_int.to_bytes(key_len, byteorder="big")

    # Kiểm tra cấu trúc PKCS#1 v1.5: 0x00 0x01 [0xFF...] 0x00 DigestInfo hash
    if em_bytes[0] != 0x00 or em_bytes[1] != 0x01:
        print("  [!] Padding không hợp lệ (byte 0-1)")
        return False

    # Tìm vị trí 0x00 kết thúc padding
    sep = em_bytes.find(b"\x00", 2)
    if sep == -1:
        print("  [!] Không tìm thấy byte phân cách 0x00")
        return False

    # Kiểm tra vùng padding toàn 0xFF
    if any(b != 0xFF for b in em_bytes[2:sep]):
        print("  [!] Vùng padding không phải toàn 0xFF")
        return False

    payload = em_bytes[sep + 1:]  # DigestInfo || hash

    # Kiểm tra DigestInfo prefix SHA-256
    if not payload.startswith(SHA256_DIGEST_INFO):
        print("  [!] DigestInfo không khớp SHA-256")
        return False

    extracted_hash = payload[len(SHA256_DIGEST_INFO):]
    return extracted_hash == tbs_hash


# ── Main ──────────────────────────────────────────────────────────────────────

if __name__ == "__main__":
    print("=== Nhiệm vụ 2.4: Xác minh chứng chỉ X.509 ===\n")

    # Bước 2
    e, n = extract_ca_public_key(C1_PEM)
    print(f"[Bước 2] CA Public Key:")
    print(f"  e = {e}")
    print(f"  n = {hex(n)}\n")

    # Bước 3
    sig = extract_signature(C0_PEM)
    print(f"[Bước 3] Chữ ký (hex): {hex(sig)}\n")

    # Bước 4
    tbs_hash = extract_tbs_hash(C0_BODY)
    print(f"[Bước 4] SHA-256(TBS): {tbs_hash.hex()}\n")

    # Bước 5
    print("[Bước 5] Xác minh chữ ký RSA...")
    valid = rsa_verify(sig, e, n, tbs_hash)
    if valid:
        print("\n✅ Chứng chỉ HỢP LỆ – chữ ký khớp!")
    else:
        print("\n❌ Chứng chỉ KHÔNG HỢP LỆ – chữ ký sai!")
