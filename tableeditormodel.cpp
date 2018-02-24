#include "tableeditormodel.h"

TableEditorModel::TableEditorModel(QObject *parent) : QObject(parent)
{

}

void TableEditorModel::updateFacultyModel(QSqlTableModel *model, operationType type, int row, QString arg1, QString arg2, QString arg3)
{
    if (type == fDELETE)
    {
        model->removeRow(row);
        int check = model->submitAll();
        if (!check)
        {
            emit updateError(model->lastError().text());
            model->revertAll();
        }
    }
    else if (type == fINSERT)
    {
        const int rowNumber = model->rowCount();
        model->insertRows(rowNumber, 1);
        model->setData(model->index(rowNumber, 1), arg1);
        model->setData(model->index(rowNumber, 2), arg2);
        model->setData(model->index(rowNumber, 3), arg3);
        int check = model->submitAll();
        if (!check)
        {
            emit updateError(model->lastError().text());
            model->revertAll();
        }
    }
    else if (type == fUPDATE)
    {
        int check = model->submitAll();
        if (!check)
        {
            emit updateError(model->lastError().text());
            model->revertAll();
        }
    }
}

void TableEditorModel::updateTeacherModel(QSqlTableModel *model, operationType type, int row, QString arg1, QString arg2, QString arg3, QString arg4, QString arg5, QString arg6, QString arg7, QString arg8)
{
    if (type == TableEditorModel::fDELETE)
    {
        model->removeRow(row);
        int check = model->submitAll();
        if (!check)
        {
            emit updateError(model->lastError().text());
            model->revertAll();
        }
    }
    else if (type == TableEditorModel::fINSERT)
    {
        const int rowNumber = model->rowCount();
        model->insertRows(rowNumber, 1);
        model->setData(model->index(rowNumber, 1), arg1);
        model->setData(model->index(rowNumber, 2), arg2);
        model->setData(model->index(rowNumber, 3), arg3);
        model->setData(model->index(rowNumber, 4), arg4);
        model->setData(model->index(rowNumber, 5), arg5);
        model->setData(model->index(rowNumber, 6), arg6);
        model->setData(model->index(rowNumber, 7), arg7);

        int check = model->submitAll();
        if (!check)
        {
            emit updateError(model->lastError().text());
            model->revertAll();
        }
    }
}

void TableEditorModel::updateSpecialtyModel(QSqlTableModel *model, TableEditorModel::operationType type, int row, QString arg1, QString arg2)
{
    if (type == fDELETE)
    {
        model->removeRow(row);
        int check = model->submitAll();
        if (!check)
        {
            emit updateError(model->lastError().text());
            model->revertAll();
        }
    }
    else if (type == fINSERT)
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
    else if (type == fUPDATE)
    {
        int check = model->submitAll();
        if (!check)
        {
            emit updateError(model->lastError().text());
            model->revertAll();
        }
    }
}

void TableEditorModel::updateDisciplineModel(QSqlTableModel *model, TableEditorModel::operationType type, int row, QString arg1)
{
    if (type == fDELETE)
    {
        model->removeRow(row);
        int check = model->submitAll();
        if (!check)
        {
            emit updateError(model->lastError().text());
            model->revertAll();
        }
    }
    else if (type == fINSERT)
    {
        const int rowNumber = model->rowCount();
        model->insertRows(rowNumber, 1);
        model->setData(model->index(rowNumber, 1), arg1);
        int check = model->submitAll();
        if (!check)
        {
            emit updateError(model->lastError().text());
            model->revertAll();
        }
    }
    else if (type == fUPDATE)
    {
        int check = model->submitAll();
        if (!check)
        {
            emit updateError(model->lastError().text());
            model->revertAll();
        }
    }
}
