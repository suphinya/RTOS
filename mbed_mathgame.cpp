#include "SPI.h"
#include "Adafruit_GFX.h"     
#include "Adafruit_ILI9341.h"
#include "mbed.h"
using namespace mbed; // for DigitalOut, Ticker
using namespace rtos; // for ThisThread
using namespace std::literals::chrono_literals; // for ms

//----------------- ILI9341 lcd connection ---------------------------------------
#define TFT_DC 9
#define TFT_CS 13
#define TFT_MOSI 11
#define TFT_CLK 10
#define TFT_RST -1
#define TFT_MISO 12
// connect lcd
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

// button to return to home (red button)
DigitalIn pb1(p2);
// button for mode and select answer
DigitalIn btn1( p16 );  // button 1
DigitalIn btn2( p17 );  // button 2
DigitalIn btn3( p18 );  // button 3
DigitalIn btn4( p19 );  // button 4

int score = 0;      
float answer = 0 ;
float choice1 = 0;  // save value choice 1
float choice2 = 0;  // save value choice 2
float choice3 = 0;  // save value choice 3
float choice4 = 0;  // save value choice 4

// mode button state
bool plus_mode = 0;
bool minus_mode = 0;
bool multiple_mode = 0;
bool divide_mode = 0;

float ch1 = 0;  // for random other value choice 1
float ch2 = 0;  // for random other value choice 2
float ch3 = 0;  // for random other value choice 3

Thread t1( osPriorityHigh );    // thread 1 (function start game)
Thread t2( osPriorityNormal );  // thread 2 (function play game)
EventFlags flags; // flag for switch thread ( inter-synchronization)

void home_page();
void show_problem();
void clear_problem();
void back_to_home();
void random_problem();
void start_game();
void play_game();

void home_page(){ // function show home page
  // reset all mode value for restart
  plus_mode = 0;
  minus_mode = 0;
  multiple_mode = 0;
  divide_mode = 0;
  // show home on display 
  tft.begin();
  tft.setCursor(55, 80);
  tft.setTextColor(ILI9341_RED);
  tft.setTextSize(3.8);
  tft.println("MathGame");
  tft.setCursor(60, 160);
  tft.setTextColor(ILI9341_GREEN);
  tft.setTextSize(2);
  tft.println("Select mode");
  tft.setCursor(10, 190);
  tft.println("+(1) -(2) x(3) /(4)");
  tft.setCursor(70, 230);
  tft.setTextColor(ILI9341_RED);
  tft.println("to start!");
}

// function show problem 
void show_problem(int x , int y ,String m , float choice1 ,float choice2,float choice3,float choice4){
  tft.begin();
  tft.setTextColor(ILI9341_RED);
  tft.setCursor(110, 20);
  tft.setTextSize(3);
  tft.println("Score " + String(score));
  tft.setCursor(20, 80);
  tft.setTextSize(3);
  tft.println(String(x)+" "+String(m)+" "+String(y)+" = ?");
  tft.setCursor(70, 150);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.println("a.) "+String(choice1));
  tft.setCursor(70, 180);
  tft.println("b.) "+String(choice2));
  tft.setCursor(70, 210);
  tft.println("c.) "+String(choice3));
  tft.setCursor(70, 240);
  tft.println("d.) "+String(choice4));
  delay(1000);
}

// function clear problem 
void clear_problem(int x , int y ,String mode , float choice1 ,float choice2,float choice3,float choice4){
  tft.begin();
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(110, 20);
  tft.setTextSize(3);
  tft.println("Score " + String(score-1));
  tft.setCursor(20, 80);
  tft.setTextSize(3);
  tft.println(String(x)+" "+String(mode)+" "+String(y)+" = ?");
  tft.setCursor(70, 150);
  tft.setTextSize(2);
  tft.println("a.) "+String(choice1));
  tft.setCursor(70, 180);
  tft.println("b.) "+String(choice2));
  tft.setCursor(70, 210);
  tft.println("c.) "+String(choice3));  
  tft.setCursor(70, 240);
  tft.println("d.) "+String(choice4));
  // reset value all choice
  choice1 = 0;
  choice2 = 0;
  choice3 = 0;
  choice4 = 0;

}

// function back ho home page when push red button
void back_to_home(){
  // check button to homepage
  if (pb1 == 1){
    tft.fillScreen(ILI9341_BLACK);
    home_page();
  }
}

// function calculate question, answer, choice 
void random_problem(String parameter){
  String mode = parameter ;
  //check mode 
  if (mode == "+" || mode == "-" || mode == "*" || mode == "/") {
    // random number for question 
    float x = random(1,100);
    float y = random(1,100);
    // calculate answer of question with each mode
    if (mode == "+"){
      answer = x + y ;
    }
    else if (mode == "-") {
      answer = x - y ;
    }
    else if (mode == "*") {
      answer = x * y ;
    }
    else if (mode == "/") {
      y = random(1,20);
      answer = x / y ;
    }
    // random other choice value
    ch1 = answer + random(1,10);
    ch2 = answer - random(1,10);
    ch3 = answer + random(1,10) - random(10,15) ;

    // list for random in choice -----------------
    float mylist[4] = {answer,ch1,ch2,ch3};
    //สุ่มเลข index list
    //เอาไปใส่ในช้อย
    for (int i = 0; i < 4; i++){
      int numindex = 0 ;
      numindex = random(0,4);
      if (i==0){
        choice1 = mylist[numindex] ;
      }
      else if (i == 1){
        choice2 = mylist[numindex];
        while(choice2 == choice1) {
          numindex = random(1,4);
          choice2 = mylist[numindex];
        }
      }
      else if (i == 2){
        choice3 = mylist[numindex];
        for(int k = 0; k < 4; k++){
          if (choice3 == choice1 || choice3 == choice2){
            choice3 = mylist[k];
          }
          else{
            k = 4 ;
          }
        }
      }
      else if (i == 3){
        choice4 = mylist[numindex];
        for(int k = 0; k < 4; k++){
          if (choice4 == choice1 || choice4 == choice2 || choice4 == choice3){
            choice4 = mylist[k];
          }
          else{
            k = 4 ;
          }
        }
      }
    }

    // show problem and random choice
    show_problem(x,y,mode,choice1,choice2,choice3,choice4);

    // polling check answer
    while (1) {
      back_to_home(); // case push red button

      // check choice and answer is equal
      // if answer = choice  ==> score +1 and go to next answer 
      // if answer != choice ==> back to home and gameover 
      if ( btn1.read()==1 ){
        if ( choice1 != answer ) {
          clear_problem(x,y,mode,choice1,choice2,choice3,choice4);
          home_page();
          score = 0;
        } else {
          score += 1;
        }
        break;
      } else if ( btn2.read()==1 ) {
        if ( choice2 != answer ) {
          clear_problem(x,y,mode,choice1,choice2,choice3,choice4);
          home_page();
          score = 0;
        } else {
          score += 1;
        }
        break;
      } else if ( btn3.read()==1 ) {
        if ( choice3 != answer ) {
          clear_problem(x,y,mode,choice1,choice2,choice3,choice4);
          home_page();
          score = 0;
        } else {
          score += 1;
        }
        break;
      } else if ( btn4.read()==1 ) {
        if ( choice4 != answer ) {
          clear_problem(x,y,mode,choice1,choice2,choice3,choice4);
          home_page();
          score = 0;
        } else {
          score += 1;
        }
        break;
      }
      ThisThread::sleep_for( 1000ms );
    }
    // test
    ThisThread::sleep_for( 1000ms );
    
    if (score != 0 ){
      clear_problem(x,y,mode,choice1,choice2,choice3,choice4);
    }

    
  }
}

void start_game(){
  // polling check if button 1,2,3,4 (mode) is push
  while( 1 ){ 
    flags.wait_any( 1, osWaitForever ); // wait for flag to run this thread
    //push button green to start game
    if( btn1.read()==1 || btn2.read()==1 || btn3.read()==1 || btn4.read()==1 ) { 
      if ( btn1.read()==1 ) {
        plus_mode = 1; // mode + 
      } else if ( btn2.read()==1 ) {
        minus_mode = 1; // mode -
      } else if ( btn3.read()==1 ) { 
        multiple_mode = 1; // mode * 
      } else if ( btn4.read()==1 ) {
        divide_mode = 1; // mode /
      }
      tft.fillScreen(ILI9341_BLACK); // clear screen
      score = 0;
    }
    ThisThread::sleep_for( 1000ms );
  }
  ThisThread::sleep_for( 1000ms );
}

void play_game(){
  while(1){
    // check mode for send to function random_problem
    //generate problem
    if ( plus_mode == 1 ) {
      random_problem("+");
    } else if ( minus_mode == 1 ) {
      random_problem("-");
    } else if ( multiple_mode == 1 ) {
      random_problem("*");
    } else if ( divide_mode == 1 ) {
      random_problem("/");
    }
    flags.set(1); // switch thread to thread 1
    ThisThread::sleep_for( 1000ms );
  }
  ThisThread::sleep_for( 1000ms );
}

int main(){
  // mode for push button pulldown
  pb1.mode(PullDown);
  btn1.mode( PullDown );
  btn2.mode( PullDown );
  btn3.mode( PullDown );
  btn4.mode( PullDown );

  home_page();

  // start thread
  t1.start(start_game);
  t2.start(play_game); 
  osDelay( osWaitForever );
   
}
