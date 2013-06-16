#include "antitrendfolloweragent.h"

AntiTrendFollowerAgent::AntiTrendFollowerAgent(qint64 upTrendTime, qint64 downTrendTime,
                                       qint64 cancelTime, double bestChooseChance,
                                       int repeatTime, qint32 userId, QObject *parent)
    : Agent(repeatTime, std::max(upTrendTime, downTrendTime), userId, parent),
      m_upTrendTime(upTrendTime), m_downTrendTime(downTrendTime), m_cancelTime(cancelTime), m_bestChooseChance( bestChooseChance)
{
}


Order AntiTrendFollowerAgent::bestRateOrder()
{
    QMap<double, Order> bestOrders;
    for(int i = 2; i <=  STOKS_NUMBER; ++i)
    {
        double downTrendValue =  - m_stocksInfo[i].trendValue(m_downTrendTime) - 2*m_stocksInfo[i].trendValue(m_downTrendTime/2);
        if(m_store.contains(i) && m_stocksInfo[i].lastPrice() &&
                ( (m_stocksInfo[i].getBestSellPrice()*1.005 <=  m_store.getMoney() && m_stocksInfo[i].getBestSellPrice()) ||
                  (m_stocksInfo[i].lastPrice() <= m_store.getMoney()) ) )
        {
            bestOrders.insertMulti(downTrendValue, Order(Order::SELL, i, 0, 0));
        }
        if(  m_stocksInfo[i].lastPrice() &&
              ( (m_stocksInfo[i].getBestBuyPrice()*1.025 <=  m_store.getMoney() && m_stocksInfo[i].getBestBuyPrice()) ||
                (m_stocksInfo[i].lastPrice() <= m_store.getMoney()) ))
        {
            double upTrendValue1 = m_stocksInfo[i].trendValue(m_upTrendTime);
            double upTrendValue2 = 2*m_stocksInfo[i].trendValue(m_upTrendTime/2);
            double upTrendValue = 0;
            if(upTrendValue1 && upTrendValue2)
                upTrendValue = - 1.0/upTrendValue1 - 1.0/upTrendValue2;
            bestOrders.insertMulti(upTrendValue, Order(Order::BUY, i, 0, 0));
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
            qint32 amount = pow(pow(std::min(-i.key()/3.0, 1.075), 25.0 + qrand()%10), 2.0 + static_cast<double>(qrand())/RAND_MAX) *
                    (1 + static_cast<double>(2*qrand())/RAND_MAX) + 2 + qrand() % 5;
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
    qint32 amount = pow(pow(std::min(-i.key()/3.0, 1.075), 25.0 + qrand()%10), 2.0 + static_cast<double>(qrand())/RAND_MAX) *
            (1 + static_cast<double>(2*qrand())/RAND_MAX) + 2 + qrand() % 5;
    if(maxAmount)
        amount %= maxAmount;
    order.setAmount(amount);
    if(order.getAmount()&& order.getPrice())
        return order;
    else return Order();
}

void AntiTrendFollowerAgent::cancelOrder()
{
    QVector<qint32> toCancel = m_myOrders.olderOrders(m_cancelTime);
    foreach(qint32 orderId, toCancel)
        cancelReq(orderId);
}

void AntiTrendFollowerAgent::changeData()
{
}

