#include <time.h> 
#include <stdio.h> 
#include <sys/times.h> 
  
struct timespec { 
    time_t tv_sec; 
    long tv_nsec; 
}; 

 int gettimes(struct timespec *real, struct timespec *user, struct timespec *sys) { 

     struct tms tms_start, tms_end; 

     clock_t start = clock(); 

     if (start == (clock_t) -1) {  

         perror("Error en clock");  

         return -1;  

     }  

     times(&tms_start); .  

     clock_t end = times(&tms_end); 

     if (end == (clock_t) -1) {  

         perror("Error en times");  

         return -1;  

     }  

     real->tv_sec = end - start;  

     user->tv_sec = tms_end.tms _utime - tms _start .tm s _utime ;  

     sys->tv _sec = tm s _end .tm s _sti me -tm s _star t .tm s _sti me ; 

      return 0 ;  }  int main(void){ struct timespec real , user , sys ; gettimes(&real , &user , &sys ) ; printf("Real : %ld \\ nUser : %ld \\ nSys : %ld \\ n" , real .tv _sec , user .tv _sec , sys .tv _sec ) ; return 0 ; }