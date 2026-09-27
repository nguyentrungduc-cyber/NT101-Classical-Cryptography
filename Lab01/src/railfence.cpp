/**
 * Task 2.7 — Rail Fence Cipher
 * ============================================
 * Thuat toan ma hoa hoan vi (transposition cipher):
 * khong thay doi gia tri ky tu, chi thay doi VI TRI cua chung.
 *
 * Nguyen ly:
 *  - Chon so "hang rao" (rails) = key, vd key = 3.
 *  - Ghi lan luot tung ky tu cua ban ro theo duong zig-zag
 *    qua cac hang, tu tren xuong duoi roi tu duoi len tren, lap lai:
 *
 *      key = 3, ban ro = "HELLOWORLD"
 *      Hang 0: H . . . O . . . L .
 *      Hang 1: . E . L . W . R . D
 *      Hang 2: . . L . . . O . . .
 *
 *  - Ban ma hoa = doc lan luot tung hang tu tren xuong duoi,
 *    trong moi hang doc tu trai sang phai:
 *      "H O L" + "E L W R D" + "L O"  ->  "HOLELWRDLO"
 *
 *  - Giai ma: lam nguoc lai — xac dinh truoc "khuon" zig-zag
 *    (vi tri nao thuoc hang nao), dem so ky tu moi hang,
 *    cat ciphertext thanh tung doan tuong ung roi "do" lai
 *    vao dung khuon zig-zag do de doc ra ban ro.
 *
 * Compile: g++ -std=c++17 -O2 -o railfence railfence.cpp
 * Run:
 *   ./railfence encrypt <key> "<plaintext>"
 *   ./railfence decrypt <key> "<ciphertext>"
 *   ./railfence demo          (chay vi du minh hoa co san)
 */

#include <iostream>
#include <string>
#include <vector>
using namespace std;

// ---------- MA HOA ----------
string encryptRailFence(const string &text, int numRails) {
    if (numRails <= 1 || (int)text.length() <= numRails) return text;

    vector<string> fence(numRails, "");
    int rail = 0, direction = 1;

    for (char c : text) {
        fence[rail] += c;
        if (rail == 0) direction = 1;
        else if (rail == numRails - 1) direction = -1;
        rail += direction;
    }

    string result = "";
    for (auto &row : fence) result += row;
    return result;
}

// ---------- GIAI MA ----------
string decryptRailFence(const string &cipher, int numRails) {
    int n = cipher.length();
    if (numRails <= 1 || n <= numRails) return cipher;

    // Buoc 1: xac dinh "khuon" zig-zag — vi tri thu i thuoc hang nao
    vector<int> railCuaViTri(n);
    int rail = 0, direction = 1;
    for (int i = 0; i < n; i++) {
        railCuaViTri[i] = rail;
        if (rail == 0) direction = 1;
        else if (rail == numRails - 1) direction = -1;
        rail += direction;
    }

    // Buoc 2: dem so ky tu roi vao moi hang
    vector<int> soLuong(numRails, 0);
    for (int i = 0; i < n; i++) soLuong[railCuaViTri[i]]++;

    // Buoc 3: cat ciphertext thanh tung doan theo dung hang
    vector<string> noiDungHang(numRails);
    int pos = 0;
    for (int r = 0; r < numRails; r++) {
        noiDungHang[r] = cipher.substr(pos, soLuong[r]);
        pos += soLuong[r];
    }

    // Buoc 4: doc lai theo dung thu tu zig-zag ban dau -> ra ban ro
    vector<int> conTro(numRails, 0);
    string ketQua = "";
    rail = 0; direction = 1;
    for (int i = 0; i < n; i++) {
        ketQua += noiDungHang[rail][conTro[rail]];
        conTro[rail]++;
        if (rail == 0) direction = 1;
        else if (rail == numRails - 1) direction = -1;
        rail += direction;
    }
    return ketQua;
}

// ---------- IN MINH HOA DUONG ZIG-ZAG (de nguoi dung de hinh dung) ----------
void inZigZag(const string &text, int numRails) {
    vector<string> fence(numRails, string(text.length(), ' '));
    int rail = 0, direction = 1;
    for (int i = 0; i < (int)text.length(); i++) {
        fence[rail][i] = text[i];
        if (rail == 0) direction = 1;
        else if (rail == numRails - 1) direction = -1;
        rail += direction;
    }
    for (auto &row : fence) cout << row << endl;
}

// ---------- NHAP VA MA HOA / GIAI MA TUONG TAC ----------
void nhapVaMaHoa() {
    string p; int key;
    cout << "Nhap ban ro: ";
    getline(cin, p);
    cout << "Nhap khoa (so hang rao): ";
    cin >> key;
    cin.ignore();

    cout << "\nMinh hoa duong zig-zag:\n";
    inZigZag(p, key);

    string c = encryptRailFence(p, key);
    cout << "\nBan ma hoa: " << c << endl;
}

void nhapVaGiaiMa() {
    string c; int key;
    cout << "Nhap ban ma hoa: ";
    getline(cin, c);
    cout << "Nhap khoa (so hang rao): ";
    cin >> key;
    cin.ignore();

    string p = decryptRailFence(c, key);
    cout << "Ban ro sau khi giai ma: " << p << endl;
}

// ---------- VI DU MINH HOA CHUNG MINH CHUONG TRINH DUNG ----------
void demo() {
    cout << "===== VI DU MINH HOA RAIL FENCE CIPHER =====\n\n";

    string plaintext = "ATTACKATDAWN";
    int key = 3;

    cout << "Ban ro goc : " << plaintext << endl;
    cout << "Khoa (rails): " << key << "\n\n";

    cout << "Duong zig-zag khi ma hoa:\n";
    inZigZag(plaintext, key);

    string cipher = encryptRailFence(plaintext, key);
    cout << "\nBan ma hoa : " << cipher << endl;

    string decrypted = decryptRailFence(cipher, key);
    cout << "Ban ro sau giai ma: " << decrypted << endl;

    cout << "\nKiem tra: " << (decrypted == plaintext ? "DUNG (khop voi ban ro goc)" : "SAI") << endl;

    // Vi du thu 2 voi key khac de chung minh tong quat
    cout << "\n--- Vi du 2 (key = 4) ---\n";
    string pt2 = "DEFENDTHEEASTWALLOFTHECASTLE";
    int key2 = 4;
    cout << "Ban ro goc : " << pt2 << endl;
    string ct2 = encryptRailFence(pt2, key2);
    cout << "Ban ma hoa : " << ct2 << endl;
    string dt2 = decryptRailFence(ct2, key2);
    cout << "Giai ma lai: " << dt2 << endl;
    cout << "Kiem tra: " << (dt2 == pt2 ? "DUNG" : "SAI") << endl;
}

int main(int argc, char* argv[]) {
    if (argc >= 2) {
        string mode = argv[1];
        if (mode == "demo") {
            demo();
            return 0;
        }
        if (mode == "encrypt" && argc >= 4) {
            int key = stoi(argv[2]);
            cout << encryptRailFence(argv[3], key) << endl;
            return 0;
        }
        if (mode == "decrypt" && argc >= 4) {
            int key = stoi(argv[2]);
            cout << decryptRailFence(argv[3], key) << endl;
            return 0;
        }
    }

    // Che do menu tuong tac neu khong truyen tham so dong lenh
    int luaChon;
    cout << "===== RAIL FENCE CIPHER =====\n";
    cout << "1. Ma hoa\n";
    cout << "2. Giai ma\n";
    cout << "3. Chay vi du minh hoa (demo)\n";
    cout << "Lua chon: ";
    cin >> luaChon;
    cin.ignore();

    if (luaChon == 1) nhapVaMaHoa();
    else if (luaChon == 2) nhapVaGiaiMa();
    else if (luaChon == 3) demo();
    else cout << "Lua chon khong hop le.\n";

    return 0;
}