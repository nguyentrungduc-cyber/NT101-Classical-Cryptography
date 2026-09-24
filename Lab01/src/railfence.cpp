/**
 * Task 2.7 — Mở rộng: Rail Fence Cipher
 * ============================================
 * Nguyên lý hoạt động:
 * Rail Fence là một dạng MẬT MÃ HOÁN VỊ (transposition cipher) — khác với
 * Caesar/Substitution/Playfair/Vigenère (đều là mật mã THAY THẾ). Rail Fence
 * không đổi ký tự nào cả, chỉ đổi VỊ TRÍ của chúng.
 *
 * Cách hoạt động: viết bản rõ theo hình zíc-zắc xuống rồi lên trên "num_rails"
 * hàng ngang (giống hàng rào - rail fence), sau đó đọc ciphertext theo TỪNG
 * HÀNG một, từ trái sang phải, từ hàng trên xuống hàng dưới.
 *
 * Ví dụ với "WEAREDISCOVEREDFLEEATONCE", 3 hàng (key=3):
 *   W . . . E . . . C . . . R . . . L . . . T . . . E
 *   . E . R . D . S . O . E . E . F . E . A . O . C .
 *   . . A . . . I . . . V . . . D . . . E . . . N . .
 * Đọc theo hàng: "WECRLTEERDSOEEFEAOCAIVDEN"
 *
 * Giải mã: dựng lại khung zíc-zắc (đánh dấu vị trí nào thuộc hàng nào trước),
 * rồi điền ciphertext vào đúng các vị trí đã đánh dấu theo thứ tự hàng,
 * cuối cùng đọc lại theo đường zíc-zắc để ra bản rõ.
 *
 * Ưu điểm: đơn giản, dễ cài đặt.
 * Nhược điểm: rất yếu — với văn bản đủ dài, số lượng khóa (num_rails) chỉ
 * giới hạn trong khoảng nhỏ (2 đến độ dài văn bản), dễ dàng brute-force toàn
 * bộ khóa để tìm ra bản rõ có nghĩa.
 *
 * Compile: g++ -std=c++17 -O2 -o railfence railfence.cpp
 * Run:
 *   ./railfence encrypt <num_rails> "<plaintext>"
 *   ./railfence decrypt <num_rails> "<ciphertext>"
 *   ./railfence bruteforce "<ciphertext>"
 */

#include <iostream>
#include <string>
#include <vector>
#include <cctype>

// ---- Sinh ra chuỗi chỉ số hàng cho từng vị trí ký tự theo đường zíc-zắc ----
// Ví dụ num_rails=3: 0,1,2,1,0,1,2,1,0,1,2,... (đi xuống rồi đi lên, lặp lại)
std::vector<int> generateRailPattern(int length, int numRails) {
    std::vector<int> pattern(length);
    int row = 0;
    int direction = 1; // 1 = đi xuống, -1 = đi lên

    for (int i = 0; i < length; i++) {
        pattern[i] = row;
        if (row == 0) direction = 1;
        else if (row == numRails - 1) direction = -1;
        row += direction;
    }
    return pattern;
}

std::string railFenceEncrypt(const std::string& plaintext, int numRails) {
    if (numRails <= 1) return plaintext; // 1 hàng thì không đổi gì cả

    auto pattern = generateRailPattern(plaintext.size(), numRails);

    // Gom ký tự theo từng hàng, giữ đúng thứ tự xuất hiện trong bản rõ
    std::vector<std::string> rails(numRails);
    for (size_t i = 0; i < plaintext.size(); i++) {
        rails[pattern[i]] += plaintext[i];
    }

    std::string result;
    for (auto& rail : rails) result += rail;
    return result;
}

std::string railFenceDecrypt(const std::string& ciphertext, int numRails) {
    if (numRails <= 1) return ciphertext;

    int length = ciphertext.size();
    auto pattern = generateRailPattern(length, numRails);

    // Đếm số ký tự thuộc mỗi hàng để biết cắt ciphertext thành từng đoạn ra sao
    std::vector<int> railLengths(numRails, 0);
    for (int r : pattern) railLengths[r]++;

    // Cắt ciphertext thành từng hàng theo đúng độ dài đã đếm được
    std::vector<std::string> rails(numRails);
    int pos = 0;
    for (int r = 0; r < numRails; r++) {
        rails[r] = ciphertext.substr(pos, railLengths[r]);
        pos += railLengths[r];
    }

    // Dùng con trỏ riêng cho từng hàng, điền lại theo đúng thứ tự zíc-zắc ban đầu
    std::vector<int> railPos(numRails, 0);
    std::string result;
    for (int r : pattern) {
        result += rails[r][railPos[r]];
        railPos[r]++;
    }
    return result;
}

// Đếm số từ tiếng Anh phổ biến xuất hiện trong text — dùng để đánh giá
// brute-force result nào "giống tiếng Anh có nghĩa" nhất.
int scoreEnglishness(const std::string& text) {
    static const std::vector<std::string> commonWords = {
        "THE", "AND", "ARE", "FOR", "YOU", "THIS", "THAT", "WITH", "FROM",
        "HAVE", "WAS", "NOT", "BUT", "ALL", "CAN", "HER", "HIS", "ONE"
    };
    int score = 0;
    for (auto& word : commonWords) {
        size_t pos = 0;
        while ((pos = text.find(word, pos)) != std::string::npos) {
            score++;
            pos += word.size();
        }
    }
    return score;
}

// Brute-force: Rail Fence chỉ có tối đa (độ dài văn bản - 1) khóa khả dĩ
// -> thử hết rất nhanh, không cần thuật toán tối ưu phức tạp như Vigenère.
void bruteForce(const std::string& ciphertext) {
    int maxRails = std::min<int>(ciphertext.size(), 20); // giới hạn hợp lý để in gọn

    if (ciphertext.size() < 40) {
        std::cout << "CANH BAO: Ciphertext kha ngan (" << ciphertext.size() << " ky tu).\n"
                  << "Voi van ban ngan, phuong phap dem tu tieng Anh pho bien de bi nham lan\n"
                  << "vi cac ket qua sai cung co the tinh co chua 1-2 tu ngau nhien trung khop.\n"
                  << "Nen uu tien kiem tra bang mat cac ket qua co diem cao thay vi tin tuyet\n"
                  << "doi vao 'ket qua tot nhat' duy nhat ben duoi.\n\n";
    }

    int bestScore = -1;
    int bestRails = 2;
    std::string bestResult;

    std::cout << "=== Thu tat ca so hang tu 2 den " << maxRails << " ===\n";
    for (int rails = 2; rails <= maxRails; rails++) {
        std::string candidate = railFenceDecrypt(ciphertext, rails);
        int score = scoreEnglishness(candidate);

        std::cout << "So hang=" << rails << "  diem=" << score;
        if (score > bestScore) {
            bestScore = score;
            bestRails = rails;
            bestResult = candidate;
            std::cout << "  <-- tot nhat hien tai";
        }
        std::cout << "\n";
    }

    std::cout << "\n=== KET QUA TOT NHAT (theo diem so) ===\n";
    std::cout << "So hang: " << bestRails << "\n";
    std::cout << "Ban ro: " << bestResult << "\n";
}

void printUsage() {
    std::cout << "Su dung:\n"
              << "  railfence encrypt <num_rails> <plaintext>\n"
              << "  railfence decrypt <num_rails> <ciphertext>\n"
              << "  railfence bruteforce <ciphertext>\n";
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printUsage();
        return 1;
    }

    std::string mode = argv[1];

    if (mode == "encrypt" && argc >= 4) {
        int numRails = std::stoi(argv[2]);
        std::cout << "Ciphertext: " << railFenceEncrypt(argv[3], numRails) << "\n";
    }
    else if (mode == "decrypt" && argc >= 4) {
        int numRails = std::stoi(argv[2]);
        std::cout << "Plaintext: " << railFenceDecrypt(argv[3], numRails) << "\n";
    }
    else if (mode == "bruteforce" && argc >= 3) {
        bruteForce(argv[2]);
    }
    else {
        printUsage();
        return 1;
    }

    return 0;
}
