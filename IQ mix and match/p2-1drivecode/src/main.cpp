#include "vex.h"

using namespace vex;
using namespace std;

vex::brain Brain;
vex::controller Controller;
motor leftLift = motor(PORT6, true);
motor rightLift = motor(PORT12, false);
pneumatic pnumatic = pneumatic(PORT7);
distance dist1 = distance(PORT1);
distance dist2 = distance(PORT2);
inertial imu = inertial();

int main() {
    while(true) {
        if(Controller.ButtonFUp.pressing()){
            leftLift.spin(vex::forward);
            rightLift.spin(vex::forward);
            wait(0.3, seconds);
            if(dist1 < 60 && dist1 > 15 && dist2 < 60 && dist2 > 15){
                pnumatic.extend(cylinder1);
                pnumatic.extend(cylinder2);
            } else if (dist1 < 60 && dist1 > 15){
                pnumatic.extend(cylinder1);
            } else if (dist2 < 60 && dist2 > 15){
                pnumatic.extend(cylinder2);
            } else {
                pnumatic.retract(cylinder1);
                pnumatic.retract(cylinder2);
            }
            while(dist !< 15){
                wait(20, msec);
            }
            pnumatic.retract(cylinder1);
            pnumatic.retract(cylinder2);
            rightLift.turnFor(vex::reverse, 1.7, turn);
            leftLift.turnFor(vex::reverse, 1.7, turn);
            while(Controller.ButtonFUp.released()){
                wait(20, msec);
            }
        }

        if(Controller.ButtonEUp.pressing()){
            leftLift.spin(vex::forward);
            rightLift.spin(vex::forward);
            while(dist !< 15){
                wait(20, msec);
            }
            pnumatic.extend(cylinder1);
            pnumatic.extend(cylinder2);
            rightLift.turnFor(vex::reverse, 1.7, turn);
            leftLift.turnFor(vex::reverse, 1.7, turn);
        }
    }
}