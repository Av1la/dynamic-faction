#if !defined isnull
    #define isnull(%1) ((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
#endif


#include "kernel/faction/Faction.p"
#include "kernel/faction/MemberList.p"

#include "kernel/faction/Faction.interface.p"

#include "kernel/faction/commands.p"

#include <YSI\y_hooks>



hook OnGameModeInit()
{
	new factions[10];
	factions[0] = Faction_Create("Grove Street Families", "{800080}", 0x800080);
	factions[1] = Faction_Create("Varrio Los Aztecas", "{00BFFF}", 0x00BFFF);
	factions[2] = Faction_Create("Front Yard Ballas", "{800080}", 0x800080);
	factions[3] = Faction_Create("Los Santos Vagos", "{00BFFF}", 0x00BFFF);

	printf("faction %d criada", factions[0]);
	printf("faction %d criada", factions[1]);
	printf("faction %d criada", factions[2]);
	printf("faction %d criada", factions[3]);

	Faction_Delete(factions[0]);
	printf("faction %d apagada", factions[0]);

	factions[4] = Faction_Create("Front Yard Ballas", "{800080}", 0x800080);

	printf("faction %d criada", factions[4]);

	MemberList_Add("Diego_Rodrigues", 0, 10);
	MemberList_SetLeader(0, true);

	printf("leder da org 0 = %s", MemberList_GetName(MemberList_GetFactionLeader(0)));
	printf("leder da org 0 = %s", MemberList_GetName(MemberList_GetFactionLeader(1)));
	printf("leder da org 0 = %s", MemberList_GetName(MemberList_GetFactionLeader(2)));
	return true;
}




