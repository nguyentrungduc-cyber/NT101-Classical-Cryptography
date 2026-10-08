# Lab 02 — Mật mã học Hiện đại (Modern Cryptography)

**Môn học:** An toàn Mạng máy tính (NT101) — UIT
**Ngôn ngữ:** Python 3.x

## Mục tiêu

Làm quen với mật mã khối (DES, AES), các chế độ hoạt động (ECB, CBC, CFB, OFB, CTR),
hiệu ứng thác đổ, và mật mã khóa công khai (RSA — chuẩn bị qua bài toán số học).

## Cài đặt

```bash
cd Lab02
pip install -r requirements.txt
```

## Cấu trúc thư mục

```
Lab02/
├── README.md
├── requirements.txt
└── src/
    ├── task2_1_feistel.py          # Task 2.1: Feistel Cipher & lan truyền thay đổi
    ├── task2_2_modes.py            # Task 2.2: AES — ECB, CBC, CFB, OFB, CTR
    ├── task2_3_avalanche.py        # Task 2.3: Avalanche Effect với DES
    ├── task2_4_error_propagation.py # Task 2.4: SEED Lab — lỗi lan truyền theo mode
    ├── task2_5_compare_des_aes.py   # Task 2.5: So sánh DES / 3DES / AES
    └── task2_6_mersenne.py          # Task 2.6: Mersenne Prime, GCD, lũy thừa modulo
```

## Cách chạy từng nhiệm vụ

```bash
cd Lab02/src
python task2_1_feistel.py
python task2_2_modes.py
python task2_3_avalanche.py
python task2_4_error_propagation.py
python task2_5_compare_des_aes.py
python task2_6_mersenne.py
```

## Danh sách nhiệm vụ

| Nhiệm vụ | File | Nội dung | Trạng thái |
|---|---|---|---|
| 2.1 | `task2_1_feistel.py` | Feistel 4 vòng, quan sát avalanche effect theo từng vòng | ✅ |
| 2.2 | `task2_2_modes.py` | AES tất cả 5 mode, so sánh ECB (lộ cấu trúc) vs CBC/CFB/OFB/CTR | ✅ |
| 2.3 | `task2_3_avalanche.py` | DES avalanche: "STAYHOME" vs "STAYHOMA", đổi key theo MSSV | ✅ |
| 2.4 | `task2_4_error_propagation.py` | SEED Lab Task 5: flip 1 bit byte #26, so sánh 4 mode | ✅ |
| 2.5 | `task2_5_compare_des_aes.py` | Lý thuyết + benchmark: DES / 3DES / AES, tại sao không dùng 2DES | ✅ |
| 2.6 | `task2_6_mersenne.py` | Số nguyên tố Mersenne, Miller-Rabin, GCD Euclid, lũy thừa modulo | ✅ |

## Kết quả nổi bật khi chạy

**Task 2.1:** M1=0xAB vs M2=0xAC (khác 1 bit) → C1 vs C2 khác **6/8 bit (75%)** sau 4 vòng Feistel.

**Task 2.2:** Bản rõ `UIT_LAB_UIT_LAB_...` (lặp lại):
- ECB: Khối 1 == Khối 2 (⚠️ lộ cấu trúc)
- CBC/CFB/OFB/CTR: Khối 1 ≠ Khối 2 ✅

**Task 2.3:** DES với key `87654321` — "STAYHOME" vs "STAYHOMA" (khác 1 bit):
→ Bản mã khác nhau **37/64 bit (57.8%)** — xấp xỉ lý tưởng 50%.

**Task 2.4:** Flip 1 bit ở byte #26 của bản mã:
- ECB: 1 khối bị ảnh hưởng
- CBC/CFB: 2 khối bị ảnh hưởng (khối chứa lỗi + khối tiếp theo)
- OFB: chỉ 1 byte bị sai (lỗi KHÔNG lan truyền)

**Task 2.6:** 7^40 mod 19 = **7** | 10 số nguyên tố lớn nhất < M10 (2^89-1) đã liệt kê đầy đủ.
