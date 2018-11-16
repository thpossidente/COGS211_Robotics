const int BLUE = 0;

void main(){
	printf("hello");
	int num_blue;
	int area_blue;
	int i;
	camera_open(HIGH_RES);

	while(1){
		camera_update();
		num_blue = get_object_count(BLUE);
		printf("object count = %d\n",num_blue);
		if(num_blue > 0){
			for(i=0; i<num_blue; i++){
				area_blue = get_object_area(BLUE, i);
				printf("blue area = %f\n",(float)area_blue);
				printf("goodbye");
			}
		}
	}
}
