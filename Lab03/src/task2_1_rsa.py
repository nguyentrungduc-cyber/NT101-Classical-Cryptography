"""
n = p·q
φ = (p−1)(q−1)
Có sẵn e (nguyên tố cùng nhau với φ)
d = e⁻¹ mod φ. Trong Python dùng pow(e, -1, phi)
PU = (e, n), PR = (d, n)
"""

"""Yêu cầu 1: Xác định khóa công khai PU và khóa riêng PR"""

def egcd(e, phi): # Lặp 2 thực chất đệ quy thì e là phi trước đó, phi là du trước đó
    # Tra ve (g, d, k) sao cho: e*d + phi*k = g
    if phi == 0:
        return e, 1, 0
    thuong = e // phi # Lần lặp 2 thực chất là phi // du trước
    du = e % phi
    g, d1, k1 = egcd(phi, du)
    d = k1
    k = d1 - thuong * k1
    return g, d, k


def tim_khoa(p, q, e):
    n = p * q
    phi = (p - 1) * (q - 1)
    g, d, k = egcd(e, phi)
    if g != 1:
        raise ValueError("e khong nguyen to cung nhau voi phi")
    d = d % phi
    return (e, n), (d, n)

#p1 = 11, q1 = 17, e1 = 7
print("p1 = 11, q1 = 17, e1 = 7 ")
pu, pr = tim_khoa(11, 17, 7)
print("Public Key:",pu,"\nPrivate Key:", pr)

"""
p2 = 20079993872842322116151219,
q2 = 676717145751736242170789,
e2 = 17 (hệ thập phân)
"""
print("\np2 = 20079993872842322116151219, \nq2 = 676717145751736242170789, \ne2 = 17 ")
pu, pr = tim_khoa(20079993872842322116151219, 676717145751736242170789, 17)
print("Public Key:",pu,"\nPrivate Key:", pr)

"""
p2 = 20079993872842322116151219,
q2 = 676717145751736242170789,
e2 = 17 (hệ thập phân)
"""

"""
p3 = F7E75FDC469067FFDC4E847C51F452DF,
q3 = E85CED54AF57E53E092113E62F436F4F,
e3 = 0D88C3 (hệ thập lục phân)
"""
print("\np3 = F7E75FDC469067FFDC4E847C51F452DF, \nq3 = E85CED54AF57E53E092113E62F436F4F, \ne3 = 0D88C3 ")
p3 = int("F7E75FDC469067FFDC4E847C51F452DF", 16)
q3 = int("E85CED54AF57E53E092113E62F436F4F", 16)
e3 = int("0D88C3", 16)
pu, pr = tim_khoa(p3, q3, e3)
print("Public Key:",pu,"\nPrivate Key:", pr)



"""
-------------------------------------------------------------------------------
2. Sử dụng khóa được tạo ra từ p1, q1, e1 để mã hóa và giải mã bản rõ M=5 trong
cả hai trường hợp Mã hóa cho tính bảo mật (Encryption for Confidentiality)
và Mã hóa cho tính xác thực (Encryption for Authentication).
-------------------------------------------------------------------------------
"""

def ma_hoa(M, k, n):
    return pow(M, k, n)

e, d, n = 7, 23, 187
M = 5

# Bảo mật: Mã hóa bằng e, giải mã bằng d
C = ma_hoa(M, e, n)
M_lai = ma_hoa(C, d, n)
print("\nBao mat  : C =", C, "| giai ma =", M_lai)

# Xac thuc: ma hoa bang d, giai ma bang e
S = ma_hoa(M, d, n)
M_lai = ma_hoa(S, e, n)
print("\nXac thuc : C =", S, "| giai ma =", M_lai)

""" Yêu cầu 3:
-------------------
Sử dụng các khóa trên để mã hóa thông điệp sau: The University of Information
Technology. Xác định bản mã dưới dạng Base64.
------------------------
"""
import base64

def ma_hoa_chuoi(text, k, n):
    du_lieu = text.encode()                        # chuoi -> mang byte
    byte_moi_khoi = max(1, (n.bit_length() - 1) // 8)
    byte_ban_ma = (n.bit_length() + 7) // 8
    ket_qua = b""
    for i in range(0, len(du_lieu), byte_moi_khoi):
        khoi = du_lieu[i:i + byte_moi_khoi]
        M = int.from_bytes(khoi, "big")            # khoi byte -> so
        C = pow(M, k, n)
        ket_qua += C.to_bytes(byte_ban_ma, "big")  # do dai co dinh
    return base64.b64encode(ket_qua).decode()


def giai_ma_chuoi(b64, k, n):
    byte_ban_ma = (n.bit_length() + 7) // 8
    raw = base64.b64decode(b64)
    ket_qua = b""
    for i in range(0, len(raw), byte_ban_ma):
        C = int.from_bytes(raw[i:i + byte_ban_ma], "big")
        M = pow(C, k, n)
        ket_qua += M.to_bytes((M.bit_length() + 7) // 8, "big")
    return ket_qua.decode()

bo_khoa = {
    "Bo 1": (11, 17, 7),
    "Bo 2": (20079993872842322116151219, 676717145751736242170789, 17),
    "Bo 3": (int("F7E75FDC469067FFDC4E847C51F452DF", 16),
             int("E85CED54AF57E53E092113E62F436F4F", 16),
             int("0D88C3", 16)),
}

thong_diep = "The University of Information Technology"

for ten, (p, q, e) in bo_khoa.items():
    (e, n), (d, _) = tim_khoa(p, q, e)
    bm = ma_hoa_chuoi(thong_diep, e, n)
    xt = ma_hoa_chuoi(thong_diep, d, n)
    print(ten)
    print("  Bao mat :", bm)
    print("    giai ma:", giai_ma_chuoi(bm, d, n))
    print("  Xac thuc:", xt)
    print("    giai ma:", giai_ma_chuoi(xt, e, n))
    print()


"""
--------------------
Yêu cầu 4:
Tìm bản rõ tương ứng của mỗi bản mã sau, biết rằng chúng được mã hóa bằng
một trong ba khóa đã cho ở trên
----------------------"""


def giai_ma_tung_byte(b64, k, n):
    raw = base64.b64decode(b64)                 # Base64 -> mang byte
    ket_qua = []
    for c in raw:
        if c >= n:                              # byte lon hon n thi khong phai khoa nay
            return None
        ket_qua.append(pow(c, k, n))
    return bytes(ket_qua)


e, d, n = 7, 23, 187                            # bo khoa 1 (cau 1)
ban_ma = "Z2BUSkJcg0w4XEpgm0JcMExEQmBlVH6dYEpNTHpMHptMQ7NgTHlgQrNMQ2BKTQ=="

print("Thu giai bang d (khoa rieng):")
print(giai_ma_tung_byte(ban_ma, d, n).decode(errors="replace"))
print()
print("Thu giai bang e (khoa cong khai):")
print(giai_ma_tung_byte(ban_ma, e, n).decode(errors="replace"))