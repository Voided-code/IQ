#include "vex.h"

using namespace vex;

vex::brain Brain;
vex::controller Controller = controller();
vex::motor rightDrive = motor(vex::PORT11, false);
vex::motor leftDrive = motor(vex::PORT8, true);
vex::motor rightPArm = motor(vex::PORT7, false);
vex::motor leftPArm = motor(vex::PORT12, true);
vex::pneumatic pneumatic1 = pneumatic(vex::PORT9);
vex::pneumatic pneumatic2 = pneumatic(vex::PORT10);

vex::motor_group armGroup = motor_group(rightPArm, leftPArm);

void drive(float forwdS, float turnS) {
    rightDrive.setVelocity((Controller.AxisA.position()/100 * forwdS) - (Controller.AxisC.position()/100 * turnS), percent);
    leftDrive.setVelocity((Controller.AxisA.position()/100 * forwdS) + (Controller.AxisC.position()/100 * turnS), percent);
}

int drivecode() {
    rightDrive.spin(vex::forward);
    leftDrive.spin(vex::forward);
    while (true) {
        drive(70,70);
        wait(20,msec);
    }
    return 0;
}

int mainCode(){
    pneumatic2.pump(true);
    pneumatic1.retract(cylinder1);
    pneumatic1.retract(cylinder2);
    armGroup.setVelocity(40, percent);
    armGroup.spin(vex::forward);
    wait(0.3, seconds);
    while(armGroup.velocity(percent) > 5){
        wait(20, msec);
    }
    while (Controller.ButtonFUp.pressing()){
        wait(20,msec);
    }
    armGroup.setVelocity(100, percent);
    armGroup.setMaxTorque(100, percent);
    
    while (true){
        wait(20,msec);
        if(Controller.ButtonEUp.pressing()) {
            pneumatic1.extend(cylinder1);
            pneumatic1.extend(cylinder2);
            wait(0.2, seconds);
            armGroup.spinTo(150, degrees);
            while (Controller.ButtonEUp.pressing()){
                wait(20, msec);
            }
            while (!Controller.ButtonEUp.pressing() && !Controller.ButtonFUp.pressing() && !Controller.ButtonFDown.pressing()){
                wait(20, msec);
            }
            if(Controller.ButtonEUp.pressing()){
                armGroup.setVelocity(60, percent);
                armGroup.spinTo(0, degrees);
                pneumatic1.retract(cylinder1);
                pneumatic1.retract(cylinder2);
                armGroup.setVelocity(100, percent);
                while (Controller.ButtonEUp.pressing()){
                    wait(20, msec);
                }
            }
        } else if(Controller.ButtonFUp.pressing()){
            armGroup.spinTo(0, degrees);
            pneumatic1.extend(cylinder1);
            pneumatic1.extend(cylinder2);

            armGroup.setVelocity(78, percent);
            armGroup.spinTo(480, degrees);
            pneumatic1.retract(cylinder1);
            pneumatic1.retract(cylinder2);
            armGroup.setVelocity(100, percent);
            wait(0.2, seconds);

            // armGroup.spinTo(340, degrees);
            // pneumatic1.extend(cylinder1);
            // pneumatic1.extend(cylinder2);
            // wait(0.2, seconds);

            armGroup.spinTo(0, degrees);
            pneumatic1.retract(cylinder1);
            pneumatic1.retract(cylinder2);
        } else if(Controller.ButtonFDown.pressing()){
            armGroup.spinTo(210, degrees);
            pneumatic1.retract(cylinder2);
            pneumatic2.extend(cylinder1);
            wait(1,seconds);

            armGroup.spinTo(0, degrees);
            pneumatic2.retract(cylinder1);
            pneumatic1.retract(cylinder1);
            while (Controller.ButtonFDown.pressing()){
                wait(20, msec);
            }
        } else if(Controller.ButtonEDown.pressing()){
            armGroup.spinTo(30, degrees);
            pneumatic1.retract(cylinder2);
            while (Controller.ButtonEUp.pressing()){
                wait(20, msec);
            }
            while (!Controller.ButtonEUp.pressing()){
                wait(20, msec);
            }
            pneumatic1.retract(cylinder1);
            pneumatic1.retract(cylinder2);
            wait(1, seconds);
            armGroup.spinTo(0, degrees);
            pneumatic1.extend(cylinder2);
            while (Controller.ButtonEUp.pressing()){
                wait(20, msec);
            }
        }
    }
    return 0;
}

int main() {
    vex::task ws1(drivecode);
    // vex::task ws2(whenStarted3);
    mainCode();
}