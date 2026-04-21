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

//when passing an array to function, it decays to a pointer -> sizeof checks pointer lenght not array length
void print_array(int a[], int len){
  for (int i = 0; i < len; i++) {
    printf("%d ", a[i]);
  }
}


void read_csv(const char *filepath) {
  FILE *file = fopen(filepath, "r");
  if (file == NULL) {
    printf("%s\n", "file not found");
  } else {

    //DEFINITION
    int byte_size_cell = 12;
    int rows = 20;
    int columns = 5;

    char ***m_array = malloc(sizeof(char *) * rows);

    for (int a = 0; a < rows; a++) {
      m_array[a] = malloc(sizeof(char *) * columns);
      for (int b = 0; b < columns; b++){
        m_array[a][b] = malloc(sizeof(char) * byte_size_cell);
      }
    }

    m_array[3][4][0] = 't';
    m_array[3][4][1] = 'e';
    m_array[3][4][2] = 's';
    m_array[3][4][3] = 't';
    m_array[3][4][4] = '\0';

    printf("%s\n", m_array[3][4]);


    //malloc nimmt eine größe und gibt einen pointer zu memory
    //mit dieser größe zurück
    char **array = malloc(sizeof(char *) * 5);

    for (int i = 0; i < 5; i++) {
      array[i] = malloc(sizeof(char) * byte_size_cell);
    }

    char line[255];

    fgets(line, 255, file);

    int cell_counter = 0;
    char *token = strtok(line, " ");

    while (token != NULL){
      strncpy(array[cell_counter], token, byte_size_cell -1);
      array[cell_counter][byte_size_cell - 1] = '\0';
      token = strtok(NULL, " ");
      cell_counter++;
    }

    printf("%s\n", array[4]);

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

    
    //TEST
    if (strcmp(buffer, "t\n") == 0){

      printf("\033[2J\033[H");
      printf("\033[31mCommands: q- > quit, Enter -> start/stop\033[0m\n\n");
      read_csv("data/time.csv");

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
        (*start_time).tm_mon,
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
        (*start_time).tm_mon,
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