#include <stdio.h>

#include <stdlib.h>

#include <utmp.h>

#include <ctype.h>

#include <fcntl.h>

#include <unistd.h>

#include <string.h>

#include <time.h>



void show_info(struct utmp* current_user);

void show_time(time_t cur_time);

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

int counter = 0;

lseek(fd,-1*(size_utmp),SEEK_END);

while (read(fd,&current_user,size_utmp) == size_utmp && counter<number)

{

    if(strcmp(current_user.ut_user,"runlevel")!= 0 && strcmp(current_user.ut_user,"shutdown")!=0) 

    {

    show_info(&current_user);

    counter++;

    }

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



    if (strcmp(current_user->ut_user, "reboot") == 0)

    {

        printf("system boot ");

    }

    else{

        printf("%-8.8s    ",current_user->ut_line);

    }

    printf(" ");

     

    if(strcmp(current_user->ut_host, ":0") == 0 || strcmp(current_user->ut_host, ":1") == 0)

    {

        printf("%s              ", current_user->ut_host);

    }

    else{

        printf("%16.16s", current_user->ut_host);

    }

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