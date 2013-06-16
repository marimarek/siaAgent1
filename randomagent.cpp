#include "randomagent.h"

RandomAgent::RandomAgent(qint64 cancelTime, int repeatTime, int userId, QObject *parent)
    : Agent(repeatTime, 0, userId, parent), m_cancelTime(cancelTime)
{
}


Order RandomAgent::bestRateOrder()
{
    QVector<Order> bestOrders;
    for(int i = 2; i <=  STOKS_NUMBER; ++i)
    {
        if(m_store.contains(i) &&
                ( (m_stocksInfo[i].getBestSellPrice()*1.005 <=  m_store.getMoney() && m_stocksInfo[i].getBestSellPrice() && m_stocksInfo[i].getBestBuyPrice()) ||
                  (m_stocksInfo[i].lastPrice() && m_stocksInfo[i].lastPrice() <= m_store.getMoney()) ) )
        {
            bestOrders.push_back( Order(Order::SELL, i, 0, 0));
        }
        if( ( (m_stocksInfo[i].getBestBuyPrice()*1.025 <=  m_store.getMoney() && m_stocksInfo[i].getBestSellPrice() && m_stocksInfo[i].getBestBuyPrice()) ||
              (m_stocksInfo[i].lastPrice() && m_stocksInfo[i].lastPrice() <= m_store.getMoney()) ))
        {
            bestOrders.push_back( Order(Order::BUY, i, 0, 0));
        }
    }

    if(bestOrders.empty())
        return Order();

    Order order = bestOrders[qrand()%bestOrders.size()];
    qint32 price;
    if(m_stocksInfo[order.getStockId()].getBestSellPrice() && m_stocksInfo[order.getStockId()].getBestBuyPrice())
    {
        if(order.getTransactionType() == Order::SELL)
        {
            price = m_stocksInfo[order.getStockId()].getBestSellPrice()*(1.005 - (0.03*qrand())/RAND_MAX);
        }
        else
        {
            price = m_stocksInfo[order.getStockId()].getBestBuyPrice()*(0.095 + (0.03*qrand())/RAND_MAX);
        }
    }
    else price = m_stocksInfo[order.getStockId()].lastPrice();
    if(!price)
        return Order();
    order.setPrice(price);

    qint32 maxAmount = m_store.getMoney()/price;
    double rand = (1.0*qrand())/RAND_MAX;
    qint32 amount = 3 + qrand()%13;
    if(rand > 0.45 & rand < 0.88)
        amount = 16 + qrand()%35;
    else if(rand >= 0.88)
        amount = 51 + qrand()%75;
    if(maxAmount)
        amount %= maxAmount;
    order.setAmount(amount);
    if(order.getAmount()&& order.getPrice())
        return order;
    else return Order();
}

void RandomAgent::cancelOrder()
{
    QVector<qint32> toCancel = m_myOrders.olderOrders(m_cancelTime);
    foreach(qint32 orderId, toCancel)
        cancelReq(orderId);
}

void RandomAgent::changeData()
{

}
