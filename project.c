#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <time.h>

#define WORDLEN 30

typedef union {
  int plithos_leksewn;
  char *word;
} Data;

typedef enum { SORTED,BEGIN,END } ListCondition;

struct List {
  ListCondition typos;
  Data dedomena;
  struct List *next;
  struct List *prev;
};
typedef struct List ListT;

int elegxos_orismatwn(int argc, char *argv[], int *length, int *steps);
void basic_part_of_the_game(int steps,int sunolo_leksewn,ListT *game_dict);
ListT *createHistory(ListT *head,char *selected_word,int plithos_leksewn_istorikou);
void print_history(ListT *root);
ListT *createAvailableWords(ListT *root,char *selected_word);
void print_available_words(ListT *root);
char *getWord(char *filename);
ListT *createDict(int length,char *filename,int *sunolo_leksewn);
char *random_word(ListT *game_dict ,int number);
int elegxos_diaforas(char *s1,char *s2);
int check_validity(ListT *root,char *selected_word);
void freeList(ListT *root);
ListT *insertNode(ListT *root,char *str,ListCondition typos);


/**********************************arxh main************************************/
int main(int argc, char *argv[]) {

int length;   /* mhkos lekshs */
int steps,sunolo_leksewn;    /* arithmos vhmatwn */
char c;       /*xarakthras pou vohtha sthn epilogh gia to an tha ksanapaiksei*/
ListT *game_dict;

if(elegxos_orismatwn(argc, argv, &length, &steps)==1){

  srand((int)time(NULL));

  game_dict=createDict(length,argv[3],&sunolo_leksewn);
  if (game_dict==NULL) {
	  return 0;	
  }
  do{
     basic_part_of_the_game(steps,sunolo_leksewn,game_dict);
	do {
	  printf("Play again? (y/n): ");  /* rwtaei to xrhsth an thelei na ksanapaixei */
	  scanf(" %c",&c);
	  c=tolower(c);
	  if ( c != 'y' && c != 'n'){
	    printf("Please enter y for yes or n for no! \n");
	  }
        } while (c!='y' && c != 'n');
 } while (c=='y');
}
freeList(game_dict);

  return(0);
}
/**********************************telos main kai arxh sunarthswsn**************/


/*sunarthsh elegxou orismatwn*/
int elegxos_orismatwn(int argc, char *argv[], int *length, int *steps){
    int numberSteps;

    if (argc != 4) {                               /* elegxoume ton arithmo twn orismatwn */
        if (argc < 4) {
            printf("Insufficient arguments\n");
        }
	else if(argc>4) {
            printf("Too many arguments\n");
        }
        printf("./project LENGTH STEPS DICTIONARY\n");
        printf("\tLENGTH: requested length of word. Must be >1, <30 \n");
        printf("\tSTEPS: requested number of steps. Must be >0\n");
        printf("\tDICTIONARY: name of dictionary file \n");
        return (0);
    }


    numberSteps=atoi(argv[2]);  /* arithmos vhmatwn    */
    while (numberSteps <=0 ){                /*elegxoume ton arithmo twn vhmatwn*/
     printf("You may not have fewer than one step.Specify a different number of steps: \n");
     scanf("%d", &numberSteps);
    }

    *steps=numberSteps;
    *length=atoi(argv[1]);
    return (1);
}

/*kurio loop paixnidou*/
void basic_part_of_the_game(int steps,int sunolo_leksewn,ListT *game_dict){
  char *target;
  char *start;
  ListT *head,*kefalh;
  char selected_word[WORDLEN]; // h leksi pou plhktrologei o xrhsths me vash tis diathesimes
  int n1,n2;           // ni oi tuxaioi arithmoi pou pairnoume apo th rand gia epilogh tuxaias lekshs
  int check=1;           //metavlhth elegxou gia apothhkeush sto istoriko
  int plithos_leksewn_istorikou=0;

  do{
    n1=rand()%sunolo_leksewn;
    start=random_word(game_dict,n1);
    strcpy(selected_word,start);

    n2=rand()%sunolo_leksewn;
    target=random_word(game_dict,n2);
    
  }while(strcmp(start,target)==0);
    printf("\tStarting word: ");
    printf("%s \n",start);
    printf("\tTarget word: ");
    printf("%s \n",target);
  printf("Try to go from %s to %s in no more than %d steps \n\n",start,target,steps);
   plithos_leksewn_istorikou=0;
  do{
    if(check){ //elegxoume an prepei na ginei h eisagwgh ths lekshs sto istoriko
      head=createHistory(head,selected_word,plithos_leksewn_istorikou);
	 kefalh=createAvailableWords(game_dict,selected_word); //dhmiourgia listas diathesimwn leksewn pros epilogh	  
    }
    printf("Progress so far: ");
    print_history(head);
    check=1;  //se periptwsh pou eixe ginei 0
    
   
    if(kefalh==NULL){   //elegxos an den uparxoun diathesimes lekseis
      printf("There are no words to choose from. Sorry! \n");
      break;
    }  
    printf("Remaining steps: %d",steps);
    printf("\n");
    printf("Target: %s \n",target);
    printf("\n");
    printf("Choose one of the following words: \n");
    print_available_words(kefalh); //ektypwsh ths listas me ts diathesimes lekseis
    printf("\n");
    printf("Enter Selection: ");        
    scanf("%29s",selected_word);     //epilegoume nea leksh
    steps--;  //meiwsh vhmatwn
    if(!check_validity(kefalh,selected_word)){   //elegxos an to selected_word anhkei stis diathesimes epiloges
      printf("ERROR: Your selection is not available. Try again! \n");
      steps++; //epanafora vhmatos
      check=0; //gia na mn apothhkeutei sto istoriko
      continue; //teleiwnei autos o guros
    }
    plithos_leksewn_istorikou++;
    
    //kommati elegxou nikhth
    if(strcmp(selected_word,target)==0 ){
	 printf("\nGOOD WORD!");
	  head=createHistory(head,selected_word,plithos_leksewn_istorikou);
	 print_history(head);   
	 printf("\n");
	 break;
    }
    else if(steps<=0){
   head=createHistory(head,selected_word,plithos_leksewn_istorikou);
 	printf("\nTOO BAD :( ");
	print_history(head); 
	printf("Target was: %s\n",target);
	break;
    }
    if (kefalh!=NULL) freeList(kefalh);
  }while(1);
  freeList(head);
}   
//-------------------sunarthseis listwn---------------------------------------------------------------------
//dhmiourgia leksikou
ListT *createDict(int length,char *filename,int *sunolo_leksewn){
        ListT *root;
	char  *new_word;
	
	//dhmiourgia sentinel
	root=(ListT*)malloc(sizeof(ListT));
	if(root==NULL){
	  printf("No memory available in createList! \n");
	  exit(1);
	}
	root->next=root;
	root->prev=root;
	root->dedomena.plithos_leksewn=0;
        root->typos=BEGIN;

	new_word=getWord(filename);
	while(new_word != NULL){
	  if(strlen(new_word)==length ){ //an exei h nea leksh idio mhkos tote eisagwgh sto leksiko
	    insertNode(root,new_word,root->typos); 
	  }
          new_word=getWord(filename);
	}

	*sunolo_leksewn=root->dedomena.plithos_leksewn; //epistrofh tou arithmou leksewn
	if(*sunolo_leksewn<2){
	   freeList(root);
	   return(NULL);
	}           //an periexei ligoteres apo 2 lekseis den epistrefei tpt
	return(root);
}

//synarthsh dhmiourgias istorikou
ListT *createHistory(ListT *head, char *selected_word,int plithos_leksewn_istorikou){
  
  
  //apothikeusi plithous leksewn istorikou sto sentinel komvo.
  if(plithos_leksewn_istorikou<=0){
    head=(ListT*)malloc(sizeof(ListT));
    if(head==NULL){
     printf("No memory available for list history! \n");
     exit(1);
    }
    head->next=head;
    head->prev=head;
  } 
  head->dedomena.plithos_leksewn=plithos_leksewn_istorikou;   
  head->typos=END;
  
  insertNode(head,selected_word,head->typos);
  return(head);
}

//sunarthsh dhmiourgias diathesimwn leksewn
ListT *createAvailableWords(ListT *root,char *selected_word){
  ListT *riza,*cersor;  //cersor: trexei th lista leksikou curr:trexei thn createAvailableWords
  int i=0; // kratame ton arithmo twn diathesimwn leksewn
  
  //dhmiourgia sentinel xwris dedomena
  riza=(ListT*)malloc(sizeof(ListT));
  if(riza==NULL){
    printf("No memory in createAvailableWords.\n");
    exit(1);
  }  
  riza->dedomena.plithos_leksewn=0;
  riza->typos=SORTED;
  riza->next=riza;
  riza->prev=riza;
  
  for(cersor=root->next; cersor!=root; cersor=cersor->next){
    
    if(elegxos_diaforas(selected_word,cersor->dedomena.word)==1){
      i++;
      
      insertNode(riza,cersor->dedomena.word,riza->typos);
    }
  }
  if(i==0){ return(NULL);}
  return(riza);
}

//sunarthsh eisagwghs komvou vash ton typo ths listas
ListT *insertNode(ListT *root,char *str,ListCondition typos){
  ListT *new,*curr,*previous;
  
  new=(ListT*)malloc(sizeof(ListT));
  if(new==NULL){
    printf("No memory available for list history! \n");
    exit(1);
  }
  
  if(typos==BEGIN){
   new->dedomena.word=strdup(str); //eisagwgh leksewn me epithimito mhkos, sthn arxh ths listas leksikou
   new->next=root->next;
   new->prev=root;
   root->next->prev=new;
   root->next=new;
   root->dedomena.plithos_leksewn++;
   return(root);
  } 
  
  
  if(typos==END){
   //prosthiki neas leksis sto telos ths listas istorikou
   new->dedomena.word=strdup(str);
   new->next=root;
   new->prev=root->prev;
   root->prev->next=new;
   root->prev=new;
   return(root);
  } 
  
  
  if(typos==SORTED){
   
   curr=root->next;  //arxikopoihsh deiktwn sth lista createAvailableWords
   previous=root;
  
   while(curr!=root){
     if(strcmp(curr->dedomena.word,str)>=0){break;}
     previous=curr;
     curr=curr->next;
   }
  
   //taksinomimenh eisagwgh sthn createAvailableWords.
   new->dedomena.word=strdup(str);
   new->next=curr;
   new->prev=previous;
   curr->prev=new;
   previous->next=new;
   return(root);
  } 
  return(NULL);
}  

//-------------------sunarthseis ektypwshs--------------------------------------------------------------------------------
void print_history(ListT *root){
	ListT *cersor;
	for(cersor=root->next; cersor!=root; cersor=cersor->next){
	   printf("%s",cersor->dedomena.word);
	   if(cersor->next!=root){
	     printf("-->");
	  }
	}
	printf("\n");
}

void print_available_words(ListT *root){
	ListT *cersor;
	int i=0;
	for(cersor=root->next; cersor!=root; cersor=cersor->next){
		printf("%s ",cersor->dedomena.word);
		if((i+1)%4==0) {
		 printf("\n");
		}
		i++;
	}
}
//---------------diafores vohthitikes sunarthseis-----------------------------------------------------------------------------

//sunarthsh pou epilegei tuxaia 2 strings
char *random_word(ListT *game_dict,int number){
  int counter=0;
  ListT *current;

  for(current=game_dict->next;current!=game_dict;current=current->next){
    if(counter==number){
      return (current->dedomena.word);
    }
    counter++;
  }
    return(NULL);
}

//sunarthsh pou elegxei an 2 strings diaferoun kata 1
int elegxos_diaforas(char s1[],char s2[]){
   int i,k=0;
   for(i=0; i<strlen(s1); i++){
     if(s1[i] != s2[i]){ k++; }
   }
    if(k==1){return(1); }
    else { return(0); }
}

//sunarthsh pou elegxei an to selected_word yparxei stis diathesimes pros epilogh lekseis
int check_validity(ListT *root,char *selected_word){
  ListT *cersor;
  for(cersor=root->next; cersor!=root; cersor=cersor->next){
    if(strcmp(cersor->dedomena.word,selected_word)==0){
      return(1);
    }
  }
  return(0);
}  

//sunarthsh apeleutherwshs mnhmhs
void freeList(ListT *root){
  ListT *cersor;
  
  while(root->next!=root){
    cersor=root->next;
    root->next=root->next->next;
    free(cersor->dedomena.word);
    free(cersor);
  }
 free(root); // diagrafi sentinel
}

/* Given the name of a file, read and return the next word from it,
or NULL if there are no more words */

char *getWord(char *filename)  {
	char formatstr[15], *word;
	static FILE *input;
	static int firstTime = 1;
	if (firstTime) {
		input = fopen(filename, "r");
		if (input == NULL) {
			printf("ERROR: Could not open file \"%s\"\n", filename);
			exit(1);
		}
		firstTime = 0;
	}
	word = (char*)malloc(sizeof(char)*WORDLEN);
	if (word == NULL) {
		printf("ERROR: Memory allocation error in getWord\n");
		exit(1);
	}
	sprintf(formatstr, "%%%ds", WORDLEN-1);
	fscanf(input, formatstr, word);
	if (feof(input)) {
		fclose(input);
		firstTime = 1;
		return NULL;
	}
	return word;
}
