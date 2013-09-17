#include <stdio.h>
#include <stdlib.h>
#define HEADER_0 0x79
#define HEADER_1 0x62

char* bbytes[16] = {"halt","nop","rrmovl","irmovl",
		  "rmmovl","mrmovl","addl","jmp",
		  "call","ret","pushl","popl",
		  "invalid","invalid","invalid","invalid"};

char* getop(int opcode)
{
		  if((opcode & 0x0F) == 0)
					 return bbytes[(opcode & 0xF0) >> 4];

		  switch ((opcode & 0xF0) >> 4) {
					 case 2: switch (opcode & 0x0F) {
										  case 1: return "cmovle";
										  case 2: return "cmovl";
										  case 3: return "cmove";
										  case 4: return "cmovne";
										  case 5: return "cmovge";
										  case 6: return "cmovg";
										  default: return bbytes[12]; }
					 case 6: switch (opcode & 0x0F) {
										  case 1: return "subl";
										  case 2: return "andl";
										  case 3: return "xorl";
										  default: return bbytes[12]; }
					 case 7: switch (opcode & 0x0F) {
										  case 1: return "jle";
										  case 2: return "jl";
										  case 3: return "je";
										  case 4: return "jne";
										  case 5: return "jge";
										  case 6: return "jg";
										  default: return bbytes[12]; }
					 default: return "invalid";
		  }
		  return "invalid";
}

int main(int argc, char** argz)
{
		  static int count = 0;
		  static int start = 0;
		  static int max_size = 0;

		  unsigned char * buffer;
		  unsigned char * mp;
		  mp = malloc(max_size);

		  FILE * fp = fopen(argz[1],"rb");
		  //FILE * fp = fopen("/u/jiyinjie/CS429-hw3/prog9.yb","rb");
		  fread(buffer, 1, 2, fp);
		  int i;
		  //  printf("%X %X\n",buffer[0],buffer[1]);
		  //  printf("\n");
		  if (buffer[0] != HEADER_0 || buffer[1] != HEADER_1)
		  {
					 printf("This is not a y86 file.\n");
					 return 1;
					 // Error code 1 means incorrect file format.
		  }

		  while (fread(buffer, 1, 4, fp) == 4)
		  {
					 //     printf("\nNext block:\n");
					 count = buffer[3] + buffer[2]*256;
					 //     printf("%d\n",count);
					 start = buffer[1] + buffer[0]*256;
					 int temp = count+start;
					 max_size = (temp>max_size) ? temp : max_size;
					 mp = realloc(mp,sizeof(char*)*max_size);

					 static int check_count = 0;
					 check_count = fread(buffer, 1, count, fp);
					 if (! (count == check_count))
					 {
								printf("Corrupt object file!\n");
								return 2;
					 }

					 for (i = 0; i < count; i++)
								mp[i+start] = buffer[i];
		  }

		  int *tst = malloc(0);
		  for (i = 0; i < max_size; i++)
		  {
					 tst = (int*) &mp[i];
					 printf("%04X\t",i);
					 printf("%02X\t",mp[i]);
					 printf("%08X\t",*tst);
					 printf("%10d\t",*tst);
					 printf("%s\n",getop(mp[i]));
				//	printf("%04X\t%02X\t%08X\t%10d\t%s\n", i, mp[i], *tst, *tst, getop(mp[i]));
		  }
		  fclose(fp);
		  return 0;
}
