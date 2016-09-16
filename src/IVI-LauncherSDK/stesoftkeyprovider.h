#ifndef STESOFTKEYPROVIDER_H
#define STESOFTKEYPROVIDER_H

#include <IVI-LauncherSDK/qtstelaunchersdkglobal.h>
#include <QOpenGLContext>

QT_BEGIN_NAMESPACE

class QWaylandCompositor;
class STESoftKey;
class QQuickItem;
class STEAppInstance;

class Q_STE_LAUNCHERSDK_EXPORT STESoftKeyProvider : public QObject
{
    Q_OBJECT

public:
    STESoftKeyProvider(QObject* parent = nullptr);

    virtual QList<STESoftKey*> createSoftKeys() = 0;
    virtual QQuickItem* createSoftKeyVisualization();
    virtual void activeAppChanged(STEAppInstance* appInstance);

signals:
    void changeActiveApp(STEAppInstance* appInstance);
};

QT_END_NAMESPACE

#endif // STESOFTKEYPROVIDER_H
