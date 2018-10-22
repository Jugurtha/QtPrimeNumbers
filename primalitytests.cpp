#include "primalityTests.h"


void PrimalityTest::cancelAllFutures()
{
    if(!((QFutureWatcher<bool>*)sender())->result())
        for (int i = 0; i < fWatchers.size(); ++i) {
            if(!(fWatchers[i]->isFinished()))
                    fWatchers[i]->cancel();
        }
}

bool SimplePrimalityTest::isPrime(uint32_t number) {

    uint32_t sqrtNum = squareRoot(number);

    bool result = true;

    QFutureSynchronizer<bool> sync;
    sync.setCancelOnWait(false);
    
    for (int i = 0; i < fWatchers.size(); ++i) {
        connect(fWatchers[i],QFutureWatcher<bool>::finished,this,cancelAllFutures);
    }

    uint32_t nbrTests = sqrtNum - 2 + 1;// 2 -> sqrtNum

    for (int i = 0; i < fWatchers.size(); ++i) {
        fWatchers[i]->setFuture(TaskExecutor::run(new PriimalityTester(number,2 + i*(nbrTests/NBR_THREADS), 2 + (i+1)*(nbrTests/NBR_THREADS))));
        sync.addFuture(fWatchers[i]->future());        
    }

    sync.waitForFinished();
    
    QList<QFuture<bool>> futures(sync.futures());

    for (int i = 0; i < futures.size(); ++i) {
        result &= futures[i].result();
    }
    
    if(result){
        for (uint32_t i = 2 + nbrTests - (nbrTests % NBR_THREADS) ; i < 2 + nbrTests ; ++i) {
            if(!(number % i))
            {
                result = false;
                break;
            }
        }
    }

    return result;
}




bool OptimisedPrimalityTest::isPrime(uint32_t number)
{
    uint32_t sqrtNum = squareRoot(number);
    uint32_t limit = m_nbrPreviousPrimes;
//   static uint32_t cacheHits = 0;
//    static uint32_t cacheMiss = 0;


    //Verify if the element's index is cached
    static uint32_t cache[CACHE_SIZE] = {0};
    uint32_t cacheIndex = sqrtNum % CACHE_SIZE;

    if(m_previousPrimes[cache[cacheIndex]] == sqrtNum)//if sqrtNum is a prime that was already cached
    {
        limit = cache[cacheIndex];
//        cacheHits++;//Counting cache hit
    }
    else
    {
//        cacheMiss++;//counting cache miss
        limit = binarySearchGreaterOrEqual(m_previousPrimes,0,m_nbrPreviousPrimes,sqrtNum);//returns the index of sqrtNum in previousPrimes or the index of the smallest element that's greater than sqrtNum
        cacheIndex = m_previousPrimes[limit] % CACHE_SIZE;
        if(!cache[cacheIndex])// we add it to the cache if it's not already cashed
            cache[cacheIndex] = limit;
        else
            if(cacheIndex > NBR_SMALL_PRIMES)// we change it if it's not too small
                cache[cacheIndex] = limit;
    }



    bool result = true;

    QFutureSynchronizer<bool> sync;
    sync.setCancelOnWait(false);

    for (int i = 0; i < fWatchers.size(); ++i) {
        connect(fWatchers[i],QFutureWatcher<bool>::finished,this,cancelAllFutures);
    }

    uint32_t nbrTests = limit - 0 + 1;// 0 -> limit

    for (int i = 0; i < fWatchers.size(); ++i) {
        fWatchers[i]->setFuture(TaskExecutor::run(new OptimisedPriimalityTester(number, m_previousPrimes, i*(nbrTests/NBR_THREADS), (i+1)*(nbrTests/NBR_THREADS))));
        sync.addFuture(fWatchers[i]->future());
    }

    sync.waitForFinished();

    QList<QFuture<bool>> futures(sync.futures());

    for (int i = 0; i < futures.size(); ++i) {
        result &= futures[i].result();
    }

    if(result){
        for (uint32_t i = nbrTests - (nbrTests % NBR_THREADS) ; i < nbrTests ; ++i) {
            if(!(number % m_previousPrimes[i]))
            {
                result = false;
                break;
            }
        }
    }


/*Dumping cache content in file for testing
    if(num == 15485863) {
        FILE *pfile = NULL;
        pfile = fopen("../Tests/cache.txt", "w");
        if (pfile == NULL)
            perror("Unable to open cache output file.");
        else {
            fprintf(pfile, "Cache Hits : %d, Cache Miss : %d\n", cacheHits, cacheMiss);
            for (int i = 0; i < CACHE_SIZE; ++i)
                fprintf(pfile, "%d: %d\n", i, cache[i]);
            fclose(pfile);
        }
    }
//*/

    return result;
}




