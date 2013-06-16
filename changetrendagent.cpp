#include "changetrendagent.h"

changeTrendAgent::changeTrendAgent(qint64 greatestBuyTime, qint64 lowestBuyTime, qint64 greatestSellTime, qint64 lowestSellTime,
                                   qint64 cancelTime, double bestChooseChance,
                                   int repeatTime, qint32 userId, QObject *parent)
    : Agent(repeatTime, std::max(greatestBuyTime, std::max(lowestBuyTime, std::max(greatestSellTime, lowestSellTime))), userId, parent),
      m_greatestBuyTime(greatestBuyTime), m_lowestBuyTime(lowestBuyTime),  m_greatestSellTime(greatestSellTime), m_lowestSellTime(lowestSellTime),
      m_cancelTime(cancelTime), m_bestChooseChance(bestChooseChance)
{
}



Order changeTrendAgent::bestRateOrder()
{
    QMap<double, Order> bestOrders;
    for(int i = 2; i <=  STOKS_NUMBER; ++i)
    {
        double diffForSell = m_stocksInfo[i].lowestPrice(m_lowestBuyTime) - m_stocksInfo[i].greatestPrice(m_greatestBuyTime);
        if(m_store.contains(i) && m_stocksInfo[i].lastPrice() && diffForSell > 0.001 &&
                ( (m_stocksInfo[i].getBestSellPrice()*1.005 <=  m_store.getMoney() && m_stocksInfo[i].getBestSellPrice()) ||
                  (m_stocksInfo[i].lastPrice() <= m_store.getMoney()) ) )
        {
            bestOrders.insertMulti(diffForSell, Order(Order::SELL, i, 0, 0));
        }
        double diffForBuy =  m_stocksInfo[i].lowestPrice(m_lowestSellTime) - m_stocksInfo[i].greatestPrice(m_greatestSellTime);
        if(  m_stocksInfo[i].lastPrice() && diffForBuy > 0.001 &&
              ( (m_stocksInfo[i].getBestBuyPrice()*1.025 <=  m_store.getMoney() && m_stocksInfo[i].getBestBuyPrice()) ||
                (m_stocksInfo[i].lastPrice() <= m_store.getMoney()) ))
        {
            bestOrders.insertMulti(diffForBuy, Order(Order::BUY, i, 0, 0));
        }
    }

    if(bestOrders.empty())
        return Order();

    QMutableMapIterator<double, Order> i(bestOrders);
    while (i.hasNext()) {
        i.next();
        if((1.0*qrand())/RAND_MAX < m_bestChooseChance)
        {
            Order order = i.value();
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
                continue;
            order.setPrice(price);

            qint32 maxAmount = m_store.getMoney()/price;
            double rand = (1.0*qrand())/RAND_MAX;
            qint32 amount = 3 + qrand()%13;
            if(rand > 0.45 && rand < 0.88)
                amount = 16 + qrand()%35;
            else if(rand >= 0.88)
                amount = 51 + qrand()%75;
            if(maxAmount)
                amount %= maxAmount;
            order.setAmount(amount);
            if(order.getAmount()&& order.getPrice())
                return order;
        }
    }

    Order order = i.value();
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
    if(rand > 0.45 && rand < 0.88)
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

void changeTrendAgent::cancelOrder()
{
    QVector<qint32> toCancel = m_myOrders.olderOrders(m_cancelTime);
    foreach(qint32 orderId, toCancel)
        cancelReq(orderId);
}

void changeTrendAgent::changeData()
{
}
