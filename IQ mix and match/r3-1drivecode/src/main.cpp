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
vex::drivetrain drive = drivetrain(leftDrive, leftDrive, 200, 200, 152, mm, 2.0);

int rc_auto_loop_function_Controller() {
    bool DrivetrainLNeedsToBeStopped_Controller = true;
    bool DrivetrainRNeedsToBeStopped_Controller = true;
    // process the controller input every 20 milliseconds
    // update the motors based on the input values
    while(true) {
        // calculate the drivetrain motor velocities from the controller joystick axies
        // left = AxisA + AxisC
        // right = AxisA - AxisC
        int drivetrainLeftSideSpeed = Controller.AxisA.position() + Controller.AxisC.position();
        int drivetrainRightSideSpeed = Controller.AxisA.position() - Controller.AxisC.position();
        
        // check if the value is inside of the deadband range
        if (drivetrainLeftSideSpeed < 5 && drivetrainLeftSideSpeed > -5) {
            // check if the left motor has already been stopped
            if (DrivetrainLNeedsToBeStopped_Controller) {
                // stop the left drive motor
                leftDrive.stop();
                // tell the code that the left motor has been stopped
                DrivetrainLNeedsToBeStopped_Controller = false;
            }
        } else {
            // reset the toggle so that the deadband code knows to stop the left motor nexttime the input is in the deadband range
            DrivetrainLNeedsToBeStopped_Controller = true;
        }
        // check if the value is inside of the deadband range
        if (drivetrainRightSideSpeed < 5 && drivetrainRightSideSpeed > -5) {
            // check if the right motor has already been stopped
            if (DrivetrainRNeedsToBeStopped_Controller) {
                // stop the right drive motor
                rightDrive.stop();
                // tell the code that the right motor has been stopped
                DrivetrainRNeedsToBeStopped_Controller = false;
            }
        } else {
            // reset the toggle so that the deadband code knows to stop the right motor next time the input is in the deadband range
            DrivetrainRNeedsToBeStopped_Controller = true;
        }
        
        // only tell the left drive motor to spin if the values are not in the deadband range
        if (DrivetrainLNeedsToBeStopped_Controller) {
            leftDrive.setVelocity(drivetrainLeftSideSpeed, percent);
            leftDrive.spin(forward);
        }
        // only tell the right drive motor to spin if the values are not in the deadband range
        if (DrivetrainRNeedsToBeStopped_Controller) {
            rightDrive.setVelocity(drivetrainRightSideSpeed, percent);
            rightDrive.spin(forward);
        }
        // wait before repeating the process
        wait(20, msec);
    }
    return 0;
}

// void drive(float forwdS, float turnS) {
//     rightDrive.setVelocity((Controller.AxisA.position()/100 * forwdS) - (Controller.AxisC.position()/100 * turnS), percent);
//     leftDrive.setVelocity((Controller.AxisA.position()/100 * forwdS) + (Controller.AxisC.position()/100 * turnS), percent);
//     Brain.Screen.setCursor(1,1);
//     Brain.Screen.print("c: %f", Controller.AxisA.position());
//     Brain.Screen.setCursor(2,1);
//     Brain.Screen.print("oa: %f", Controller.AxisA.position()/100 * forwdS);
//     Brain.Screen.setCursor(3,1);
//     Brain.Screen.print("oa: %f", Controller.AxisA.position()/100 * forwdS);
//     wait(20,msec);
//     Brain.Screen.clearScreen();
// }

// int drivecode() {
//     rightDrive.spin(vex::forward);
//     leftDrive.spin(vex::forward);
//     while (true) {
//         drive(100,70);
//         vex::wait(20,msec);
//     }
//     return 0;
// }

int mainCode(){
    pneumatic2.pump(true);
    pneumatic1.retract(cylinder1);
    pneumatic1.retract(cylinder2);
    pneumatic2.retract(cylinder1);
    pneumatic2.retract(cylinder2);
    while (Controller.ButtonFUp.pressing()){
        vex::wait(20,msec);
    }
    armGroup.setVelocity(100, percent);
    armGroup.setMaxTorque(100, percent);
    
    bool Amiddle = false;
    bool Aclose = false;
    bool Acorner = false;
    while (true){
        if(Controller.ButtonLUp.pressing()){
            if(Amiddle == false){
                Amiddle = true;
                pneumatic1.extend(cylinder1);
                pneumatic1.extend(cylinder2);
                vex::wait(0.2, seconds);
                armGroup.spinTo(150, degrees);
                Aclose = true;
            } else if(Amiddle == true && !Acorner == true){
                Amiddle = false;
                armGroup.setVelocity(60, percent);
                armGroup.spinTo(480, degrees);
                pneumatic1.retract(cylinder1);
                pneumatic1.retract(cylinder2);
                armGroup.setVelocity(100, percent);
                vex::wait(0.2, seconds);

                armGroup.spinTo(0, degrees);
                pneumatic1.retract(cylinder1);
                pneumatic1.retract(cylinder2);
                Aclose = false;
            }
            while (Controller.ButtonLUp.pressing()){
                vex::wait(20, msec);
            }
        } 
        else if(Controller.ButtonLDown.pressing()){
            if(Amiddle == false){
                if(Aclose == false){
                    Aclose = true;
                    pneumatic1.extend(cylinder1);
                    pneumatic1.extend(cylinder2);
                } else{
                    Aclose = false;
                    pneumatic1.retract(cylinder1);
                    pneumatic1.retract(cylinder2);
                }
            } else if(Amiddle == true){
                Amiddle = false;
                Acorner = false;
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
        else if(Controller.ButtonFDown.pressing()){
            armGroup.spinTo(0, degrees, false);
            beamGroup.spinTo(0, degrees, false);
            while(armGroup.isSpinning() || beamGroup.isSpinning()){
                vex::wait(20, msec);
            }
            pneumatic1.retract(cylinder1);
            pneumatic1.retract(cylinder2);
            pneumatic2.retract(cylinder2);
            Acorner = false;
            Amiddle = false;
            Aclose = false;
            while (Controller.ButtonFUp.pressing()){
                vex::wait(20, msec);
            }
        }
        else if(Controller.ButtonFUp.pressing()){
            armGroup.spinTo(200, degrees);
            pneumatic1.retract(cylinder2);
            pneumatic2.extend(cylinder1);
            vex::wait(1,seconds);

            armGroup.spinTo(0, degrees);
            pneumatic2.retract(cylinder1);
            pneumatic1.retract(cylinder1);
            Aclose = false;
            Amiddle = false;
            while (Controller.ButtonFUp.pressing()){
                vex::wait(20, msec);
            }
        } 
        else if(Controller.ButtonEUp.pressing()){
            if(Acorner == false && Amiddle == false){
                Acorner = true;
                Amiddle = true;
                pneumatic1.extend(cylinder1);
                pneumatic1.extend(cylinder2);
                armGroup.spinTo(75, degrees);
            } else if(Acorner == true && Amiddle == true){
                Acorner = true;
                Amiddle = true;
                pneumatic1.retract(cylinder1);
                pneumatic1.retract(cylinder2);
            }
            while (Controller.ButtonEUp.pressing()){
                vex::wait(20, msec);
            }
        }
        vex::wait(20,msec);
    }
    return 0;
}

int beamArm() {
    bool pnumatic = false;
    int Bpos = 1;
    beamGroup.setVelocity(100, percent);
    beamGroup.setMaxTorque(100, percent);
    beamGroup.setStopping(hold);
    while (true) {
        if(Controller.ButtonRUp.pressing()){
            if(Bpos == 1){
                Bpos = 2;
                beamGroup.spinTo(325, degrees);
            } else if(Bpos == 2){
                Bpos = 3;
                beamGroup.spinTo(600, degrees);
            }
            
            while(Controller.ButtonRUp.pressing()){
                vex::wait(20, msec);
            }
        }

        else if(Controller.ButtonRDown.pressing()){
            if(Bpos == 1){
                if(pnumatic == false)
                    pneumatic2.extend(cylinder2); 
                else 
                    pneumatic2.retract(cylinder2);
                pnumatic = !pnumatic;
            } else if(Bpos == 2){
                beamGroup.spinTo(250, degrees);
                pneumatic2.retract(cylinder2);
                wait(3, seconds);
                beamGroup.spinTo(0, degrees);
                pnumatic = false;
                Bpos = 1;
            } else if(Bpos == 3){
                beamGroup.spinTo(550, degrees);
                pneumatic2.retract(cylinder2);
                wait(3, seconds);
                beamGroup.spinTo(0, degrees);
                pnumatic = false;
                Bpos = 1;
            }

            if(Controller.ButtonEUp.pressing()){
                Bpos = 1;
                while(Controller.ButtonEUp.pressing()){
                    wait(20,msec);
                }
            }

            while(Controller.ButtonRDown.pressing()){
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
    vex::task ws1(rc_auto_loop_function_Controller);
    vex::task ws2(beamArm);
    mainCode();
}