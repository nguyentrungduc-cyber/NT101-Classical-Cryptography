"""
Nhiệm vụ 2.4 – Xác minh thủ công chứng chỉ X.509 bằng RSA
NT101 – An toàn Mạng máy tính

Quy trình:
  1. Lấy chứng chỉ máy chủ (c0.pem) và chứng chỉ CA (c1.pem)
  2. Trích xuất public key (e, n) từ c1.pem
  3. Trích xuất chữ ký từ c0.pem
  4. Trích xuất phần thân (TBS) của c0.pem
  5. Xác minh: RSA_decrypt(signature, e, n) == SHA256(TBS)

Lệnh OpenSSL chuẩn bị (chạy terminal trước):
  openssl s_client -connect <domain>:443 -showcerts > /tmp/certs.txt
  # Tách c0.pem (chứng chỉ server) và c1.pem (chứng chỉ CA)
  openssl x509 -in c0.pem -text          # xem thông tin + chữ ký
  openssl x509 -in c1.pem -noout -modulus # lấy n
  openssl asn1parse -i -in c0.pem -strparse 4 -out data/task2_4/c0_body.bin -noout
"""

import hashlib
import subprocess


DATA_DIR = "data/task2_4"
C0_PEM  = f"{DATA_DIR}/c0.pem"   # chứng chỉ server
C1_PEM  = f"{DATA_DIR}/c1.pem"   # chứng chỉ CA (intermediate)
C0_BODY = f"{DATA_DIR}/c0_body.bin"


# ── Bước 2: Trích xuất public key của CA ──────────────────────────────────────

def extract_ca_public_key(c1_pem: str) -> tuple[int, int]:
    """
    Trả về (e, n) từ chứng chỉ CA.
    e thường là 65537 (0x10001).
    """
    # TODO: dùng subprocess gọi openssl x509 để lấy modulus và exponent
    # Gợi ý:
    #   n_hex = subprocess.check_output(["openssl", "x509", "-in", c1_pem, "-noout", "-modulus"])
    #   text  = subprocess.check_output(["openssl", "x509", "-in", c1_pem, "-text", "-noout"])
    #   parse e từ dòng "Exponent: 65537 (0x10001)"
    e = None
    n = None
    return e, n


# ── Bước 3: Trích xuất chữ ký từ c0.pem ──────────────────────────────────────

def extract_signature(c0_pem: str) -> int:
    """
    Trả về chữ ký dưới dạng số nguyên.
    """
    # TODO: parse output của `openssl x509 -in c0.pem -text`,
    #       tìm block "Signature Algorithm: sha256WithRSAEncryption",
    #       ghép hex bytes lại → int
    pass


# ── Bước 4: Trích xuất TBS và tính hash ───────────────────────────────────────

def extract_tbs_hash(c0_body_bin: str) -> bytes:
    """
    Đọc c0_body.bin (TBS certificate) và trả về SHA-256 hash.
    (File được tạo bởi: openssl asn1parse -strparse 4 -out c0_body.bin)
    """
    # TODO: đọc file binary, tính sha256
    pass


# ── Bước 5: Xác minh bằng RSA ─────────────────────────────────────────────────

def rsa_verify(signature: int, e: int, n: int, tbs_hash: bytes) -> bool:
    """
    Giải mã chữ ký: EM = signature^e mod n
    Bỏ padding PKCS#1 v1.5, so sánh hash bên trong với tbs_hash.
    """
    # TODO:
    #   1. em_int = pow(signature, e, n)
    #   2. chuyển em_int → bytes (big-endian, độ dài = len(n) bytes)
    #   3. parse PKCS#1 v1.5: 0x00 || 0x01 || PS(0xFF...) || 0x00 || DigestInfo || hash
    #   4. so sánh hash được trích xuất với tbs_hash
    pass


# ── Main ───────────────────────────────────────────────────────────────────────

if __name__ == "__main__":
    print("=== Nhiệm vụ 2.4: Xác minh chứng chỉ X.509 ===\n")

    # Bước 2
    e, n = extract_ca_public_key(C1_PEM)
    print(f"CA Public Key:\n  e = {e}\n  n = {n}\n")

    # Bước 3
    sig = extract_signature(C0_PEM)
    print(f"Chữ ký (int): {sig}\n")

    # Bước 4
    tbs_hash = extract_tbs_hash(C0_BODY)
    print(f"SHA-256(TBS): {tbs_hash.hex()}\n")

    # Bước 5
    valid = rsa_verify(sig, e, n, tbs_hash)
    if valid:
        print("✅ Chứng chỉ HỢP LỆ – chữ ký khớp!")
    else:
        print("❌ Chứng chỉ KHÔNG HỢP LỆ – chữ ký sai!")
