#include<unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>

void dir(char *dir_path)
{
    DIR *drctry;
    struct dirent *ent;

    drctry = opendir(dir_path);
    if (drctry == NULL)
    {
        printf("Error in opening the file");
    }

    while ((ent = readdir(drctry)) != NULL)
    {
        if(ent->d_name[0]=='.')
       {
        continue;
       }
        printf("%s\n", ent->d_name);
    }

    closedir(drctry);
}
void prnt_dir(char *dir_path)
{
    DIR *drctry;
    struct dirent *ent;
    char *contents[1000];
    int i,num_contents = 0;

    drctry = opendir(dir_path);
    if (drctry == NULL)
    {
      printf("Error in opening directory");
    }

    while ((ent = readdir(drctry)) != NULL)
    {

        if (ent->d_name[0]== '.')
        {
         continue;
        }

        contents[num_contents] = strdup(ent->d_name);
        num_contents++;
    }

    closedir(drctry);

    for ( i = num_contents - 1; i >= 0; i--)
   {
        printf("%s\n", contents[i]);
    }
}

void prnt_md(mode_t mode)
{
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((S_ISDIR(mode)) ? "d" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IRGRP) ? "r" : "-");
}

void print_dir_contents(char *dir_path)
{
    DIR *dir;
    struct dirent *entry;

    dir = opendir(dir_path);
    if (dir == NULL)
    {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((entry = readdir(dir)) != NULL)
    {
        printf("%s\n", entry->d_name);
    }

    closedir(dir);
}

void fl_info(struct dirent *ent)
{
    struct stat fl_stat;
    
    if (stat(ent->d_name, &fl_stat) == -1)
    {
      printf("Error in accessing information");
    }
    prnt_md(fl_stat.st_mode);

    printf(" %ld", (long)fl_stat.st_nlink);

    struct passwd *pwd = getpwuid(fl_stat.st_uid);
    printf(" %s", pwd->pw_name);

    struct group *grp = getgrgid(fl_stat.st_gid);
    printf(" %s", grp->gr_name);
    printf(" %d", fl_stat.st_size);

    char time_str[20];
    strftime(time_str, sizeof(time_str), "%b %d %H:%M", localtime(&fl_stat.st_mtime));

    printf(" %s", time_str);
    printf(" %s\n", ent->d_name);
}

void fl_info_files(struct dirent *ent,int p_optind,int p_argc, char *p_argv[])
{   int i;
    struct stat fl_stat;
  for(i=p_optind;i<p_argc;i++)
   { char *fname=ent->d_name;
     strcat(strcat(fname,"/"),p_argv[i]);
     printf("\n file name : %s", fname);
      
    if (stat(fname, &fl_stat) == -1)
    {
      printf("Error in accessing information");
    }
    prnt_md(fl_stat.st_mode);

    printf(" %ld", (long)fl_stat.st_nlink);

    struct passwd *pwd = getpwuid(fl_stat.st_uid);
    printf(" %s", pwd->pw_name);

    struct group *grp = getgrgid(fl_stat.st_gid);
    printf(" %s", grp->gr_name);
    printf(" %d", fl_stat.st_size);

    char time_str[20];
    strftime(time_str, sizeof(time_str), "%b %d %H:%M", localtime(&fl_stat.st_mtime));

    printf(" %s", time_str);
    printf(" %s\n", ent->d_name);
 }
}

void fl_size(struct dirent *ent)
{
    struct stat fl_stat;
    if (stat(ent->d_name, &fl_stat) == -1)
    {
      printf("Error in accessing information");
    }
    printf(" %d", fl_stat.st_size);
   printf(" %s\n", ent->d_name);
}

void fl_inode(struct dirent *ent)
{
    struct stat fl_stat;
    if (stat(ent->d_name, &fl_stat) == -1)
    {
      printf("Error in accessing information");
    }
   printf(" %ld", (long)fl_stat.st_ino);
 
   printf(" %s\n", ent->d_name);
}

int process_ls_options(int argc,char *argv[])
{
int aflag,lflag,iflag,sflag,i,option,file_count=0; 

aflag=lflag=iflag=sflag=0;   
optind=1;

while((option=getopt(argc,argv,"alis"))!=-1)
 {
  switch(option)
  {
    case 'a':
       aflag=1;
       break;
    case 'l':
      lflag=1;
      break;
    case 'i':
      iflag=1;
   break;
    case 's':
      sflag=1;
      break;
    default:
      printf("\n Invalid option");
  }
 }
 for(i=optind;i<argc;i++)
 { printf("\n %s",argv[i]);
   file_count++; }
  if (aflag)
  {
                  DIR *drctry;
                  struct dirent *entry;
                  
                  drctry = opendir(".");

                  if (drctry == NULL)
                 {
                    printf("Unable to open directory");
                    return 1;
                 }

                 while ((entry = readdir(drctry)) != NULL)
                {
                    printf("%s\n", entry->d_name);
                }

                 closedir(drctry);
} 
else if(lflag)
{
     DIR *drctry;
                  struct dirent *ent;
                     drctry = opendir(".");
                     if (drctry == NULL)
                 {
                    printf("Error in opening Directory");
                 }

                  while ((ent = readdir(drctry)) != NULL)
                 {
                  if (ent->d_name[0] == '.')
                 {
                    continue;
                  }
                if (file_count==0)
                   fl_info(ent);
                }
}
else if (iflag)
  {    DIR *drctry;
       struct dirent *ent;
       if (optind>=argc)
       { 
            drctry = opendir(".");
            if (drctry == NULL)
            {
              printf("Error in opening Directory");
             }

            while ((ent = readdir(drctry)) != NULL)
            {
              if (ent->d_name[0] == '.')
              {
                 continue;
               }
             fl_inode(ent);
            }
         }
        else
        {  for(i=optind;i<argc;i++)
           {   printf("\n %s",argv[i]);
               drctry = opendir(argv[i]);
              drctry = opendir(".");
             if (drctry == NULL)
              {
                    printf("Error in opening Directory");
                 }

              while ((ent = readdir(drctry)) != NULL)
               {
                 if (ent->d_name[0] == '.')
                 {
                    continue;
                  }
              fl_inode(ent);
             }
            }                  
     } 
   }
else if (sflag)
   { 
        DIR *drctry;
                  struct dirent *ent;

                  drctry = opendir(".");
                  if (drctry == NULL)
                 {
                    printf("Error in opening Directory");
                 }

                  while ((ent = readdir(drctry)) != NULL)
                 {
                  if (ent->d_name[0] == '.')
                 {
                    continue;
                  }
       fl_size(ent);
                }

   }
else 
 {
        DIR *drctry;
                  struct dirent *ent;

                  drctry = opendir(".");
                  if (drctry == NULL)
                 {
                    printf("Error in opening Directory");
                 }

                  while ((ent = readdir(drctry)) != NULL)
                 {
                  if (ent->d_name[0] == '.')
                 {
                    continue;
                  }
                 printf("\n%s",ent->d_name); 
                }

 } 
}
