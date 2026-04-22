#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#endif

#define MAX_LINE 1024
#define FILE_PATH "data/time.csv"


struct time_data {
  int seconds;
  int minutes;
  int hours;
};


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




void sum_time_by_date(char *filepath, char *date){
  FILE *file = fopen(filepath, "r");
  if (file == NULL) {
    printf("%s\n", "file not found");
  } else {
    char line_buffer[255];
    char row[5][12];
    int total_sec = 0;

    while(fgets(line_buffer, 255, file)){

      char *token = strtok(line_buffer, ", ");
      int counter = 0;
      while (token != NULL) {
        strncpy(row[counter], token, 12);
        row[counter][11] = '\0';
        counter++;
        token = strtok(NULL, " ");

      }

      if (strcmp(date, row[0]) == 0) {
        row[4][8] = '\0';

        char *hours = strtok(row[4], ":");
        char *minutes = strtok(NULL, ":");
        char *seconds = strtok(NULL, ":");

        total_sec += atoi(seconds) + atoi(minutes) * 60 + atoi(hours) * 3600;
      }



  }

  printf("%d\n", total_sec);

}
}


char *** read_csv(const char *filepath) {
  FILE *file = fopen(filepath, "r");
  if (file == NULL) {
    printf("%s\n", "file not found");
    return NULL;
  } else {



    int byte_size_cell = 12;
    int rows = 200000;
    int columns = 5;


    char ***m_array = malloc(sizeof(char *) * rows);

    for (int a = 0; a < rows; a++) {
      m_array[a] = malloc(sizeof(char *) * columns);
      for (int b = 0; b < columns; b++){
        m_array[a][b] = malloc(sizeof(char) * byte_size_cell);
      }
    }

    int row_count = 0;

    char line_buffer[255];

    while(fgets(line_buffer, 255, file)){

      int col_count = 0;
      char *token = strtok(line_buffer, " ");

      while(token != NULL){
        strncpy(m_array[row_count][col_count], token, byte_size_cell -1);
        m_array[row_count][col_count][byte_size_cell -1] = '\0';
        token = strtok(NULL, " ");
        col_count++;

      }

      row_count++;

  }


  return m_array;


  }


}


void sum_time_since(const char *filepath, const char *date) {

  FILE *file = fopen(filepath, "r");

  if (file == NULL) {
    printf("%s\n", "file not found");
  } else {

    int total_sec = 0;

    //iterate over lines
    char line[255];
    while (fgets(line, 255, file)) {

      //iterate over csvd
      int counter = 0;
      char* token = strtok(line, " ");
      while (token != NULL) {
        
        
        token  = strtok(NULL, " ");
        counter++;
        if (counter == 4) {

          //iterate over time

          char *hours = strtok(token, ":");
          char *minutes = strtok(NULL, ":");
          char *seconds = strtok(NULL, ":");

          total_sec += atoi(seconds) + atoi(minutes) * 60 + atoi(hours) * 3600;

        }
      }
    }

    struct time_data converted = double_to_time(total_sec);
    printf("> Totale Zeit: \033[32m%d\033[0m Stunden, \033[32m%d\033[0m Minuten und \033[32m%d\033[0m Sekunde(n)\n", converted.hours, converted.minutes, converted.seconds);


  }
}

void sum_time_today() {
}

void sum_time_week() {

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
      (*previous_time).tm_mon+1,
      (*previous_time).tm_year + 1900,
      (*previous_time).tm_hour,
      (*previous_time).tm_min,
      (*previous_time).tm_sec,
      (*t).tm_mday,
      (*t).tm_mon+1,
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

  #ifdef _WIN32
  SetConsoleOutputCP(CP_UTF8);
  #endif


  int enter_flag = 0;

  time_t now;

  char buffer[MAX_LINE];

  printf("\033[2J\033[H");
  printf("\033[31mCommands: q- > quit, Enter -> start/stop\033[0m\n\n");

  for(;;) {


    fgets(buffer, MAX_LINE, stdin);

    
    //TODAY
    if (strcmp(buffer, "t\n") == 0){

      printf("\033[2J\033[H");
      printf("\033[31mCommands: q- > quit, Enter -> start/stop\033[0m\n\n");

      now = time(NULL);
      struct tm *time = localtime(&now);
      char date_as_string[200];
      snprintf(date_as_string, 200, "%02d.%02d.%04d", time->tm_mday, time->tm_mon+1, time->tm_year+1900);
      sum_time_by_date(FILE_PATH, date_as_string);


    } 

    //STARTZEIT
    else if (strcmp(buffer, "\n") == 0 && enter_flag == 0){
      now = time(NULL);
      struct tm *start_time = localtime(&now);

      printf("\033[2J\033[H");
      printf("\033[31mCommands: q- > quit, Enter -> start/stop\033[0m\n\n");
      printf("> Startzeit: \033[32m%02d:%02d:%02d\033[0m Uhr, am %02d.%02d.%02d\n", 
        (*start_time).tm_hour,
        (*start_time).tm_min,
        (*start_time).tm_sec,
        (*start_time).tm_mday,
        (*start_time).tm_mon+1,
        (*start_time).tm_year + 1900
      );
      printf("┌────────────────────────────────────────────────┐\n");
      printf("│.########.####.##.....##.####.##....##..######..│\n");
      printf("│....##.....##..###...###..##..###...##.##....##.│\n");
      printf("│....##.....##..####.####..##..####..##.##.......│\n");
      printf("│....##.....##..##.###.##..##..##.##.##.##...####│\n");
      printf("│....##.....##..##.....##..##..##..####.##....##.│\n");
      printf("│....##.....##..##.....##..##..##...###.##....##.│\n");
      printf("│....##....####.##.....##.####.##....##..######..│\n");
      printf("└────────────────────────────────────────────────┘\n");
      
      enter_flag = 1;
    } 


    //ENDZEIT
    else if (strcmp(buffer, "\n") == 0 && enter_flag == 1){

      struct time_data diff = append_time(FILE_PATH, &now);

      now = time(NULL);
      struct tm *start_time = localtime(&now);

      printf("\033[2J\033[H");
      printf("\033[31mCommands: q- > quit, Enter -> start/stop\033[0m\n\n");
      printf("> Endzeit: \033[32m%02d:%02d:%02d\033[0m Uhr, am %02d.%02d.%02d\n\n", 
        (*start_time).tm_hour,
        (*start_time).tm_min,
        (*start_time).tm_sec,
        (*start_time).tm_mday,
        (*start_time).tm_mon+1,
        (*start_time).tm_year + 1900
      );

      printf("> Zeitdauer: \033[32m%d\033[0m Stunden, \033[32m%d\033[0m Minuten und \033[32m%d\033[0m Sekunde(n)\n", diff.hours, diff.minutes, diff.seconds);

      enter_flag = 0;
    } 

    else if (strcmp(buffer, "q\n") == 0) {
      break;
    }

  

}

return 0;
}