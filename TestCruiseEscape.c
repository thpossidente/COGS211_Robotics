
// Initializing Relevant Variables // 
const int FRONT_BUMP = 1;
const int RIGHT_IR = 5;
const int LEFT_MOTOR = 0;
const int RIGHT_MOTOR = 3;
const float TOP_SPEED = 1023.0;
const int BLUE_CH = 0;
const int TRUE = 1;
const int FALSE = 0;

int num_blue;




void main() // Contains Behavioral Hierarchy (Dance, Escape, Approach, Cruise)
{
	int i;
	enable_servos();  // Must be called to execute motor commands
    camera_open(HIGH_RES);  // Called once to "turn camera on"
		
	while(1){   // While loop is always true (1), so this loop will execute continuously until program termination
		if(check_dancing_conditions()){ // If dance condition returns TRUE, robot will execute dance function
			dance();
		}
		else if(check_escape_conditions()){  // If escape condition returns TRUE, robot will execute escape function
			escape();
		}
		else if(check_approach_conditions()){   // If approach condition returns TRUE, robot will execute approach function
			approach();
		}
		else{straight_cruise();}   // If cruise condition returns TRUE, robot will execute cruise function

	}
}

void drive(float left, float right, float delay_seconds){  // arguments are - left: percent motor activation to left wheel, right: percent motor activation to right wheel, delay_seconds: amount of time the motor activations will be executed for
	// This function is called whenever we want the robot to move via its wheels 
	
    float left_speed;
    float right_speed;
  

    delay_seconds *= 1000;
    
    right_speed = TOP_SPEED - (right * TOP_SPEED);  // converts percents to actual motor values
    left_speed  = TOP_SPEED + (left * TOP_SPEED);
    
    set_servo_position(LEFT_MOTOR, left_speed);  // actual execution of motor values 
    set_servo_position(RIGHT_MOTOR, right_speed);
    msleep(delay_seconds);  // actually instantiates amount of time motor executions are allowed to run
}



/******************************************************/
int check_escape_conditions(){ // This function returns TRUE if the front bumper is activated above zero
    int bump_threshold = 0;  // Value above which function will return TRUE
    int bump_max = 400; // Bumper sensor should not output above this value 
    int front_bump_value = analog10(FRONT_BUMP);  // getting output value from front bumper
    
    if(front_bump_value >= bump_threshold && front_bump_value <= bump_max){  // if front bumper value is above 0 and below 400 return TRUE, else FALSE
   	 return TRUE;
    }else{
   	 return FALSE;
    }
}
/******************************************************/
void escape(){  // This function executes the escape maneuver (back up and turn)
    int bump_threshold = 250;
    int bump_max = 400;
    int front_bump_value = analog10(FRONT_BUMP); // getting bumper value
    if(front_bump_value < bump_threshold){ // if value is below 250, indicating collision on left side of bumper
   	 drive(-0.0, -0.90, 0.50);   // back up and turn to face right
    }
    else if(front_bump_value >= bump_threshold && front_bump_value <= bump_max){  // if value is between 250 and 400, indicating collision on right side of bumper 
   	 drive(-0.90, -0.0, 0.50); // back up and turn to face left
    }

}
/********************************************/

void straight_cruise(){
	drive(0.40, 0.40, 0.5); // go straight for 0.5 seconds at 40% speed
}


/******************************************************/
int check_approach_conditions(){  // This function returns TRUE if a blue object that has an area above a certain threshold is detected by the camera
	int a;
	int area_blue = 0;
	camera_update(); 
	int min_blue_area = 500;  // area above which a blue object will be approached 
	int num_blue = get_object_count(BLUE_CH); // getting the first 10 blue objects in the camera's view
	
	if(num_blue > 0){  // If there are any blue objects in view
		for(a = 0; a < num_blue; a++) {  // this for loop goes through each blue object and assigns the largest object to the variable area_blue
			int obj_area = get_object_area(BLUE_CH, a);  
			if ((obj_area > min_blue_area) && (obj_area > area_blue)) {  
				area_blue = obj_area;
		}
		}
	}
	if(area_blue > min_blue_area){  // If the largest blue object is above 500 area return TRUE. Else FALSE
		return TRUE;
	} else {return FALSE;}
}

/******************************************************/
void approach(){  // This function turns the robot towards the largest blue object in view

	camera_update();
	num_blue = get_object_count(BLUE_CH); // Gets number of blue objects in view
	int x_con = get_object_center(BLUE_CH, 0).x; // Gets the x coordinate of the first (largest) blue object in view
	if(x_con == 90) {    // if x coordinate is 90 (indicating object is directly in front
		drive(0.5, 0.5, 0.5);  // drive straight at half speed for half a second
	}
	else if (x_con < 90) {  // if x coordinate is less than 90, indicating object is to the left
		float leftDegree = 0.85; // set left turn value to 85% full speed
		if (x_con < 80) { // If x coordinate is less than 80, indicating object is fairly far to the left
			leftDegree = 1; // set left turn value to 100% full speed
		}
		drive(0.1, leftDegree, 0.5);  // turn left for 0.5 seconds
	}
	else { // if x coordinate is not less than 90, this indicates object is to the right
		float rightDegree = 0.85; // set right turn value to 85% full speed
		if (x_con > 100) { // If x coordinate is greater than 100, indicating object is fairly far to the right
			rightDegree = 1; // set right turn value to 100% full speed
		}
		drive(rightDegree, 0.1, 0.5); // turn right for 0.5 seconds
	}
	
}
/******************************************************/

int check_dancing_conditions(){  // this function checks if 1) a blue object is in view, and 2) the object is close enough to start dancing. 
	int other_robot_present = check_approach_conditions();  // TRUE if blue object is still in view
	
    int right_ir_value = analog_et(RIGHT_IR); // gets IR sensor value
	if (right_ir_value > 380) { // if IR is over threshold
		close_enough = TRUE;  
    }
	if (other_robot_present && close_enough){ // if blue object is in view and the IR value indicates robot is close to it
		return TRUE;
	}else{ 
		return FALSE;}
}

/******************************************************/

void dance(){    // sequence of motor commands (choreography) for our robot?s dance
	drive(0.2,0.2,0.05);
	drive(-0.2,-0.2,0.05);
	drive(0.2,0.2,0.05);
	drive(-0.2,-0.2,0.05);
	drive(0.2,0.2,0.05);
	drive(-0.2,-0.2,0.05);
	drive(0.2,0.2,0.05);
	drive(-0.2,-0.2,0.05);
	drive(0.2,0.2,0.05);
	drive(-0.2,-0.2,0.05);
	drive(0.2,0.2,0.05);
	drive(-0.2,-0.2,0.05);
	drive(0.2,0.2,0.05);
	drive(-0.2,-0.2,0.05);
	drive(0.2,0.2,0.05);
	drive(-0.2,-0.2,0.05);
	drive(0.2,0.2,0.05);
	drive(-0.2,-0.2,0.05);
	drive(0.2,0.2,0.05);
	drive(-0.2,-0.2,0.05);
	drive(0,0.5,0.1);
	drive(0.5,0,0.1);
	drive(0,0.5,0.1);
	drive(0.5,0,0.1);
	drive(0,0.5,0.1);
	drive(0.5,0,0.1);
	drive(0,0.5,0.1);
	drive(0.5,0,0.1);
	drive(-0.5,-0.05,0.1);
	drive(-0.05,-0.5,0.1);
	drive(-0.5,-0.05,0.1);
	drive(-0.05,-0.5,0.1);
	drive(0,0.5,0.1);
	drive(0.5,0,0.1);
	drive(0,0.5,0.1);
	drive(0.5,0,0.1);
	drive(0,0.5,0.1);
	drive(0.5,0,0.1);
	drive(0,0.5,0.1);
	drive(0.5,0,0.1);
	drive(-0.5,-0.05,0.1);
	drive(-0.05,-0.5,0.1);
	drive(-0.5,-0.05,0.1);
	drive(-0.05,-0.5,0.1);
	drive(0.2,0.2,0.05);
	drive(-0.2,-0.2,0.05);
	drive(0.2,0.2,0.05);
	drive(-0.2,-0.2,0.05);
	drive(0.2,0.2,0.05);
	drive(-0.2,-0.2,0.05);
	drive(0.2,0.2,0.05);
	drive(-0.2,-0.2,0.05);
	drive(0.2,0.2,0.05);
	drive(-0.2,-0.2,0.05);
	drive(0.2,0.2,0.05);
	drive(-0.2,-0.2,0.05);
	drive(0.2,0.2,0.05);
	drive(-0.2,-0.2,0.05);
	drive(0.2,0.2,0.05);
	drive(-0.2,-0.2,0.05);
	drive(0.2,0.2,0.05);
	drive(-0.2,-0.2,0.05);
	drive(0.2,0.2,0.05);
	drive(-0.2,-0.2,0.05);
	drive(1,0,4);
}

