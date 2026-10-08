from Crypto.Cipher import AES

key = b'1234567890123456'
iv  = b'0000000000000000'          # IV cố định 16 byte
plaintext = b"UIT_LAB_UIT_LAB_UIT_LAB_UIT_LAB_"   # 32 byte = 2 khối

def split_blocks(ct):
    return [ct[i:i+16].hex() for i in range(0, len(ct), 16)]

results = {
    "ECB": AES.new(key, AES.MODE_ECB).encrypt(plaintext),
    "CBC": AES.new(key, AES.MODE_CBC, iv=iv).encrypt(plaintext),
    "CFB": AES.new(key, AES.MODE_CFB, iv=iv, segment_size=128).encrypt(plaintext),
    "OFB": AES.new(key, AES.MODE_OFB, iv=iv).encrypt(plaintext),
    "CTR": AES.new(key, AES.MODE_CTR, nonce=b'', initial_value=iv).encrypt(plaintext),
}

for mode, ct in results.items():
    b = split_blocks(ct)
    print(f"{mode}: {ct.hex()}")
    print(f"  Khối 1: {b[0]}")
    print(f"  Khối 2: {b[1]}")
    print(f"  Hai khối giống nhau? {b[0] == b[1]}\n")