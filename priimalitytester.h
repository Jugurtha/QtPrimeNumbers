#ifndef PRIIMALITYTESTER_H
#define PRIIMALITYTESTER_H

#include "controlabletasks.h"

class PriimalityTester : public ControllableTask<bool>
{
public:
    explicit PriimalityTester(const uint32_t number, uint32_t first, uint32_t limit);
    virtual bool run(TaskControl& control) override;

private :
    uint32_t m_number;
    uint32_t m_first;
    uint32_t m_limit;

};

#endif // PRIIMALITYTESTER_H
