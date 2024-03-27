#include <Arduino.h>
#include <String.h>
#include <PrayerTimes.h>

// seting date to 19 Jul 2020
int my_year = 2023;
int my_month = 1;
int my_day = 7;
double my_latitude = 18.4088;
double my_longitude = 76.5604;
double my_timezone;
bool my_dst_on;
String my_calc_method;
String my_asr_method;
String my_high_lats_method;
String fajar,Zuhar,asar,magrib,isha,sunrise,sunset;
String fjarh,fjarm,sunriseh,sunrisem,zuharh,zuharm,asarh,asarm,magribh,magribm,sunseth,sunsetm,ishah,isham;

// Please refer to http://praytimes.org/manual for details of all above parameters.

PrayerTimes my_prayer_object;                        // PrayerTimes object
double my_prayer_times[my_prayer_object.TimesCount]; // Array for receiveing prayer times

// Sunnyvale , CA, USA specific value
void set_to_sunnyvale()
{
  // my_latitude = 37.368832;
  // my_longitude = -122.036346;
  my_timezone = -8;
  my_dst_on = true; // True between March to September
  my_calc_method = "ISNA";
  my_asr_method = "Shafii";
  my_high_lats_method = "None";
}

// Mumbai , MH , India specific value
void set_to_mumbai()
{
  // my_latitude = 19.097403;
  // my_longitude = 72.874245;
  my_timezone = 5.5;
  my_dst_on = false; // Never used in India
  my_calc_method = "Karachi";
  my_asr_method = "juristic";
  my_high_lats_method = "None";
}

void setupapi()
{
  Serial.begin(115200);
  String inStringForm;

  Serial.print("*******  Prayer Times for the date : ");
  Serial.print(my_month);
  Serial.print("/");
  Serial.print(my_day);
  Serial.print("/");
  Serial.print(my_year);
  Serial.print(" *******\n\n");
  set_to_sunnyvale();
  Serial.println("===================  Sunnyvale, CA, USA  =================");
  Serial.println("Prayer\t\tNumberic-Time\t24Hr-Format\t12Hr-Format");
  Serial.println("----------------------------------------------------------");
  my_prayer_object.get_prayer_times(my_year, my_month, my_day, my_latitude, my_longitude, my_timezone, my_dst_on, my_calc_method, my_asr_method, my_high_lats_method, my_prayer_times);
  for (int i = 0; i < my_prayer_object.TimesCount; ++i)
  {
    Serial.print(my_prayer_object.get_prayer_name(i));
    Serial.print("\t\t");
    Serial.print(my_prayer_times[i]);
    Serial.print("\t\t");
    inStringForm = my_prayer_object.float_time_to_time24(my_prayer_times[i]);
    Serial.print(inStringForm);
    ;
    Serial.print("\t\t");
    inStringForm = my_prayer_object.float_time_to_time12(my_prayer_times[i]);
    Serial.println(inStringForm);
  }

  Serial.println("\n");

  set_to_mumbai();
  Serial.println("===================== Mumbai , India =====================");
  Serial.println("Prayer\t\tNumberic-Time\t24Hr-Format\t12Hr-Format");
  Serial.println("----------------------------------------------------------");
  my_prayer_object.get_prayer_times(my_year, my_month, my_day, my_latitude, my_longitude, my_timezone, my_dst_on, my_calc_method, my_asr_method, my_high_lats_method, my_prayer_times);
  for (int i = 0; i < my_prayer_object.TimesCount; ++i)
  {
    Serial.print(my_prayer_object.get_prayer_name(i));
    Serial.print("\t\t");
    Serial.print(my_prayer_times[i]);
    Serial.print("\t\t");
    inStringForm = my_prayer_object.float_time_to_time24(my_prayer_times[i]);
    Serial.print(inStringForm);
    ;
    Serial.print("\t\t");
    inStringForm = my_prayer_object.float_time_to_time12(my_prayer_times[i]);
    Serial.println(inStringForm);
    if(i == 0){
 fajar =inStringForm;
    fjarh = fajar.substring(0,(fajar.indexOf(":")));
    fjarm = fajar.substring((fajar.indexOf(":")+1),(fajar.length()));
Serial.println(fjarh);
Serial.println(fjarm);
    }
    if(i == 1){
         sunrise = inStringForm;
         sunriseh = sunrise.substring(0,(sunrise.indexOf(":")));
         sunrisem = sunrise.substring((sunrise.indexOf(":")+1),(sunrise.length()));
Serial.println(sunrise);
Serial.println(sunrise);
    }
    if(i == 2){
      Zuhar = inStringForm;
        zuharh = Zuhar.substring(0,(Zuhar.indexOf(":")));
         zuharm = Zuhar.substring((Zuhar.indexOf(":")+1),(Zuhar.length()));
Serial.println(zuharh);
Serial.println(zuharm);
    }
    if(i == 3 )
    {
asar = inStringForm;
 asarh = asar.substring(0,(asar.indexOf(":")));
         asarm = asar.substring((asar.indexOf(":")+1),(asar.length()));
Serial.println(asarh);
Serial.println(asarm);
    }
    if(i == 4){
sunset = inStringForm;
 sunseth = sunset.substring(0,(sunset.indexOf(":")));
         sunsetm = sunset.substring((sunset.indexOf(":")+1),(sunset.length()));
Serial.println(sunseth);
Serial.println(sunsetm);
    }
    if(i == 5){
      magrib =inStringForm;
      magribh = magrib.substring(0,(magrib.indexOf(":")));
         magribm = magrib.substring((magrib.indexOf(":")+1),(magrib.length()));
Serial.println(magribh);
Serial.println(magribm);
    }
    if(i == 6){
isha = inStringForm;
     ishah = isha.substring(0,(isha.indexOf(":")));
         isham = isha.substring((isha.indexOf(":")+1),(isha.length()));
Serial.println(ishah);
Serial.println(isham);
    }
    Serial.println("Isha : "+isha);
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
}

