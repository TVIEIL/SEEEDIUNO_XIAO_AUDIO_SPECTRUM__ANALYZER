# 1 "C:\\Users\\Vieil\\Documents\\GitHub\\SEEEDIUNO_XIAO_AUDIO_SPECTRUM_ANALYZER\\SEEEDIUNO_XIAO_AUDIO_SPECTRUM_ANALYZER.ino"


# 4 "C:\\Users\\Vieil\\Documents\\GitHub\\SEEEDIUNO_XIAO_AUDIO_SPECTRUM_ANALYZER\\SEEEDIUNO_XIAO_AUDIO_SPECTRUM_ANALYZER.ino" 2
# 5 "C:\\Users\\Vieil\\Documents\\GitHub\\SEEEDIUNO_XIAO_AUDIO_SPECTRUM_ANALYZER\\SEEEDIUNO_XIAO_AUDIO_SPECTRUM_ANALYZER.ino" 2
# 6 "C:\\Users\\Vieil\\Documents\\GitHub\\SEEEDIUNO_XIAO_AUDIO_SPECTRUM_ANALYZER\\SEEEDIUNO_XIAO_AUDIO_SPECTRUM_ANALYZER.ino" 2
# 7 "C:\\Users\\Vieil\\Documents\\GitHub\\SEEEDIUNO_XIAO_AUDIO_SPECTRUM_ANALYZER\\SEEEDIUNO_XIAO_AUDIO_SPECTRUM_ANALYZER.ino" 2
# 8 "C:\\Users\\Vieil\\Documents\\GitHub\\SEEEDIUNO_XIAO_AUDIO_SPECTRUM_ANALYZER\\SEEEDIUNO_XIAO_AUDIO_SPECTRUM_ANALYZER.ino" 2
# 19 "C:\\Users\\Vieil\\Documents\\GitHub\\SEEEDIUNO_XIAO_AUDIO_SPECTRUM_ANALYZER\\SEEEDIUNO_XIAO_AUDIO_SPECTRUM_ANALYZER.ino"
const String TitreStr1 = "AUDIO SPECTRUM ANALYZER";
const String TitreStr2 = "Thierry VIEIL - F4HRB";

uint8_t Index_Freq_value_Data =0;
int Posi =0;
uint16_t xmin =60;
uint16_t x =xmin;
uint16_t YStep =3;
uint16_t y =60;
uint16_t Freq_max = 1800;
uint16_t Freq_min = 100;
uint16_t Test_Freq = 0;
uint8_t xwide_value_FFT =3;
int test_valeur =0;
int Trigger = 0;


// Use hardware SPI (on Uno, #13, #12, #11) and the above for CS/DC
ILI9488 tft = ILI9488((5u), (4u), (3u));



arduinoFFT FFT = arduinoFFT(); /* Create FFT object */
/*

These values can be changed in order to evaluate the functions

*/
# 46 "C:\\Users\\Vieil\\Documents\\GitHub\\SEEEDIUNO_XIAO_AUDIO_SPECTRUM_ANALYZER\\SEEEDIUNO_XIAO_AUDIO_SPECTRUM_ANALYZER.ino"
const uint16_t samples = 128; //This value MUST ALWAYS be a power of 2 //32
const double samplingFrequency = 4000; //Hz, must be less than 10000 due to ADC //2500

unsigned int sampling_period_us;
unsigned long microseconds;


double vReal[samples];
double vImag[samples];
uint16_t Freq_value_Data[samples][2];


int iymin = 70;
int iy = iymin;


void setup()
{

 Serial.begin(74880);

 sampling_period_us = round(1000000*(1.0/samplingFrequency));

  tft.begin();
  tft.setRotation(1);
  tft.setScrollArea(-453, -101); //100 200 0x6400, 0xC800


  PageDemarrage();

    tft.setCursor(10, 40);
    tft.setTextColor(0xFFFF /* 255, 255, 255 */); tft.setTextSize(2);
    tft.println("  Hz");


    tft.setCursor(10, 239);
    tft.println(" 400");

    tft.setCursor(10, 203);
    tft.println(" 700");

    tft.setCursor(10, 187);
    tft.println(" 800");

    tft.setCursor(10, 171);
    tft.println(" 900");

    tft.setCursor(10, 148);
    tft.println("1100");

    tft.setCursor(10, 121);
    tft.println("1300");

    tft.setCursor(10, 101);
    tft.println("1500");

    tft.setCursor(10, 85);
    tft.println("1600");

    tft.setCursor(10, 70);
    tft.println("1700");

    tft.setCursor(56, 272);
    tft.println(".");

    tft.setCursor(170, 272);
    tft.println(".");

    tft.setCursor(304, 272);
    tft.println(".");


}

void loop()
{
          /*SAMPLING*/
          microseconds = micros();

         for(int i=0; i<samples; i++)
          {
            vReal[i] = analogRead(A0);
            vImag[i] = 0;
            while(micros() - microseconds < sampling_period_us){
              //empty loop 
            }
            microseconds += sampling_period_us;
          }

        FFT.Windowing(vReal, samples, FFTWindow::Hamming /* hamming */, FFTDirection::Forward); /* Weigh data */
        FFT.Compute(vReal, vImag, samples, FFTDirection::Forward); /* Compute FFT */
        FFT.ComplexToMagnitude(vReal, vImag, samples); /* Compute magnitudes */
        /*Serial.println("Computed magnitudes:");*/


         for (uint8_t i = 0; i < samples; i++) {
           if (i>9) {
            Freq_value_Data[i][0] =uint16_t( ((i * 1.0 * samplingFrequency) / samples)); //abscissa
            Freq_value_Data[i][1] = uint16_t( vReal[i]); //tempdata
           } //end if
            } //end for 


  if (Serial.available() > 0) {


      Serial.read();

       for (byte w = 10;w<samples; w++) { //w = 5;w<15; w++


        if (Freq_value_Data[w][1]>254) {Freq_value_Data[w][1]=254;}

        Serial.print(Freq_value_Data[w][0]); // fréquence FFT
        Serial.print(" Hz; ");
        Serial.println(Freq_value_Data[w][1]); // valeur FFT


       } //end for

} //end if serial

//DIPLAY DATA ON LCD

      for ( y = 295 /*320*/; y>0; y-=(YStep+1)){

        Test_Freq = Freq_value_Data[Posi][0];
        test_valeur = Freq_value_Data[Posi][1];

          //Gradient Bleu au jaune
        if (Test_Freq <Freq_max && Test_Freq > Freq_min && test_valeur> Trigger) {
        if (test_valeur <25) {tft.drawRect(x, y,xwide_value_FFT,YStep,0x000F /*   0,   0, 128 */);}
          if (test_valeur >=25 && test_valeur <50) {tft.drawRect(x, y,xwide_value_FFT,YStep,0x1428);}
          if (test_valeur>=50 && test_valeur<75) {tft.drawRect(x, y,xwide_value_FFT,YStep,0x25B7);}
          if (test_valeur>=75 && test_valeur<100) {tft.drawRect(x, y,xwide_value_FFT,YStep,0x363E);}
          if (test_valeur>=100 && test_valeur<125) {tft.drawRect(x, y,xwide_value_FFT,YStep,0x52FF);}
          if (test_valeur>=125 && test_valeur<150) {tft.drawRect(x, y,xwide_value_FFT,YStep,0x6B9F);}
          if (test_valeur>=150 && test_valeur<175) {tft.drawRect(x, y,xwide_value_FFT,YStep,0xFFFF /* 255, 255, 255 */);}
          if (test_valeur>=175 && test_valeur<200) {tft.drawRect(x, y,xwide_value_FFT,YStep,0xFFFF /* 255, 255, 255 */);}
          if (test_valeur>=200 && test_valeur<225) {tft.drawRect(x, y,xwide_value_FFT,YStep,0xFFFF /* 255, 255, 255 */);}
          if (test_valeur>=225) {tft.drawRect(x, y,xwide_value_FFT,YStep,0xFFE0 /* 255, 255,   0 */);}

        }


        Posi++;
        if (Posi >samples ) {Posi = 0; y=295 /*320*/; break;}



       }//end for y


  tft.scroll(iy);
  iy+=1;
  if (iy>290) {iy=iymin;}
  //Serial.println(iy);
  x=x+xwide_value_FFT +1;
  if (x>(480 -xmin)) x=xmin;


//réinitialisation
test_valeur =0;
//Test_Freq =0;

       } //End void

void PageDemarrage(){
  //tft.fillScreen(ILI9488_BLACK);
  tft.setCursor(30, 120);
  tft.fillRect(20, 110, 440, 90, 0x001F /*   0,   0, 255 */);
  tft.setTextColor(0xFFFF /* 255, 255, 255 */); tft.setTextSize(3);
  tft.println(TitreStr1);
  tft.setCursor(50, 170);
  tft.setTextColor(0xFFE0 /* 255, 255,   0 */); tft.setTextSize(3);
  tft.println(TitreStr2);
  delay(2000);
  tft.fillScreen(0x0000 /*   0,   0,   0 */);
}
