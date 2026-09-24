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
#include<vector>
#include<set>
using namespace std;

//Ma hoa 1 ki tu
char MaHoaKiTu(char char_in_P,int key){
    if(!isalpha(char_in_P)) return char_in_P;
    char base = isupper(char_in_P)?'A' : 'a';
    int vi_tri = char_in_P - base;
    int vi_tri_moi = (vi_tri + key) % 26;
    if (vi_tri_moi < 0) vi_tri_moi += 26;

    return base + vi_tri_moi;
}
//Ma hoa chuoi ki tu
string MaHoaChuoi(string P,int key){
    key = (((key%26) +26) % 26);

    string ket_qua = "";
    for(char c:P){
        ket_qua += MaHoaKiTu(c,key);
    }
    return ket_qua;
}
//Cau 2.1.1 - Nhap vao 1 chuoi roi ma hoa chuoi do
void NhapVaMaHoa(){
    string p;
    int key;
    
    cout << "Nhap van ban goc: ";
    getline(cin, p);
    
    cout << "Nhap khoa (key): ";
    cin >> key;
    
    string maHoaXong = MaHoaChuoi(p, key);
    cout << "Van ban da ma hoa: " << maHoaXong << endl;
}

//Cau 2.1.2 - Ma hoa van ban 

set<string> tuDienPhoBien = {
    "the","be","to","of","and","a","in","that","have","i",
    "it","for","not","on","with","he","as","you","do","at",
    "this","but","his","by","from","they","we","say","her","she",
    "or","an","will","my","one","all","would","there","their","what",
    "so","up","out","if","about","who","get","which","go","me",
    "when","make","can","like","time","no","just","him","know","take",
    "people","into","year","your","good","some","could","them","see","other"
};
// Tách chuỗi thành các từ (bỏ dấu câu), chuyển về chữ thường
vector<string> tachTu(string s) {
    vector<string> tuVung;
    string tuHienTai = "";
    for (char c : s) {
        if (isalpha(c)) {
            tuHienTai += tolower(c);
        } else {
            if (!tuHienTai.empty()) {
                tuVung.push_back(tuHienTai);
                tuHienTai = "";
            }
        }
    }
    if (!tuHienTai.empty()) tuVung.push_back(tuHienTai);
    return tuVung;
}

// Chấm điểm: đếm số từ hợp lệ trong bản giải mã
int chamDiem(string s) {
    vector<string> tuVung = tachTu(s);
    int diem = 0;
    for (string tu : tuVung) {
        if (tuDienPhoBien.count(tu)) diem++;
    }
    return diem;
}

void GiaiMaVanBan(){
    string ciphertext;
    cout << "Nhap van ban ma hoa: ";
    cin.ignore();
    getline(cin, ciphertext);
    

    int diemCaoNhat = -1;
    int keyDung = -1;
    string ketQuaDung = "";

    for (int key = 0; key < 26; key++) {
        string thuGiaiMa = MaHoaChuoi(ciphertext, -key); // giải mã = mã hóa với -key
        int diem = chamDiem(thuGiaiMa);

        if (diem > diemCaoNhat) {
            diemCaoNhat = diem;
            keyDung = key;
            ketQuaDung = thuGiaiMa;
        }
    }

    cout << "\nKhoa tim duoc: " << keyDung << endl;
    cout << "Ban ro: " << ketQuaDung << endl;
}

int main(){
    //NhapVaMaHoa();
    GiaiMaVanBan();
    return 0;
} 