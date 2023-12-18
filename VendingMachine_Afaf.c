/* ****************************************
VendingMachine.c
A program for controlling a drink vending machine
Written by Dr T Rachidi
Last updated Nov 19, 2022
 ******************************************* */

#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <windowsx.h>
#include <mmsystem.h>

// number of drinks supported by the machine
#define DRINKS  3

// codes for drinks
#define COFFEE  0
#define TEA     1
#define HOTCOCO 2


// initial dozes of ingredients
#define INITIAL_COFFEE_DOZES  5
#define INITIAL_TEA_DOZES  5
#define INITIAL_HOTCOCO_DOZES  5


int available_quantities[DRINKS]; // tracks the available ingredients


// names of drinks
#define MAXNAME 15
char drinks[DRINKS][MAXNAME] = {"Coffee", "Tea", "Hot coco"};


int water ;             //tracks water in the machine in ml
int water_ml[DRINKS];   // water needed per drink

// initial water in the machine
#define INITIAL_WATER_MLs  600

// quantities of water per drink type
#define COFFEE_ML 30
#define TEA_ML 100
#define HOTCOCO_ML 100


int price[DRINKS];  // prices for drinks

// prices for drinks
#define COFFEE_PRICE  4
#define TEA_PRICE     5
#define HOTCOCO_PRICE 6




int suggar;// tracks suggar in the machine in mg
#define INITIAL_SUGGAR_MGs  5
#define SL_STEP 2


int canserve=1;// flag. will be zero when the machine cannot serve i.e., no water or no ingredients

// number of coins supported 1, 2, and 5dh
#define MAXCOINS        3

// codes for coins to be used with  available_coins[]
#define ONE_DH_COIN     2
#define TWO_DH_COIN     1
#define FIVE_DH_COIN    0

// actual value of each coin
#define VAL_ONE_DH_COIN 1
#define VAL_TWO_DH_COIN 2
#define VAL_FIVE_DH_COIN 5

int available_coins[MAXCOINS]; // tracks available coins in the machine


// initial coins put in the machine
#define INITIAL_ONE_DH_COINS    5
#define INITIAL_TWO_DH_COINS    5
#define INITIAL_FIVE_DH_COINS   0

int get_user_choice(void );                  // returns a valid user choice or -1 . Only drinks for which there are ingredients should showup on the menu and returned
void serve_drink(int choice, int suggarmg);// serves a drink and updates available quantities of ingredients
void print_available_quantities(void); // prints to the standard ouput all available quantities including coins
void init_vending_machine(void);      // initializes the machine with all initial quantities including coins
int get_payment(int choice);         // takes the user choice asks for payment in valid coins and return the change to be given back to the custommer. Updates avaialble coins
void deliver_change(int change);    // delivers the change in available coins. Updates available coins
void return_coin(int coin );       // reurns back the invalid coin to the custommer
void start_vending_machine(void); // welcome message and sound
void stop_vending_machine(void); // prints machine out of order
int get_suggar_level(void);     // returns user sugar  preference in mg


int main(void)
{
  int uchoice,uchange,usuggar; 
  
    init_vending_machine();
    start_vending_machine();

    do {

        print_available_quantities();  
        
        uchoice=get_user_choice ();
        if(uchoice== -1)
            continue;

        usuggar=get_suggar_level();
        if(usuggar==3){
            printf("Not enough suggar\n");
            continue;
        }

        uchange=get_payment(uchoice);
        if(uchange)
            deliver_change(uchange);

        serve_drink(uchoice,usuggar);
        
    }while(canserve==1);
    stop_vending_machine();
  
  
  return 0;
}




 // initializes the machine with all initial quantities including coins
void init_vending_machine(void){
available_quantities[0]=INITIAL_COFFEE_DOZES;
available_quantities[1]=INITIAL_TEA_DOZES;
available_quantities[2]=INITIAL_HOTCOCO_DOZES;


available_coins[2]=INITIAL_ONE_DH_COINS;
available_coins[1]=INITIAL_TWO_DH_COINS;
available_coins[0]=INITIAL_FIVE_DH_COINS;

suggar=INITIAL_SUGGAR_MGs;

water=INITIAL_WATER_MLs;
water_ml[COFFEE]=COFFEE_ML;
water_ml[TEA]=TEA_ML;
water_ml[HOTCOCO]=HOTCOCO_ML;
}  




void start_vending_machine(){
  printf("welcome to the vending machine.\n");
  //PlaySound("C:\\Users\\DELL\\OneDrive\\Bureau\\welcome.wav",NULL,SND_SYNC | SND_LOOP | SND_FILENAME);
   
}   




void print_available_quantities(){
int i;
printf("Available quantities:\n");
  for(i=0;i<DRINKS;i++){
    printf("%s=%d\n",drinks[i],available_quantities[i]);
  }

printf("Available coins:\n1dh=%d\n2dhs=%d\n5dhs=%d\n",available_coins[2],available_coins[1],available_coins[0]);
printf("water=%d\n",water);
printf("sugar=%d\n",suggar);
}




int get_user_choice(){ //!!!
int c;
int i;
 
 for(i=0;i<DRINKS;i++){

  if(available_quantities[i]>0){
    printf("Type %d for %s.\n",i,drinks[i]);
    
  }

 }
  scanf("%d",&c);

  if(c!=0 && c!=1 && c!=2)
     c=-1;

return(c);
}




int get_suggar_level(){
 int s;
 int t=suggar;

 do{
 printf("How do you want your drink?\nChoose 0 for a drink with no sugar.\n1 for a medium sugar level.\n2 for a high sugar level.\n");
 scanf("%d",&s);
 }while(s!=0 && s!=1 && s!=2);

 switch(s){
      case 1:
       t=t-SL_STEP;
       break;
      case 2:
       t=t-2*SL_STEP;
       break;
  }
  
  if(t<0)
    s=3;
  else
    suggar=t;

return(s);
}



int get_payment(int c){
int pay, t, i, a, sum;
  sum=0;

    switch(c){

      case 0:
        printf("Please enter %d dhs to pay:\n",COFFEE_PRICE);      
          for(i=0; i<COFFEE_PRICE;i++){
             scanf("%d",&pay);

               while(pay!=2 && pay!=1 && pay!=5){
                 return_coin(pay);
                 printf("Only 1dh, 2dhs and 5 dhs are accepted. Please reenter the right type of coins>\n");
                 scanf("%d",&pay);
               } 

              if(pay==1)
                available_coins[2]=available_coins[2]+1;
              if(pay==2)
                available_coins[1]=available_coins[1]+1;
              if(pay==5)
                available_coins[0]=available_coins[0]+1;

              sum=pay+sum;
              t=COFFEE_PRICE-sum;
            
              if (t>0)
                printf("Please enter the remaining %d dhs:\n",t);
              else
               break;
          }

      break;
    
      case 1:
        printf("Please enter %d dhs to pay:\n",TEA_PRICE);
          for(i=0; i<TEA_PRICE;i++){
             scanf("%d",&pay);

               while(pay!=2 && pay!=1 && pay!=5){
                 return_coin(pay);
                 printf("Only 1dh, 2dhs,and 5 dhs are accepted. Please reenter the right type of coins>\n");
                 scanf("%d",&pay);
               } 

               if(pay==1)
                 available_coins[2]=available_coins[2]+1;
               if(pay==2)
                 available_coins[1]=available_coins[1]+1;
               if(pay==5)
                 available_coins[0]=available_coins[0]+1;

              sum=pay+sum;
              t=TEA_PRICE-sum;

              if (t>0)
                printf("Please enter the remaining %d dhs:\n",t);
              else
               break;
          }
         
      break;


      case 2:
        printf("Please enter %d dhs to pay:\n",HOTCOCO_PRICE);

          for(i=0; i<HOTCOCO_PRICE;i++){
             scanf("%d",&pay);

               while(pay!=2 && pay!=1 && pay!=5){
                 return_coin(pay);
                 printf("Please reenter the right type of coins>\n");
                 scanf("%d",&pay);
               } 

               if(pay==1)
                 available_coins[2]=available_coins[2]+1;
               if(pay==2)
                 available_coins[1]=available_coins[1]+1;
               if(pay==5)
                 available_coins[0]=available_coins[0]+1;

               sum=pay+sum;
               t=HOTCOCO_PRICE-sum;

               if (t>0)
                 printf("Please enter the remaining %d dhs:\n",t);
               else
                break;
          }

      break;
    } 
 
 return(abs(t));
}


    
void return_coin(int coin){
printf("Only 1dh, 2dhs and 5 dhs are accepted. Please collect your entered coin before reentering a valid coin.");
//PlaySound("C:\\Users\\DELL\\OneDrive\\Bureau\\change.wav",NULL,SND_SYNC | SND_LOOP | SND_FILENAME);
}      




void deliver_change(int change){
  int r;
  int r2;
    printf("please get your change: %d dhs\n",change);
      if(change>=5){
          r=change%5;
            if(r=0){
               change=change/5;
               printf("%d piece of 5dhs.\n",change);
               available_coins[0]=available_coins[0]-change;
            }else if(r>=2){
               r2=r%2;
                 if(r2=0){
                    change=r/2;
                    printf("1 piece of 5dhs and %d pieces of 2dhs.\n",change);
                    available_coins[0]=available_coins[0]-1;
                    available_coins[1]=available_coins[1]-change;
                 }else{
                    printf("1 piece of 5dhs and 1 piece of 2 dhs and 1 piece of 1dh.\n");
                    available_coins[0]=available_coins[0]-1;
                    available_coins[1]=available_coins[1]-1;
                    available_coins[2]=available_coins[2]-1;
                 }
            }else{
               printf("1 piece of 5 dhs and a piece of 1 dhs.\n");
               available_coins[0]=available_coins[0]-1;
               available_coins[2]=available_coins[2]-1;
            }

      }else if(change>=2){
          r=change%2;

            if(r==0){
               change=change/2;
               printf("%d pieces of 2dhs.\n",change);
               available_coins[1]=available_coins[1]-change;
            }else{
               printf("1 piece of 2dhs and 1 piece of 1dh.\n");
               available_coins[1]=available_coins[1]-1;
               available_coins[2]=available_coins[2]-1;
            }
      }

      //PlaySound("C:\\Users\\DELL\\OneDrive\\Bureau\\change.wav",NULL,SND_SYNC | SND_LOOP | SND_FILENAME);
      
    

}   



void serve_drink(int c, int s){ //c for choice and s for suggarmg
  printf("your beverage is in the making. Thank you for your patience.\n");
  //PlaySound("C:\\Users\\DELL\\OneDrive\\Bureau\\preparation.wav",NULL,SND_SYNC | SND_LOOP | SND_FILENAME);
  printf("Your beverage is ready.\nPlease collect your beverage.\n");

  switch(c){
   
      case 0:
        available_quantities[0]=available_quantities[0]-1;
        water=water-COFFEE_ML;
       break;

      case 1:
         available_quantities[1]=available_quantities[1]-1;
         water=water-TEA_ML;
       break;

      case 2:
         available_quantities[2]=available_quantities[2]-1;
         water=water-HOTCOCO_ML;
       break;
  }

  if(water==0 || (available_quantities[0]==0 && available_quantities[1]==0 && available_quantities[2]==0 ))
     canserve=0;

}

void stop_vending_machine(){
  //PlaySound("C:\\Users\\DELL\\OneDrive\\Bureau\\ok.wav",NULL,SND_SYNC | SND_LOOP | SND_FILENAME);
  printf("Machine out of order.\n");
}
