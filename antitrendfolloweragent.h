#ifndef ANTITRENDFOLLOWERAGENT_H
#define ANTITRENDFOLLOWERAGENT_H

#include "agent.h"

class AntiTrendFollowerAgent : public Agent
{

    Q_OBJECT

    qint64 m_upTrendTime;
    qint64 m_downTrendTime;
    qint64 m_cancelTime;
    double m_bestChooseChance;
public:
    AntiTrendFollowerAgent(qint64 upTrendTime, qint64 downTrendTime,
                       qint64 cancelTime, double bestChooseChange,
                       int repeatTime, qint32 userId = 0, QObject *parent = 0);
protected:
    virtual Order bestRateOrder();
    virtual void cancelOrder();
protected slots:
     virtual void changeData();
};

#endif // ANTITRENDFOLLOWERAGENT_H
