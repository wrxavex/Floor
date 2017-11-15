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
    int num, iNum;
    uint8_t colorS; // colorStart
    uint8_t *COLORNum;
    int unsigned long  lastOn, lastOff;
    int onTime, offTime;
    int *Color;
    uint8_t *ColorBit;



  public:
    RGB(uint8_t num_, uint8_t iNum_, uint8_t colorS_) {
      colorS = colorS_;
      num = num_;
      iNum = iNum_;
      lastOn = 0;
      lastOff = 0;
      onTime = 1000;
      offTime = 1000;

    }

    void colorT() {

      //Serial.print("count:"); Serial.println(count);
      //colorR();
      Serial.println("...................");

    }

    void colorR(uint8_t *COLOR_SW) {
      Serial.println(COLOR_SW[0]);
      Serial.println(COLOR_SW[1]);
      Serial.println(COLOR_SW[2]);
    }


    void colorTotal() {


      //Serial.println(*COLORS[18]);

    }

    int* switchColor(uint8_t *COLORNum) {
      uint8_t r, g, b;
      int R, G, B;
      r = int(COLORNum[0]);
      g = int(COLORNum[1]);
      b = int(COLORNum[2]);
      R = map(r, 0, 255, 0, 4095);
      G = map(g, 0, 255, 0, 4095);
      B = map(b, 0, 255, 0, 4095);  //  Serial.println(COLORNum[0]);  //  Serial.println(COLORNum[1]);  //  Serial.println(COLORNum[2]);  //  Serial.println(I[0]);  //  Serial.println(I[1]);  //  Serial.println(I[2]);
      int rgbColor[3] = {R, G, B};
//      Serial.print("R:"); Serial.print(COLORNum[0]);
//      Serial.print(", G:"); Serial.print(COLORNum[1]);
//      Serial.print(", B:"); Serial.print(COLORNum[2]);
      Serial.print("");
      //delayMicroseconds(1000);
      return rgbColor;
    }

    void rise( Adafruit_PWMServoDriver pca) {

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
      uint8_t *COLORS[19] = {I, II, III, IV, V, VI, VII, VIII, IX, X, XI, XII, XIII, XIV, XV, XVI, XVII, XVIII, XIX};


      if (millis() - lastOn >= onTime) {
        lastOn = millis();
        if (colorS >= 19 ) colorS = 0;
         Color = switchColor(COLORS[colorS]); // 需要全域變數去接..若在內部接 宣告會出錯
        Serial.print("colorNum:"); Serial.println(colorS);
        Serial.print("R_:"); Serial.print(Color[0]);
        Serial.print(", G_:"); Serial.print(Color[1]);
        Serial.print(", B_:"); Serial.print(Color[2]);
        Serial.println("....");
       
        pca.setPWM(iNum, 0, Color[0]);
        pca.setPWM(iNum + 1, 0, Color[1]);
        pca.setPWM(iNum + 2, 0, Color[2]);
        colorS++;
      }
      /*
        pca[num].setPWM(iNum, 0, R);
        pca[num].setPWM(iNum + 1, 0, G);
        pca[num].setPWM(iNum + 2, 0, B);
      */
    }

    void down( Adafruit_PWMServoDriver pca) {
      pca.setPWM(iNum, 0, 0);
      pca.setPWM(iNum + 1, 0, 0);
      pca.setPWM(iNum + 2, 0, 0);
    }
};

Adafruit_PWMServoDriver pca[4] = {0x40, 0x41, 0x42, 0x43};



//uint8_t z[] = {255, 0, 0};
//uint8_t *C[] = {z};

int *bigSmall; // it can search max, min function
uint16_t spd = 4; //light pwm speed..
int valueBig;
boolean finish = false;
int now = 0;
int next = 0;
int total = 15; //need change..11
int rise = 1 , reverse = 1;

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
  for (int i = 0;  i < 4 ; i++) {
    pca[i].begin();
    pca[i].setPWMFreq(1600);
  }
  uint8_t twbrbackup = TWBR; // save I2C bitrate
  TWBR = 12; // upgrade to 400KHz! // must be changed after calling Wire.begin() (inside pwm.begin())

}

void loop() {
  //Floor[0].colorTotal();
  //  Floor[0].rise(pca[0],0);
  //  delay(1000);
  if ((millis() - last >= timer1) && (sw == 1)) {
    //Serial.print("on:"); Serial.println(last);
    last = millis();
    //    One.colorT(COLORS[count]);
    Floor[0].rise(pca[0]);
//    Floor[1].rise(pca[0]);
//    Floor[2].rise(pca[0]);
//    Floor[3].rise(pca[0]);
//    Floor[4].rise(pca[0]);
//    Floor[5].rise(pca[1]);
    //count++;
    sw = 0;
  }  else if ((millis() - last >= timer2) && (sw == 0)) {
    //Serial.print("off:"); Serial.println(last);
    last = millis();
    Floor[0].down(pca[0]);
//    Floor[1].down(pca[0]);
//    Floor[2].down(pca[0]);
//    Floor[3].down(pca[0]);
//    Floor[4].down(pca[0]);
//    Floor[5].down(pca[1]);
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

  int* rgbBiggest(int* rgb, int next_) {  // choice bigest Value
  int maxNum = rgb[0]; // if it is max
  int minNum = rgb[0]; // if it is mini
  int innerN[2];
  for (int i; i < 3 ; i++) {
    if (rgb[i] > maxNum) maxNum = rgb[i];
    if (rgb[i] < minNum) minNum = rgb[i];
    //bigSmall[i] = &rgb[i];
  }
  innerN[0] = maxNum; innerN[1] = minNum;
  //info(maxNum, minNum, innerN[0], innerN[1], next ); //print information..
  return innerN;
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


  void info(int maxNum_ , int minNum_ , int innerN_0 , int innerN_1, int nextNum) {
  Serial.print("next : "); Serial.print(next);
  Serial.print(":  max = "); Serial.print(maxNum_); Serial.print(" , "); Serial.print("min = "); Serial.print(minNum_);
  Serial.print(" , ");  Serial.print("innerN[0]= "); Serial.print(innerN_0);
  Serial.print(" , ");  Serial.print("innerN[1]= "); Serial.print(innerN_1);
  Serial.println("");
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
*/
