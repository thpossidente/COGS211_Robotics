const int FRONT_BUMP = 0;
const int LEFT_IR = 4;
const int RIGHT_IR = 5;
const int LEFT_MOTOR = 0;
const int RIGHT_MOTOR = 3;
const float TOP_SPEED = 1023.0;//100;
const int BLUE_CH = 0;
const int TRUE = 1;
const int FALSE = 0;

int num_blue;




void main() 
{
	enable_servos();
	int i;
    camera_open(HIGH_RES);
		
	while(1){
		if(check_escape_conditions()){
			escape();
		}else if(check_approach_conditions()){
			approach();
		}else{straight_cruise();
			}

	}
}

void drive(float left, float right, float delay_seconds){
    float left_speed;
    float right_speed;
    
    delay_seconds *= 1000;
    //printf("left = %f, right = %f, TOP_SPEED = %f\n", left, right, TOP_SPEED);
    
    right_speed = TOP_SPEED - (right * TOP_SPEED);
    left_speed  = TOP_SPEED + (left * TOP_SPEED);
    
    //printf("left_speed = %f, right_speed = %f\n", left_speed, right_speed);    
    set_servo_position(LEFT_MOTOR, left_speed);
    set_servo_position(RIGHT_MOTOR, right_speed);
    msleep(delay_seconds);
    //disable_servos();
}



/******************************************************/
int check_escape_conditions(){
    int bump_threshold = 0;
    int bump_max = 400;
    int front_bump_value = analog10(FRONT_BUMP);
    
    if(front_bump_value >= bump_threshold && front_bump_value <= bump_max){
   	 return TRUE;
    }else{
   	 return FALSE;
    }
}
/******************************************************/
void escape(){
    int bump_threshold = 250;
    int bump_max = 400;
    int front_bump_value = analog10(FRONT_BUMP);
    if(front_bump_value < bump_threshold){
   	 drive(-0.0, -0.90, 0.50);
    }
    else if(front_bump_value >= bump_threshold && front_bump_value <= bump_max){
   	 drive(-0.90, -0.0, 0.50);
    }

}
/********************************************/

void straight_cruise(){
	drive(0.40, 0.40, 0.5);
}

/********************************************/

void stop(){
	drive(0.0, 0.0, 1);
}


/******************************************************/
int check_approach_conditions(){
	int a;
	int area_blue = 0;
	camera_update();
	int min_blue_area = 500;
	int num_blue = get_object_count(BLUE_CH); 
	
	if(num_blue > 0){
		for(a = 0; a < num_blue; a++) {
			int obj_area = get_object_area(BLUE_CH, a);
			if ((obj_area > min_blue_area) && (obj_area > area_blue)) {
				area_blue = obj_area;
		}
		}
	}
	if(area_blue > min_blue_area){
		return TRUE;
	} else {return FALSE;}
}

/******************************************************/
void approach(){
	//get the x-coordinates of the other robot and move based on them
	// need to determine which index is the object we are getting the 
	// x coordinate for. This will likely require a for loop similar to the
	// one in check_approach_conditions. 
	
	camera_open(HIGH_RES);
	camera_update();
	num_blue = get_object_count(BLUE_CH);
	float x_con = get_object_center(BLUE_CH, 0).x;
	printf("x coordinate: %f", x_con);
	if(x_con == 0) {
		drive(0.5, 0.5, 0.5);
		printf("straight");
	}
	else if (x_con < 0) {
		drive(0.25, 0.75, 0.5);
		printf("Left");
	}
	else {
		drive(0.75, 0.25, 0.5);
		printf("Right");
	}
	
}
/******************************************************/
