/**
 * Task 2.1 — Caesar Cipher
 * ============================================
 * Mã hóa/giải mã bằng Caesar cipher, và brute-force khi chỉ biết ciphertext.
 *
 * Công thức:
 *   Mã hóa:  C = (p + k) mod 26
 *   Giải mã: p = (C - k) mod 26
 *
 * Compile: g++ -std=c++17 -O2 -o caesar caesar.cpp
 * Run:
 *   ./caesar encrypt <key> "<plaintext>"
 *   ./caesar decrypt <key> "<ciphertext>"
 *   ./caesar bruteforce "<ciphertext>"
 */

#include <iostream>
#include <string>
#include <cctype>
#include <map>
#include <algorithm>

// ---- Bảng tần suất chữ cái tiếng Anh chuẩn (%) — dùng để đánh giá kết quả brute-force ----
// Nguồn: tần suất trung bình các chữ cái trong văn bản tiếng Anh
static const double ENGLISH_FREQ[26] = {
    8.2, 1.5, 2.8, 4.3, 12.7, 2.2, 2.0, 6.1, 7.0, 0.15, 0.77, 4.0, 2.4,
    6.7, 7.5, 1.9, 0.095, 6.0, 6.3, 9.1, 2.8, 0.98, 2.4, 0.15, 2.0, 0.074
};

// Mã hóa/giải mã 1 ký tự với dịch chuyển shift (dùng chung cho encrypt/decrypt)
char shiftChar(char c, int shift) {
    if (std::isupper(static_cast<unsigned char>(c))) {
        return static_cast<char>('A' + ((c - 'A' + shift) % 26 + 26) % 26);
    }
    if (std::islower(static_cast<unsigned char>(c))) {
        return static_cast<char>('a' + ((c - 'a' + shift) % 26 + 26) % 26);
    }
    return c; // giữ nguyên ký tự không phải chữ cái (khoảng trắng, dấu câu...)
}

std::string caesarEncrypt(const std::string& plaintext, int key) {
    std::string result = plaintext;
    for (char& c : result) c = shiftChar(c, key);
    return result;
}

std::string caesarDecrypt(const std::string& ciphertext, int key) {
    return caesarEncrypt(ciphertext, -key); // giải mã = mã hóa với dịch chuyển ngược
}

// Chi-squared: đo độ "giống tiếng Anh" của 1 văn bản dựa trên tần suất chữ cái.
// Điểm càng THẤP thì văn bản càng giống tiếng Anh thật (khớp phân bố chuẩn).
double chiSquaredScore(const std::string& text) {
    int counts[26] = {0};
    int total = 0;

    for (char c : text) {
        if (std::isalpha(static_cast<unsigned char>(c))) {
            counts[std::tolower(static_cast<unsigned char>(c)) - 'a']++;
            total++;
        }
    }
    if (total == 0) return 1e9;

    double chiSquared = 0.0;
    for (int i = 0; i < 26; i++) {
        double observed = counts[i];
        double expected = ENGLISH_FREQ[i] / 100.0 * total;
        if (expected > 0) {
            chiSquared += (observed - expected) * (observed - expected) / expected;
        }
    }
    return chiSquared;
}

// Brute-force: thử toàn bộ 26 khóa, chọn khóa cho ra văn bản "giống tiếng Anh nhất"
void bruteForce(const std::string& ciphertext) {
    double bestScore = 1e18;
    int bestKey = 0;
    std::string bestPlain;

    std::cout << "=== Ket qua thu tung khoa (Chi-squared score - cang thap cang giong tieng Anh) ===\n";
    for (int key = 0; key < 26; key++) {
        std::string candidate = caesarDecrypt(ciphertext, key);
        double score = chiSquaredScore(candidate);

        std::cout << "Key=" << key << "\tScore=" << score;
        if (score < bestScore) {
            bestScore = score;
            bestKey = key;
            bestPlain = candidate;
            std::cout << "  <-- tot nhat hien tai";
        }
        std::cout << "\n";
    }

    std::cout << "\n=== KET QUA DUY NHAT (khoa dung nhat) ===\n";
    std::cout << "Khoa: " << bestKey << "\n";
    std::cout << "Ban ro: " << bestPlain << "\n";
}

void printUsage() {
    std::cout << "Su dung:\n"
              << "  caesar encrypt <key> <plaintext>\n"
              << "  caesar decrypt <key> <ciphertext>\n"
              << "  caesar bruteforce <ciphertext>\n";
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printUsage();
        return 1;
    }

    std::string mode = argv[1];

    if (mode == "encrypt" && argc >= 4) {
        int key = std::stoi(argv[2]);
        std::string plaintext = argv[3];
        std::cout << "Ciphertext: " << caesarEncrypt(plaintext, key) << "\n";
    }
    else if (mode == "decrypt" && argc >= 4) {
        int key = std::stoi(argv[2]);
        std::string ciphertext = argv[3];
        std::cout << "Plaintext: " << caesarDecrypt(ciphertext, key) << "\n";
    }
    else if (mode == "bruteforce" && argc >= 3) {
        std::string ciphertext = argv[2];
        bruteForce(ciphertext);
    }
    else {
        printUsage();
        return 1;
    }

    return 0;
}
