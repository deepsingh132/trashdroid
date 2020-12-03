#include<SoftwareSerial.h>

SoftwareSerial GPRS(5, 6);

int buzzerPin = 8;                          //Pin number the buzzer is connected

int echoPin = 11;                           // Echo Pin of the ultrasonic sensor

int trigPin = 12;                          // Trigger Pin of the ultrasonic sensor

unsigned long previousMillis = 0;        //This will be used to store the millisecnd value after each time the buzzer is triggered. This is used because delay function is not used for the buzzer timing.

int maxBeepInterval = 1000;             //Max beep interval

int minBeebInterval = 300;             //Min beep interval

int frequency = 1000;                    //Buzzer frequency in Hz

long distance;                              //Distance of the object from sensorlong echoTime Time taken for the echo

int triggerAlarmDistance = 10;              //Buzzer will be triggered if the distance is less than this value. (cm)

int echoTime;

void setup()

{

	pinMode(buzzerPin, OUTPUT);                   //Set pin modes

	pinMode(echoPin, INPUT);

	pinMode(trigPin, OUTPUT);

	GPRS.begin(9600);                         // the GPRS baud rate

	Serial.begin(9600);                  // the Serial port of Arduino baud rate.

	delay(500);

}

void loop()

{

	digitalWrite(trigPin, LOW);                   //Set the trigger pin of the SR04 sensor to low

	delayMicroseconds(2);

	digitalWrite(trigPin, HIGH);                //Set the trigger pin of the SR04 sensor to high which will which will send a pulse.

	delayMicroseconds(10);

	digitalWrite(trigPin, LOW);                 //Set the trigger pin of the SR04 sensor to low

	echoTime = pulseIn(echoPin, HIGH);        //Reads a pulse (either HIGH or LOW) on a pin. For example, if value is HIGH, pulseIn() waits for the pin to go HIGH, starts timing, then waits for the pin to go LOW and stops timing. Returns the length of the pulse in microseconds or 0 if no complete pulse was received within the timeout https://www.arduino.cc/en/Reference/PulseIn

	distance = echoTime * 0.0340 / 2;

	if(distance < triggerAlarmDistance)

	{

		unsigned long currentMillis = millis();

		int val = map(distance, 2, triggerAlarmDistance, minBeebInterval, maxBeepInterval);

		sendSMS();

		Serial.println("Sent SMS!");

		Serial.println( "dustbin is reached max level");

		delay(10);

		if (currentMillis - previousMillis >= val)

		{

			tone(buzzerPin, frequency, val * 0.75);          //Trigger the buzzer (Buzzer pin, frequency, duration). Here durationn is set to 75% of the time before it will be triggered next.

			previousMillis = currentMillis;            //Save current millisend value when the buzzer is triggered.

		}

	}

	delay(6000);                    //Delay 60ms before next reading.

}


void sendSMS()

{

	GPRS.println("AT+CMGF=1");      // Set modem to text mode

	delay(2000);

	GPRS.write("AT+CMGS=");           // Start composing message

	GPRS.write(0x22);             // hex equivalent of double-quote '"'

	GPRS.write("+xxxxxxx");           // the number to send the SMS to

	GPRS.write(0x22);
	GPRS.write(0x0D);              // hex equivalent of Carraige return

	GPRS.write(0x0A);                 // hex equivalent of newline

	delay(2000);

	GPRS.print("ID:=1 LOCATION:=VISIT  THE ABOVE DUSTBIN REACHED ITS MAXIMUM CAPACITY PLEASE COME AND COLLECT TO KEEP CITY CLEAN"); // Send the text message to the GSM module

	GPRS.write(0x0D);            // hex equivalent of Carraige return

	GPRS.write(0x0A);        // hex equivalent of newline

	delay(1000);

	GPRS.write(26);            // equivalent of CTRL-Z delay(3000);

}
