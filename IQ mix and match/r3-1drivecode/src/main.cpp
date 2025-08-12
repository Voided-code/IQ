#include "vex.h"

using namespace vex;

vex::brain Brain;
vex::controller Controller = controller();
vex::motor rightDrive = motor(vex::PORT11, false);
vex::motor leftDrive = motor(vex::PORT8, true);
vex::motor rightPArm = motor(vex::PORT7, false);
vex::motor leftPArm = motor(vex::PORT12, true);
vex::motor rightBArm = motor(vex::PORT6, false);
vex::motor leftBArm = motor(vex::PORT1, true);
vex::pneumatic pneumatic1 = pneumatic(vex::PORT9);
vex::pneumatic pneumatic2 = pneumatic(vex::PORT10);

vex::motor_group armGroup = motor_group(rightPArm, leftPArm);
vex::motor_group beamGroup = motor_group(rightBArm, leftBArm);

void drive(float forwdS, float turnS) {
    rightDrive.setVelocity((Controller.AxisA.position()/100 * forwdS) - (Controller.AxisC.position()/100 * turnS), percent);
    leftDrive.setVelocity((Controller.AxisA.position()/100 * forwdS) + (Controller.AxisC.position()/100 * turnS), percent);
}

int drivecode() {
    rightDrive.spin(vex::forward);
    leftDrive.spin(vex::forward);
    while (true) {
        drive(70,70);
        vex::wait(20,msec);
    }
    return 0;
}

int mainCode(){
    pneumatic2.pump(true);
    pneumatic1.retract(cylinder1);
    pneumatic1.retract(cylinder2);
    while (Controller.ButtonFUp.pressing()){
        vex::wait(20,msec);
    }
    armGroup.setVelocity(100, percent);
    armGroup.setMaxTorque(100, percent);
    
    bool Amiddle = false;
    bool Aclose = false;
    while (true){
        if(Controller.ButtonLUp.pressing()){
            if(Amiddle == false){
                Amiddle = true;
                pneumatic1.extend(cylinder1);
                pneumatic1.extend(cylinder2);
                vex::wait(0.2, seconds);
                armGroup.spinTo(150, degrees);
                Aclose = true;
            } else if(Amiddle == true){
                Amiddle = false;
                armGroup.spinTo(0, degrees);
                pneumatic1.extend(cylinder1);
                pneumatic1.extend(cylinder2);

                armGroup.setVelocity(78, percent);
                armGroup.spinTo(480, degrees);
                pneumatic1.retract(cylinder1);
                pneumatic1.retract(cylinder2);
                armGroup.setVelocity(100, percent);
                vex::wait(0.2, seconds);

                // armGroup.spinTo(340, degrees);
                // pneumatic1.extend(cylinder1);
                // pneumatic1.extend(cylinder2);
                // wait(0.2, seconds);

                armGroup.spinTo(0, degrees);
                pneumatic1.retract(cylinder1);
                pneumatic1.retract(cylinder2);
                Aclose = false;
            }
            while (Controller.ButtonLUp.pressing()){
                vex::wait(20, msec);
            }
        } else if(Controller.ButtonLDown.pressing()){
            if(Amiddle == false){
                if(Aclose == false){
                    Aclose = true;
                    pneumatic1.extend(cylinder1);
                    pneumatic1.extend(cylinder2);
                } else{
                    Aclose = true;
                    pneumatic1.retract(cylinder1);
                    pneumatic1.retract(cylinder2);
                }
            } else if(Amiddle == true){
                armGroup.setVelocity(60, percent);
                armGroup.spinTo(0, degrees);
                pneumatic1.retract(cylinder1);
                pneumatic1.retract(cylinder2);
                armGroup.setVelocity(100, percent);
            }
            while (Controller.ButtonLDown.pressing()){
                vex::wait(20, msec);
            }
        }

        vex::wait(20,msec);
        // if(Controller.ButtonFDown.pressing()){
        //     armGroup.spinTo(210, degrees);
        //     pneumatic1.retract(cylinder2);
        //     pneumatic2.extend(cylinder1);
        //     vex::wait(1,seconds);

        //     armGroup.spinTo(0, degrees);
        //     pneumatic2.retract(cylinder1);
        //     pneumatic1.retract(cylinder1);
        //     while (Controller.ButtonFDown.pressing()){
        //         vex::wait(20, msec);
        //     }
        // } else if(Controller.ButtonEDown.pressing()){
        //     armGroup.spinTo(30, degrees);
        //     pneumatic1.retract(cylinder2);
        //     while (Controller.ButtonEUp.pressing()){
        //         vex::wait(20, msec);
        //     }
        //     while (!Controller.ButtonEUp.pressing()){
        //         vex::wait(20, msec);
        //     }
        //     pneumatic1.retract(cylinder1);
        //     pneumatic1.retract(cylinder2);
        //     vex::wait(1, seconds);
        //     armGroup.spinTo(0, degrees);
        //     pneumatic1.extend(cylinder2);
        //     while (Controller.ButtonEUp.pressing()){
        //         vex::wait(20, msec);
        //     }
        // }
    }
    return 0;
}

int beamArm() {
    bool pnumatic = false;
    bool Bmiddle = false;
    beamGroup.setVelocity(100, percent);
    beamGroup.setMaxTorque(100, percent);
    beamGroup.setStopping(hold);
    while (true) {
        if(Controller.ButtonRUp.pressing()){
            if(Bmiddle == false){
                Bmiddle = true;
                beamGroup.spinTo(300, degrees);
            } else if(Bmiddle == true){
                beamGroup.spinTo(600, degrees);
            }
            
            while(!Controller.ButtonRUp.pressing()){
                vex::wait(20, msec);
            }
        }

        // if(Controller.ButtonRDown.pressing()) { 
        //     beamGroup.spin(vex::reverse);
        //     if(beamGroup.position(degrees) <= 10){
        //         if(pnumatic)
        //             pneumatic2.retract(cylinder2);
        //         else
        //             pneumatic2.extend(cylinder2);
        //         beamGroup.stop();
        //         while(!Controller.ButtonRUp.pressing()){
        //             vex::wait(20, msec);
        //         }
        //     }
        // } else if(Controller.ButtonRUp.pressing()) {
        //     beamGroup.spin(vex::forward);
        // } else {
        //     beamGroup.stop();
        // }
        vex::wait(20, msec);
    }
    return 0;
}

int main() {
    vex::task ws1(drivecode);
    vex::task ws2(beamArm);
    mainCode();
}