#ifndef STARTER_H
#define STARTER_H

#include "agent.h"
#include "trendfolloweragent.h"
#include "antitrendfolloweragent.h"
#include "randomagent.h"
#include "changetrendagent.h"
#include "calmagent1.h"
#include "szef.h"

#include <QObject>
#include <QThread>

class Starter : public QObject
{
    Q_OBJECT
    QThread threads[1000];
//    QThread threads1[50];
//    QThread threads2[50];
//    QThread threads3[350];
//    QThread threads4[50];

public:
    explicit Starter(QObject *parent = 0);
    
signals:
    
public slots:
    void start();
    
};

#endif // STARTER_H
