#include "cbase.h"
#include "ai_basenpc.h"
#include "npc_citizen17.h"

class CNPCHuman : public CNPC_Citizen
{
	DECLARE_CLASS(CNPCHuman, CNPC_Citizen);
public:
	void Spawn() override;
};