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
 */

#include <iostream>
#include <string>
#include <vector>
using namespace std;

char Matran[5][5] = {};

string Tach_kytu_Key (string key){
    string danhsach = "";
    bool ds_da_xuat_hien [26] = {false};

    //Duyet va them key vao danh sach
    for (char c:key){
        if(!isalpha(c)) continue; // ko phai ky tu thi ko cho vao ma tran
        c = toupper (c); // đưa về dạng chữ hoa 
        if(c == 'J') c == 'I';
        
        int idx = c - 'A';// vi trí của chữ hiện tại trong mảng ds_da_xuat_hien
        if(!ds_da_xuat_hien[idx]){
        ds_da_xuat_hien[idx] = true;
        danhsach += c;
        }
    }

    //Them ky tu con lai vao danh sach
    for(char c='A'; c <='Z';c++){
        if (c=='J') continue;
        int idx = c - 'A';
        if (!ds_da_xuat_hien[idx]) {
            ds_da_xuat_hien[idx]=true;
            danhsach+=c;
        } 
    }

    return danhsach;
};

void ThemDanhSachVaoMaTran(string Key){
    string DanhSach = Tach_kytu_Key(Key);
    
    int idx = 0;
    for(int i =0; i< 5; i++){
        for(int j = 0; j<5;j++){
            Matran[i][j] = DanhSach[idx];
            idx++;
        }
    }
}

void InMaTran_DaThemKey(){
    for(int i =0; i< 5; i++){
        for(int j = 0; j<5;j++){
            cout << Matran[i][j];
            if(j != 4) cout << " "; 
        }
    cout << endl;
    }
}

vector<string> TachCap_KyTu_BanRo(string BanRo){
    vector<string> DanhSach_CapBanRo;
    int i = 0;

    while(i< BanRo.length()){
        char a = BanRo[i];
        char b;

        if(i+1>=BanRo.length()){
            b = 'X';
            i+=1;
        }
        else{
            b = BanRo[i+1];
            if(a==b){
                b ='X';
                i+=1;
            }
            else{
                i+=2;
            }
        }

        string cap ="";
        cap+=a; cap+=b;
        DanhSach_CapBanRo.push_back(cap);
    }

    return DanhSach_CapBanRo;
}

void MaHoa_Playfair(){

    string key;
    cout <<" Nhap Key: "<<endl;
    getline(cin,key);
    cin.ignore();

    string ban_ro;
    cout <<" Nhap ban ro: "<<endl;
    getline(cin,ban_ro);
    cin.ignore();
}

int main(){
    return 0;
}
