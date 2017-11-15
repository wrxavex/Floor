//zero. 2017/10/21 V6.. 2017/10/30 v7 .. 1.ON led, 2. wait 9s,3.off led
// ======================================================================== //
//  Pin setup:                                                              //
//                                                                          //
// -------                                                                  //
// ARDUINO|                           PCA9685|                              //
//     GND|                              GND |                              //
//      5V|                              VCC |                              //
//      19|A5                            SCL |      A5 Arduino              //
//      18|A4                          　SDA |      A4 Arduino              //
//      17|                             pwm0 |      R_1                     //
//      16|                             pwm1 |      G_1                     //
//      15|                             pwm2 |      B_1                     //
//      14|                             pwm3 |      R_2                     //
//      13|                             pwm4 |      G_2                     //
//      12|                             pwm5 |      B_2                     //
//      11|                             pwm6 |      R_3                     //
//      10|                             pwm7 |      G_3                     //
//       9|                             pwm8 |      B_3                     //
//       8|                             pwm9 |      R_4                     //
//       7|                            pwm10 |      G_4                     //
//       6|                            pwm11 |      B_4                     //
//       5|                            pwm12 |      R_5                     //
//       4|                            pwm13 |      G_5                     //
//       3|                            pwm14 |      B_5                     //
//       2|                            pwm15 |    < NO use                  //
//       1|                                                                 //
//       0|                                                                 //
// -------                                                                  //
//                                                                          //
//                                                                          //
//     notice: We use TIP 120 MOSFET: use 220 risistor                      //
//  -  Put the longer leg (anode) of the LEDs in VCC and the shorter leg    //
//          (cathode) in OUT(0-15). notice: we NO use pin 15                //
//                                                                          //
//  -  +5V from Arduino -> VCC   (VCC)                                      //
//  -  GND from Arduino ->       (GND)                                      //
//  -                                                                       //
//     use 4 Pca9685 0x40+0x41+0x42+0x43                                    //
//     setColor = 顏色設定 > setColor(顏色碼, 最大值, 第幾階, 哪個板子);  = set 1.COLORS && 2.bigValue = [0] && 3.now floor && 4.pca
//     rgbBiggest = 色碼判斷 最大值 > 色彩呈現才能在後面以倍速去追慢的色碼 = 色彩較接近當初的設定色，否則會有點色差
//     指標應用請參考 練習指標的程式..


#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
//Adafruit_PWMServoDriver pca[4] = {0x40, 0x41, 0x42, 0x43};

class RGB {

  private:
    uint8_t num, iNum; //  PCB編號 & 腳位
    int colorS; // 起始顏色
    int unsigned long  lastOn, lastOff;
    int onTime, offTime;
    int j, j2, valueBig; // pwm
    boolean finState;
  public:
    RGB(uint8_t num_, uint8_t iNum_, uint8_t colorS_) {
      colorS = colorS_;
      num = num_;
      iNum = iNum_;
      lastOn = 0;
      lastOff = 0;
      onTime = 100;
      offTime = 100;
      j = 0;
      j2 = 0;
      valueBig = 0;
      finState = false;
    }
    void changValue(int T_on, int T_off) {
      onTime = T_on;
      offTime = T_off;
    }

    void boolValue(boolean t) {

      finState = t ;
    }

    int* switchColor(uint8_t *COLORNum, int rgbColor[]) {

      rgbColor[0] = map(COLORNum[0], 0, 255, 0, 4095); //R
      rgbColor[1] = map(COLORNum[1], 0, 255, 0, 4095); //G
      rgbColor[2] = map(COLORNum[2], 0, 255, 0, 4095); //B
      rgbColor[3] = map(COLORNum[3], 0, 255, 0, 4095); //Max
      //      Serial.print("R: "); Serial.print(rgbColor[0]); Serial.print(",G: ");    Serial.print(rgbColor[1]); Serial.print(",B: ");    Serial.print(rgbColor[2]);
      //      Serial.print(" | ");// need use Serial....
      return rgbColor;
    }

    uint8_t* colorTotal(uint8_t inC[], int num) {

      uint8_t I[3] = {255, 5, 0}; // 1
      uint8_t II[3] = {254, 74, 0}; //2
      uint8_t III[3] = {254, 143, 0}; //3
      uint8_t IV[3] = {254, 205, 0}; //4
      uint8_t V[3] = {246, 237, 2}; //5
      uint8_t VI[3] = { 201, 240, 16}; //6
      uint8_t VII[3] = { 122, 246, 39}; //7
      uint8_t VIII[3] = { 30, 253, 66}; //8
      uint8_t IX[3] = {0, 240, 98 }; //9
      uint8_t X[3] = { 0, 212, 142}; //10
      uint8_t XI[3] = {0, 176, 198 }; //11
      uint8_t XII[3] = {1, 135, 251 }; //12
      uint8_t XIII[3] = {12, 103, 214 }; //13
      uint8_t XIV[3] = {22, 73, 179 }; //14
      uint8_t XV[3] = { 30, 51, 154}; //15
      uint8_t XVI[3] = {42, 33, 137 }; //16
      uint8_t XVII[3] = { 66, 30, 145}; //17
      uint8_t XVIII[3] = { 107, 25, 158}; //18
      uint8_t XIX[3] = {152, 20, 172 }; //19
      /*
        uint8_t I[3] = {22, 73, 179 }; // 1
        uint8_t II[3] = {22, 73, 179 }; //2
        uint8_t III[3] = {22, 73, 179 }; //3
        uint8_t IV[3] = {22, 73, 179 }; //4
        uint8_t V[3] = {22, 73, 179 }; //5
        uint8_t VI[3] = {22, 73, 179 }; //6
        uint8_t VII[3] = {22, 73, 179 }; //7
        uint8_t VIII[3] = {22, 73, 179 }; //8
        uint8_t IX[3] = {22, 73, 179 }; //9
        uint8_t X[3] = {22, 73, 179 }; //10
        uint8_t XI[3] = {22, 73, 179 }; //11
        uint8_t XII[3] = {22, 73, 179 }; //12
        uint8_t XIII[3] = {22, 73, 179 }; //13
        uint8_t XIV[3] = {22, 73, 179 }; //14
        uint8_t XV[3] = {22, 73, 179 }; //15
        uint8_t XVI[3] = {22, 73, 179 }; //16
        uint8_t XVII[3] = {22, 73, 179 }; //17
        uint8_t XVIII[3] = {22, 73, 179 }; //18
        uint8_t XIX[3] = {22, 73, 179 }; //19*/
        
      uint8_t *COLORS[19] = {I, II, III, IV, V, VI, VII, VIII, IX, X, XI, XII, XIII, XIV, XV, XVI, XVII, XVIII, XIX};

      inC[0]  = COLORS[num][0];       inC[1]  = COLORS[num][1];       inC[2]  = COLORS[num][2];

      uint8_t maxNum = inC[0]; // if it is max
      for (int i; i < 3 ; i++)   if (inC[i] > maxNum)  maxNum = inC[i];       //--1 if send array into there  ---  // uint8_t* colorTotal(uint8_t inC[]) {  //  need return inC;
      inC[3] = maxNum;

      return inC ;
    }

   

    void getData() { // try function.. noUse
      uint8_t Z[3];
      uint8_t *C;
      if (colorS > 19) colorS = 0;
      if (millis() - lastOn >= 1) {
        lastOn = millis();
        C = colorTotal( Z , colorS);
        Serial.print(C[0]); Serial.print(F(" , ")); Serial.print( C[1]); Serial.print(F(" , ")); Serial.print( C[2]); Serial.println();
        colorS++;
      }
    }

    void rise( Adafruit_PWMServoDriver pca) { //** //      肉醬麵寫法.. int r, g, b;      //      int R, G, B;        //      r = int(COLORS[colorS][0]);// 0-18        //      g = int(COLORS[colorS][1]);// 0-18        //      b = int(COLORS[colorS][2]);// 0-18
      uint8_t Z[4]; // z[0] = r, z[1] = g, z[2] = b, z[3] = MaxValue.
      uint8_t *C;
      int rgbColor[3];
      int *Color;  // 255 to 4095

      if (colorS >= 19 ) colorS = 0; // need outside.. can't inside..

      // if (millis() - lastOn >= (onTime-random(80,100))) { // play other
      if (millis() - lastOn >= (onTime)) {
        lastOn = millis();
        C = colorTotal( Z , colorS);// getColor   //uint8_t Z[3];     uint8_t *C;    |  //Serial.print(C[0]); Serial.print(F(" , ")); Serial.print( C[1]); Serial.print(F(" , ")); Serial.print( C[2]);  Serial.print(", max: ");Serial.print(C[3]); Serial.println(F(""));
        Color = switchColor(C, rgbColor); //255 to 4095 初次:需要全域變數去接..若在內部接 宣告會出錯
        //info();
        pca.setPWM(iNum, 0, Color[0]);
        pca.setPWM(iNum + 1, 0, Color[1]);
        pca.setPWM(iNum + 2, 0, Color[2]);
        colorS++;
      }
    }

    void down( Adafruit_PWMServoDriver pca) {
      pca.setPWM(iNum, 0, 0);
      pca.setPWM(iNum + 1, 0, 0);
      pca.setPWM(iNum + 2, 0, 0);
    }



    //boolean readyPWM(Adafruit_PWMServoDriver pca, int spd, boolean finState, int p) { // finState = 0
    boolean readyPWM(Adafruit_PWMServoDriver pca, int spd, int p) { // finState = 0
      uint8_t Z[4]; // z[0] = r, z[1] = g, z[2] = b, z[3] = MaxValue.
      uint8_t *C;
      int rgbColor[4];
      int *Color;  // 255 to 4095
      boolean stateNow;
      int R, G, B, Max;
      int r, g , b;



      C = colorTotal( Z , colorS);// getColor   //uint8_t Z[3];     uint8_t *C;    |  //Serial.print(C[0]); Serial.print(F(" , ")); Serial.print( C[1]); Serial.print(F(" , ")); Serial.print( C[2]);  Serial.print(", max: ");Serial.print(C[3]); Serial.println(F(""));
      Color = switchColor(C, rgbColor); //255 to 4095 初次:需要全域變數去接..若在內部接 宣告會出錯
      R = Color[0]; G = Color[1]; B = Color[2]; Max = Color[3];

      // r = Color[0]/t; g = Color[1] / t; b = Color[2]/t;

      // if (millis() - lastOn >= (onTime-random(80,100))) { // play other
      if ((millis() - lastOn >= onTime) && (finState == false)) {
        lastOn = millis();

        j += spd;

        if ((j < Max) ) {
          if ((R == Max) && (valueBig <= R)) {
            valueBig = j * 2;
            pca.setPWM(iNum, 0, valueBig);
          } else if ( j <= R) {
            pca.setPWM(iNum, 0, j);
          }
          if (G == Max && (valueBig <= G)) {
            valueBig = j * 2;
            pca.setPWM(iNum + 1, 0 , valueBig);
          } else if (j <= G)  {
            pca.setPWM(iNum + 1, 0, j); // g
          }
          if (B == Max && (valueBig <= B) ) {
            valueBig = j * 2;
            pca.setPWM(iNum + 2, 0, valueBig);
          } else if (j <= B) {
            pca.setPWM(iNum + 2, 0, j); //b
          }

          //easy..
          //          if (j <=  R) pca.setPWM(iNum, 0, j);
          //          if (j <=  G) pca.setPWM(iNum + 1, 0, j);
          //          if (j <=  B) pca.setPWM(iNum + 2, 0, j);
          finState = false;
          return finState;
        } else if (j >= Max) {
          finState = true;  //Serial.println(finState);
          return finState;
        }
      }
      return finState;
    }




    // boolean readyPWMOff(Adafruit_PWMServoDriver pca, int spd, boolean finState, int p) { // 我將finState改成全域變數
    boolean readyPWMOff(Adafruit_PWMServoDriver pca, int spd, int p) { // finState = 1
      uint8_t Z[4]; // z[0] = r, z[1] = g, z[2] = b, z[3] = MaxValue.
      uint8_t *C;
      int rgbColor[4];
      int *Color;  // 255 to 4095
      boolean stateNow;
      int R, G, B, Max;
      int minR = R, minG = G, minB = B;
      //Serial.println(p);
      C = colorTotal( Z , colorS);// getColor   //uint8_t Z[3];     uint8_t *C;    |  //Serial.print(C[0]); Serial.print(F(" , ")); Serial.print( C[1]); Serial.print(F(" , ")); Serial.print( C[2]);  Serial.print(", max: ");Serial.print(C[3]); Serial.println(F(""));
      Color = switchColor(C, rgbColor); //255 to 4095 初次:需要全域變數去接..若在內部接 宣告會出錯
      R = Color[0]; G = Color[1]; B = Color[2]; Max = Color[3];

      if ((millis() - lastOn >= offTime) && (finState == true)) {
        lastOn = millis();
        j2 += spd;
        /*
          if (j2 <= Max) {
          if (R > 200) minR = R - j2 * 2;
          else   minR = R - j2;
          if (G >  200)  minG = G - j2 * 2;
          else minG = G - j2;
          if (B > 200) minB = B - j2 * 2;
          else minB = B - j2;

          if (minR >=  0) pca .setPWM(iNum, 0, minR);  // Serial.print("downR: "); Serial.print(minR);
          else pca .setPWM(iNum, 0, 0);     // Close R
          if (minG >=  0) pca.setPWM(iNum + 1, 0, minG);  //Serial.print(", downG: "); Serial.print(minG);
          else pca .setPWM(iNum + 1, 0, 0); // Close G
          if (minB >=  0) pca.setPWM(iNum + 2, 0, minB);  //Serial.print(", downB: "); Serial.print(minB);
          else pca .setPWM(iNum + 2, 0, 0); // Close B
          return  finState;
          } else if (j2 > Max) {
          pca .setPWM(iNum, 0, 0);     // Close R
          pca .setPWM(iNum + 1, 0, 0); // Close G
          pca .setPWM(iNum + 2, 0, 0); // Close B
          j = 0;
          j2 = 0;
          finState = false;
          return  finState;
          }
        */

        j2 += spd;
        if (j2 <= 4095) {
          minR = R - j2;
          minG = G - j2;
          minB = B - j2;
          if (minR >=  0) pca .setPWM(iNum, 0, minR);  // Serial.print("downR: "); Serial.print(minR);
          else pca .setPWM(iNum, 0, 0);     // Close R
          if (minG >=  0) pca.setPWM(iNum + 1, 0, minG);  //Serial.print(", downG: "); Serial.print(minG);
          else pca .setPWM(iNum + 1, 0, 0); // Close G
          if (minB >=  0) pca.setPWM(iNum + 2, 0, minB);  //Serial.print(", downB: "); Serial.print(minB);
          else pca .setPWM(iNum + 2, 0, 0); // Close B
          finState = true;
          return  finState;
        } else if (j2 > 4095) {
          pca .setPWM(iNum, 0, 0);     // Close R
          pca .setPWM(iNum + 1, 0, 0); // Close G
          pca .setPWM(iNum + 2, 0, 0); // Close B
          j = 0;
          j2 = 0;
          finState = false;
          return  finState;
        }
      }
      return finState;

    }




    void info(uint8_t maxNum_ , uint8_t minNum_ , uint8_t innerN_0 , uint8_t innerN_1, uint8_t rgb[] ) {//
      Serial.print(F("TotalSum: ")); Serial.println(rgb[0] + rgb[1] + rgb[2]);
      Serial.print(F("rgb1: ")); Serial.print(rgb[0]); Serial.print(F(", rgb2: ")); Serial.print(rgb[1]); Serial.print(F(", rgb3: ")); Serial.print(rgb[2]);
      Serial.print(F(":  max = ")); Serial.print(maxNum_); Serial.print(F(" , ")); Serial.print(F("min = ")); Serial.print(minNum_);
      Serial.print(F(" , "));  Serial.print(F("innerN[0] = ")); Serial.print(innerN_0);
      Serial.print(F(" , "));  Serial.print(F("innerN[1] = ")); Serial.print(innerN_1);
      Serial.println(F(""));
    }



    void info() {
      //        Serial.print("colorNum: "); Serial.println(colorS);
      //        Serial.print("R_: "); Serial.print(r);
      //        Serial.print(", G_: "); Serial.print(g);
      //        Serial.print(", B_: "); Serial.println(b);
      //
      //        Serial.print("R_: "); Serial.print(R);
      //        Serial.print(", G_: "); Serial.print(G);
      //        Serial.print(", B_: "); Serial.print(B);
      //        Serial.println("....");
      //        Serial.println("************");
    }


    boolean tryPWM(Adafruit_PWMServoDriver pca, int spd, int i) { // finState = 0 //pca[0], 4000, finState[0], 0
      //void  tryPWM(Adafruit_PWMServoDriver pca, int spd, int i) { // finState = 0 //pca[0], 4000, finState[0], 0
      uint8_t Z[4]; // z[0] = r, z[1] = g, z[2] = b, z[3] = MaxValue.
      uint8_t *C;
      int rgbColor[4];
      int *Color;  // 255 to 4095
      boolean stateNow;
      int R, G, B, Max;
      int minR = R, minG = G, minB = B;
      //colorS = 0; // ...test
      //if (colorS >= 19 ) colorS = 0; // need outside.. can't inside..
      C = colorTotal( Z , colorS);// getColor   //uint8_t Z[3];     uint8_t *C;    |  //Serial.print(C[0]); Serial.print(F(" , ")); Serial.print( C[1]); Serial.print(F(" , ")); Serial.print( C[2]);  Serial.print(", max: ");Serial.print(C[3]); Serial.println(F(""));
      Color = switchColor(C, rgbColor); //255 to 4095 初次:需要全域變數去接..若在內部接 宣告會出錯
      R = Color[0]; G = Color[1]; B = Color[2]; Max = Color[3];
      //Serial.println(finState);

      //****************finState必須是全域變數
      if ((millis() - lastOn >= onTime) && (finState == false)) { //stateNow == false
        lastOn = millis();
        if (j < 4095) {
          j += spd;
          if (j <=  R) pca.setPWM(iNum, 0, j);
          if (j <=  G) pca.setPWM(iNum + 1, 0, j);
          if (j <=  B) pca.setPWM(iNum + 2, 0, j);
          finState = false;
          return finState;
        } else if (j >= 4095) {//Serial.print("finState:");Serial.print(finState); Serial.println("");   //spd = 0;     //try..
          finState = true;
          return finState;
        }
      } else if ((millis() - lastOn >= offTime) && (finState == true)) {
        lastOn = millis();
        // if (j >= 4095) { //Serial.print("j----: "); Serial.print(j);
        if (j2 <= 4095) { // j <= MAX
          j2 += spd;
          minR = R - j2;
          minG = G - j2;
          minB = B - j2;
          if (minR >=  0) pca .setPWM(iNum, 0, minR);  // Serial.print("downR: "); Serial.print(minR);
          else pca .setPWM(iNum, 0, 0);     // Close R
          if (minG >=  0) pca.setPWM(iNum + 1, 0, minG);  //Serial.print(", downG: "); Serial.print(minG);
          else pca .setPWM(iNum + 1, 0, 0); // Close G
          if (minB >=  0) pca.setPWM(iNum + 2, 0, minB);  //Serial.print(", downB: "); Serial.print(minB);
          else pca .setPWM(iNum + 2, 0, 0); // Close B
          return  finState;
        } else if (j2 > 4095) {
          pca .setPWM(iNum, 0, 0);     // Close R
          pca .setPWM(iNum + 1, 0, 0); // Close G
          pca .setPWM(iNum + 2, 0, 0); // Close B
          j = 0;
          j2 = 0;
          finState = false;
          // delay(100);
          return  finState;
        }
      }
      return finState;
    }


};

Adafruit_PWMServoDriver pca[4] = {0x40, 0x41, 0x42, 0x43};

//uint8_t z[] = {255, 0, 0};
uint8_t *C ;
uint8_t inC[3];
//uint8_t *Z[2];


uint8_t spd = 4; //light pwm speed..
int valueBig;
boolean finish = false;
int now = 0;
int next = 0;
int total = 15; //need change..11
int rise = 1 , reverse = 1;
int pcaNum = 0;


boolean finState[19];
boolean stateNow[19];
boolean sw = 0;
int unsigned long last = 0;
int unsigned long last2 = 0;
int timer1 = 1000;
int timer2 = 1000;
uint8_t count = 0;

RGB One(0, 0, 0); //pcb array , floor  // pca[num] = Adafruit_PWMServoDriver(board);
RGB Two(0, 3, 1);
RGB Three(0, 6, 2);
RGB Four(0, 9, 3);
RGB Five(0 , 12, 4);
RGB Six(1, 0, 5);
RGB Seven(1, 3, 6);
RGB Eight(1, 6, 7);
RGB Nine(1, 9, 8);
RGB Ten(1, 12, 9);
RGB El(2, 0, 10); //Eleven
RGB Tw(2, 3, 11); //Twelve
RGB Th(2, 6, 12); //Thirteen
RGB Fo(2, 9, 13); //Fourteen
RGB Fif(2, 12, 14); //Fifteen
RGB Sixt(3, 0, 15); //Sixteen
RGB Se(3, 3, 16); //Seventeen
RGB Ei(3, 6, 17); //Eighteen
RGB Ni(3, 9, 18); //geteen
RGB Floor[19] = {One, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, El, Tw, Th, Fo, Fif, Sixt, Se, Ei, Ni};
//---------------------------******************************--------------------******************

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 19 ; i++) finState[i] = false;
  for (int i = 0; i < 19 ; i++) stateNow[i] = false;

  for (int i = 0;  i < 4 ; i++) {
    pca[i].begin();
    pca[i].setPWMFreq(1600);
  }
  uint8_t twbrbackup = TWBR; // save I2C bitrate
  TWBR = 12; // upgrade to 400KHz! // must be changed after calling Wire.begin() (inside pwm.begin())
  for (int i=0; i<19;i++){
    Floor[i].changValue(200, 200);
  }
}

void loop() {



  // tryPWMOff();   //測試PWM OFF

  // PWMing();    //19顆燈的PWM
  PWMing_ONE(); // 一顆燈的PWM
  // rgbPWM_TWO();  //rgbPWM();  //以上兩個function 都是內部PWM 自我循環沒傳值出來
  //rgbALL();        //彩虹燈程式(沒有pwm)
  //rgbOFF();      // 全部燈關閉
  //
  //switchTest();


}
void PWMing() {

  for (int i = 0; i < 19; i++) {
    if (i >= 0 && i < 5) pcaNum = 0;
    else if (i >= 5 && i < 10) pcaNum = 1;
    else if (i >= 10 && i < 15) pcaNum = 2;
    else if (i >= 15 && i < 19) pcaNum = 3;

    Floor[i].changValue(10, 10);
    if (stateNow[i] == false) {
      //Serial.print(F("stateNow-false: ")); Serial.print(stateNow[1]); Serial.println("");
      stateNow[i] = Floor[i].readyPWM(pca[pcaNum], 100, 1); // (Adafruit_PWMServoDriver pca, int spd)
    }  else if (stateNow[0] == true) {
      //Serial.print(F("-----------------stateNow-true: ")); Serial.print(stateNow[1]); Serial.println("");
      stateNow[i] = Floor[i].readyPWMOff(pca[pcaNum], 100, 1); // (Adafruit_PWMServoDriver pca, int spd)
    }
  }

}

void PWMing_ONE() {
  Floor[1].changValue(35, 35);
  if (stateNow[1] == false) {
    Serial.print(F("stateNow-false: ")); Serial.print(stateNow[1]); Serial.println("");
    stateNow[1] = Floor[1].readyPWM(pca[0], 100, 1); // (Adafruit_PWMServoDriver pca, int spd)
  }  else if (stateNow[1] == true) {
    Serial.print(F("-----------------stateNow-true: ")); Serial.print(stateNow[1]); Serial.println("");
    stateNow[1] = Floor[1].readyPWMOff(pca[0], 100, 1); // (Adafruit_PWMServoDriver pca, int spd)
  }


}

void  rgbPWM_TWO() {
  for (int i = 0; i < 19; i++) {
    if (i >= 0 && i < 5) pcaNum = 0;
    else if (i >= 5 && i < 10) pcaNum = 1;
    else if (i >= 10 && i < 15) pcaNum = 2;
    else if (i >= 15 && i < 19) pcaNum = 3;
    // Floor[i].changValue(10, 10);
    stateNow[i] =  Floor[i].tryPWM(pca[pcaNum], 100, 0);
  }
}

void rgbPWM() {
  for (int i = 0; i < 19 ; i++)  Floor[i].changValue(10, 10);
  stateNow[0] =  Floor[0].tryPWM(pca[0], 100, 0);
  stateNow[1] =  Floor[1].tryPWM(pca[0], 100, 0);
  stateNow[2] =  Floor[2].tryPWM(pca[0], 100, 0);
  stateNow[3] =  Floor[3].tryPWM(pca[0], 100, 0);
  stateNow[4] =  Floor[4].tryPWM(pca[0], 100, 0);
  stateNow[5] =  Floor[5].tryPWM(pca[1], 100, 0);
  stateNow[6] =  Floor[6].tryPWM(pca[1], 100, 0);
  stateNow[7] =  Floor[7].tryPWM(pca[1], 100, 0);
  stateNow[8] =  Floor[8].tryPWM(pca[1], 100, 0);
  stateNow[9] =  Floor[9].tryPWM(pca[1], 100, 0);
  stateNow[10] =  Floor[10].tryPWM(pca[2], 100, 0);
  stateNow[11] =  Floor[11].tryPWM(pca[2], 100, 0);
  stateNow[12] =  Floor[12].tryPWM(pca[2], 100, 0);
  stateNow[13] =  Floor[13].tryPWM(pca[2], 100, 0);
  stateNow[14] =  Floor[14].tryPWM(pca[2], 100, 0);
  stateNow[15] =  Floor[15].tryPWM(pca[3], 100, 0);
  stateNow[16] =  Floor[16].tryPWM(pca[3], 100, 0);
  stateNow[17] =  Floor[17].tryPWM(pca[3], 100, 0);
  stateNow[18] =  Floor[18].tryPWM(pca[3], 100, 0);



}

void rgbALL() {
  Floor[0].rise(pca[0]);
  Floor[1].rise(pca[0]);
  Floor[2].rise(pca[0]);
  Floor[3].rise(pca[0]);
  Floor[4].rise(pca[0]);
  Floor[5].rise(pca[1]);
  Floor[6].rise(pca[1]);
  Floor[7].rise(pca[1]);
  Floor[8].rise(pca[1]);
  Floor[9].rise(pca[1]);
  Floor[10].rise(pca[2]);
  Floor[11].rise(pca[2]);
  Floor[12].rise(pca[2]);
  Floor[13].rise(pca[2]);
  Floor[14].rise(pca[2]);
  Floor[15].rise(pca[3]);
  Floor[16].rise(pca[3]);
  Floor[17].rise(pca[3]);
  Floor[18].rise(pca[3]);
}

void rgbOFF() {
  Floor[0].down(pca[0]);
  Floor[1].down(pca[0]);
  Floor[2].down(pca[0]);
  Floor[3].down(pca[0]);
  Floor[4].down(pca[0]);
  Floor[5].down(pca[1]);
  Floor[6].down(pca[1]);
  Floor[7].down(pca[1]);
  Floor[8].down(pca[1]);
  Floor[9].down(pca[1]);
  Floor[10].down(pca[2]);
  Floor[11].down(pca[2]);
  Floor[12].down(pca[2]);
  Floor[13].down(pca[2]);
  Floor[14].down(pca[2]);
  Floor[15].down(pca[3]);
  Floor[16].down(pca[3]);
  Floor[17].down(pca[3]);
  Floor[18].down(pca[3]);
}


void tryPWMOff() {

  for (int i = 0; i < 19; i++) {
    Floor[i].changValue(20, 20);
    Floor[i].boolValue(true);
    if (i >= 0 && i < 5) pcaNum = 0;
    else if (i >= 5 && i < 10) pcaNum = 1;
    else if (i >= 10 && i < 15) pcaNum = 2;
    else if (i >= 15 && i < 19) pcaNum = 3;
    Floor[i].readyPWMOff(pca[pcaNum], 100, 1); // (Adafruit_PWMServoDriver pca, int spd)
  }
}


void switchTest() {

  if ((millis() - last >= timer1) && (sw == 1)) {
    //Serial.print("on: "); Serial.println(last);
    last = millis();
    //    One.colorT(COLORS[count]);
    Floor[0].rise(pca[0]);
    Floor[1].rise(pca[0]);
    Floor[2].rise(pca[0]);
    Floor[3].rise(pca[0]);
    Floor[4].rise(pca[0]);
    Floor[5].rise(pca[1]);
    //count++;
    sw = 0;
  }  else if ((millis() - last >= timer2) && (sw == 0)) {
    //Serial.print("off: "); Serial.println(last);
    last = millis();
    Floor[0].down(pca[0]);
    Floor[1].down(pca[0]);
    Floor[2].down(pca[0]);
    Floor[3].down(pca[0]);
    Floor[4].down(pca[0]);
    Floor[5].down(pca[1]);
    sw = 1;
  }


}

/*
  // led , pca[]change
  boolean setColor(int* color, int maxValue, int i, int now_, int rise) {
  int r = color[0];//0-255
  int g = color[1];//0-255
  int b = color[2];//0-255
  boolean state = false; // call outside it finish
  int R, G, B, MAX;
  R = map(r, 0, 255, 0, 4095);
  G = map(g, 0, 255, 0, 4095);
  B = map(b, 0, 255, 0, 4095);
  MAX = map(maxValue, 0, 255, 0, 4095);
  // int minR = R, minG = G, minB = B;

  Serial.print(rise);
  if (rise == 1) riseLed (i, now_, R, G, B, MAX); // 1
  else  downLed(i, now_, R, G, B,  MAX); //2

  state = true;
  return state;
  }


  void riseLed(int i, int now_, int R, int G, int B, int MAX) {
  //判斷 內色碼最大值 然後倍速去呈現.
  for (int j = 0; j <= MAX; j += spd) { // max .. 有點變速 .. 可改回 4095
  if (R == MAX) {
  valueBig = j * 8;
  if (valueBig <= R)  pca[now_].setPWM(i, 0, valueBig);
  } else {
  if (j <= R ) pca[now_].setPWM(i, 0, j); //r
  }
  if (G == MAX) {
  valueBig = j * 8;
  if (valueBig <= G)  pca[now_].setPWM(i + 1, 0 , valueBig);
  } else {
  if (j <= G)  pca[now_].setPWM(i + 1, 0, j); // g
  }
  if (B == MAX) {
  valueBig = j * 8;
  if (valueBig <= B) pca[now_].setPWM(i + 2, 0, valueBig);
  } else {
  if (j <= B)  pca[now_].setPWM(i + 2, 0, j); //b
  }
  }

  }


  void downLed(int i, int now_, int R, int G, int B, int MAX) {
  int minR = R, minG = G, minB = B;
  //pwm(lewss..)
  for (int j = 0; j <= MAX; j += spd) { // max .. 有點變速 .. 可改回 4095
  minR = R - j;
  minG = G - j;
  minB = B - j;

  if (minR >=  0)  {
  pca[now_].setPWM(i, 0, minR);
  // Serial.print("downR: "); Serial.print(minR);
  }
  if (minG >=  0)  {
  pca[now_].setPWM(i + 1, 0, minG);
  //Serial.print(", downG: "); Serial.print(minG);
  }
  if (minB >=  0)  {
  pca[now_].setPWM(i + 2, 0, minB);
  //Serial.print(", downB: "); Serial.print(minB);
  }
  //Serial.println("...");
  }
  pca[now_].setPWM(i, 0, 0);     // Close R
  pca[now_].setPWM(i + 1, 0, 0); // Close G
  pca[now_].setPWM(i + 2, 0, 0); // Close B
  }




  void allOFF() {
  for (int j = 0 ; j < 15 ; j++) {
  pca[0].setPWM(j, 0, 0);     pca[1].setPWM(j, 0, 0);
  pca[2].setPWM(j, 0, 0);     pca[3].setPWM(j, 0, 0);
  }
  }



*/



/*


  COLORS[0] = I;  COLORS[1] = II; COLORS[2] = III; COLORS[3] = IV; COLORS[4] = V; COLORS[5] = VI; COLORS[6] = VII;
  COLORS[7] = VIII;  COLORS[8] = IX; COLORS[9] = X; COLORS[10] = XI; COLORS[11] = XII; COLORS[12] = XIII; COLORS[13] = XIV;
  COLORS[14] = XV;  COLORS[15] = XVI; COLORS[16] = XVII; COLORS[17] = XVIII; COLORS[18] = XIX;

  uint8_t I[3] ;
  uint8_t II[3] ;
  uint8_t III[3] ;
  uint8_t IV[3];
  uint8_t V[3] ;
  uint8_t VI[3] ;
  uint8_t VII[3] ;
  uint8_t VIII[3];
  uint8_t IX[3] ;
  uint8_t X[3] ;
  uint8_t XI[3] ;
  uint8_t XII[3] ;
  uint8_t XIII[3];
  uint8_t XIV[3] ;
  uint8_t XV[3] ;
  uint8_t XVI[3];
  uint8_t XVII[3];
  uint8_t XVIII[3];
  uint8_t XIX[3];
  for (int i; i < 3; i++) {
  I[i] = I_[i];     II[i] = II_[i];     III[i] = III_[i];     IV[i] = IV_[i];    V[i] = V_[i];
  VI[i] = VI_[i];     VII[i] = VII_[i];     VIII[i] = VIII_[i];     IX[i] = IX_[i];     X[i] = X_[i];
  XI[i] = XI_[i];     XII[i] = XII_[i];     XIII[i] = XIII_[i];     XIV[i] = XIV_[i];     XV[i] = XV_[i];
  XVI[i] = XVI_[i];     XVII[i] = XVII_[i];     XVIII[i] = XVIII_[i];     XIX[i] = XIX_[i];
  }
  Serial.println(VI[0]); Serial.println(VI[1]); Serial.println(VI[2]);









        uint8_t rgbBiggest(uint8_t rgb[]) {  // choice bigest Value
      uint8_t maxNum = rgb[0]; // if it is max
      //uint8_t minNum = rgb[0]; // if it is mini
      //uint8_t innerN[2];

      for (int i; i < 3 ; i++) {
        if (rgb[i] > maxNum)  maxNum = rgb[i];
        //bigSmall[i] = &rgb[i];
        //if (rgb[i] < minNum) minNum = rgb[i];
      }
      //innerN[0] = maxNum;
      //innerN[1] = minNum;
      //info(maxNum, minNum, innerN[0], innerN[1], rgb); //print information..,
      return maxNum;
    }




*/
