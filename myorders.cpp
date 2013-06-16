#include "myorders.h"

MyOrders::MyOrders(): m_upToDate(false)
{   }

void MyOrders::setOrders(const QVector<QPair<qint32, Order> >& orders)
{
    m_notAssignedOrders.clear();
    clear();

    for(int i = 0; i < orders.size(); ++i)
        (*this)[orders[i].first] = qMakePair(orders[i].second, QDateTime::currentDateTime());

    m_upToDate = true;
}

void MyOrders::pushOrder(const Order& order)
{
    m_notAssignedOrders.enqueue(order);
}

void MyOrders::pushOrder(Order::OrderType transactionTyp, qint32 stockId, qint32 amount, qint32 price)
{
    pushOrder(Order(transactionTyp, stockId, amount, price));
}

void MyOrders::removeLastUnassigned()
{
    m_notAssignedOrders.dequeue();
}

bool MyOrders::assigned(qint32 orderId)
{
    if(m_notAssignedOrders.empty())
        return false;

    (*this)[orderId] = qMakePair(m_notAssignedOrders.dequeue(), QDateTime::currentDateTime());
    return true;
}



void MyOrders::changeOrder(qint32 orderId, qint32 amount)
{
    (*this)[orderId].first.setAmount(amount);
    (*this)[orderId].second = QDateTime::currentDateTime();
    if((*this)[orderId].first.getAmount() == 0)
        remove(orderId);
}

QVector<qint32> MyOrders::olderOrders(qint64 time)
{
    QVector<qint32> ret;
    QMutableMapIterator<qint32, QPair<Order, QDateTime> > i(*this);
    while (i.hasNext()) {
        i.next();
        if(i.value().second.msecsTo(QDateTime::currentDateTime()) > time)
            ret.push_back(i.key());
    }

    return ret;
}


void MyOrders::expire()
{
    m_upToDate = false;
}

bool MyOrders::isUpToDate()
{
    return m_upToDate;
}
