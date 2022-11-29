#define PinStartowy 0    //moduł startowy
#define PWM_MAX 254//254        //Max predkosc
#define MAX_Odleglosc 80 //odleglosc czunika max
#define MIN_Odleglosc 1023  //odleglosc czujnika min

//Piny do ustawienia
#define LSilnikPrzod 9
#define LSilnikTyl 6
#define PSilnikPrzod 10
#define PSilnikTyl 11

#define PcBialyPrzod 12 //2
#define LcBialyPrzod 2 //1
#define PcBialyTyl 7 //4
#define LcBialyTyl 8 //3

#define POko A0
#define LOko A1



int protokolWalka =44;         //Skupia robota na nacieraniu na wroga
int protokolSzukaj=33;         //Skupia robota na sprawdzaniu czunikow bialej linii, oczekujac wykrycia wroga
volatile int coRobic=protokolSzukaj;    //
int ostatniaZnanaPozycja=100;
int ostatniaPrednoscLSilnika=0;
int ostatniaPrednoscPSilnika=0;

void setup() {

    Serial.begin(9600); //wylaczyc do walki bo zbagujesz modul
    //Te czujniki modyfikujemy
    pinMode(LSilnikPrzod, OUTPUT);
    pinMode(LSilnikTyl,   OUTPUT);
    pinMode(PSilnikPrzod, OUTPUT);
    pinMode(PSilnikTyl,   OUTPUT);

    //Te czujniki czytamy
    pinMode(PcBialyPrzod,   INPUT);
    pinMode(LcBialyPrzod,   INPUT);
    pinMode(PcBialyTyl,     INPUT);
    pinMode(LcBialyTyl,     INPUT);

    pinMode(LOko, INPUT_PULLUP);
    pinMode(POko, INPUT_PULLUP);


    modulStartowy();

}

void loop() {
    modulTestowySilnikow();
    modulTestowyKonfiguracji();
    /*

     if(coRobic==protokolSzukaj){               //sprawdza czujniki bieli i szuka wroga
       alarmBialychCzujnikow(sprawdzajCzujnikiBiale());
       wykrywanieWroga();
     }
     if(coRobic==protokolWalka){         //Przypadek starcia w szarej strefie na odległośc 5cm
         naprowadzanieNaWroga();
         delay(300);
       }
     }*/


}



void modulTestowySilnikow(){
    jazdaLewySilnik(90);
    jazdaPrawySilnik(90);
    delay(1000);
    jazdaLewySilnik(60);
    jazdaPrawySilnik(10);
    delay(2000);
    jazdaLewySilnik(-90);
    jazdaPrawySilnik(-90);
    delay(1000);
    jazdaLewySilnik(0);
    jazdaPrawySilnik(0);
    delay(500);
}

void modulTestowyKonfiguracji(){
    Serial.print("Prawy: ");
    Serial.print(analogRead(POko));
    Serial.print("  ");
    Serial.print("Lewy: ");
    Serial.print(analogRead(LOko));
    Serial.print("  ");
    Serial.print(sprawdzajCzujnikiBiale());
    Serial.println("");
    delay(100);
    //jazdaLewySilnik(300);
    //jazdaPrawySilnik(300);
}





/******************************************************************
 * void sprawdzajCzujnikiBiale()
 *  Parametry:
 *      Brak
 *  Opis:
 *       Sprawdza stan na czujnikach bialej linii
 *
 ***************************************************************/
int sprawdzajCzujnikiBiale(){
    if (digitalRead(LcBialyPrzod) == HIGH) return 1;
    if (digitalRead(PcBialyPrzod) == HIGH) return 2;
    if (digitalRead(LcBialyTyl)   == HIGH) return 3;
    if (digitalRead(PcBialyTyl)   == HIGH) return 4;
    else                                   return 0;
}



/******************************************************************
 * alarmBialychCzujnikow
 *  Parametry:
 *      int numerCzujnika - reprezentuje numer czujnika na ktorym
 *                          wystapil wzrost napiecia
 *  Opis:
 *       W zaleznosci od rodzaju problemu
 *       case 1: Silniki rozpoczna cofanie
 *       case 2: Silniki zaczną skręcać w Lewo
 *       case 3: Silniki zaczną sręcać w Prawo
 *       case 4: Silniki zaczną jechać do przodu
 *
 ***************************************************************/
void alarmBialychCzujnikow(int numerCzujnika){

    //Serial.print(numerCzujnika);
    switch(numerCzujnika){
        case 1: // LcBialyPrzod
            jazdaLewySilnik(-90);
            jazdaPrawySilnik(-90);
            delay(500);
            jazdaLewySilnik(20);
            jazdaPrawySilnik(90);
            break;

        case 2: //PcBialyPrzod
            jazdaLewySilnik(40);
            jazdaPrawySilnik(90);
            delay(500);
            jazdaLewySilnik(30);
            jazdaPrawySilnik(90);
            break;

        case 3: //LcBialyTyl
            jazdaLewySilnik(90);
            jazdaPrawySilnik(40);
            delay(500);
            jazdaLewySilnik(90);
            jazdaPrawySilnik(30);
            break;

        case 4: //PcBialyTyl
            jazdaLewySilnik(90);
            jazdaPrawySilnik(90);
            delay(500);
            jazdaLewySilnik(30);
            jazdaPrawySilnik(90);
            break;

        default:
            break;
    }
}



/******************************************************************
 * void jazdaLewySilnik(int predkosc)
 *
 *  Parametry:
 *      int predkosc - ujemna bądź dodatnia wartość procentowa
 *
 *  Opis:
 *       Służy do sterowania silnikiem lewym w przod i tyl
 *       przez podanie wartosci dodatniej - przod
 *       lub ujemnej - tyl w skali od 0-100% predkosci
 *       maksymalnej
 ***************************************************************/
void jazdaLewySilnik(int predkosc){
    if(predkosc!=ostatniaPrednoscLSilnika){
        if(predkosc>0){
            analogWrite(LSilnikPrzod,0);
            int wartoscPrzetworzona=predkosc;
            wartoscPrzetworzona=abs(wartoscPrzetworzona);
            wartoscPrzetworzona=map(wartoscPrzetworzona, 0, 100, 1, PWM_MAX);
            analogWrite(LSilnikPrzod,wartoscPrzetworzona);
        }
        if(predkosc<0){
            analogWrite(LSilnikTyl,0);
            int wartoscPrzetworzona=predkosc;
            wartoscPrzetworzona=abs(wartoscPrzetworzona);
            wartoscPrzetworzona=map(wartoscPrzetworzona, 0, 100, 1, PWM_MAX);
            analogWrite(LSilnikTyl,wartoscPrzetworzona);
        }
        if(predkosc==0){
            analogWrite(LSilnikTyl,0);
            analogWrite(LSilnikPrzod,0);
        }
        ostatniaPrednoscLSilnika=predkosc;
    }
}



/******************************************************************
 * void jazdaPrawySilnik(int predkosc)
 *
 *  Parametry:
 *      int predkosc - ujemna bądź dodatnia wartość procentowa
 *
 *  Opis:
 *       Służy do sterowania silnikiem prawym w przod i tyl
 *       przez podanie wartosci dodatniej - przod
 *       lub ujemnej - tyl w skali od 0-100% predkosci
 *       maksymalnej
 ***************************************************************/
void jazdaPrawySilnik(int predkosc){
    if(predkosc!=ostatniaPrednoscPSilnika){
        if(predkosc>0){
            analogWrite(PSilnikPrzod,0);
            int wartoscPrzetworzona=predkosc;
            wartoscPrzetworzona=abs(wartoscPrzetworzona);
            wartoscPrzetworzona=map(wartoscPrzetworzona, 0, 100, 1, PWM_MAX);
            analogWrite(PSilnikPrzod,wartoscPrzetworzona);
        }
        if(predkosc<0){
            analogWrite(PSilnikTyl,0);
            int wartoscPrzetworzona=predkosc;
            wartoscPrzetworzona=abs(wartoscPrzetworzona);
            wartoscPrzetworzona=map(wartoscPrzetworzona, 0, 100, 1, PWM_MAX);
            analogWrite(PSilnikTyl,wartoscPrzetworzona);
        }
        if(predkosc==0){
            analogWrite(PSilnikTyl,0);
            analogWrite(PSilnikPrzod,0);
        }
        ostatniaPrednoscPSilnika=predkosc;
    }
}




int rozpoznajOdleglosc(int pobraneDane){
    return map(pobraneDane,700,70,MIN_Odleglosc,MAX_Odleglosc); //Ile oczy moga max V wysylac?
}

void wykrywanieWroga(){
    if(rozpoznajOdleglosc(analogRead(POko))>MAX_Odleglosc ||rozpoznajOdleglosc(analogRead(POko))<(MIN_Odleglosc+1))
        coRobic=protokolWalka;
}


/******************************************************************
 * naprowadzanieNaWroga
 *
 *  Parametry:
 *      Brak
 *
 *  Wyjście:
 *      Przestawia zmienna globalna coRobic na opcje protokolWalka
 *
 *  Opis:
 *       Jest to zarazem przerwanie i normalna funkcja reagowania
 *       po znaleznieniu wroga.
 *       Dwa przypadki z podziałami na dwa kolejne:
 *          Prawe oko zaczyna widziec:
 *                Lewe widzi:     pełna na przód
 *                Lewe nie widzi: naprowadzanie na wroga
 *                                przez skręcanie w prawo
 *          Prawe oko przestało widziec:
 *                Lewe widzi:     korekcja toru ruchu przez
 *                                skrecanie w prawo
 *                Lewe nie widzi: korekcja ruchu przez skrecanie
 *                                w lewo
 ***************************************************************/
void naprowadzanieNaWroga(){

    ostatniaZnanaPozycja=rozpoznajOdleglosc(analogRead(POko));

    //Prawe zaczęło widzieć
    if(rozpoznajOdleglosc(analogRead(POko))>(MAX_Odleglosc)){
        coRobic=protokolWalka;
        if(rozpoznajOdleglosc(analogRead(LOko))>(MAX_Odleglosc)){ //Lewe widzi
            jazdaLewySilnik(90);
            jazdaPrawySilnik(90);
        }
        if(rozpoznajOdleglosc(analogRead(LOko))<(MAX_Odleglosc)){ //Lewe nie widzi
            jazdaLewySilnik(90);
            jazdaPrawySilnik(10);
        }
    }
//======================================

    //Prawe przestało widzieć
    if (rozpoznajOdleglosc(analogRead(POko))<(MAX_Odleglosc)){
        if(rozpoznajOdleglosc(analogRead(LOko))>(MAX_Odleglosc)){ //Lewe widzi
            jazdaLewySilnik(10);
            jazdaPrawySilnik(90);
        }
        if(rozpoznajOdleglosc(analogRead(LOko))<(MAX_Odleglosc)){ //Lewe nie widzi
            jazdaLewySilnik(90);
            jazdaPrawySilnik(10);
        }
    }
}





/******************************************************************
 * naprowadzanieNaWroga
 *
 *  Parametry:
 *      Brak
 *
 *  Wyjście:
 *      Przestawia zmienna globalna coRobic na opcje protokolWalka
 *
 *  Opis:
 *       Jest to zarazem przerwanie i normalna funkcja reagowania
 *       po znaleznieniu wroga.
 *       Dwa przypadki z podziałami na dwa kolejne:
 *          Prawe oko zaczyna widziec:
 *                Lewe widzi:     pełna na przód
 *                Lewe nie widzi: naprowadzanie na wroga
 *                                przez skręcanie w prawo
 *          Prawe oko przestało widziec:
 *                Lewe widzi:     korekcja toru ruchu przez
 *                                skrecanie w prawo
 *                Lewe nie widzi: korekcja ruchu przez skrecanie
 *                                w lewo
 ***************************************************************/

void Walka(){
    coRobic=protokolWalka;
    ostatniaZnanaPozycja=rozpoznajOdleglosc(analogRead(POko));
    if(rozpoznajOdleglosc(analogRead(POko))>(MIN_Odleglosc+1)){
        if(rozpoznajOdleglosc(analogRead(LOko))>(MIN_Odleglosc+1)){ //Lewe widzi
            jazdaLewySilnik(90);
            jazdaPrawySilnik(90);
        }
        if(rozpoznajOdleglosc(analogRead(LOko))<(MIN_Odleglosc+1)){ //Lewe nie widzi
            jazdaLewySilnik(90);
            jazdaPrawySilnik(10);
        }
    }
//======================================

    //Prawe przestało widzieć
    if (rozpoznajOdleglosc(analogRead(POko))<(MIN_Odleglosc+1)){
        if(rozpoznajOdleglosc(analogRead(LOko))>(MIN_Odleglosc+1)){ //Lewe widzi
            jazdaLewySilnik(10);
            jazdaPrawySilnik(90);
        }
        if(rozpoznajOdleglosc(analogRead(LOko))<(MIN_Odleglosc+1)){ //Lewe nie widzi
            jazdaLewySilnik(90);
            jazdaPrawySilnik(10);
        }
    }
}




/*
 *  void modulStartowy()
 *
 *    Parametry:
 *         Brak
 *
 *    Wyjscie:
 *        Przestawia zmienna globalna coRobic na protokolSzukaj
 *
 *    Opis:
 *         Pętla która wykonuje się do momentu, aż moduł startowy nie zostanie
 *         włączony tzn. zostanie podana wartość HIGH na wejśćie
 *         Po jej zakonczeniu rozpoczyna sie rozpedzanie silnikow
 *
 */
void modulStartowy(){
    coRobic=protokolSzukaj;
    while (digitalRead(PinStartowy) == HIGH){
        jazdaLewySilnik(0);
        jazdaPrawySilnik(0);
        Serial.print("Czekam");
    }
    for(int i=0;i<3;i++){
        jazdaLewySilnik(15+15*i);
        jazdaPrawySilnik(30+30*i);
        delay(166);
    }
}
