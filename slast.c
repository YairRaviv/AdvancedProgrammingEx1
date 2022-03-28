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

int main(int argc, char * argv[])
{
    int number;
    if(argc != 2 )
    {
        return -1;
    }
    else
    {
        for (int i = 0, n = strlen(argv[1]); i < n; i++)
            {
            if (!isdigit(argv[1][i]))
               {
                 return -1;}    
            }

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
    FILE *fp = fopen(path_wtmp,"r");

    fseek(fp,0,SEEK_END);
    long total_size = ftell(fp);
    fclose(fp);
    int total_users = total_size/size_utmp;

    lseek(fd,0,SEEK_SET);

    //int a = read(fd,&current_user,size_utmp);
    //time_t first_log = current_user.ut_tv.tv_sec;

    int i = 1;
    int counter = 0;
    lseek(fd,-1*(size_utmp),SEEK_END);

    time_t prev_time = 0;

    while (read(fd,&current_user,size_utmp) == size_utmp && counter<number && i <= total_users)
    {
        if(current_user.ut_type == 2 || current_user.ut_type == 7 ) 
        {

            show_info(&current_user , prev_time);

            counter++;
        }
        i++;
        lseek(fd,-1*(i*size_utmp),SEEK_END);

        prev_time = current_user.ut_tv.tv_sec;

    }
    printf("\n");
    //char *cp;
    //cp = ctime (&first_log);
    //printf ("wtmp begins %s", cp);
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
    
    printf("\n");
}

void show_time(time_t cur_time)
{
    char *cp;
    cp = ctime (&cur_time);
    cp[strlen(cp)-9] = '\0';
    printf ("%s", cp);

}

