#include "JCDemoDataWidgetMapper.h"

#include <QDebug>
#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlTableModel>

#include "TableDefine.h"

void JCDemoDataWidgetMapper::verifyTable(QSqlDatabase db, QString tableName)
{
    if (tableExist(db, tableName) == true) {
        if (tableCorrect(db, tableName) == false) {
            if (dropTable(db, tableName) == false) {
                QMessageBox::critical(this, QString("Error"), QString("Can not drop table!"), QMessageBox::Abort);
                return;
            }
            if (createTable(db, tableName) == false) {
                QMessageBox::critical(this, QString("Error"), QString("Can not create table! - 1"), QMessageBox::Abort);
                return;
            }
        }
    } else {
        if (createTable(db, tableName) == false) {
            QMessageBox::critical(this, QString("Error"), QString("Can not create table! - 2"), QMessageBox::Abort);
            return;
        }
    }
}

/*!
 * @brief is table exist
 * @details
 * @note
 */
bool JCDemoDataWidgetMapper::tableExist(QSqlDatabase db, QString tableName)
{
    QSqlQuery query(db);
    QString sql = QString("select * from sqlite_master where name='%1'").arg(tableName);
    query.prepare(sql);
    query.exec(sql);
    return query.next();
}

/*!
 * @brief is table correct
 * @details
 * @note
 */
bool JCDemoDataWidgetMapper::tableCorrect(QSqlDatabase db, QString tableName)
{
    // 1 - temp variable
    int32_t rowCount = 0;
    int32_t columnCount = 0;

    // 2 - rowCount & columnCount
    QSqlTableModel *model = new QSqlTableModel(this, db);
    model->setTable(tableName);
    model->select();
    rowCount = model->rowCount();
    columnCount = model->columnCount();
    delete model;

    // 3 - column count correct
    if ((rowCount != TableRow::Count) || (columnCount != TableColumn::Count)) {
        return false;
    }

    return true;
}

/*!
 * @brief drop table
 * @details
 * @note
 */
bool JCDemoDataWidgetMapper::dropTable(QSqlDatabase db, QString tableName)
{
    QSqlQuery query(db);
    QString sql = QString("DROP TABLE %1").arg(tableName);
    query.prepare(sql);
    return query.exec();
}

/*!
 * @brief create table
 * @details
 * @note
 */
bool JCDemoDataWidgetMapper::createTable(QSqlDatabase db, QString tableName)
{
    // 1 - temp variable
    QSqlQuery query(db);
    QString sql;

    // 2 - sql
    sql.clear();
    sql.append(QStringLiteral("CREATE TABLE IF NOT EXISTS %1").arg(tableName));
    sql.append(QStringLiteral("("));
    for (TableColumn::T columnIndex = TableColumn::Min; columnIndex <= TableColumn::Max; columnIndex = static_cast<TableColumn::T>(columnIndex + 1)) {
        // (1)
        if (columnIndex != TableColumn::PrimaryKey)
            sql.append(QStringLiteral("%1 VARCHAR(255)").arg(TableColumn::Text[columnIndex]));
        else
            sql.append(QStringLiteral("%1 INTEGER PRIMARY KEY AUTOINCREMENT").arg(TableColumn::Text[columnIndex]));
        // (2)
        if (columnIndex < TableColumn::Max)
            sql.append(QStringLiteral(","));
    }
    sql.append(QStringLiteral(")"));

    // 3 - create
    query.prepare(sql);
    if (!query.exec()){
        return false;
    }

    // 4 - insert
    QSqlTableModel *model = new QSqlTableModel(this, db);
    model->setTable(tableName);
    model->select(); // TODO
    for (TableRow::T rowIndex = TableRow::Min; rowIndex <= TableRow::Max; rowIndex = static_cast<TableRow::T>(rowIndex + 1)) {
        // (1) insert row
        model->insertRow(rowIndex);

        // (2) default data
        for (int columnIndex = TableColumn::Min; columnIndex <= TableColumn::Max; columnIndex++) {
            switch (columnIndex) {
            case TableColumn::Key:
                model->setData(model->index(rowIndex, columnIndex), TableRow::Text[rowIndex]);
                break;
            case TableColumn::Value:
                switch (rowIndex) {
                case TableRow::LineEdit:
                    model->setData(model->index(rowIndex, columnIndex), 1);
                    break;
                case TableRow::PushButton:
                    model->setData(model->index(rowIndex, columnIndex), QString("true"));
                    break;
                default:
                    qDebug() << __FUNCTION__ << __LINE__;
                    break;
                }
                break;
            case TableColumn::PrimaryKey:
                break;
            default:
                qDebug() << __FUNCTION__ << __LINE__;
                break;
            }
        }

        // (3) Submit: 必须逐行提交
        model->submitAll();
    }
    delete model;

    // 5 - 返回
    return true;
}







