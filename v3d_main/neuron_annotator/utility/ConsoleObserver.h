#ifndef CONSOLEOBSERVER_H
#define CONSOLEOBSERVER_H

#include <QtGui>
#include "JacsUtil.h"
#include "DataThread.h"
#include "../entity_model/Entity.h"
#include "../../webservice/console/cdsConsoleDataServiceProxy.h"
#include "../../webservice/impl/EntityAdapter.h"

class NaMainWindow;

class ConsoleObserver : public QObject
{
    Q_OBJECT
public:
    explicit ConsoleObserver(NaMainWindow *mainWindow = 0, QObject *parent = 0);
    void loadCurrentOntology();

signals:
    void openOntology(Entity *entity);
    void openMulticolorImageStack(QString filepath);

public slots:
    void ontologySelected(long rootId);
    void entitySelected(long entityId);
    void entityViewRequested(long entityId);
    void annotationsChanged(long entityId);

private slots:
    void loadOntologyResults(const void *results);
    void loadOntologyError(const QString & error);
    void entitySelectedResults(const void *results);
    void entitySelectedError(const QString & error);
    void entityViewRequestedResults(const void *results);
    void entityViewRequestedError(const QString & error);

private:
    NaMainWindow *mainWindow;
    DataThread *loadOntologyThread;
    DataThread *entitySelectedThread;
    DataThread *entityViewRequestedThread;
};

#endif // CONSOLEOBSERVER_H
