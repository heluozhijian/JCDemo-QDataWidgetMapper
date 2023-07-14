#include "TableDefine.h"

const QString TableName::Text = QStringLiteral("TableDemo");

const QString TableRow::Text[TableRow::Count] = {
    QStringLiteral("LineEdit"),
    QStringLiteral("PushButton"),
};

const QString TableColumn::Text[TableColumn::Count] = {
    QStringLiteral("Key"),
    QStringLiteral("Value"),
    QStringLiteral("PrimaryKey"),
};
