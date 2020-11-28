#include <LiquidCrystal.h>
#include <Keypad.h>

const int ROW_NUM = 4; //four rows
const int COLUMN_NUM = 4; //four columns
int count=0;
int balance[3]={5000,5000,5000};

const char tags[3][12]={"1234567890ab","1234567890cd","1234567890ef"};
char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};
char input[13];

byte pin_rows[ROW_NUM] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

#define rs A0
#define en A1
#define d4 A2
#define d5 A3
#define d6 A4
#define d7 A5

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

class Person{
  public:
    char pname[12];
    int balance;
    char tag[13];
    Person(){}
    Person(char nm[12],int blc ,char t[13]){
      strcpy(pname,nm);
      balance=blc;
      strcpy(tag,t);
    }
};

Person pObj[3]={Person("A",5000,"1234567890ab"),Person("B",5000,"1234567890cd"),Person("C",5000,"1234567890ef")};

void setup() {
  lcd.begin(16,2);
  Serial.begin(9600);
}

void loop() {
  int result=-1;
  int index=-1;
  //lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("   WELCOME TO");
  lcd.setCursor(0,1);
  lcd.print("   PETROL PUMP");

  if(Serial.available())
  {
    count = 0;
    while(Serial.available() && count < 12)
    {
      input[count] = Serial.read(); // Read 1 Byte of data and store it in the input[] variable
      count++; // increment counter
      delay(2);
    }
    lcd.clear();
//    lcd.print(pObj[0].tag);
//    delay(1000);
    for(int i=0;i<3;i++){
      result=strcmp(input,pObj[i].tag);
      if(result==0){
        index=i;
        break;
      }
    }
    if(result==0){
      int amount=0;
      int i=1;
      lcd.print("Enter Amount:");
      lcd.setCursor(0,1);
      lcd.print("Rs.");
      while(1){
        char key = keypad.getKey();
        if(key){
          if(key=='A')
          break;
          lcd.print(key);
          int digit=key-'0';
          amount=amount*i;
          amount=amount+digit;
          i=i*10;
        }
        delay(50);
      }
      lcd.clear();
      if(pObj[index].balance<amount){
        lcd.print("  Insufficient");
        lcd.setCursor(0,1);
        lcd.print("    Balance");
      }else{
        pObj[index].balance=pObj[index].balance-amount;
        lcd.print("Balance:");
        lcd.setCursor(0,1);
        lcd.print("Rs.");
        lcd.print(pObj[index].balance);
      }
      delay(1000);
    }
    else
    lcd.print("User not found");
    delay(2000);
  }
}
