/*
  @author - Ian Goegebuer <iangoegebuer.com>
*/
#include <MAKERPad.h>

MAKERPad mp = MAKERPad();

int game[16];
int gamecount = 2;
int gamecountstart = 3;
int gamecurrent = 0;
int gamedifficulty = 0;

void setup(){
 LEDWipe();
 startRound(3,1);
}

void LEDWipe() {
  // Do a pretty looking screen wipe!
  for(int i = 0; i < 5; i++)
    for(int j = 0; j < 4; j++) {
      mp.Set(i*4+j);
      delay(25);
    }

  for(int i = 0; i < 5; i++)
    for(int j = 0; j < 4; j++) {
      mp.Clear(i*4+j);
      mp.Set(19);
      delay(25);
    }

   // Turn on the status LED
   mp.Set(19);

}

void startRound(int count, int diff) {
  gamecount = count;
  gamedifficulty = diff;
  gamecurrent = 0;

  // Generate random combos!
  for(int i = 0; i < count; i++) {
    game[i] = random(0,16);
    for(int j = 0; j < i; j++) if(game[i] == game[j]) {
      i--;
      continue;
    }
  }

  // Light em up!
  for(int i = 0; i < count; i++) {
     mp.Set(game[i]);
     gamedelay();
  }
}

void gameshowscore() {
  // Count up to show how many levels you got!
  for(int j = 0; j < gamecount; j++) {
      mp.Set(j);
      delay(25);
    }
}

void gamedelay() {
  delay((3-gamedifficulty)*100);
}

void loop(){
  // Update the buttons down
  mp.UpdateButtons();

  // Make a random number to keep it random
  int temp = random(0,16);

  // Iterate through buttons
  for(int j = 0; j < 4; j++) {
    for(int i = 0; i < 4; i++)
     {
       if(mp.CheckButtonPressed(j*4+i))
       {
          // If the currect button was pressed clear it!
          if(game[gamecurrent] == j*4+i) {
            gamecurrent++;
            mp.Clear(j*4+i);
            return;
          } else {
            // Only worry if the wrong lit button was pressed
            // This helps with buttons double firing
            // This way we don't need to debounce
            for(int k = gamecurrent; k < gamecount; k++) {
              if(game[k] == j*4+i) {
                tone(6, 110,500);
                LEDWipe();
                gameshowscore();
              }
            }
          }
       }
     }
  }

  // Yay! We finished the level!
  // Increase the number of lights and start again!
  if(gamecurrent == gamecount) {
    gamedelay();
    gamedelay();
    gamedelay();
    gamecount++;
    startRound(gamecount,gamedifficulty);
  }

  // Top row of buttons chooses level
  // Level is just the show up delay
  if(mp.CheckButtonPressed(16)) {
    gamecountstart = 3;
    LEDWipe();
    startRound(3,0);
  } else if(mp.CheckButtonPressed(17)) {
    gamecountstart = 3;
    LEDWipe();
    startRound(3,1);
  } else if(mp.CheckButtonPressed(18)) {
    gamecountstart = 3;
    LEDWipe();
    startRound(3,2);
  }

  // A little bit of a delay.
  delay(2);
}
