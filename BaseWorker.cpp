#include "BaseWorker.h"

bool BaseWorker::checkIfRestaurantIsClosed()
{
    std::lock_guard<std::mutex> isRestaurantClosedUniqueLock(m_rest.isRestaurantClosedMutex);
    return m_rest.isRestaurantClosed;
}
