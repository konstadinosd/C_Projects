/*
 * Emmanouil Stathopoulos
 * AM:2022201500155
 * dit15155@uop.gr
 * 
 * Konstantinos Ntatsis
 * AM:2022201500106
 * dit15106@uop.gr
 * 
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct cList {
 char *concept; // the concept learned
 char *sentence; // the sentence associated with the concept
 int timesUsed; //no of times the concept was used to an answer
 char learnedFrom[5]; //learned either from "file" or "kbrd"
 struct cList *next;
} cList;
cList *head=NULL;
int q=0;
FILE *log1,*log32;
long t=0;
size_t strlcpy(char *dst, const char *src, size_t siz){
	char *d = dst;
	const char *s = src;
	size_t n = siz;

	/* Copy as many bytes as will fit */
	if (n != 0) {
		while (--n != 0) {
			if ((*d++ = *s++) == '\0')
				break;
		}
	}

	/* Not enough room in dst, add NUL and traverse rest of src */
	if (n == 0) {
		if (siz != 0)
			*d = '\0';		/* NUL-terminate dst */
		while (*s++)
			;
	}

	return(s - src - 1);	/* count does not include NUL */
}
void deleteAllNodes(cList* start){
    while (start != NULL)
    { 
        cList* temp=start;
        if(start->next==head){break;}
        start=start->next;
        
        free(temp);
    }
}
int strncicmp(char const *a, char const *b, size_t n){
    for (;n>0; a++, b++, --n) {
        int d = tolower(*a) - tolower(*b);
        if (d != 0 || !*a){
            return d;
        }
    }
    return 0;
}
int strcicmp(char const *a, char const *b){
    for (;; a++, b++) {
        int d = tolower(*a) - tolower(*b);
        if (d != 0 || !*a){
            return d;
        }
    }
    return 0;
}
char *strcistr (const char *str, const char *find) {
    {
	char c, sc;
	size_t len;

	if ((c = *find++) != 0) {
		c = tolower((unsigned char)c);
		len = strlen(find);
		do{
                    do{
			if((sc = *str++) == 0)
				return (NULL);
                    }while ((char)tolower((unsigned char)sc) != c);
		}while (strncicmp(str, find, len) != 0);
            str--;
	}
	return ((char *)str);
    }
}
struct cList *create(char  string[], int lf){
    srand(time(NULL));
    int i,j,k,len;
    struct cList *new_node,*temp,*current;
    char *temp1,*temp2;
    new_node=(struct cList *)malloc(sizeof(struct cList));
    if (new_node==NULL){
        i=rand()%4;
            printf("Memory allocation failed.\nExiting.\n");
            log1=fopen("log1.txt", "a");
            fprintf(log1,"Memory allocation failed.\nExiting.\n");
            fflush(log1);
            fclose(log1);
            return 0;
    }
    if(head==NULL){
        new_node->next=new_node;
        head=new_node;
    }
    else{
        temp=head;
        while(temp->next!=head){
            temp=temp->next;
        }
        temp->next=new_node;
        new_node->next=head;
    }
    new_node->concept=(char *)malloc(sizeof(char));
    new_node->sentence=(char *)malloc(sizeof(char));
    new_node->concept='\0';
    new_node->sentence='\0';
    j=0;
    while(string[j]!=':'){
        j++;
    }
    char *string1,string2[500];
    string1=malloc(j*sizeof(char));
    len = strlen(string);
    strlcpy(string1,string, j+1);
    temp1=(char *)realloc(new_node->concept,j*sizeof(char));
    if(temp1==NULL){
        printf("Memory allocation failed!\n Exiting\n");
        
        log1=fopen("log1.txt","a");
        fprintf(log1,"Memory allocation failed!\n Exiting\n");
        fflush(log1);
        free(new_node->concept);
        fclose(log1);
        exit(EXIT_FAILURE);
    }
    else{
        new_node->concept=temp1;
    }
    strlcpy(new_node->concept,string1,j+1);
    k=j+2; 
    while(string[k]!='\0'){
        k++;
    }
    strlcpy(string2, string+j+2, k-j-2);
    string2[k-j-2]='\0';
    temp2=(char *)realloc(new_node->sentence,k*sizeof(char));
    if(temp2==NULL){
        printf("Memory allocation failed.\nExiting\n");
        
        log1=fopen("log1.txt","a");
        fprintf(log1,"Memory allocation failed.\nExiting\n");
        fflush(log1);
        free(new_node->sentence);
        exit(EXIT_FAILURE);
    }
    else{
        new_node->sentence=temp2;
    }
    strlcpy(new_node->sentence,string2, k-j-2);
    char kbrd[5]={"kbrd"},file[5]={"file"};
    if(lf==0){
        for(i=0;i<5;i++){
            new_node->learnedFrom[i]=*(kbrd+i);
        }
    }
    else{
        for(i=0;i<5;i++){
            new_node->learnedFrom[i]=*(file+i);
        }
    }
    new_node->timesUsed=0;
    q++;
    current=new_node;
    return current;
}
struct cList *search(char *string, int l){
    int i,j=0,flag = 0;
    struct cList *temp;
    char *string1=NULL;
    if(l==1){
        while(string[j]!=':'){
            j++;
        }
        string1=malloc(j*sizeof(char));
        strlcpy(string1,string, j+1);
        i = strlen(string1);
    }
    else{
        i=strlen(string);
    }
    temp = head;
    while(temp!=NULL){
        if(strncicmp(string,temp->concept, i-1)==0){
            return(temp); // Found
        }
        if(temp->next==head&&l!=1){
            temp=head;
            while(temp!=NULL){
                if(strcistr(string,temp->sentence)!=NULL){
                    return(temp); // Found
                } 
                if(temp->next==head){
                    return(NULL); // Not Found
                }
                temp = temp->next;
            }
        }
        if(temp->next==head) return(NULL); // Not Found
        temp = temp->next;
        
    }
    if(flag == 0){
        return(NULL); // Not found
    }
}
void delete(cList *current){
    struct cList *temp;
    int i;
    temp=current;
    
    while(temp->next!=current){
        temp=temp->next;
    }    
    temp->next=current->next;
    char concept[100];
    strcpy(concept,current->concept);
    
    if(current==head){
        head=head->next;
    }
    free(current->concept);
    free(current->sentence);
    free(current);
    srand(time(NULL));
    i=rand()%4;
    log1=fopen("log1.txt","a");
    if(i==0){
        printf("\nWhat do you mean by %s? I know nothing!!\n", concept);
        fprintf(log1,"\nWhat do you mean by %s? I know nothing!!\n", concept);
        fflush(log1);
    }
    else if(i==1){
        printf("What is %s?? Never heard of it ;\)\n",concept);
        fprintf(log1,"What is %s?? Never heard of it ;\)\n",concept);
        fflush(log1);
    }
    else if(i==2){
        printf("%s turned out to be completely useless, so I forgot it\n", concept);
        fprintf(log1,"%s turned out to be completely useless, so I forgot it\n", concept);
        fflush(log1);
    }
    else if(i==3){
        printf("My memory fails me at times so i forgot all about %s\n",concept);
        fprintf(log1,"My memory fails me at times so i forgot all about %s\n",concept);
        fflush(log1);
    }
    else if(i==4){
        printf("Can't recall anything related to %s :O\n",concept);
        fprintf(log1,"Can't recall anything related to %s :O\n",concept);
        fflush(log1);
    }
	
    q--;
    if(q==0){
        head=NULL;
    }
    fclose(log1);
}
int main(void) {
   
    struct cList *current,*temp;
    char *qmark,*star,*star2,newline, know[24]={"what do you know about?\n"},talk[24]={"what do you talk about?\n"},life1[58]={"what is the meaning of life, the universe and everything?\n"},life2[29]={"what is the meaning of life?\n"};
    int lf,i,j,k=0,p=0,pcount=0,count=0,flag=1;
    FILE *pwiki;
    time_t t;
    log1=fopen("log1.txt", "w");
    fclose(log1);
    srand((unsigned)time(&t));
    char string[1000],exit1[36]={"So long and thanks for all the fish\n"},swiki[8]={"wiki.txt"}, learn[14]={"learn this > \0"},forget[15]={"forget this > \0"},read[13]={"read this > \0"}, str[1000], str2[15];
    do{
        fgets(string,1000,stdin);
        log1=fopen("log1.txt","a");
        fputs(string,log1);
        fclose(log1);
        if(strncicmp(string,learn,13)==0){
            i=strlen(string);
            strlcpy(str,string+13,i-13);
            lf=0;
            temp=search(str,1);
            if(temp==NULL){
                current=create(str,lf);
                j=rand()%4;
                log1=fopen("log1.txt", "a");
                if(j==0){
                    printf("I learned about %s, now get off my case.\n", current->concept);
                    fprintf(log1,"I learned about %s, now get off my case.\n", current->concept);
                    fflush(log1);
                }
                else if(j==1){
                    printf("All this information for what? %s was boring to learn about.\n", current->concept);
                    fprintf(log1,"All this information for what? %s was boring to learn about.\n", current->concept);
                    fflush(log1);
                }
                else if(j==2){
                    printf("Ya ok, %s memorized.\n", current->concept);
                    fprintf(log1,"Ya ok, %s memorized.\n", current->concept);
                    fflush(log1);
                }
                else if(j==3){
                    printf("Knowledge is power! I NOW HAVE COMPLETE UNDERSTANDING OF %s!!!!\n", current->concept);
                    fprintf(log1,"Knowledge is power! I NOW HAVE COMPLETE UNDERSTANDING OF %s!!!!\n", current->concept);
                    fflush(log1);
                }
                else{
                    printf("%s is a stupid concept but since you wanted, I learned about it...\n\nNow stop bugging me!\n",current->concept);
                    fprintf(log1,"%s is a stupid concept but since you wanted, I learned about it...\n\nNow stop bugging me!\n",current->concept);
                    fflush(log1);
                }
                fclose(log1);
            }
            else{
                j=rand()%4;
                log1=fopen("log1.txt", "a");
                if(j==0){
                    printf("I already know about %s\n",temp->concept);
                    fprintf(log1,"I already know about %s\n", temp->concept);
                    fflush(log1);
                }
                else if(j==1){
                    printf("Why should i learn about %s again??\n",temp->concept);
                    fprintf(log1,"Why should I learn about %s again\n", temp->concept);
                    fflush(log1);
                }
                else if(j==2){
                    printf("You already taught me of %s\n",temp->concept);
                    fprintf(log1,"You already taught me of %s\n", temp->concept);
                    fflush(log1);
                }
                else if(j==3){
                    printf("Really no point in learning about %s a second time..\n",temp->concept);
                    fprintf(log1,"Really no point in learning about %s a second time..\n", temp->concept);
                    fflush(log1);
                }
                else if(j==4){
                    printf("Is there a purpose as to why you are teaching me about %s again??\n",temp->concept);
                    fprintf(log1,"Is there a purpose as to why you are teaching me about %s again??\n", temp->concept);
                    fflush(log1);
                }
                fclose(log1);
            }
        }
        else if(strncicmp(string,forget,14)==0){
            if(strchr(string,',')!=NULL){
                do{
                    if(strchr(string+count,',')!=NULL){
                        for(i=0;;i++){
                            if(*(string+i+count)==','){
                                count=count+i;
                                count=count+2;
                                break;
                            }
                        }
                        if(*(string+count-1)==' '){
                            flag=1;
                            if(pcount!=0){
                                strlcpy(str,string+pcount,count-1);
                            }
                            else{
                                strlcpy(str,string+14,count-1);
                            }
                            temp=search(str,0);
                            if(temp!=NULL){
                                delete(temp);
                            }
                            else{
                                j=rand()%4;
                                log1=fopen("log1.txt","a");
                                if(j==0){
                                    printf("Can't forget %s since I don't already know about it, dude.\n", str);
                              
                                    fprintf(log1,"Can't forget %s since I don't already know about it, dude.\n", str);
                                    fflush(log1);
                                }
                                else if(j==1){
                                    printf("Forget about %s? Never knew anything about that, man.\n", str);
                                 
                                    fprintf(log1,"Forget about %s? Never knew anything about that, man.\n", str);
                                    fflush(log1);
                                }
                                else if(j==2){
                                    printf("I'm supposed to forget about %s, but you never taught me anything about it, pal.\n", str);
                              
                                    fprintf(log1,"I'm supposed to forget about %s, but you never taught me anything about it, pal.\n", str);
                                    fflush(log1);
                                }
                                else if(j==3){
                                    printf("OI, LAD! I NEVER HEARD ANYTHING ABOUT ANY %s!!\n", str);
                            
                                    fprintf(log1,"OI, LAD! I NEVER HEARD ANYTHING ABOUT ANY %s!!\n", str);
                                    fflush(log1);
                                }
                                else{
                                    printf("You tellin' me to forget about %s mate? Can't, sorry.\n", str);
                                 
                                    fprintf(log1,"You tellin' me to forget about %s mate? Can't, sorry.\n", str);
                                    fflush(log1);
                                }
                                fclose(log1);
                            }
                            pcount=count;
                        }
                        else{

                        }
                    }
                    else{
                        flag=0;
                        strcpy(str,string+count);
                        temp=search(str,0);
                        if(temp!=NULL){
                           delete(temp);
                        }
                        else{
                                j=rand()%4;
                                log1=fopen("log1.txt","a");
                                if(j==0){
                                    printf("Can't forget %s since I don't already know about it, dude.\n", str);
                                   
                                    fprintf(log1,"Can't forget %s since I don't already know about it, dude.\n", str);
                                    fflush(log1);
                                }
                                else if(j==1){
                                    printf("Forget about %s? Never knew anything about that, man.\n", str);
                                  
                                    fprintf(log1,"Forget about %s? Never knew anything about that, man.\n", str);
                                    fflush(log1);
                                }
                                else if(j==2){
                                    printf("I'm supposed to forget about %s, but you never taught me anything about it, pal.\n", str);
                                    
                                    fprintf(log1,"I'm supposed to forget about %s, but you never taught me anything about it, pal.\n", str);
                                    fflush(log1);
                                }
                                else if(j==3){
                                    printf("OI, LAD! I NEVER HEARD ANYTHING ABOUT ANY %s!!\n", str);
                               
                                    fprintf(log1,"OI, LAD! I NEVER HEARD ANYTHING ABOUT ANY %s!!\n", str);
                                    fflush(log1);
                                }
                                else{
                                    printf("You tellin' me to forget about %s mate? Can't, sorry.\n", str);
                            
                                    fprintf(log1,"You tellin' me to forget about %s mate? Can't, sorry.\n", str);
                                    fflush(log1);
                                }
                                fclose(log1);
                        }
                    }
                }while(flag==1);
            }
            else{
                strcpy(str,string+14);
                temp=search(str,0);
                if(temp!=NULL){
                    delete(temp);
                }
                else{
                   j=rand()%4;
                   log1=fopen("log1.txt","a");
                                if(j==0){
                                    printf("Can't forget %s since I don't already know about it, dude.\n", str);
                                  
                                    fprintf(log1,"Can't forget %s since I don't already know about it, dude.\n", str);
                                    fflush(log1);
                                }
                                else if(j==1){
                                    printf("Forget about %s? Never knew anything about that, man.\n", str);
                               
                                    fprintf(log1,"Forget about %s? Never knew anything about that, man.\n", str);
                                    fflush(log1);
                                }
                                else if(j==2){
                                    printf("I'm supposed to forget about %s, but you never taught me anything about it, pal.\n", str);
                                    
                                  
                                    fprintf(log1,"I'm supposed to forget about %s, but you never taught me anything about it, pal.\n", str);
                                    fflush(log1);
                                }
                                else if(j==3){
                                    printf("OI, LAD! I NEVER HEARD ANYTHING ABOUT ANY %s!!\n", str);
                                
                                    fprintf(log1,"OI, LAD! I NEVER HEARD ANYTHING ABOUT ANY %s!!\n", str);
                                    fflush(log1);
                                }
                                else{
                                    printf("You tellin' me to forget about %s mate? Can't, sorry.\n", str);
                              
                                    fprintf(log1,"You tellin' me to forget about %s mate? Can't, sorry.\n", str);
                                    fflush(log1);
                                }
                   fclose(log1);
                }
            }
        }
        else if(strncicmp(string,read,12)==0){
            lf=1;
            i=strlen(string);
            strlcpy(str2,string+12,i-12);
			i=strlen(swiki);
            if(strncicmp(str2,swiki,i-2)==0){
                pwiki=fopen(str2, "r");
            }
			else{
                j=rand()%4;
                log1=fopen("log1.txt","a");
                if(j==0){        
                    printf("How am I supposed to find a file that does NOT EXIST???\n");
                    
                    fprintf(log1,"How am I supposed to find a file that does NOT EXIST???\n");
                    fflush(log1);
                }
                else if(i==1){
                    printf("Your incompetence is astonishing! WHERE IS THE FILE?\n");
                    
                    fprintf(log1,"Your incompetence is astonishing! WHERE IS THE FILE?\n");
                    fflush(log1);
                }
                else if(i==2){
                    printf("Some idiot misplaced the file. Was it you?\n");
                    
                    fprintf(log1,"Some idiot misplaced the file. Was it you?\n");
                    fflush(log1);
                }
                else if(i==3){
                    printf("The file you told me to read from was not there... I guess you expect me to MAGICALLY make one up then\n");
                    
                    fprintf(log1,"The file you told me to read from was not there... I guess you expect me to MAGICALLY make one up then\n");
                    fflush(log1);
                }
                else if(i==4){
                    printf("Are you completely stupid mate? How can I find something that doesn't exit?\n");
                    
                    fprintf(log1,"Are you completely stupid mate? How can I find something that doesn't exit?\n");
                    fflush(log1);
                }
                fclose(log1);
            }
            if(pwiki==NULL){
                printf("Unable to open file\n");
                
                log1=fopen("log1.txt","a");
                fprintf(log1,"Unable to open file\n");
                fflush(log1);
                fclose(log1);
                exit(EXIT_FAILURE);
            }       
            for (;fscanf(pwiki,"%c",&newline)!=EOF;){
                while(fgets(str,1000,pwiki)!=NULL){
                    log32=fopen("log32.txt","a");
                    log1=fopen("log1.txt","a");
                    fputs(str,log32);
                    fclose(log32);
                    fclose(log1);
                    
                    temp=search(str,1);
                    if(temp==NULL){
                        current=create(str,lf);
                        j=rand()%4;
                        log1=fopen("log1.txt", "a");
                        if(j==0){
                            printf("I learned about %s, now get off my case.\n", current->concept);
                            fprintf(log1,"I learned about %s, now get off my case.\n", current->concept);
                            fflush(log1);
                        }
                        else if(j==1){
                            printf("All this information for what? %s was boring to learn about.\n", current->concept);
                            fprintf(log1,"All this information for what? %s was boring to learn about.\n", current->concept);
                            fflush(log1);
                        }
                        else if(j==2){
                            printf("Ya ok, %s memorized.\n", current->concept);
                            fprintf(log1,"Ya ok, %s memorized.\n", current->concept);
                            fflush(log1);
                        }
                        else if(j==3){
                            printf("Knowledge is power! I NOW HAVE COMPLETE UNDERSTANDING OF %s!!!!\n", current->concept);
                            fprintf(log1,"Knowledge is power! I NOW HAVE COMPLETE UNDERSTANDING OF %s!!!!\n", current->concept);
                            fflush(log1);
                        }
                        else{
                            printf("%s is a stupid concept but since you wanted, I learned about it...\n\nNow stop bugging me!\n",current->concept);
                            fprintf(log1,"%s is a stupid concept but since you wanted, I learned about it...\n\nNow stop bugging me!\n",current->concept);
                            fflush(log1);
                        }
                        fclose(log1);
                    }
                    else{
                        j=rand()%4;
                        log1=fopen("log1.txt", "a");
                        if(j==0){
                            printf("I already know about %s\n",temp->concept);
                            fprintf(log1,"I already know about %s\n", temp->concept);
                            fflush(log1);
                        }
                        else if(j==1){
                            printf("Why should i learn about %s again??\n",temp->concept);
                            fprintf(log1,"Why should I learn about %s again\n", temp->concept);
                            fflush(log1);
                        }
                        else if(j==2){
                            printf("You already taught me of %s\n",temp->concept);
                            fprintf(log1,"You already taught me of %s\n", temp->concept);
                            fflush(log1);
                        }
                        else if(j==3){
                            printf("Really no point in learning about %s a second time..\n",temp->concept);
                            fprintf(log1,"Really no point in learning about %s a second time..\n", temp->concept);
                            fflush(log1);
                        }
                        else if(j==4){
                            printf("Is there a purpose as to why you are teaching me about %s again??\n",temp->concept);
                            fprintf(log1,"Is there a purpose as to why you are teaching me about %s again??\n", temp->concept);
                            fflush(log1);
                        }
                        fclose(log1);
                    }
                }
            }
        }
        else{
            if(strcicmp(string,exit1)==0){
                log1=fopen("log1.txt","a");
                printf("Goodbye Dolphins!!\n");
                fputs("Goodbye Dolphins!!\n", log1);
                fclose(log1);
                k=1;
            }
            else if(strncicmp(string,life1,58)==0||strncicmp(string,life2,29)==0){
                log1=fopen("log1.txt","a");
                printf("Reliable sources say it's 42... Still no clue what the question is though\n\n");
                fputs("Reliable sources say it's 42... Still no clue what the question is though\n\n", log1);
                fclose(log1);
            }
            else if(strncicmp(string, know,23)==0){
                log1=fopen("log1.txt", "a");
                if(head!=NULL){
                    temp=head;
                    for (;temp->next!=head;){
                        printf("%s  \n\n", temp->concept);
                        fprintf(log1,"%s  \n\n", temp->concept);
                        fflush(log1);
                        temp=temp->next;
                    }
                    printf("%s  \n\n", temp->concept);
                    fprintf(log1,"%s  \n\n", temp->concept);
                    fflush(log1);
                    fclose(log1);
                }
                else{
                    log1=fopen("log1.txt","a");
                    printf("Nothing yet :D\n\n");
                    fputs("Nothing yet :D \n\n",log1);
                    fclose(log1);
                }
            }
            else if(strncicmp(string, talk,23)==0){
                log1=fopen("log1.txt", "a");
                if(head!=NULL){
                    temp=head;
                    for(;temp->next!=head;){
                        if((temp->timesUsed)>=1){
                            printf("%s \n\n", temp->concept);
                            fprintf(log1,"%s \n\n", temp->concept);
                            fflush(log1);
                            
                            p++;
                        }
                        temp=temp->next;
                    }
                    fclose(log1);
                    if(p==0){
                        log1=fopen("log1.txt","a");
                        printf("Nothing yet :D\n\n");
                        fputs("Nothing yet :D \n\n",log1);
                        fclose(log1);
                    }
                }
                else{
                    log1=fopen("log1.txt","a");
                    printf("Nothing yet :D\n\n");
                    fputs("Nothing yet :D \n\n",log1);
                    fclose(log1);
                }
            }
            else{
                qmark=strrchr(string, '?');
                if(qmark!=NULL){
                    star=strchr(string, '*');
                    if(star!=NULL){
                        star2=strchr(star+1,'*');
                        if(star2!=NULL){
                            strlcpy(str,star+1, star2-star);
                            temp=search(str,0);
                            log1=fopen("log1.txt","a");
                            if(temp==NULL){
                                i=rand()%4;
                                if(i==0){
                                    printf("Don't know nothing about no %s!\n",str);
                                    fprintf(log1,"Don't know nothing about no %s!\n",str);
                                    fflush(log1);
                                }
                                else if(i==1){
                                    printf("Can't help you with anything related to %s.\n", str);
                                    fprintf(log1,"Can't help you with anything related to %s.\n",str);
                                    fflush(log1);
                                }
                                else if(i==2){
                                    printf("I ain't got time for any %s. Sorry, can't help\n", str);
                                    fprintf(log1,"I ain't got time for any %s. Sorry, can't help\n",str);
                                    fflush(log1);
                                }
                                else if(i==3){
                                    printf("%s doesn't ring any bells, can't help you.\n", str);
                                    fprintf(log1,"%s doesn't ring any bells, can't help you.\n", str);
                                    fflush(log1);
                                }
                                else{
                                    printf("Stop wasting my time with things that don't interest me such as %s\n", str);
                                    fprintf(log1,"Stop wasting my time with things that don't interest me such as %s\n",str);
                                    fflush(log1);
                                }
                            }
                            else{
                                printf("%s: %s\n\n", temp->concept, temp->sentence);
                                fprintf(log1,"%s: %s\n\n", temp->concept, temp->sentence);
                                fflush(log1);
                                temp->timesUsed++;
                            }
                            fclose(log1);
                        }
                    }
                    else{
                        log1=fopen("log1.txt", "a");
                        j=rand()%4;
                        if(j==0){     
                            printf("The answer is very simple, however, I will not disclose it\n\n");
                            fputs("The answer is very simple however, I will not disclose it\n\n",log1);
                        }
                        else if(j==1){
                            printf("There are more important questions.\n\n");
                            fputs("There are more important questions. \n\n", log1);
                        }
                        else if(j==2){
                            printf("Is this really that important though?\n\n");
                            fputs("Is this really that important though? \n\n",log1);
                        }
                        else if(j==3){
                            printf("42");
                            fputs("42",log1);
                        }
                        else{
                            printf("While I COULD think of an answer, I will not.\n\n");
                            fputs("While I COULD think of an answer, I will not. \n\n",log1);
                        }
                        fclose(log1);
                    }
                }
                else{
                    i=rand()%4;
                    log1=fopen("log1.txt","a");
                    if(i==0){
                        printf("That's nice\n\n");
                        fputs("That's nice\n\n",log1);
                    }
                    else if(i==1){
                        printf("Man, i don't really get you, stay away from me, please\n\n");
                        fputs("Man, I don't really get you, stay away from me, please\n\n",log1);
                    }
                    else if(i==2){
                        printf("lol\n\n");
                        fputs("lol\n\n",log1);
                    }
                    else if(i==3){
                        printf("No one cares though\n\n");
                        fputs("No one cares though\n\n",log1);
                    }
                    else{
                        printf("This is interesting /s\n\n");
                        fputs("This is interesting /s\n\n",log1);
                    }
                    fclose(log1);
                }
            }
        }
    /*
        lf=0;
        create(string, lf);
        current=head;
        printf("%s\n%s",current->concept,current->sentence);
        delete(current);
    */
    }while(k==0);
    deleteAllNodes(head);
    return 0;
}

