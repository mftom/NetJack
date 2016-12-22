/** Fichier : inc.c (Communication Sockets/UDP)
 *   Les deux processus distants s'envoient un nombre qu'ils
 *   incrementent successivement : L'un compte en pair, l'autre en impair ...  */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <ctype.h>


struct variable {
    int valeur;
    char nomVariable[10];
};

void envoi(int looptime,int sd0,char *msg_out,struct sockaddr_in *padr0,socklen_t ls,float i,int decla){

    sprintf(msg_out,"%.2f",i);
printf("\n\nEnvoi(%d) ... ", looptime);
if (sendto(sd0,msg_out,sizeof(msg_out),0,(struct sockaddr *)padr0,ls) >0)
    if(!decla){
        printf("affectation de variable faite");
    }
    else {
        printf("termine : valeur = %s !\n",msg_out);}
else
printf("inacheve : %s !\n",msg_out);
}

int main(int argc, char *argv[]){
    
    struct variable *addrVar = malloc(sizeof(int)*26);
    int indexTabAddr = 0;
    
    int looptime = 0; /* Numero de la boucle */
    socklen_t ls = sizeof(struct sockaddr_in); /* Taille des adresses */
    /*---- Caracterisation de la socket d'émission ----------*/
    int sd0;                 /* Descripteur  */
    int ps0 = 5001;    /* Port         */
    struct sockaddr_in adr0, *padr0 = &adr0; /* Adresse  */
    /*---- Buffers pour Messages -------------------------------*/
    char msg_in[40] = "0";      /* Message recu de "0" a "99" */
    char msg_out[40] = "0";    /* Message a envoyer "0" a "99" */

    /* 1) Concernant la socket d'émission ================*/
    
    //Detection port
    
    if(argc != 2){
        perror("Entrer numero de port du serveur");
    }
    
    /* a) Creation : Domaine AF_INET, type DGRAM, proto. par defaut*/
    if ((sd0=socket(AF_INET, SOCK_DGRAM, 0)) == -1)
        perror("[SOCK_DGRAM, AF_INET, 0]");
    else
        printf("socket [SOCK_DGRAM, AF_INET, 0] creee\n");
    /* b) Preparation de l'adresse d'attachement */
    adr0.sin_family      = AF_INET;
    adr0.sin_addr.s_addr = htonl(INADDR_ANY);  /* Format reseau */
    adr0.sin_port        = htons(ps0);  /* Format reseau */
    /* c) Demande d'attachement de la socket */
    if(bind(sd0,(struct sockaddr *)(padr0),ls) == -1) {
        perror("Attachement de la socket impossible");
        close(sd0);  /* Fermeture de la socket               */
        exit(2);       /* Le processus se termine anormalement.*/
    }
    /* 3) Boucle emission-reception*/
    //int x = 3;
    for(;;) {
  float i = 0;
        printf("\n------------------\n");
        /* b) Reception */
        printf("Attente de reception ... ");
        if (recvfrom(sd0,msg_in,sizeof(msg_in),0, (struct sockaddr *)padr0, &ls) == -1)
            printf("inachevee : %s !\n",msg_in);
        else  {
            printf("\nCalcul recu : %s \nenvoye par %s sur le port %d \n",msg_in,inet_ntoa(padr0->sin_addr), ntohs(padr0->sin_port));
            //printf("terminee : valeur = %s envoye par %s sur le port %d !\n",msg_in, inet_ntoa(padr0->sin_addr), ntohs(padr0->sin_port));
            /* c) Traitement : La reception est bonne, on fait evoluer i */
            
            // creation des variable pour l'operateur, et les nombres du message recu
            char op;
            char nb1[10];
            char nb2[10];
            
            // On remplace les valeurs de nb1, op et nb2 par les valeurs recu dans le message
            sscanf(msg_in, "%s %c %s",nb1,&op,nb2);
            
            // Si le premier nombre recu est une variable on remplace par sa valeur
            if (!isdigit(nb1[0])){
                for (int c = 0; c < indexTabAddr; c++) {
                    printf("%d tour de boucle, nom variable = %s, nom variable recu= %s, valeur = %d \n",c, addrVar[c].nomVariable,nb1, addrVar[c].valeur);
                    if (!strcmp(addrVar[c].nomVariable,nb1)){
                        sprintf(nb1, "%d",addrVar[c].valeur);
                        printf("variable = %d\n",addrVar[c].valeur);
                    }
                }
            }
            
            // Si le deuxieme nombre recu est une variable on remplace par sa valeur
            if (!isdigit(nb2[0])){
                for (int c = 0; c < indexTabAddr; c++) {
                    printf("%d tour de boucle, nom variable = %s, nom variable recu= %s, valeur = %d \n",c, addrVar[c].nomVariable,nb1, addrVar[c].valeur);
                    if (!strcmp(addrVar[c].nomVariable,nb2)){
                        sprintf(nb2, "%d",addrVar[c].valeur);
                        printf("variable = %d\n",addrVar[c].valeur);
                    }
                }
            }
            
            // On remplace la valeur des char[] par leur
            if (op == '+') {
                i = atof(nb1) + atof(nb2);
                envoi(looptime, sd0, msg_out, padr0, ls,i,1);
            }
            if (op == '-') {
                i = atof(nb1) - atof(nb2);
                envoi(looptime, sd0, msg_out, padr0, ls,i,1);
            }
            if (op == '/') {
                i = atof(nb1) / atof(nb2);
                envoi(looptime, sd0, msg_out, padr0, ls,i,1);
            }
            if (op == '*') {
                i = atof(nb1) * atof(nb2);
                envoi(looptime, sd0, msg_out, padr0, ls,i,1);
            }
            
            if (op == '='){
                
                // on cree une instance de variable
                struct variable nvAddr;
                
                // Et on l'ajoute dans une liste où se trouvent toutes les autres instances de variables
                addrVar[indexTabAddr] = nvAddr;
                addrVar[indexTabAddr].valeur = atoi(nb2);
                sprintf(addrVar[indexTabAddr].nomVariable,"%s",nb1);
                indexTabAddr++;
                envoi(looptime, sd0, msg_out, padr0, ls,0,0);}
            
            
            
            sprintf(msg_out,"");
        }
        sleep(1);
        looptime++; 
    } 
}
