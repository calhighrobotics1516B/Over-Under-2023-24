#include "main.h"
#include <ostream>

#include "api.h"
#include "pros/apix.h"
#include "pros/serial.h"

using namespace Robot;

/*
  (      (     (      (     (   
  )\    (()    )\     )\   (_)  
 ((_)  ((_)   ((_)   ((_) (_))  
 (/ |  | __|  (/ |  (/ /  | |() 
  | |  |__ \   | |  / _ \ |  _ \
  |_|  |___/   |_|  \___/ |____/

*/


struct RobotSubsystems {
	Robot::Autonomous autonomous;
	Robot::Drivetrain drivetrain;
	Robot::Wings wings;
	Robot::Puncher puncher;
	Robot::Intake intake;
} subsystem;

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */



void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(1, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
}

void toggles() {
	Robot::Utility::toggleSubsystemOptions(subsystem.autonomous, subsystem.drivetrain, subsystem.puncher);
}
/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */


void initialize() {


	pros::lcd::initialize();
	pros::Task toggler(toggles);
	
	if (pros::c::registry_get_plugged_type(13) == pros::c::E_DEVICE_IMU) {
		chassis.calibrate();
		chassis.setPose(0, 0, 90); // X: 0, Y: 0, Heading: 0
		// Setting an example start location for the robot so it is all relatativistic 
		pros::Task screenTask(Robot::Utility::displayLocation);
	}
	else {
		pros::Task screenTask(Robot::Utility::displayMotorVel);
	}
	
}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */



void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.<asd></asd>
 */
void competition_initialize() {
	// Allows the user to select the autonomous routine, drive control type as well as whether the distance puncher is enabled.
	// The user can select the auton routine by pressing the right buttons on the controller.
	// The user can select the drive control type by pressing the down button on the controller.
	// The user can select the distance puncher by pressing the left button on the controller.
	// The state of each subsystem is displayed on the controller screen.
	
	
}






/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 */
void autonomous() {
	
	subsystem.autonomous.AutoDrive(subsystem.puncher, true);



}



/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {


	subsystem.puncher.setDistancePuncher(false);
    while (true) {
        subsystem.drivetrain.run();
		


		subsystem.puncher.run(0);


		

		// Catapult controller, uses the X button holded down to push the elevation up.
		subsystem.intake.run();

		// Intake controller, moves the left and right intakes and stops them if nothing is pressed.
	

		subsystem.wings.run();

	


		
    


        pros::delay(2); // Small delay to reduce CPU usage
    }
	
	
}
