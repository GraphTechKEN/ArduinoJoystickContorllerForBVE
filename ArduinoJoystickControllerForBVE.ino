/* BVE向けArduino Leonardo (Micro) Joystick化スケッチ
* 使い方： 
* 1.アナログ入力：A0～A3 ポテンショの電源端子を5V、GNDをGNDに、中位の信号線をA0～A3に入力します。
* 2.デジタル入力：D0～D13,A4,A5が対応、スイッチの片側の信号線をD0～D13,A4,A5に入力、もう片側をGNDに接続します。
* 3.oneHandleInputプラグイン設定(アナログ入力)：A0:X軸、A1:Y軸、A2:Z軸、A3:Rx軸に対応します。それぞれプラグインで方向としきい値を調整してください。
* 4.oneHandleInputプラグイン設定(デジタル入力)：スイッチを配線後、プラグイン内の所望スイッチ(OFF表示)を選択しスイッチを押下し設定(ピン番号を反映)してください。
*
* ArduinoIDEでのスケッチ書き込み方法：Mheironimus/ArduinojoystickLibraryを使用します
* 1.ライブラリをダウンロードします -> https://github.com/MHeironimus/ArduinoJoystickLibrary/archive/master.zip
* 2.Arduino IDE を開き、 スケッチ > ライブラリをインクルード > .ZIP形式のライブラリをインストール を選択し、上記ファイルを選択します
* 3.ボード選択窓から「Arduino Leonardo (もしくはMicro)」表示されない場合は、ツール > ボード > Arduino AVR Boards > Arduino Leonardo または Arduino Micro を選択します。
* 3.本スケッチを書き込みます
*
* 注意事項：
* 1.デジタル入力は全てPULL_UPとしています。OUTPUTに変更した場合、スイッチの押下でショートし破損しますので注意してください。
* 2.このスケッチはマトリクス入力には対応していません、最大16入力となります。
*
* その他：
* スケッチに余裕がありますので、シリアル出力など拡張できるかもしれません。マトリクス回路やIOエキスパンダなどで拡張できるかもしれませんが、入出力などのピン設定をよく確認してください。
*
* V1.0.0.0 リリース
*/

#include <Joystick.h>

// アナログ軸数(4)、デジタルボタン数(16)
Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID, JOYSTICK_TYPE_JOYSTICK,
                   16, 0,                   // ボタン数, ハットスイッチ数
                   true, true, true, true,  // X, Y, Z, Rx 軸を有効化
                   false, false, false,     // Ry, Rz, Rudder 軸を無効化
                   false, false, false);    // スロットル、アクセル、ブレーキ無効

// C#側の変数名（A, B1, B2, B3...）に合わせてArduinoのデジタルピンを定義
const int PIN_A = 0;       // D0 -> switchA
const int PIN_B1 = 1;      // D1 -> switchB1
const int PIN_B2 = 2;      // D2 -> switchB2
const int PIN_C1 = 3;      // D3 -> switchC1
const int PIN_C2 = 4;      // D4 -> switchC2
const int PIN_D = 5;       // D5 -> switchD
const int PIN_E = 6;       // D6 -> switchE
const int PIN_F = 7;       // D7 -> switchF
const int PIN_G = 8;       // D8 -> switchG
const int PIN_H = 9;       // D9 -> switchH
const int PIN_I = 10;      // D10 -> switchI
const int PIN_J = 11;      // D11 -> switchJ
const int PIN_K = 12;      // D12 -> switchK
const int PIN_L = 13;      // D13 -> switchL
const int PIN_Horn1 = 22;  // 22(A4) -> switchHorn1
const int PIN_Horn2 = 23;  // 23(A5) -> switchHorn2


// 割り当て用配列（インデックス 0=A, 1=B1, 2=B2... となるように並べる）
const int buttonPins[] = { PIN_A, PIN_B1, PIN_B2, PIN_C1, PIN_C2, PIN_D, PIN_E, PIN_F, PIN_G, PIN_H, PIN_I, PIN_J, PIN_K, PIN_L, PIN_Horn1, PIN_Horn2 };
const int NUM_BUTTONS = 16;

const int xAxisPinX = A0;   // X軸をA0に接続
const int xAxisPinY = A1;   // Y軸をA1に接続
const int xAxisPinZ = A2;   // Z軸をA2に接続
const int xAxisPinRx = A3;  // Rx軸をA3に接続

void setup() {
  // 0〜13,A4,A5ピンをプルアップ入力に設定
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }

  // ジョイスティック通信を開始
  Joystick.setXAxisRange(0, 65535);
  Joystick.setYAxisRange(0, 65535);
  Joystick.setZAxisRange(0, 65535);
  Joystick.setRxAxisRange(0, 65535);
  Joystick.begin();
  //Serial.begin(); シリアル通信開始命令はJoystuckプラグイン内で実行されているため不要
}

void loop() {
  // アナログ入力読み取り
  int sensorValueX = analogRead(xAxisPinX);
  int sensorValueY = analogRead(xAxisPinY);
  int sensorValueZ = analogRead(xAxisPinZ);
  int sensorValueRx = analogRead(xAxisPinRx);

  bool buttonState[NUM_BUTTONS];

  //アナログ入力デバッグ用
  /*
  Serial.print("X:");
  Serial.print(sensorValueX);
  Serial.print(" Y:");
  Serial.print(sensorValueY);
  Serial.print(" Z:");
  Serial.print(sensorValueZ);
  Serial.print(" Rx:");
  Serial.println(sensorValueRx);
  */

  // ジョイスティック特性へ変換
  uint16_t mappedValueX = map(sensorValueX, 0, 1023, 0, 65535);
  uint16_t mappedValueY = map(sensorValueY, 0, 1023, 0, 65535);
  uint16_t mappedValueZ = map(sensorValueZ, 0, 1023, 0, 65535);
  uint16_t mappedValueRx = map(sensorValueRx, 0, 1023, 0, 65535);

  // PCへ変換値を送信
  Joystick.setXAxis(mappedValueX);
  Joystick.setYAxis(mappedValueY);
  Joystick.setZAxis(mappedValueZ);
  Joystick.setRxAxis(mappedValueRx);

  // デジタル入力(ボタン状態)を順にスキャンして送信
  for (int i = 0; i < NUM_BUTTONS; i++) {
    // 物理的に押された時（LOW）に反転して 1（ON）をPCへ送信
    buttonState[i] = !digitalRead(buttonPins[i]);
    Joystick.setButton(i, buttonState[i]);
  }
 
  //デジタル入力デバッグ用
  /*for (int i = 0; i < NUM_BUTTONS; i++) {
    Serial.print(buttonState[i]); 
  }
  Serial.println(); 
  */
 
  delay(10);
}