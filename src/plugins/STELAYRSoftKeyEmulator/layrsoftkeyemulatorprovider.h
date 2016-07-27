#ifndef LAYRSOFTKEYEMULATORPROVIDER_H
#define LAYRSOFTKEYEMULATORPROVIDER_H

#include <STESoftKeyProvider>

#include "layrcontroller.h"
#include "layronscreenbuttonsoftkey.h"

class LAYRSoftKeyEmulatorProvider : public STESoftKeyProvider
{
    Q_OBJECT

public:
    explicit LAYRSoftKeyEmulatorProvider(QObject *parent = 0);

    QList<STESoftKey*> createSoftKeys() Q_DECL_OVERRIDE;
    QQuickItem* createSoftKeyVisualization() Q_DECL_OVERRIDE;

private:
    LAYRController* controller;
    QList<STESoftKey*> softKeyList;

    LAYRController* getController();
};

#endif // LAYRSOFTKEYEMULATORPROVIDER_H
