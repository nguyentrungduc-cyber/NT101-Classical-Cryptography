/**
 * Task 2.4 — Playfair Cipher
 * ============================================
 * Mã hóa/giải mã theo cặp ký tự (digraph) dùng bảng khóa 5x5.
 * Quy tắc I/J gộp chung 1 ô (bảng chuẩn 26 chữ cái - 1 = 25 ô vừa đủ 5x5).
 * Trong Key chữ bị trùng thi thay bằng X
 * Trong Key bị lẻ không đủ cặp thì chèn X vào cuối
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


// Phần Mã Hóa
///////////////////////////
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

void ThemKeyVaoMaTran(string Key){
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

void TimViTri(char c, int &row, int &col){
    if(c=='J') c='I';
    for(int i = 0; i <  5;i++){
        for(int j = 0; j <  5;j++){
            if(Matran[i][j]==c){
                row = i;
                col = j;
                return;
            }
        }
    }
}

string MaHoaBanRoTheoQuyTac(string Key, string BanRo ){
    string BanMaHoa="";
    ThemKeyVaoMaTran(Key);
    InMaTran_DaThemKey();
    vector<string> Ds_cap = TachCap_KyTu_BanRo(BanRo);
    for(string cap:Ds_cap){

        char a = cap[0];
        char b = cap[1];

        int row_a,col_a,row_b,col_b;
        TimViTri(a,row_a,col_a);
        TimViTri(b,row_b,col_b);
       
        //Doi ky tu theo quy tac
        if(row_a==row_b){
             a = Matran[row_a][(col_a+1)%5];
             b = Matran[row_b][(col_b+1)%5];
        }
        else if (col_a==col_b)
        {
             a = Matran[(row_a + 1)%5][(col_a)];
             b = Matran[(row_b + 1)%5][(col_b)];
        }
        else{
             a = Matran[row_a][col_b];
             b = Matran[row_b][col_a];
        }
        
        BanMaHoa+=a;
        BanMaHoa+=b;
    }
    return BanMaHoa;
}
///////////////////////////
// Phần Giải Mã

vector<string> TachCap_KyTu_BanMaHoa(string BanMaHoa){
    vector<string> DanhSach_CapBanMaHoa;
    int i = 0;

    while(i< BanMaHoa.length()){
        char a = BanMaHoa[i];
        if (i + 1 >= BanMaHoa.length()) break;
        char b = BanMaHoa[i+1];
        i += 2;
        string cap ="";
        cap+=a; cap+=b;
        DanhSach_CapBanMaHoa.push_back(cap);
    }

    return DanhSach_CapBanMaHoa;
}

string GiaiMa_TheoQuyTac(string Key,string VanBanMaHoa){
    //Tái sử dụng hàm ThemKeyVaoMaTran để tạo ma trận khóa mới
    ThemKeyVaoMaTran(Key);
    InMaTran_DaThemKey();
    string BanGoc="";

    vector<string> Ds_cap = TachCap_KyTu_BanMaHoa(VanBanMaHoa);// Tách kí tự theo cặp từ văn bản
    for(string cap:Ds_cap){

        char a = cap[0];
        char b = cap[1];

        int row_a,col_a,row_b,col_b;
        TimViTri(a,row_a,col_a);
        TimViTri(b,row_b,col_b);
       
        //Doi ky tu theo quy tac
        if(row_a==row_b){
             a = Matran[row_a][(col_a - 1 + 5)%5];
             b = Matran[row_b][(col_b - 1 + 5)%5];
        }
        else if (col_a==col_b)
        {
             a = Matran[(row_a - 1 + 5)%5][(col_a)];
             b = Matran[(row_b - 1 + 5)%5][(col_b)];
        }
        else{
             a = Matran[row_a][col_b];
             b = Matran[row_b][col_a];
        }
        
        BanGoc+=a;
        BanGoc+=b;
    }
    return BanGoc;

}

///////////////////////
void MaHoa_Playfair(){

    string key;
    cout <<" Nhap Key: "<<endl;
    getline(cin,key);

    string ban_ro;
    cout <<" Nhap ban ro: "<<endl;
    getline(cin,ban_ro);
   
    string ban_ma_hoa = MaHoaBanRoTheoQuyTac(key,ban_ro);
    cout<<"Van ban sau khi duoc ma hoa PlayFair: "<<endl;
    cout<<ban_ma_hoa;
}

void GiaiMa_Playfair(){
    string vanban_mahoa;
    cout<<"Nhap van ban da ma hoa: "<<endl;
    getline(cin,vanban_mahoa);

    string key_giai_ma;
    cout <<" Nhap Key: "<<endl;
    getline(cin,key_giai_ma);

    string van_ban_goc = GiaiMa_TheoQuyTac(key_giai_ma,vanban_mahoa);
    cout<<"Van ban sau khi duoc giai ma PlayFair: "<<endl;
    cout<<van_ban_goc;
}

int main(){
    // MaHoa_Playfair();
    GiaiMa_Playfair();
    return 0;
}
