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

**Kết quả (đo bằng `verify_accuracy.py`, đối chiếu ký tự-với-ký tự với
plaintext gốc `task2.3_testcase2_plaintext_ground_truth.txt`):**

Chạy 5 lần liên tiếp (chương trình dùng `std::random_device`, không seed
cố định, nên mỗi lần chạy kết quả có thể khác nhau):

| Lần chạy | Độ chính xác |
|---|---|
| 1 | 93.8% |
| 2 | 93.8% |
| 3 | 93.6% |
| 4 | 91.9% |
| 5 | 91.6% |

**Dao động trung bình: 91.6% – 93.8%** (dùng số liệu này, không dùng
đánh giá chủ quan "đọc hiểu được bằng mắt" — vì đọc bằng mắt dễ đánh giá
cao hơn thực tế do não người tự "sửa" các lỗi nhỏ khi đọc).

Ví dụ 1 lần chạy cho bản rõ:
> THE QUICK WROJN FOZYULPS OVER THE MABXDOG THIS SENTENCE CONTAINS
> EVERX METTER OF THE AMPHAWET AND IS COLLONMX USED FOR TESTING...

Các lỗi lặp lại nhiều nhất là hoán đổi nhầm giữa **B↔V/W**, **L↔R**,
**X↔Y**, **M↔Z** — đều là các chữ cái có tần suất tiếng Anh thấp
(dưới 2%), khiến bảng bigram không đủ tín hiệu thống kê để phân biệt.

**Cách tái tạo kết quả này:**
```bash
./substitution < data/task2.3_testcase2_long_random_key.txt > /tmp/result.txt
python3 data/verify_accuracy.py data/task2.3_testcase2_plaintext_ground_truth.txt /tmp/result.txt
```

---

## Testcase 3 — `task2.3_testcase3_too_short.txt`

**Mô tả:** Văn bản cực ngắn "MEET ME AT NOON" (15 ký tự), mã hóa bằng
khóa hoán vị ngẫu nhiên.

**Kết quả:** ❌ Sai hoàn toàn — bản rõ ra vô nghĩa.

**Nguyên nhân:** Với 15 ký tự chỉ tạo ra 14 cặp bigram — quá ít để
thống kê có ý nghĩa. Đây là giới hạn dưới rõ ràng của phương pháp.

---

## Bảng tổng hợp

| Testcase | Độ dài (ký tự chữ) | Loại khóa | Độ chính xác |
|---|---|---|---|
| 1 | ~180 | Caesar (dịch đều) | Không đo được bằng % (không có ground truth từng ký tự sẵn), quan sát bằng mắt: đọc hiểu kém |
| 2 | ~420 | Hoán vị ngẫu nhiên | **91.6% – 93.8%** (đo qua 5 lần chạy, xem chi tiết ở trên) |
| 3 | 15 | Hoán vị ngẫu nhiên | Sai hoàn toàn, không đọc hiểu được |

## Nhận xét về hiệu quả phương pháp

Hiệu quả của thuật toán hill-climbing + bigram scoring phụ thuộc chủ
yếu vào **độ dài văn bản**, không phụ thuộc nhiều vào loại khóa (Caesar
hay hoán vị ngẫu nhiên đều dùng chung 1 cơ chế giải mã bên dưới, vì
Caesar chỉ là trường hợp đặc biệt của substitution cipher tổng quát).

- Dưới ~50 ký tự: không đủ dữ liệu thống kê, kết quả gần như ngẫu nhiên
- Trên ~400 ký tự: khôi phục được **91.6% – 93.8%** ký tự đúng (đo định
  lượng, không phải ước lượng bằng mắt), đủ để đọc hiểu nội dung tổng
  thể, nhưng KHÔNG đạt 100%
- Sai lệch tập trung ở các chữ cái tần suất tiếng Anh thấp (dưới 2%):
  B, V, W, L, R, X, Y, M, Z — do các chữ này không đủ số lần xuất hiện
  để bảng bigram phân biệt chính xác

**Về tính ngẫu nhiên:** vì chương trình dùng `std::random_device` để
xáo trộn khóa ban đầu cho random restart (không seed cố định), **mỗi
lần chạy cho kết quả hơi khác nhau** — dao động 91.6% đến 93.8% qua
5 lần thử với cùng 1 ciphertext. Đây là đặc tính vốn có của thuật toán
ngẫu nhiên (metaheuristic), không phải lỗi hay sự không nhất quán của
chương trình.

**Phương pháp đo:** dùng script `verify_accuracy.py` so sánh ký tự-với-
ký tự với ground truth đã biết trước, thay vì chỉ đọc bằng mắt — đọc
bằng mắt có xu hướng đánh giá cao hơn thực tế vì não người tự động "sửa"
các lỗi chính tả nhỏ khi đọc câu có ngữ cảnh, dẫn đến nhận định sai lệch
như "gần hoàn hảo" trong khi số liệu thực tế cho thấy vẫn còn ~6-8% ký
tự sai.

Đây là hạn chế đã được biết đến của phương pháp thống kê n-gram khi áp
dụng cho corpus (kích thước mẫu) nhỏ — không phải lỗi cài đặt thuật
toán, mà là bản chất thống kê của bài toán.
