#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

void show_info(struct utmp* current_user);
int main(int argc, char * argv[])
{
if(argc <2)
{
    exit(1);
}
int number =atoi(argv[1]);
struct utmp current_user;
char* path_wtmp = "/var/log/wtmp";
int fd;
int size_utmp = sizeof(current_user);
if ((fd = open(path_wtmp,O_RDONLY))==-1)
{
    perror(path_wtmp);
    exit(1);
}
int i = 1;
lseek(fd,-1*(size_utmp),SEEK_END);
while (read(fd,&current_user,size_utmp) == size_utmp && i<=number)
{
    show_info(&current_user);
    i++;
    lseek(fd,-1*(i*size_utmp),SEEK_END);
}
close(fd);
return 0;
}
void show_info(struct utmp* current_user)
{
    // if(current_user->ut_line == RUNLVL_MSG)
    // {
    //     printf("im here");
    //     return ;
    // }

    printf("%-8.8s",current_user->ut_user);
    printf(" ");
    printf("%s",current_user->ut_line);
    printf(" ");
    printf("%s",current_user->ut_host);
    printf(" ");
    show_time(current_user->ut_tv.tv_sec);
    printf("\n");
}
void show_time(time_t cur_time)
{
    char *cp;
    cp = ctime (&cur_time);
    cp[strlen(cp)-9] = '\0';
  printf ("%s", cp);
}