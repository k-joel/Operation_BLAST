#include "CAudioTrigger.h"

CAudioTrigger::CAudioTrigger()
{
	SetType(OBJ_AUDIO_TRIGGER);
	SetRenderType(RT_DONOTRENDER);
	SetIsAffectedByGrav(false);
}

CAudioTrigger::~CAudioTrigger()
{
}