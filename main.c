#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PASSWORD "hello"

int verifPass(char* pass)
{
    if( !(strcmp(pass,PASSWORD)) )
        return 1;
    else
        return 0;
}

int main()
{
   char ascii[]="azertyuiopqsdfghjklmwxcvbn",*resultBruteForce;
   int tailleString=strlen(ascii),tailleMini=0,tailleMax=0,i=0,j=0,tailleActu=0,nbPassFound=0,r=0,s=0;
   int *tabVal;

   printf("Taille min :");
   scanf("%d",&tailleMini);

   printf("Taille max :");
   scanf("%d",&tailleMax);

   resultBruteForce=malloc(sizeof(char)*(tailleMini+1) );
   tabVal=malloc(sizeof(int)*(tailleMini+1) );

    for(tailleActu=tailleMini;tailleActu<=tailleMax;tailleActu++)
    {
        realloc(resultBruteForce,(tailleActu+1));
        realloc(tabVal,(tailleActu+1));

        for(i=0;i<=tailleActu;i++)
            resultBruteForce[i]=ascii[0];
        resultBruteForce[i+1]='\0';

        for(i=0;i<=tailleActu;i++)
            tabVal[i]=0;

        while(tabVal[0]!=(tailleString-1))
        {
            for(i=0;i<tailleString;i++)
            {
                resultBruteForce[tailleActu]=ascii[i];
                tabVal[tailleActu]=i;
                printf("%s\n",resultBruteForce);
            }
            for(i=tailleActu;i>=0;i--)
            {
                if(tabVal[i]==(tailleString-1))
                {
                    if(i!=0)
                        tabVal[i]=0;
                    if((i-1)>=0)
                        tabVal[i-1]+=1;
                }
                resultBruteForce[i]=ascii[tabVal[i]];
            }
            system("PAUSE");
        }
    }

    if(verifPass(resultBruteForce))
       printf("MDP TROUVER ! %s\n",resultBruteForce);
    else
        printf("ECHEC, NO FOUND !\n");


    free(resultBruteForce);




    return 0;
}
