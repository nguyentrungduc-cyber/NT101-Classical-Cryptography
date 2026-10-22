#!/bin/bash
# Nhiệm vụ 2.3 – Tạo MD5 Collision với md5collgen
# NT101 – An toàn Mạng máy tính

DATA_DIR="$(dirname "$0")/../data/task2_3"
PREFIX="$DATA_DIR/prefix.txt"
OUT1="$DATA_DIR/out1.bin"
OUT2="$DATA_DIR/out2.bin"

# ---------- Bài 1: prefix KHÔNG phải bội số của 64 ----------
echo "[*] Tạo prefix không phải bội số của 64 (ví dụ: 10 bytes)..."
# TODO: tạo prefix 10 bytes và chạy md5collgen, quan sát kết quả

# ---------- Bài 2: prefix đúng 64 bytes ----------
echo "[*] Tạo prefix 64 bytes..."
# TODO: tạo prefix 64 bytes
# dd if=/dev/urandom bs=1 count=64 of="$PREFIX"

echo "[*] Chạy md5collgen..."
# md5collgen -p "$PREFIX" -o "$OUT1" "$OUT2"

echo "[*] Kiểm tra diff..."
# diff "$OUT1" "$OUT2"

echo "[*] Kiểm tra MD5..."
# md5sum "$OUT1"
# md5sum "$OUT2"

echo "[*] Dump hex out1..."
# xxd "$OUT1"
