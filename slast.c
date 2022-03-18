#include <stdio.h>
#include <stdlib.h>
#include <utmp.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <time.h>




void show_info(struct utmp* current_user , time_t prev_time);
void show_time(time_t cur_time);
char* calculate_time(time_t prev_time , time_t cur_time);

int main(int argc, char * argv[])
{
    int number;
    if(argc <2)
    {
        number = 1;
    }

    else
    {
        number =atoi(argv[1]);
    }

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

    time_t prev_time = 0;

    while (read(fd,&current_user,size_utmp) == size_utmp && counter<number)
    {
        if(strcmp(current_user.ut_user,"runlevel")!= 0 && strcmp(current_user.ut_user,"shutdown")!=0) 
        {

            show_info(&current_user , prev_time);

            counter++;
        }
        i++;
        lseek(fd,-1*(i*size_utmp),SEEK_END);

        prev_time = current_user.ut_tv.tv_sec;

    }
    close(fd);
    return 0;
}


void show_info(struct utmp* current_user , time_t prev_time)

{
    printf("%-8.8s",current_user->ut_user);
    printf(" ");
    if (strcmp(current_user->ut_user, "reboot") == 0)
    {
        printf("system boot ");
    }
    else
    {
        printf("%-8.8s    ",current_user->ut_line);
    }
    printf(" ");
    if(strcmp(current_user->ut_host, ":0") == 0 || strcmp(current_user->ut_host, ":1") == 0)
    {
        printf("%s              ", current_user->ut_host);
    }

    else
    {
        printf("%16.16s", current_user->ut_host);
    }
    printf(" ");
    show_time(current_user->ut_tv.tv_sec);

    printf(" ");
    if(prev_time == 0)
    {
        printf("still logged in");
    }
    else
    {
        if (strcmp(current_user->ut_user, "reboot") == 0)
        {
            printf("still running");
        }
        else
        {
            printf("crash  (%s)" , calculate_time(prev_time,current_user->ut_tv.tv_sec));
        }
    }

    printf("\n");
}

char* calculate_time(time_t prev_time , time_t cur_time)
{
    int32_t diff = cur_time - prev_time;
    if(diff <= 86400)
    {
        int min = diff/60;
        int hours = min/60;
        int only_min = min - (hours*60) +1;
        char* res_h;
        char* res_m;
        char* points = ":";
        char* res = "";
        sprintf(res_h , "%d" , hours);
        sprintf(res_m , "%d" , only_min);
        strcat(res,res_h);
        strcat(res,points);
        strcat(res,res_m);
        //har* res = res_h+":"+res_m;
        return res;
    }
    else
    {
        int days = diff/86400;
        diff = diff-(days*86400);
        int min = diff/60;
        int hours = min/60;
        int only_min = min - (hours*60) +1;
        char* res_d;
        char* res_h;
        char* res_m;
        char* res = "";
        char* points = ":";
        char* plus = "+";
        sprintf(res_d , "%d" , days);
        sprintf(res_h , "%d" , hours);
        sprintf(res_m , "%d" , only_min);
        strcat(res,res_d);
        strcat(res,plus);
        strcat(res,res_h);
        strcat(res,points);
        strcat(res,res_m);
        return res;

    }
}

void show_time(time_t cur_time)
{
    char *cp;
    cp = ctime (&cur_time);
    cp[strlen(cp)-9] = '\0';
    printf ("%s", cp);

}

