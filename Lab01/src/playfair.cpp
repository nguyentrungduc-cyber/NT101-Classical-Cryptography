/**
 * Task 2.4 — Playfair Cipher
 * ============================================
 * Mã hóa/giải mã theo cặp ký tự (digraph) dùng bảng khóa 5x5.
 * Quy tắc I/J gộp chung 1 ô (bảng chuẩn 26 chữ cái - 1 = 25 ô vừa đủ 5x5).
 *
 * Quy tắc mã hóa cho từng cặp (a, b):
 *   - Cùng hàng   -> lấy chữ bên PHẢI mỗi chữ (vòng về đầu hàng nếu ở cuối)
 *   - Cùng cột    -> lấy chữ bên DƯỚI mỗi chữ (vòng về đầu cột nếu ở cuối)
 *   - Khác hàng/cột (hình chữ nhật) -> đổi chéo: lấy chữ cùng hàng với mình,
 *     cùng cột với chữ kia
 * Giải mã: làm ngược lại (trái/trên thay vì phải/dưới), quy tắc chữ nhật giữ nguyên.
 *
 * Compile: g++ -std=c++17 -O2 -o playfair playfair.cpp
 * Run:
 *   ./playfair encrypt <key> "<plaintext>"
 *   ./playfair decrypt <key> "<ciphertext>"
 */

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <cctype>
#include <set>
#include <fstream>
#include <sstream>

using Matrix = std::array<std::array<char, 5>, 5>;

// ---- Đọc input: nếu là file tồn tại thì đọc file, không thì coi là chuỗi trực tiếp ----
std::string readInput(const std::string& arg) {
    std::ifstream file(arg);
    if (file.good()) {
        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }
    return arg;
}

// ---- Xây ma trận 5x5 từ khóa: khóa trước (bỏ trùng), sau đó điền phần chữ còn lại ----
// Quy ước: J được gộp vào I (chuẩn Playfair phổ biến)
Matrix buildMatrix(const std::string& key) {
    std::string cleanKey;
    std::set<char> used;

    for (char c : key) {
        if (!std::isalpha(static_cast<unsigned char>(c))) continue;
        char upper = std::toupper(static_cast<unsigned char>(c));
        if (upper == 'J') upper = 'I'; // gộp J vào I
        if (used.find(upper) == used.end()) {
            cleanKey += upper;
            used.insert(upper);
        }
    }
    for (char c = 'A'; c <= 'Z'; c++) {
        if (c == 'J') continue; // bỏ qua J vì đã gộp vào I
        if (used.find(c) == used.end()) {
            cleanKey += c;
            used.insert(c);
        }
    }

    Matrix m{};
    for (int i = 0; i < 25; i++) {
        m[i / 5][i % 5] = cleanKey[i];
    }
    return m;
}

void printMatrix(const Matrix& m) {
    std::cout << "Ma tran Playfair 5x5:\n";
    std::cout << "+---+---+---+---+---+\n";
    for (int r = 0; r < 5; r++) {
        std::cout << "|";
        for (int c = 0; c < 5; c++) std::cout << " " << m[r][c] << " |";
        std::cout << "\n+---+---+---+---+---+\n";
    }
}

// Tìm vị trí (hàng, cột) của 1 chữ cái trong ma trận
std::pair<int, int> findPos(const Matrix& m, char c) {
    if (c == 'J') c = 'I';
    for (int r = 0; r < 5; r++)
        for (int col = 0; col < 5; col++)
            if (m[r][col] == c) return {r, col};
    return {-1, -1}; // không xảy ra nếu input đã chuẩn hóa đúng
}

// ---- Chuẩn hóa bản rõ: bỏ ký tự không phải chữ, chuyển hoa, gộp J->I,
// chèn 'X' giữa 2 chữ giống nhau liền kề, thêm 'X' cuối nếu số ký tự lẻ ----
std::string prepareText(const std::string& text) {
    std::string clean;
    for (char c : text) {
        if (std::isalpha(static_cast<unsigned char>(c))) {
            char upper = std::toupper(static_cast<unsigned char>(c));
            if (upper == 'J') upper = 'I';
            clean += upper;
        }
    }

    std::string prepared;
    for (size_t i = 0; i < clean.size(); i++) {
        prepared += clean[i];
        if (i + 1 < clean.size() && clean[i] == clean[i + 1]) {
            prepared += 'X'; // chèn X để tách cặp chữ giống nhau
        }
    }
    if (prepared.size() % 2 != 0) prepared += 'X'; // đệm cuối nếu số ký tự lẻ

    return prepared;
}

std::string playfairEncrypt(const std::string& plaintext, const Matrix& m) {
    std::string prepared = prepareText(plaintext);
    std::string result;

    for (size_t i = 0; i < prepared.size(); i += 2) {
        char a = prepared[i], b = prepared[i + 1];
        auto [ra, ca] = findPos(m, a);
        auto [rb, cb] = findPos(m, b);

        if (ra == rb) {
            // Cùng hàng: lấy chữ bên phải (vòng về đầu hàng)
            result += m[ra][(ca + 1) % 5];
            result += m[rb][(cb + 1) % 5];
        } else if (ca == cb) {
            // Cùng cột: lấy chữ bên dưới (vòng về đầu cột)
            result += m[(ra + 1) % 5][ca];
            result += m[(rb + 1) % 5][cb];
        } else {
            // Hình chữ nhật: đổi chéo cột
            result += m[ra][cb];
            result += m[rb][ca];
        }
    }
    return result;
}

std::string playfairDecrypt(const std::string& ciphertext, const Matrix& m) {
    std::string result;

    for (size_t i = 0; i + 1 < ciphertext.size(); i += 2) {
        char a = ciphertext[i], b = ciphertext[i + 1];
        auto [ra, ca] = findPos(m, a);
        auto [rb, cb] = findPos(m, b);

        if (ra == rb) {
            // Cùng hàng: lấy chữ bên trái (vòng ngược về cuối hàng)
            result += m[ra][(ca + 4) % 5];
            result += m[rb][(cb + 4) % 5];
        } else if (ca == cb) {
            // Cùng cột: lấy chữ bên trên (vòng ngược về cuối cột)
            result += m[(ra + 4) % 5][ca];
            result += m[(rb + 4) % 5][cb];
        } else {
            // Hình chữ nhật: đổi chéo cột (giống hệt lúc mã hóa)
            result += m[ra][cb];
            result += m[rb][ca];
        }
    }
    return result;
}

void printUsage() {
    std::cout << "Su dung:\n"
              << "  playfair encrypt <key> <plaintext_hoac_file>\n"
              << "  playfair decrypt <key> <ciphertext_hoac_file>\n";
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printUsage();
        return 1;
    }

    std::string mode = argv[1];
    std::string key = argv[2];
    std::string input = readInput(argv[3]);

    Matrix m = buildMatrix(key);
    printMatrix(m);
    std::cout << "\n";

    if (mode == "encrypt") {
        std::string prepared = prepareText(input);
        std::cout << "Ban ro sau chuan hoa: " << prepared << "\n";
        std::cout << "Ciphertext: " << playfairEncrypt(input, m) << "\n";
    }
    else if (mode == "decrypt") {
        // Chuẩn hóa ciphertext (bỏ ký tự lạ, xuống dòng...) trước khi giải mã
        std::string clean;
        for (char c : input) {
            if (std::isalpha(static_cast<unsigned char>(c))) {
                clean += std::toupper(static_cast<unsigned char>(c));
            }
        }
        std::cout << "Plaintext: " << playfairDecrypt(clean, m) << "\n";
        std::cout << "(Luu y: ket qua co the con lan 'X' dem them tu buoc chuan hoa ban ro\n"
                  << "luc ma hoa — can loai bo thu cong khi doc ket qua cuoi cung.)\n";
    }
    else {
        printUsage();
        return 1;
    }

    return 0;
}
