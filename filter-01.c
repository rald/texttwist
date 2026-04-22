#include <stdio.h>
#include <string.h>
#include <ctype.h>

int main(void) {

    char *line=NULL;
    size_t llen=0;
    ssize_t rlen=0;
    int len=0;

    while((rlen=getline(&line,&llen,stdin))!=-1) {
        char *p=strchr(line,'\n'); if(p) *p='\0';
        len=strlen(line);
        if(len>=6 && len<=7) {
            puts(line);
        }
    }

    return 0;
}
