#ifndef PRIMALITYTESTS_H
#define PRIMALITYTESTS_H

#include <QtConcurrent>
#include <QDebug>
#include <cstdint>
#include <cstdlib>
#include "priimalitytester.h"
#include <QVector>
#include <QFutureWatcher>

// By using the list of primes from http://www.naturalnumbers.org/primes.html we can approximate the distance between
// two consecutive prime number in average to 16 (with average distance of 10,4729 and average
// absolute deviation of 6,07694858 using a sample of 10000 primes). Using this, we can approximate the value of the
// 1000000th prime to 16000000. So, by using the algorithm in isPrimeWithMem, with tests the prime numbers inferior to
// sqrt(num) we can safely assume that a cache size of sqrt(16000000)=4000 elements will be sufficient.
#define CACHE_SIZE (4000/4)
#define NBR_SMALL_PRIMES 50

#define NBR_THREADS 2


uint32_t squareRoot(uint32_t a_nInput);
uint32_t binarySearchGreaterOrEqual(const uint32_t arr[], uint32_t l, uint32_t r, uint32_t x);


class PrimalityTest : public QObject
{
    Q_OBJECT

public :

    PrimalityTest(QObject *parent):QObject(parent), fWatchers(NBR_THREADS)
    {
        for (int i = 0; i < NBR_THREADS; ++i) {
            fWatchers[i] = new QFutureWatcher<bool>(this);
        }
    }

    virtual bool isPrime(uint32_t number) = 0;

    virtual ~PrimalityTest() {
        for (int var = 0; var < fWatchers.size(); ++var) {
            delete fWatchers[var];
        }
    }

public slots:
    void cancelAllFutures();

protected :
    QVector<QFutureWatcher<bool>*> fWatchers;
};

class SimplePrimalityTest : public PrimalityTest
{
public:
    SimplePrimalityTest(QObject *parent) : PrimalityTest(parent) {}
    bool isPrime(uint32_t num);
    virtual ~SimplePrimalityTest() {}
};

class OptimisedPrimalityTest : public PrimalityTest
{
public:
    OptimisedPrimalityTest(QObject *parent, const uint32_t *previousPrimes, uint32_t &nbrPreviousPrimes) : PrimalityTest(parent), m_previousPrimes(previousPrimes), m_nbrPreviousPrimes(nbrPreviousPrimes) {}
    bool isPrime(uint32_t number);
    virtual ~OptimisedPrimalityTest() {}

private :
    const uint32_t *m_previousPrimes;
    uint32_t &m_nbrPreviousPrimes;
};



#endif // PRIMALITYTESTS_H
