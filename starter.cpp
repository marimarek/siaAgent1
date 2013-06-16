#include "starter.h"

Starter::Starter(QObject *parent) :
    QObject(parent)
{
    qsrand(QDateTime::currentDateTime().toMSecsSinceEpoch());
    connect(new Szef(), SIGNAL(end()), SLOT(start()));
}


void Starter::start()
{
    int i = 0;
    for(; i < 100; ++i)
    {


        calmAgent1* agent = new calmAgent1(10 + qrand() % 30, 60 + qrand() % 240,
                                             1.025 + (0.05*qrand())/RAND_MAX, 0.7 + (0.2*qrand())/RAND_MAX,
                                             1500 + qrand()%1500);
        agent->moveToThread(&threads[i]);
        threads[i].start();
    }

    for(int j = 0; j < 100; ++j, ++i)
    {


        TrendFollowerAgent* agent = new TrendFollowerAgent(30 + qrand() % 60, 30 + qrand() % 60,
                                                           60 + qrand() % 180, 0.7 + (0.2*qrand())/RAND_MAX,
                                                           500 + qrand()%1000);
        agent->moveToThread(&threads[i]);
        threads[i].start();
    }

    for(int j = 0; j < 100; ++j, ++i)
    {


        AntiTrendFollowerAgent* agent = new AntiTrendFollowerAgent(30 + qrand() % 60, 30 + qrand() % 60,
                                                           60 + qrand() % 180, 0.7 + (0.2*qrand())/RAND_MAX,
                                                           500 + qrand()%1000);
        agent->moveToThread(&threads[i]);
        threads[i].start();
    }

    for(int j = 0; j < 100; ++j, ++i)
    {


        changeTrendAgent* agent = new changeTrendAgent(30 + qrand() % 80, 5 + qrand() % 10,
                                                       30 + qrand() % 80, 5 + qrand() % 10,
                                                       60 + qrand() % 180, 0.7 + (0.2*qrand())/RAND_MAX,
                                                        500 + qrand()%1000);
        agent->moveToThread(&threads[i]);
        threads[i].start();
    }

    for(int j = 0; j < 100; ++j, ++i)
    {


        RandomAgent* agent = new RandomAgent(60 + qrand() % 240, 1000 + qrand()%2500);
        agent->moveToThread(&threads[i]);
        threads[i].start();
    }

}
