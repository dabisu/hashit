#include <stdio.h>
#include "mobs.h"
#include "hashit.h"

int main(int argc, char **argv) {

    FILE *fp;
    hash_t table;
    char buffer[2048];
    char *word;

    if (argc<2) exit(1);

    table=hashit_create(2048, 0, NULL, (int(*)(void *, void*))strcmp, CHAIN_H);

    int n;
    fp=fopen(argv[1],"r");
    do {
	fgets(buffer,sizeof(buffer),fp);
	int linei, words=0, worde=0; 
	for (linei=0; buffer[linei]!='\0'; linei++) {
	    
	    if (buffer[linei]==' ') {
	       	worde=linei;
	        // Do something
		word=malloc(worde-words+1);
	        strncpy(word, &buffer[words], worde-words);
		
		if (hashit_insert(table, word, word)!=-1) {
		    //SAY("Added hash element...");
		    sleep(5);
		    n++;
		}
		
	        words=worde+1;
	    } else if (buffer[linei]=='\n' || buffer[linei]=='\r') {
		if ( linei!=0 && (buffer[linei-1]!='\r' || buffer[linei-1]!='\n')) {
	       	  worde=linei;
	          // Do something
		  word=malloc(worde-words+1);
	          strncpy(word, &buffer[words], worde-words);

		  if (hashit_insert(table, word, word)!=-1) {
		      n++;
		      //SAY("Added hash element...");
		  }
		  
	          words=worde+1;
		  break;
		}
	        else break;	  
	    }
	}
	fflush(stdout);
	
    } while(!feof(fp));

    SAY("Inserts: %d",n);

    char *val1, *val2;
    val1=malloc(128); val2=malloc(128);
    val1=(char *)hashit_lookup(table,"henchman");
    val2=(char *)hashit_lookup(table,"sword");

    SAY("lookupz %s %s", val1, val2);
    SAY("tablesize %d hashitsize %d", hashit_tablesize(table), hashit_size(table));
    sleep(2);
    
    char **mykeys;

    mykeys=(char **)hashit_getkeys(table);
    int i=0;
    while (mykeys[i]!=NULL) {
	//SAY("Deleting hash element...");
	hashit_delete(table, mykeys[i]);
	i++;
    }
    
    SAY("Deletions: %d",i);
    free(mykeys);

    return 0;
}

