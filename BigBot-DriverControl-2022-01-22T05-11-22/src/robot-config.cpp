#include "vex.h"

using namespace vex;


// A global instance of brain used for printing to the V5 brain screen
brain Brain;

controller cont = controller();

competition comp = competition();

// triport gyroPort = triport();
// gyro gyroSensor = gyro();
motor mot_dtRightFront = motor(PORT2, ratio18_1, false);  // Front Right Drive Motor (right)
motor mot_dtRightBack = motor(PORT3, ratio18_1, false);   // Back Right Drive Motor (right)
motor mot_dtLeftFront = motor(PORT4, ratio18_1, true);   // Front Left Drive Motor (left)
motor mot_dtLeftBack = motor(PORT5, ratio18_1, true);    // Back Left Drive Motor (left)

motor mot_frontClaw = motor(PORT6, ratio36_1, true);
motor mot_backClaw = motor(PORT7, ratio36_1, true);


/*motor mot_scoopMotor = motor(PORT6, ratio18_1, false);    // Ring Scoop Motor (left)
motor mot_intakeMotor = motor(PORT7, ratio18_1, false);   // Ring Intake Motor (left)
motor mot_sweeperMotor = motor(PORT8, ratio18_1, false);  // Sweeper Motor (left)
*/
/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */

 
void vexcodeInit(void) {
  // Nothing to initialize
}