#include "vex.h"

using namespace vex; // avoiding the need to use vex:: before almost everything

vex::brain Brain; // setup of all the devices
vex::controller Controller = controller();
vex::motor rightDrive = motor(PORT6, false);
vex::motor leftDrive = motor(PORT12, true);
vex::motor rightPArm = motor(PORT10, false);
vex::motor leftPArm = motor(PORT4, true);
vex::motor rightBArm = motor(PORT5, false);
vex::motor leftBArm = motor(PORT11, true);
vex::pmumatic pneumatic1 = pneumatic(PORT9);
vex::pneumatic pnumatic2 = pneumatic(PORT3);

vex::motor_group armGroup = motor_group(rightPArm, leftPArm);

int drivecode() {
    rightDrive.spin(vex::forward); // this will spin the right drive motors forward
    leftDrive.spin(vex::forward);
    while (true) {
        drive(100,100); // this will call set the proper speed for both drive motors
    }
    return 0;
}

void drive(float forwdS, float turnS) { // this is the drive code for the robot in arcade style
    rightDrive.velicity(Controller.AxisA.position()/100 * forwdS + Controller.AxisC.position()/100 * turnS);
    // The robot gets the controllers position for the left stick (forwads and bacwards) then divides it by 100 to get a percentage of the speed
    // then it multiplies it by the forwdS variable to tell it how fast to go in the forwards and backwards direction
    leftDrive.velicity(Controller.AxisA.position()/100 * forwdS - Controller.AxisC.position()/100 * turnS);
    // The robot then gets the controllers position for the right stick (left and right) then divides it by 100 aswell to get a percentage of the speed
    // then it multiplies it by the turnS variable to tell it how fast to go when turning left and right

    // After this we add the two together to get the final speed for each side of the robot
}

int mainCode(){
    bool backPnumatic = false; // This variable is used to keep track of the state of the back pneumatic
    pneumatic2.pump(true); // This turns on the pneumatic pump to keep the pneumatics pressurized
    armGroup.setVelocity(100, percent); // This sets the velocity of the arm group to 100% so it moves at full speed
    armGroup.setTorque(100, percent); // This sets the torque of the arm group to 100% so it can move the pins
    while (true){
        if(Controller.ButtonEUp.pressing()) { // This makes it so when you press E up the robot will grab the pins and lift the arm
            pneumatic1.retract(cylinder1); // grabbing the pins
            pneumatic1.retract(cylinder2); 
            wait(0.5, seconds); // waiting for the pneumatics to retract
            armGroup.spinTo(167, degrees); // lifting the arm
            while (Controller.ButtonEUp.pressing()) // this peice of code will wait till the button is released before continuing so the code doesnt do it again
                wait(20, msec);
        } else if(Controller.ButtonFUp.pressing()) { // This makes it so when you press F up the robot will lower the arm to drop pins ontop of the stack
            armGroup.spinTo(0, degrees); // lowering the arm
            pneumatic1.extend(cylinder1); // dropping the pins
            pneumatic1.extend(cylinder2);
            while (Controller.ButtonFUp.pressing()) // same peice of code as above (will be used a lot)
                wait(20, msec);
        } else if(Controller.ButtonFDown.pressing()){ // This makes it so when you press F down the robot will go all the way pack to put pins into the beam
            armGroup.spinTo(0, degrees); // this will lower the arm to realign the pins as the alignment needs to be spot on
            pneumatic1.retract(cylinder1); // making sure the pins are grabbed
            pneumatic1.retract(cylinder2);
            wait(0.3, seconds); // waiting for the pneumatics to retract
            armGroup.spinTo(510, degrees); // this will spin the arm to the back of the robot
            pneumatic1.extend(cylinder1); // this will extend the pneumatics to put the pins into the beam
            pneumatic1.extend(cylinder2);
            wait(0.3, seconds); // waiting for the pneumatics to extend
            armGroup.spinTo(330, degrees); // this will spin the arm half way to the front as the robot's arm cannot pass the two beams on the side with the arms fully extended
            pneumatic1.retract(cylinder1); // this will retract the pneumatics
            pneumatic1.retract(cylinder2);
            wait(0.4, seconds); // waiting for the pneumatics to retract
            armGroup.spinTo(0, degrees); // this will spin the arm back to the front of the robot
            pneumatic1.extend(cylinder1); // this will open the pneumatics ready to grab more pins
            pneumatic1.extend(cylinder2);
        } else if(Controller.ButtonEDown.pressing()){ // This makes it so when you press E down the robot will toggle the back pneumatic for grabbing the beam
            if(backPnumatic) // this will check if the back pneumatic is currently extended or retracted
                pneumatic1.extend(cylinder2); // if it is retracted then it will extend it
            else
                pneumatic1.retract(cylinder2); // otherwise it will retract it
            backPnumatic = !backPnumatic; // this will toggle the backPnumatic variable so it can be used next time
            while(Controller.ButtonEDown.pressing()) // this will wait until the button is released so it does not toggle again
                wait(20, msec);
        }
    }
    return 0;
}

int beamArm() {
    rightBArm.setVelocity(100, percent); // this sets the velocity of the beam arm motors to 100% so they move at full speed
    leftBArm.setVelocity(100, percent); // same as above but for the left
    rightBArm.setTorque(100, percent); // this sets the torque of the beam arm motors to 100% so they can move the beam
    leftBArm.setTorque(100, percent); // same as above but for the left
    while (true) {
        if(Controller.ButtonLUp.pressing()) { // these are the controls for the beam arm
            rightBArm.spin(vex::reverse); // this will spin the right beam arm motor in reverse when the L1 button is pressed
            leftBArm.spin(vex::reverse);
        } else if(Controller.ButtonLDown.pressing()) {
            rightBArm.spin(vex::forward); // this will spin the right beam arm motor in forward when the L2 button is pressed
            leftBArm.spin(vex::forward);
        } else {
            rightBArm.stop(); // otherwise it will stop the motors
            leftBArm.stop();
        }
    }
    return 0;
}

int main() {
    vex::task ws1(drivecode); // running all of the code at once
    vex::task ws2(beamArm);
    mainCode();
}