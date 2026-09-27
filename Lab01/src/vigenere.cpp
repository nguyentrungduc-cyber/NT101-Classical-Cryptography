// Task 2.5 va 2.6 - Vigenere Cipher
// Ma hoa/giai ma theo cong thuc:
//   C = (P + K) mod 26
//   P = (C - K + 26) mod 26
// K la ky tu khoa tuong ung, lap lai theo chieu dai ban ro

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>

using namespace std;

// bang tan suat chu cai tieng Anh (%), lay tu slide mon hoc
double englishFreq[26] = {
    8.2, 1.5, 2.8, 4.3, 12.7, 2.2, 2.0, 6.1, 7.0, 0.15, 0.77, 4.0, 2.4,
    6.7, 7.5, 1.9, 0.095, 6.0, 6.3, 9.1, 2.8, 0.98, 2.4, 0.15, 2.0, 0.074};

// chi lay chu cai, doi het thanh in hoa, may cai khac (dau cau, so...) bo qua
string cleanText(string s)
{
    string out = "";
    for (int i = 0; i < (int)s.size(); i++)
    {
        if (isalpha(s[i]))
        {
            out += toupper(s[i]);
        }
    }
    return out;
}

// ma hoa Vigenere - giu nguyen dau cau khoang trang cho de doc
string encrypt(string plain, string key)
{
    string k = cleanText(key);
    string result = "";
    int j = 0; // dem vi tri trong khoa, chi tang khi gap chu cai

    for (int i = 0; i < (int)plain.size(); i++)
    {
        char c = plain[i];
        if (isupper(c))
        {
            int shift = k[j % k.size()] - 'A';
            result += (char)('A' + (c - 'A' + shift) % 26);
            j++;
        }
        else if (islower(c))
        {
            int shift = k[j % k.size()] - 'A';
            result += (char)('a' + (c - 'a' + shift) % 26);
            j++;
        }
        else
        {
            result += c; // dau cau, khoang trang thi giu nguyen
        }
    }
    return result;
}

string decrypt(string cipher, string key)
{
    string k = cleanText(key);
    string result = "";
    int j = 0;

    for (int i = 0; i < (int)cipher.size(); i++)
    {
        char c = cipher[i];
        if (isupper(c))
        {
            int shift = k[j % k.size()] - 'A';
            result += (char)('A' + ((c - 'A' - shift) % 26 + 26) % 26);
            j++;
        }
        else if (islower(c))
        {
            int shift = k[j % k.size()] - 'A';
            result += (char)('a' + ((c - 'a' - shift) % 26 + 26) % 26);
            j++;
        }
        else
        {
            result += c;
        }
    }
    return result;
}

// ================= PHAN PHA MA (Task 2.6) =================

// Index of Coincidence - do xac suat 2 chu cai random trong text giong nhau
// Tieng Anh that thi IC ~ 0.067, con random thi thap hon nhieu ~0.038
double tinhIC(string text)
{
    int dem[26] = {0};
    for (int i = 0; i < (int)text.size(); i++)
        dem[text[i] - 'A']++;

    int n = text.size();
    if (n <= 1)
        return 0;

    double tong = 0;
    for (int i = 0; i < 26; i++)
    {
        tong += dem[i] * (dem[i] - 1);
    }
    return tong / (n * (n - 1));
}

// Kasiski: tim cac cum 3 ky tu bi lap lai, ghi lai khoang cach giua cac lan xuat hien
// -> do dai khoa thuong la uoc chung cua may cai khoang cach nay
vector<int> timKhoangCachLapLai(string text)
{
    map<string, vector<int>> viTri;
    for (int i = 0; i + 3 <= (int)text.size(); i++)
    {
        string cum = text.substr(i, 3);
        viTri[cum].push_back(i);
    }

    vector<int> khoangCach;
    for (auto &p : viTri)
    {
        vector<int> &vt = p.second;
        if (vt.size() < 2)
            continue;
        for (int i = 1; i < (int)vt.size(); i++)
        {
            khoangCach.push_back(vt[i] - vt[0]);
        }
    }
    return khoangCach;
}

// thu doan do dai khoa tu 2 den 20, xem cai nao co phieu bau (Kasiski) va IC hop ly nhat
int doanDoDaiKhoa(string text)
{
    vector<int> khoangCach = timKhoangCachLapLai(text);

    // dem xem moi so tu 2->20 la uoc so cua bao nhieu khoang cach
    map<int, int> phieuBau;
    for (int d : khoangCach)
    {
        for (int f = 2; f <= 20; f++)
        {
            if (d % f == 0)
                phieuBau[f]++;
        }
    }

    cout << "Ket qua Kasiski (top 5 do dai duoc bau nhieu nhat):\n";
    vector<pair<int, int>> ds(phieuBau.begin(), phieuBau.end());
    sort(ds.begin(), ds.end(), [](pair<int, int> a, pair<int, int> b)
         { return a.second > b.second; });

    int soLuongXet = min((int)ds.size(), 5);
    for (int i = 0; i < soLuongXet; i++)
    {
        cout << "  Do dai " << ds[i].first << " -> " << ds[i].second << " phieu\n";
    }

    // xac nhan lai bang IC, do dai nao cho IC gan 0.067 nhat thi chon
    cout << "\nKiem tra IC cho cac ung vien:\n";
    int doDaiTot = 1;
    double lechNhoNhat = 999;

    for (int i = 0; i < soLuongXet; i++)
    {
        int len = ds[i].first;
        double tongIC = 0;
        for (int off = 0; off < len; off++)
        {
            string nhom = "";
            for (int j = off; j < (int)text.size(); j += len)
                nhom += text[j];
            tongIC += tinhIC(nhom);
        }
        double icTB = tongIC / len;
        cout << "  Do dai " << len << " -> IC trung binh = " << icTB << "\n";

        double lech = abs(icTB - 0.067);
        if (lech < lechNhoNhat)
        {
            lechNhoNhat = lech;
            doDaiTot = len;
        }
    }

    return doDaiTot;
}

// voi 1 nhom ky tu (da biet cung 1 vi tri khoa) va shift dang thu, tinh chi-squared
// giong het bai Caesar, cang thap thi cang giong tieng Anh
double tinhChiSquare(string nhom, int shift)
{
    int dem[26] = {0};
    for (char c : nhom)
    {
        int idx = ((c - 'A') - shift + 26) % 26;
        dem[idx]++;
    }

    int n = nhom.size();
    if (n == 0)
        return 999999;

    double chiSq = 0;
    for (int i = 0; i < 26; i++)
    {
        double expect = englishFreq[i] / 100.0 * n;
        if (expect > 0)
        {
            chiSq += (dem[i] - expect) * (dem[i] - expect) / expect;
        }
    }
    return chiSq;
}

// voi do dai khoa da biet, tach text thanh tung nhom theo vi tri, roi doan tung ky tu khoa
string doanKhoa(string text, int doDaiKhoa)
{
    string khoa = "";
    for (int off = 0; off < doDaiKhoa; off++)
    {
        string nhom = "";
        for (int j = off; j < (int)text.size(); j += doDaiKhoa)
            nhom += text[j];

        double diemTot = 999999;
        int shiftTot = 0;
        for (int shift = 0; shift < 26; shift++)
        {
            double diem = tinhChiSquare(nhom, shift);
            if (diem < diemTot)
            {
                diemTot = diem;
                shiftTot = shift;
            }
        }
        khoa += (char)('A' + shiftTot);
    }
    return khoa;
}

void phaMa(string cipherGoc)
{
    string cipher = cleanText(cipherGoc);

    if ((int)cipher.size() < 20)
    {
        cout << "Luu y: ban tin hoi ngan, ket qua co the khong chinh xac lam\n\n";
    }

    int doDai = doanDoDaiKhoa(cipher);
    cout << "\n>> Chon do dai khoa la: " << doDai << "\n\n";

    string khoa = doanKhoa(cipher, doDai);
    cout << "Khoa doan duoc: " << khoa << "\n\n";

    string banRo = decrypt(cipher, khoa);
    cout << "Ban ro: " << banRo << "\n";
}

int main()
{
    cout << "=== VIGENERE CIPHER ===\n";
    cout << "Chon che do:\n";
    cout << "  1. Ma hoa\n";
    cout << "  2. Giai ma\n";
    cout << "  3. Pha ma (khong biet khoa)\n";
    cout << "Nhap lua chon (1/2/3): ";

    int luaChon;
    cin >> luaChon;
    cin.ignore(); // xoa ki tu xuong dong con sot lai sau khi cin >> so

    if (luaChon == 1)
    {
        string key, plain;
        cout << "Nhap khoa: ";
        getline(cin, key);
        cout << "Nhap ban ro can ma hoa: ";
        getline(cin, plain);

        cout << "\nKet qua ma hoa: " << encrypt(plain, key) << "\n";
    }
    else if (luaChon == 2)
    {
        string key, cipher;
        cout << "Nhap khoa: ";
        getline(cin, key);
        cout << "Nhap ban ma can giai: ";
        getline(cin, cipher);

        cout << "\nKet qua giai ma: " << decrypt(cipher, key) << "\n";
    }
    else if (luaChon == 3)
    {
        string cipher;
        cout << "Nhap ban ma (chua biet khoa la gi): ";
        getline(cin, cipher);
        cout << "\n";

        phaMa(cipher);
    }
    else
    {
        cout << "Lua chon khong hop le, chi duoc nhap 1, 2 hoac 3\n";
        return 1;
    }

    return 0;
}
