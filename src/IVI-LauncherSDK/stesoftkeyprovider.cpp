#include "stesoftkeyprovider.h"

STESoftKeyProvider::STESoftKeyProvider(QObject* parent)
    : QObject(parent)
{

}

QQuickItem* STESoftKeyProvider::createSoftKeyVisualization()
{
    return nullptr;
}

void STESoftKeyProvider::activeAppChanged(STEAppInstance* appInstance)
{
    ;
}
