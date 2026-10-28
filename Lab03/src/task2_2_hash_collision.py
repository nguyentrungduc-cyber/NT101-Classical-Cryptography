"""
Nhiệm vụ 2.2 – Tính chất hàm băm: MD5 và SHA-1 Collision
NT101 – An toàn Mạng máy tính
"""

import hashlib

hex1 = (
    "d131dd02c5e6eec4693d9a0698aff95c2fcab58712467eab4004583eb8fb7f8955ad3"
    "40609f4b30283e488832571415a085125e8f7cdc99fd91dbdf280373c5bd8823e315"
    "6348f5bae6dacd436c919c6dd53e2b487da03fd02396306d248cda0e99f33420f577"
    "ee8ce54b67080a80d1ec69821bcb6a8839396f9652b6ff72a70"
)
hex2 = (
    "d131dd02c5e6eec4693d9a0698aff95c2fcab50712467eab4004583eb8fb7f8955ad3"
    "40609f4b30283e4888325f1415a085125e8f7cdc99fd91dbd7280373c5bd8823e315"
    "6348f5bae6dacd436c919c6dd53e23487da03fd02396306d248cda0e99f33420f577e"
    "e8ce54b67080280d1ec69821bcb6a8839396f965ab6ff72a70"
)

m1 = bytes.fromhex(hex1)
m2 = bytes.fromhex(hex2)
print("Do dai:", len(m1), len(m2))

vi_tri = [i for i in range(len(m1)) if m1[i] != m2[i]]
print("So byte khac nhau:", len(vi_tri))
for i in vi_tri:
    print(f"  byte {i}: {m1[i]:02x} -> {m2[i]:02x}")

print("MD5 m1:", hashlib.md5(m1).hexdigest())
print("MD5 m2:", hashlib.md5(m2).hexdigest())
print("MD5 giong nhau:", hashlib.md5(m1).digest() == hashlib.md5(m2).digest())
print("SHA-256 m1:", hashlib.sha256(m1).hexdigest())
print("SHA-256 m2:", hashlib.sha256(m2).hexdigest())

    # --- Bài 2: MD5 của hello và erase (file thực thi) ---
    # TODO: tính MD5 của file hello và erase, in kết quả

    # --- Bài 3: SHA-1 của shattered-1.pdf và shattered-2.pdf ---
    # TODO: tính SHA-1 của hai file PDF, in kết quả
