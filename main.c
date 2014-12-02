#define WIN32
#if defined (WIN32)

    #include <winsock2.h>

    typedef int socklen_t;
#elif defined (linux)
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>

    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket(s) close (s)

    typedef int SOCKET;
    typedef struct sockaddr_in SOCKADDR_IN;
    typedef struct sockaddr SOCKADDR;
#endif

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

#include "main.h"

#define PORT 23
#define PASSWORD "bonjour"

//variable global du programme
char ascii[]="azertyuiopqsdfghjklmwxcvbn@";
int mdpTrouver=0;

int main()
{
    unsigned long t_start = 0;
    unsigned long t_end = 0;
    int server=0,reseau=0,i=0;

    mdp threadPrincipal,*dataThread;
    pthread_t *thread;
    threadPrincipal.tailleActu=0,threadPrincipal.tailleString=strlen(ascii),threadPrincipal.numThread=0,threadPrincipal.tailleMax=0,threadPrincipal.tailleMini=0,threadPrincipal.nbThread=0;

    /*printf("Utiliser le programme en reseau ? :");
    scanf("%d",&reseau);*/
    /*if(reseau)
    {
        #if defined (WIN32)
        WSADATA WSAData;
        int erreur = WSAStartup(MAKEWORD(2,2), &WSAData);
        #else
            int erreur = 0;
        #endif

        // Socket et contexte d'adressage du serveur
        SOCKADDR_IN sin;
        SOCKET sock;
        socklen_t recsize = sizeof(sin);

        // Socket et contexte d'adressage du client
        SOCKADDR_IN csin;
        SOCKET csock;
        socklen_t crecsize = sizeof(csin);

        //si il y a une erreur
        int sock_err;

        if(!erreur)
        {
            // Création d'une socket
            sock = socket(AF_INET, SOCK_STREAM, 0);

            // Si la socket est valide
            if(sock != INVALID_SOCKET)
            {
                printf("La socket %d est maintenant ouverte en mode TCP/IP\n", sock);

                // Configuration
                sin.sin_addr.s_addr = htonl(INADDR_ANY);  // Adresse IP automatique
                sin.sin_family = AF_INET;                 // Protocole familial (IP)
                sin.sin_port = htons(PORT);               // Listage du port
                sock_err = bind(sock, (SOCKADDR*)&sin, recsize); //ajout des données dans la socket

                // Si la socket fonctionne
                if(sock_err != SOCKET_ERROR)
                {
                    //Adresse IP
                    printf("Adresse serveur : %s:%d\n", inet_ntoa(sin.sin_addr), htons(sin.sin_port));

                    // Démarrage du listage (mode server)
                    sock_err = listen(sock, 5);
                    printf("Listage du port %d...\n", PORT);

                    // Si la socket fonctionne
                    if(sock_err != SOCKET_ERROR)
                    {
                        // Attente pendant laquelle le client se connecte
                        printf("Patientez pendant que le client se connecte sur le port %d...\n", PORT);
                        csock = accept(sock, (SOCKADDR*)&csin, &crecsize);
                        printf("Un client se connecte avec la socket %d de %s:%d\n", csock, inet_ntoa(csin.sin_addr), htons(csin.sin_port));
                    }
                    else
                        perror("listen");
                }
                else
                    perror("bind");
            }
            else
                perror("socket");
        }
    }*/


    printf("Nombre de Thread a utilise :");
    scanf("%d",&threadPrincipal.nbThread);

    if(threadPrincipal.nbThread<=0)
        threadPrincipal.nbThread=1;

    printf("Taille min :");
    scanf("%d",&threadPrincipal.tailleMini);

    printf("Taille max :");
    scanf("%d",&threadPrincipal.tailleMax);

    //------CREATION DES THREADS-------
    if(threadPrincipal.nbThread>1)
    {
        thread=malloc(sizeof(pthread_t)*(threadPrincipal.nbThread-1));

        dataThread=malloc(sizeof(mdp)*(threadPrincipal.nbThread-1));

        for(i=0;i<(threadPrincipal.nbThread-1);i++)
        {
            dataThread[i]=threadPrincipal;
            dataThread[i].numThread=i+1;
            pthread_create(&thread[i], NULL, bruteForceThread,&dataThread[i]);
        }
    }
    //---------------------------------

    t_start = clock();

    threadPrincipal.resultBruteForce=malloc(sizeof(char)*(threadPrincipal.tailleMax+1) );
    threadPrincipal.tabVal=malloc(sizeof(int)*(threadPrincipal.tailleMax+1) );

    threadPrincipal.tailleMini-=1;
    threadPrincipal.tailleMax-=1;

    for(threadPrincipal.tailleActu=threadPrincipal.tailleMini;threadPrincipal.tailleActu<=threadPrincipal.tailleMax && !(mdpTrouver);threadPrincipal.tailleActu+=1)
    {
        for(i=0;i<=threadPrincipal.tailleActu;i++)
            threadPrincipal.resultBruteForce[i]=ascii[0];
        threadPrincipal.resultBruteForce[threadPrincipal.tailleActu+1]='\0';

        for(i=0;i<=threadPrincipal.tailleActu;i++)
            threadPrincipal.tabVal[i]=0;

        while(threadPrincipal.tabVal[0]!=threadPrincipal.tailleString && !(mdpTrouver) )
        {
            for(i=0;i<threadPrincipal.tailleString && !(mdpTrouver);i++)
            {
                threadPrincipal.resultBruteForce[threadPrincipal.tailleActu]=ascii[i];
                if(verifPass(threadPrincipal.resultBruteForce))
                    mdpTrouver=1;
            }
            threadPrincipal.tabVal[threadPrincipal.tailleActu-1]+=threadPrincipal.nbThread;
            for(i=(threadPrincipal.tailleActu-1);i>=0 && !(mdpTrouver);i--)
            {
                if(threadPrincipal.tabVal[i]>=threadPrincipal.tailleString)
                {
                    if(i!=0)
                    {
                        threadPrincipal.tabVal[i]-=threadPrincipal.tailleString;
                        threadPrincipal.tabVal[i-1]+=1;
                    }
                }
                threadPrincipal.resultBruteForce[i]=ascii[threadPrincipal.tabVal[i]];
            }
        }
    }

    if(mdpTrouver && verifPass(threadPrincipal.resultBruteForce) )
       printf("MDP TROUVER ! %s\n",threadPrincipal.resultBruteForce);
    else if(!(mdpTrouver) )
        printf("ECHEC, NO FOUND !\n");

    if(threadPrincipal.nbThread>1){
        free(thread);
        free(dataThread);
    }
    free(threadPrincipal.resultBruteForce);
    free(threadPrincipal.tabVal);

    t_end = clock();
    printf("Terminer en : %0.4f secondes.\n",(t_end*1.0 - t_start*1.0)/1000);
    system("PAUSE");

    /*if(reseau && sock != INVALID_SOCKET && !erreur)
    {
        // Fermeture de la socket client et de la socket serveur
        printf("Fermeture de la socket client\n");
        closesocket(csock);
        printf("Fermeture de la socket serveur\n");
        closesocket(sock);
        printf("Fermeture du serveur terminée\n");
    }

    #if defined (WIN32)
    if(reseau)
        WSACleanup();
    #endif*/

    return 0;
}

int verifPass(char* pass)
{
    return ( !(strcmp(pass,PASSWORD)) );
}

void* bruteForceThread(void* data)
{
    mdp *threadn=data;
    int i=0;

    threadn->resultBruteForce=malloc(sizeof(char)*(threadn->tailleMax+1) );
    threadn->tabVal=malloc(sizeof(int)*(threadn->tailleMax+1) );

    threadn->tailleMini-=1;
    threadn->tailleMax-=1;

    for(threadn->tailleActu=threadn->tailleMini;threadn->tailleActu<=threadn->tailleMax && !(mdpTrouver);threadn->tailleActu+=1)
    {
        for(i=0;i<=threadn->tailleActu;i++)
            threadn->resultBruteForce[i]=ascii[0];
        threadn->resultBruteForce[threadn->tailleActu+1]='\0';
        threadn->resultBruteForce[threadn->tailleActu-1]=ascii[0+threadn->numThread];

        for(i=0;i<=threadn->tailleActu;i++)
            threadn->tabVal[i]=0;
        threadn->tabVal[threadn->tailleActu-1]=threadn->numThread;

        while(threadn->tabVal[0]!=threadn->tailleString && !(mdpTrouver) )
        {
            for(i=0;i<threadn->tailleString && !(mdpTrouver);i++)
            {
                threadn->resultBruteForce[threadn->tailleActu]=ascii[i];
                if(verifPass(threadn->resultBruteForce))
                    mdpTrouver=1;
            }
            threadn->tabVal[threadn->tailleActu-1]+=threadn->nbThread;
            for(i=(threadn->tailleActu-1);i>=0 && !(mdpTrouver);i--)
            {
                if(threadn->tabVal[i]>=threadn->tailleString)
                {
                    if(i!=0)
                    {
                        threadn->tabVal[i]-=threadn->tailleString;
                        threadn->tabVal[i-1]+=1;
                    }
                }
                threadn->resultBruteForce[i]=ascii[threadn->tabVal[i]];
            }
        }
    }

    if(mdpTrouver && verifPass(threadn->resultBruteForce) )
       printf("MDP TROUVER ! %s\n",threadn->resultBruteForce);

    free(threadn->resultBruteForce);
    free(threadn->tabVal);
}
