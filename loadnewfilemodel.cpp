#include "loadnewfilemodel.h"
#include <QDebug>

LoadNewFileModel::LoadNewFileModel(QString uID, QObject *parent) : QObject(parent)
{
    receivedID = uID;
    runner = new QueryRunner;
}

QString LoadNewFileModel::rtfToPlainText(const QString rtf)
{
    QString strCopy;
    QChar ch = 0;
    bool bBrace = false;
    bool bSlash = false;
    int nLength = rtf.length();
    if (nLength < 1)
        return strCopy;
    bool bFirstLetter = false;
    for (int i = 0; i < nLength; i++)
    {
        ch = rtf[i];
        if (ch == '\\')
        {
            bSlash = true;
            continue;
        }
        else if (ch == ' ')
        {
            bSlash = false;
            if (!bFirstLetter)
                continue;
        }
        else if (ch == '{')
        {
            bBrace = true;
            bSlash = false;
            continue;
        }
        else if (ch == '}')
        {
            bSlash = false;
            bBrace = false;
            continue;
        }

        if (!bSlash && !bBrace)
        {
            if (!bFirstLetter)
                bFirstLetter = true;
            strCopy += ch;
            continue;
        }

    }
    qDebug() << strCopy;
    return strCopy;
}

void LoadNewFileModel::convertRtf(const QString dir)
{
    QFile file (dir);
    if(file.open(QIODevice::ReadOnly))
    {
        QTextStream stream(&file);
        stream.setCodec("Windows-1251");
        QString wholeText = "";
        QString singleLine;
        while (!stream.atEnd())
        {
            singleLine=stream.readLine();
            qDebug() << singleLine;
            wholeText += rtfToPlainText(singleLine);
        }
    }
}

void LoadNewFileModel::processData(const QString dir)
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
                //______________________________________________________________вставка специальности начата_______________________________________________

                int facultyId = 1;
                QSqlQuery returnedQuery;
                int querySize = 5;


                if (facultyName != "")


                    query = QString("SELECT id_fakulteta FROM fakultet WHERE nazvanie_fakulteta = '%1'").arg(facultyName);
                    returnedQuery = runner->tryQuery(query);
                    querySize = returnedQuery.size();

                    if (querySize == 1)
                    {
                        returnedQuery.next();
                        facultyId = returnedQuery.value("id_fakulteta").toInt();
                    }

                    else if (querySize == 0)
                    {
                        mySelector = new DataSelectorView("fakultet",facultyName,"noValue","noValue",receivedID);
                        mySelector->show();
                        //                    QTimer timer;
                        //                    timer.setSingleShot(true);
                        QEventLoop loop;
                        connect(mySelector,SIGNAL(sendData()),&loop,SLOT(quit()));
                        //                    connect(&timer,SIGNAL(timeout()),&loop,SLOT(quit()));
                        //                    timer.start();
                        loop.exec();
                        facultyId = mySelector->faculty_id;
                        mySelector->close();
                        delete mySelector;
                    }


                    query = QString("SELECT id_spec FROM specialnost WHERE nazvanie_spec = '%1' AND id_fakulteta = %2").arg(specialty).arg(facultyId);
                    //                delete runner;
                    //                runner = new QueryRunner;
                    returnedQuery = runner->tryQuery(query);
                    querySize = returnedQuery.size();
                    int specialtyID = 0;

                    if (querySize == 1)
                    {
                        returnedQuery.next();
                        specialtyID = returnedQuery.value("id_spec").toInt();
                    }


                    else if (querySize >= 0)
                    {
                        mySelector = new DataSelectorView("specialnost","noValue",specialty,"noValue",QString::number(facultyId));
                        mySelector->show();
                        QEventLoop loop;
                        connect(mySelector,SIGNAL(sendData()),&loop,SLOT(quit()));
                        loop.exec();
                        specialtyID = mySelector->specialty_id;
                        mySelector->close();
                        delete mySelector;
                    }


                //______________________________________________________________вставка специальности закончена_______________________________________________




                studentsCount = QString(streamInfo[8]).remove(" ").toInt();

                groupsCount = QString(streamInfo[10]).remove(" ").toInt();

                subgroupsCount = QString(streamInfo[12]).remove(" ").toInt();

                //                qDebug() << year << studentsCount << groupsCount << subgroupsCount;

                singleLine=stream.readLine();
                singleLine=stream.readLine();

                //

                ////______________________________________________________________вставка групп начата_______________________________________________

                //                for (int i=1; i<groupsCount+1; i++)
                //                {
                //                    QString groupName = specialty + " - " +QString::number(i);
                //                    query = QString("INSERT INTO gruppa(nazvanie_gruppy,kolvo_studentov,kurs,id_spec,kolvo_podgrupp,abbr_gruppy) VALUES ('%1',%2,%3,%4,%5,'%6')").arg(groupName)
                //                            .arg(studentsCount).arg(year).arg(specialtyID).arg(subgroupsCount).arg(groupName);
                //                    runner->tryQuery(query);
                //                }

                ////______________________________________________________________вставка групп закончена_______________________________________________

                //______________________________________________________________вставка потока начата_______________________________________________


                query = QString("SELECT id_potoka FROM potok WHERE kurs = %1 AND id_spec = %2").arg(year).arg(specialtyID);
                returnedQuery = runner->tryQuery(query);
                querySize = returnedQuery.size();
                int streamID = 0;

                if (querySize == 1)
                {
                    returnedQuery.next();
                    streamID = returnedQuery.value("id_potoka").toInt();
                }

                else if (querySize == 0)
                {
                    QString streamName = specialty + tr(" (Переименуйте)");
                    query = QString("INSERT INTO potok(nazvanie_potoka,kolvo_studentov,kurs,id_spec,kolvo_podgrupp,abbr_potoka,kolvo_grupp) VALUES ('%1',%2,%3,%4,%5,'%6',%7)")
                            .arg(streamName).arg(studentsCount).arg(year).arg(specialtyID).arg(subgroupsCount).arg(streamName).arg(groupsCount);
                    runner->tryQuery(query);

                    query = QString("SELECT LAST_INSERT_ID() AS id");
                    returnedQuery = runner->tryQuery(query);
                    returnedQuery.next();
                    streamID = returnedQuery.value(0).toInt();

                }








                //______________________________________________________________вставка потока закончена_______________________________________________



                disciplineInfo = singleLine.split("|");

                //                qDebug() << disciplineInfo;

                disciplineName = disciplineInfo[1];
                disciplineName.remove("  ");
                lectureMultiplier = QString(disciplineInfo[2]).remove(" ").toDouble();
                seminarMultiplier = QString(disciplineInfo[3]).remove(" ").toDouble();
                labMultiplier = QString(disciplineInfo[4]).remove(" ").toDouble();
                kont = QString(disciplineInfo[7]).remove(" ").toDouble(); // consider rename
                refr = QString(disciplineInfo[8]).remove(" ").toDouble(); // consider rename

                //______________________________________________________________вставка дисциплины начата_______________________________________________

                query = QString("SELECT id_discipliny FROM disciplina WHERE nazvanie_discipliny = '%1'").arg(disciplineName);
                returnedQuery = runner->tryQuery(query);
                querySize = returnedQuery.size();
                int disciplineID = 0;

                if (querySize == 1)
                {
                    returnedQuery.next();
                    disciplineID = returnedQuery.value("id_discipliny").toInt();
                }

                else if (querySize == 0)
                {

                    mySelector = new DataSelectorView("disciplina","noValue","noValue",disciplineName,0);
                    mySelector->show();
                    QEventLoop loop;
                    connect(mySelector,SIGNAL(sendData()),&loop,SLOT(quit()));
                    loop.exec();
                    disciplineID = mySelector->discipline_id;
                    mySelector->close();
                    delete mySelector;
                }

//                query = QString("INSERT INTO disciplina(nazvanie_discipliny) VALUES ('%1')").arg(disciplineName);
//                runner->tryQuery(query);


//                query = QString("SELECT LAST_INSERT_ID() AS id");
//                returnedQuery = runner->tryQuery(query);
//                returnedQuery.next();
//                disciplineID = returnedQuery.value(0).toInt();
                //______________________________________________________________вставка дисциплины закончена_______________________________________________

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


                QString reportType = "";

                if (zExamHours == 0 && examHours == 0 && gExamHours == 0)
                {
                    reportType = tr("Нет отчетности");
                }
                else if (zExamHours != 0 && examHours == 0 && gExamHours == 0)
                {
                    reportType = tr("Зачет");
                }
                else if (zExamHours == 0 && examHours != 0 && gExamHours == 0)
                {
                    reportType = tr("Экзамен");
                }
                else if (zExamHours == 0 && examHours == 0 && gExamHours != 0)
                {
                    reportType = tr("Госэкзамен");
                }

                //
               // QString streamName = specialty + tr(" (Переименуйте)");
                query = QString("INSERT INTO zanyatost(id_potoka,id_discipliny,vid_itog_otch,nedeli,lekcii_chasov,seminary_chasov,lab_chasov,kontrol_chasov,konsultacii_chasov,"
                                "zachet_chasov,ekzamen_chasov,semestr,kursovie_chasov,ucheb_praktika_chasov,proizv_praktika_chasov,preddipl_praktika_chasov,vkl_chasov,obz_lekcii_chasov,gek_chasov,nirs_chasov,"
                                "asp_dokt_chasov,lekcii_ed_v_ned,sem_ed_v_ned,lab_ed_v_ned) VALUES (%1,%2,'%3',%4,%5,%6,%7,%8,%9,%10,%11,%12,%13,%14,%15,%16,%17,%18,%19,%20,%21,%22,%23,%24)")
                        .arg(streamID)
                        .arg(disciplineID)
                        .arg(reportType)
                        .arg(weeks)
                        .arg(lectureHours)
                        .arg(seminarHours)
                        .arg(labHours)
                        .arg(kontHours)
                        .arg(consultEx+consultTek)
                        .arg(zExamHours)
                        .arg(examHours)
                        .arg(semester)
                        .arg(qualHours)
                        .arg(practiceHours)
                        .arg(0)
                        .arg(0)
                        .arg(0)
                        .arg(0)
                        .arg(0)
                        .arg(0)
                        .arg(0)
                        .arg(lectureMultiplier)
                        .arg(seminarMultiplier)
                        .arg(labMultiplier);



                runner->tryQuery(query);




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
