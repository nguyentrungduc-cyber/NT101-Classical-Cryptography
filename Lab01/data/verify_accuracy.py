#!/usr/bin/env python3
"""
verify_accuracy.py — Đo % chính xác của kết quả substitution crack
so với plaintext gốc đã biết trước (ground truth).

Dùng để verify khách quan bằng số liệu, thay vì chỉ đọc bằng mắt
(đọc bằng mắt dễ đánh giá quá cao vì não người tự "sửa" lỗi nhỏ).

Cách dùng:
    ./substitution < data/task2.3_testcase2_long_random_key.txt > /tmp/result.txt
    python3 data/verify_accuracy.py data/task2.3_testcase2_plaintext_ground_truth.txt /tmp/result.txt
"""

import sys
import re


def normalize(text: str) -> str:
    """Chỉ giữ chữ cái, chuyển thành in hoa — khớp cách chương trình C++ xử lý."""
    return re.sub(r"[^A-Za-z]", "", text).upper()


def extract_plaintext_from_output(raw: str) -> str:
    """Trích dòng 'Ban ro (uoc luong tot nhat):' ra khỏi output đầy đủ của chương trình."""
    lines = raw.splitlines()
    for i, line in enumerate(lines):
        if "Ban ro" in line and i + 1 < len(lines):
            return lines[i + 1]
    return raw  # fallback nếu không tìm thấy, coi cả input là bản rõ


def main():
    if len(sys.argv) != 3:
        print("Su dung: python3 verify_accuracy.py <ground_truth.txt> <program_output.txt>")
        sys.exit(1)

    ground_truth_path, output_path = sys.argv[1], sys.argv[2]

    ground_truth = normalize(open(ground_truth_path, encoding="utf-8").read())
    raw_output = open(output_path, encoding="utf-8").read()
    result = normalize(extract_plaintext_from_output(raw_output))

    if len(ground_truth) != len(result):
        print(f"CANH BAO: do dai khac nhau — ground truth={len(ground_truth)}, "
              f"ket qua={len(result)}. So sanh theo do dai ngan hon.")

    n = min(len(ground_truth), len(result))
    matches = sum(1 for i in range(n) if ground_truth[i] == result[i])

    print(f"Do dai so sanh: {n} ky tu")
    print(f"So ky tu khop dung: {matches}")
    print(f"Ty le chinh xac: {100 * matches / n:.1f}%")

    # In ra vài vị trí sai đầu tiên để dễ soi
    print("\nMot vai vi tri sai dau tien (vi tri: dung -> chuong trinh cho):")
    shown = 0
    for i in range(n):
        if ground_truth[i] != result[i] and shown < 15:
            print(f"  [{i}] {ground_truth[i]} -> {result[i]}")
            shown += 1


if __name__ == "__main__":
    main()
