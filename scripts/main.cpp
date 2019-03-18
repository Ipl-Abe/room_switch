/*
mbed用プログラム
web上のmbedコンパイラを使用する必要がある
*/

#include "mbed.h"
#include "QEI.h"     // エンコーダのライブラリ
#include "VNH5019.h" // モータドライバのライブラリ
#include "Servo.h"   // サーボのライブラリ

Serial pc(USBTX, USBRX, 9600); // tx, rx シリアル通信
QEI wheel (p27,p28,NC,1536); 
VNH5019 motorShield(p20,p22,p25,p23,p24,p26); // mbedのピン番号を指定 モータ1A,モータ1B,モータ1PWM,モータ2A,モータ2B,モータ2PWM
Servo servo(p21); // サーボの初期化
AnalogIn ain(p15); // 力センサ用のピン

DigitalOut myled(LED1);
DigitalIn limit1(p10); // リミットスイッチ検出用ピン

char buf[] = "Hello World!"; // テスト用文字列
char command = 's'; // 初期コマンド
int i = 0; 
int motorPWM = 230; // モータのPWM値 
 
// シリアル割り込み関数
void pc_rx(){
    command = pc.getc();
}
 
int main() {
    float servo_angle = 3; // サーボの初期角度
    float force_out = 0; 
    servo.position(servo_angle);
    pc.baud(9600); // ラズパイとのボーレート
    wait_ms(3000); // 電源投入次の誤動作防止
    
    while(1) {
        pc.attach(pc_rx, Serial::RxIrq); // 割り込み
        force_out = ain.read();          // 力センサ読み込み
        wait_ms(100);

        switch(command){
            case 'f':  // 前
                motorShield.changeSpeed(1,motorPWM,0,motorPWM);
                break;
            case 'b':  // 後
                motorShield.changeSpeed(2,motorPWM,0,motorPWM);
                break;
            case 'u':  
                motorShield.changeSpeed(0,motorPWM,1,motorPWM);
                break;
            case 'd':
                motorShield.changeSpeed(0,motorPWM,2,motorPWM);
                break;
            case 'w':  // スイッチ オン
                servo_angle = 3; 
                servo.position(servo_angle);
                break;
            case 'q':  // スイッチ オン
                servo_angle = -14; 
                servo.position(servo_angle);
                break;
            case 's':  // 停止
                motorShield.changeSpeed(0,motorPWM,0,motorPWM);
                break;
            default:
                //pc.printf("else");
                break;
        }
        if(limit1.read()){  // リミットスイッチ検出
            motorShield.changeSpeed(1,motorPWM,0,motorPWM);
            wait_ms(1000);
            command = 's';
        //pc.printf("state limit1 : %d \n", limit1.read());            
        }
        else{}
        //pc.printf("  force : %f  ", force_out);
        // 力センサ読み取り値に応じた動作
        if( 0.18 < force_out && force_out < 0.24){
          motorPWM = 80;
        }
        if(0.24 < force_out ){
            motorShield.changeSpeed(0,motorPWM,0,motorPWM);
            wait_ms(1000);
            motorShield.changeSpeed(2,200,0,0);
            wait_ms(1000);
            motorShield.changeSpeed(0,motorPWM,0,motorPWM);
            motorPWM = 230;
            command = 's';
            //pc.printf("state limit1 : %d \n", limit1.read());            
        }
        //pc.printf("AnalogIn : %f", ain.read());
        wait_ms(50);
    }
}
