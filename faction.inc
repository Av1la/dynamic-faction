
// # inclui nucleo
#include "kernel\faction\Faction.p"
#include "kernel\faction\MemberList.p"

// # inclui interface de interação
#include "kernel\faction\Faction.interface.p"
#include "kernel\faction\MemberList.interface.p"

// # inclui comandos
#include "kernel\faction\commands.p"


#include <YSI\y_hooks>

/**
 * OnGameModeInit
 *
 * Função chamada quando o gamemode é iniciado.
 *
 * @return (bool) (undefined)
 */
hook OnGameModeInit()
{
	new factions[10], member;
	factions[0] = Faction_Create("Exercito Brasileiro", "{800080}", 0x800080);
	factions[1] = Faction_Create("Grove Street Families", "{800080}", 0x800080);
	factions[2] = Faction_Create("Varrio Los Aztecas", "{00BFFF}", 0x00BFFF);
	factions[3] = Faction_Create("Front Yard Ballas", "{800080}", 0x800080);
	factions[4] = Faction_Create("Los Santos Vagos", "{00BFFF}", 0x00BFFF);

	Faction_SetRankName(factions[0], 1, "Recruta");
	Faction_SetRankName(factions[0], 2, "Soldado");
	Faction_SetRankName(factions[0], 3, "Cabo");
	Faction_SetRankName(factions[0], 4, "Sargento");
	Faction_SetRankName(factions[0], 5, "Subtenente");
	Faction_SetRankName(factions[0], 6, "Aspirante-a-Oficial");
	Faction_SetRankName(factions[0], 7, "Tenente");
	Faction_SetRankName(factions[0], 8, "Capitão");
	Faction_SetRankName(factions[0], 9, "Major");
	Faction_SetRankName(factions[0], 10, "Coronel");

	member = MemberList_Add("Diego_Rodrigues", 0, 10);
	MemberList_Add("Jogador_1", factions[0], 2);
	MemberList_Add("Jogador_2", factions[0], 3);
	MemberList_Add("Jogador_3", factions[0], 5);
	MemberList_Add("Jogador_4", factions[0], 1);
	MemberList_Add("Jogador_5", factions[0], 6);
	MemberList_Add("Jogador_7", factions[0], 1);
	MemberList_SetLeader(member, true);

	member = MemberList_Add("Matheus_Ferreira", factions[1], 10);
	MemberList_Add("Jogador_8", factions[1], 3);
	MemberList_Add("Jogador_9", factions[1], 3);
	MemberList_Add("Jogador_10", factions[1], 2);
	MemberList_Add("Jogador_11", factions[1], 4);
	MemberList_Add("Jogador_12", factions[1], 2);
	MemberList_Add("Jogador_13", factions[1], 1);
	MemberList_SetLeader(member, true);	
	return true;
}

hook OnPlayerSpawn(playerid)
{
	
	return true;
}



