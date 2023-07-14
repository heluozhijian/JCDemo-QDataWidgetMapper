#include "JCDemoDataWidgetMapper.h"

#include <QApplication>
#include <QDataWidgetMapper>
#include <QDebug>
#include <QFile>
#include <QGlobalStatic>
#include <QLayout>
#include <QMessageBox>
#include <QThreadPool>

#include "TableDefine.h"

Q_GLOBAL_STATIC(JCDemoDataWidgetMapper, theInstance)

JCDemoDataWidgetMapper::JCDemoDataWidgetMapper(QWidget *parent) : QWidget(parent)
{
    setWindowTitle(QString("DataWidgetMapper"));
    resize(480, 320);

    // setup ui
    setupUi();

    // connect database
    if (connectDatabase() == false) {
        QMessageBox::critical(this, QString("Error"), QString("Can not connect database!"), QMessageBox::Abort);
    }

    // create table
    verifyTable(database, TableName::Text);

    // setup model
    setupModel();

    // show ui
    this->show();

#if MAIN_THREAD_TIMER_ENABLE
    // start timer
    mTimerId = startTimer(500);
#endif

#if 1
    qDebug() << __FUNCTION__ << QThread::currentThread();
#endif
}

JCDemoDataWidgetMapper::~JCDemoDataWidgetMapper()
{
#if MAIN_THREAD_TIMER_ENABLE
    /* kill timer */
    killTimer(mTimerId);
    mTimerId = 0;
#endif
}

/*!
 * @brief global instance
 * @details
 * @note
 */
JCDemoDataWidgetMapper *JCDemoDataWidgetMapper::globalInstance()
{
    return theInstance();
}

/*!
 * @brief setup ui
 * @details
 * @note
 */
void JCDemoDataWidgetMapper::setupUi()
{
    lineEdit = new QLineEdit(QString::number(1), this);

    pushButton = new QPushButton(QString("true"), this);
    QObject::connect(pushButton, SIGNAL(clicked()), this, SLOT(onPushButtonClicked()));

    QHBoxLayout *mainLayout = new QHBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(20, 0, 20, 0); // left top right bottom
    mainLayout->addWidget(lineEdit, 1);
    mainLayout->addWidget(pushButton, 1);
}

/*!
 * @brief pushbutton clicked slot
 * @details
 * @note
 */
void JCDemoDataWidgetMapper::onPushButtonClicked()
{
    if (pushButton->text() == QString("true")) {
        pushButton->setText(QString("false"));
    } else {
        pushButton->setText(QString("true"));
    }
}

/*!
 * @brief setup model
 * @details
 * @note
 */
void JCDemoDataWidgetMapper::setupModel()
{
    // table model
    mSqlTableModel = new QSqlTableModel(this, database);
    mSqlTableModel->setTable(TableName::Text);
    mSqlTableModel->setEditStrategy(QSqlTableModel::OnFieldChange); // OnFieldChange
    mSqlTableModel->select();

    // data widget mapper
    QDataWidgetMapper *dataWidgetMapper = new QDataWidgetMapper(this);
    dataWidgetMapper->setOrientation(Qt::Vertical);
    dataWidgetMapper->setSubmitPolicy(QDataWidgetMapper::AutoSubmit);
    dataWidgetMapper->setModel(mSqlTableModel);
    for (int32_t rowIndex = TableRow::Min; rowIndex <= TableRow::Max; rowIndex++) {
        switch (rowIndex) {
        case TableRow::LineEdit:
            dataWidgetMapper->addMapping(lineEdit, rowIndex);
            break;
        case TableRow::PushButton:
            dataWidgetMapper->addMapping(pushButton, rowIndex, "text");
            break;
        default:
            break;
        }
    }
    dataWidgetMapper->setCurrentIndex(TableColumn::Value);
}

/*!
 * @brief get data
 * @details
 * @note
 */
QVariant JCDemoDataWidgetMapper::data(const int32_t row)
{
    QReadLocker locker(&mDataLock);
    if ((row < TableRow::Min) || (row > TableRow::Max))
        return QVariant();
    return mSqlTableModel->data(mSqlTableModel->index(row, TableColumn::Value), Qt::EditRole);
}

/*!
 * @brief set data
 * @details
 * @note
 */
void JCDemoDataWidgetMapper::setData(const int32_t row, const QVariant value)
{
    QWriteLocker locker(&mDataLock);
    if ((row < TableRow::Min) || (row > TableRow::Max))
        return;
    mSqlTableModel->setData(mSqlTableModel->index(row, TableColumn::Value), value, Qt::EditRole);
}

#if MAIN_THREAD_TIMER_ENABLE
/*!
 * @brief timer event
 * @details
 * @note
 */
void JCDemoDataWidgetMapper::timerEvent(QTimerEvent *event)
{
    if (event->timerId() != mTimerId) {
        qDebug() << __FUNCTION__ << __LINE__;
        return;
    }

    int value = mSqlTableModel->data(mSqlTableModel->index(TableRow::LineEdit, TableColumn::Value), Qt::EditRole).toInt();
    value++;
    mSqlTableModel->setData(mSqlTableModel->index(TableRow::LineEdit, TableColumn::Value), value, Qt::EditRole);
    qDebug() << __PRETTY_FUNCTION__ << value;
}
#endif


