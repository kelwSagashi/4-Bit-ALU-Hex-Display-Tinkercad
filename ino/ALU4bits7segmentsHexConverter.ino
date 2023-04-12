//This code does not run except by the arduino IDE or thinkerCad, 
//use it to copy and paste in any of these environments
//arrays for input pins
bool bitsA[4], bitsB[4], bitsC[4], opCodeBits[2], carry = 0;
int opCode, pins[13];

//pins of segments
#define a 13
#define b A0
#define c A1
#define d A2
#define e A3
#define f A4
#define g A5

#define INPUT 0
#define OUTPUT 1

//global variables
bool D0, D1, D2, D3;
bool segA, segB, segC, segD, segE, segF, segG;

// sum bits
bool add(bool A, bool B, bool **carry){
	bool carry_in = **carry;
    bool resultado = (!A && !B && carry_in) ||
                     (!A && B && !carry_in) ||
                     (A && !B && !carry_in) ||
                     (A && B && carry_in);

    **carry = ((A && B) || (A && carry_in) || (B && carry_in));
    return resultado;
}

bool AandB(bool A, bool B){
    return A && B;
}

bool AorB(bool A, bool B){
    return A || B;
}

bool notB(bool B){
    return !B;
}

/*
selector(0,0) returns 1 -> A&B()
selector(1,0) returns 2 -> A|B()
selector(0,1) returns 3 -> ~B()
selector(1,1) returns 4 ->  +()
*/
int selector(bool A, bool B){
    if (A && B)
        return 3;
    if (B)
        return 2;
    if (A)
        return 1;
    return 0;
}

bool ALU(bool *carry, int opCode, bool bitA, bool bitB){
    switch (opCode){
    case 0:
        return AandB(bitA, bitB);
    case 1:
        return AorB(bitA, bitB);
    case 2:
        return notB(bitB);
    case 3:
        return add(bitA, bitB, &carry);
    }
}

void setup(){
    // defining INPUTs and OUTPUTs
    for (int i = 0; i < 13; i++){
        pins[i] = i;
        pinMode(pins[i], INPUT);
    }
  	pinMode(12, OUTPUT);
  	pinMode(a, OUTPUT);
  	pinMode(b, OUTPUT);
  	pinMode(c, OUTPUT);
  	pinMode(d, OUTPUT);
  	pinMode(e, OUTPUT);
  	pinMode(f, OUTPUT);
  	pinMode(g, OUTPUT);
    Serial.begin(9600);
}

void loop(){

  	carry = 0;
  	
  	readInputs();
  	define7seg();
  	showInDisplay();
    printMonitor();
    delay(500);
}

void readInputs(){
    //pins of A: 2, 3, 4, 5
    //pins of B: 6, 7, 8, 9
    //pins of OpCode: 10, 11
    for (int i = 0; i < 4; i++){
        bitsA[i] = digitalRead(pins[i + 2]);
        bitsB[i] = digitalRead(pins[i + 6]);
        if (i <= 1)
            opCodeBits[i] = digitalRead(i + 10);
    }
  
    opCode = selector(opCodeBits[0], opCodeBits[1]);
    for (int i = 0; i < 4; i++){
        bitsC[i] = ALU(&carry, opCode, bitsA[i], bitsB[i]);
    }
}

void printMonitor(){
	Serial.println("---------------");
  	Serial.println(opCode);
  	for (int i = 0; i < 4; i++){
        Serial.print(bitsA[3-i]);
    }
  	Serial.println(' ');
  	for (int i = 0; i < 4; i++){
        Serial.print(bitsB[3-i]);
    }
  	Serial.println(' ');
  	for (int i = 0; i < 4; i++){
        Serial.print(bitsC[3-i]);
    }
  	Serial.println(' ');
    Serial.print("overflow: ");
  	Serial.println(carry);
    Serial.println("---------------"); 
}

void define7seg(){
  D0 = bitsC[0];
  D1 = bitsC[1];
  D2 = bitsC[2];
  D3 = bitsC[3];
  
  segA = (!D0 && !D2) || (!D3 && D1)       || (D2 && D1)          || (D3 && !D2 && !D1)  || (!D3 && D2 && D0)   ||  (D3 && !D0);
  segB = (!D3 && !D2) || (!D2 && !D0)      || (!D3 && !D1 && !D0) || (!D3 && D1 && D0)   || (D3 && !D1 && D0);
  segC = (!D3 && !D1) || (!D3 && D0)       || (!D3 && D2)         || (!D1 && D0)         || (D3 && !D2);
  segD = (D3 && !D1)  || (!D2 && D1 && D0) || (D2 && D1 && !D0)   || (D2 && !D1 && D0)   || (!D2 && !D0 && !D3);
  segE = (!D0 && !D2) || (!D0 && D1)       || (D3 && D2)          || (D0 && D1 && D3);
  segF = (!D0 && D2)  || (D3 && D1)        || (D3 && !D2)         || (!D0 && !D1)        || (!D3 && !D1 && D2);
  segG = (!D0 && D1)  || (D0 && D3)        || (D3 && !D2)         || (!D3 && !D1 && D2)  || (D1 && !D2);
}

void showInDisplay(){
  digitalWrite(12, carry); 
  //the outputs are normal because the display is a common cathode, 
  //if it is year the outputs must be denied with "!"
  digitalWrite(a, segA);
  digitalWrite(b, segB);
  digitalWrite(c, segC);
  digitalWrite(d, segD);
  digitalWrite(e, segE);
  digitalWrite(f, segF);
  digitalWrite(g, segG);
}