#include <string.h>        // Pustaka string.h untuk memproses input string

#define DXL_BUS_SERIAL3 3  // Menggunakan Serial3 (USART3) untuk komunikasi dengan Dynamixel
#define ID_NUM1 1          // ID motor Dynamixel pertama
#define ID_NUM2 2          // ID motor Dynamixel kedua

Dynamixel Dxl(DXL_BUS_SERIAL3);

void setup() {
  Serial1.begin(115200);    // Inisialisasi komunikasi Serial1 dengan komputer (Web Serial API)
  delay(2000);              // Delay untuk memastikan Serial1 siap

  Dxl.begin(1000000);       // Inisialisasi Dynamixel dengan baud rate 1 Mbps
  Dxl.jointMode(ID_NUM1);   // Set motor dalam mode joint
  Dxl.jointMode(ID_NUM2);

  delay(1000);              // Delay tambahan untuk memastikan semuanya siap
  Serial1.println("Siap menerima data dari Web Serial API...");
}

void loop() {
  // Memeriksa apakah ada data yang masuk dari Serial1 (Web Serial API)
  if (Serial1.available() > 0) {
    char input[256];  // Buffer untuk menyimpan data input yang lebih besar
    int index = 0;

    // Membaca data dari Serial1
    while (Serial1.available() && index < sizeof(input) - 1) {
      char c = Serial1.read();
      if (c == '\n') {  // Menghentikan input jika ada newline
        input[index] = '\0';  // Mengakhiri string
        break;
      }
      input[index++] = c;
    }

    // Debug: Menampilkan data yang diterima
    Serial1.print("Data diterima: ");
    Serial1.println(input);

    // Variabel untuk menyimpan nilai dari input
    float startPos1 = 0;
    float startPos2 = 0;
    float endPos1 = 0;
    float endPos2 = 0;
    float velocity = 0;
    float trajectory = 0;
    float trajectoryDelay = 0;

    // Memproses input yang diterima
    char* token = strtok(input, ",");
    while (token != NULL) {
      if (strncmp(token, "startPos1:", 10) == 0) {
        startPos1 = atof(token + 10);
      } else if (strncmp(token, "startPos2:", 10) == 0) {
        startPos2 = atof(token + 10);
      } else if (strncmp(token, "endPos1:", 8) == 0) {
        endPos1 = atof(token + 8);
      } else if (strncmp(token, "endPos2:", 8) == 0) {
        endPos2 = atof(token + 8);
      } else if (strncmp(token, "velocity:", 9) == 0) {
        velocity = atof(token + 9);
      } else if (strncmp(token, "trajectory:", 11) == 0) {
        trajectory = atof(token + 11);
      } else if (strncmp(token, "trajectoryDelay:", 16) == 0) {
        trajectoryDelay = atof(token + 16);
      }
      token = strtok(NULL, ",");
    }

    // Debug: Menampilkan nilai yang diambil
    Serial1.print("startPos1: ");
    Serial1.println(startPos1);
    Serial1.print("startPos2: ");
    Serial1.println(startPos2);
    Serial1.print("endPos1: ");
    Serial1.println(endPos1);
    Serial1.print("endPos2: ");
    Serial1.println(endPos2);
    Serial1.print("velocity: ");
    Serial1.println(velocity);
    Serial1.print("trajectory: ");
    Serial1.println(trajectory);
    Serial1.print("trajectoryDelay: ");
    Serial1.println(trajectoryDelay);

    // Mengatur posisi awal motor Dynamixel
    Dxl.setPosition(ID_NUM1, startPos1, velocity);
    Dxl.setPosition(ID_NUM2, startPos2, velocity);
    delay(500);  // Delay untuk memastikan motor berada di posisi awal

    // Menghitung langkah pergerakan per trajectory
    float step1 = (startPos1 - endPos1) / trajectory;
    float step2 = (endPos2 - startPos2) / trajectory;

    // Menggerakkan motor melalui beberapa langkah (trajectory)
    for (int i = 1; i <= trajectory; i++) {
      float currentPos1 = startPos1 - (step1 * i);
      float currentPos2 = startPos2 + (step2 * i);

      // Debug: Menampilkan posisi saat ini
      Serial1.print("currentPos1: ");
      Serial1.println(currentPos1);
      Serial1.print("currentPos2: ");
      Serial1.println(currentPos2);

      // Mengatur posisi motor
      Dxl.setPosition(ID_NUM1, currentPos1, velocity);
      Dxl.setPosition(ID_NUM2, currentPos2, velocity);

      // Delay setelah setiap langkah
      delay(trajectoryDelay);
    }

    // Memberi tahu bahwa pergerakan telah selesai
    Serial1.println("Pergerakan selesai.");
  }
}
