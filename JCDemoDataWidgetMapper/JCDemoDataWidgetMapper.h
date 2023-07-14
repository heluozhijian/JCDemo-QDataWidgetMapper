#ifndef JCDEMODATAWIDGETMAPPER_H
#define JCDEMODATAWIDGETMAPPER_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QReadWriteLock>
#include <QSqlDatabase>
#include <QSqlTableModel>

#include "main.h"

class JCDemoDataWidgetMapper : public QWidget
{
    Q_OBJECT

public:
    JCDemoDataWidgetMapper(QWidget *parent = nullptr);
    ~JCDemoDataWidgetMapper();

    static JCDemoDataWidgetMapper *globalInstance();           //!< global instance

    // UI
private:
    QLineEdit *lineEdit = nullptr;
    QPushButton *pushButton = nullptr;
    void setupUi();
private slots:
    void onPushButtonClicked();

    // setup model
private:
    void setupModel();

    /* 3. data */
public slots:
    QVariant data(const int32_t row);
    void setData(const int32_t row,
                 const QVariant value);
private:
    mutable QReadWriteLock mDataLock;

    // DB
private:
    QSqlDatabase database;                                      //!< database
    QString databaseFile(void);                                 //!< database file
    QString databaseConnectionName(void);                       //!< database connection name
    bool connectDatabase();
    bool disconnectDatabase();

    // Table
private:
    void verifyTable(QSqlDatabase db, QString tableName);       //!< verify table
    bool tableExist(QSqlDatabase db, QString tableName);        //!< is table exist
    bool tableCorrect(QSqlDatabase db, QString tableName);      //!< is table correct
    bool dropTable(QSqlDatabase db, QString tableName);         //!< drop table
    bool createTable(QSqlDatabase db, QString tableName);       //!< create table

    // Model
private:
    QSqlTableModel *mSqlTableModel = nullptr;

#if MAIN_THREAD_TIMER_ENABLE
    /* timer */
private:
    int mTimerId = 0;
protected:
    virtual void timerEvent(QTimerEvent *event);    //!< 定时器事件
#endif
};
#endif // JCDEMODATAWIDGETMAPPER_H
