#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//La structure instruction sert � stocker les instruction �crit dans le fichier Programme
struct instruction {
	char mot[100];
	struct instruction *nextInstruction;
};
typedef struct instruction L;

char aide1[30]; // cette variable va nous aider pour stocker l'identifiant declar� dans le Fichier Programme
int errorLine=1; // cette variable va contenir le num�ro de la ligne qui contient des mots qui n'appartient pas au fichier Dictionnaire

//Cette fonction permet la lecture caract�re par carat�re dans notre fichier Programme et recup�re des mots
char * words(FILE * ff ){
    char f,*t;
    int i=0;
    t=malloc(sizeof(char));
    while(1){
        f=getc(ff);
     if (f=='\n') errorLine++; //lorsqu'il trouve un retour � ligne il va incr�menter la variable errorLine
        if(f!=' '){  //si le caract�re r�cuper� dans la variable f diff�rent � espace il va ajout� au tableau t
               t[i]=f;
               i++;
               t=realloc(t,(strlen(t)+1)*sizeof(char));
               }
	 
	 /* si le caract�re r�cuper� dans la variable f est �gale � espace ou retour � la 
	 ligne ou fin de fichier, on va affecter au tableau le caract�re null \0 qui indique la fin de cha�ne ou le mot */
        if(f==' ' ||  f == '\n' || f==EOF ){
            t[i]='\0';
            break;
        }
    }
     
return t;
}


// La fonction de L'analyse lexicale il sert � v�rifier est ce que les mots ou bien les commandes �crit dans le fichier Programme sont appartient au fichier Dictionnaire
int Analiser_Lix(char *word){
	int etat=0;
    FILE *f1= fopen("Dictionnaire.txt","r");
	if (f1 == NULL) return -1; 
		char *string;
		
		while(!feof(f1)){ //Tant que le fichier Dictionnaire n'est pas encore atteint End Of File 
			string = words(f1); //Stockage des mots du fichier dictionnaire retourn� par la foction words dans le pointeur des caract�re string
			if (strcmp(word,string) == 0){ //Faire comparer le mot pass� en param�tre avec toutes les mots du fichier Dictionnaire � l'aide de la variable string 
				etat = 1; break; // si le mot pass� en parametre a trouv� dans le fichier dictionnaire la variable etat prenne la valeur 1 et on fait sortir de la boucle
			} else {
				strcpy(string," "); //affecter un espace au pointeur string pour stocker des nouvelles mots
			     etat =0;
			}
		}
			
	fclose(f1); // fermeture du fichier
return etat; // r�cup�ration d'�tat de comparaison	  
}

/*La fonction Error re�oit comme param�tre un mot qui n'appartient pas au dictionnaire, il copie un message dans un fichier nomm� ERROR ce message indique le num�ro
de la ligne qui contient le mot re�coit en param�tre */ 
void Error(char *word){

	FILE *er=fopen("ERROR.txt","a");
	fprintf(er,"Ligne %d : Le mot %s n appartient pas au dictionnaire \n",errorLine,word);
	fclose(er);
}

//Insertion au d�but de la liste cha�n� les mots du fichier Programme
L* insererElementsdebut(char *m,L *lf){
	L *nv =  (L*)malloc(sizeof(L));
	strcpy(nv ->mot,m);
	nv->nextInstruction = lf;
	return nv;
}
//Insertion � la fin de la liste cha�n� les mots du fichier Programme
L* insererElements(char *ch,L *lf){
	L *Nv = (L*)malloc(sizeof(L));
	strcpy(Nv->mot,ch); Nv->nextInstruction = NULL;
	L *aide = lf;
	if (lf == NULL) 
	   lf = insererElementsdebut(ch,lf);
	else{
	while(aide->nextInstruction != NULL){
		aide = aide->nextInstruction;
	}
	aide->nextInstruction = Nv;
     }	
	return lf;
}
//Afficher le contenu de cellule de la liste
void display(L *lp){
	 L *help = lp;
     printf("\nliste :\n");
    while(help != NULL){
    	printf("%s\n",help->mot);
    	help=help->nextInstruction;
    }
}

//La fonction de l'analyse syntaxique il permet de v�rifier est ce que le syntaxe des instructions du fichier Programme sont correct ou non 
//Il re�oit comme param�tre une liste qui contient les instructions du fichier Programme
int Analyse_syntaxique(L *lst){
     L *help = lst;  
    if(help != NULL){
    	if (strcmp(help->mot,"Lire") == 0){
    	  help=help->nextInstruction;
			if(strcmp(help->mot,"(") == 0) {
			  help=help->nextInstruction;
			    if( (strcmp(help->mot,"A") >= 0 && strcmp(help->mot,"Z") <= 0) || ( strcmp(help->mot,"a") >= 0 && strcmp(help->mot,"z") <= 0)) { 
			    
			    
			    	if(Analyse_Semantique(help->mot,aide1) ==1) { /* On exploite la fonction Analyse_Semantique pour s'assurer est ce que 
					     la variable est utilis� dans l'instruction Lire est d�clar� */
				 help=help->nextInstruction;
			       if(strcmp(help->mot,")") == 0) {
				     help=help->nextInstruction;
			           if(strcmp(help->mot,";") == 0) {	
			              printf("Syntaxe est correct.");return 1;
		        	    } else {
		        	    	printf("Vous avez oublie ; !!!");return 0;
		        	    }
		        	}
		           else {
		        	  printf("Vous avez oublie parenthese ferment!!!");	return 0;
		        	    }
		        }
		        
		        
		    }
		        else {
		        	  printf("variable � lire pas un charactere ");return 0;	
		        	 }
		        }
		  else {
		        	  printf("Vous avez oublie parenthese ouvrant!!! ");return 0;	
		       }
		   }
		          
		  else if (strcmp(help->mot,"Ecrire") == 0){
    	  help=help->nextInstruction;
			if(strcmp(help->mot,"(") == 0) {
			  help=help->nextInstruction;
			    if( (strcmp(help->mot,"A") >= 0 && strcmp(help->mot,"Z") <= 0) || ( strcmp(help->mot,"a") >= 0 && strcmp(help->mot,"z") <= 0)) {
			    	
			    	if(Analyse_Semantique(help->mot,aide1) == 1) { /*On exploite la fonction Analyse_Semantique pour s'assurer est ce que 
					     la variable est utilis� dans l'instruction Ecrire est d�clar�*/
				 help=help->nextInstruction;
			       if(strcmp(help->mot,")") == 0) {
				     help=help->nextInstruction;
			           if(strcmp(help->mot,";") == 0) {	
			              printf("Syntaxe est correct.");return 1;
		        	    } else {
		        	    	printf("Vous avez oublie ; !!!");return 0;
		        	    }
		        	}
		           else {
		        	  printf("Vous avez oublie parenthese ferment!!!");	return 0;
		        	    }
		        }
		    }
		        else {
		        	  printf("variable � lire pas un charactere ");return 0;	
		        	 }
		        }
		  else {
		        	  printf("Vous avez oublie parenthese ouvrant!!! ");return 0;	
		       }
		   }  else if (strcmp(help->mot,"Declaration") == 0 ){
		   	    help=help->nextInstruction; 
		   	      	if(strcmp(help->mot,";") == 0) {	
			              printf("Syntaxe est correct.");return 1;
		   	        }else {
		   	        	  printf("Vous avez oublie le ;");return 0;
		   	        }
          }
		   else if  (strcmp(help->mot,"Debut") == 0 ){
		   	    help=help->nextInstruction; 
		   	      	if(strcmp(help->mot,";") == 0) {	
			              printf("Syntaxe est correct.");return 1;
		   	        } else {
		   	        	  printf("Vous avez oublie le ;");return 0;
		   	        }
          } 
          
          else if (strcmp(help->mot,"Variable") == 0 ){
           	     help=help->nextInstruction; 
			   if( (strcmp(help->mot,"A") >= 0 && strcmp(help->mot,"Z") <= 0) || ( strcmp(help->mot,"a") >= 0 && strcmp(help->mot,"z") <= 0)) {
				   strcpy(aide1,help->mot); // Copier le caract�re qui repr�sente indentifiant d�clar� dans la variable aide1 pour l'�xploiter dans la fonction Analyse_Semantique
				   help=help->nextInstruction; 
			              if(strcmp(help->mot,":") == 0){
			               help=help->nextInstruction;
						      if(strcmp(help->mot,"int") == 0 || strcmp(help->mot,"float") == 0 || strcmp(help->mot,"char") == 0  ){
						         help=help->nextInstruction;
						         if(strcmp(help->mot,";") == 0) {
								   printf("Syntaxe est correct.");return 1; 
							  } else {
		   	        	  printf("Vous avez oublie le ;");return 0;
		   	        }
						  } else {
						  	printf("Ce type n existe pas !! ");return 0;
						  }     
		   	        } else {
		   	        	printf("Le s�parateur : est manqu�");return 0;
		   	        }
          } else {
          	    printf("Identificateur unvalide !!!");return 0;
          }
	    } 
	    
	     else if( strcmp(help->mot,"Si") == 0) {
		   	     help=help->nextInstruction;
		   	      if ( strcmp(help->mot,"(") == 0 ){
		   	      	  help=help->nextInstruction; 
		   	      	  if ( (strcmp(help->mot,"A") >= 0 && strcmp(help->mot,"Z") <= 0) || ( strcmp(help->mot,"a") >= 0 && strcmp(help->mot,"z") <= 0) ){
		   	      	  	help=help->nextInstruction;
			             if(strcmp(help->mot,"==") == 0 || strcmp(help->mot,"<") == 0 || strcmp(help->mot,">") == 0 || strcmp(help->mot,"<=") == 0 || strcmp(help->mot,">=") == 0) {	
			               help=help->nextInstruction;
						     if ( atoi(help->mot) >= 0 || atoi(help->mot) <= 0 ){
		   	      	  	      help=help->nextInstruction;
		   	      	  	      if(strcmp(help->mot,")") == 0) {
				               help=help->nextInstruction;
				                if( strcmp(help->mot,"alors") == 0) {
		                          help=help->nextInstruction; 
		                         	if (strcmp(help->mot,"Finsi") == 0 ){
		                         		help=help->nextInstruction;
		                         		 if(strcmp(help->mot,";") == 0) {	
			                                printf("Syntaxe est correct.");return 1;
		   	      	                     }else {
		   	        	                    printf("Vous avez oublie le ;");return 0;
		   	                            }
		                         	} else {
	 	  printf("L'instruction Finsi est manqu� !!!");return 0;
	 }                       
								} else {
	 	  printf("Le mot cle alors est manqu� !!!");return 0;
	 }
	     } else {
	 	  printf("parenthese ferment est manqu� dans l/'instruction Si");return 0;
	 }
	 }
	 } else {
	 	printf("Operateur logique non reconnu !!!"); return 0;
	 }
	 }
	 } else {
	 	  printf("parenthese ouvrant est manqu� dans l'instruction Si");return 0;
	 }
	 } 
	 else if (strcmp(help->mot,"Fin") == 0){
	   	       help=help->nextInstruction;
			     if(strcmp(help->mot,";") == 0) {	
			       printf("Syntaxe est correct.");return 1;
		   	     }else {
		   	       printf("Vous avez oublie le ; dans le fin de programme");return 0;
		   	        }
	   }
	 else if( (strcmp(help->mot,"A") >= 0 && strcmp(help->mot,"Z") <= 0) || ( strcmp(help->mot,"a") >= 0 && strcmp(help->mot,"z") <= 0)) {
		   	     help=help->nextInstruction;
		   	      if ( strcmp(help->mot,":=") == 0 ){
		   	      	  help=help->nextInstruction; 
		   	      	  if ( atoi(help->mot) >= 0 || atoi(help->mot) <= 0 ){ /*La fonction atoi permet de transformer une cha�ne de caract�res, 
						   repr�sentant une valeur enti�re, en une valeur num�rique de type int*/
		   	      	  	help=help->nextInstruction;
			             if(strcmp(help->mot,";") == 0) {	
			              printf("Syntaxe est correct.");return 1;
		   	      	      }else {
		   	        	  printf("Vous avez oublie le ;");return 0;
		   	        }
		   	         } else {
		   	      	 printf("La valeur affecte au variable n est pas numerique"); return 0;
		   	      }
		   } else {
		   	  printf("erreur au niveau de symbole d affectation"); return 0;
		   }
	   } 
          
	    else {
		        	  printf("Syntax error!!! ");	return 0;
		       }
		       
		       
}

}

/*Cette fonction il re�oit comme param�tre 2 cha�nes, le premier repr�sente le nom de 
la variable utilis� soit dans l'instruction Lire ou Ecrire, Le deuxi�me l'indentifiant d�clar� */
int Analyse_Semantique(char m[30],char n[30]){ 
	if (strcmp(m,n)==0) return 1; /*Si la variable n qui existe dans la s�ction des d�clarations 
	                                est identique � la variable m alors la variable est d�clar� */
	else {
		printf("%s n'est pas declare!!",m); return 0;
	}
	
}
//Permet de lib�rer l'�space m�moire occup� par la liste
void liberer(L *liste){
	L *aide;
       while(liste!=NULL) 
      {
       aide = liste;
       liste = liste->nextInstruction;
       free (aide);   
      }
}

int main(int argc, char *argv[]) {
	L *liste=NULL; //la liste qu'on va utilis� pour stocker les instructions
    FILE *f2;
	
	f2= fopen("Programme.txt","r");
    char *ch,*help1=";",*help2=" ";
    do{
	     ch=words(f2); //On r�cup�re les mots dans le pointeur des caract�re ch
	     //On fait appel � la fonction Analyse_lix pour v�rifier est que les mots du fichier Progrmme n'appartient pas au fichier Dictionnaire
	    	if (Analiser_Lix(ch) == 0) {
	    	   if (strcmp(ch,"\n") != 0) 
			    Error(ch);	// appel � la fonction Error qui va charg� le fichier ERROR si Analiser_Lix(ch) == 0
	    	}	   
	    	   
	    if (Analiser_Lix(ch) ==1)	{ /* si les mots du fichier Progrmme appartient au fichier Dictionnaire on passe � v�rifier le syntaxe
	       on exploite la fonction Analyse_syntaxique */
	      if (strcmp(ch,help2) != 0){ //si le mot r�cup�r� dans le pointeur ch est diff�rent � ; on fait l'insertion du mot dans notre liste
	      	liste = insererElements(ch,liste);
	      } 
	      if (strcmp(ch,help1) == 0) { //si le mot r�cup�r� dans le pointeur ch est �gale � �space on passe � l'analyse syntaxique
	      	display(liste); // affichage de l'instruction
	        Analyse_syntaxique(liste); //appel � la fonction Analyse_syntaxique pour la v�rification de syntaxe
	        	 //	liberer(liste); // lib�ration de la liste pour ins�rer nouvelle instruction
				 	L *aide;
                  while(liste!=NULL) 
                   {
                     aide = liste;
                     liste = liste->nextInstruction;
                     free (aide);   
                   }
	      } } 
	}while(!feof(f2));
    fclose(f2);	 //fermeture du fichier Programme
	return 0;
}
