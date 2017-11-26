#include "loadnewfilelogic.h"

LoadNewFileLogic::LoadNewFileLogic(QObject *parent) : QObject(parent)
{

}

void LoadNewFileLogic::processData(const QString dir)
{
    QFile file (dir);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        stream.setCodec("Windows-1251");
        QString singleLine;
        singleLine=stream.readLine();

        while (!stream.atEnd()&&!singleLine.contains("ИТОГИ")&&!singleLine.contains("Сегодня"))
        {
            singleLine=stream.readLine();
        }
        if (!stream.atEnd())
        {
            QString planDate = singleLine.split(":")[1];
            planDate.remove(" ");
            planDate = QDate::fromString(planDate, "MM/dd/yy").addYears(100).toString("yyyy.MM.dd");
            //            qDebug() << planDate;
        }
        else
            return;

        //


        while (!stream.atEnd()&&!singleLine.contains("ИТОГИ")&&!singleLine.contains("Кафедра"))
        {
            singleLine=stream.readLine();
        }

        if (!stream.atEnd())
        {
            singleLine[singleLine.indexOf("Кафедра")+7]='|';
            QString chairName = singleLine.split("|")[1];
            chairName.remove(" ");
            //            qDebug() << chairName;
        }
        else
            return;


        //

        while (!stream.atEnd()&&!singleLine.contains("ИТОГИ"))
        {
            while (singleLine[0]!="|")
                singleLine=stream.readLine();

            //

            singleLine=stream.readLine();
            singleLine=stream.readLine();
            singleLine=stream.readLine();
            singleLine=stream.readLine();

            //

            while (!stream.atEnd()&&!singleLine.contains("ИТОГИ")&&singleLine.contains("Факультет"))
            {

                singleLine[singleLine.indexOf("Факультет")+9]='|';
                singleLine[singleLine.indexOf("Курс")+4]='|';
                singleLine[singleLine.indexOf("Специальность")+13]='|';
                singleLine[singleLine.indexOf("Студентов")+9]='|';
                singleLine[singleLine.indexOf("Групп")+5]='|';
                singleLine[singleLine.indexOf("Подгрупп")+8]='|';

                singleLine[singleLine.indexOf("Курс")-1]='|';
                singleLine[singleLine.indexOf("Специальность")-1]='|';
                singleLine[singleLine.indexOf("Студентов")-1]='|';
                singleLine[singleLine.indexOf("Групп")-1]='|';
                singleLine[singleLine.indexOf("Подгрупп")-1]='|';


                streamInfo = singleLine.split("|");

                facultyName = streamInfo[2];
                facultyName.remove(" ");

                year = QString(streamInfo[4]).remove(" ").toInt();

                specialty = streamInfo[6];
                specialty.remove(" ");

                studentsCount = QString(streamInfo[8]).remove(" ").toInt();

                groupsCount = QString(streamInfo[10]).remove(" ").toInt();

                subgroupsCount = QString(streamInfo[12]).remove(" ").toInt();

                //                qDebug() << year << studentsCount << groupsCount << subgroupsCount;

                singleLine=stream.readLine();
                singleLine=stream.readLine();

                //

                disciplineInfo = singleLine.split("|");

                //                qDebug() << disciplineInfo;

                disciplineName = disciplineInfo[1];
                lectureMultiplier = QString(disciplineInfo[2]).remove(" ").toDouble();
                seminarMultiplier = QString(disciplineInfo[3]).remove(" ").toDouble();
                labMultiplier = QString(disciplineInfo[4]).remove(" ").toDouble();
                kont = QString(disciplineInfo[7]).remove(" ").toDouble(); // consider rename
                refr = QString(disciplineInfo[8]).remove(" ").toDouble(); // consider rename

                singleLine=stream.readLine();

                //

                singleLine[singleLine.indexOf("семестр")+7]='|';
                singleLine[singleLine.indexOf("нед")+3]='|';
                singleLine[singleLine.indexOf("Инд")+3]='|';

                singleLine[singleLine.indexOf("семестр")-1]='|';
                singleLine[singleLine.indexOf("нед")-1]='|';
                singleLine[singleLine.indexOf("Инд")-1]='|';

                semesterInfo = singleLine.split("|");

                //                qDebug() << semesterInfo;

                semester = QString(semesterInfo[1]).remove(" ").toInt();
                weeks = QString(semesterInfo[3]).remove(" ").toInt();
                individual = QString(semesterInfo[7]).remove(" ").toInt();


                lectureHours = QString(semesterInfo[8]).remove(" ").toDouble();
                seminarHours = QString(semesterInfo[9]).remove(" ").toDouble();
                labHours = QString(semesterInfo[10]).remove(" ").toDouble();

                consultTek = QString(semesterInfo[11]).remove(" ").toDouble(); // consider rename
                consultEx = QString(semesterInfo[12]).remove(" ").toDouble(); // consider rename

                kontHours = QString(semesterInfo[13]).remove(" ").toDouble(); // consider rename
                refrHours = QString(semesterInfo[14]).remove(" ").toDouble(); // consider rename

                zExamHours = QString(semesterInfo[15]).remove(" ").toDouble(); // consider rename
                examHours = QString(semesterInfo[16]).remove(" ").toDouble(); // consider rename
                gExamHours = QString(semesterInfo[17]).remove(" ").toDouble(); // consider rename

                practiceHours = QString(semesterInfo[18]).remove(" ").toDouble();
                qualHours = QString(semesterInfo[19]).remove(" ").toDouble();
                totalHours = QString(semesterInfo[20]).remove(" ").toDouble();
                //                qDebug() << totalHours;

                singleLine=stream.readLine();
                singleLine=stream.readLine();

                //

                singleLine[singleLine.indexOf("семестр")+7]='|';
                singleLine[singleLine.indexOf("нед")+3]='|';
                singleLine[singleLine.indexOf("Инд")+3]='|';

                singleLine[singleLine.indexOf("семестр")-1]='|';
                singleLine[singleLine.indexOf("нед")-1]='|';
                singleLine[singleLine.indexOf("Инд")-1]='|';

                disciplineInfo = singleLine.split("|");

                //                qDebug() << disciplineInfo;

                semester = QString(disciplineInfo[1]).remove(" ").toInt();
                weeks = QString(disciplineInfo[3]).remove(" ").toInt();
                individual = QString(disciplineInfo[7]).remove(" ").toInt();

                lectureMultiplier = QString(disciplineInfo[8]).remove(" ").toDouble();
                seminarMultiplier = QString(disciplineInfo[9]).remove(" ").toDouble();
                labMultiplier = QString(disciplineInfo[10]).remove(" ").toDouble();
                kont = QString(disciplineInfo[13]).remove(" ").toDouble(); // consider rename
                refr = QString(disciplineInfo[14]).remove(" ").toDouble(); // consider rename

                singleLine=stream.readLine();

                semesterInfo = singleLine.split("|");

                //                qDebug() << semesterInfo;

                lectureHours = QString(semesterInfo[2]).remove(" ").toDouble();
                seminarHours = QString(semesterInfo[3]).remove(" ").toDouble();
                labHours = QString(semesterInfo[4]).remove(" ").toDouble();

                consultTek = QString(semesterInfo[5]).remove(" ").toDouble(); // consider rename
                consultEx = QString(semesterInfo[6]).remove(" ").toDouble(); // consider rename

                kontHours = QString(semesterInfo[7]).remove(" ").toDouble(); // consider rename
                refrHours = QString(semesterInfo[8]).remove(" ").toDouble(); // consider rename

                zExamHours = QString(semesterInfo[9]).remove(" ").toDouble(); // consider rename
                examHours = QString(semesterInfo[10]).remove(" ").toDouble(); // consider rename
                gExamHours = QString(semesterInfo[11]).remove(" ").toDouble(); // consider rename

                practiceHours = QString(semesterInfo[12]).remove(" ").toDouble();
                qualHours = QString(semesterInfo[13]).remove(" ").toDouble();
                totalHours = QString(semesterInfo[14]).remove(" ").toDouble();
                //                qDebug() << totalHours;
                singleLine=stream.readLine();
                singleLine=stream.readLine();
                //

            }

        }

    }
    else
        return;

    emit processingFinished();
}
