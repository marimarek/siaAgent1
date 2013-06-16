#include "calmagent1.h"

calmAgent1::calmAgent1(qint64 historyTime, qint64 cancelTime, double diff, double bestChooseChange,
                       int repeatTime, int userId, QObject *parent)
    : Agent(repeatTime, historyTime, userId, parent), m_cancelTime(cancelTime), m_diff(diff), m_bestChooseChance(bestChooseChange)
{
    for(int i = 2; i <= STOKS_NUMBER; ++i)
        m_myPrognozePrice[i] = PRICES[i] * (0.97 + (0.06 * qrand())/RAND_MAX);
}

Order calmAgent1::bestRateOrder()
{
    QMap<double, Order> bestOrders;
    for(int i = 2; i <=  STOKS_NUMBER; ++i)
    {
        if(m_store.contains(i) && m_stocksInfo[i].lastPrice() &&
                ( (m_stocksInfo[i].getBestSellPrice()*1.005 <=  m_store.getMoney() && m_stocksInfo[i].getBestSellPrice()) ||
                  (m_stocksInfo[i].lastPrice() <= m_store.getMoney()) ) )
        {
            if(m_stocksInfo[i].getBestSellPrice())
                bestOrders.insertMulti(static_cast<double>(m_myPrognozePrice[i])/m_stocksInfo[i].getBestSellPrice(), Order(Order::SELL, i, 0, 0));
            else bestOrders.insertMulti(static_cast<double>(m_myPrognozePrice[i])/m_stocksInfo[i].lastPrice(), Order(Order::SELL, i, 0, 0));
        }
        if(  m_stocksInfo[i].lastPrice() &&
              ( (m_stocksInfo[i].getBestBuyPrice()*1.025 <=  m_store.getMoney() && m_stocksInfo[i].getBestBuyPrice()) ||
                (m_stocksInfo[i].lastPrice() <= m_store.getMoney()) ))
        {
            if(m_stocksInfo[i].getBestBuyPrice())
                bestOrders.insertMulti(m_stocksInfo[i].getBestBuyPrice()/static_cast<double>(m_myPrognozePrice[i]), Order(Order::BUY, i, 0, 0));
            else bestOrders.insertMulti(m_stocksInfo[i].lastPrice()/static_cast<double>(m_myPrognozePrice[i]), Order(Order::BUY, i, 0, 0));
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
            qint32 amount = pow(pow(std::min(i.key(), 1.075), 25.0 + qrand()%10), 2.0 + static_cast<double>(qrand())/RAND_MAX) *
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
    qint32 amount = pow(pow(std::min(i.key(), 1.075), 25.0 + qrand()%10), 2.0 + static_cast<double>(qrand())/RAND_MAX) *
            (1 + static_cast<double>(2*qrand())/RAND_MAX) + 2 + qrand() % 5;
    if(maxAmount)
        amount %= maxAmount;
    order.setAmount(amount);
    if(order.getAmount()&& order.getPrice())
        return order;
    else return Order();
}

void calmAgent1::cancelOrder()
{
    QVector<qint32> toCancel = m_myOrders.olderOrders(m_cancelTime);
    foreach(qint32 orderId, toCancel)
        cancelReq(orderId);
}

void calmAgent1::changeData()
{
    for(int i = 2; i <=  STOKS_NUMBER; ++i)
    {
        qint32 lowestDiff = m_stocksInfo[i].lowestDiff(m_historyTime, m_myPrognozePrice[i]);
        double diff = lowestDiff/static_cast<double>(m_myPrognozePrice[i]);
        if(m_myPrognozePrice[i] > lowestDiff && lowestDiff)
            diff = static_cast<double>(m_myPrognozePrice[i])/lowestDiff;

        if(diff > m_diff )
        {
            qint32 bestSellPrice = m_stocksInfo[i].getBestSellPrice();
            qint32 bestBuyPrice = m_stocksInfo[i].getBestBuyPrice();
            qint32 avg = (bestSellPrice + bestBuyPrice)/2;
            if(!bestBuyPrice)
            {
                avg = bestSellPrice;
            }
            if(!bestSellPrice)
            {
                avg = bestBuyPrice;
            }
            if(avg)
                m_myPrognozePrice[i] = avg * (0.97 + (0.06 * qrand())/RAND_MAX);
        }
    }
}
