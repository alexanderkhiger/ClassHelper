#ifndef QUERYRUNNER_H
#define QUERYRUNNER_H

#include <QObject>

class QueryRunner : public QObject
{
    Q_OBJECT
public:
    explicit QueryRunner(QObject *parent = nullptr);

signals:

public slots:
};

#endif // QUERYRUNNER_H