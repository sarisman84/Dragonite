#include "PollingStation.h"

Dragonite::PollingStation::ManagerInfo::~ManagerInfo()
{
	myRefData = nullptr;
	myPtrData.reset();
}

Dragonite::PollingStation::~PollingStation()
{
	myUniqueDataMembers.clear();
}
