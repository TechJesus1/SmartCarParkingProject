#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Initialize the LCD with I2C address 0x27 and set it to a 16x2 display
LiquidCrystal_I2C lcd(0x27,16,2);   

#include <Servo.h> 

// Create a Servo object to control the servo motor
Servo myservo;

// Define the IR sensor pins
int IR1 = 2;
int IR2 = 3;

// Initialize the total number of parking slots
int Slot = 4;          

// Flags to keep track of IR sensor states
int flag1 = 0;
int flag2 = 0;

void setup() {
  Serial.begin(9600); 

  // Initialize the LCD and turn on the backlight
  lcd.init(); 
  lcd.backlight(); 

  // Set the IR sensor pins as inputs
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);

  // Attach the servo to pin 4 and set it to the initial position (closed)
  myservo.attach(4);
  myservo.write(100);

  // Display the welcome message on the LCD
  lcd.setCursor (0,0);
  lcd.print("     ARDUINO    ");
  lcd.setCursor (0,1);
  lcd.print(" PARKING SYSTEM ");
  delay (2000);
  lcd.clear();  
}

void loop(){ 
  // Check if a car is detected by the first IR sensor and if the flag is not set
  if(digitalRead (IR1) == LOW && flag1 == 0){
    if(Slot > 0) { // Check if there are available parking slots
      flag1 = 1;
      if(flag2 == 0) { // If the second IR sensor flag is not set, open the gate and decrease the slot count
        myservo.write(0); 
        Slot = Slot - 1;
      }
    } else { // If no slots are available, display "Parking Full" message
      lcd.setCursor (0,0);
      lcd.print("    SORRY :(    ");  
      lcd.setCursor (0,1);
      lcd.print("  Parking Full  "); 
      delay (3000);
      lcd.clear(); 
    }
  }

  // Check if a car is detected by the second IR sensor and if the flag is not set
  if(digitalRead (IR2) == LOW && flag2 == 0) {
    flag2 = 1;
    if(flag1 == 0) { // If the first IR sensor flag is not set, open the gate and increase the slot count
      myservo.write(0); 
      Slot = Slot + 1;
    }
  }

  // If both IR sensors are triggered, reset the flags and close the gate
  if(flag1 == 1 && flag2 == 1) {
    delay (1000); // Delay to ensure the car has passed through
    myservo.write(100); // Close the gate
    flag1 = 0; 
    flag2 = 0;
  }

  // Display the available slots on the LCD
  lcd.setCursor (0,0);
  lcd.print("    WELCOME!    ");
  lcd.setCursor (0,1);
  lcd.print("Slot Left: ");
  lcd.print(Slot);
}
