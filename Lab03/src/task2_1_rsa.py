"""
Nhiệm vụ 2.1 – Mã hóa khóa công khai RSA
NT101 – An toàn Mạng máy tính
"""

import math
import base64


def compute_keys(p: int, q: int, e: int) -> tuple:
    """Tính khóa công khai (e, n) và khóa riêng tư (d, n) từ p, q, e."""
    # TODO: tính n, phi(n), d
    pass


def encrypt_confidentiality(M: int, e: int, n: int) -> int:
    """Mã hóa bảo mật: C = M^e mod n"""
    # TODO
    pass


def decrypt_confidentiality(C: int, d: int, n: int) -> int:
    """Giải mã bảo mật: M = C^d mod n"""
    # TODO
    pass


def encrypt_authentication(M: int, d: int, n: int) -> int:
    """Mã hóa xác thực: C = M^d mod n"""
    # TODO
    pass


def decrypt_authentication(C: int, e: int, n: int) -> int:
    """Giải mã xác thực: M = C^e mod n"""
    # TODO
    pass


def encrypt_message_string(message: str, e: int, n: int) -> str:
    """Mã hóa chuỗi văn bản, trả về Base64."""
    # TODO: chuyển từng ký tự → số 2 chữ số → ghép block 4 chữ số → mã hóa RSA
    pass


def decrypt_ciphertext_b64(b64_cipher: str, d: int, n: int) -> str:
    """Giải mã chuỗi Base64, trả về plaintext."""
    # TODO
    pass


if __name__ == "__main__":
    # --- Bộ khóa 1: p1=11, q1=17, e1=7 (thập phân) ---
    p1, q1, e1 = 11, 17, 7
    PU1, PR1 = compute_keys(p1, q1, e1)
    print(f"[Key1] PU = {PU1}  |  PR = {PR1}")

    # Bài 2: mã hóa/giải mã M=5
    M = 5
    # TODO: in kết quả cả hai hướng Confidentiality và Authentication

    # --- Bộ khóa 2 & 3 ---
    p2 = 20079993872842322116151219
    q2 = 676717145751736242170789
    e2 = 17
    PU2, PR2 = compute_keys(p2, q2, e2)
    print(f"[Key2] PU = {PU2}  |  PR = {PR2}")

    p3 = int("F7E75FDC469067FFDC4E847C51F452DF", 16)
    q3 = int("E85CED54AF57E53E092113E62F436F4F", 16)
    e3 = int("0D88C3", 16)
    PU3, PR3 = compute_keys(p3, q3, e3)
    print(f"[Key3] PU = {PU3}  |  PR = {PR3}")

    # Bài 3: mã hóa chuỗi
    msg = "The University of Information Technology"
    # TODO: thử cả 3 bộ khóa, in Base64

    # Bài 4: giải mã các bản mã cho sẵn
    ciphertexts = [
        "raUcesUlOkx/8ZhgodMoo0Uu18sC20yXlQFevSu7W/FDxIy0YRHMyXcHdD9PBvIT2aUft5fCQEGomiVVPv4I",
        "C87F570FC4F699CEC24020C6F54221ABAB2CE0C3",
        "Z2BUSkJcg0w4XEpgm0JcMExEQmBlVH6dYEpNTHpMHptMQ7NgTHlgQrNMQ2BKTQ==",
        "001010000001010011111111101101110010111011001010111011000110011"
        "110111111001111110110100011001111001100001001010001010100111101"
        "010100110011101110111011110101101100000100",
    ]
    # TODO: thử từng bộ khóa, in plaintext tương ứng
