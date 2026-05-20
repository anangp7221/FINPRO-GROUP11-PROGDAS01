#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BATAS_SUHU_MAKS 33.0        
#define BATAS_KELEMBAPAN_MIN 70.0   
#define JUMLAH_BLOK 5

// ---------------------------------------------------------------------
// Azzam, merancang struktur data dan membuat fungsi penghasil data dummy
typedef struct {
    char id_blok[10];
    float suhu;
    float kelembapan;
    int status_pompa;
} BlokLahan;

void generate_data_sensor(BlokLahan lahan[], int jumlah) {
    for (int i = 0; i < jumlah; i++) {
        sprintf(lahan[i].id_blok, "Blok-%d", i + 1);
        lahan[i].suhu = 28.0 + ((float)rand() / RAND_MAX) * 8.0;
        lahan[i].kelembapan = 50.0 + ((float)rand() / RAND_MAX) * 40.0;
        lahan[i].status_pompa = 0; 
    }
}
// ---------------------------------------------------------------------

// ---------------------------------------------------------------------
// Anang, mengembangkan algoritma pengambil keputusan irigasi presisi
void evaluasi_irigasi(BlokLahan *lahan, int jumlah) {
    for (int i = 0; i < jumlah; i++) {
        /* 
           LOGIKA SMART FARMING (SDG 2 - Zero Hunger)
           Pompa menyala (status_pompa = 1) JIKA: 
           1. Kelembapan tanah kurang dari 70% (Tanaman kritis kekurangan air) ATAU
           2. Suhu melebihi 33.0 C (Butuh pendinginan mikroklimat agar padi tidak stres panas)
           
           Jika kondisi aman, pompa mati (0) untuk menghemat efisiensi penggunaan air.
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
void tampilkan_dashboard(const BlokLahan *lahan, int jumlah, int siklus) {
    printf("\n=================================================================\n");
    printf("       DASHBOARD SMART IRRIGASI PADI - SIKLUS KE-%d\n", siklus);
    printf("=================================================================\n");
    printf("| %-8s | %-12s | %-15s | %-12s |\n", "Blok", "Suhu (*C)", "Kelembapan (%)", "Status Pompa");
    printf("-----------------------------------------------------------------\n");
    
    for (int i = 0; i < jumlah; i++) {
        printf("| %-8s | %-12.2f | %-15.2f | ", 
               (lahan + i)->id_blok, 
               (lahan + i)->suhu, 
               (lahan + i)->kelembapan);
        
        if ((lahan + i)->status_pompa == 1) {
            printf("ON (MENYIRAM) |\n"); 
        } else {
            printf("OFF (AMAN)    |\n");   
        }
    }
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
    srand(time(NULL)); 
    
    BlokLahan lahan_sawah[JUMLAH_BLOK]; 
    
    int jumlah_siklus_simulasi = 3; 

    printf("=================================================================\n");
    printf(" Memulai Sistem Irigasi Otomatis Berbasis Cerdas (SDG 2 - Zero Hunger)\n");
    printf("=================================================================\n");
    
    for (int siklus = 1; siklus <= jumlah_siklus_simulasi; siklus++) {
        
        generate_data_sensor(lahan_sawah, JUMLAH_BLOK);
        
        evaluasi_irigasi(lahan_sawah, JUMLAH_BLOK);
        
        tampilkan_dashboard(lahan_sawah, JUMLAH_BLOK, siklus);
        
    }
    
    printf("\n[INFO] Simulasi selesai. Sistem Smart Farming hemat air sukses diterapkan!\n");
    return 0;
}
// ---------------------------------------------------------------------
