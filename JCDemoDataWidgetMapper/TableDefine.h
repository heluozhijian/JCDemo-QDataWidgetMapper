#ifndef TABLEDEFINE_H
#define TABLEDEFINE_H

#include <QString>

class TableName
{
public:
    static const QString Text;
};

class TableRow
{
public:
    enum T {
        LineEdit = 0,
        PushButton,
        Min = LineEdit,
        Max = PushButton,
        Count,
    };
    static const QString Text[Count];
};

class TableColumn
{
public:
    enum T {
        Key = 0,
        Value,
        PrimaryKey,
        Min = Key,
        Max = PrimaryKey,
        Count
    };
    static const QString Text[Count];
};

#endif // TABLEDEFINE_H
