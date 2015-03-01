/*
  @author - Ian Goegebuer <iangoegebuer.com>
*/
#include <MAKERPad.h>

MAKERPad mp = MAKERPad();

void setup(){
 Serial.begin(9600);
 mp.Set(19);
}


void loop(){
  // Update the buttons down
  mp.UpdateButtons();

  // Iterate through buttons
  for(int j = 0; j < 5; j++) {
    for(int i = 0; i < 4; i++)
     {
       // Check for one shot presses.
       // Will only be called once per button down
       if(mp.CheckButtonPressed(j*4+i))
       {
          // Toggle the LED
          mp.Toggle(j*4+i);
          Serial.print("Button: ");
          Serial.print(j);
          Serial.print(" - ");
          Serial.println(i);
       }
     }
  }

  // A little bit of a delay.
  delayMicroseconds(500);
}
