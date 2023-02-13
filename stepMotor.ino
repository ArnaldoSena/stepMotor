#include <AccelStepper.h>

#define motorPin1  5
#define motorPin2  4
#define motorPin3  0
#define motorPin4  2

int stepsPerRevolution = 64;        // steps per revolution
int degreePerRevolution = 5.625;    // degree per revolution

/*
 * AccelStepper::FULL2WIRE (2) means: 2 wire stepper (2 pins needed). 
 * AccelStepper::FULL3WIRE (3) means: 3 wire stepper, like a harddisk motor (3 pins needed). 
 * AccelStepper::FULL4WIRE (4) means: 4 wire stepper (4 pins needed). 
 * AccelStepper::HALF3WIRE (6) means: 3 wire half stepper, like a harddisk motor (3 pins needed) 
 * AccelStepper::HALF4WIRE (8) means: 4 wire half stepper (4 pins needed) 
 *
 * AccelStepper uses AccelStepper::FULL4WIRE (4 pins needed) by default.
 */
AccelStepper stepper(AccelStepper::HALF4WIRE, motorPin1, motorPin3, motorPin2, motorPin4);

void setup() {
  Serial.begin(9600);               // initialise the serial monitor

  stepper.setMaxSpeed(1000.0);      // set the max motor speed
  stepper.setAcceleration(100.0);   // set the acceleration
  stepper.setSpeed(200);            // set the current speed

}

// Degrees of the momevement. So first to 90 degrees, then to -90 graden then to 180 degrees etc.
//int steps[] = { 90, -90, 180, -180, 0 };
int steps[] = {60, 120, 180, 240, 300, 360};

// The total entries in steps[]
//int stepsCount = 5;
int stepsCount = 6;

// Keeps track of the position in steps[] we are about to run
int stepsIndex = 0;

void loop() {
   // If the stepper isn't moving and doesn't have to go any distance
  if (!stepper.isRunning() && stepper.distanceToGo() == 0) {

    stepper.setCurrentPosition(0);

    // Move the stepper to the degrees on the position of stepIndex in steps[]
    stepper.moveTo(degToSteps(steps[stepsIndex]));

    // Increase the index when each movement has finished
    stepsIndex++;

    // If we have executed all positions of steps[]
    if (stepsIndex > stepsCount) {

      // Set the index to 0 to restart the process
      stepper.stop();
      stepper.setCurrentPosition(0);
      stepsIndex = 0;
    }
  }

  stepper.run();

}

/*
 * Converts degrees to steps
 * 
 * 28BYJ-48 motor has 5.625 degrees per step
 * 360 degrees / 5.625 = 64 steps per revolution
 * 
 * Example with degToSteps(45):
 * (64 / 5.625) * 45 = 512 steps
 */
float degToSteps(float deg) {
  return (stepsPerRevolution / degreePerRevolution) * deg;
}
