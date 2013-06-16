#ifndef MYORDERS_H
#define MYORDERS_H

#include "order.h"
#include <QQueue>
#include <QMap>
#include <QDebug>
#include <QDateTime>

class MyOrders : public QMap<qint32, QPair<Order, QDateTime> >
{
    QQueue<Order> m_notAssignedOrders;

    bool m_upToDate;
public:
    MyOrders();

    void setOrders(const QVector<QPair<qint32, Order> >& orders);

    void pushOrder(const Order &order);
    void pushOrder(Order::OrderType transactionTyp, qint32 stockId, qint32 amount, qint32 price);
    void removeLastUnassigned();
    bool assigned(qint32 orderId);

    void changeOrder(qint32 orderId, qint32 amount);

    QVector<qint32> olderOrders(qint64 time);
    void expire();
    bool isUpToDate();
};

#endif // MYORDERS_H
