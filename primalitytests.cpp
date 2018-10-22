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

    // 2 -> sqrtNum

    //qDebug() << "sqrt(" << num << ") = " << sqrtNum << "\n";




   // qDebug() << testNums;
   // getchar();

//    QThreadPool pool;
//    pool.setMaxThreadCount(NBR_THREADS);


    for (int i = 0; i < fWatchers.size(); ++i) {
        connect(fWatchers[i],QFutureWatcher<bool>::finished,this,cancelAllFutures);
    }

    uint32_t nbrTests = sqrtNum - 2 + 1;

    for (int i = 0; i < fWatchers.size(); ++i) {
        fWatchers[i]->setFuture(TaskExecutor::run(new PriimalityTester(number,2 + i*(nbrTests/NBR_THREADS), 2 + (i+1)*(nbrTests/NBR_THREADS))));
    }


/*
    QFutureWatcher<uint32_t> fWatcher;
    QList<uint32_t> testNums;//(sqrtNum - 2 + 1);
    for(uint32_t i = 2; i<= sqrtNum; i++)
        testNums.append(i);
    fWatcher.setFuture( QtConcurrent::filteredReduced(testNums,
                                                      [=](const uint32_t &i){ return (num % i) == 0;},
                                                      &PriimalityTester::reducer
                                                      )
                        );

    fWatcher.waitForFinished();

    result = fWatcher.result() == 0;
 */
 //   qDebug() << "result : " << result << "\n";

    return result;
}




bool OptimisedPrimalityTest::isPrime(uint32_t num)
{
    uint32_t sqrtNum = squareRoot(num);
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

    for (uint32_t i = 0; i <= limit; ++i) {
        if (!(num % m_previousPrimes[i])) {
            result = false;
            break;
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




