#include<unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>

int process_ps_cmd(int argc,char *argv[])
{
int aflag,xflag,eflag,lflag,i,option; 

aflag=xflag=eflag=lflag=0;   
optind=1;

while((option=getopt(argc,argv,"axel"))!=-1)
 {
  switch(option)
  {
    case 'a':
       aflag=1;
       break;
    case 'x':
      xflag=1;
      break;
    case 'e':
       eflag=1;
        break;
    case 'l':
      lflag=1;
      break;
    default:
      printf("\n Invalid option");
  }
 }
if (!aflag && !xflag && !eflag && !lflag)
   {DIR *drctry;
    struct dirent *entry;
    char bfr[512];

    drctry = opendir("/proc");
    if (drctry == NULL)
    {
        printf("opendir failed");
        return 1;
    }

      printf("%s %s\n", "PID", "CMD");
    while ((entry = readdir(drctry)) != NULL)
    {
        if (entry->d_type == DT_DIR && atoi(entry->d_name) != 0)
        {
            snprintf(bfr, sizeof(bfr), "/proc/%s/cmdline", entry->d_name);
            FILE *fp = fopen(bfr, "r");
            if (fp != NULL)
            {
                fgets(bfr, sizeof(bfr), fp);
                fclose(fp);
                printf("%s %s\n", entry->d_name, bfr);
            }
        }
    }

    closedir(drctry);}
else{
 if (aflag)
 {
   char command[50];
   sprintf(command, "ps -a %d", getuid()); 
   FILE* fp = popen(command, "r"); 
   if (!fp) {
      perror("popen failed");
      exit(EXIT_FAILURE);
   }
  char output[256];
   while (fgets(output, sizeof(output), fp)) 
   {
      printf("%s", output); 
   }
   pclose(fp); 
 }
else if (xflag)
{
char command[50];
   sprintf(command, "ps -x"); 
   FILE* fp = popen(command, "r"); 
   if (!fp) {
      perror("popen failed");
      exit(EXIT_FAILURE);
   }
   char output[256];
   while (fgets(output, sizeof(output), fp)) 
   {
      printf("%s", output); 
   }
   pclose(fp); 
}
else if (eflag)
 {   char command[50];
   sprintf(command, "ps -u %d", getuid()); 
   FILE* fp = popen(command, "r"); 
   if (!fp) {
      perror("popen failed");
      exit(EXIT_FAILURE);
   }
   char output[256];
   while (fgets(output, sizeof(output), fp)) 
   { 
      printf("%s", output); 
   }
   pclose(fp); 
}
else if (lflag)
{
   DIR *proc_dir;
    struct dirent *entry;
    char path[256], line[256], *p;
    FILE *file;
    int pid, ppid, uid, gid, vsz;
    char state, name[256], comm[256];

    proc_dir = opendir("/proc");
    if (proc_dir == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

 printf("%-5s %-5s %-5s %-5s %-5s %-5s %-5s %s\n", "PID", "PPID", "UID", "GID", "VSZ", "STAT", "COMMAND", "NAME");

    while ((entry = readdir(proc_dir)) != NULL) {
                if (*entry->d_name < '0' || *entry->d_name > '9') {

            continue;
        }

        sprintf(path, "/proc/%s/stat", entry->d_name);
        file = fopen(path, "r");
        if (file == NULL) {
         perror("fopen");
            continue;
        }

        if (fscanf(file, "%d %s %c %d %d %d %d", &pid, comm, &state, &ppid, &uid, &gid, &vsz) != 7) {
            fclose(file);
            continue;
        }

        fclose(file);
        sprintf(path, "/proc/%s/cmdline", entry->d_name);
        file = fopen(path, "r");
if (file == NULL) {
            perror("fopen");
            continue;
        }

        if (fgets(line, sizeof(line), file) == NULL) {
            fclose(file);
            continue;
        }

        for (p = line; *p != '\0'; ++p) {
            if (*p == '\0') {
                *p = ' ';
      }
        }

        printf("%-5d %-5d %-5d %-5d %-5d %-5c %-5s %s\n", pid, ppid, uid, gid, vsz, state, line, comm);

        fclose(file);
    }

    closedir(proc_dir);
}

} 




}
