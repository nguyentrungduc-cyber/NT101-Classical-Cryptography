"""
Nhiệm vụ 2.2 – Tính chất hàm băm: MD5 và SHA-1 Collision
NT101 – An toàn Mạng máy tính
"""

import hashlib


MSG1_HEX = (
    "d131dd02c5e6eec4693d9a0698aff95c2fcab58712467eab4004583eb8fb7f89"
    "55ad340609f4b30283e488832571415a085125e8f7cdc99fd91dbdf280373c5b"
    "d8823e3156348f5bae6dacd436c919c6dd53e2b487da03fd02396306d248cda0"
    "e99f33420f577ee8ce54b67080a80d1ec69821bcb6a8839396f9652b6ff72a70"
)

MSG2_HEX = (
    "d131dd02c5e6eec4693d9a0698aff95c2fcab50712467eab4004583eb8fb7f89"
    "55ad340609f4b30283e4888325f1415a085125e8f7cdc99fd91dbd7280373c5b"
    "d8823e3156348f5bae6dacd436c919c6dd53e23487da03fd02396306d248cda0"
    "e99f33420f577ee8ce54b67080280d1ec69821bcb6a8839396f965ab6ff72a70"
)


def count_different_bytes(hex1: str, hex2: str) -> int:
    """Đếm số byte khác nhau giữa hai chuỗi hex."""
    # TODO
    pass


def md5_of_hex(hex_str: str) -> str:
    """Tính MD5 từ chuỗi hex (bytes)."""
    # TODO
    pass


def sha1_of_file(filepath: str) -> str:
    """Tính SHA-1 của một file."""
    # TODO
    pass


if __name__ == "__main__":
    # --- Bài 1: So sánh hai message hex ---
    diff = count_different_bytes(MSG1_HEX, MSG2_HEX)
    print(f"Số byte khác nhau: {diff}")

    md5_1 = md5_of_hex(MSG1_HEX)
    md5_2 = md5_of_hex(MSG2_HEX)
    print(f"MD5 Message1: {md5_1}")
    print(f"MD5 Message2: {md5_2}")
    print(f"MD5 giống nhau: {md5_1 == md5_2}")

    # --- Bài 2: MD5 của hello và erase (file thực thi) ---
    # TODO: tính MD5 của file hello và erase, in kết quả

    # --- Bài 3: SHA-1 của shattered-1.pdf và shattered-2.pdf ---
    # TODO: tính SHA-1 của hai file PDF, in kết quả
