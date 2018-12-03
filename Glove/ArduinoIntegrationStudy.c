#include <Servo.h>//library for motors
	#include <Wire.h>//library for accelerometer, which measures movement of top of hand

	#define address 0x1E //0011110b, I2C 7bit address of HMC5883

	//////////////////////////////////STRUCT AND VARIABLE DEFINITIONS////////////////////////////////////////////////
	struct sensor{//creating a struct to manage all six sensors
	   int port;//port of corresponding sensor
	   long int value, minvalue, maxvalue;//degree to which sensor is flexed, and range of numbers that each flex sensor bends to; roughly 600-900. adjusted in code later
	};

	struct servo{
	  int port;//port of corresponding motor
	  long int value;//position of motor
	  Servo objservo;//storing the object 'servo'. this works with 'move' and 'attach' commands to command the actual motor
	};

	int ac_x, ac_y, ac_z;//xyz values of accelerometer- note that accelerometer uses ports 4 and 5 within the analog input.

	//NOTE: both Arrays should only require 5 ports, but for some goddamn reason that doesn't work. doing so causes minvalue/maxvalue to kill the program through their very existence.
	sensor SensorArray[5];//creating vector to store sensor structs
	servo ServoArray [5];//creating vector to store motor structs
	int pairnumber =  4;//number of paired servos/sensors

	//////////////////////////////////PROGRAM BEGIN////////////////////////////////////////////////
	void setup() {
	Serial.begin(9600);//sets up console window
	Wire.begin();//sets up wire that transmits data from accelerometer

	  //Put the HMC5883 IC into the correct operating mode
	  Wire.beginTransmission(address); //open communication with HMC5883
	  Wire.write(0x02); //select mode register
	  Wire.write(0x00); //continuous measurement mode
	  Wire.endTransmission();

	for(int x  = 0; x <= (pairnumber-1); x++){//defining all six structs within the vector, and initializing values
	  SensorArray[x] = {x, 0, 1000, 0};//600-900 is the rough range of the sensors. range is adjusted over time within the program. The range is reversed to allow for the actual min/max to be established correctly
	  ServoArray[x] = {(x+2), 0};//'+2' shifts ports to correct value (servo ports go from 3-7 rather than 0-4 in the case of the flex sensors)
	  ServoArray[x].objservo.attach(ServoArray[x].port);//a bit redundant(port is both stored in the struct, and used immediately then never again- but I figure port number might be good info to have for later, and it will make the struct more comprehensive)
	}
	}
	void loop() {

	//////////////////////////////////UPDATING VARIABLES AND MOVING MOTORS////////////////////////////////////////////////

	//accelerometer values being updated:

	//Tell the HMC5883L where to begin reading data
	  Wire.beginTransmission(address);
	  Wire.write(0x03); //select register 3, X MSB register
	  Wire.endTransmission();

	//Read data from each axis, 2 registers per axis
	  Wire.requestFrom(address, 6);//requests reading of zix bytes of data, one at a time (chip automatically increments which data is read, and
	  if(6<=Wire.available()){
	    ac_x = Wire.read()<<8; //X msb
	    ac_x |= Wire.read(); //X lsb
	    ac_z = Wire.read()<<8; //Z msb
	    ac_z |= Wire.read(); //Z lsb
	    ac_y = Wire.read()<<8; //Y msb
	    ac_y |= Wire.read(); //Y lsb
	}

	for(int x  = 0; x <= (pairnumber-1); x++){//this for loop runs through each servo and sensor pair (1-5, or 0-6)

	SensorArray[x].value = analogRead(SensorArray[x].port);//updating 'flex' magnitude to current values

	if(SensorArray[x].value > SensorArray[x].maxvalue) {SensorArray[x].maxvalue = SensorArray[x].value;}//adjusting range of numbers that each flex sensor has.Here, if a number exceeds the range, it becomes the new maximum
	if(SensorArray[x].value < SensorArray[x].minvalue) {SensorArray[x].minvalue = SensorArray[x].value;}//here, if a number is below the minimum, it becomes the new minimum.

	//min/maxvalue is the flex sensor reading, servo's range is 0-180
	ServoArray[x].value = map(SensorArray[x].value, SensorArray[x].minvalue, SensorArray[x].maxvalue, 0, 180);//map() function reduces range of 0-1023 (0 to 5 volts) to 600-900
	ServoArray[x].value = constrain(ServoArray[x].value, 0, 180);//constrain() 'clips' numbers into a specifid range (if above/below range, makes that number max/min of range. if within range, no change)

	ServoArray[x].objservo.write(ServoArray[x].value);//move motor to position identified via map()/constrain()/analogRead()

	//////////////////////////////////CONSOLE OUPUT OPTIONS////////////////////////////////////////////////

	//for testing all flex sensor positions
	/*Serial.print("sensor");
	Serial.print(x);
	Serial.print(": ");
	Serial.print(SensorArray[x].value);
	Serial.print(", ");*/

	//for testing the flex sensor measured min/max values
	/*Serial.print("sensor_min_");
	Serial.print(x);
	Serial.print(": ");
	Serial.print(SensorArray[x].minvalue);
	Serial.print("sensor_max_");
	Serial.print(x);
	Serial.print(": ");
	Serial.print(SensorArray[x].maxvalue);*/


	}//end of for loop

	//for testing the accelerometer data (runs once outside of flex sensor for loop)
	Serial.print("x: ");
	Serial.print(ac_x);
	Serial.print(", y: ");
	Serial.print(ac_y);
	Serial.print(", z: ");
	Serial.print(ac_z);

	Serial.println(".");//puts all console commands on one line, moves to next line for new loop
	delay(250);//delays code to make data more comprehensible (and not make everything work harder than it needs to)
	}//end of 'loop' section
