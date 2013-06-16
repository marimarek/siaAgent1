#ifndef CHANGETRENDAGENT_H
#define CHANGETRENDAGENT_H

#include "agent.h"

class changeTrendAgent : public Agent
{
    qint64 m_greatestBuyTime;
    qint64 m_lowestBuyTime;

    qint64 m_greatestSellTime;
    qint64 m_lowestSellTime;

    qint64 m_cancelTime;

    double m_bestChooseChance;
public:
    changeTrendAgent(qint64 greatestBuyTime, qint64 lowestBuyTime, qint64 greatestSellTime, qint64 lowestSellTime,
                     qint64 cancelTime, double bestChooseChance,
                     int repeatTime, qint32 userId = 0, QObject *parent = 0);

protected:
    virtual Order bestRateOrder();
    virtual void cancelOrder();
protected slots:
     virtual void changeData();
};

#endif // CHANGETRENDAGENT_H
