using namespace vex;

extern brain Brain;

extern controller cont;

extern competition comp;

// extern triport gyroPort;
// extern gyro gyroSensor;

extern motor mot_dtLeftFront;   // Front Left Drive Motor (left)
extern motor mot_dtLeftBack;    // Back Left Drive Motor (left)
extern motor mot_dtRightFront;  // Front Right Drive Motor (right)
extern motor mot_dtRightBack;   // Back Right Drive Motor (right)
extern motor mot_frontClaw;     //front claw for grabbing mobile goals
extern motor mot_backClaw;

/**
 * Used to initialize code/tasks/devices added using tools in VEXcode Pro.
 *
 * This should be called at the start of your int main function.
 */
void vexcodeInit(void);
