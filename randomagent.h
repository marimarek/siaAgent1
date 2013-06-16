#ifndef RANDOMAGENT_H
#define RANDOMAGENT_H

#include "agent.h"

class RandomAgent : public Agent
{
    double m_cancelTime;
public:
    RandomAgent(qint64 cancelTime, int repeatTime, int userId = 0, QObject *parent = NULL);
protected:
    virtual Order bestRateOrder();
    virtual void cancelOrder();
protected slots:
     virtual void changeData();
};

#endif // RANDOMAGENT_H
