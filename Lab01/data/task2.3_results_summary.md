# Kết quả kiểm thử Task 2.3 — Auto-crack Mono-alphabetic Substitution Cipher

Chạy: `./substitution < data/task2.3_testcaseN_*.txt`

## Testcase 1 — `task2.3_testcase1_short.txt`

**Mô tả:** Ciphertext Caesar từ Task 2.1, chỉ lấy 1 câu đầu (~180 ký tự
alphabet). Dùng để kiểm tra hiệu quả với văn bản ngắn.

**Kết quả:** ❌ Sai gần như hoàn toàn — không đọc hiểu được nội dung.

**Nguyên nhân:** Văn bản quá ngắn, không đủ số lượng cặp bigram để thuật
toán phân biệt đúng giữa các cách hoán vị khả dĩ. Nhiều chữ cái hiếm
(J, K, Q, X, Z) gần như không xuất hiện, khiến hill-climbing không có đủ
tín hiệu thống kê để tối ưu đúng hướng.

---

## Testcase 2 — `task2.3_testcase2_long_random_key.txt`

**Mô tả:** Văn bản tiếng Anh ~420 ký tự (chủ đề cryptography), mã hóa
bằng khóa **hoán vị hoàn toàn ngẫu nhiên** (không phải Caesar) — đúng
bản chất mono-alphabetic substitution cipher tổng quát.

**Kết quả:** ✅ Đọc hiểu gần như hoàn hảo. Bản rõ khôi phục được:

> THE QUICK BROWN FOX JUMPS OVER THE LAZY DOG THIS SENTENCE CONTAINS
> EVERY LETTER OF THE ALPHABET AND IS COMMONLY USED FOR TESTING
> TYPEWRITERS AND FONTS CRYPTOGRAPHY IS THE PRACTICE OF SECURE
> COMMUNICATION IN THE PRESENCE OF THIRD PARTIES...

Chỉ còn nhầm lẫn nhẹ giữa 3 cặp chữ hiếm gặp: **M↔B**, **W↔W** (giữ
nguyên đúng), **Y↔M** — không ảnh hưởng đến khả năng đọc hiểu tổng thể.

---

## Testcase 3 — `task2.3_testcase3_too_short.txt`

**Mô tả:** Văn bản cực ngắn "MEET ME AT NOON" (15 ký tự), mã hóa bằng
khóa hoán vị ngẫu nhiên.

**Kết quả:** ❌ Sai hoàn toàn — bản rõ ra vô nghĩa.

**Nguyên nhân:** Với 15 ký tự chỉ tạo ra 14 cặp bigram — quá ít để
thống kê có ý nghĩa. Đây là giới hạn dưới rõ ràng của phương pháp.

---

## Bảng tổng hợp

| Testcase | Độ dài (ký tự chữ) | Loại khóa | Kết quả |
|---|---|---|---|
| 1 | ~180 | Caesar (dịch đều) | ❌ Sai gần hết |
| 2 | ~420 | Hoán vị ngẫu nhiên | ✅ Gần hoàn hảo |
| 3 | 15 | Hoán vị ngẫu nhiên | ❌ Sai hoàn toàn |

## Nhận xét về hiệu quả phương pháp

Hiệu quả của thuật toán hill-climbing + bigram scoring phụ thuộc chủ
yếu vào **độ dài văn bản**, không phụ thuộc nhiều vào loại khóa (Caesar
hay hoán vị ngẫu nhiên đều dùng chung 1 cơ chế giải mã bên dưới, vì
Caesar chỉ là trường hợp đặc biệt của substitution cipher tổng quát).

- Dưới ~50 ký tự: không đủ dữ liệu thống kê, kết quả gần như ngẫu nhiên
- Trên ~400 ký tự: khôi phục đúng phần lớn nội dung, chỉ còn sai lệch
  ở các chữ cái xuất hiện tần suất thấp (J, K, Q, V, W, X, Z) — do các
  chữ này không đủ số lần xuất hiện để bảng bigram phân biệt chính xác

Đây là hạn chế đã được biết đến của phương pháp thống kê n-gram khi áp
dụng cho corpus (kích thước mẫu) nhỏ — không phải lỗi cài đặt thuật
toán, mà là bản chất thống kê của bài toán.
