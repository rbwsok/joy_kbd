/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
// joystick
#define rows 3
#define cols 2
// keys number (from https://e-a.d-cd.net/f4f8accs-960.jpg)
const byte keys[rows][cols] = 
{
  {1,4},
  {2,5},
  {6,3}
};

// encoder direction
const byte encoderdirection[3][3] = // 0 - none, 1 - down, 2 - up
{
  {0, 1, 2},  // 0
  {2, 0, 1},  // 1
  {1, 2, 0}   // 2     
};

// 10 9 8 7 6 5

// arduino nano pins
const byte rowPins[rows] = {10, 9, 8}; // rows (yellow, green, blue)
const byte colPins[cols] = {7, 5}; // columns (black, red)
#define encoderPin 6 // encoder (brown)
byte encodervalue;

// keys masks
#define MaskMediaButton      1 
#define MaskRadioButton      2
#define MaskVolumeUpButton   4
#define MaskVolumeDownButton 8
#define MaskOkButton         16
#define MaskMuteButton       32 
#define MaskScrollDown       64 
#define MaskScrollUp         128 

unsigned long starttime;
bool workkbd;

/////////////////////////////////////////////////////////////////

void setup() 
{  
  int i;
  // rows - output
  for (i = 0; i < rows; ++i)
  {
    pinMode(rowPins[i], OUTPUT);
  }
  
  // columns - input
  for (i = 0; i < cols; ++i)
  {
    pinMode(colPins[i], INPUT);
    digitalWrite(colPins[i], HIGH); // pullup resistor on
  }

  // encoder
  pinMode(encoderPin, INPUT);
  digitalWrite(encoderPin, HIGH); // pullup resistor on
   
  encodervalue = 0;
  
  starttime = millis();
  workkbd = false;
}

// joystick scan
byte scan(void)
{
  int i, j;
  byte code = 0;
          
  for (i = 0; i < rows; ++i)
  {
    digitalWrite(rowPins[i], LOW);

    // buttons
    for (j = 0; j < cols; ++j)
    {
      if(digitalRead(colPins[j]) == false)
      {
        code |= 1 << (keys[i][j] - 1);
      }         
    }
       
    // encoder
    if (digitalRead(encoderPin) == false)
    {
      int oldencodervalue = encodervalue;
      encodervalue = i;
                        
      switch (encoderdirection[oldencodervalue][encodervalue])
      {
        case 1:  // вниз
          code |= 64;
          break;
        case 2:  // вверх
          code |= 128;
          break;
      }
    }
        
    digitalWrite(rowPins[i], HIGH);    
  }
  
  return code;
}

#define DELAY_COMMAND 200

// loop cycle
void loop(void) 
{
  byte code; 
    
  code = scan();
  
  if (workkbd == false)
  {
    if (millis() - starttime > 3000)
      workkbd = true;
    
    return;
  }

  if (code & MaskMediaButton)
  {
    Remote.rewind();
    Remote.clear();
    delay(DELAY_COMMAND);
  }
    
  if (code & MaskRadioButton)
  {
    Remote.forward();
    Remote.clear();
    delay(DELAY_COMMAND);
  }
    
  if (code & MaskVolumeUpButton)
  {
    Remote.next();
    Remote.clear();
    delay(DELAY_COMMAND);
  }

  if (code & MaskVolumeDownButton)
  {
    Remote.previous();
    Remote.clear();
    delay(DELAY_COMMAND);
  }

  if (code & MaskOkButton)
  {
    Remote.pause();
    Remote.clear();
    delay(DELAY_COMMAND);
  }
    
  if (code & MaskMuteButton)  
  {
    Remote.mute();
    Remote.clear();
    delay(DELAY_COMMAND);
  }

  if (code & MaskScrollDown)
  {
    Remote.decrease();
    Remote.clear();    
  }

  if (code & MaskScrollUp)
  {
    Remote.increase();
    Remote.clear();
  }    
}


