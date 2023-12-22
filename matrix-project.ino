#include <LiquidCrystal.h>
#include <LedControl.h>

const byte rs = 9;
const byte en = 8;
const byte d4 = 7;
const byte d5 = 6;
const byte d6 = 5;
const byte d7 = 4;

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

LedControl lc = LedControl(12, 11, 10, 1);

const int LCDbrightnessPin = 3;
const int pinSW = 2;
const int joystickXPin = A0;
const int joystickYPin = A1;

int freqBuzzer = 700;
int timeBuzzer = 500;
const int pinBuzzer = 13;

byte yJoyMoved = false;
byte xJoyMoved = false;
const int minThreshold = 200;
const int maxThreshold = 600;

unsigned long debounceDelay = 50;
unsigned long lastDebounceTime = 0;
int buttonState;
int lastButtonState = LOW;
bool buttonValue = true;

int menuOption = 1;
bool inMenu = false;
int settingsMenuState = 1;
int aboutMenuState = 1;

int LCDbrightness = 6;
byte matrixBrightness = 6; 

bool sound = 1;

char player[3] = { 'A', 'A', 'A' };
int nameState = 0;
int highscoreMenuState = 1;

//////////////////////////////////////////////////////////////

const int matrixSize = 8;

byte mapPlay[matrixSize][matrixSize];

int playerX = 0;
int playerY = 0;

struct Bomb {
  int x;
  int y;
  unsigned long startTime;
  bool isBlinking;
  unsigned long lastBlinkTime;
};

struct player {
  char name[3] = { '-', '-', '-' };
  int score = 0;
} first, second, third;

const int maxBombs = 5; // Maximum number of bombs allowed
Bomb bombs[maxBombs];   // Array to store bomb information
int numBombs = 0;       // Number of currently active bombs

bool isPlayerVisible = true;
bool isPlayerBlinking = true;
unsigned long lastPlayerBlinkTime = 0;
const unsigned long playerBlinkInterval = 500;

bool isPlayerMoving = false;
unsigned long lastMoveTime = 0;
const unsigned long moveInterval = 200;

int lastSwState = HIGH;
int swState = HIGH;

bool bombLedState = false;
const unsigned long bombDetonationTime = 3000; 
const unsigned long bombBlinkInterval = 250;  
bool isDead = 0;
bool youWin = 0;

int currentLevel = 1; 

int ledsLevel1 = 21;
int ledsLevel2 = 31;
int ledsLevel3 = 45;

const unsigned long level3TimeLimit = 60000; 
unsigned long level3StartTime;
unsigned long level3ElapsedTime;

bool isPlaying = 0;
bool wasHighscore = 0;
bool start = 0;

byte down[8] = {
  0b00000,
  0b00000,
  0b00000,
  0b11111,
  0b01110,
  0b00100,
  0b00000,
  0b00000
};

byte up[8] = {
  0b00000,
  0b00000,
  0b00100,
  0b01110,
  0b11111,
  0b00000,
  0b00000,
  0b00000
};

byte select[8] = {
  0b00000,
  0b00100,
  0b00110,
  0b11111,
  0b11111,
  0b00110,
  0b00100,
  0b00000
};

byte right[8] = {
  0b00000,
  0b01000,
  0b01100,
  0b01110,
  0b01110,
  0b01100,
  0b01000,
  0b00000
};

byte left[8] = {
  0b00000,
  0b00010,
  0b00110,
  0b01110,
  0b01110,
  0b00110,
  0b00010,
  0b00000
};

void setup() {
  randomSeed(analogRead(A3));
  lcd.begin(16, 2); 
  pinMode(pinSW, INPUT_PULLUP);
  pinMode(LCDbrightnessPin, OUTPUT);

  pinMode(joystickXPin, INPUT);
  pinMode(joystickYPin, INPUT);

  lc.shutdown(0, false);
  lc.setIntensity(0, matrixBrightness);
  lc.clearDisplay(0);

  youWin = 0;
  isDead = 0;
  currentLevel = 1;
  isPlayerBlinking = true;
  wasHighscore = 0;

  // analogWrite(LCDbrightnessPin, LCDbrightness * 10);
  generateMap(ledsLevel1);
}

void loop() {
  lc.setIntensity(0, matrixBrightness);
  // analogWrite(LCDbrightnessPin, LCDbrightness * 10);
  if (millis() < 3000) { //the welcome message is displayed for 3 seconds
    lcd.setCursor(0, 0);
    lcd.print("Welcome to");
    lcd.setCursor(0, 1);
    lcd.print("Bomberman");
  } else {
    buttonDebounce();
    
    inMenu = buttonValue; 

    // the button debounce logic it that if the button is in the false state the main menu is diplayed
    // and if the button state is true the second menu is displayed
    if (inMenu == false and menuOption != 6) {
      settingsMenuState = 1;
      nameState = 0;
      mainMenu();
      navigate();
    } else {
      secondMenu(); 
    }
  }
}


void mainMenu() {
  lcd.createChar(1, select);
  lcd.createChar(2, up);
  lcd.createChar(3, down);

  switch (menuOption) {
    case 0:
      menuOption = 1;
      break;

    case 1:
      
      lcd.clear();
      lcd.print(" About");
      lcd.setCursor(0, 1);
      lcd.print(" How to play");
      lcd.setCursor(15, 1);
      lcd.write(3);
      lcd.setCursor(0, 0);
      lcd.write(1);
      break;

    case 2:
      lcd.clear();
      lcd.print(" How to play");
      lcd.setCursor(15, 0);
      lcd.write(2);
      lcd.setCursor(0, 1);
      lcd.print(" Settings");
      lcd.setCursor(15, 1);
      lcd.write(3);
      lcd.setCursor(0, 0);
      lcd.write(1);
      break;

    case 3:
      lcd.clear();
      lcd.print(" Settings");
      lcd.setCursor(15, 0);
      lcd.write(2);
      lcd.setCursor(0, 1);
      lcd.print(" Highscore");
      lcd.setCursor(15, 1);
      lcd.write(3);
      lcd.setCursor(0, 0);
      lcd.write(1);
      break;

    case 4:
      lcd.clear();
      lcd.print(" Highscore");
      lcd.setCursor(15, 0);
      lcd.write(2);
      lcd.setCursor(0, 1);
      lcd.print(" Reset Highs");
      lcd.setCursor(15, 1);
      lcd.write(3);
      lcd.setCursor(0, 0);
      lcd.write(1);
      break;

    case 5:
      lcd.clear();
      lcd.print(" Reset Highs");
      lcd.setCursor(15, 0);
      lcd.write(2);
      lcd.setCursor(0, 1);
      lcd.print(" Start game");
      lcd.setCursor(15, 1);
      lcd.write(3);
      lcd.setCursor(0, 0);
      lcd.write(1);
      break;

    case 6:
      lcd.clear();
      lcd.print(" Reset Highs");
      lcd.setCursor(15, 0);
      lcd.write(3);
      lcd.setCursor(0, 1);
      lcd.print(" Start game");
      lcd.setCursor(0, 1);
      lcd.write(1);
      break;

    case 7:
      menuOption = 6;
      break;
  }
}

void secondMenu() {

  switch (menuOption) {
    case 1:
      lcd.clear();
      navigateAbout();
      about();
      break;

    case 2:
      lcd.clear();
      howToPlay();
      break;

    case 3:
      lcd.clear();
      navigateSettings();
      settings();
      break;

    case 4:
      lcd.clear();
      navigateHighScore();
      highscore();
      break;

    case 5:
      lcd.clear();
      resetHighscore();
      break;

    case 6:
      lcd.clear();
      startGame();
      playerBombBlink();
      updateLCD();
      break;
  }
  delay(10);
}

void navigate() {
  int yValue = analogRead(joystickYPin);

  if (yJoyMoved == false) {
    if (yValue > maxThreshold) {
      menuOption++;
      mainMenu();
      makeSound(200);
      yJoyMoved = true;
    }
    if (yValue < minThreshold) {
      menuOption--;
      mainMenu();
      makeSound(200);
      yJoyMoved = true;
    }
  }

  if (yValue > minThreshold && yValue < maxThreshold) {
    yJoyMoved = false;
  }
}

///////////////////////////////////////////////////////////////////

void about() {

  switch (aboutMenuState) {

    lcd.createChar(2, up);
    lcd.createChar(3, down);

    case 0:
      aboutMenuState = 1;
      break;
    case 1:
      lcd.clear();
      lcd.print("Name: Bomberman");
      lcd.setCursor(0, 1);
      lcd.print("Saicu Carina");
      lcd.setCursor(15, 1);
      lcd.write(3);
      break;
    case 2:
      lcd.clear();
      lcd.print("Saicu Carina");
      lcd.setCursor(0, 1);
      lcd.print("GH: SaicuCarina");
      lcd.setCursor(15, 0);
      lcd.write(2);
      break;
    case 3:
      aboutMenuState = 2;
      break;
  }

  delay(10);
}

void navigateAbout() {
  int yValue = analogRead(joystickYPin);

    if (yJoyMoved == false) {
      if (yValue > maxThreshold) {
        aboutMenuState++;
        makeSound(200);
        yJoyMoved = true;
      }
      if (yValue < minThreshold) {
        aboutMenuState--;
        makeSound(200);
        yJoyMoved = true;
      }
    }

    if (yValue > minThreshold && yValue < maxThreshold) {
      yJoyMoved = false;
    }
}

////////////////////////////////////////////////////////////////

void howToPlay(){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Destroy walls as");
  lcd.setCursor(0, 1);
  lcd.print("fast as possible");
  delay(10);
}

////////////////////////////////////////////////////////////////

void settings() {

  switch (settingsMenuState) {

    lcd.createChar(1, down);
    lcd.createChar(2, select);
    lcd.createChar(3, up);
    lcd.createChar(4, left);
    lcd.createChar(5, right);
    
    case 0:
      settingsMenuState = 1;
      break;
      
    case 1:
      lcd.clear();
      lcd.print(" Settings menu");
      lcd.setCursor(0, 0);
      lcd.write(1);
      lcd.setCursor(0, 1);
      lcd.write(" LCD Bright:");
      lcd.setCursor(12, 1);
      lcd.print(LCDbrightness);
      lcd.setCursor(15, 1);
      lcd.write(3);
      break;

    case 2:  // LCD BRIGHTNESS CONTROL
      lcd.createChar(4, left);
      lcd.createChar(5, right);

      lcd.clear();
      lcd.write(" LCD Bright:");
      lcd.setCursor(0, 0);
      lcd.write(1);
      lcd.setCursor(14, 0);
      lcd.write(4);
      lcd.setCursor(15, 0);
      lcd.write(5);
      lcd.setCursor(15, 1);
      lcd.write(3);
      lcd.setCursor(0, 1);
      lcd.write(" Mat Bright:");
      lcd.setCursor(12, 0);
      lcd.print(LCDbrightness);
      lcd.setCursor(12, 1);
      lcd.print(matrixBrightness);

      navigateLCDbrightness();

      delay(10);

      break;

    case 3:  // MATRIX BRIGHTNESS CONTROL
      lcd.createChar(4, left);
      lcd.createChar(5, right);

      lcd.clear();
      lcd.print(" Mat Bright:");
      lcd.setCursor(0, 0);
      lcd.write(1);
      lcd.setCursor(14, 0);
      lcd.write(4);
      lcd.setCursor(15, 0);
      lcd.write(5);
      lcd.setCursor(15, 1);
      lcd.write(3);
      lcd.setCursor(0, 1);
      lcd.print(" Sound:");
      lcd.setCursor(12, 0);
      lcd.print(matrixBrightness);
      lcd.setCursor(8, 1);
      if (sound == 1) {
        lcd.print("ON");
      } else {
        lcd.print("OFF");
      }

      navigateMatrixBrightness();
      
      delay(10);

      break;

    case 4:  // SOUND
      lcd.clear();
      lcd.print(" Sound:");
      lcd.setCursor(0, 0);
      lcd.write(1);
      lcd.setCursor(0, 1);
      lcd.print(" Name:");
      lcd.setCursor(7, 1);
      lcd.print(player[0]);
      lcd.print(player[1]);
      lcd.print(player[2]);
      lcd.setCursor(15, 0);
      lcd.write(2);
      lcd.setCursor(15, 1);
      lcd.write(3);
      lcd.setCursor(8, 0);
      if (sound == 1) {
        lcd.print("ON");
      } else {
        lcd.print("OFF");
      }

      navigateSound();

      delay(10);

      break;

     case 5:  // NAME
      lcd.clear();
      lcd.print(" Sound:");
      lcd.setCursor(15, 0);
      lcd.write(2);
      lcd.setCursor(0, 1);
      lcd.write(1);
      lcd.print("Name:");
      lcd.setCursor(7, 1);
      lcd.print(player[0]);
      lcd.print(player[1]);
      lcd.print(player[2]);
      lcd.setCursor(14, 1);
      lcd.write(4);
      lcd.setCursor(15, 1);
      lcd.write(5);
      lcd.setCursor(8, 0);
      if (sound == 1) {
        lcd.print("ON");
      } else {
        lcd.print("OFF");
      }

      navigateName();

      delay(10);

      break;

    case 6:
      settingsMenuState = 5;
      break;
  }

  delay(20);
}

void navigateSettings() {

  int yValue = analogRead(joystickYPin);

    if (yJoyMoved == false) {
      if (yValue > maxThreshold) {
        settingsMenuState++;
        makeSound(200);
        yJoyMoved = true;
      }
      if (yValue < minThreshold) {
        settingsMenuState--;
        makeSound(200);
        yJoyMoved = true;
      }
    }

    if (yValue > minThreshold && yValue < maxThreshold) {
      yJoyMoved = false;
    }
}

void navigateLCDbrightness(){
  int xValue = analogRead(joystickXPin);

  if (xJoyMoved == false) {
    if (xValue > maxThreshold) {
      if (LCDbrightness < 25) {
        LCDbrightness++;
      }
      xJoyMoved = true;
    }
    if (xValue < minThreshold) {
      if (LCDbrightness > 6) {
        LCDbrightness--;
      }
      xJoyMoved = true;
    }
  }

  if (xValue < maxThreshold && xValue > minThreshold) {
    xJoyMoved = false;
  }
}

void navigateMatrixBrightness(){
  int xValue = analogRead(joystickXPin);

  if (xJoyMoved == false) {
    if (xValue > maxThreshold) {
      if (matrixBrightness < 15) {
         matrixBrightness++;
      }
      xJoyMoved = true;
    }
    if (xValue < minThreshold) {
      if (matrixBrightness > 0) {
        matrixBrightness--;
      }
      xJoyMoved = true;
    }
  }

  if (xValue < maxThreshold && xValue > minThreshold) {
     xJoyMoved = false;
  }
}

void navigateSound(){
  int xValue = analogRead(joystickXPin);

    if (xJoyMoved == false) {
      if (xValue > maxThreshold || xValue < minThreshold) {
        sound = !sound;
        xJoyMoved = true;
      }
    }

    if (xValue < maxThreshold && xValue > minThreshold) {
      xJoyMoved = false;
    }
}

void navigateName(){
  int xValue = analogRead(joystickXPin);

      if (xJoyMoved == false) {
        if (xValue > maxThreshold) {
          if (nameState < 3) {
            nameState++;
            player[nameState - 2] = (char)(player[nameState - 2] - 1);
            // because of the delay I had to make the player take the second last character
          }
          xJoyMoved = true;
        }
        if (xValue < minThreshold) {
          if (nameState > 0) {
            nameState--;
            player[nameState] = (char)(player[nameState] - 1);
            // because of the delay I had to make the player take the second last character
          }
          xJoyMoved = true;
        }
      }

      if (xValue < maxThreshold && xValue > minThreshold) {
        xJoyMoved = false;
      }

      // here I make the current character to toggle continuously with a delay of 0.75 sec
      if (player[nameState - 1] != 'Z') {
        player[nameState - 1] = (char)(player[nameState - 1] + 1);
        delay(750);
      } else {
        player[nameState - 1] = 'A';
        delay(750);
      }
}


/////////////////////////////////////////////////////////////////

void highscore() {
  lcd.createChar(2, up);
  lcd.createChar(3, down);
  
  switch (highscoreMenuState) {
    case 0:
      highscoreMenuState = 1;
      break;

    case 1:
      lcd.clear();
      lcd.print("I.  ");
      lcd.print(first.name[0]);
      lcd.print(first.name[1]);
      lcd.print(first.name[2]);
      lcd.print(" Scor:");
      lcd.print(first.score);
      lcd.setCursor(0, 1);
      lcd.print("II. ");
      lcd.print(second.name[0]);
      lcd.print(second.name[1]);
      lcd.print(second.name[2]);
      lcd.print(" Scor:");
      lcd.print(second.score);
      lcd.setCursor(15, 1);
      lcd.write(3);
      break;

    case 2:
      lcd.clear();
      lcd.print("II. ");
      lcd.print(second.name[0]);
      lcd.print(second.name[1]);
      lcd.print(second.name[2]);
      lcd.print(" Scor:");
      lcd.print(second.score);
      lcd.setCursor(0, 1);
      lcd.print("III.");
      lcd.print(third.name[0]);
      lcd.print(third.name[1]);
      lcd.print(third.name[2]);
      lcd.print(" Scor:");
      lcd.print(third.score);
      lcd.setCursor(15, 0);
      lcd.write(2);
      break;

    case 3:
      highscoreMenuState = 2;
      break;
  }

  delay(10);
}

void navigateHighScore() {
  int yValue = analogRead(joystickYPin);

  if (nameState == 0) {
    if (yJoyMoved == false) {
      if (yValue > maxThreshold) {
        highscoreMenuState++;
        makeSound(200);
        yJoyMoved = true;
      }
      if (yValue < minThreshold) {
        highscoreMenuState--;
        makeSound(200);
        yJoyMoved = true;
      }
    }

    if (yValue > minThreshold && yValue < maxThreshold) {
      yJoyMoved = false;
    }
  }
}

///////////////////////////////////////////////////////////

// reset the highscore when the reset menu was enetered

void resetHighscore() {
  first.name[0] = '-';
  first.name[1] = '-';
  first.name[2] = '-';
  first.score = 0;

  second.name[0] = '-';
  second.name[1] = '-';
  second.name[2] = '-';
  second.score = 0;

  third.name[0] = '-';
  third.name[1] = '-';
  third.name[2] = '-';
  third.score = 0;

  lcd.clear();
  lcd.print(" Reset");
  lcd.setCursor(0, 1);
  lcd.print(" Complete");

}

/////////////////////////////////////////////////////////////////

void buttonDebounce() {
  int reading = digitalRead(pinSW);

  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        makeSound(200); // the buzzer makes a sound enery time the sw is pressed
        buttonValue = !buttonValue;
      }
    }
  }

  lastButtonState = reading;
}

void makeSound(int toneSound) {
  // if the sound is on the buzzer makes sounds
  if (sound) {
    tone(pinBuzzer, toneSound, 30);
  }
}

/////////////////////////////////////////////////////////////////////

// when the game is over a sad face is displayed on the matrix
void displaySadFace() {
  byte sadFace[8] = {
    B00000000,
    B00100100,
    B00100100,
    B00100100,
    B00000000,
    B00011000,
    B00100100,
    B00000000
  };

  for (int row = 0; row < 8; row++) {
    lc.setRow(0, row, sadFace[row]);
  }
}

// when the game is won a cup is displayed on the matrix

void displayCup() {
  byte cup[8] = {
    B01000010,
    B11000011,
    B11000011,
    B01000010,
    B00111100,
    B00011000,
    B00011000,
    B00111100
  };

  for (int row = 0; row < 8; row++) {
    lc.setRow(0, row, cup[row]);
  }
}


//a generate a different map for every game with different ocupability for every level
void generateMap(int numLedsLevels) {

  for (int i = 0; i < numLedsLevels; ++i) {
    int row, col;

    row = random(1, matrixSize);
    col = random(matrixSize);

    lc.setLed(0, row, col, true);
    mapPlay[row][col] = 1;
  }
}

// i need the reset map function because when the game is lost and i want to start a new game
// the map have some ghost walls
void resetMap() {
  for(int i=0; i<8; i++){
    for(int j=0; j<8; j++){
      mapPlay[i][j] = 0;
    }
  }
}

void updateLCD() {
  lcd.clear();
  // if i am still playing i want the display to show the level and my name
  if(youWin == 0){
    lcd.setCursor(0, 0);
    lcd.print("Level:");
    lcd.print(currentLevel);
    lcd.setCursor(8, 0);
    lcd.print("Name:");
    lcd.print(player[0]);
    lcd.print(player[1]);
    lcd.print(player[2]);
  }
  else{
    lcd.clear();
    lcd.print("You Won!");
  }
  
  // if the current level is 3 i want to have a countdown on the time remaining
  if (currentLevel == 3) {
    lcd.setCursor(0, 1);
    lcd.print("Time: ");
    lcd.print(max(0, (level3TimeLimit - level3ElapsedTime) / 1000));  // display seconds remaining
    lcd.print("sec");
  }

  // i want the disply to tell me when i have a highscore
  if (wasHighscore == 1) {
    lcd.setCursor(9, 0);
    lcd.print("New HC");
  }

  delay(10);
}

// this function verify if the player succeeded in destroying all the walls
bool isLevelCompleted() {
  for (int row = 0; row < matrixSize; ++row) {
    for (int col = 0; col < matrixSize; ++col) {
      if (mapPlay[row][col] == 1) {
        return false;
      }
    }
  }
  return true;
}

void howToMove() {
  int joystickX = analogRead(joystickXPin);
  int joystickY = analogRead(joystickYPin);

  int deltaX = 0;
  int deltaY = 0;

  if (joystickX < minThreshold) {
    deltaX = -1; // the joystick moved to the left
  } else if (joystickX > maxThreshold) {
    deltaX = 1; // the joystick moved to the rigth
  }

  if (joystickY < minThreshold) {
    deltaY = -1; // the joystick moved up
  } else if (joystickY > maxThreshold) {
    deltaY = 1; // the joystick moved down
  }

  movePlayer(deltaX, deltaY);
}

void movePlayer(int deltaX, int deltaY) {
  isPlayerMoving = (deltaX != 0 || deltaY != 0); // if deltaX != 0 or deltaY != 0 then the isPlayerMoveing = 1;
  // I need this variable because while moeing the player should not blink

  unsigned long currentTime = millis();
  // I need a moveInterval for the player so that the movemente are not too quick
  if (currentTime - lastMoveTime >= moveInterval) {
    // I initialize the new coordinates for the player
    int nextX = playerX + deltaX;
    int nextY = playerY + deltaY;

    if (nextX >= 0 && nextX < matrixSize && nextY >= 0 && nextY < matrixSize && mapPlay[nextY][nextX] != 1) {
      lc.setLed(0, playerY, playerX, false);
      // if the player can move to the new spot the player is given the new coordinates

      playerX = nextX;
      playerY = nextY;
    }

    lastMoveTime = currentTime;
  }
}

void bomb() {
  int reading = digitalRead(pinSW);

  if (reading != lastSwState) {
    lastDebounceTime = millis();
  }

  // if the sw was pressed i have to verify if i do not have to many bombs placed

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != swState) {
      swState = reading;
      if (swState == LOW) {
        if (numBombs < maxBombs) {
          // I can place the new bomb and initalize the startTime(for detonation) and lastBlinkingTime(for blinking)
          bombs[numBombs].x = playerX;
          bombs[numBombs].y = playerY;
          bombs[numBombs].startTime = millis();
          bombs[numBombs].isBlinking = true;
          bombs[numBombs].lastBlinkTime = millis();
          numBombs++;

          isPlayerBlinking = true;
        }
      }
    }
  }

  lastSwState = reading;

  // Update active bombs
  for (int i = 0; i < numBombs; ++i) {
    unsigned long currentTimeBomb = millis();
    if (currentTimeBomb - bombs[i].startTime >= bombDetonationTime) {
      // if my current bomb should explode 
      // I have to verify if my player is not dead (for level 2 and 3)
      // the player is dead if he was in the bomb area when it exploded

      if (currentLevel >= 2 ) {
        if((playerX == bombs[i].x - 1 && playerY == bombs[i].y) || (playerX == bombs[i].x && playerY == bombs[i].y - 1) 
        || (playerX == bombs[i].x + 1 && playerY == bombs[i].y) || (playerX == bombs[i].x && playerY == bombs[i].y + 1)
        || (playerX == bombs[i].x && playerY == bombs[i].y))
          gameOver();
          // the player died and the game is over
      }
      if(isDead == 0){
        // if the player is not dead i have to destry the walls
        destroyWalls(bombs[i].x - 1, bombs[i].y);
        destroyWalls(bombs[i].x, bombs[i].y - 1);
        destroyWalls(bombs[i].x + 1, bombs[i].y);
        destroyWalls(bombs[i].x, bombs[i].y + 1);

        // after the walls have been destroyed the bomb should disappear
        lc.setLed(0, bombs[i].y, bombs[i].x, false);
        // the bomb is removed from the array
        numBombs--;
        bombs[i] = bombs[numBombs];

        isPlayerBlinking = true;
        if(sound == 1){
          tone(pinBuzzer, freqBuzzer, timeBuzzer); // the buzzer makes a sound when a bomb explodes
        }
        
      }
      
    }
  }

  // here i check if any of the levels where completed 

  if (currentLevel == 1 && isLevelCompleted()) {
    currentLevel = 2;
    numBombs = 0;
    playerX = 0;
    playerY = 0;
    // resetMap();
    generateMap(ledsLevel2);
  }

  if(currentLevel == 2 && isLevelCompleted()) {
    currentLevel = 3;
    numBombs = 0;
    playerX = 0;
    playerY = 0;
    // resetMap();
    level3StartTime = millis();  // Start the timer for level 3
    generateMap(ledsLevel3);
  }

  // if the level 3 was completed then the player has won and I have to check if the score is a highscore

  if(currentLevel == 3 && isLevelCompleted()) {
    isPlayerBlinking = false;
    youWin = 1;
    menuOption = 1;
    int currentScore = max(0, (level3TimeLimit - level3ElapsedTime) / 1000);
    if(currentScore > first.score and wasHighscore == 0)
    {
      wasHighscore = 1;

      third.name[0] = second.name[0];
      third.name[1] = second.name[1];
      third.name[2] = second.name[2];
      third.score = second.score;

      second.name[0] = first.name[0];
      second.name[1] = first.name[1];
      second.name[2] = first.name[2];
      second.score = first.score;

      first.name[0] = player[0];
      first.name[1] = player[1];
      first.name[2] = player[2];
      first.score = currentScore;
    }
    else{
      if(currentScore > second.score and wasHighscore == 0){
        wasHighscore = 1;

        third.name[0] = second.name[0];
        third.name[1] = second.name[1];
        third.name[2] = second.name[2];
        third.score = second.score;

        second.name[0] = player[0];
        second.name[1] = player[1];
        second.name[2] = player[2];
        second.score = currentScore;
      }
      else{
        if(currentScore > third.score and wasHighscore == 0){
          wasHighscore = 1;

          third.name[0] = player[0];
          third.name[1] = player[1];
          third.name[2] = player[2];
          third.score = currentScore;
        }
      }
    }
    lcd.clear();
    updateLCD();
    displayCup();
    delay(3000);
    playerX = 0;
    playerY = 0;
    // resetMap();
    setup();
  }

}

// if the wall was on I set it to off and mark it on the map with 0
void destroyWalls(int x, int y) {
  if (x >= 0 && x < matrixSize && y >= 0 && y < matrixSize && mapPlay[y][x] == 1) {
    lc.setLed(0, y, x, false);
    mapPlay[y][x] = 0;
  }
}

void playerBombBlink() {
  unsigned long currentTime = millis();

  // the player is blinking with the playerBlinkInterval
  if (isPlayerBlinking && currentTime - lastPlayerBlinkTime >= playerBlinkInterval) {
    isPlayerVisible = !isPlayerVisible;
    lastPlayerBlinkTime = currentTime;
  }

  // I add this check so that when the game is over and the cup or the sad face is displayed on the matrix the player is not visible
  if (isPlayerVisible) {
    lc.setLed(0, playerY, playerX, true);
  } else {
    lc.setLed(0, playerY, playerX, false);
  }

  // while the player is moveing the led should not blink for a smoother game
  if (isPlayerMoving) {
    lc.setLed(0, playerY, playerX, true);
  }

  // display active bombs and make them blink
  for (int i = 0; i < numBombs; ++i) {
    if (bombs[i].isBlinking && currentTime - bombs[i].lastBlinkTime >= bombBlinkInterval) {
      bombLedState = !bombLedState;
      bombs[i].lastBlinkTime = currentTime;
    }

    lc.setLed(0, bombs[i].y, bombs[i].x, bombLedState);
  }
}

void gameOver() {
  isDead = 1;
  menuOption = 1;
  playerX = 0;
  playerY = 0;
  if(sound == 1){
    tone(pinBuzzer, 300, 500);
  }
  lcd.clear();
  lcd.print("Game Over!");
  isPlayerBlinking = false;
  resetMap();
  displaySadFace();
  delay(3000);
  setup();
}

void startGame() {
  isPlaying = 1;
  howToMove();
  bomb();
  playerBombBlink();

  // update elapsed time for level 3
  if (currentLevel == 3 && isLevelCompleted() == false && isDead == 0) {
    level3ElapsedTime = millis() - level3StartTime;

    // check if time limit exceeded
    if (level3ElapsedTime >= level3TimeLimit) {
      gameOver();
      return;
    }
  }
  
  if(isDead == 0){
    updateLCD();
  }
}
