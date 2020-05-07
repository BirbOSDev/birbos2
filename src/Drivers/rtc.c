#include "../common.h"
#include "../all_drivers.h"



century_register = 0x32; 
 

 
int get_update_in_progress_flag() {
      outportb(cmos_address, 0x0A);
      return (inportb(cmos_data) & 0x80);
}
 
unsigned char get_RTC_register(int reg) {
      outportb(cmos_address, reg);
      return inportb(cmos_data);
}

void write_RTC_register(int reg, int data) {
      outportb(cmos_address, reg);
      outportb(cmos_data, data);
}
 
void read_rtc() {
      
      unsigned char last_second;
      unsigned char last_minute;
      unsigned char last_hour;
      unsigned char last_day;
      unsigned char last_month;
      unsigned char last_weekday;
      unsigned char last_year;
      unsigned char last_century;
      unsigned char registerB;
 
      // Note: This uses the "read registers until you get the same values twice in a row" technique
      //       to avoid getting dodgy/inconsistent values due to RTC updates
 
      while (get_update_in_progress_flag());                // Make sure an update isn't in progress
      second = get_RTC_register(0x00);
      minute = get_RTC_register(0x02);
      hour = get_RTC_register(0x04);
      weekday = get_RTC_register(0x06);
      day = get_RTC_register(0x07);
      month = get_RTC_register(0x08);
      year = get_RTC_register(0x09);
      if(century_register != 0) {
            century = get_RTC_register(century_register);
      }
 
      do {
            last_second = second;
            last_minute = minute;
            last_hour = hour;
            last_day = day;
            last_month = month;
            last_weekday = weekday;
            last_year = year;
            last_century = century;
 
            while (get_update_in_progress_flag());           // Make sure an update isn't in progress
            second = get_RTC_register(0x00);
            minute = get_RTC_register(0x02);
            hour = get_RTC_register(0x04);
            weekday = get_RTC_register(0x06);
            day = get_RTC_register(0x07);
            month = get_RTC_register(0x08);
            year = get_RTC_register(0x09);
            if(century_register != 0) {
                  century = get_RTC_register(century_register);
            }
      } while( (last_second != second) || (last_minute != minute) || (last_hour != hour) || (last_weekday != weekday) ||
               (last_day != day) || (last_month != month) || (last_year != year) ||
               (last_century != century) );
 
      registerB = get_RTC_register(0x0B);
 
      // Convert BCD to binary values if necessary
 
      if (!(registerB & 0x04)) {
            second = (second & 0x0F) + ((second / 16) * 10);
            minute = (minute & 0x0F) + ((minute / 16) * 10);
            hour = ( (hour & 0x0F) + (((hour & 0x70) / 16) * 10) ) | (hour & 0x80);
            day = (day & 0x0F) + ((day / 16) * 10);
            month = (month & 0x0F) + ((month / 16) * 10);
            year = (year & 0x0F) + ((year / 16) * 10);
            if(century_register != 0) {
                  century = (century & 0x0F) + ((century / 16) * 10);
            }
      }
 
      // Convert 12 hour clock to 24 hour clock if necessary
 
      if (!(registerB & 0x02) && (hour & 0x80)) {
            hour = ((hour & 0x7F) + 12) % 24;
      }
 
      // Calculate the full (4-digit) year
      
      if(century_register != 0) {
            year += century * 100;
      }
}



void rtc_print_formatted_time(){
    switch(weekday){
          case 1: print("Sunday "); break;
          case 2: print("Monday "); break;
          case 3: print("Tuesday "); break;
          case 4: print("Wednesday "); break;
          case 5: print("Thursday "); break;
          case 6: print("Friday "); break;
          case 7: print("Saturday "); break;
          default: break;


    }
    print(itoa(day,10));
    print("/");
    print(itoa(month,10));
    print("/");
    print(itoa(year,10));
    print(" ");
    if(hour<10){
        print("0");
    }
    print(itoa(hour,10));
    print(":");
    if(minute<10){
        print("0");
    }
    print(itoa(minute,10));
    print(":");
    if(second<10){
        print("0");
    }
    print(itoa(second,10));
}
