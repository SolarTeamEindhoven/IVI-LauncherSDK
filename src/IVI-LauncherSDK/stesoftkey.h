#ifndef STESOFTKEY_H
#define STESOFTKEY_H

#include <QObject>
#include <QSize>

#include <IVI-LauncherSDK/qtstelaunchersdkglobal.h>

QT_BEGIN_NAMESPACE

class QWaylandSurface;
class STEAppInstance;

class Q_STE_LAUNCHERSDK_EXPORT STESoftKey : public QObject
{
    Q_OBJECT

public:
    STESoftKey(QObject* parent = nullptr);
    ~STESoftKey();

    uint32_t getID() const {return ID;}

    virtual QString getHint() const = 0;
    virtual bool isVisual() const = 0;
    virtual QSize getHintSize() const = 0;
    virtual void setSurface(STEAppInstance* appInstance, QWaylandSurface* newSurface) = 0;

signals:
    void triggerStateChange(uint32_t state);
    void triggerClick();
    void triggerRotated(int32_t ticks, int32_t angle);
    void hintSizeChanged(QSize hintSize);

private:
    uint32_t ID;
};

QT_END_NAMESPACE

#endif // STESOFTKEY_H
