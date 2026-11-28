#include <iostream>
#include <string>
#include <limits>
#include <cctype>
#include <fstream>
#include <sstream>
using namespace std;

const int TOTAL_SEPEDA = 20;
const int MAKS_BOOKING = 1000;
const int PANJANG_NIM  = 12;
const int DURASI_MIN   = 10;
const int DURASI_MAX   = 120;
const int JAM_BUKA     = 0 * 60;
const int JAM_TUTUP    = 24 * 60;
const string NAMA_FILE = "data_peminjam.txt";

struct Peminjam {
    string nama;
    string nim;
    int sepeda;
    int mulai;
    int selesai;
};

// Utilitas Input

void bersihkanInput() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Input angka untuk menu lainnya
int bacaAngka(const string& prompt, int minV, int maxV) {
    int x;

    while (true) {
        cout << prompt;

        if (cin >> x && x >= minV && x <= maxV) {
            bersihkanInput();
            return x;
        }

        bersihkanInput();
        cout << "Input harus angka "
             << minV << "-" << maxV << "!" << endl;
    }
}

// Input khusus menu utama
// Jika salah, kembali ke menu utama
int bacaPilihanMenu(const string& prompt, int minV, int maxV) {
    int x;

    cout << prompt;

    if (cin >> x && x >= minV && x <= maxV) {
        bersihkanInput();
        return x;
    }

    bersihkanInput();

    cout << "Input harus angka "
         << minV << "-" << maxV << "!" << endl;

    return -1;
}

// Validasi Nama

bool namaValid(const string& s) {
    bool adaHuruf = false;

    for (char ch : s) {
        unsigned char c = static_cast<unsigned char>(ch);

        if (isalpha(c)) {
            adaHuruf = true;
        }
        else if (c != ' ' && c != '.' &&
                 c != '\'' && c != '-') {
            return false;
        }
    }

    return adaHuruf;
}

string bacaNama() {
    string n;

    while (true) {
        cout << "Masukkan Nama: ";
        getline(cin, n);

        if (namaValid(n)) {
            return n;
        }

        cout << "Nama tidak boleh kosong dan hanya berisi huruf!"
             << endl;
    }
}

// Validasi NIM

bool nimValid(const string& s) {
    if (s.length() != PANJANG_NIM) {
        return false;
    }

    for (char c : s) {
        if (!isdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }

    return true;
}

string bacaNIM() {
    string m;

    while (true) {
        cout << "Masukkan NIM (" << PANJANG_NIM
             << " digit angka): ";

        getline(cin, m);

        if (nimValid(m)) {
            return m;
        }

        cout << "NIM tidak valid! Harus tepat "
             << PANJANG_NIM
             << " digit dan hanya angka." << endl;
    }
}

// Pengaturan Jam

bool parseJam(const string& s, int& menit) {
    size_t p = s.find_first_of(":.");

    if (p == string::npos) {
        return false;
    }

    string jam = s.substr(0, p);
    string mnt = s.substr(p + 1);

    if (jam.empty() || jam.length() > 2 ||
        mnt.length() != 2) {
        return false;
    }

    for (char c : jam + mnt) {
        if (!isdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }

    int h = stoi(jam);
    int m = stoi(mnt);

    if (h > 23 || m > 59) {
        return false;
    }

    menit = h * 60 + m;

    return true;
}

string formatJam(int menit) {
    int h = menit / 60;
    int m = menit % 60;

    string r;

    if (h < 10) r += "0";
    r += to_string(h);
    r += ":";

    if (m < 10) r += "0";
    r += to_string(m);

    return r;
}

int bacaJam(const string& prompt) {
    string s;
    int menit;

    while (true) {
        cout << prompt;
        getline(cin, s);

        if (parseJam(s, menit)) {
            return menit;
        }

        cout << "Format jam salah! Contoh: 12:00 atau 08.30"
             << endl;
    }
}

// Logika Booking

bool nimSudahAda(
    const Peminjam data[],
    int jumlah,
    const string& nim
) {
    for (int i = 0; i < jumlah; i++) {
        if (data[i].nim == nim) {
            return true;
        }
    }

    return false;
}

bool bentrok(
    const Peminjam data[],
    int jumlah,
    int sepeda,
    int mulai,
    int selesai,
    int& idxBentrok
) {
    for (int i = 0; i < jumlah; i++) {
        if (data[i].sepeda == sepeda &&
            mulai < data[i].selesai &&
            data[i].mulai < selesai) {

            idxBentrok = i;
            return true;
        }
    }

    return false;
}

// Penyimpanan File

void simpanData(const Peminjam data[], int jumlah) {
    ofstream f(NAMA_FILE);

    for (int i = 0; i < jumlah; i++) {
        f << data[i].nama << '|'
          << data[i].nim << '|'
          << data[i].sepeda << '|'
          << data[i].mulai << '|'
          << data[i].selesai << '\n';
    }
}

int muatData(Peminjam data[]) {
    ifstream f(NAMA_FILE);

    if (!f) {
        return 0;
    }

    int jumlah = 0;
    string baris;

    while (jumlah < MAKS_BOOKING &&
           getline(f, baris)) {

        stringstream ss(baris);

        string a, b, c, d, e;

        if (getline(ss, a, '|') &&
            getline(ss, b, '|') &&
            getline(ss, c, '|') &&
            getline(ss, d, '|') &&
            getline(ss, e)) {

            try {
                Peminjam p;

                p.nama = a;
                p.nim = b;
                p.sepeda = stoi(c);
                p.mulai = stoi(d);
                p.selesai = stoi(e);

                data[jumlah++] = p;
            }
            catch (...) {
                // Baris yang rusak dilewati
            }
        }
    }

    return jumlah;
}

// Program Utama

int main() {
    Peminjam data[MAKS_BOOKING];

    int jumlah = muatData(data);

    while (true) {

        cout << "\n=== Booking Sepeda UM ===" << endl;

        cout << "Total Sepeda: " << TOTAL_SEPEDA
             << " | Booking Aktif: " << jumlah << endl;

        cout << "1. Pinjam Sepeda" << endl;
        cout << "2. Mengembalikan Sepeda" << endl;
        cout << "3. Lihat Daftar Peminjam" << endl;
        cout << "4. Keluar" << endl;

        int pilihan = bacaPilihanMenu(
            "Pilih Menu (1-4): ", 1, 4
        );

        // Jika input menu salah, ulangi seluruh menu
        if (pilihan == -1) {
            continue;
        }

        if (pilihan == 1) {

            // ---------- PINJAM SEPEDA ----------

            if (jumlah >= MAKS_BOOKING) {
                cout << "Maaf, kapasitas booking penuh!"
                     << endl;
                continue;
            }

            Peminjam p;

            p.nama = bacaNama();
            p.nim = bacaNIM();

            if (nimSudahAda(data, jumlah, p.nim)) {
                cout << "NIM ini masih memiliki peminjaman aktif!"
                     << endl;
                continue;
            }

            while (true) {

                p.sepeda = bacaAngka(
                    "Nomor sepeda (1-" +
                    to_string(TOTAL_SEPEDA) +
                    ", 0 = batal): ",
                    0,
                    TOTAL_SEPEDA
                );

                if (p.sepeda == 0) {
                    cout << "Booking dibatalkan." << endl;
                    break;
                }

                p.mulai = bacaJam(
                    "Jam mulai (contoh 12:00 atau 12.00): "
                );

                int durasi = bacaAngka(
                    "Durasi (" +
                    to_string(DURASI_MIN) + "-" +
                    to_string(DURASI_MAX) + " menit): ",
                    DURASI_MIN,
                    DURASI_MAX
                );

                p.selesai = p.mulai + durasi;

                if (p.mulai < JAM_BUKA ||
                    p.selesai > JAM_TUTUP) {

                    cout << "Jadwal di luar jam operasional ("
                         << formatJam(JAM_BUKA)
                         << " - "
                         << formatJam(JAM_TUTUP)
                         << ")." << endl;

                    continue;
                }

                int idx;

                if (bentrok(
                    data,
                    jumlah,
                    p.sepeda,
                    p.mulai,
                    p.selesai,
                    idx
                )) {

                    cout << "Sepeda no " << p.sepeda
                         << " sudah dibooking pukul "
                         << formatJam(data[idx].mulai)
                         << " - "
                         << formatJam(data[idx].selesai)
                         << ". Pilih sepeda atau jam lain."
                         << endl;

                    continue;
                }

                data[jumlah++] = p;

                simpanData(data, jumlah);

                cout << "Booking berhasil! Sepeda no "
                     << p.sepeda
                     << ", pukul "
                     << formatJam(p.mulai)
                     << " - "
                     << formatJam(p.selesai)
                     << "." << endl;

                cout << "Tunjukkan KTM ke petugas "
                     << "untuk mengambil kunci."
                     << endl;

                break;
            }

        }

        else if (pilihan == 2) {

            // MENGEMBALIKAN SEPEDA

            string m = bacaNIM();

            int idx = -1;

            for (int i = 0; i < jumlah; i++) {
                if (data[i].nim == m) {
                    idx = i;
                    break;
                }
            }

            if (idx == -1) {

                cout << "Data peminjaman dengan NIM "
                     << "tersebut tidak ditemukan."
                     << endl;

            }
            else {

                for (int i = idx; i < jumlah - 1; i++) {
                    data[i] = data[i + 1];
                }

                jumlah--;

                simpanData(data, jumlah);

                cout << "Sepeda berhasil dikembalikan. "
                     << "Terima kasih!"
                     << endl;
            }

        }

        else if (pilihan == 3) {

            // DAFTAR PEMINJAM

            cout << "\n--- Daftar Peminjam ---" << endl;

            if (jumlah == 0) {

                cout << "Belum ada peminjam." << endl;

            }
            else {

                for (int i = 0; i < jumlah; i++) {

                    cout << i + 1 << ". "
                         << data[i].nama
                         << " | NIM: "
                         << data[i].nim
                         << " | Sepeda no "
                         << data[i].sepeda
                         << " | "
                         << formatJam(data[i].mulai)
                         << " - "
                         << formatJam(data[i].selesai)
                         << " ("
                         << data[i].selesai - data[i].mulai
                         << " menit)"
                         << endl;
                }
            }

        }

        else if (pilihan == 4) {

            // KELUAR

            simpanData(data, jumlah);

            cout << "Terima kasih, sampai jumpa!"
                 << endl;

            break;
        }
    }

    return 0;
}