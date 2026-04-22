#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <sys/types.h>

#define GAME_TITLE "TEXTTWIST"

char *w=NULL;
char *s=NULL;
char **a=NULL;
size_t na=0;
bool *g=NULL;
int gl=0,sc=0,p=0;

char *my_strdup(const char *s) {
    size_t len = strlen(s) + 1;
    char *new_str = malloc(len);
    if (new_str != NULL) {
        memcpy(new_str, s, len);
    }
    return new_str;
}

void to_upper(char *str) {
    for (; *str; ++str) {
        *str = toupper((unsigned char)*str);
    }
}

void trim(char *str) {
    if (!str) return;

    int len = strlen(str);
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[--len] = '\0';
    }

    char *start = str;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }

    if (start != str) {
        memmove(str, start, strlen(start) + 1);
    }
}

char *randline(const char *filename) {
    char *line = NULL;
    size_t llen = 0;
    ssize_t rlen;
    FILE *fp = fopen(filename, "r");
    char *chosen = NULL;
    size_t count = 0;

    if (!fp) return NULL;

    while ((rlen = getline(&line, &llen, fp)) != -1) {
        count++;
        if (rand() % count == 0) {
            line[strcspn(line, "\n")] = '\0';
            if(chosen) free(chosen);
            chosen = my_strdup(line);
        }
    }

    free(line);
    fclose(fp);
    return chosen;
}

bool iszero(int *f) {
	int i;
	for(i=0;i<26;i++) {
		if(f[i]!=0) return false;
	}
	return true;
}

int *freq(const char *w) {
	int *f=calloc(26,sizeof(*f));
	int i;
	for(i=0;i<strlen(w);i++) {
		f[toupper(w[i])-'A']++;
	}
	return f;
}

bool isanagram(const char *w0,const char *w1) {
    int *f0=freq(w0);
    int *f1=freq(w1);
    int i;
    bool ret=true;
    
    if(iszero(f0) || iszero(f1)) {
    	ret=false;
	} else {    
		for(i=0;i<26;i++) {
			if(f0[i]<f1[i]) {
				ret=false;
				break;
			}
		}
	}

    free(f1);
    free(f0);
    
    return ret;
}

void getanagrams(char ***a,size_t *na,const char *w0) {
    char *w1=NULL;
    size_t llen=0;
    ssize_t rlen=0;
    FILE *fp=fopen("dict.txt","r");
    while((rlen=getline(&w1,&llen,fp))!=-1) {
        w1[strcspn(w1,"\n")]='\0';
        trim(w1);        
        if(isanagram(w0,w1)) {
            (*a)=realloc(*a,sizeof(**a)*((*na)+1));
            (*a)[(*na)++]=my_strdup(w1);
        }
    }
    free(w1);
    fclose(fp);
}

char *shuffle(const char *w) {
	char *s=calloc(strlen(w)+1,sizeof(*s));
	char t;
	int i,j;
	strcpy(s,w);
	for(i=strlen(s)-1;i>0;i--) {
		j=rand()%(i+1);
		t=s[i];
		s[i]=s[j];
		s[j]=t;
	}
	return s;
}

int indexof(char **a,size_t na,const char *w) {
	size_t i;
	ssize_t j=-1;
	for(i=0;i<na;i++) {
	
		char *w0=my_strdup(w);
		char *w1=my_strdup(a[i]);
		
		to_upper(w0);
		to_upper(w1);

		if(!strcmp(w0,w1)) {
			free(w0);
			free(w1);

			j=i;
			break;
		}
		
		free(w0);
		free(w1);

	}

	return j;
}

void clue(const char *w) {
	int i;
	for(i=0;i<strlen(w);i++) {
		printf("*");
	}
}

int main(void) {

    char *line=NULL;
    size_t llen=0;
    ssize_t rlen=0;
    size_t i;
    ssize_t j;

    srand(time(NULL));

    w=randline("rand.txt");
    s=shuffle(w);
    
    getanagrams(&a,&na,w);
	g=malloc(sizeof(*g)*na);
	gl=na;	
	
	for(i=0;i<na;i++) g[i]=false;

/*    
    for(i=0;i<na;i++) {
    	if(i!=0) printf(", ");
    	printf("%s",a[i]);
    }
    printf("\n\n");
*/

    printf(GAME_TITLE": TYPE .HELP FOR HELP.\n");
    printf(GAME_TITLE": %s\n",s);
    printf(GAME_TITLE"> ");
    fflush(stdout);

    while((rlen=getline(&line,&llen,stdin))!=-1) {
        line[strcspn(line, "\n")] = '\0';
        trim(line);
        to_upper(line);

		if(!line || line[0]=='\0') {
			break;
		} else if(line[0]=='.') {
			if(!strcmp(line+1,"HELP")) {
				printf("COMMAND\t\tDESCRIPTION\n");
				printf("------------------------------\n");
				printf(".TWIST\t\tSHUFFLE WORD\n");
				printf(".SCORE\t\tSHOW SCORE\n");
				printf(".LIST\t\tSHOW WORD LIST\n");
				printf("<EMPTY LINE>\tQUIT\n");
			} else if(!strcmp(line+1,"TWIST")) {
				shuffle(s);
			    printf(GAME_TITLE": %s\n",s);
			} else if(!strcmp(line+1,"SCORE")) {
				shuffle(s);
			    printf(GAME_TITLE": your score is %d.\n",sc);
			} else if(!strcmp(line+1,"LIST")) {
				for(i=0;i<na;i++) {
					if(i!=0) printf(", ");
					if(g[i]) 
						printf("%s",a[i]);
					else
						clue(a[i]);
				}		
				printf("\n");
			}
		} else if((j=indexof(a,na,line))!=-1) {
			if(g[j]) {
				printf(GAME_TITLE": word '%s' already guessed.\n",a[j]);
			} else {
				p=strlen(a[j]);
				sc+=p;
				printf(GAME_TITLE": word '%s' found plus %d points score %d.\n",a[j],p,sc);
				g[j]=true;
				gl--;
				if(gl==0) {
					printf(GAME_TITLE": game complete!\n");
					break;
				}
			}
		} else {
			printf(GAME_TITLE": word '%s' not found.\n",line);
		}
		        
	    printf(GAME_TITLE"> ");
        fflush(stdout);
    }
    
    for(i=0;i<na;i++) {
    	free(a[i]);
    	a[i]=NULL;
    }
	na=0;

	free(g);
    free(s);
    free(w);

    return 0;
}
