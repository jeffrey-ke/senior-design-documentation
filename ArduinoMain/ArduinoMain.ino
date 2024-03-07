/*
#define IS_TEST 0
#define IS_ACTUAL 0
#define FLIP_TEST 1

#if IS_ACTUAL
#include "MasterComputeBridge.h"
#include "Constants.h"
#include "WaypointController.h"
#include "StateMachine.h"


StateMachine sm(1, 0, 0, 0,
                1, 0, 0, 0,
                5);
void setup() {
  // put your setup code here, to run once:
  PISerial.begin(115200);
  
}

void loop() {
  // put your main code here, to run repeatedly:
    sm.DecideState();
    sm.ExecuteState();
    auto pwm = sm.GetCommandedPWM();
    PISerial.print("FL: ");
    PISerial.println(pwm.FL);
    PISerial.print("FR: ");
    PISerial.println(pwm.FR);
    PISerial.print("DL: ");
    PISerial.println(pwm.DL);
    PISerial.print("DR: ");
    PISerial.println(pwm.DR);
}
#elif IS_TEST
#include "_tests.h"
#include "_StateMachineTests.h"
void setup() {
// put your setup code here, to run once:
    Serial.begin(115200);
}

void loop() {
// put your main code here, to run repeatedly:
    Test::run();
}
#elif FLIP_TEST
*/
#include "OrientationController.h"
#include "IMUDriver.h"
#include "ThrusterDriver.h"
#include "BarometerDriver.h"

OrientationController o_con_(10, 0, 0);
IMUDriver imu_;
BarometerDriver baro_;

using milliseconds = double;

void FlipTest(milliseconds duration, double Kp, double Ki, double Kd);
void DiveTest(double duration, double Kp, double Ki, double Kd);
void PressureTest(double duration);

void setup() {
    Serial.begin(115200);
    imu_.Init();
    baro_.Init();

    auto timer = millis();
    o_con_.SetDesiredToVertical();
    while (millis() - timer < 60000) {
        
    }
    o_con_.SetDesiredToHorizontal();
}

void FlipTest(milliseconds duration, double Kp, double Ki, double Kd) {
    o_con_.SetDesiredToVertical();
    auto timer = millis();
    while (millis() - timer < duration) {
        auto pwm = o_con_.CalculateControlEffort(imu_.GetData());
        Serial.print("Current orientation: ");
        Serial.println(imu_.GetData().z);
        Serial.print("Desired orientation: ");
        Serial.println(o_con_.GetDesiredOrientation());
        Serial.print("FL: ");
        Serial.println(pwm.FL);
        Serial.print("FR: ");
        Serial.println(pwm.FR);
        Serial.print("DL: ");
        Serial.println(pwm.DL);
        Serial.print("DR: ");
        Serial.println(pwm.DR);
        delay(1000);

    }

}

void loop() {

}

//#endif

