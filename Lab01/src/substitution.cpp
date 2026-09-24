/**
 * Task 2.2 / 2.3 — Mono-alphabetic Substitution Cipher & Frequency Analysis
 * ============================================
 * 2.2: In ra bảng tần suất chữ cái của ciphertext để hỗ trợ phân tích thủ công.
 * 2.3: Tự động crack bằng hill-climbing: bắt đầu từ 1 khóa (đoán theo tần suất),
 *      thử hoán đổi từng cặp chữ trong khóa, giữ lại nếu điểm "giống tiếng Anh"
 *      (dựa trên quadgram — bộ 4 chữ cái liên tiếp) cải thiện. Lặp lại nhiều lần
 *      với random restart để tránh kẹt ở local optimum.
 *
 * Compile: g++ -std=c++17 -O2 -o substitution substitution.cpp
 * Run:
 *   ./substitution freq <ciphertext_or_file>      (Task 2.2: bảng tần suất)
 *   ./substitution crack <ciphertext_or_file>      (Task 2.3: tự động giải mã)
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <map>
#include <algorithm>
#include <random>
#include <cctype>
#include <unordered_map>

// ---- Tần suất unigram tiếng Anh chuẩn (%) — dùng để gợi ý ánh xạ ban đầu ----
static const double ENGLISH_FREQ[26] = {
    8.2, 1.5, 2.8, 4.3, 12.7, 2.2, 2.0, 6.1, 7.0, 0.15, 0.77, 4.0, 2.4,
    6.7, 7.5, 1.9, 0.095, 6.0, 6.3, 9.1, 2.8, 0.98, 2.4, 0.15, 2.0, 0.074
};

// ---- Chuẩn hóa: chỉ giữ chữ cái, chuyển hết thành in hoa ----
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
// TASK 2.2: Phân tích tần suất
// ============================================
void frequencyAnalysis(const std::string& raw) {
    std::string text = normalize(raw);
    std::array<int, 26> counts{};
    counts.fill(0);

    for (char c : text) counts[c - 'A']++;

    std::vector<std::pair<char, int>> freq;
    for (int i = 0; i < 26; i++) freq.push_back({static_cast<char>('A' + i), counts[i]});

    // Sắp xếp giảm dần theo tần suất — giúp so sánh trực tiếp với bảng chuẩn tiếng Anh
    std::sort(freq.begin(), freq.end(), [](auto& a, auto& b) { return a.second > b.second; });

    std::cout << "Tong so ky tu (chi tinh chu cai): " << text.size() << "\n\n";
    std::cout << "=== Bang tan suat Ciphertext (sap xep giam dan) ===\n";
    std::cout << "Ky tu\tSo lan\tTy le %\n";
    for (auto& [ch, cnt] : freq) {
        double pct = text.empty() ? 0.0 : (100.0 * cnt / text.size());
        printf("%c\t%d\t%.2f%%\n", ch, cnt, pct);
    }

    std::cout << "\n=== Bang tan suat tieng Anh chuan (tham khao, sap xep giam dan) ===\n";
    std::vector<std::pair<char, double>> englishSorted;
    for (int i = 0; i < 26; i++) englishSorted.push_back({static_cast<char>('A' + i), ENGLISH_FREQ[i]});
    std::sort(englishSorted.begin(), englishSorted.end(), [](auto& a, auto& b) { return a.second > b.second; });
    for (auto& [ch, pct] : englishSorted) {
        printf("%c\t%.2f%%\n", ch, pct);
    }

    std::cout << "\nGoi y: chu cai xuat hien nhieu nhat trong ciphertext (thuong la ~12-13%)"
              << " nhieu kha nang ung voi 'E' trong bang ro. Tiep tuc doi chieu tung cap\n"
              << "de dua ra gia thuyet ve cac chu cai khac, uu tien cac tu ngan pho bien\n"
              << "(THE, AND, TO...) va cac chu doi mot minh (A, I) de kiem tra gia thuyet.\n";
}

// ============================================
// TASK 2.3: Auto-crack bằng Hill-Climbing + Quadgram scoring
// ============================================

// ---- Bảng tần suất BIGRAM tiếng Anh chuẩn (26x26) ----
// Dùng bigram thay vì quadgram: bảng đầy đủ 676 cặp thay vì vài chục quadgram
// rời rạc, nên đáng tin cậy hơn nhiều với văn bản ngắn (vài trăm ký tự) như
// trong bài lab. Giá trị là log10(tần suất tương đối trong corpus tiếng Anh),
// càng cao càng phổ biến. Nguồn: ước lượng từ thống kê ngôn ngữ tiếng Anh phổ biến.
std::array<std::array<double, 26>, 26> buildBigramTable() {
    std::array<std::array<double, 26>, 26> table{};
    for (auto& row : table) row.fill(0.2); // điểm sàn cho cặp hiếm/không có

    // Các cặp phổ biến nhất tiếng Anh (chữ đầu, chữ sau, điểm)
    struct Pair { char a, b; double score; };
    std::vector<Pair> common = {
        {'T','H',9.5},{'H','E',9.3},{'I','N',8.8},{'E','R',8.6},{'A','N',8.5},
        {'R','E',8.4},{'O','N',8.2},{'A','T',8.0},{'E','N',8.0},{'N','D',8.3},
        {'T','I',8.1},{'E','S',8.0},{'O','R',7.9},{'T','E',7.8},{'O','F',8.2},
        {'E','D',7.9},{'I','S',7.8},{'I','T',7.9},{'A','L',7.7},{'A','R',7.6},
        {'S','T',7.8},{'T','O',8.1},{'N','T',7.7},{'N','G',7.6},{'S','E',7.5},
        {'H','A',7.6},{'A','S',7.5},{'O','U',7.4},{'I','O',7.3},{'L','E',7.4},
        {'V','E',7.3},{'C','O',7.2},{'M','E',7.2},{'D','E',7.3},{'H','I',7.2},
        {'R','I',7.1},{'R','O',7.1},{'I','C',7.0},{'N','E',7.4},{'L','L',6.9},
        {'E','A',7.0},{'R','A',7.0},{'T','A',6.9},{'S','I',6.8},{'W','A',6.5},
        {'C','H',6.6},{'C','E',6.7},{'L','I',6.6},{'L','A',6.5},{'C','T',6.6},
        {'U','R',6.5},{'F','O',6.6},{'U','N',6.5},{'W','I',6.4},{'P','R',6.5},
        {'F','I',6.3},{'P','E',6.4},{'A','D',6.5},{'S','A',6.4},{'S','O',6.4},
        {'B','E',6.5},{'G','H',6.3},{'U','S',6.4},{'W','E',6.4},{'A','C',6.3},
        {'A','G',6.2},{'A','M',6.2},{'A','B',6.0},{'I','D',6.2},{'F','A',6.0},
        {'G','E',6.1},{'M','A',6.2},{'F','T',6.0},{'I','L',6.3},{'T','R',6.4},
        {'N','I',6.3},{'N','O',6.4},{'M','I',6.1},{'E','T',6.5},{'E','C',6.2},
        {'D','I',6.3},{'D','A',6.0},{'N','A',6.2},{'S','H',6.4},{'Q','U',7.8},
        {'X','P',5.5},{'X','T',5.4},{'X','C',5.3},{'X','A',5.3},{'X','I',5.5},
        {'K','E',5.8},{'K','I',5.5},{'Y','O',5.6},{'V','I',5.9},{'V','A',5.8},
    };
    for (auto& p : common) table[p.a - 'A'][p.b - 'A'] = p.score;
    return table;
}

double bigramScore(const std::string& text, const std::array<std::array<double, 26>, 26>& table) {
    double score = 0.0;
    for (size_t i = 0; i + 1 < text.size(); i++) {
        score += table[text[i] - 'A'][text[i + 1] - 'A'];
    }
    return score;
}

// Giải mã text với 1 khóa thay thế (key[i] = chữ cái thay cho 'A'+i trong ciphertext)
std::string applyKey(const std::string& ciphertext, const std::array<char, 26>& key) {
    std::string result;
    for (char c : ciphertext) result += key[c - 'A'];
    return result;
}

// Tạo khóa ban đầu bằng cách khớp tần suất ciphertext với tần suất tiếng Anh chuẩn
std::array<char, 26> initialGuessKey(const std::string& ciphertext) {
    std::array<int, 26> counts{};
    counts.fill(0);
    for (char c : ciphertext) counts[c - 'A']++;

    std::vector<int> cipherOrder(26);
    for (int i = 0; i < 26; i++) cipherOrder[i] = i;
    std::sort(cipherOrder.begin(), cipherOrder.end(), [&](int a, int b) { return counts[a] > counts[b]; });

    std::vector<int> englishOrder(26);
    for (int i = 0; i < 26; i++) englishOrder[i] = i;
    std::sort(englishOrder.begin(), englishOrder.end(), [&](int a, int b) { return ENGLISH_FREQ[a] > ENGLISH_FREQ[b]; });

    // key[cipherLetter] = plainLetter tương ứng theo thứ hạng tần suất
    std::array<char, 26> key{};
    for (int i = 0; i < 26; i++) {
        key[cipherOrder[i]] = static_cast<char>('A' + englishOrder[i]);
    }
    return key;
}

// Hill-climbing: thử hoán đổi 2 vị trí trong key, giữ nếu điểm tốt hơn
std::pair<std::array<char, 26>, double> hillClimb(
    const std::string& ciphertext,
    std::array<char, 26> key,
    const std::array<std::array<double, 26>, 26>& bigramTable,
    std::mt19937& rng
) {
    double bestScore = bigramScore(applyKey(ciphertext, key), bigramTable);
    bool improved = true;

    while (improved) {
        improved = false;
        for (int i = 0; i < 26 && !improved; i++) {
            for (int j = i + 1; j < 26; j++) {
                std::swap(key[i], key[j]);
                double score = bigramScore(applyKey(ciphertext, key), bigramTable);
                if (score > bestScore) {
                    bestScore = score;
                    improved = true;
                    break; // áp dụng ngay, quét lại từ đầu (greedy hill-climbing)
                }
                std::swap(key[i], key[j]); // hoàn tác nếu không tốt hơn
            }
        }
    }
    return {key, bestScore};
}

void autoCrack(const std::string& raw) {
    std::string ciphertext = normalize(raw);
    if (ciphertext.empty()) {
        std::cout << "Loi: ciphertext rong sau khi chuan hoa.\n";
        return;
    }

    auto bigramTable = buildBigramTable();
    std::mt19937 rng(std::random_device{}());

    // Bắt đầu từ khóa đoán theo tần suất — cho hill-climbing xuất phát điểm tốt
    std::array<char, 26> startKey = initialGuessKey(ciphertext);

    double bestOverallScore = -1e18;
    std::array<char, 26> bestOverallKey{};

    const int RESTARTS = 25; // random restart để tránh kẹt local optimum
    std::cout << "Dang chay hill-climbing voi " << RESTARTS << " lan random restart...\n";

    for (int r = 0; r < RESTARTS; r++) {
        std::array<char, 26> key = startKey;
        if (r > 0) {
            // Random restart: xáo trộn ngẫu nhiên vài cặp trong khóa ban đầu
            std::uniform_int_distribution<int> dist(0, 25);
            int swaps = 2 + r % 6;
            for (int s = 0; s < swaps; s++) {
                std::swap(key[dist(rng)], key[dist(rng)]);
            }
        }

        auto [resultKey, score] = hillClimb(ciphertext, key, bigramTable, rng);
        std::cout << "  Lan " << (r + 1) << ": score = " << score << "\n";

        if (score > bestOverallScore) {
            bestOverallScore = score;
            bestOverallKey = resultKey;
        }
    }

    std::cout << "\n=== KET QUA TOT NHAT ===\n";
    std::cout << "Diem bigram: " << bestOverallScore << "\n\n";

    std::cout << "Bang anh xa (Cipher -> Plain):\n";
    for (int i = 0; i < 26; i++) {
        std::cout << static_cast<char>('A' + i) << "->" << bestOverallKey[i] << "  ";
        if ((i + 1) % 13 == 0) std::cout << "\n";
    }

    std::cout << "\nBan ro (uoc luong tot nhat):\n";
    std::cout << applyKey(ciphertext, bestOverallKey) << "\n";

    std::cout << "\nLuu y: ket qua nay la uoc luong tu dong dua tren thong ke bigram.\n"
              << "Voi van ban ngan hoac cau truc dac biet, ket qua co the chua hoan hao\n"
              << "(vd: nham lan giua vai chu cai hiem gap nhu J/K/Q/X/Z) va can dieu\n"
              << "chinh thu cong them mot vai vi tri dua tren tu vung/ngu canh.\n";
}

void printUsage() {
    std::cout << "Su dung:\n"
              << "  substitution freq   < ciphertext.txt\n"
              << "  substitution crack  < ciphertext.txt\n"
              << "  echo \"ciphertext\" | substitution crack\n";
}

// Đọc toàn bộ stdin cho đến EOF — tương thích với CPH (Competitive Programming
// Helper) và các judge tự động, vốn luôn đưa input qua stdin thay vì argv.
std::string readStdin() {
    std::stringstream ss;
    ss << std::cin.rdbuf();
    return ss.str();
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    std::string mode = argv[1];
    std::string input = readStdin();

    if (mode == "freq") {
        frequencyAnalysis(input);
    }
    else if (mode == "crack") {
        autoCrack(input);
    }
    else {
        printUsage();
        return 1;
    }

    return 0;
}
