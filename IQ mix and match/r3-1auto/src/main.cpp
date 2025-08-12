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
vex::inertial imu = inertial();

vex::motor_group armGroup = motor_group(rightPArm, leftPArm);


int main() {
    rightPArm.setVelocity(100, percent);
    leftPArm.setMaxTorque(100, percent);
    rightDrive.setStopping(hold);
    leftDrive.setStopping(hold);

    pneumatic2.pumpOff();
    pneumatic1.retract(cylinder2);
    imu.calibrate();
    while(imu.isCalibrating())
        wait(20, msec);
    
    pneumatic2.pumpOn();
    while(!Brain.buttonCheck.pressing())
        wait(20, msec);
    pneumatic2.pumpOff();
    while(Brain.buttonCheck.pressing())
        wait(20, msec);
    while(!Brain.buttonCheck.pressing())
        wait(20, msec);

    pneumatic1.extend(cylinder2);
    armGroup.spinTo(135, degrees, true);

    float Heading180 = imu.heading(degrees);
    if(Heading180 > 180)
        Heading180 -= 360;
    leftDrive.spin(forward);
    rightDrive.spin(forward);
    while((leftDrive.position(turns) + leftDrive.position(turns))/2 * (200 / 25.4) < 8){
        leftDrive.setVelocity(50 - (Heading180 * 1), percent);
        rightDrive.setVelocity(50 - (-Heading180 * 1), percent);
        wait(20, msec);
    }

    leftDrive.stop();
    rightDrive.stop();

    wait(0.5, seconds);
    
    leftDrive.spin(forward);
    rightDrive.spin(forward);
    Heading180 = imu.heading(degrees);
    if(Heading180 > 180)
        Heading180 -= 360;
    
    while(Heading180 < 50){
        leftDrive.setVelocity(40 - (Heading180 / 50 * 40), percent);
        rightDrive.setVelocity(-(40 - (Heading180 / 50 * 40)), percent);
        Heading180 = imu.heading(degrees);
        if(Heading180 > 180)
            Heading180 -= 360;
        wait(20, msec);
    }
    leftDrive.stop();
    rightDrive.stop();
    wait(0.5, seconds);
    leftDrive.setPosition(0, degrees);
    rightDrive.setPosition(0, degrees);

    leftDrive.spin(forward);
    rightDrive.spin(forward);
    while((leftDrive.position(turns) + leftDrive.position(turns))/2 * (200 / 25.4) < 6){
        leftDrive.setVelocity(50 - ((imu.heading(degrees) - 50) * 1), percent);
        rightDrive.setVelocity(50 - ((-imu.heading(degrees) + 50) * 1), percent);
        wait(20, msec);
    }

    leftDrive.stop();
    rightDrive.stop();

    armGroup.spinTo(0, degrees, true);
    pneumatic1.retract(cylinder2);
    wait(0.5, seconds);
    pneumatic1.extend(cylinder2);
    armGroup.spinTo(210, degrees, true);

    return 0;
}