#ifndef LOADNEWFILELOGIC_H
#define LOADNEWFILELOGIC_H

#include <QObject>

class QString;

class LoadNewFileLogic : public QObject
{
    Q_OBJECT
public:
    explicit LoadNewFileLogic(QObject *parent = nullptr);

signals:
    void progress(const int percentage);
    void processingFinished();

private slots:
    void processData(const QString dir);
};

#endif // LOADNEWFILELOGIC_H
