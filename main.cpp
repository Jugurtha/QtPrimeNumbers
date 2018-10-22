
#include <cstdint>
#include <ctime>
#include <cstdlib>
#include <QCoreApplication>
#include <QDebug>
#include "primalitytests.h"





enum{SEQ,SEQMEM,OMP,OMPMEM, QTCONCURRENT, QTCONCURRENTMEM, NB_VERSIONS};
const char* VERSION_STR[] = {"SEQ","SEQMEM","OMP","OMPMEM","QTCONCURRENT","QTCONCURRENTMEM"};

const int VERSION = QTCONCURRENTMEM;



int main(int argc, char *argv[]) {
    qDebug() << "Version : " << VERSION_STR[VERSION] << "\n";
    
    uint32_t nbrPrimes = strtoul(argv[1],NULL,10);//Getting number of primes to find from stdin at lunch
    qDebug() << "Nbr Primes to find : " << nbrPrimes << "\n";

    QCoreApplication app(argc, argv);

    uint32_t *primes = NULL;
    primes = new uint32_t[nbrPrimes];
    if(primes==NULL)
    {
        perror("Unable to allocate memory for prime numbers.");
        exit(EXIT_FAILURE);
    }
    primes[0] = 2;
    primes[1] = 3;

    uint32_t testNum;
    uint32_t cpt = 2;//primes counter
    uint32_t index = 1;

    PrimalityTest *pTest;
    if(QString::fromLatin1(VERSION_STR[VERSION]).endsWith("MEM"))
    {
        pTest = new OptimisedPrimalityTest(nullptr,primes,cpt);qDebug() << "Here !!!\n";
    }
    else
        pTest = new SimplePrimalityTest(nullptr);

    clock_t wallClock = clock();//Starting timer...

    while(cpt<nbrPrimes)
    {
        for (uint32_t k = 0; k < 2 && cpt < nbrPrimes; ++k) {
            //Compute next test number
            testNum = 6*index + (k?1:-1);
            if(pTest->isPrime(testNum))
            {
                primes[cpt] = testNum;
                cpt++;
            }
        }
        index++;
    }

    wallClock =  clock() - wallClock;
    qDebug() << "wall clock time : " << (double) wallClock/CLOCKS_PER_SEC << "\n";

    delete pTest;


    //Comparing results with the first 1000000 primes from http://www.naturalnumbers.org/primes.html using WinMerge
    // (this is not portable)
//*
    FILE *pfile = NULL;
    pfile = fopen("../Tests/testPrimes.txt","w");
    if(pfile==NULL)
        perror("Unable to open test output file.");
    else{
        for (uint32_t i = 0; i < nbrPrimes; ++i)
            fprintf(pfile, "%d\n", primes[i]);
        fclose(pfile);
        system("C:\\\"Program Files (x86)\"\\WinMerge\\WinMergeU /wl /u /e C:\\Development\\C++\\TESTS\\PrimeNumbers\\Tests\\primes.txt ..\\Tests\\testPrimes.txt");
    }
//*/


    FILE *pfileStats = NULL;
    pfileStats = fopen("C:/Development/C++/Qt/PrimeNumbers/Tests/stats","r+");
    if(pfileStats==NULL)
        perror("Unable to open stats output file.");
    else {
        int version[NB_VERSIONS] = {0};
        fscanf(pfileStats, "%d,%d,%d,%d,%d,%d\n", &version[SEQ], &version[SEQMEM], &version[OMP], &version[OMPMEM], &version[QTCONCURRENT], &version[QTCONCURRENTMEM]);
        version[VERSION]++;
        fseek(pfileStats,0,SEEK_END);
        fprintf(pfileStats, "%s,%d,%d,%d,%lf\n", VERSION_STR[VERSION], version[VERSION], NBR_THREADS, nbrPrimes, (double) wallClock/CLOCKS_PER_SEC);
        fseek(pfileStats,0,SEEK_SET);
        fprintf(pfileStats, "%d,%d,%d,%d,%d,%d\n", version[SEQ], version[SEQMEM], version[OMP], version[OMPMEM], version[QTCONCURRENT], version[QTCONCURRENTMEM]);
        fclose(pfileStats);
    }

    qDebug() << nbrPrimes << " -> " << primes[nbrPrimes-1] << "\n";

    delete[] primes;
    return 0;
}

