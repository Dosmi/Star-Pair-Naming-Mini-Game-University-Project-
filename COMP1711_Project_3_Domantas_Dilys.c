 #include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define col 60 // number of columns in the frame buffer
#define row 30 // number of rows in the frame buffer

typedef struct star_t{
  int number;                     // star serial number
  char name[50];                 // star name
  int in_named;                 // if the star is in a named pair 1 - true, 0 - false
  int in_pair_with;            // saves the ID of the other star with which is in a pair
  float distance_to_closest;  // distance to star pair
  struct
  {
      double x;            // star x coordinate
      double y;           // star y coordinate
  } Point;
}star_t;

typedef struct pair_t{
  int pair_number;           // the number of a pair
  star_t x;                 // first serial number of the star of the pair
  star_t y;                // second serial number of the star of the pair
  double distance;        // distance (in light years) between the two stars
  char your_name[50];    // name of the user in a pair
  char spouse_name[50]; // name of spouse in a pair
  int named;           // bool type if has been named or not
}pair_t;

// -------------GLOBAL VARIABLES--------------

char draw[col][row];            // frame buffer matrix
int n;                         // number of stars to be generated
star_t star[col*row];         // star_t type variable, stores all stars
pair_t star_pairs[col*row];  // pair_t type variable, stores all pairs
pair_t closest;             // pair_t type variable, stores the closest pair

//--------------------------------------------

void bang (int n)// function that generates n stars
{
  int i,j; // loop iteratives
  for(i=0;i<n;i++)
  {
      star[i].number=i;
      star[i].Point.x=rand()%col;
      star[i].Point.y=rand()%row;
      star[i].in_named=0;

      for(j=0;j<n;j++) // additional loop to avoid stars being generated in the same coordinate
      {
         if(i!=j) // not checking the star itself
          while((star[j].Point.x==star[i].Point.x)&&(star[j].Point.y==star[i].Point.y)) // while we encounter a star with same coordinate, we generate a new one
            {
                star[i].Point.x=rand()%col;
                star[i].Point.y=rand()%row;
                j=0; // reset the search loop
            }

      }
  }
}

pair_t ClosestPair (int number_stars) // function to find two nearest stars
{

    float MIN=99999.0; // minimum distance
    int i,j;          // loop iteratives
    int pair=0;      // pair counter
    float distance; // variable to save the distance
    for (i=0;i<number_stars;i++)
    {
        for(j=i+1;j<number_stars;j++)
        {
            if ((star[i].in_named==0)&&(star[j].in_named==0))
            {
                distance=sqrt(pow((star[i].Point.x-star[j].Point.x),2)+(pow((star[i].Point.y-star[j].Point.y),2)));
                if(distance<MIN)
                {
                    MIN=distance;
                    closest.x.number=star[i].number;
                    closest.y.number=star[j].number;
                }

                else if (star[i].in_named==1)
                {
                    continue;
                }

            }
        }

        if (star[i].in_named==0)
        {
          star[i].distance_to_closest=MIN;
          star[i].in_pair_with=closest.x.number;
        }

    }

    printf("The closest pair of stars are no. %i and %i.\n", closest.x.number, closest.y.number);
    printf("They are %lf light years apart.\n", MIN);

}

void Plot (int x, int y, char c) // store c in row y and column x of the frame buffer
{
  draw[x][y]=c;
}
void Clear ()
// clear (erase) the whole frame buffer by filling
//it with white space (ASCII 32)
{
  int i, j;
  for (i=0;i<row;i++)
  {
      for (j=0;j<col;j++)
      {
          draw[j][i]=' '; //clear (erase) the whole frame buffer by filling it with white space (ASCII 32)
      }
  }

}


void WriteAt (int x, int y, char str)
// copy the string str to the frame buffer starting at
// position x, y
{
  draw[x][y]=str; //copy the string to the frame buffer
}
void Refresh ()
//clear the terminal and print the entire frame buffer
{
  //system ("clear"); // clearing the terminal
  //system ("cls"); // clearing the Windows console
  int i,j;
  for (i=0;i<row;i++)
  {
      for(j=0;j<col;j++)
      {
          printf("%c", draw[j][i]);
      }
      printf("\n");
  }
  Clear();
}
void Remove_Pairs()
{
    int i;
    for(i=0;i<col*row;i++)
    {
        star_pairs[i].named=0;
        strcpy(star_pairs[i].your_name,  "\0");
        strcpy(star_pairs[i].spouse_name,"\0");
    }
}

int main()
{
  time_t t;
  srand((unsigned) time(&t));

  char command[10];                                  // user input command
  char choice='\0';                                 // user choice
  int i=0,j,k;                                     // loop iteratives
  char pair_name[50];                             // user endered star pair name
  char your_name_[50], spouse_name_[50];         // name of the user
  char your_name [50]="", spouse_name [50]="";  // name of spouse
  char input_your_name[50]="";                 // the input name in command show
  char star_pair_name[50]="";                 // the pair name used in command show
  char full_pair_name[50]="";                // full star pair name
  float begins, ends;

  Remove_Pairs();                         // removes the pairs before beginning the program

  while (command != "quit")
  {
      printf(">>> ");
      scanf(" %s",&command);

      if (strcmp(command,"bang")==0) //command to create 'n' stars
      {
        scanf("%i", &n);
        bang(n);
        printf("The Big Bang has occured!\n\n");
        continue;
      }

      if (strcmp(command,"list")==0) //command to print the list with star coordinates
      {
        for(i=0;i<n;i++)
        {
          printf("Star %i\n coords: (%lf,%lf)\n",star[i].number, star[i].Point.x, star[i].Point.y);
          if(star[i].in_named==1) printf(" name: %s\n",star[i].name);
        }
        continue;
      }

      if (strcmp(command,"draw")==0)
      {
        Clear();

          for (i=0;i<n;i++)
          {

              if (star[i].in_named==0)
              {
                Plot((int)star[i].Point.x,(int)star[i].Point.y,'.');
              }
              else if (star[i].in_named==1)
              {
                Plot((int)star[i].Point.x,(int)star[i].Point.y,'*');
              }
          }

          Refresh();
          continue;
      }

      if (strcmp(command,"name")==0) //command to find the closest pair of stars that hasnt been named
      {
            int pair=0;
            int named_stars=0;
            for(k=0;k<n;k++)
            {
                if (star[k].in_named==1)
                    named_stars++;
            }

                if (n-named_stars==1)
                {
                    printf("There is only one star left! Cannot be paired.\n\n");
                    continue;
                }

                else if (n-named_stars==0)
                {
                    printf("Sorry no pairs were found!\nWish you a better luck in the next universe\n\n");
                    continue;
                }

                else if (n-named_stars>1)
                {
                  ClosestPair(n);

                  printf("Would you like to name this pair (y/n)? ");
                  scanf(" %c", &choice);
                  getchar(); // clean input buffer

                  int x_in=closest.x.number;
                  int y_in=closest.y.number;
                  printf("\n");

                  if (choice =='y')
                  {

                      printf("Enter your full name: ");
                      fgets(your_name,50,stdin);        // gets 50 characters and assigns it to variable 'your_name'
                      for (i=0;i<50;i++)               // clearing the last character, which is the \n (endline) symbol
                      {
                          if(your_name[i]=='\n')
                          {
                              your_name[i]='\0';
                          }
                      }

                      printf("Enter your spouse full name: ");

                      fgets(spouse_name, 50, stdin);
                      for (i=0;i<50;i++)               // clearing the last character, which is the \n (endline) symbol
                      {
                          if(spouse_name[i]=='\n')
                          {
                              spouse_name[i]='\0';
                          }
                      }

                      strcpy(closest.your_name,your_name);      // copies the string 'your_name' to 'closest.your_name'
                      strcpy(closest.spouse_name,spouse_name); // copies the string 'spouse_name' to 'closest.spouse_name'

                      strcpy(star_pairs[pair].your_name,your_name);        // copies the string 'your_name' to 'star_pairs[pair].your_name'
                      strcpy(star_pairs[pair].spouse_name,spouse_name);   // copies the string 'spouse_name' to 'star_pairs[pair].spouse_name'

                      star_pairs[pair].named=1;             // setting the n-th pair to named
                      closest.named=1;                     // the closest pair is now named
                      star[closest.x.number].in_named=1;  // the star, which is paired, is now also named

                      strcpy(star[closest.x.number].name,your_name);            // renaming closest star
                      star[closest.x.number].in_pair_with=closest.y.number;    // star is in pair with closest star number
                     // star[closest.y.number].in_pair_with;
                      strcpy(star[closest.y.number].name,spouse_name);        // renaming the other star in pair is named to spouse name

                      star[closest.y.number].in_named=1;                     // setting other star in pair to named
                      printf("Congratulations, a pair of stars has been named after you and your spouse\n\n");
                      continue;
                  }

                }
      }

        if (strcmp(command,"pairs")==0) //command to print the list with star coordinates
        {
          int i;            // loop iterative
          int pair=0;      // pair counter
          for(i=0;i<n;i++)
          {
            if (star[i].in_named==1)
            {
                if(star[i].in_pair_with>i)
                {
                  printf("Pair %i:\n", pair);
                  printf("distance: %f\n", star[i].distance_to_closest);
                  printf("star %i\n",star[i].number);
                  printf("name: %s\n",star[i].name);
                  printf("coords: (%f,%f)\n",star[i].Point.x,star[i].Point.y);
                  printf("star %i\n",star[star[i].in_pair_with].number);
                  printf("name: %s\n",star[star[i].in_pair_with].name);
                  printf("coords: (%f,%f)\n",star[star[i].in_pair_with].Point.x,star[star[i].in_pair_with].Point.y);

                  printf("*********************************\n\n");
                  pair++;
                }
            }
          }
          continue;
        }

      if (strcmp(command,"show")==0) //command to print the list with star coordinates
        {
            Clear();
            int found=0; // variable that keeps track if the input name was matched with an existing star name, value 1 or 0

            printf("Enter your full name: ");
            getchar();                          // cleaning the input buffer
            fgets(input_your_name, 50, stdin); // getting first 50 characters and stores in variable
              for (i=0;i<50;i++)              // clearing the last character, which is the \n (endline) symbol
              {
                  if(input_your_name[i]=='\n')
                  {
                      input_your_name[i]='\0';
                  }
              }
              printf("\n");

              for (i=0;i<n;i++)
              {
                  if (strcmp(input_your_name,star[i].name)==0)
                    found=1; // found a star with the same name as input
              }

              if (!found) // if the name was not found
              {
                  printf("No star is named \"%s\"\n\n",input_your_name);
                  continue;
              }
          // ---------- filling the input buffer with stars accordingly if they have been named or not --------
          for (i=0;i<n;i++)
          {
              if (star[i].in_named==0)
              {
                Plot((int)star[i].Point.x,(int)star[i].Point.y,'.');
              }
              else if (star[i].in_named==1)
              {
                Plot((int)star[i].Point.x,(int)star[i].Point.y,'*');
              }
          }
          //-------------------------------------------------------------

          for(i=0;i<n;i++) // checking if the star is named and the name matches the input name, then save the star name to star_pair_name
          {
              if ((star[i].in_named==1)&&(strcmp(star[i].name,input_your_name)==0))
              {
                strcpy(star_pair_name,star[star[i].in_pair_with].name);
              }
          }

         // using 'star_pair_name' from now on:

          for(i=0;i<n;i++)
          {
              if ((star[i].in_named==1)&&(strcmp(star[star[i].in_pair_with].name,star_pair_name)==0))
              {
                //WriteAt(star[i].Point.x,star[i].Point.y,'*');
                // converting a pair name to this format: 'name_1 & name_2' and calling it a 'full_pair_name'
                strcpy(full_pair_name,strcat(input_your_name," & "));         // concatinating 'name_1' with '%'
                strcpy(full_pair_name,strcat(full_pair_name,star_pair_name)); // concatinating 'name_1 &' with 'name_2'

                // Checking if the name is out of bounds:
                int len=strlen(full_pair_name);  // getting a length of the full_pair_name
                begins=star[i].Point.x;         // this is the row coordinate where the name writing begins (under star's coordinate)
                ends=star[i].Point.x+len;      // this is where the name writing ends (begin + length of full_pair_name)

                for(j=0;j<len;j++)  // repeating until length of full_pair_name reached
                {
                    // ----------------------- Case 1: if paired star is below the current star: ----------------------
                    if(star[star[i].in_pair_with].Point.y < star[i].Point.y)
                    {
                        if ((begins>=0)&&(ends<=col)) // this is the normal case, when the word is within frame buffer limits
                        {
                            if(star[i].Point.y+1>=row) // checking if the star is at the bottom, then write the name above it
                                 WriteAt(star[i].Point.x+j-(int)len/2,star[i].Point.y-1,full_pair_name[j]);
                            else
                                 WriteAt(star[i].Point.x+j-(int)len/2,star[i].Point.y+1,full_pair_name[j]);
                        }

                        else if(begins<=0) // checks if the text is out of bounds (the left side of the frame buffer)
                        {
                            if(star[i].Point.y+1>=row)
                                 WriteAt(star[i].Point.x+j,star[i].Point.y-1,full_pair_name[j]);
                            else
                                 WriteAt(star[i].Point.x+j,star[i].Point.y+1,full_pair_name[j]);
                        }

                        else if(ends>=col) // checks if the text is out of bounds (the right side of the frame buffer)
                        {
                            if(star[i].Point.y+1>=row)
                                 WriteAt(star[i].Point.x+j-len,star[i].Point.y-1,full_pair_name[j]);
                            else
                                 WriteAt(star[i].Point.x+j-len,star[i].Point.y+1,full_pair_name[j]);
                        }

                    }
                    // --------------------- End of Case 1 ------------------------

                    // -------------------- Case 2: if the paired star is in the same row ------------------------
                    else if(star[star[i].in_pair_with].Point.y==star[i].Point.y)
                    {
                        if((begins>=0)&&(ends<=col))
                        {
                            if(star[i].Point.y+1>=row) // checks if it is at the bottom, and then writes the name on top
                                WriteAt(star[i].Point.x+j-(int)len/2,star[i].Point.y-1,full_pair_name[j]);
                            else
                                WriteAt(star[i].Point.x+j-(int)len/2,star[i].Point.y+1,full_pair_name[j]);
                        }

                        else if (ends>=col)
                        {
                            if(star[i].Point.y+1>=row) // checks if it is at the bottom, and then writes the name on top
                                WriteAt(star[i].Point.x+j-len,star[i].Point.y-1,full_pair_name[j]);
                            else
                                WriteAt(star[i].Point.x+j-len,star[i].Point.y+1,full_pair_name[j]);
                        }

                        else
                        {
                            if(star[i].Point.y+1>=row) // checks if it is at the bottom, and then writes the name on top
                                WriteAt(star[i].Point.x+j,star[i].Point.y-1,full_pair_name[j]);

                            else if ((begins+len>=col)&&(begins>len))
                                WriteAt(star[i].Point.x+j-len,star[i].Point.y+1,full_pair_name[j]);
                            else
                                WriteAt(star[i].Point.x+j,star[i].Point.y+1,full_pair_name[j]);
                        }
                    }
                    //---------------------- End of Case 2 ----------------------------

                    // ------------------ Case 3: if paired star is above the current star -----------------
                    else
                    {
                        if  ((begins>=0)&&(ends<=col)) // this is the normal case, when the word is within frame buffer limits
                            WriteAt(star[star[i].in_pair_with].Point.x+j-(int)len/2,star[star[i].in_pair_with].Point.y+1,full_pair_name[j]); // len/2 aligns text in the middle

                        else if ((begins>=0)&&(ends>=col)) // if it begins within the frame buffer, but ends after the right limit
                            WriteAt(star[star[i].in_pair_with].Point.x+j-(int)len,star[star[i].in_pair_with].Point.y+1,full_pair_name[j]);

                        else if (begins<=0) // if the word begins before the left frame buffer limit
                            WriteAt(star[star[i].in_pair_with].Point.x+j,star[star[i].in_pair_with].Point.y+1,full_pair_name[j]);
                    }
                    // ------------------- End of Case 3 -----------------------
                }
              }

            }

            Refresh();
            Clear();
            continue;
        }

        if (strcmp(command,"save")==0) //command to save star related structures and variables
        {
          FILE* ptr_to_file = fopen("save.bin","wb"); // pointer variable to a save file, named save.bin
          for (i=0;i<n;i++)
          {
            fwrite(&star,sizeof(star),1,ptr_to_file);                  // saving the star structure
            fwrite(&star_pairs,sizeof(star_pairs),1,ptr_to_file);     // saving the star pairs structure
            fwrite(&n,sizeof(int),1,ptr_to_file);                    // saving how many stars were generated
          }
          printf("Thanks, you have saved the universe!\n\n");
          continue;
        }

        if (strcmp(command,"load")==0) //command to load star related structures and variables
        {
          FILE* ptr_to_file = fopen("save.bin","rb"); // pointer variable to a save file, named save.bin
          if (ptr_to_file!=NULL)
          {
            fread(&star,sizeof(star),1,ptr_to_file);               // loading the star structure
            fread(&star_pairs,sizeof(star_pairs),1,ptr_to_file);  // loading the star pairs structure
            fread(&n,sizeof(int),1,ptr_to_file);                 // loading how many stars were generated
          }

          printf("Congratulations, your saved universe was restored!\n\n");
          continue;
        }

        // ---------------------------------- TIER 2 - PROJECT EXTENSION  ------------------------------------

        if (strcmp(command,"tier2")==0)
        {
            int star_number, planet_number;
            int universe_age=0;
            int remaining_stars, remaining_planets;
            int digits; // for planet numbering, how many digits are needed to number the planets
            char string_num[11];  // variable to store a converted integer planet number

            int guess_number, guess_years;

            typedef struct t2_star_t{        // tier 2 star structure
              int life;                     // how long the star lives
              int shooting_direction;      // what direction star shoots when it dies
              int range;                  // the planet is destroyed if it is in this range from the shooting star
              int exploded;              // if the star has exploded
              struct
              {
                  double x;            // star x coordinate
                  double y;           // star y coordinate
              } Point;
            }t2_star_t;

            typedef struct t2_planet_t{
                double x;              // planet x and y coordinates
                double y;
                int destroyed;       // if the planet has been destroyed
                int chosen;         // if the bet was made on this planet
            }t2_planet_t;

            typedef struct destroyed_list_t{ // structure that stores data for a destroyed planet
                int number;
                int survived_years;
            }destroyed_list_t;


            destroyed_list_t destroyed_planets[row*col]; // an array of destroyed planets

            t2_star_t   t2star[col*row]; // array of stars
            t2_planet_t planet[col*row]; // array of planets

            // ---------------- POINT SYSTEM ------------------
            int exact_guessed_years=1000,
                survived_longer_than_expected=400,
                survived=400,
                last_standing=500,
                total;
            //-------------------------------------------------
            int number_from_last;

            printf("Enter star number (optimal number 10-40): ");
            scanf("%i", &star_number);
            printf("\nEnter planet number: ");
            scanf("%i", &planet_number);

            remaining_stars   = star_number;
            remaining_planets = planet_number;

            int precision; // used to calculate the score

            int i,j, m,k; // loop iteratives
            for(i=0;i<star_number;i++)
            {
                // generating star data randomly
                t2star[i].life=rand()%100;
                t2star[i].life=t2star[i].life*10;
                t2star[i].shooting_direction=rand()%4;
                t2star[i].Point.x=rand()%col;
                t2star[i].Point.y=rand()%row;
                t2star[i].range=rand()%4;
                if (t2star[i].range==3) // if the range of explosion is 3, we generate the range again to lower the chance of such a powerful star being created, but it can still be created!
                    t2star[i].range=rand()%4;

                for(j=0;j<star_number;j++) // additional loop to avoid stars with same lifespan generated
                  {
                     if(i!=j) // not checking the star itself
                      while((t2star[j].life==t2star[i].life)) // while we encounter a star with same coordinate, we generate a new one
                        {
                            t2star[i].life=rand()%100;
                            t2star[i].life=t2star[i].life*10;
                            j=0; // reset the search loop
                        }
                  }

                  for(j=0;j<star_number;j++) // additional loop to avoid stars being generated in the same coordinate
                  {
                     if(i!=j) // not checking the star itself
                      while((t2star[j].Point.x==t2star[i].Point.x)&&(t2star[j].Point.y==t2star[i].Point.y)) // while we encounter a star with same coordinate, we generate a new one
                        {
                            t2star[i].Point.x=rand()%col;
                            t2star[i].Point.y=rand()%row;
                            j=0; // reset the search loop
                        }
                  }
            }

            for(i=0;i<planet_number;i++)
            {
                planet[i].x=rand()%col;
                planet[i].y=rand()%row;

                  for(j=0;j<n;j++) // additional loop to avoid stars being generated in the same coordinate
                  {
                     if(i!=j) // not checking the planet itself
                      while(((planet[j].x==planet[i].x)&&(planet[j].y==planet[i].y))||((t2star[j].Point.x==planet[i].x)&&(t2star[j].Point.y==planet[i].y))) // while we encounter a star with same coordinate, we generate a new one
                        {
                            t2star[i].Point.x=rand()%col;
                            t2star[i].Point.y=rand()%row;
                            j=0; // reset the search loop
                        }
                  }

            }

            Clear();

              for (i=0;i<star_number;i++)
              {
                Plot((int)t2star[i].Point.x,(int)t2star[i].Point.y,'*');
              }
              for (i=0;i<planet_number;i++)
              {
                printf("%i\n",i);
                planet[i].destroyed=0; // setting all plannets to NOT be destroyed in the beginning
                Plot((int)planet[i].x,(int)planet[i].y,'o');

                snprintf(string_num, 11, "%d", i+1); // prints a string integer below the planet
                digits=strlen(string_num);
                for(j=0;j<digits;j++)
                {
                    WriteAt(planet[i].x+j,planet[i].y+1,string_num[j]);
                }
              }
              Refresh();
                  printf("Try to guess, which planet will survive the longest (enter its number from 1 to %i): ", planet_number);
                  scanf("%i",&guess_number);
                  if ((guess_number>planet_number)||(guess_years<=0))
                  {
                    while ((guess_number>planet_number)||(guess_number<=0)){
                      printf("\nInvalid input, it has to be a number between 1 and %i, try again: ", planet_number);
                      scanf("%i",&guess_number);
                    }

                  }
                  planet[guess_number-1].chosen=1;
                  printf("\nTry to guess, how many million years it will survive (0-1000 million years): ");
                  scanf("%i",&guess_years);
                  if ((guess_years>1000)||(guess_years<0))
                  {
                    while ((guess_years>1000)||(guess_years<0)){
                      printf("\nInvalid input, it has to be a number between 0 and 1000, try again: ");
                      scanf("%i",&guess_years);
                    }

                  }

            for (universe_age;universe_age<=1000;universe_age+=10)
            {
                int at_least_one_exploded=0; // setting this to zero, because we do not need to print out the frame buffer if no star has exploded
                for (i=0;i<star_number;i++)
                {
                    if(t2star[i].life==universe_age)
                    {
                        remaining_stars--;

                        if(t2star[i].shooting_direction==0) // the star will shoot to the left
                        {
                            at_least_one_exploded=1;
                            printf("Star marked 'X' shooting left\n\n");
                            for (k=t2star[i].Point.y-(t2star[i].range);k<=t2star[i].Point.y+(t2star[i].range);k++)
                            {
                                for (j=0;j<=t2star[i].Point.x+(t2star[i].range);j++)
                                {
                                    WriteAt(j,k,'-');

                                    t2star[i].exploded=2; // setting it to 2 to print a different character since it exploded this turn
                                    for (m=0;m<planet_number;m++)
                                    {
                                        if ((planet[m].x==j)&&(planet[m].y==k))
                                        {
                                            //printf("Universe update: planet %i destroyed.\n", m+1);this was removed to keep the frame buffer in the same place
                                            planet[m].destroyed=2;
                                            destroyed_planets[remaining_planets].number=m;
                                            destroyed_planets[remaining_planets].survived_years=universe_age;
                                            remaining_planets--;

                                        }
                                    }
                                }
                            }

                        }

                        if(t2star[i].shooting_direction==1) // the star will shoot to the right
                        {
                            at_least_one_exploded=1;
                            printf("Star marked 'X' shooting right\n\n");
                            for (k=t2star[i].Point.y-(t2star[i].range);k<=t2star[i].Point.y+(t2star[i].range);k++)
                            {
                                for (j=t2star[i].Point.x-(t2star[i].range);j<=col;j++)
                                {
                                    WriteAt(j,k,'-');
                                    t2star[i].exploded=2; // setting it to 2 to print a different character since it exploded this turn
                                    for (m=0;m<planet_number;m++)
                                    {
                                        if ((planet[m].x==j)&&(planet[m].y==k))
                                        {
                                            //printf("Universe update: planet %i destroyed.\n", m+1); this was removed to keep the frame buffer in the same place
                                            planet[m].destroyed=2;
                                            destroyed_planets[remaining_planets].number=m;
                                            destroyed_planets[remaining_planets].survived_years=universe_age;
                                            remaining_planets--;

                                        }
                                    }
                                }
                            }

                        }

                        if(t2star[i].shooting_direction==2) // the star will shoot up
                        {
                            at_least_one_exploded=1;
                            printf("Star marked 'X' shooting up\n\n");
                            for (k=0;k<=t2star[i].Point.y+(t2star[i].range);k++)
                            {
                                for (j=t2star[i].Point.x-(t2star[i].range);j<=t2star[i].Point.x+(t2star[i].range);j++)
                                {
                                    WriteAt(j,k,'|');
                                    t2star[i].exploded=2; // setting it to 2 to print a different character since it exploded this turn
                                    for (m=0;m<planet_number;m++)
                                    {
                                        if ((planet[m].x==j)&&(planet[m].y==k))
                                        {
                                            //printf("Universe update: planet %i destroyed.\n", m+1);this was removed to keep the frame buffer in the same place
                                            planet[m].destroyed=2;
                                            destroyed_planets[remaining_planets].number=m;
                                            destroyed_planets[remaining_planets].survived_years=universe_age;
                                            remaining_planets--;

                                        }
                                    }
                                }
                            }

                        }

                        if(t2star[i].shooting_direction==3) // the star will shoot down
                        {
                            at_least_one_exploded=1;
                            printf("Star marked 'X' shooting down\n\n");
                            for (k=t2star[i].Point.y-(t2star[i].range);k<row;k++)
                            {
                                for (j=t2star[i].Point.x-(t2star[i].range);j<=t2star[i].Point.x+(t2star[i].range);j++)
                                {
                                    WriteAt(j,k,'|');
                                    t2star[i].exploded=2; // setting it to 2 to print a different character since it exploded this turn
                                    for (m=0;m<planet_number;m++)
                                    {
                                        if ((planet[m].x==j)&&(planet[m].y==k))
                                        {
                                            planet[m].destroyed=2;
                                            destroyed_planets[remaining_planets].number=m;
                                            destroyed_planets[remaining_planets].survived_years=universe_age;
                                            remaining_planets--;

                                        }
                                    }
                                }
                            }

                        }

                    }
                    if (t2star[i].exploded==0)
                        Plot((int)t2star[i].Point.x,(int)t2star[i].Point.y,'*');

                    else if (t2star[i].exploded==1)
                        Plot((int)t2star[i].Point.x,(int)t2star[i].Point.y,'.');

                    else if (t2star[i].exploded==2) // printing the star which JUST exploded as 'X'
                    {
                        Plot((int)t2star[i].Point.x,(int)t2star[i].Point.y,'X');
                        t2star[i].exploded=1;
                    }

                }

                for (m=0;m<planet_number;m++)
                {
                  if ((planet[m].chosen==1)&&(!planet[m].destroyed)) Plot((int)planet[m].x,(int)planet[m].y,'+');
                  else if ((planet[m].chosen==1)&&(planet[m].destroyed)) Plot((int)planet[m].x,(int)planet[m].y,'~');
                  else{
                    if(planet[m].destroyed==0)
                        Plot((int)planet[m].x,(int)planet[m].y,'o');
                    else if (planet[m].destroyed==1)
                        Plot((int)planet[m].x,(int)planet[m].y,'@');
                    else if (planet[m].destroyed==2) // if a planet was just destroyed this turn, we print 'D'
                    {
                        Plot((int)planet[m].x,(int)planet[m].y,'D');
                        planet[m].destroyed=1;
                    }
                  }

                    snprintf(string_num, 11, "%d", m+1);
                    digits=strlen(string_num);

                    for(j=0;j<digits;j++)
                    {
                        WriteAt(planet[m].x+j,planet[m].y+1,string_num[j]);
                    }
                }

                if (at_least_one_exploded) // if a star exploded, we display the events
                {
                    Refresh();
                    printf("%i million years have passed\n", universe_age);
                    if (remaining_stars>1)
                        printf("\nStar exploded.\n%i stars remain\n\n",remaining_stars);
                    else if (remaining_stars==1)
                        printf("\nStar exploded.\nOne star remains\n\n");
                    else if (remaining_stars==0)
                        printf("\nStar exploded.\nThere are no more stars in the universe!\n\n");

                    if (remaining_planets>1)
                        printf("\n%i planets remain\n\n",remaining_planets);
                    else if (remaining_planets==1)
                        printf("\nOne planet remains\n\n");
                    else if (remaining_planets==0)
                        printf("\nThere are no more planets in the universe!\n\n");

                    for (i = 1 ; i <= 32767 ; i++ ) // A huge loop to delay the display screen, it makes the user able to follow the events
                        for ( j = 1 ; j <= 32767 ; j++ )
                        {}

                    system("clear");
                }


            }

                if(remaining_stars==0) // printing the final screen
                {
                  universe_age=1000;
                    printf("\n|======================== OVERVIEW ============================|\n");
                    Refresh();
                    printf("\n|======================== oooooooo ============================|\n");
                    printf("This is how the universe currently looks like after all the explosions.\n");
                    printf("Your bet was on planet #%i\n", guess_number);
                    //printf("%i planets survived\n",remaining_planets);
                    if(!planet[guess_number-1].destroyed)
                    {
                        if (remaining_planets==1)
                        {
                            printf("Your planet was the only one which survived!\n");
                            if(guess_years==1000)
                            {
                              printf("Total points: \n  \
                                     Last standing planet: %i\n \
                                     Years guessed: %i\n \
                                     ",last_standing,exact_guessed_years);
                            }


                        }
                        else if (remaining_planets>1)
                        {
                            printf("Your planet survived together with these planets:\n");
                            for (m=0;m<planet_number;m++)
                            {
                                if (planet[m].destroyed==0)
                                {
                                    printf("Nr. %i\n",m+1);
                                }

                            }
                            if (guess_years==1000)
                              printf("Total score: %i\n",exact_guessed_years);
                            else printf("Total score: %i\n", survived/remaining_planets);


                        }
                    }
                    if(planet[guess_number-1].destroyed)
                    {
                      if(destroyed_planets[1].number==m)
                      {
                        printf("Congratulations, the planet you bet on, survived the longest,\n");

                        if (destroyed_planets[1].survived_years>guess_years)
                        {
                          printf("and it survived longer than expected!\n");
                          printf("Total points: \n \
                                 Last standind planet: %i",last_standing );
                        }
                      }
                      else
                      {
                        for (m=0;m<planet_number;m++)
                        {
                          if (destroyed_planets[m].number==guess_number-1)
                              number_from_last=m;
                        }
                        if (destroyed_planets[number_from_last].survived_years<guess_years)
                        {
                          printf("You lost, the planet #%i did not survive %i million years\nit survived for %i million years\n",guess_number,guess_years,destroyed_planets[number_from_last].survived_years);
                          printf("Total points: 0\n");
                        }
                        if (destroyed_planets[number_from_last].survived_years>guess_years)
                        {
                          printf("You won, the planet #%i survived longer than %i million years\nit survived for %i million years\n",guess_number,guess_years,destroyed_planets[number_from_last].survived_years);
                          if (guess_years<500)
                          {
                            precision=destroyed_planets[number_from_last].survived_years-guess_years;
                            printf("Total points: %i\n",survived-precision);
                            if (survived-precision<=0)
                              printf("However, the guess was not accurate, hence the score is negative\n");
                          }
                          else if (guess_years>=500)
                            printf("Total points: %i\n", survived-100);
                        }
                        if (destroyed_planets[number_from_last].survived_years==guess_years)
                        {
                          printf("Impressive guess! The planet #%i did survive for %i million years\n",guess_number,guess_years);
                          printf("Total points: %i\n",exact_guessed_years);
                        }
                      }
                    }
                }
              continue;
          }




      if (strcmp(command,"quit")==0) break; // command to quit the program

      else printf("Invalid input.\n\n");
  }
  return 0;
}
