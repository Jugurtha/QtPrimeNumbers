#ifndef PRIIMALITYTESTER_H
#define PRIIMALITYTESTER_H

#include "controlabletasks.h"

class PriimalityTester : public ControllableTask<bool>
{
public:
    explicit PriimalityTester(const uint32_t number, uint32_t first, uint32_t limit);
    virtual bool run(TaskControl& control) override;

private :
    const uint32_t m_number;
    const uint32_t m_first;
    const uint32_t m_limit;

};

class OptimisedPriimalityTester : public ControllableTask<bool>
{
public:
    explicit OptimisedPriimalityTester(const uint32_t number, const uint32_t *previousPrimes, uint32_t first, uint32_t limit);
    virtual bool run(TaskControl& control) override;

private :
    const uint32_t m_number;
    const uint32_t *m_previousPrimes;
    const uint32_t m_first;
    const uint32_t m_limit;
};

#endif // PRIIMALITYTESTER_H
