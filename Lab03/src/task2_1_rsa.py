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

print("p1 = 11, q1 = 17, e1 = 7 ")
pu, pr = tim_khoa(11, 17, 7)
print("Public Key: ",pu,"Private Key:", pr)