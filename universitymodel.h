#ifndef UNIVERSITYMODEL_H
#define UNIVERSITYMODEL_H

#include <QSqlError>
#include <QSqlTableModel>

class UniversityModel : public QObject
{
    Q_OBJECT
public:
    explicit UniversityModel(QObject *parent = nullptr);

    enum operationType {uINSERT,uDELETE,uUPDATE};

signals:
    void updateError(QSqlError error);
public slots:
    void updateModel(QSqlTableModel *model, operationType, int row, QString arg1 = 0, QString arg2 = 0);
};

#endif // UNIVERSITYMODEL_H
