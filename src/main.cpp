#include "main.h"
#include <string>
#include <vector>
#define startingv 50
// test2
// function headers
void moveToPos1(); //Moves to big bot 
void lookForBig();
void grabDisk();
void lookForDisks();

#define maxfly 170 //Shoot from one low goal to the other
#define smallfly 135 //shoot high goal right outside net
#define midfly 145

//seperate belt and wheel intake
//lower speed of flywheel by mucho


pros::Controller master(pros::E_CONTROLLER_MASTER);
pros::Motor topleft(16, false);
pros::Motor spring(1, false);
pros::Motor topright(17, true);
pros::Motor bottleft(15, true);
pros::Motor bottright(18, false);
pros::Motor flywheel(13, true);
pros::Motor intake(20, false);
pros::Motor belt(10, true);
pros::Motor roller(2, false);
pros::Vision vision_sensor(18);
pros::Distance dist_sensor(16);


//pros::Motor_Group Left ({3, 1});
//pros::Motor_Group Right ({7,2});
//pros::Motor_Group Total ({1,2,-3,-7});

bool rollerbool = false;	
bool distancetrue = false; // for checking if distance sensor glitch yaaaaa
bool lookFoDisk = false;
int distance = 700;
int diskCheckVelocity = 20;
int vision = 0;
int ymotion;
int xmotion;
int flywheelV = smallfly;

void initialize() {
   spring.set_brake_mode(pros::E_MOTOR_BRAKE_HOLD);
  pros::lcd::initialize();
 // pros::lcd::set_text(1, "real");
 // pros::lcd::set_text(2, "Analog Sticks to Drive");
  //pros::lcd::set_text(3, "L1 - Slow down on turns");
 // pros::lcd::set_text(4, "R1 - Rollers");
  pros::delay(1);
}


void autonomous() {
  //Keep it simple
  //Move forward, turn to the left (where lower goal is)
  //Shoot preloads into lower goal
  //ez ez points yippee
  //go to adjacent roller
  topleft.tare_position();
  topright.tare_position();
  bottleft.tare_position();
  bottright.tare_position();
/*
  topleft.move_absolute(-454, 100); //Turn 45 deg (position values gained from opcontrol driving)
  topright.move_absolute(286, 100);
  bottleft.move_absolute(1704, 100);
  bottright.move_absolute(-1575, 100);
  while (!((bottleft.get_position() < 1710) && (bottleft.get_position() > 1700))) {
      pros::lcd::set_text(1, std::to_string(topleft.get_position()));
      pros::lcd::set_text(2, std::to_string(topright.get_position()));
      pros::lcd::set_text(3, std::to_string(bottleft.get_position()));
      pros::lcd::set_text(4, std::to_string(bottright.get_position()));
    // Continue running this loop as long as the motor is not within +-5 units of its goal
    pros::delay(2);
  }
  pros::lcd::set_text(5, "OVER!");
  topleft.tare_position();
  topright.tare_position();
  bottleft.tare_position();
  bottright.tare_position();
  pros::delay(1000);
  topleft.move_absolute(-2676, 100); //Drive straight towards disc
  topright.move_absolute(-1738, 100);
  bottleft.move_absolute(9657, 100);
  bottright.move_absolute(10456, 100);
  while (!((bottright.get_position() < 10460) && (bottright.get_position() > 10450))) {
    // Continue running this loop as long as the motor is not within +-5 units of its goal
    pros::delay(2);
    pros::lcd::set_text(1, std::to_string(topleft.get_position()));
      pros::lcd::set_text(2, std::to_string(topright.get_position()));
      pros::lcd::set_text(3, std::to_string(bottleft.get_position()));
      pros::lcd::set_text(4, std::to_string(bottright.get_position()));
  }

  topleft.move_absolute(215, 100); //Drive straight towards disc
  topright.move_absolute(250, 100);
  bottleft.move_absolute(-149, 100);
  bottright.move_absolute(-176, 100);
  pros::delay(1000);
*/

  flywheel.move_velocity(90);
  while(flywheel.get_actual_velocity() < 90) {
    pros::delay(2);
  }
  int timervar = 0;
  while(timervar != 5000) {
    flywheel.move_velocity(120);
    intake.move_velocity(200);
    belt.move_velocity(120);
    pros::delay(1);
    timervar++;
  }
   flywheel.move_velocity(0);
    intake.move_velocity(0);
    belt.move_velocity(0);
  pros::delay(1);

	topleft.move_absolute(1438, 20); //Drive straight towards disc
  topright.move_absolute(1561, 20);
  bottleft.move_absolute(-1135, 20);
  bottright.move_absolute(-1194, 20);
    pros::delay(1700);

topleft.move_absolute(43, 20); //Drive straight towards disc
  topright.move_absolute(-804, 20);
  bottleft.move_absolute(-71, 20);
  bottright.move_absolute(597, 20);
  pros::delay(1000);


}

  

void opcontrol() {

  // for op control I was thinking ab making it more user friendly,
  // idk how to do it but I have ideas, lez finish auton forst bb
  while (true) {

      pros::lcd::set_text(1, std::to_string(topleft.get_position()));
      pros::lcd::set_text(2, std::to_string(topright.get_position()));
      pros::lcd::set_text(3, std::to_string(bottleft.get_position()));
      pros::lcd::set_text(4, std::to_string(bottright.get_position()));

    pros::Controller master(pros::E_CONTROLLER_MASTER);
    //master.print(0, 0, "ur ass lol");
    // default movement control
    ymotion = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X);
    xmotion = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);

    int right = -xmotion + ymotion; //-power + turn
    int left = xmotion + ymotion;   // power + turn

    topleft.move(left); // tells which motor to move at what voltage/direction
    bottleft.move(-left);
    bottright.move(right);
    topright.move(-right);



    if(ymotion < -1 || ymotion > 1) { //slows down turning speed  by 1.75
      int right = (-xmotion + ymotion) / 1.75; //-power + turn
      int left = (xmotion + ymotion) / 1.75;   // power + turn

      topleft.move(left);
      bottleft.move(-left);
      bottright.move(right);
      topright.move(-right);
    }
   
    ////

    // Runs rollers motors while holding R1
      if(master.get_digital(DIGITAL_UP)) {
      pros::lcd::set_text(6, std::to_string(flywheelV));
  

		if (flywheelV == midfly) {
      pros::delay(100);
			flywheelV = maxfly;
			 master.print(0, 0, "far      ");
      
  
		}

		else if (flywheelV == smallfly) {
       pros::delay(100);
			flywheelV = midfly;
			 master.print(0, 0, "medium");
     
      
		}
     pros::delay(100);

		}

	if(master.get_digital(DIGITAL_DOWN)) {
    pros::lcd::set_text(6, std::to_string(flywheelV));
		
		if (flywheelV == maxfly) {
			pros::delay(100);
			flywheelV = midfly;
			 master.print(0, 0, "medium");
	
			

		} 

		else if (flywheelV == midfly) {
			pros::delay(100);
			flywheelV = smallfly;
			master.print(0, 0, "close   ");
			 
   
		}

	}

    if (master.get_digital(DIGITAL_R1)) { // intake

      intake.move_velocity(200);
      belt.move_velocity(120);

    } else {
      belt.move_velocity(0);
      intake.move_velocity(0);
    }
    
    if (master.get_digital(DIGITAL_L1)) { // intake seperate

      intake.move_velocity(200);

    } 

    if (master.get_digital(DIGITAL_Y)) { // belt seperate

      belt.move_velocity(200);

    } 

    if (master.get_digital(DIGITAL_L2)) { // belt/flywheel reverse in case of jam

      belt.move_velocity(-200);

    } 

 

    if (master.get_digital(DIGITAL_A)) { //releases string shooter

     spring.move_relative(-100,100);

    }



	

    // Slowdown feature (Cuts robots speed in half while holding down L1 on
    // controller)

    if(master.get_digital(DIGITAL_R2)) { //flywheel
		
		flywheel.move_velocity(flywheelV);
    pros::lcd::set_text(5, std::to_string(flywheel.get_actual_velocity()));
    pros::lcd::set_text(6, std::to_string(flywheelV));
    pros::delay(10);


		} else {
			flywheel.move_velocity(0);
		}


     if (master.get_digital(DIGITAL_B)) { // roller

      roller.move_velocity(400);


    } else {
      roller.move_velocity(0);
    }
    

  pros::delay(1);

  }
}
