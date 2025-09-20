#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<readline/readline.h>
#include<readline/history.h>
int main()
{
 char sh_prompt[50];
sprintf(sh_prompt,"%s@%s:~%s ",getenv("USER"),getenv("HOSTNAME"),getenv("PWD"));
 char *cmd;
 system("clear");
 while(1)
 {
   cmd=readline(sh_prompt);
   if (strcmp(cmd,"exit")!=0)
     { if (strcmp(cmd,"help")!=0)
        {system(cmd);
         if (strlen(cmd)>0)
           add_history(cmd);
        }
      else
      { 
           printf("\nls: ls [-a][-l][-i][-s] [file] ...");
           printf("\nps: ps [-a][-l][-x][-e]");
           printf("\ncp: cp [-v][-i][-l][-f]");
           printf("\nmv: mv [-v][-i][-l][-f]");
           printf("\ngrep: grep [-i][-c][-n][-v]");
           printf("\n"); 
      }
    }       
   else
     {  printf("\nGOOD BYE");
	     break;
     }
 }
 printf("\n");
 return 0;
}
