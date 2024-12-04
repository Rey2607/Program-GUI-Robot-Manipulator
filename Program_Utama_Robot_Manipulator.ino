// Definisi untuk Dynamixel dan ID motor
#define DXL_BUS_SERIAL1 1 //Dynamixel on Serial1(USART1) <-OpenCM9.04
#define DXL_BUS_SERIAL2 2 //Dynamixel on Serial2(USART2) <-LN101,BT210
#define DXL_BUS_SERIAL3 3 // Dynamixel on Serial3(USART3)  <-OpenCM 485EXP

// Dynamixel ID defines
#define ID_NUM1 1
#define ID_NUM2 2

// Control table defines
#define GOAL_POSITIONL 30
#define GOAL_POSITIONH 31
#define PRESENT_POS 36
#include<math.h>

Dynamixel Dxl(DXL_BUS_SERIAL3);

// Input manual langsung di kode
float startPos1 = 805;      // Posisi awal untuk servo ID_NUM1
float startPos2 = 215;      // Posisi awal untuk servo ID_NUM2
float endPos1 = 215;        // Posisi akhir untuk servo ID_NUM1
float endPos2 = 805;        // Posisi akhir untuk servo ID_NUM2
float velocity = 150;       // Kecepatan servo
float trajectory = 10;      // Jumlah trajectory untuk mencapai posisi akhir
float trajectoryDelay = 20; // Delay di antara setiap trajectory (dalam ms)

// Variabel tambahan untuk gerakan
int min_pos = 215;  //0 degree
int mid_pos = 512;  //90 degree
int max_pos = 805;  //180 degree
int a=0;
int b=1023;
float i;
boolean flag=0;
int pos1, pos2;
float l2 = 6.4;
float l1 = 6.7;
int input1, input2, rawdata1, rawdata2, inputT1, inputT2;
float T1, T2, T1rad, T2rad, X, Y, c, d, e, f, g, T2deg, T1deg;

void setup() {
  // Inisialisasi baudrate Dynamixel -> 3: 1Mbps
  Dxl.begin(3);

  Dxl.jointMode(ID_NUM1);
  Dxl.jointMode(ID_NUM2);

  delay(1000);  // Delay untuk persiapan
}

void gerak() {
  // Kalkulasi IK (Inverse Kinematics) untuk menghitung posisi motor berdasarkan posisi X, Y
  c = (X*X) + (Y*Y) - (l1*l1) - (l2*l2);
  d = 2 * l1 * l2;
  e = c / d;
  T2rad = acos(e);
  T2deg = (T2rad * 180) / 3.141592654;
  f = atan2(Y, X);
  g = atan2((l2 * sin(T2rad)), (l1 + l2 * cos(T2rad)));
  T1rad = f - g;
  T1deg = (T1rad * 180) / 3.141592654;

  // Konversi derajat ke posisi servo
  inputT1 = ((T1deg * 614) / 180) + 205;
  inputT2 = (((T2deg + 90) * 614) / 180) + 205;

  // Gerakkan motor sesuai dengan hasil IK
  Dxl.writeWord(ID_NUM1, GOAL_POSITIONL, inputT1); 
  Dxl.writeWord(ID_NUM2, GOAL_POSITIONL, inputT2);

  // Membaca posisi servo saat ini
  pos1 = Dxl.readWord(ID_NUM1, PRESENT_POS);
  pos2 = Dxl.readWord(ID_NUM2, PRESENT_POS);
  
  delay(50);  // Delay kecil agar gerakan halus
}

void tambah() {
  for (float i = 0; i <= 1; i += 0.01) {
    X = 4;
    Y = 9 + i;
    gerak();
  }
  for (float i = 0; i <= 1; i += 0.01) {
    X = 4 - i;
    Y = 10;
    gerak();
  }
  for(float i=0; i<=1; i+=0.01) {
    X = 3;
    Y = 10+i;
    gerak();
  }
  for(float i=0; i<=1; i+=0.01) {
    X = 3-i;
    Y = 11;
    gerak();
  }
  for(float i=0; i<=1; i+=0.01) {
    X = 2;
    Y = 11-i;
    gerak();
  }
  for(float i=0; i<=1; i+=0.01) {
    X = 2-i;
    Y = 10;
    gerak();
  }
  for(float i=0; i<=1; i+=0.01) {
    X = 1;
    Y = 10-i;
    gerak();
  }
  for(float i=0; i<=1; i+=0.01) {
    X = 1+i;
    Y = 9;
    gerak();
  }
  for(float i=0; i<=1; i+=0.01) {
    X = 2;
    Y = 9-i;
    gerak();
  }
  for(float i=0; i<=1; i+=0.01) {
    X = 2+i;
    Y = 8;
    gerak();
  }
  for(float i=0; i<=1; i+=0.01) {
    X = 3;
    Y = 8+i;
    gerak();
  }
  for(float i=0; i<=1; i+=0.01) {
    X = 3+i;
    Y = 9;
    gerak();
  }
}

void persegipanjang() {
  for (float i = 0; i <= 5; i += 0.01) {
    X = i;
    Y = 10;
    gerak();
  }
  for (float i = 0; i <= 1; i += 0.01) {
    X = 5;
    Y = 10 - i;
    gerak();
  }
  for (float i = 0; i <= 5; i += 0.01) {
    X = 5 - i;
    Y = 9;
    gerak();
  }
  for (float i = 0; i <= 1; i += 0.01) {
    X = 0;
    Y = 9 + i;
    gerak();
  }
}

void trajectoryMove() {
  // Posisi awal (set servo di posisi awal)
  Dxl.setPosition(ID_NUM1, startPos1, velocity); 
  Dxl.setPosition(ID_NUM2, startPos2, velocity);
  delay(5000);  // Delay untuk memastikan servo mulai dari posisi awal

  // Hitung jarak pergerakan per trajectory
  float step1 = (startPos1 - endPos1) / trajectory;  // Langkah posisi untuk servo 1
  float step2 = (endPos2 - startPos2) / trajectory;  // Langkah posisi untuk servo 2

  // Loop untuk menggerakkan servo melalui beberapa trajectory
  for (int i = 1; i <= trajectory; i++) {
    float currentPos1 = startPos1 - (step1 * i);  // Posisi servo 1 untuk trajectory ke-i
    float currentPos2 = startPos2 + (step2 * i);  // Posisi servo 2 untuk trajectory ke-i

    Dxl.setPosition(ID_NUM1, currentPos1, velocity);
    Dxl.setPosition(ID_NUM2, currentPos2, velocity);

    delay(trajectoryDelay);  // Delay antara setiap langkah trajectory
  }
}

void loop() {
  if (SerialUSB.available()) {
    char Cmd = SerialUSB.read();
    SerialUSB.print("Received command: ");
    SerialUSB.println(Cmd);

    if (Cmd == 'A') {
      trajectoryMove();  // Gerakan setengah lingkaran
      
    } else if (Cmd == 'B') {
      tambah();  // Gerakan tambah
      
    } else if (Cmd == 'C') {
      persegipanjang();  // Gerakan persegi panjang
      
    } else if (Cmd == 'R') {
      Dxl.setPosition(ID_NUM1, 805, 50);  // Posisi nol (reset)
      Dxl.setPosition(ID_NUM2, 512, 50);
    }
  }
}
