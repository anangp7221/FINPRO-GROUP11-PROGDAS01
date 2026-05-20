#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

// --- KONSTANTA PARAMETER LINGKUNGAN (Referensi: IRRI & JICA) ---
// Batas suhu maksimal padi (di atas 33C memicu stres panas)
#define BATAS_SUHU_MAKS 33.0        
// Batas kelembapan minimal tanah untuk tanaman padi (kritis jika < 70%)
#define BATAS_KELEMBAPAN_MIN 70.0   
// Jumlah blok lahan pertanian yang disimulasikan
#define JUMLAH_BLOK 5               

// ---------------------------------------------------------------------
// Azzam, merancang struktur data dan membuat fungsi penghasil data dummy
// Definisi Tipe Data Terstruktur (Struct) untuk merepresentasikan satu blok sawah
typedef struct {
    // Array of char (string) untuk menyimpan identitas blok, misal: "Blok-1"
    char id_blok[10];
    // Menyimpan data suhu mikroklimat dalam satuan derajat Celcius
    float suhu;
    // Menyimpan data kelembapan tanah dalam satuan persen (%)
    float kelembapan;
    // Status aktuator fisik: 0 berarti MATI (OFF), 1 berarti MENYALA (ON)
    int status_pompa;
} BlokLahan;

/*
 * Fungsi: generate_data_sensor
 * Parameter: 
 *   - BlokLahan lahan[]: Array of struct yang menampung data seluruh blok
 *   - int jumlah: Total blok lahan yang akan diproses
 * Deskripsi: Menyimulasikan akuisisi data sensor IoT dengan menghasilkan angka acak secara agronomis
 */
void generate_data_sensor(BlokLahan lahan[], int jumlah) {
    for (int i = 0; i < jumlah; i++) {
        // Format string secara otomatis untuk memberi nama id_blok ("Blok-1", "Blok-2", dst.)
        sprintf(lahan[i].id_blok, "Blok-%d", i + 1);
        // Formula Angka Acak: Nilai_Minimal + (Rentang * Angka_Acak_0_Sampai_1)
        // Menghasilkan nilai suhu acak fluktuatif antara 28.0°C hingga 36.0°C
        lahan[i].suhu = 28.0 + ((float)rand() / RAND_MAX) * 8.0;
        // Menghasilkan nilai kelembapan acak fluktuatif antara 50.0% hingga 90.0%
        lahan[i].kelembapan = 50.0 + ((float)rand() / RAND_MAX) * 40.0;
        // Aturan Dasar: Reset status pompa ke posisi MATI (0) di setiap awal siklus waktu baru
        lahan[i].status_pompa = 0; 
    }
}
// ---------------------------------------------------------------------

// ---------------------------------------------------------------------
// Anang, mengembangkan algoritma pengambil keputusan irigasi presisi
/*
 * Fungsi: evaluasi_irigasi
 * Parameter:
 *   - BlokLahan *lahan: Pointer yang menunjuk ke alamat array data blok lahan
 *   - int jumlah: Total blok lahan yang akan dievaluasi
 * Deskripsi: Mengimplementasikan logika "Smart Farming" untuk menghemat air sekaligus mencegah gagal panen
 */
void evaluasi_irigasi(BlokLahan *lahan, int jumlah) {
    for (int i = 0; i < jumlah; i++) {
        /* 
         * LOGIKA PENGKONDISIAN (Decision Making):
         * Pompa air WAJIB diaktifkan (status_pompa = 1) JIKA:
         * 1. Kelembapan tanah berada di bawah batas minimum (< 70.0%) -> Sawah kekeringan.
         *    -- ATAU --
         * 2. Suhu udara melebihi batas maksimal (> 33.0°C) -> Tanaman butuh pendinginan mikroklimat.
         * 
         * Jika kedua kondisi kritis di atas tidak terpenuhi, pompa tetap MATI (0) untuk efisiensi air.
         */
        if (lahan[i].kelembapan < BATAS_KELEMBAPAN_MIN || lahan[i].suhu > BATAS_SUHU_MAKS) {
            lahan[i].status_pompa = 1; // Aktifkan Pompa (ON)
        } else {
            lahan[i].status_pompa = 0; // Matikan Pompa (OFF)
        }
    }
}
// --------------------------------------------------------------------------

// ---------------------------------------------------------------------
// Akmal, memanfaatkan pointer untuk efisiensi memori dan mendesain dashboard
/*
 * Fungsi: tampilkan_dashboard
 * Parameter:
 *   - const BlokLahan *lahan: Pointer read-only (const) untuk efisiensi memori (Pass by Reference)
 *   - int jumlah: Total blok lahan yang akan ditampilkan datanya
 *   - int siklus: Indikator siklus waktu pemantauan yang sedang berjalan
 * Deskripsi: Menampilkan tabel summary status lahan terupdate ke layar terminal
 */
void tampilkan_dashboard(const BlokLahan *lahan, int jumlah, int siklus) {
    // Mencetak header visual dashboard
    printf("\n=================================================================\n");
    printf("       DASHBOARD SMART IRRIGASI PADI - SIKLUS KE-%d\n", siklus);
    printf("=================================================================\n");
    printf("| %-8s | %-12s | %-15s | %-12s |\n", "Blok", "Suhu (*C)", "Kelembapan (%)", "Status Pompa");
    printf("-----------------------------------------------------------------\n");
    
    for (int i = 0; i < jumlah; i++) {
        /*
         * OPTIMASI MEMORI (Aritmatika Pointer):
         * Menggunakan sintaks `(lahan + i)->atribut` sebagai pengganti `lahan[i].atribut`.
         * Konsep Pass by Reference ini mencegah sistem melakukan duplikasi/copy data struct utuh 
         * ke dalam RAM fungsi, sehingga sangat menghemat resource memori mikrokontroler.
         */
        printf("| %-8s | %-12.2f | %-15.2f | ", 
               (lahan + i)->id_blok, 
               (lahan + i)->suhu, 
               (lahan + i)->kelembapan);
        
        // Konversi data boolean/integer status_pompa menjadi visual teks yang informatif
        if ((lahan + i)->status_pompa == 1) {
            printf("ON (MENYIRAM) |\n"); 
        } else {
            printf("OFF (AMAN)    |\n");   
        }
    }

    // Mencetak footer beserta parameter baku pembanding dari IRRI/JICA
    printf("=================================================================\n");
    printf("Referensi Agronomi Padi (IRRI/JICA):\n");
    printf("- Suhu Kritis (Stres Panas) : > %.1f *C\n", BATAS_SUHU_MAKS);
    printf("- Kelembapan Kritis (Kering): < %.1f %%\n", BATAS_KELEMBAPAN_MIN);
    printf("=================================================================\n");
}
// --------------------------------------------------------------------------

// ---------------------------------------------------------------------
// Anang, menghidupkan siklus utama sistem simulasi mikrokontroler
int main() {
    // Mengatur "seed" generator angka acak menggunakan waktu sistem agar data sensor selalu berubah tiap dijalankan
    srand(time(NULL)); 

    // Alokasi memori array lokal untuk menampung data dari 5 blok sawah
    BlokLahan lahan_sawah[JUMLAH_BLOK]; 

    // Konfigurasi batasan jumlah iterasi perulangan simulasi monitoring
    int jumlah_siklus_simulasi = 3; 

    printf("=================================================================\n");
    printf(" Memulai Sistem Irigasi Otomatis Berbasis Cerdas (SDG 2 - Zero Hunger)\n");
    printf("=================================================================\n");

    // Siklus Utama Operasional Mikrokontroler
    for (int siklus = 1; siklus <= jumlah_siklus_simulasi; siklus++) {

        // Tahap 1: Panggil fungsi modul input milik Azzam untuk generate data lapangan baru
        generate_data_sensor(lahan_sawah, JUMLAH_BLOK);

        // Tahap 2: Panggil fungsi modul kontrol logika milik Anang untuk mengevaluasi status aktuator
        evaluasi_irigasi(lahan_sawah, JUMLAH_BLOK);

        // Tahap 3: Panggil fungsi modul output milik Akmal untuk mencetak visual monitoring terupdate
        tampilkan_dashboard(lahan_sawah, JUMLAH_BLOK, siklus);
        
    }
    
    printf("\n[INFO] Simulasi selesai. Sistem Smart Farming hemat air sukses diterapkan!\n");
    return 0;
}
// ---------------------------------------------------------------------
