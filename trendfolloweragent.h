#ifndef TRENDFOLLOWERAGENT_H
#define TRENDFOLLOWERAGENT_H

#include "agent.h"

class TrendFollowerAgent : public Agent
{
    Q_OBJECT

    qint64 m_upTrendTime;
    qint64 m_downTrendTime;
    qint64 m_cancelTime;
    double m_bestChooseChance;
public:
    TrendFollowerAgent(qint64 upTrendTime, qint64 downTrendTime,
                       qint64 cancelTime, double bestChooseChange,
                       int repeatTime, qint32 userId = 0, QObject *parent = 0);
protected:
    virtual Order bestRateOrder();
    virtual void cancelOrder();
protected slots:
     virtual void changeData();
};

#endif // TRENDFOLLOWERAGENT_H
