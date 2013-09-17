#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int search(char * find, int size, char ** find_in);
char * getsymname(FILE * f);
void exit_logger(int code);
void resize_matrix(int *** matrix, int o_size, int size);

int main(int argc, char** argz)
{
   int num_symbols = 0;
   unsigned char buffer = 0;
   char ** names;
   int * in_deg;
   int size = 0;

   int ** matrix;

   if(argc < 2) exit_logger(3);

   FILE * fp = fopen(argz[1],"r");

   names = malloc(sizeof(char*)*size);
   in_deg = malloc(sizeof(int)*size);
   matrix = malloc(sizeof(int*)*size);

   /* Generate symbol list and a djacency matrix. */
   while(buffer != 255 && buffer != EOF)
   {
      char* a = getsymname(fp);
      char* b = getsymname(fp);
      if((a != NULL) ^ (b != NULL) ^ (getsymname(fp) != NULL)) exit_logger(1);
      int loc_a = search(a, size, names);
      int loc_b = search(b, size, names);
      int o_size = size;
      if(loc_a == -1)
      {
         if(loc_b == -1)
         {
            size += 2;
            names = realloc(names, sizeof(char*)*size);
            in_deg = realloc(in_deg, sizeof(int)*size);
            names[size-2] = a;
            names[size-1] = b;
            in_deg[size-2] = 0;
            in_deg[size-1] = 1;
            resize_matrix(&matrix, o_size, size);
            matrix[size-2][size-1] += 1;
         }
         else
         {
            size++;
            names = realloc(names, sizeof(char*)*size);
            in_deg = realloc(in_deg, sizeof(int)*size);
            names[size-1] = a;
            in_deg[size-1] = 0;
            in_deg[loc_b]++;
            resize_matrix(&matrix, o_size, size);
            matrix[size-1][loc_b] += 1;
         }
      }
      else if(loc_b == -1)
      {
         size ++;
         names = realloc(names, sizeof(char*)*size);
         in_deg = realloc(in_deg, sizeof(int)*size);
         names[size-1] = b;
         in_deg[size-1] = 1;
         resize_matrix(&matrix, o_size, size);
         matrix[loc_a][size-1] += 1;
      }
      else
      {
         matrix[loc_a][loc_b] += 1;
         in_deg[loc_b]++;
      }

      while((buffer = fgetc(fp)) == '\n');
      fseek(fp, -1, SEEK_CUR);
   }
   int m;
   /* Symbol and graph information.
   
   int n;
   for(m = 0; m < size; m++)
      printf("%2d %2d  %s\n",
            search(names[m],size, names), in_deg[m], names[m]);
   for(m = 0; m < size; m++)
   {
      for(n = 0; n < size; n++)
         printf("%d ", matrix[m][n]);
      printf("\n");
   }
*/
   /* Print lowest in-degree, reduce adjacent in-degrees by one. */
   int all_done = 0;
   int * roots;
   int next;
   while(1)
   {
      int no_deg = 0;
      roots = malloc((sizeof(int)*no_deg));
      all_done = 1;
      for(m = 0; m < size; m++)
      {
         if(in_deg[m] != -1) all_done = 0;
         if(in_deg[m] == 0)
         {
             no_deg ++;
             roots = realloc(roots, sizeof(int)*no_deg);
             roots[no_deg-1] = m;
         }
      }
      if(all_done == 1) break;
      if(no_deg == 0) exit_logger(2);
      next = roots[0];
      for(m = 0; m < no_deg; m++)
      {
          if(strcmp(names[roots[m]],names[next]) < 0)
               next = roots[m];
      }
      in_deg[next] = -1;
      for(m = 0; m < size; m++)
      {
         if(matrix[next][m] != 0)
         {
            in_deg[m] -= matrix[next][m];
         }
      }
      printf("%s\n", names[next]);
   }
   return 0;
}

char * getsymname(FILE * f)
{
   char * symname = NULL;
   char buffer = 0;
   buffer = fgetc(f);
   int symlen = 0;

   while(buffer != '\n' &&
         buffer != ' ' &&
         buffer != '\t' &&
         buffer != '\r' &&
         buffer != 255 &&
         buffer != EOF)
   {
      symname = realloc(symname, ++symlen);
      symname[symlen-1] = buffer;
      buffer = fgetc(f);
   }
   while( buffer == '\t' ||
         buffer == ' ' ||
         buffer == '\r')
      buffer = fgetc(f);

   if(buffer != EOF) fseek(f, -1, SEEK_CUR);
   return symname;
}

void exit_logger(int code)
{
   if(code == 1) { printf("Invalid file!\n"); exit(1); }
   if(code == 2) { printf("Invalid file: dependency loop!\n"); exit(1); }
   if(code == 3) { printf("No file provided!\n"); exit(1); }
   else exit(0);
}

int search (char* a, int size, char** names)
{
   int i;
   for(i = 0; i < size; i++)
   {
      if(strcmp(names[i], a) == 0)
         return i;
   }
   return -1;
}

void resize_matrix(int *** matrix, int o_size, int size)
{
   *matrix = realloc(*matrix, sizeof(int*)*size);
   int j,k;
   for (j = 0; j < size; j++)
   {
      (*matrix)[j] = realloc((*matrix)[j], sizeof(int)*size);
      for (k = 0; k < size; k++)
      {
         if (j < o_size && k < o_size) continue;
         (*matrix)[j][k] = 0;
      }
   }
}
