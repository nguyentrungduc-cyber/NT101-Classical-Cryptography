# Lab 03 – Hàm băm và Chữ ký số

Môn học: **An toàn Mạng máy tính (NT101)**

## Mục tiêu

- Hiểu và phân tích các tính chất bảo mật của hàm băm một chiều và MAC.
- Quan sát tấn công va chạm trên MD5 và SHA-1.
- Lập trình RSA mã hóa/giải mã.
- Xác minh thủ công chứng chỉ số X.509 và chữ ký số RSA.

## Cấu trúc thư mục

```
Lab03/
├── src/
│   ├── task2_1_rsa.py            # Nhiệm vụ 2.1 – Mã hóa RSA
│   ├── task2_2_hash_collision.py # Nhiệm vụ 2.2 – Tính chất hàm băm
│   ├── task2_3_md5collgen.sh     # Nhiệm vụ 2.3 – Tạo MD5 collision
│   └── task2_4_verify_cert.py   # Nhiệm vụ 2.4 – Xác minh X.509 (RSA)
├── data/
│   ├── task2_2/                  # Dữ liệu hex messages, PDF shattered
│   ├── task2_3/                  # Prefix file và output collision
│   └── task2_4/                  # Chứng chỉ c0.pem, c1.pem, body.bin
├── requirements.txt
└── README.md
```

## Cài đặt

```bash
pip install -r requirements.txt
```

## Hướng dẫn chạy

### Nhiệm vụ 2.1 – RSA
```bash
python src/task2_1_rsa.py
```

### Nhiệm vụ 2.2 – Hash Collision Demo
```bash
python src/task2_2_hash_collision.py
```

### Nhiệm vụ 2.3 – MD5 Collision Generation
```bash
# Cài md5collgen trước (Ubuntu/Debian):
# sudo dpkg --add-architecture i386
# sudo apt-get install -y libc6:i386
bash src/task2_3_md5collgen.sh
```

### Nhiệm vụ 2.4 – X.509 Certificate Verification
```bash
# 1. Lấy chứng chỉ từ web server
openssl s_client -connect <domain>:443 -showcerts > certs.txt
# 2. Tách thành c0.pem và c1.pem rồi chạy
python src/task2_4_verify_cert.py
```

## Yêu cầu hệ thống

- Python 3.x
- OpenSSL (CLI)
- `md5collgen` (Nhiệm vụ 2.3)
