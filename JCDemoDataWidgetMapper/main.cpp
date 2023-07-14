#include "JCDemoDataWidgetMapper.h"

#include <QApplication>
#include <QDebug>
#include <QThread>
#include <QThreadPool>

#include "JCDemoDataWidgetMapper.h"
#include "ThreadPoolTest.h"
#include "main.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    JCDemoDataWidgetMapper::globalInstance();

#if THREAD_POOL_ENABLE
    ThreadPoolTest *threadPoolTest = new ThreadPoolTest(nullptr);
    QThreadPool::globalInstance()->start(threadPoolTest);
#endif

    return a.exec();
}
