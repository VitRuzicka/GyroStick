#define CPU 80  //frekvence v MHz (80 taktů v us)
#define PPM_out 14  //pin kde bude generovan ppm
#define pocet_kanalu 6 //ppm kanaly
#define nabeznaHr 500*CPU  //0.5ms při startu nových dat
#define delkacyklu 20000*CPU

// clockCyclesPerMicrosecond() = 80
uint8_t  poradi = 0;
uint8_t  faze = 0;
uint32_t delka_framu = 0;
uint32_t kanal[pocet_kanalu];
uint32_t dalsi;
    
ICACHE_RAM_ATTR void inline PPM_ISR(void)
{
  uint32_t ppm_sig;
  if (faze == 0)           //pocatecni puls
  {
    dalsi += nabeznaHr; // 0.5 ms
    timer0_write(dalsi);
    digitalWrite(PPM_out,LOW);
    delka_framu += 40000;
    faze = 1;
  }
  else
  {
    if (poradi >= pocet_kanalu)   //vsechny kanaly poslany, 
    {
      dalsi += delkacyklu - delka_framu; // 20ms total 
      timer0_write(dalsi);
      digitalWrite(PPM_out,HIGH);
      poradi = 0;
      delka_framu = 0;
    } 
    else 
    {  
      ppm_sig = kanal[poradi];
      dalsi += ppm_sig;
      timer0_write(dalsi);
      digitalWrite(PPM_out,HIGH);
      delka_framu += ppm_sig;
      poradi++;
    }
    faze = 0;
  }
}

void init_RC()
{
  for (int i=0;i<pocet_kanalu;i++) if(i != 2){kanal[i] = 1500*CPU - nabeznaHr;}else{kanal[i] = 1000*CPU - nabeznaHr;};  //1500*80 pro stredovou pozici
  pinMode(PPM_out, OUTPUT); 
  noInterrupts();
  timer0_isr_init();
  timer0_attachInterrupt(PPM_ISR);
  dalsi=ESP.getCycleCount()+100000; //2ms - doba mezi jednotlivými framy
  timer0_write(dalsi);
  interrupts();
}
int konvertuj(int a){ //konvertuje 0-255 na 1000-2000 na cpu cykly (pro ISR)
  a = constrain(a, 0, 255);
  a = map(a, 0,255, 1000, 2000);
  a = a *CPU - nabeznaHr;       //doba po kterou bude pin LOW pri odesilani PPM signalu kanalu
return a;
}


void setup() {
  // put your setup code here, to run once:
init_RC();
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < 254; i++){
kanal[0] = konvertuj(i);
delay(20);
  
  }

}
