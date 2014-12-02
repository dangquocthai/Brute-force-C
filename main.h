#ifndef MAIN_H_INCLUDED
#define MAIN_H_INCLUDED

typedef struct mdp mdp;
struct mdp
{
    char* resultBruteForce;
    int tailleActu;
    int* tabVal;
    int tailleString;
    int tailleMini;
    int tailleMax;
    int nbThread;
    int numThread;
};

int verifPass(char* pass);
void BruteForce(int LongMin, int LongMax);
void* bruteForceThread(void* data);

#endif // MAIN_H_INCLUDED
