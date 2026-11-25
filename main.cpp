#include <iostream>
#include <string>
using namespace std;

int main() {
    int stok = 200;
    int pilihan;
    string nama[200];
    string nim[200];
    int jumlahPinjam = 0;

    while (true) {
        cout << "\nBooking Sepeda UM" << endl;
        cout << "Sisa Stok Sepeda: " << stok << endl;
        cout << "1. Pinjam Sepeda" << endl;
        cout << "2. Mengembalikan Sepeda" << endl;
        cout << "3. Lihat Daftar Peminjam" << endl;
        cout << "4. Keluar" << endl;
        cout << "Pilih Menu (1-4): ";
        cin >> pilihan;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Input salah! Harap masukkan angka 1-4" << endl;
            continue;
        }

        if (pilihan == 1) {
            if (stok > 0) {
                cin.ignore();
                cout << "Masukkan Nama: ";
                getline(cin, nama[jumlahPinjam]);
                cout << "Masukkan NIM: ";
                getline(cin, nim[jumlahPinjam]);

                stok--;
                jumlahPinjam++;
                cout << "Booking berhasil! Silakan ambil sepeda." << endl;
            } else {
                cout << "Maaf, stok sepeda habis! Tidak bisa booking." << endl;
            }
        }

    }
    return 0;
} 