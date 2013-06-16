#ifndef CALMAGENT1_H
#define CALMAGENT1_H

#include "agent.h"

#include <QMap>

class calmAgent1 : public Agent
{
    Q_OBJECT

    QMap<qint32, qint32> m_myPrognozePrice;

    qint64 m_cancelTime;
    double m_diff;
    double m_bestChooseChance;
public:
    calmAgent1(qint64 historyTime, qint64 cancelTime, double diff, double bestChooseChange, int repeatTime, int userId = 0, QObject *parent = 0);

protected:
    virtual Order bestRateOrder();
    virtual void cancelOrder();
protected slots:
     virtual void changeData();
};

#endif // CALMAGENT1_H
