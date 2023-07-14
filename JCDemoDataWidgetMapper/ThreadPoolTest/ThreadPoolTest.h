#ifndef THREADPOOLTEST_H
#define THREADPOOLTEST_H

// Qt Includes
#include <QObject>
#include <QRunnable>
#include <QTimerEvent>

/*!
 * @brief ThreadPoolTest
 * @details
 * @note
 */
class ThreadPoolTest : public QObject, public QRunnable
{
    Q_OBJECT
public:
    ThreadPoolTest(QObject *parent = nullptr);
    ~ThreadPoolTest();

private:
    void run();

private:
    int mTimerId = 0;
protected:
    virtual void timerEvent(QTimerEvent *event);
};

#endif // THREADPOOLTEST_H
