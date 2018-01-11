#include "universitymodel.h"

UniversityModel::UniversityModel(QObject *parent) : QObject(parent)
{

}

void UniversityModel::updateModel(QSqlTableModel *model, operationType type, int row, QString arg1, QString arg2)
{
    if (type == uDELETE)
    {
        model->removeRow(row);
        int check = model->submitAll();
        if (!check)
        {
            emit updateError(model->lastError().text());
            model->revertAll();
        }
    }
    else if (type == uINSERT)
    {
        const int rowNumber = model->rowCount();
        model->insertRows(rowNumber, 1);
        model->setData(model->index(rowNumber, 1), arg1);
        model->setData(model->index(rowNumber, 2), arg2);
        int check = model->submitAll();
        if (!check)
        {
            emit updateError(model->lastError().text());
            model->revertAll();
        }
    }
}
