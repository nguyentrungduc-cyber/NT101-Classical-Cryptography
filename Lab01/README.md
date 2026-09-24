# Lab 01 — Mật mã học Cổ điển (Classical Cryptography)

**Môn học:** An toàn Mạng máy tính (NT101) — UIT
**Ngôn ngữ:** C++ (C++17)

## Mục tiêu

Làm quen với các thuật toán mật mã cổ điển (mật mã thay thế và mật mã hoán vị),
thực hành mã hóa/giải mã, và thực hiện thám mã (cryptanalysis) khi chỉ biết
ciphertext.

## Cấu trúc thư mục

```
Lab01/
├── README.md                              # File này
├── Makefile                                # Build tất cả chương trình bằng `make`
├── src/
│   ├── caesar.cpp                          # Task 2.1: Caesar cipher
│   ├── substitution.cpp                    # Task 2.2/2.3: Mono-alphabetic + Frequency Analysis
│   ├── playfair.cpp                        # Task 2.4: Playfair cipher
│   ├── vigenere.cpp                        # Task 2.5/2.6: Vigenère cipher + phá mã
│   └── railfence.cpp                       # Task 2.7: Rail Fence cipher (mở rộng)
└── data/
    ├── task2.1_caesar_ciphertext.txt       # Ciphertext đề bài cung cấp
    ├── task2.4_playfair_ciphertext.txt
    └── task2.6_vigenere_ciphertext.txt
```

## Hướng dẫn build & chạy

### Build tất cả chương trình

```bash
cd Lab01
make
```

Sẽ tạo ra 5 file thực thi: `caesar`, `substitution`, `playfair`, `vigenere`, `railfence`.

### Build 1 chương trình riêng lẻ

```bash
make caesar
```

### Dọn dẹp file thực thi

```bash
make clean
```

### Build thủ công (không dùng Makefile)

```bash
g++ -std=c++17 -O2 -o caesar src/caesar.cpp
```

---

## Task 2.1 — Caesar Cipher

```bash
./caesar encrypt <key> "<plaintext>"
./caesar decrypt <key> "<ciphertext>"
./caesar bruteforce "<ciphertext>"
```

**Ví dụ:**
```bash
./caesar encrypt 3 "HELLO WORLD"
./caesar bruteforce "$(cat data/task2.1_caesar_ciphertext.txt)"
```

Brute-force thử toàn bộ 26 khóa, dùng **Chi-squared statistic** so với bảng tần
suất chữ cái tiếng Anh chuẩn để tự động chọn ra khóa đúng — không cần người
dùng tự nhìn bằng mắt.

**Kết quả đã verify với ciphertext đề bài:** Key=19, bản rõ nói về UIT (VNU-HCM).

---

## Task 2.2 / 2.3 — Mono-alphabetic Substitution & Frequency Analysis

```bash
./substitution               < ciphertext.txt       # mặc định = crack, dùng được ngay với CPH
./substitution freq          < ciphertext.txt       # Task 2.2
./substitution crack         < ciphertext.txt       # Task 2.3 (tường minh)
```

**Task 2.2** in ra bảng tần suất chữ cái của ciphertext (sắp xếp giảm dần),
đối chiếu với bảng tần suất tiếng Anh chuẩn — hỗ trợ phân tích thủ công theo
đúng yêu cầu đề bài (mô tả từng bước đưa ra giả thuyết, kiểm tra, điều chỉnh).

**Task 2.3** tự động giải mã bằng thuật toán:
1. Đoán khóa ban đầu bằng cách khớp tần suất ciphertext với tần suất tiếng Anh
2. **Hill-climbing**: thử hoán đổi từng cặp ký tự trong khóa, giữ lại nếu điểm
   đánh giá "giống tiếng Anh" (dựa trên **bảng bigram** — tần suất các cặp chữ
   cái liên tiếp) cải thiện
3. **Random restart** 25 lần để tránh bị kẹt ở kết quả cục bộ tối ưu (local
   optimum) không phải đáp án đúng toàn cục

**Lưu ý về độ chính xác:** với văn bản dài (~230 ký tự trở lên như ciphertext
trong đề bài), thuật toán cho kết quả gần đúng hoàn toàn, có thể còn nhầm lẫn
vài ký tự hiếm gặp (J, K, Q, X, Z) cần điều chỉnh thủ công dựa trên ngữ cảnh —
đây là đặc điểm chung của phương pháp hill-climbing, không phải lỗi cài đặt.

**3 testcase kiểm thử độ dài văn bản khác nhau:**
```bash
./substitution < data/task2.3_testcase1_short.txt              # ngắn (~180 ký tự) — kém
./substitution < data/task2.3_testcase2_long_random_key.txt    # dài (~420 ký tự) — gần hoàn hảo
./substitution < data/task2.3_testcase3_too_short.txt           # rất ngắn (15 ký tự) — sai hoàn toàn
```
Xem [`data/task2.3_results_summary.md`](./data/task2.3_results_summary.md) để biết
kết quả chi tiết và nhận xét đầy đủ về hiệu quả phương pháp (dùng cho báo cáo).

**Đo độ chính xác khách quan** (khuyên dùng thay vì chỉ đọc bằng mắt — đọc bằng
mắt dễ đánh giá cao hơn thực tế):
```bash
./substitution < data/task2.3_testcase2_long_random_key.txt > /tmp/result.txt
python3 data/verify_accuracy.py data/task2.3_testcase2_plaintext_ground_truth.txt /tmp/result.txt
```
Script so sánh ký tự-với-ký tự với plaintext gốc đã biết trước, in ra % chính xác
và liệt kê các vị trí sai đầu tiên.

---

## Task 2.4 — Playfair Cipher

```bash
./playfair encrypt <key> "<plaintext>"
./playfair decrypt <key> "<ciphertext>"
```

Chương trình luôn in ra **ma trận 5×5** tương ứng với khóa đã nhập (theo đúng
yêu cầu đề bài). Quy ước: J được gộp chung với I.

**Ví dụ với ciphertext đề bài (khóa "Harry Potter"):**
```bash
./playfair decrypt "Harry Potter" "$(cat data/task2.4_playfair_ciphertext.txt | tr -d '\n')"
```
Kết quả: đoạn văn bản giới thiệu về series tiểu thuyết Harry Potter.

**Đã verify roundtrip:** encrypt → decrypt cho ra đúng 100% bản rõ đã chuẩn hóa.

---

## Task 2.5 / 2.6 — Vigenère Cipher

```bash
./vigenere encrypt <key> "<plaintext>"                      # Task 2.5
./vigenere decrypt <key> "<ciphertext>"                      # Task 2.5
./vigenere crack <ciphertext_hoặc_đường_dẫn_file>            # Task 2.6
```

**Task 2.6** phá mã khi không biết khóa, gồm 2 bước:

1. **Ước lượng độ dài khóa:**
   - *Kasiski Examination*: tìm các chuỗi con ≥3 ký tự lặp lại trong ciphertext,
     tính khoảng cách giữa các lần xuất hiện — độ dài khóa thật thường là ước
     số chung của phần lớn các khoảng cách này
   - *Index of Coincidence (IC)*: xác nhận lại bằng cách thử tách ciphertext
     theo từng độ dài khóa ứng viên; IC trung bình gần 0.067 (đặc trưng tiếng
     Anh) thì độ dài đó khả năng đúng cao

2. **Tìm từng ký tự khóa:** tách ciphertext thành các nhóm theo từng vị trí
   khóa (giống hệt Caesar cipher đơn với mỗi nhóm), áp dụng lại kỹ thuật
   Chi-squared của Task 2.1 để tìm shift đúng cho mỗi nhóm

**Kết quả đã verify với ciphertext đề bài:** tự động tìm ra khóa **"HCMUIT"**,
bản rõ giải mã đúng 100%, không sai một ký tự nào (nội dung về cryptography
trong khoa học máy tính).

---

## Task 2.7 — Mở rộng: Rail Fence Cipher

```bash
./railfence encrypt <num_rails> "<plaintext>"
./railfence decrypt <num_rails> "<ciphertext>"
./railfence bruteforce "<ciphertext>"
```

**Nguyên lý:** Rail Fence là **mật mã hoán vị** (transposition cipher) — khác
hẳn với 3 thuật toán trên (đều là mật mã thay thế/substitution). Bản rõ được
viết theo đường zíc-zắc xuống-lên trên N hàng ("rail"), ciphertext là kết quả
đọc lần lượt từng hàng một.

**Ví dụ đã verify khớp 100% với ví dụ chuẩn trong giáo trình mật mã học:**
```bash
./railfence encrypt 3 "WEAREDISCOVEREDFLEEATONCE"
# => WECRLTEERDSOEEFEAOCAIVDEN
```

**Brute-force:** vì số khóa khả dĩ (số hàng) chỉ giới hạn từ 2 đến độ dài văn
bản, việc thử hết toàn bộ khóa rất nhanh — không cần thuật toán tối ưu phức
tạp như Vigenère. Chương trình có cảnh báo khi ciphertext quá ngắn (<40 ký
tự) vì phương pháp đếm từ tiếng Anh phổ biến dễ bị nhầm lẫn với dữ liệu ít.

---

## Tổng hợp kỹ thuật thám mã đã sử dụng

| Thuật toán | Kỹ thuật thám mã |
|---|---|
| Caesar | Brute-force toàn bộ 26 khóa + Chi-squared statistic |
| Mono-alphabetic Substitution | Frequency analysis (thủ công) + Hill-climbing với bigram scoring (tự động) |
| Vigenère | Kasiski Examination + Index of Coincidence (tìm độ dài khóa) + Chi-squared (tìm từng ký tự khóa) |
| Rail Fence | Brute-force toàn bộ số hàng khả dĩ + đếm từ tiếng Anh phổ biến |

## Môi trường đã test

- Compiler: `g++` với chuẩn C++17
- Hệ điều hành: Linux (Ubuntu)
- Không phụ thuộc thư viện ngoài — chỉ dùng C++ Standard Library
