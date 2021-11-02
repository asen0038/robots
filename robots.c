#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

char map[8][17];
char robomap[8][17];
char tempmap[8][17];
char human = 'H';
char robots = 'R';
char exit_loc = 'E';
char destroyed = 'D';
bool gameover = true;


int string_compare(const char* w1, const char* w2, int len){
  const char *p1 = w1;
  const char *p2 = w2;
  int x = 0;
  for(int i = 0; i < len; i++){
    if(*p1 == '\0' || *p2 == '\0'){
      break;
    }else if(*p1 != *p2){
      x++;
    }
    p1++;
    p2++;
  }
  return x;

}


void exitPosition(){
  int x = rand() % 8;
  int y = rand() % 8;
  if(gameover){
      map[y][(2*x) + 1] = exit_loc;
  }
}

void setupMap(){
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 17; j++){
      if(j % 2 == 0){
        map[i][j] = '|';
      }else{
        map[i][j] = ' ';
      }
    }
  }
}

void display(){
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 17; j++){
      if(j % 2 == 0){
        if(j == 16){
          printf("%c\n", map[i][j]);
        }else{
          printf("%c", map[i][j]);
        }
      }else{
        printf("%c", map[i][j]);
      }
    }
  }
}

int coordinatesCheck(const char x, const char y){

  int count = 0;
  char valid[8] = {'0', '1', '2', '3', '4', '5', '6', '7'};
  for(int i = 0; i < 8; i++){
    if(x == valid[i]){
      count++;
      break;
    }else{
      if(i == 7){
        return 0;
      }
    }
  }

  if(count != 0){
    for(int i = 0; i < 8; i++){
      if(y == valid[i]){
        return 1;
      }else{
        if(i == 7){
          return 0;
        }
      }
    }
  }

  return 0;
}

int robocheck(const char a, const char b){
  int x = a - '0';
  int y = b - '0';

  if(map[y][(2*x + 1)] == human){
    return 0;
  }

  if(map[y][(2*x + 1)] == exit_loc){
    return 0;
  }

  if(map[y][(2*x + 1)] == robots){
    return 0;
  }

  if(y + 1 > 7){
    if(map[y-1][(2*x + 1)] == human || map[y-2][(2*x + 1)] == human){
      return 0;
    }
  }else if(y + 2 > 7){
    if(map[y+1][(2*x + 1)] == human || map[y-1][(2*x + 1)] == human || map[y-2][(2*x + 1)] == human){
      return 0;
    }
  }

  if(y - 1 < 0){
    if(map[y+1][(2*x + 1)] == human || map[y+2][(2*x + 1)] == human){
      return 0;
    }
  }else if(y - 2 < 0){
    if(map[y-1][(2*x + 1)] == human || map[y+1][(2*x + 1)] == human || map[y+2][(2*x + 1)] == human){
      return 0;
    }
  }


  if((2*x + 1) + 2 > 16){
    if(map[y][(2*x + 1) - 2] == human || map[y][(2*x + 1) - 4] == human){
      return 0;
    }
  }else if((2*x + 1) + 4 > 16){
    if(map[y][(2*x + 1) + 2] == human || map[y][(2*x + 1) - 2] == human || map[y][(2*x + 1) - 4] == human){
      return 0;
    }
  }

  if((2*x + 1) - 2 < 0){
    if(map[y][(2*x + 1) + 2] == human || map[y][(2*x + 1) + 4] == human){
      return 0;
    }
  }else if((2*x + 1) - 4 < 0){
    if(map[y][(2*x + 1) - 2] == human || map[y][(2*x + 1) + 2] == human || map[y][(2*x + 1) + 4] == human){
      return 0;
    }
  }

  if(map[y+1][(2*x + 1)] == human || map[y+2][(2*x + 1)] == human || map[y-1][(2*x + 1)] == human || map[y-2][(2*x + 1)] == human || map[y][(2*x + 1) + 2] == human || map[y][(2*x + 1) + 4] == human || map[y][(2*x + 1) - 2] == human || map[y][(2*x + 1) - 4] == human){
    return 0;
  }

  if(map[y+1][(2*x + 1) + 2] == human || map[y+1][(2*x + 1) - 2] == human || map[y-1][(2*x + 1) - 2] == human || map[y-1][(2*x + 1) + 2] == human){
    return 0;
  }

  if(map[y+2][(2*x + 1) + 2] == human || map[y+2][(2*x + 1) - 2] == human || map[y-2][(2*x + 1) - 2] == human || map[y-2][(2*x + 1) + 2] == human){
    return 0;
  }

  if(map[y+1][(2*x + 1) + 4] == human || map[y+1][(2*x + 1) - 4] == human || map[y-1][(2*x + 1) + 4] == human || map[y-1][(2*x + 1) - 4] == human){
    return 0;
  }

  return 1;
}


void restart(){
  char pos[5];
  char rob[5];
  char p[5];
  int r;

  for(int i = 0; i < 2; i++){
    if(i == 0){
      printf("Human starting point: ");
      fgets(pos, 10, stdin);
      if(pos[1] != ' ' || pos[3] != '\n'){
        printf("Invalid Coordinates, Try again\n");
        i--;
      }else{
        int check = coordinatesCheck(pos[0], pos[2]);
        if(check == 0){
          printf("Invalid Coordinates, Try again\n");
          i--;
        }else{
          int x = atoi(&pos[0]);
          int y = atoi(&pos[2]);
          map[y][(2*x) + 1] = human;
        }
      }
    }if(i == 1){

      printf("Number of robots: ");
      fgets(p, 10, stdin);
      for(int f = 0; f < 10; f++){
        if(p[f] == '\n'){
          p[f] = '\0';
          break;
        }
      }
      if(strlen(p) > 2 || strlen(p) == 0){
        printf("Invalid number, Try again\n");
        i--;
      }else if(strlen(p) == 2){
        if(isdigit(p[0]) == 0 || isdigit(p[1]) == 0){
          printf("Invalid number, Try again\n");
          i--;
        }
      }else if(strlen(p) == 1){
        if(isdigit(p[0]) == 0){
          printf("Invalid number, Try again\n");
          i--;
        }
      }

      r = atoi(p);
      if(r > 28 || r == 0){
        printf("Invalid number, Try again\n");
        i--;
      }else{
        for(int j = 1; j <= r; j++){
          printf("Robot %d: ", j);
          fgets(rob, 100, stdin);

          if(rob[1] != ' ' || rob[3] != '\n'){
            printf("Invalid Coordinates, Try again\n");
            j--;
          }else{
            int check2 = coordinatesCheck(rob[0], rob[2]);
            if(check2 == 0){
              printf("Invalid Coordinates, Try again\n");
              j--;
            }else{
              int check3 = robocheck(rob[0], rob[2]);
              if(check3 == 0){
                printf("Invalid Coordinates, Try again\n");
                j--;
              }else{
                int x = atoi(&rob[0]);
                int y = atoi(&rob[2]);
                /*if(map[y][(2*x + 1)] == exit_loc){
                  tempmap[y][(2*x + 1)] = exit_loc;
                }*/
                map[y][(2*x) + 1] = robots;
              }
            }
          }
        }
      }
    }
  }

}

void doNothing(){
}

void resest(){
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 17; j++){
      tempmap[i][j] = ' ';
    }
  }
}

void clearRoboMaps(){
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 17; j++){
      robomap[i][j] = ' ';
    }
  }
}

int move(const char a, const char b){


  if(b != 'T'){
    return 0;
  }

  char dir[4] = {'N', 'S', 'E', 'W'};
  for(int i = 0; i < 4; i++){
    if(a == dir[i]){
      break;
    }else{
      if(i == 3){
        return 0;
      }
    }
  }

  int x;
  int y;
  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 17; j++){
      if(map[i][j] == human){
        if(a == 'N'){
          map[i][j] = ' ';
          if(map[i-1][j] == robots){
            return 2;
          }else if(map[i-1][j] == exit_loc){
            return 3;
          }else if(map[i-1][j] == destroyed){
            tempmap[i-1][j] = destroyed;
            map[i-1][j] = human;
            x = j;
            y = i - 1;
          }else{
            map[i-1][j] = human;
            x = j;
            y = i - 1;
          }
        }else if(a == 'S'){
          map[i][j] = ' ';
          if(map[i+1][j] == robots){
            return 2;
          }else if(map[i+1][j] == exit_loc){
            return 3;
          }else if(map[i+1][j] == destroyed){
            tempmap[i+1][j] = destroyed;
            map[i+1][j] = human;
            x = j;
            y = i + 1;
            i = i + 1;
          }else{
            map[i+1][j] = human;
            x = j;
            y = i + 1;
            i = i + 1;
          }
        }else if(a == 'E'){
          map[i][j] = ' ';
          if(map[i][j+2] == robots){
            return 2;
          }else if(map[i][j+2] == exit_loc){
            return 3;
          }else if(map[i][j+2] == destroyed){
            tempmap[i][j+2] = destroyed;
            map[i][j+2] = human;
            x = j + 2;
            y = i;
            j = j + 2;
          }else{
            map[i][j+2] = human;
            x = j + 2;
            y = i;
            j = j + 2;
          }
        }else{
          map[i][j] = ' ';
          if(map[i][j-2] == robots){
            return 2;
          }else if(map[i][j-2] == exit_loc){
            return 3;
          }else if(map[i][j-2] == destroyed){
            tempmap[i][j-2] = destroyed;
            map[i][j-2] = human;
            x = j - 2;
            y = i;
          }else{
            map[i][j-2] = human;
            x = j - 2;
            y = i;
          }
        }

        if(tempmap[i][j] == destroyed){
          map[i][j] = destroyed;
        }
      }
    }
  }

  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 17; j++){
      if(map[i][j] == robots){
        map[i][j] = ' ';
        if(abs(i - y) > abs(j - x)/2){
          if(i - y < 0){
            if(map[i+1][j] == human){
              return 2;
            }else if(map[i+1][j] == exit_loc){
              tempmap[i+1][j] = exit_loc;
              robomap[i+1][j] = robots;
            }else if(map[i+1][j] == destroyed){
              tempmap[i+1][j] = destroyed;
              robomap[i+1][j] = robots;
            }else if(robomap[i+1][j] == robots){
              map[i+1][j] = destroyed;
            }else{
              robomap[i+1][j] = robots;
            }
          }else if(i - y > 0){
            if(map[i-1][j] == human){
              return 2;
            }else if(map[i-1][j] == exit_loc){
              tempmap[i-1][j] = exit_loc;
              robomap[i-1][j] = robots;
            }else if(map[i-1][j] == destroyed){
              tempmap[i-1][j] = destroyed;
              robomap[i-1][j] = robots;
            }else if(robomap[i-1][j] == robots){
              map[i-1][j] = destroyed;
            }else{
              robomap[i-1][j] = robots;
            }
          }
        }else if(abs(i - y) < abs(j - x)/2){
          if(j - x < 0){
            if(map[i][j+2] == human){
              return 2;
            }else if(map[i][j+2] == exit_loc){
              tempmap[i][j+2] = exit_loc;
              robomap[i][j+2] = robots;
            }else if(map[i][j+2] == destroyed){
              tempmap[i][j+2] = destroyed;
              robomap[i][j+2] = robots;
            }else if(robomap[i][j+2] == robots){
              map[i][j+2] = destroyed;
            }else{
              robomap[i][j+2] = robots;
            }
          }else if(j - x > 0){
            if(map[i][j-2] == human){
              return 2;
            }else if(map[i][j-2] == exit_loc){
              tempmap[i][j-2] = exit_loc;
              robomap[i][j-2] = robots;
            }else if(map[i][j-2] == destroyed){
              tempmap[i][j-2] = destroyed;
              robomap[i][j-2] = robots;
            }else if(robomap[i][j-2] == robots){
              map[i][j-2] = destroyed;
            }else{
              robomap[i][j-2] = robots;
            }
          }
        }else{
          if(a == 'N'){
            if(i - y < 0){
              if(map[i+1][j] == human){
                return 2;
              }else if(map[i+1][j] == exit_loc){
                tempmap[i+1][j] = exit_loc;
                robomap[i+1][j] = robots;
              }else if(map[i+1][j] == destroyed){
                tempmap[i+1][j] = destroyed;
                robomap[i+1][j] = robots;
              }else if(robomap[i+1][j] == robots){
                map[i+1][j] = destroyed;
              }else{
                robomap[i+1][j] = robots;
              }
            }else{
              if(map[i-1][j] == human){
                return 2;
              }else if(map[i-1][j] == exit_loc){
                tempmap[i-1][j] = exit_loc;
                robomap[i-1][j] = robots;
              }else if(map[i-1][j] == destroyed){
                tempmap[i-1][j] = destroyed;
                robomap[i-1][j] = robots;
              }else if(robomap[i-1][j] == robots){
                map[i-1][j] = destroyed;
              }else{
                robomap[i-1][j] = robots;
              }
            }
          }else if(a == 'S'){
            if(i - y < 0){
              if(map[i+1][j] == human){
                return 2;
              }else if(map[i+1][j] == exit_loc){
                tempmap[i+1][j] = exit_loc;
                robomap[i+1][j] = robots;
              }else if(map[i+1][j] == destroyed){
                tempmap[i+1][j] = destroyed;
                robomap[i+1][j] = robots;
              }else if(robomap[i+1][j] == robots){
                map[i+1][j] = destroyed;
              }else{
                robomap[i+1][j] = robots;
              }
            }else{
              if(map[i-1][j] == human){
                return 2;
              }else if(map[i-1][j] == exit_loc){
                tempmap[i-1][j] = exit_loc;
                robomap[i-1][j] = robots;
              }else if(map[i-1][j] == destroyed){
                tempmap[i-1][j] = destroyed;
                robomap[i-1][j] = robots;
              }else if(robomap[i-1][j] == robots){
                map[i-1][j] = destroyed;
              }else{
                robomap[i-1][j] = robots;
              }
            }
          }else if(a == 'E'){
            if(j - x < 0){
              if(map[i][j+2] == human){
                return 2;
              }else if(map[i][j+2] == exit_loc){
                tempmap[i][j+2] = exit_loc;
                robomap[i][j+2] = robots;
              }else if(map[i][j+2] == destroyed){
                tempmap[i][j+2] = destroyed;
                robomap[i][j+2] = robots;
              }else if(robomap[i][j+2] == robots){
                map[i][j+2] = destroyed;
              }else{
                robomap[i][j+2] = robots;
              }
            }else{
              if(map[i][j-2] == human){
                return 2;
              }else if(map[i][j-2] == exit_loc){
                tempmap[i][j-2] = exit_loc;
                robomap[i][j-2] = robots;
              }else if(map[i][j-2] == destroyed){
                tempmap[i][j-2] = destroyed;
                robomap[i][j-2] = robots;
              }else if(robomap[i][j-2] == robots){
                map[i][j-2] = destroyed;
              }else{
                robomap[i][j-2] = robots;
              }
            }
          }else{
            if(j - x < 0){
              if(map[i][j+2] == human){
                return 2;
              }else if(map[i][j+2] == exit_loc){
                tempmap[i][j+2] = exit_loc;
                robomap[i][j+2] = robots;
              }else if(map[i][j+2] == destroyed){
                tempmap[i][j+2] = destroyed;
                robomap[i][j+2] = robots;
              }else if(robomap[i][j+2] == robots){
                map[i][j+2] = destroyed;
              }else{
                robomap[i][j+2] = robots;
              }
            }else{
              if(map[i][j-2] == human){
                return 2;
              }else if(map[i][j-2] == exit_loc){
                tempmap[i][j-2] = exit_loc;
                robomap[i][j-2] = robots;
              }else if(map[i][j-2] == destroyed){
                tempmap[i][j-2] = destroyed;
                robomap[i][j-2] = robots;
              }else if(robomap[i][j-2] == robots){
                map[i][j-2] = destroyed;
              }else{
                robomap[i][j-2] = robots;
              }
            }
          }
        }

        if(tempmap[i][j] == exit_loc){
          map[i][j] = exit_loc;
        }else if(tempmap[i][j] == destroyed){
          map[i][j] = destroyed;
        }
      }
    }
  }

  for(int i = 0; i < 8; i++){
    for(int j = 0; j < 17; j++){
      if(robomap[i][j] == robots){
        if(map[i][j] != destroyed){
          map[i][j] = robots;
        }
      }
    }
  }

  clearRoboMaps();

return 1;

}

int main(int argc, char *argv[]) {
  if(argc > 0){
      doNothing();
  }

  int ex = atoi(argv[1]);
  srand(ex);

  char command[10];
  while (true) {
    //exitPosition();

    if(gameover){
      setupMap();
      exitPosition();
      resest();
      clearRoboMaps();
      restart();
      gameover = false;
    }

    printf("\n");
    fgets(command, 10, stdin);
    for(int i = 0; i < 10; i++){
       if(command[i] == '\n'){
          command[i] = '\0';
          break;
       }
     }


     if(string_compare(command, "move", 4) == 0){
       if(strlen(command) > 7 || command[4] != ' '){
         printf("Invalid\n");
       }else{
         int a = move(command[5], command[6]);
         if(a == 0){
           printf("Invalid\n");
         }else if(a == 2){
           printf("\nThe robots got you! We are doomed!\n");
           printf("\n");
           gameover = true;
         }else if(a == 3){
           printf("\nWell done! You survived, but it isn't over!\n");
           printf("\n\n");
           gameover = true;
         }
     }
   }else if(strcmp(command, "exit") == 0){
     break;
   }else if(strcmp(command, "display") == 0){
     printf("\n");
     display();
   }else{
     printf("Invalid\n");
   }

 }

  return 0;
}
