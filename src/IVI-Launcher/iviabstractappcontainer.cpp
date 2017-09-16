#include "iviabstractappcontainer.h"

#include <ivisingletonmanager_p.h>
#include <ivisurfacemanager.h>

IVIAbstractAppContainer::IVIAbstractAppContainer()
{
    IVISingletonManager::getSurfaceManager().registerAppContainer(this);
}

IVIAbstractAppContainer::~IVIAbstractAppContainer() {
    IVISingletonManager::getSurfaceManager().unregisterAppContainer(this);
}
