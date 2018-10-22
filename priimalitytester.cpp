#include "priimalitytester.h"

PriimalityTester::PriimalityTester(const uint32_t number, uint32_t first, uint32_t limit) : m_number(number), m_first(first),m_limit(limit)
{

}

bool PriimalityTester::run(TaskControl &control)
{
    bool isPrime = true;
    for (uint32_t i = m_first; i < m_limit ; ++i) {
        if(!(m_number % i) || !control.shouldRun())
        {
            isPrime = false;
            break;
        }
    }
    return isPrime;
}


OptimisedPriimalityTester::OptimisedPriimalityTester(const uint32_t number, const uint32_t *previousPrimes, uint32_t first, uint32_t limit) : m_number(number), m_previousPrimes(previousPrimes), m_first(first), m_limit(limit)
{

}

bool OptimisedPriimalityTester::run(TaskControl &control)
{
    bool isPrime = true;
    for (uint32_t i = m_first; i < m_limit ; ++i) {
        if(!(m_number % m_previousPrimes[i]) || !control.shouldRun())
        {
            isPrime = false;
            break;
        }
    }
    return isPrime;
}
