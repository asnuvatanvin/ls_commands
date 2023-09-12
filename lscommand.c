#include<stdio.h>
#include<dirent.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<time.h>
#include<pwd.h>
#include<grp.h>
#include<string.h>

void l()
{
  DIR *folder;
  struct dirent *entry;

  long long int size = 0;

  folder = opendir(".");
  if(folder == NULL)
  {
      perror("Unable to read directory");
      return;
  }
  while((entry=readdir(folder)))
  {
    char *filename = entry->d_name;
    char b = *filename;
    if(b=='.')
    {
      continue;
    }
    struct stat fs;
    int r = stat(filename,&fs);
    if( r==-1 )
    {
      fprintf(stderr,"File error\n");
      exit(1);
    }

    size  = size + (long long int)fs.st_blocks/2;

  }
  printf("total %lld\n",size);
  closedir(folder);

  folder = opendir(".");
  if(folder == NULL)
  {
      perror("Unable to read directory");
      return;
  }
  else
  {
    while((entry=readdir(folder)))
    {

      char *filename = entry->d_name;
      char b = *filename;
      if(b=='.')
      {
        continue;
      }

      struct stat fs;
      int r = stat(filename,&fs);
      if( r==-1 )
      {
        fprintf(stderr,"File error\n");
        exit(1);
      }

      //file type
      if( S_ISREG(fs.st_mode) )
        printf("-");
      else if(S_ISLNK(fs.st_mode))
        printf("l");
      else if(S_ISDIR(fs.st_mode))
        printf("d");
      else
        printf("s");
      //users field
      if( fs.st_mode & S_IRUSR )
        printf("r");
        else
            printf("-");
      if( fs.st_mode & S_IWUSR )
        printf("w");
        else
            printf("-");
      if( fs.st_mode & S_IXUSR )
        printf("e");
      else
        printf("-");
     //group field
     if( fs.st_mode & S_IRGRP )
        printf("r");
        else
            printf("-");
     if( fs.st_mode & S_IWGRP )
        printf("w");
        else
            printf("-");
     if( fs.st_mode & S_IXGRP )
        printf("e");
     else
        printf("-");
    //others
    if( fs.st_mode & S_IROTH )
        printf("r");
        else
            printf("-");
    if( fs.st_mode & S_IWOTH )
        printf("w");
        else
            printf("-");
    if( fs.st_mode & S_IXOTH )
        printf("e");
    else
        printf("-");

    printf(" ");
    /* file permissions are kept in the st_mode member */
    // printf("Permission bits: %X\n",fs.st_mode);
    //links attached to file
    printf("%ld  ",fs.st_nlink);

    struct passwd *pw;
    struct group *gid;
    pw = getpwuid(fs.st_uid);
    if(pw == NULL) {
        perror("getpwuid");
        printf("%d ", fs.st_uid);
    }else {
        printf("%s ", pw->pw_name);
    }
    gid = getgrgid(fs.st_gid);
    if(gid == NULL) {
        perror("getpwuid");
        printf("%d ", fs.st_gid);
    }else  {
        printf("%s ", gid->gr_name);
    }

    //file size
    printf("%5ld ", fs.st_size);

    //timestamp
    struct tm *tmp;
    char outstr[200];
    time_t t = fs.st_mtime;
    tmp = localtime(&t);
    if(tmp == NULL) {
        perror("localtime");
        exit(EXIT_FAILURE);
    }
    strftime(outstr, sizeof(outstr), "%b %d %R", tmp);
    printf("%s ", outstr);

    //file name
    if(S_ISDIR(fs.st_mode))
    {
      printf("\033[22;34m%s\033[0m",entry->d_name);
    }
    else
    {
      printf("%s", entry->d_name);
    }
    printf("\n");
    }
  }
  closedir(folder);
}
void a()
{
   struct dirent **namelist;
   int n;

   n = scandir(".", &namelist, 0, alphasort);
   if (n < 0)
       perror("scandir");
   else {
       for(int i=0;i<n;i++)
       {
         char *a = namelist[i]->d_name;
         char b = *a;
         if(b!='.')
         {
            printf("\033[22;34m%s\033[0m",a);
            printf("\n");
         }
         else
         {
           printf("%s\n",a );
         }
         // printf("%s\n", namelist[i]->d_name);
         free(namelist[i]);
       }
       free(namelist);
   }
}
void t()
{
  struct dirent **namelist;
  struct stat fs;
  int n,r;

  n = scandir(".", &namelist, 0, alphasort);
  if (n < 0)
      perror("scandir");
  char **name;
  name = malloc(n*sizeof(char*));
  int i=0,k = n;
  // time = (char*)malloc(n*sizeof(char));
  while(k--)
  {
    name[i]=malloc(n*sizeof(char*));
    strcpy(name[i],namelist[k]->d_name);
    i++;
  }
  n = i;
  time_t one,two;
  for(int i=0;i<n-1;i++)
  {
    for(int j=0;j<n-i-1;j++)
    {
      r = stat(name[j],&fs);
      if( r==-1 )
      {
        fprintf(stderr,"File error\n");
        exit(1);
      }
      one = fs.st_mtime;
      r = stat(name[j+1],&fs);
      if( r==-1 )
      {
        fprintf(stderr,"File error\n");
        exit(1);
      }
      two = fs.st_mtime;
      if(one<two)
      {
        char *p = name[j];
        name[j]=name[j+1];
        name[j+1]=p;

      }
    }
  }

  for(int i=0;i<n;i++)
  {
    if(*name[i]=='.')
    {
      continue;
    }
    r = stat(name[i],&fs);
    if(S_ISDIR(fs.st_mode))
    {
      printf("\033[22;34m%s\033[0m",name[i]);
      printf("  ");
    }
    else
    {
      printf("%s  ",name[i]);
    }
  }
  printf("\n");

}
int main(int argc, char *argv[])
{
  char *c = argv[1];
  c++;
  if(*c=='a')
  {
    a();
  }
  else if(*c=='l')
  {
    l();
  }
  else if(*c=='t')
  {
    t();
  }
  else
  {
    printf("Only -l,-t and -a commands are valid.\n");
  }

  return(0);
}