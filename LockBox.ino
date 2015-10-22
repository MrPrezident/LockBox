// include the library code:
#include <LiquidCrystal.h>
#include <Servo.h>
#include <Bounce.h>
#include <EEPROM.h>

Servo myservo;  // create servo object to control a servo
                // a maximum of eight servo objects can be created

//int servo_pos = 0;    // variable to store the servo position


// initialize the library with the numbers of the interface pins
/*
 *The circuit:
 * LCD RS pin to digital pin 8
 * LCD Enable pin to digital pin 9
 * LCD D4 pin to digital pin 4
 * LCD D5 pin to digital pin 5
 * LCD D6 pin to digital pin 6
 * LCD D7 pin to digital pin 7
 * LCD R/W pin to ground
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)
*/
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
char msg[20] = "                   ";
int nQuestion = 0;
byte table;
int NUM_KEYS = 5;
int adc_key_val[5] = {30, 150, 360, 535, 760};
int digital_key_pin[5] = {10, 3, 11, 2, 12}; // right, up, down, left, select
int difficulty = 1;

int MAX_TABLE = 10;
int num_questions[10] = {11, 10, 11, 10, 9, 10, 10, 1, 10, 9};
int answer[10][11] = {{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                      {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}};

int correct_answer[10][11] = {{1, 2, 0, 17, 0, 0, 2, 0, 0, 0, 3},
                              {0, 2, 13, 1, 0, 3, 1, 1, 2, 0, -1},
                              {2, 1, 2, 3, 0, 1, 0, 3, 0, 0, 2},
                              {0, 0, 2, 2, 0, 3, 1, 2, 2, 0, -1},
                              {0, 2, 1, 1, 0, 1, 0, 4, 3, -1, -1},
                              {1, 0, 3, 0, 2, 1, 1, 1, 0, 4, -1},
                              {5, 0, 1, 0, 1, 2, 3, 0, 1, 2, -1},
                              {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
                              {2, 3, 2, 2, 1, 0, 3, 1, 0, 3, -1},
                              {4, 3, 2, 1, 2, 0, 1, 3, 0, -1, -1}};

int minimum[10][11] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
                       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};

int maximum[10][11] = {{2, 4, 2, 99, 3, 3, 2, 2, 3, 2, 3},
                       {3, 3, 99, 3, 3, 3, 3, 3, 3, 3, 1},
                       {3, 2, 3, 3, 2, 4, 2, 3, 2, 3, 3},
                       {3, 3, 2, 3, 2, 3, 3, 3, 3, 2, 1},
                       {3, 3, 99, 99, 2, 2, 3, 4, 3, 1, 1},
                       {3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 1},
                       {99, 3, 3, 3, 3, 3, 3, 3, 3, 3, 1},
                       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                       {3, 99, 2, 2, 3, 3, 3, 3, 3, 3, 1},
                       {4, 99, 2, 3, 3, 3, 3, 3, 3, 1, 1}};

int isLetter[10][11] = {{1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1},
                        {1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1},
                        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                        {1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1},
                        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                        {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                        {1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

Bounce bounceKey[5] = {
      Bounce(digital_key_pin[0], 50),
      Bounce(digital_key_pin[1], 50),
      Bounce(digital_key_pin[2], 50),
      Bounce(digital_key_pin[3], 50),
      Bounce(digital_key_pin[4], 50)
};

int HEART_CHAR = 1;
byte heart[8] = {
0b00000,
0b01010,
0b11111,
0b11111,
0b01110,
0b00100,
0b00000,
0b00000
};

int BLOCK_CHAR = 2;
byte block[8] = {
0b11111,
0b11111,
0b11111,
0b11111,
0b11111,
0b11111,
0b11111,
0b11111
};

void setup() {
    //Serial.begin(9600);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  for(int key=0; key<5; key++){
    int pin = digital_key_pin[key];
    pinMode(pin, INPUT);
    digitalWrite(pin, HIGH); // enable pull-up resistor
    //Serial.println(String(digitalRead(pin)));
  }

  pinMode(19, OUTPUT);
  digitalWrite(19, LOW);
  //delay(99999999);

  lcd.createChar(HEART_CHAR, heart);
  lcd.createChar(BLOCK_CHAR, block);

  table = EEPROM.read(0);
  difficulty = EEPROM.read(1);

  if((digitalRead(digital_key_pin[1])==LOW) && (digitalRead(digital_key_pin[2])==LOW)){
    update_table();
  }else if(digitalRead(digital_key_pin[4])==LOW){
    update_difficulty();
  }

  if(table == 0){
    welcome_test_mode();
    test_mode();
  }else{
    welcome();
  }
}

void test_mode() {
  while(1){
    int key, results;
    key = wait_for_key_digital();
    switch (key)
    {
      case 0:  // right
        lcd.setCursor(0,1);
        lcd.print("Right (open)    ");
        open_lock();
        break;
      case 1:  // up
        lcd.setCursor(0,1);
        lcd.print("Up              ");
        break;
      case 2:  // down
        lcd.setCursor(0,1);
        lcd.print("Down            ");
        break;
      case 3:  // left
        lcd.setCursor(0,1);
        lcd.print("Left (close)    ");
        close_lock();
        break;
      case 4:  // select
        lcd.setCursor(0,1);
        lcd.print("Select          ");
        break;
      default:
        // do nothing
        break;
    }
  }
}

void loop() {
  int key, results;
  key = wait_for_key_digital();
  switch (key)
  {
    case 0:  // right
      if(nQuestion >= num_questions[table-1]-1){ nQuestion = 0; } else { nQuestion++; }
      break;
    case 1:  // up
      if(answer[table-1][nQuestion]<maximum[table-1][nQuestion]){ answer[table-1][nQuestion]++; } else { answer[table-1][nQuestion] = minimum[table-1][nQuestion]; }
      break;
    case 2:  // down
      if(answer[table-1][nQuestion]>minimum[table-1][nQuestion]){ answer[table-1][nQuestion]--; } else { answer[table-1][nQuestion] = maximum[table-1][nQuestion]; }
      break;
    case 3:  // left
      if(nQuestion <= 0){ nQuestion = num_questions[table-1]-1; } else { nQuestion--; }
      break;
    case 4:  // select
      if(difficulty == 2){
        results = check_completion();
        if(results == num_questions[table-1]){
          results = check_results();
          if(results == num_questions[table-1]){
            print_win();
          } else {
            print_lose(results);
            blink_display(3,300);
            delay(2000);
            print_try_again();
            delay(2000);
            welcome();
          }
          return;
        }else{
          print_not_completed(results);
          delay(2000);
        }
      }else{
        results = check_results();
        if(results == num_questions[table-1]){
          print_win();
          return;
        } else {
          print_lose(results);
          delay(2000);
        }
      }
      break;
    default:
      return;
      // do nothing
  }

  print_question();
  print_answer();
}

void update_table() {
  int key, done;
//  byte table = EEPROM.read(0);

  done = 0;
  while(!done){
    lcd.setCursor(0,0);
    lcd.print("Table           ");
    lcd.setCursor(6,0);
    lcd.print(String(table));

    key = wait_for_key_digital();
    switch (key)
    {
      case 0:  // right
        if(table >= MAX_TABLE){ table = MAX_TABLE; } else { table++; }
        break;
      case 1:  // up
        if(table >= MAX_TABLE){ table = MAX_TABLE; } else { table++; }
        break;
      case 2:  // down
        if(table > 0){ table--; }
        break;
      case 3:  // left
        if(table > 0){ table--; }
        break;
      case 4:  // select
        EEPROM.write(0, table);
        done = 1;
        break;
      default:
        return;
        // do nothing
    }
  }
}

void update_difficulty() {
  int key, done;
//  byte table = EEPROM.read(0);

  done = 0;
  while(!done){
    lcd.setCursor(0,0);
    lcd.print("Difficulty:           ");
    lcd.setCursor(12,0);
    if(difficulty == 2){
      lcd.print("Hard        ");
    }else{
      lcd.print("Easy        ");

    }
    //lcd.print(String(difficulty));

    key = wait_for_key_digital();
    switch (key)
    {
      case 0:  // right
        //if(difficulty >= 2){ difficulty = 2; } else { difficulty++; }
        difficulty = 2;
        break;
      case 1:  // up
        //if(difficulty >= 2){ difficulty = 2; } else { difficulty++; }
        difficulty = 2;
        break;
      case 2:  // down
        //if(difficulty <= 1){ difficulty = 1; } else { difficulty--; }
        difficulty = 1;
        break;
      case 3:  // left
        //if(difficulty <= 1){ difficulty = 1; } else { difficulty--; }
        difficulty = 1;
        break;
      case 4:  // select
        EEPROM.write(1, difficulty);
        done = 1;
        break;
      default:
        return;
        // do nothing
    }
  }
}


int wait_for_key_digital(){
   delay(150);
  while(1){
    for(int i=0; i<NUM_KEYS; i++){
      bounceKey[i].update();
    }
    for(int key=0; key<NUM_KEYS; key++){
      if(bounceKey[key].fallingEdge()){
        return key;
      }else if(bounceKey[key].read() == LOW && bounceKey[key].duration() > 700){
        return key;
      }
    }

  }
}

// Wait for change in ADC and return key number
int wait_for_key_analog()
{
  int key;
  static int oldkey = -1;

  key = oldkey;
  while(1){
    int adc_key_in = analogRead(0); // read the value from the sensor
    key = get_key(adc_key_in);  // convert into key press

    if(key != oldkey)
    {
      delay(50); // wait for debounce time
      adc_key_in = analogRead(0);    // read the value from the sensor
      key = get_key(adc_key_in);		        // convert into key press
      if (key != oldkey)				
      {
        oldkey = key;
        if (key >= 0)
        {
          return key;
        }
      }
    }
  }
}

// Convert ADC value to key number
int get_key(unsigned int input)
{
  int k;

  for (k = 0; k < NUM_KEYS; k++)
  {
    if (input < adc_key_val[k])
    {
      //if(input==0){ Serial.println(adc_key_val[0]); }
      return k;
    }
  }

  if (k >= NUM_KEYS)
    k = -1;     // No valid key pressed

    //if(input==0){ Serial.println(k); }
  return k;
}

void print_question()
{
  lcd.setCursor(0,0);
  sprintf(msg, "Question %d    ", nQuestion+1);
  lcd.print(msg);
}

void print_answer()
{
  lcd.setCursor(0,1);
  if(answer[table-1][nQuestion]>=0){
    if(isLetter[table-1][nQuestion]){
      sprintf(msg, "Answer: %c       ", get_letter_for_int(answer[table-1][nQuestion]));
    } else {
      sprintf(msg, "Answer: %d       ", answer[table-1][nQuestion]);
    }
  } else {
    sprintf(msg, "Answer:         ");
  }
  lcd.print(msg);
}

void print_win()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Congratulations!");
  blink_display(3, 300);
  lcd.setCursor(0,0);
  lcd.print("Preparing unlock");
  progress_bar();
  open_lock();
  welcome();
}

void print_lose(int results)
{
  lcd.clear();
  lcd.setCursor(0,0);
  sprintf(msg, "%d out of %d      ", results, num_questions[table-1]);
  lcd.print(msg);
  lcd.setCursor(0,1);
  lcd.print("are correct.    ");
  //close_lock();
}

void print_not_completed(int results)
{
  lcd.clear();
  lcd.setCursor(0,0);
  sprintf(msg, "%d out of %d      ", results, num_questions[table-1]);
  lcd.print(msg);
  lcd.setCursor(0,1);
  lcd.print("answered.    ");
  //close_lock();
}

void print_try_again()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   Please Try       ");
  lcd.setCursor(0,1);
  lcd.print("     Again!         ");
}

void welcome()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("     N   J      ");
  lcd.setCursor(7,0);
  lcd.write(HEART_CHAR);
  lcd.setCursor(0,1);
  lcd.print("    Table       ");
  lcd.setCursor(10,1);
  lcd.print(String(table));

  wait_for_key_digital();
  close_lock();
  reset();
  print_question();
  print_answer();
}

void welcome_test_mode()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   Test Mode        ");
  //wait_for_key_digital();
  reset();
}

int delay_with_key_check(int seconds)
{
  for(int i = 0; i < seconds; i++)
  {
    int key = get_key(analogRead(0));
    if(key == -1){
      delay(1);
    } else {
      return 1;
    }
  }
  return 0;
}

char get_letter_for_int(int a)
{
  return (char)(a+65);
}

int check_results(){
  int return_value = 0;
  for(int i=0; i<num_questions[table-1]; i++){
    if(answer[table-1][i] == correct_answer[table-1][i]){
      return_value++;
    }
  }
  return return_value;
}

int check_completion(){
  int return_value = 0;
  for(int i=0; i<num_questions[table-1]; i++){
    if(answer[table-1][i] != -1){
      return_value++;
    }
  }
  return return_value;
}

void blink_display(int num, int time) {
  for(int i=0; i<num; i++){
    lcd.noDisplay();
    digitalWrite(19, HIGH);
    delay(time);
    lcd.display();
    digitalWrite(19, LOW);
    delay(time);
  }
}

void progress_bar(){
  lcd.setCursor(0,1);
  lcd.print("                ");
  for(int i=0; i<16; i++){
    lcd.setCursor(i,1);
    //lcd.print("* ");
    lcd.write(BLOCK_CHAR);
    delay(100);
  }
}

void close_lock(){
  //lcd.noDisplay();
  // attaches the servo on pin 13 to the servo object
  myservo.attach(13);
  myservo.write(0);
  delay(1000);
  myservo.detach();
  //lcd.display();
}

void open_lock(){
  //lcd.noDisplay();
  // attaches the servo on pin 13 to the servo object
  myservo.attach(13);
  myservo.write(180);
  delay(1000);
  myservo.detach();
  //lcd.display();
}

void reset(){
  nQuestion = 0;
  for(int i=0; i<num_questions[table-1]; i++){
    answer[table-1][i] = -1;
  }
}
