#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/fcntl.h>
#include<sys/mman.h>
#include<regex.h>

char * map_file(const char * file,int *psize)
{
    int fd;
    fd=open(file,O_RDWR);
    int size;
    size=lseek(fd,0,SEEK_END);
    *psize=size;
    char* ptr=NULL;
    ptr=mmap(NULL,size,PROT_READ|PROT_WRITE,MAP_PRIVATE,fd,0);
    close(fd);
    return ptr;
}

int main(void)
{
    char* ptr=NULL;
    int psize;
    ptr=map_file("zy1.txt",&psize);
    char *copy_ptr=NULL;
    copy_ptr=ptr;

    regex_t preg;
    char *regex_str = "<a[^>]+href=\"([^\"]+)\"[^>]*>([^<]+)</a>";
    size_t nmatch=3;
    regmatch_t pmatch[nmatch];
    char one[1024];
    char two[1024];
    int err;
    if((err=regcomp(&preg,regex_str,REG_EXTENDED))>0){
        printf("regcomp error.\n");
        exit(1);
    }
    while((regexec(&preg,ptr,nmatch,pmatch,0))==0)
    {
        bzero(one,sizeof(one));
        bzero(two,sizeof(two));
        snprintf(one, pmatch[1].rm_eo - pmatch[1].rm_so + 1, "%s", ptr+pmatch[1].rm_so);
        snprintf(two, pmatch[2].rm_eo - pmatch[2].rm_so + 1, "%s", ptr+pmatch[2].rm_so);
        ptr+=pmatch[0].rm_eo;
        printf("%s %s\n",one,two);
    }
    regfree(&preg);
    munmap(copy_ptr,psize);
}
