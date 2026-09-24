/**
 * Task 2.5 / 2.6 — Vigenère Cipher & Phá mã tự động
 * ============================================
 * 2.5: Mã hóa/giải mã Vigenère
 *      Ci = (pi + ki mod m) mod 26
 *      pi = (Ci - ki mod m + 26) mod 26
 *
 * 2.6: Phá mã khi không biết khóa:
 *      Bước 1 - Ước lượng độ dài khóa bằng Kasiski Examination (tìm các chuỗi
 *               con lặp lại trong ciphertext, khoảng cách giữa chúng là bội số
 *               của độ dài khóa) kết hợp Index of Coincidence để xác nhận.
 *      Bước 2 - Với mỗi độ dài khóa ứng viên, tách ciphertext thành các nhóm
 *               ký tự theo từng vị trí khóa, rồi dùng phân tích tần suất
 *               (chi-squared, giống Caesar) để suy ra từng ký tự của khóa.
 *
 * Compile: g++ -std=c++17 -O2 -o vigenere vigenere.cpp
 * Run:
 *   ./vigenere encrypt <key> "<plaintext>"
 *   ./vigenere decrypt <key> "<ciphertext>"
 *   ./vigenere crack <ciphertext_hoac_file>
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <cctype>
#include <cmath>
#include <algorithm>

static const double ENGLISH_FREQ[26] = {
    8.2, 1.5, 2.8, 4.3, 12.7, 2.2, 2.0, 6.1, 7.0, 0.15, 0.77, 4.0, 2.4,
    6.7, 7.5, 1.9, 0.095, 6.0, 6.3, 9.1, 2.8, 0.98, 2.4, 0.15, 2.0, 0.074
};

std::string readInput(const std::string& arg) {
    std::ifstream file(arg);
    if (file.good()) {
        std::stringstream ss;
        ss << file.rdbuf();
        return ss.str();
    }
    return arg;
}

std::string normalize(const std::string& text) {
    std::string result;
    for (char c : text) {
        if (std::isalpha(static_cast<unsigned char>(c))) {
            result += static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        }
    }
    return result;
}

// ============================================
// TASK 2.5: Mã hóa / Giải mã
// ============================================
std::string vigenereEncrypt(const std::string& plaintext, const std::string& key) {
    std::string cleanKey = normalize(key);
    std::string result;
    int ki = 0;

    for (char c : plaintext) {
        if (std::isupper(static_cast<unsigned char>(c))) {
            int shift = cleanKey[ki % cleanKey.size()] - 'A';
            result += static_cast<char>('A' + (c - 'A' + shift) % 26);
            ki++;
        } else if (std::islower(static_cast<unsigned char>(c))) {
            int shift = cleanKey[ki % cleanKey.size()] - 'A';
            result += static_cast<char>('a' + (c - 'a' + shift) % 26);
            ki++;
        } else {
            result += c; // giữ nguyên ký tự không phải chữ cái, KHÔNG tăng ki
        }
    }
    return result;
}

std::string vigenereDecrypt(const std::string& ciphertext, const std::string& key) {
    std::string cleanKey = normalize(key);
    std::string result;
    int ki = 0;

    for (char c : ciphertext) {
        if (std::isupper(static_cast<unsigned char>(c))) {
            int shift = cleanKey[ki % cleanKey.size()] - 'A';
            result += static_cast<char>('A' + ((c - 'A' - shift) % 26 + 26) % 26);
            ki++;
        } else if (std::islower(static_cast<unsigned char>(c))) {
            int shift = cleanKey[ki % cleanKey.size()] - 'A';
            result += static_cast<char>('a' + ((c - 'a' - shift) % 26 + 26) % 26);
            ki++;
        } else {
            result += c;
        }
    }
    return result;
}

// ============================================
// TASK 2.6: Phá mã tự động
// ============================================

// ---- Index of Coincidence: đo độ "lệch" phân bố chữ cái so với ngẫu nhiên.
// IC của tiếng Anh thật ~0.067, của văn bản ngẫu nhiên ~0.038.
// Dùng để chọn độ dài khóa: tách text theo độ dài khóa ứng viên, nếu IC trung
// bình các nhóm gần 0.067 thì khóa đó có khả năng đúng (mỗi nhóm gần giống
// Caesar cipher đơn, tức đơn bảng, IC cao).
double indexOfCoincidence(const std::string& text) {
    int counts[26] = {0};
    for (char c : text) counts[c - 'A']++;

    int n = text.size();
    if (n <= 1) return 0.0;

    double sum = 0.0;
    for (int i = 0; i < 26; i++) {
        sum += counts[i] * (counts[i] - 1);
    }
    return sum / (n * (n - 1));
}

// Kasiski Examination: tìm các chuỗi con độ dài >=3 lặp lại, ghi nhận khoảng
// cách giữa các lần xuất hiện — độ dài khóa thực sự thường là ước số chung
// của phần lớn các khoảng cách này.
std::vector<int> kasiskiDistances(const std::string& text, int seqLen = 3) {
    std::map<std::string, std::vector<int>> positions;
    for (size_t i = 0; i + seqLen <= text.size(); i++) {
        positions[text.substr(i, seqLen)].push_back(i);
    }

    std::vector<int> distances;
    for (auto& [seq, pos] : positions) {
        if (pos.size() < 2) continue;
        for (size_t i = 1; i < pos.size(); i++) {
            distances.push_back(pos[i] - pos[0]);
        }
    }
    return distances;
}

int gcd(int a, int b) { return b == 0 ? a : gcd(b, a % b); }

// Ước lượng độ dài khóa: kết hợp Kasiski (ước số chung của các khoảng cách)
// với Index of Coincidence (xác nhận bằng cách thử trực tiếp)
int estimateKeyLength(const std::string& text, int maxLen = 20) {
    // Bước 1: Kasiski — đếm tần suất mỗi ước số xuất hiện trong các khoảng cách
    auto distances = kasiskiDistances(text);
    std::map<int, int> factorVotes;

    for (int d : distances) {
        for (int factor = 2; factor <= maxLen; factor++) {
            if (d % factor == 0) factorVotes[factor]++;
        }
    }

    std::cout << "=== Kasiski Examination: phieu bau cho tung do dai khoa ung vien ===\n";
    std::vector<std::pair<int, int>> sortedVotes(factorVotes.begin(), factorVotes.end());
    std::sort(sortedVotes.begin(), sortedVotes.end(), [](auto& a, auto& b) { return a.second > b.second; });
    for (size_t i = 0; i < std::min<size_t>(5, sortedVotes.size()); i++) {
        std::cout << "  Do dai " << sortedVotes[i].first << ": " << sortedVotes[i].second << " phieu\n";
    }

    // Bước 2: Xác nhận bằng Index of Coincidence cho vài ứng viên hàng đầu
    std::cout << "\n=== Xac nhan bang Index of Coincidence (IC tieng Anh that ~0.067) ===\n";
    int bestLen = 1;
    double bestICDiff = 1e9;

    int candidatesToCheck = std::min<size_t>(5, sortedVotes.size());
    for (int i = 0; i < candidatesToCheck; i++) {
        int len = sortedVotes[i].first;
        double avgIC = 0.0;
        for (int offset = 0; offset < len; offset++) {
            std::string group;
            for (size_t j = offset; j < text.size(); j += len) group += text[j];
            avgIC += indexOfCoincidence(group);
        }
        avgIC /= len;
        std::cout << "  Do dai " << len << ": IC trung binh = " << avgIC << "\n";

        double diff = std::abs(avgIC - 0.067);
        if (diff < bestICDiff) {
            bestICDiff = diff;
            bestLen = len;
        }
    }

    return bestLen;
}

// Chi-squared cho 1 nhóm ký tự với 1 shift cụ thể — dùng để tìm từng ký tự khóa
double chiSquaredForShift(const std::string& group, int shift) {
    int counts[26] = {0};
    for (char c : group) {
        int shifted = ((c - 'A') - shift + 26) % 26;
        counts[shifted]++;
    }

    int total = group.size();
    if (total == 0) return 1e9;

    double chiSquared = 0.0;
    for (int i = 0; i < 26; i++) {
        double observed = counts[i];
        double expected = ENGLISH_FREQ[i] / 100.0 * total;
        if (expected > 0) chiSquared += (observed - expected) * (observed - expected) / expected;
    }
    return chiSquared;
}

std::string findKey(const std::string& text, int keyLen) {
    std::string key;
    for (int offset = 0; offset < keyLen; offset++) {
        std::string group;
        for (size_t j = offset; j < text.size(); j += keyLen) group += text[j];

        double bestScore = 1e18;
        int bestShift = 0;
        for (int shift = 0; shift < 26; shift++) {
            double score = chiSquaredForShift(group, shift);
            if (score < bestScore) {
                bestScore = score;
                bestShift = shift;
            }
        }
        key += static_cast<char>('A' + bestShift);
    }
    return key;
}

void autoCrack(const std::string& raw) {
    std::string ciphertext = normalize(raw);
    if (ciphertext.size() < 20) {
        std::cout << "Canh bao: ciphertext qua ngan, ket qua phan tich co the khong dang tin cay.\n";
    }

    int keyLen = estimateKeyLength(ciphertext);
    std::cout << "\n=== Do dai khoa duoc chon: " << keyLen << " ===\n\n";

    std::string key = findKey(ciphertext, keyLen);
    std::cout << "Khoa tim duoc: " << key << "\n\n";

    std::string plaintext = vigenereDecrypt(ciphertext, key);
    std::cout << "Ban ro: " << plaintext << "\n";
}

void printUsage() {
    std::cout << "Su dung:\n"
              << "  vigenere encrypt <key> <plaintext>\n"
              << "  vigenere decrypt <key> <ciphertext>\n"
              << "  vigenere crack <ciphertext_hoac_file>\n";
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printUsage();
        return 1;
    }

    std::string mode = argv[1];

    if (mode == "encrypt" && argc >= 4) {
        std::cout << "Ciphertext: " << vigenereEncrypt(argv[3], argv[2]) << "\n";
    }
    else if (mode == "decrypt" && argc >= 4) {
        std::cout << "Plaintext: " << vigenereDecrypt(argv[3], argv[2]) << "\n";
    }
    else if (mode == "crack" && argc >= 3) {
        autoCrack(readInput(argv[2]));
    }
    else {
        printUsage();
        return 1;
    }

    return 0;
}
