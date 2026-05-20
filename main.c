#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define BATAS_SUHU_MAKS 33.0        
#define BATAS_KELEMBAPAN_MIN 70.0   
#define JUMLAH_BLOK 5

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

int main() {
    return 0;
}
