#include "ThreadPoolTest.h"

// Qt Includes
#include <QDebug>
#include <QEventLoop>
#include <QThread>
#include <QTime>

// Pro Includes
#include "TableDefine.h"
#include "JCDemoDataWidgetMapper.h"

/*!
 * @brief
 * @details
 * @note
 */
ThreadPoolTest::ThreadPoolTest(QObject *parent) : QObject(parent)
{
    // auto delete
    setAutoDelete(true);
    // start timer
    mTimerId = startTimer(100);
}

/*!
 * @brief
 * @details
 * @note
 */
ThreadPoolTest::~ThreadPoolTest()
{
    // kill timer
    killTimer(mTimerId);
    mTimerId = 0;
}

/*!
 * @brief main loop
 * @details
 * @note
 */
void ThreadPoolTest::run()
{
#if 1
    qDebug() << __FUNCTION__ << QThread::currentThread();
#endif

    QEventLoop eventLoop;
    eventLoop.exec();
}


/*!
 * @brief timer event
 * @details
 * @note
 */
void ThreadPoolTest::timerEvent(QTimerEvent *event)
{
    if (event->timerId() != mTimerId) {
        qDebug() << __FUNCTION__ << __LINE__;
        return;
    }

#if 0
    int value = JCDemoDataWidgetMapper::globalInstance()->data(TableRow::LineEdit).toInt();
    value++;
    JCDemoDataWidgetMapper::globalInstance()->setData(TableRow::LineEdit, value);
#else
    QVariant variant = QVariant();
    QMetaObject::invokeMethod(JCDemoDataWidgetMapper::globalInstance(), "data", Qt::AutoConnection,
                              Q_RETURN_ARG(QVariant, variant),
                              Q_ARG(int32_t, TableRow::LineEdit));

    int value = variant.toInt();
    value++;

    QMetaObject::invokeMethod(JCDemoDataWidgetMapper::globalInstance(), "setData", Qt::AutoConnection,
                              Q_ARG(int32_t, TableRow::LineEdit),
                              Q_ARG(QVariant, value));
#endif

    qDebug() << __PRETTY_FUNCTION__ << value;
}

