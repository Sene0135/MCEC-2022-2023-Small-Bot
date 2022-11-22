#include "main.h"
#include <string>
#define startingv 50
// test2
// function headers
void moveToPos1();
void lookForBig();
void grabDisk();
void lookForDisks();

pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Motor topleft(3, false);
pros::Motor topright(7, false);
pros::Motor bottleft(2, true);
pros::Motor bottright(1, true);
pros::Motor roller(13, false);
pros::Vision vision_sensor(16);
pros::Distance dist_sensor(8);

bool rollerbool = false;
bool distancetrue = false; // for checking if distance sensor glitch yaaaaa
bool lookFoDisk = false;
int distance = 700;
int diskCheckVelocity = 20;
int vision = 0;
int ymotion;
int xmotion;

void initialize() {

  pros::lcd::initialize();
  pros::lcd::set_text(1, "Habibi");
  pros::lcd::set_text(2, "Analog Sticks to Drive");
  pros::lcd::set_text(3, "L1 - Slow down on turns");
  pros::lcd::set_text(4, "R1 - Rollers");
  pros::delay(1);
}

void moveToPos1() {
  topleft.move_absolute(7, startingv);
  bottleft.move_absolute(7, startingv);
  bottright.move_absolute(7, startingv);
  topright.move_absolute(7, startingv);
  pros::delay(200);
  topleft.move_velocity(0);
  bottleft.move_velocity(0);
  bottright.move_velocity(0);
  topright.move_velocity(0);

  lookFoDisk = true;
}
void lookForBig() {
  // use the vision sensor to look for red.
}
void grabDisk() {
  // first, the bot needs to find the center of the disk, and then do the
  // grabbin and stuff, foreplay ya feel?

  // use this while loop to find the center of the disk
  while (true) {
    distance = dist_sensor.get();
    topleft.move_velocity(-diskCheckVelocity);
    bottright.move_velocity(diskCheckVelocity);
    if (distance >
        200) { // since it stops 15 cm before the desired disk, 5cm room for
               // error(considering disk curvature) should be enough
      pros::delay(50);
      topleft.move_velocity(0);
      bottright.move_velocity(0);
      pros::delay(30);
      topleft.move_velocity(diskCheckVelocity);
      bottright.move_velocity(-diskCheckVelocity);
    } 
    else if (distance <= 200) {
      
    }
  }
}

void lookForDisks() {

  while (lookFoDisk == true) {
    distance = dist_sensor.get();

    // check the ports, I think some of the motors arent moving in the right way
    // spins the bot to check for disks
    topleft.move_velocity(-diskCheckVelocity);
    bottright.move_velocity(diskCheckVelocity);
    //MAKE TIMER GOOOOOOOFY AHH

    // This command tells the sensor to return whether it has detected the disc
    // or not (or something yellow) returns 255 if not detecting, 1 if it
    // detects
    pros::vision_object_s_t color = vision_sensor.get_by_sig(0, 1);

    pros::lcd::set_text(6, "Detected?: " + std::to_string(color.signature));
    vision = color.signature;
    pros::delay(20);

    // for the millisecond error
    if (distance < 500) {
      pros::delay(1000);
      if (distance < 500) {
        distancetrue = true;
      } else {
        distancetrue = false;
      }
    }

    // if the distance is under 500 for over a second and the vision sensor has
    // detected something yellow, break loop
    if (distancetrue == true &&
        vision == 1) { // gonna want to add size requirements for accuracy
      pros::lcd::set_text(2, "detected ");
      pros::delay(2);
      break;
    }
  }

  // stop motors
  pros::lcd::set_text(3, "Stopped ");
  topleft.move_velocity(
      0); // tells which motor to move at what voltage/direction
  bottleft.move_velocity(0);
  bottright.move_velocity(0);
  topright.move_velocity(0);
  pros::delay(2);
}
void autonomous() {
  // want the bot to go to a preset location, look for the yellow disks, after
  // picking them up, turns around and looks at the general area where the big
  // bot will be, it will look for the color red. the challenge is to time both
  // bots so they do not collide with eachother. a solution for this is to have
  // a similar system with the big bot, attach a vision sensor, and have it pick
  // up disks a couple sencds after it sees them, not the best solution....but
  // its all I got so fuckin deal w it bitch

  // move to preset location:
  moveToPos1(); // stop 15 cm before the expected disk location
  lookForDisks();
  grabDisk();
  lookForBig();
}

void opcontrol() {

  // for op control I was thinking ab making it more user friendly,
  // idk how to do it but I have ideas, lez finish auton forst bb
  while (true) {

    pros::Controller master(pros::E_CONTROLLER_MASTER);

    // default movement control
    ymotion = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
    xmotion = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

    int right = -xmotion + ymotion; //-power + turn
    int left = xmotion + ymotion;   // power + turn

    topleft.move(left); // tells which motor to move at what voltage/direction
    bottleft.move(-left);
    bottright.move(right);
    topright.move(-right);
    pros::delay(1);

    ////

    // Runs rollers motors while holding R1

    if (master.get_digital(DIGITAL_R1)) { // rollers

      roller.move_velocity(120);

    } else {

      roller.move_velocity(0);
    }

    // Slowdown feature (Cuts robots speed in half while holding down L1 on
    // controller)
    if (master.get_digital(DIGITAL_L1)) {

      int right = (-xmotion + ymotion) / 2; //-power + turn
      int left = (xmotion + ymotion) / 2;   // power + turn

      topleft.move(left);
      bottleft.move(-left);
      bottright.move(right);
      topright.move(-right);
    }
  }
}
ROLLER_MASTER);
	pros::Motor topleft(3, false);
	pros::Motor topright(7, false);	
	pros::Motor bottleft(2, true);
	pros::Motor bottright(1, true);
	pros::Motor roller(13, false);
	pros::Vision vision_sensor(16);
	pros::Distance dist_sensor (8);


	bool rollerbool = false;
	int distance = 700;
	bool distancetrue = false;
	int vision = 255;
	int ymotion;
	int xmotion;

void grabDisk() { 
	distance = dist_sensor.get();
	//logic could be improved, basically it just has the bot drive towards the disc at a diff speed until its in the scoop
	while (distance > 25) { //drive into the disc until the disc is under 25mm from the dist sensor
	distance = dist_sensor.get();
	topleft.move_velocity(approachingv); //tells which motor to move at what voltage/direction
	bottleft.move_velocity(approachingv);
	bottright.move_velocity(approachingv);
	topright.move_velocity(approachingv);
	pros::delay(2);
	if (distance < 25) {
		break;
	}
	}
	
	//stops bot for a sec
	topleft.move_velocity(0); //tells which motor to move at what voltage/direction
	bottleft.move_velocity(0);
	bottright.move_velocity(0);
	topright.move_velocity(0);

	//makes bot turn with the disc in the scoop
	pros::delay(1000);
	//topleft.move_velocity(approachingv); //replace w/ encoder movement
	bottleft.move_velocity(approachingv);
	//bottright.move_velocity(approachingv);
	topright.move_velocity(-approachingv);
	pros::delay(5000);

	//stops here
	topleft.move_velocity(0); //replace w/ encoder movement
	bottleft.move_velocity(0);
	bottright.move_velocity(0);
	topright.move_velocity(0);
	pros::lcd::set_text(4, "JOB DONE! ");


	}

void initialize(){

	pros::lcd::initialize();
	pros::lcd::set_text(1, "Habibi");
	//pros::lcd::set_text(2, "Analog Sticks to Drive");
	//pros::lcd::set_text(3, "L1 - Slow down on turns");
	//pros::lcd::set_text(4, "R1 - Rollers");
	pros::delay(1);

}

void autonomous() {

	/*
	topleft.move_velocity(startingv); //tells which motor to move at what voltage/direction
	bottleft.move_velocity(startingv);
	bottright.move_velocity(startingv);
	topright.move_velocity(startingv);
	pros::delay(2000);
	*/
	

	//Checking for disk
  	while (true)   {
	distance = dist_sensor.get();
	
	//check the ports, I think some of the motors arent moving in the right way
	topleft.move_velocity(startingv); //tells which motor to move at what voltage/direction
	bottleft.move_velocity(startingv);
	bottright.move_velocity(startingv);
	topright.move_velocity(-startingv);//test
	

	//This command tells the sensor to return whether it has detected the disc or not (or something yellow)
	//returns 255 if not detecting, 1 if it detects
    pros::vision_object_s_t color = vision_sensor.get_by_sig(0, 1);

	pros::lcd::set_text(6, "Detected?: " + std::to_string(color.signature));
	vision = color.signature;
	pros::delay(2);

	//the distance sensor keeps thinking somethings in front of it for a millisecond (you can see this in devices)
	//I wrote some code to ideally check if the distance is under 500 for over a second, and if not then ignore the reading
	//not sure if this logic works entirely, worth testing more
	if (distance < 500) {

		pros::delay(1000);
		if (distance < 500) {
			distancetrue = true;
		} else { distancetrue = false;}
		

	}

    //if the distance is under 500 for over a second and the vision sensor has detected something yellow, break loop
	if (distancetrue == true && vision == 1)	{ //gonna want to add size requirements for accuracy
		 pros::lcd::set_text(2, "detected ");
		 pros::delay(2);	
		  break;
		 }

	}

	//stop motors
	pros::lcd::set_text(3, "Stopped ");
	topleft.move_velocity(0); //tells which motor to move at what voltage/direction
	bottleft.move_velocity(0);
	bottright.move_velocity(0);
	topright.move_velocity(0);
	pros::delay(2);
	grabDisk();	//run function to go grab the disc


}












void opcontrol() {
	

	while (true) {

	pros::Controller master(pros::E_CONTROLLER_MASTER);

	// default movement control
	ymotion = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);
	xmotion = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);

	int right = -xmotion + ymotion; //-power + turn
	int left = xmotion + ymotion; //power + turn

	topleft.move(left); //tells which motor to move at what voltage/direction
	bottleft.move(-left);
	bottright.move(right);
	topright.move(-right);
	pros::delay(1);

	////

		//Runs rollers motors while holding R1 
		if(master.get_digital(DIGITAL_R1)){ //rollers

			roller.move_velocity(120);

		} else {

			roller.move_velocity(0);
		}
	
		// Slowdown feature (Cuts robots speed in half while holding down L1 on controller)
		if(master.get_digital(DIGITAL_L1)) { 

			int right = (-xmotion + ymotion)/2; //-power + turn
			int left = (xmotion + ymotion)/2; //power + turn

			topleft.move(left);
			bottleft.move(-left);
			bottright.move(right);
			topright.move(-right);

		}

	}

}


