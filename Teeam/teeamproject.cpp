#include "teeamproject.h"
#include "task.h"
#include "milestone.h"
#include "errors.h"

#include <QXmlStreamWriter>
#include <QXmlStreamReader>

TeeamProject::TeeamProject(QString projectName, QList<QString> peopleList)
{
    this->name = projectName;
    this->peopleList = peopleList;
    this->bChanged = false;
    this->projectChanged = false;
    this->taskGroupChanged = false;
	this->entitiesListChanged = false;
    this->isNew = true;
}

void TeeamProject::setName(QString projectName)
{
    this->name = projectName;
    projectChanged = true;
    bChanged = true;
    notify();
    projectChanged = false;
    bChanged = false;
}

void TeeamProject::setPeopleList(QList<QString> peopleList)
{
    this->peopleList = peopleList;
    projectChanged = true;
    bChanged = true;
    notify();
    projectChanged = false;
    bChanged = false;
}

void TeeamProject::AddTaskGroup(TaskGroup *taskGroup)
{
    taskGroupList.append(taskGroup);
    taskGroupChanged = true;
    bChanged = true;
    notify();
    taskGroupChanged = false;
    bChanged = false;
    taskGroup->setNew(false);
}

void TeeamProject::AddTaskOrMilestone(GenericTask *entity)
{
    entitiesList.append(entity);
    entitiesListChanged = true;
    bChanged = true;
    notify();
    entitiesListChanged = false;
    bChanged = false;
    entity->setNew(false);
}

void TeeamProject::AddTaskOrMilestoneToGroup(GenericTask *entity, int groupIndex)
{
    if(groupIndex < taskGroupList.length())
	{
		taskGroupList.at(groupIndex)->AddTask(entity);
		taskGroupList.at(groupIndex)->setChanged(true);
		taskGroupChanged = true;
		bChanged = true;
		notify();
		taskGroupList.at(groupIndex)->setChanged(false);
		taskGroupChanged = false;
		bChanged = false;
		entity->setNew(false);
	}
}

// TODO : trovare soluzione migliore??
void TeeamProject::Show()
{
    projectChanged = true;
    bChanged = true;
    notify();
    projectChanged = false;
    bChanged = false;
}

void TeeamProject::ShowGroups()
{
    // TODO : refactor!!
    for(int i = 0; i < taskGroupList.length(); i++)
    {
        taskGroupChanged = true;
        bChanged = true;
        taskGroupList.at(i)->setChanged(true);
        taskGroupList.at(i)->setGroupChanged(true);
        notify();
        taskGroupChanged = false;
        bChanged = false;
        taskGroupList.at(i)->setChanged(false);
        taskGroupList.at(i)->setGroupChanged(false);
    }
}

void TeeamProject::ShowEntities()
{
    for(int i = 0; i < entitiesList.length(); i++)
    {
        entitiesListChanged = true;
        bChanged = true;
        entitiesList.at(i)->setChanged(true);
        notify();
        entitiesListChanged = false;
        bChanged = false;
        entitiesList.at(i)->setChanged(false);
    }
}

void TeeamProject::RemoveTaskGroup(int index)
{
    if(index < taskGroupList.length())
    {
        taskGroupChanged = true;
        bChanged = true;
        taskGroupList.at(index)->setRemoved(true);
        notify();
        taskGroupChanged = false;
        bChanged = false;
        taskGroupList.removeAt(index);
    }
}

void TeeamProject::RemoveTaskOrMilestone(int index, int parent)
{
    if(parent == -1)
    {
        // è un task/milestone che non appartiene a nessun gruppo
        if(index < entitiesList.length())
        {
            entitiesListChanged = true;
            bChanged = true;
            entitiesList.at(index)->setRemoved(true);
            notify();
            entitiesListChanged = false;
            bChanged = false;
            entitiesList.removeAt(index);
        }
    }
    else if(parent < taskGroupList.length())
    {
        // è un task/milestone che non appartiene a nessun gruppo
        if(index < taskGroupList.at(parent)->GetEntitiesListSize())
        {
            taskGroupChanged = true;
            bChanged = true;
            taskGroupList.at(parent)->setChanged(true);
            taskGroupList.at(parent)->GetEntityAt(index)->setRemoved(true);
            notify();
            taskGroupChanged = false;
            bChanged = false;
            taskGroupList.at(parent)->setChanged(false);
            taskGroupList.at(parent)->RemoveEntityAt(index);
        }
    }
}

void TeeamProject::EditTaskOrMilestone(GenericTask *entity, int index, int parent)
{
    if(parent == -1)
    {
        if(index < entitiesList.length())
        {
            entitiesList.replace(index, entity);
            entity->setNew(false);
            entity->setChanged(true);
            entitiesListChanged = true;
            bChanged = true;
            notify();
            entitiesListChanged = false;
            bChanged = false;
            entity->setChanged(false);
        }
    }
    else
    {
        if(parent < taskGroupList.length())
        {
            if(index < taskGroupList.at(parent)->GetEntitiesListSize())
            {
                taskGroupList.at(parent)->ReplaceEntity(index, entity);
                entity->setNew(false);
                entity->setChanged(true);
                taskGroupList.at(parent)->setChanged(true);
                taskGroupChanged = true;
                bChanged = true;
                notify();
                taskGroupList.at(parent)->setChanged(false);
                taskGroupChanged = false;
                bChanged = false;
                entity->setChanged(false);
            }
        }
    }
}

void TeeamProject::SaveProjectAs(const QString filename)
{
    if(filename != "")
    {
        QFile file(filename);
        file.open(QIODevice::WriteOnly);

        QXmlStreamWriter xmlWriter(&file);
        xmlWriter.setAutoFormatting(true);
        xmlWriter.writeStartDocument();

        xmlWriter.writeComment("Do not modify!");

        // Inizio il salvataggio del progetto
        xmlWriter.writeStartElement(KEY_PROJECT);

        xmlWriter.writeStartElement(KEY_PROJECTPARAMETERS);

        // Salvo il nome
        xmlWriter.writeTextElement(KEY_NAME, GetName() );

        // Salvo l'elenco di persone
        for(int i = 0; i < GetPeopleList().length(); i++)
            xmlWriter.writeTextElement(KEY_PERSON, GetPeopleList().at(i));

        xmlWriter.writeEndElement();

        // Salvo i gruppi
        for(int i = 0; i < GetTaskGroupListSize(); i++)
        {
            xmlWriter.writeStartElement(KEY_GROUP);
            xmlWriter.writeTextElement(KEY_NAME, GetTaskGroupAt(i)->getName() );
            for(int j = 0; j < GetTaskGroupAt(i)->GetEntitiesListSize(); j++)
            {
                xmlWriter.writeStartElement(KEY_ENTITY);
                if(GetTaskGroupAt(i)->GetEntityAt(j)->getEntityType() == Task_type)
                {
                    xmlWriter.writeTextElement(KEY_ENTITYTYPE, KEY_TASKTYPE);
                    xmlWriter.writeTextElement(KEY_NAME, static_cast<Task*>(GetTaskGroupAt(i)->GetEntityAt(j))->getName());

                    for(int k = 0; k < static_cast<Task*>(GetTaskGroupAt(i)->GetEntityAt(j))->getPeople().length(); k++)
                        xmlWriter.writeTextElement(KEY_PERSON, static_cast<Task*>(GetTaskGroupAt(i)->GetEntityAt(j))->getPeople().at(k));

                    xmlWriter.writeTextElement(KEY_STARTDATETIME, static_cast<Task*>(GetTaskGroupAt(i)->GetEntityAt(j))->getStart().toString());
                    xmlWriter.writeTextElement(KEY_ENDDATETIME, static_cast<Task*>(GetTaskGroupAt(i)->GetEntityAt(j))->getEnd().toString());

                    xmlWriter.writeTextElement(KEY_COMPLETITION, QString::number(static_cast<Task*>(GetTaskGroupAt(i)->GetEntityAt(j))->getCompletition()));
                }
                else if(GetTaskGroupAt(i)->GetEntityAt(j)->getEntityType() == Milestone_type)
                {
                    xmlWriter.writeTextElement(KEY_ENTITYTYPE, KEY_MILESTONETYPE);
                    xmlWriter.writeTextElement(KEY_NAME, static_cast<Milestone*>(GetTaskGroupAt(i)->GetEntityAt(j))->getName());

                    for(int k = 0; k < static_cast<Milestone*>(GetTaskGroupAt(i)->GetEntityAt(j))->getPeople().length(); k++)
                        xmlWriter.writeTextElement(KEY_PERSON, static_cast<Milestone*>(GetTaskGroupAt(i)->GetEntityAt(j))->getPeople().at(k));

                    xmlWriter.writeTextElement(KEY_STARTDATETIME, static_cast<Milestone*>(GetTaskGroupAt(i)->GetEntityAt(j))->getDateTime().toString());
                }
                 xmlWriter.writeEndElement();
            }
            xmlWriter.writeEndElement();
        }

        // Salvo i task/milestone fuori dai gruppi
        for(int i = 0; i < GetEntitiesListSize(); i++)
        {
            xmlWriter.writeStartElement(KEY_ENTITY);
            if(GetEntityAt(i)->getEntityType() == Task_type)
            {
                xmlWriter.writeTextElement(KEY_ENTITYTYPE, KEY_TASKTYPE);
                xmlWriter.writeTextElement(KEY_NAME, static_cast<Task*>(GetEntityAt(i))->getName());

                for(int k = 0; k < static_cast<Task*>(GetEntityAt(i))->getPeople().length(); k++)
                    xmlWriter.writeTextElement(KEY_PERSON, static_cast<Task*>(GetEntityAt(i))->getPeople().at(k));

                xmlWriter.writeTextElement(KEY_STARTDATETIME, static_cast<Task*>(GetEntityAt(i))->getStart().toString());
                xmlWriter.writeTextElement(KEY_ENDDATETIME, static_cast<Task*>(GetEntityAt(i))->getEnd().toString());

                xmlWriter.writeTextElement(KEY_COMPLETITION, QString::number(static_cast<Task*>(GetEntityAt(i))->getCompletition()));
            }
            else if(GetEntityAt(i)->getEntityType() == Milestone_type)
            {
                xmlWriter.writeTextElement(KEY_ENTITYTYPE, KEY_MILESTONETYPE);
                xmlWriter.writeTextElement(KEY_NAME, static_cast<Milestone*>(GetEntityAt(i))->getName());

                for(int k = 0; k < static_cast<Milestone*>(GetEntityAt(i))->getPeople().length(); k++)
                    xmlWriter.writeTextElement(KEY_PERSON, static_cast<Milestone*>(GetEntityAt(i))->getPeople().at(k));

                xmlWriter.writeTextElement(KEY_STARTDATETIME, static_cast<Milestone*>(GetEntityAt(i))->getDateTime().toString());
            }
            xmlWriter.writeEndElement();
        }

        xmlWriter.writeEndElement();

        xmlWriter.writeEndDocument();

        file.close();
    }
}

int TeeamProject::OpenFile(const QString filename, AbstractView *view)
{
    try
    {
        QFile file(filename);
        if (!file.open(QFile::ReadOnly | QFile::Text))
        {
            throw INVALID_FILE;
        }

        QXmlStreamReader xmlReader;
        xmlReader.setDevice(&file);

        QXmlStreamReader::TokenType token = xmlReader.readNext();

        if(token == QXmlStreamReader::Invalid)
        {
            throw INVALID_FILE;
        }
        else if(token == QXmlStreamReader::StartDocument)
        {
            xmlReader.readNextStartElement();
        }

        this->SetNew(true);

        // Controllo che l'elemento più alto sia un project
        if(xmlReader.name() == KEY_PROJECT)
        {
            QString name;
            QStringList people;
            xmlReader.readNextStartElement();

            if(xmlReader.name() == KEY_PROJECTPARAMETERS)
            {
               xmlReader.readNextStartElement();
               if(xmlReader.name() == KEY_NAME)
                   name = xmlReader.readElementText();
               else
                   throw INVALID_FILE;

               xmlReader.readNextStartElement();
               while(xmlReader.name() == KEY_PERSON)
               {
                   people << xmlReader.readElementText();
                   xmlReader.readNextStartElement();
               }

               // Lasciare questa parte alla view??
               setName(name);
               setPeopleList(people);
            }

            int groupIndex = 0;
            while(xmlReader.readNextStartElement())
            {
                if(xmlReader.name() == KEY_GROUP)
                {
                   xmlReader.readNextStartElement();
                   if(xmlReader.name() == KEY_NAME)
                   {
                        TaskGroup *group = new TaskGroup(xmlReader.readElementText());
                        group->attach(view);
                        AddTaskGroup(group);

                        xmlReader.readNextStartElement();
                        while(xmlReader.name() == KEY_ENTITY)
                        {
                            xmlReader.readNextStartElement();
                            if(xmlReader.name() == KEY_ENTITYTYPE)
                            {
                                QString type = xmlReader.readElementText();

                                if(type == KEY_TASKTYPE)
                                {
                                    QString name;
                                    QDateTime start, end;
                                    QStringList taskPeople;
                                    int completition;

                                    xmlReader.readNextStartElement();
                                    if(xmlReader.name() == KEY_NAME)
                                        name = xmlReader.readElementText();
                                    else
                                        throw INVALID_FILE;

                                    xmlReader.readNextStartElement();
                                    while(xmlReader.name() == KEY_PERSON)
                                    {
                                        taskPeople << xmlReader.readElementText();
                                        xmlReader.readNextStartElement();
                                    }

                                    if(xmlReader.name() == KEY_STARTDATETIME)
                                        start = QDateTime::fromString(xmlReader.readElementText());
                                    else
                                        throw INVALID_FILE;

                                    xmlReader.readNextStartElement();
                                    if(xmlReader.name() == KEY_ENDDATETIME)
                                        end = QDateTime::fromString(xmlReader.readElementText());
                                    else
                                        throw INVALID_FILE;

                                    xmlReader.readNextStartElement();
                                    if(xmlReader.name() == KEY_COMPLETITION)
                                        completition = xmlReader.readElementText().toInt();
                                    else
                                        throw INVALID_FILE;

                                    Task *task = new Task(name, start, end, taskPeople, completition);
                                    task->attach(view);
                                    AddTaskOrMilestoneToGroup(task, groupIndex);
                                }
                                else if(type == KEY_MILESTONETYPE)
                                {
                                    QString name;
                                    QDateTime dateTime;
                                    QStringList milestonePeople;

                                    xmlReader.readNextStartElement();
                                    if(xmlReader.name() == KEY_NAME)
                                        name = xmlReader.readElementText();
                                    else
                                        throw INVALID_FILE;

                                    xmlReader.readNextStartElement();
                                    while(xmlReader.name() == KEY_PERSON)
                                    {
                                        milestonePeople << xmlReader.readElementText();
                                        xmlReader.readNextStartElement();
                                    }

                                    if(xmlReader.name() == KEY_STARTDATETIME)
                                        dateTime = QDateTime::fromString(xmlReader.readElementText());
                                    else
                                        throw INVALID_FILE;

                                    Milestone *milestone = new Milestone(name, dateTime, milestonePeople);
                                    milestone->attach(view);
                                    AddTaskOrMilestoneToGroup(milestone, groupIndex);
                                }
                                else
                                    throw INVALID_FILE;
                            }
                            else
                                throw INVALID_FILE;

                            xmlReader.readNextStartElement();
                            xmlReader.readNextStartElement();
                        }
                   }
                   else
                       throw INVALID_FILE;

                   groupIndex++;
                }
                else if(xmlReader.name() == KEY_ENTITY)
                {
                    xmlReader.readNextStartElement();

                    if(xmlReader.name() == KEY_ENTITYTYPE)
                    {
                        QString type = xmlReader.readElementText();

                        if(type == KEY_TASKTYPE)
                        {
                            QString name;
                            QDateTime start, end;
                            QStringList taskPeople;
                            int completition;

                            xmlReader.readNextStartElement();
                            if(xmlReader.name() == KEY_NAME)
                                name = xmlReader.readElementText();
                            else
                                throw INVALID_FILE;

                            xmlReader.readNextStartElement();
                            while(xmlReader.name() == KEY_PERSON)
                            {
                                taskPeople << xmlReader.readElementText();
                                xmlReader.readNextStartElement();
                            }

                            if(xmlReader.name() == KEY_STARTDATETIME)
                                start = QDateTime::fromString(xmlReader.readElementText());
                            else
                                throw INVALID_FILE;

                            xmlReader.readNextStartElement();
                            if(xmlReader.name() == KEY_ENDDATETIME)
                                end = QDateTime::fromString(xmlReader.readElementText());
                            else
                                throw INVALID_FILE;

                            xmlReader.readNextStartElement();
                            if(xmlReader.name() == KEY_COMPLETITION)
                                completition = xmlReader.readElementText().toInt();
                            else
                                throw INVALID_FILE;

                            Task *task = new Task(name, start, end, taskPeople, completition);
                            task->attach(view);
                            AddTaskOrMilestone(task);
                        }
                        else if(type == KEY_MILESTONETYPE)
                        {
                            QString name;
                            QDateTime dateTime;
                            QStringList milestonePeople;

                            xmlReader.readNextStartElement();
                            if(xmlReader.name() == KEY_NAME)
                                name = xmlReader.readElementText();
                            else
                                throw INVALID_FILE;

                            xmlReader.readNextStartElement();
                            while(xmlReader.name() == KEY_PERSON)
                            {
                                milestonePeople << xmlReader.readElementText();
                                xmlReader.readNextStartElement();
                            }

                            if(xmlReader.name() == KEY_STARTDATETIME)
                                dateTime = QDateTime::fromString(xmlReader.readElementText());
                            else
                                throw INVALID_FILE;

                            Milestone *milestone = new Milestone(name, dateTime, milestonePeople);
                            milestone->attach(view);
                            AddTaskOrMilestone(milestone);
                        }
                        else
                            throw INVALID_FILE;
                    }
                    else
                        throw INVALID_FILE;

                    xmlReader.readNextStartElement();
                }
                else
                    throw INVALID_FILE;
            }
        }
        else
            throw INVALID_FILE;
    }
    catch (int error)
    {
        this->SetNew(false);
        return error;
    }

    this->projectChanged = true;

    Show();

    for(int i = 0; i < taskGroupList.length(); i++)
        taskGroupList.at(i)->setNew(true);
    ShowGroups();

    for(int i = 0; i < entitiesList.length(); i++)
        entitiesList.at(i)->setNew(true);
    ShowEntities();

    this->projectChanged = false;

    this->SetNew(false);

    return NO_ERROR;
}


