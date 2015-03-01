/*
  @author - Ian Goegebuer <iangoegebuer.com>
  Use this program to get the keys: http://sourceforge.net/projects/serialporttokey/
*/
#include <MAKERPad.h>

MAKERPad mp = MAKERPad();

void setup(){
 Serial.begin(9600);
 mp.Set(19);
}

String buttons2keys[] = {"0",
                         "0",
                         ".",
                         "{ENTER}",
                         "1",
                         "2",
                         "3",
                         "{ENTER}",
                         "4",
                         "5",
                         "6",
                         "{ADD}",
                         "7",
                         "8",
                         "9",
                         "{ADD}",
                         "{DIVIDE}",
                         "{MULTIPLY}",
                         "{SUBTRACT}",
                         "WTFHOWWASIPRESSED?"};

void loop(){
  // Update the buttons down
  mp.UpdateButtons();

  // Clear and reset status
  mp.Clear();
  mp.Set(19);

  // Iterate through buttons
  for(int j = 0; j < 5; j++) {
    for(int i = 0; i < 4; i++)
     {
       // Check for one shot presses.
       // Will only be called once per button down
       if(mp.CheckButtonPressed(j*4+i))
       {
          // Toggle the LED
          mp.Set(j*4+i);

          // Send command
          Serial.print(buttons2keys[j*4+i]);
       }
     }
  }

  // A little bit of a delay.
  delay(100);
}
