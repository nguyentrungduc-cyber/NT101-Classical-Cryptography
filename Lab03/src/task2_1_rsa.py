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