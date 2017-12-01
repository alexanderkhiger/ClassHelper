#ifndef TABLEEDITORMODEL_H
#define TABLEEDITORMODEL_H

#include <QObject>
#include <QSqlError>
#include <QSqlTableModel>

class TableEditorModel : public QObject
{
    Q_OBJECT
public:
    explicit TableEditorModel(QObject *parent = nullptr);
    enum operationType {fINSERT,fDELETE,fUPDATE};

signals:
    void updateError(QSqlError error);
public slots:
    void updateFacultyModel(QSqlTableModel *model, operationType type, int row = 0, QString arg1 = 0, QString arg2 = 0, QString arg3 = 0);
    void updateTeacherModel(QSqlTableModel *model, operationType type, int row = 0, QString arg1 = 0, QString arg2 = 0, QString arg3 = 0, QString arg4 = 0, QString arg5 = 0, QString arg6 = 0, QString arg7 = 0, QString arg8 = 0);
};

#endif // TABLEEDITORMODEL_H
