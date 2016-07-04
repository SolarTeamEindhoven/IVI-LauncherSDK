#include "stesoftkey.h"

static QList<STESoftKey*> SoftKeyList;

STESoftKey::STESoftKey(QObject* parent)
    : QObject(parent)
    , ID(SoftKeyList.count())
{
    SoftKeyList.append(this);
}

STESoftKey::~STESoftKey()
{
    SoftKeyList.removeOne(this);
}

const QList<STESoftKey*>& STESoftKey::getSoftKeyList()
{
    return SoftKeyList;
}
