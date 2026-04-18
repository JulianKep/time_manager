#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define MAX_LINE 1024


struct time_data {
  int seconds;
  int minutes;
  int hours;
};




//when passing an array to function, it decays to a pointer -> sizeof checks pointer lenght not array length
void print_array(int a[], int len){
  for (int i = 0; i < len; i++) {
    printf("%d ", a[i]);
  }
}


void output_time_since() {

}

void sum_time_since(const char *filepath) {

}

void sum_time_today() {
}

void sum_time_week() {

}




struct time_data double_to_time(double time_as_sec) {

  int time = (int) time_as_sec;
  struct time_data out;

  out.hours = time / 3600;
  time = time % 3600;
  out.minutes = time / 60;
  time = time % 60;
  out.seconds = time;

  return out;
  
}



struct time_data append_time(const char *filepath, const time_t *previous) {

  FILE *f = fopen(filepath, "a");

  struct time_data output;

  if (f == NULL) {
    printf("%s\n", "file not found");
  } else {

    //create current time object
    time_t now = time(NULL);
    struct tm *t = localtime(&now);

    //turn previous time into struct
    struct tm *previous_time = localtime(previous);

    //calculate difference
    double diff = difftime(now, *previous);

    struct time_data diff_as_struct = double_to_time(diff);

    char buffer[500];
    sprintf(buffer, "%02d.%02d.%d, %02d:%02d:%02d, %02d.%02d.%d, %02d:%02d:%02d, %02d:%02d:%02d\n", 
      (*previous_time).tm_mday,
      (*previous_time).tm_mon,
      (*previous_time).tm_year + 1900,
      (*previous_time).tm_hour,
      (*previous_time).tm_min,
      (*previous_time).tm_sec,
      (*t).tm_mday,
      (*t).tm_mon,
      (*t).tm_year + 1900,
      (*t).tm_hour,
      (*t).tm_min,
      (*t).tm_sec,
      diff_as_struct.hours,
      diff_as_struct.minutes,
      diff_as_struct.seconds
    );

    fputs(buffer, f);
    output = diff_as_struct;

  }


  fclose(f);

  return output;
}



int main() {
      /* printf("\033[2J\033[H"); */

  int enter_flag = 0;

  time_t now;

  printf("\033[31mEnter druecken um zu starten:\033[0m");



  for(;;) {


    char buffer[MAX_LINE];

    fgets(buffer, MAX_LINE, stdin);

    if (strcmp(buffer, "m\n") == 0){
      printf("Menue Optionen:\n q -> programm verlassen \n s -> \n");
      fgets(buffer, MAX_LINE, stdin);
      if (strcmp(buffer, "q\n") == 0) {
        break;
      }
    } 

    //STARTZEIT
    else if (strcmp(buffer, "\n") == 0 && enter_flag == 0){
      now = time(NULL);
      struct tm *start_time = localtime(&now);

      printf("> Startzeit: \033[32m%02d:%02d:%02d\033[0m Uhr, am %02d.%02d.%02d\n", 
        (*start_time).tm_hour,
        (*start_time).tm_min,
        (*start_time).tm_sec,
        (*start_time).tm_mday,
        (*start_time).tm_mon,
        (*start_time).tm_year + 1900
      );
      enter_flag = 1;
    } 


    //ENDZEIT
    else if (strcmp(buffer, "\n") == 0 && enter_flag == 1){

      struct time_data diff = append_time("data/time.csv", &now);

      now = time(NULL);
      struct tm *start_time = localtime(&now);

      printf("> Endzeit: \033[32m%02d:%02d:%02d\033[0m Uhr, am %02d.%02d.%02d\n\n", 
        (*start_time).tm_hour,
        (*start_time).tm_min,
        (*start_time).tm_sec,
        (*start_time).tm_mday,
        (*start_time).tm_mon,
        (*start_time).tm_year + 1900
      );

      printf("> Zeitdauer: \033[32m%d\033[0m Stunden, \033[32m%d\033[0m Minuten und \033[32m%d\033[0m Sekunde(n)\n", diff.hours, diff.minutes, diff.seconds);
      printf("\033[31mEnter druecken fuer die naechste Messung, q druecken zum beenden:\033[0m\n");

      enter_flag = 0;
    } 

  

}

return 0;
}








/* int main() {


  print_time();

  FILE *pF = fopen("data/time.csv", "r");
  char buffer[255];

  if (pF == NULL) {
    printf("Unable to open file \n");
    return 1;
    
  } else {



    while (fgets(buffer, 255, pF) != NULL) {


      char result[255];
      int result_len = 0;

      int comma_positions[20];
      int len_comma_position = 0;

      int *commas = malloc(20 * sizeof(int));


      for (int i = 0; i < strlen(buffer); i++) {
        if (buffer[i] == ',') {
          comma_positions[len_comma_position++] = i;

          

        }
      }

      print_array(comma_positions, len_comma_position);
      free(commas);
    }
  }

  fclose(pF);


  return 0;
} */