#ifndef CUSTOMDOUBLEVALIDATOR_H
#define CUSTOMDOUBLEVALIDATOR_H

#include <QWidget>
#include <QDoubleValidator>

class CustomDoubleValidator : public QDoubleValidator
{
    Q_OBJECT
public:
    CustomDoubleValidator(double bottom, double top, int decimals, QObject *parent = 0)
            :QDoubleValidator(bottom, top, decimals, parent) {}
        QValidator::State validate(QString &input, int &pos) const
        {
            const QValidator::State origState = QDoubleValidator::validate( input, pos );
            if( ( origState == QValidator::Intermediate ) && ( input.toDouble() > top() ) )
            {
                return QValidator::Invalid;
            }
            else
            {
                return origState;
            }
        }
};

#endif // CUSTOMDOUBLEVALIDATOR_H
