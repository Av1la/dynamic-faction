

/*
	FactionVehicle_Define(vehicleid, faction, rank)
	FactionVehicle_GetIdFromVehicleid(vehicleid)

	FactionVehicle_Undef(faction_vehicle)

	FactionVehicle_IsDefined(faction_vehicle)
	FactionVehicle_GetFaction(faction_vehicle)
	FactionVehicle_GetRank(faction_vehicle)
	FactionVehicle_GetVehicleid(faction_vehicle)

	FactionVehicle_SetDefined(faction_vehicle, bool create)
	FactionVehicle_SetFaction(faction_vehicle, faction)
	FactionVehicle_SetRank(faction_vehicle, rank)
	FactionVehicle_SetVehicleid(faction_vehicle, vehicleid)
*/

#include <YSI\y_hooks>

#define FACTION_VEHICLE_LIMIT 	100
#define FACTION_VEHICLE_INVALID -1

enum E_FACTION_VEHICLE
{
	bool:FactionVehicle_Created,
	FactionVehicle_Faction,
	FactionVehicle_Rank,
	FactionVehicle_Vehicleid,
}

new FactionVehicle[FACTION_VEHICLE_LIMIT][E_FACTION_VEHICLE];
new gFactionVehicleIndex;


hook OnPlayerStateChange(playerid, newstate, oldstate)
{
    if(oldstate == PLAYER_STATE_ONFOOT && newstate == PLAYER_STATE_DRIVER) // Player entered a vehicle as a driver
    {
    	new tmp_str[128];
        new faction_vehicle = FactionVehicle_GetIdFromVehid(GetPlayerVehicleID(playerid));
        new member  = MemberList_GetMemberIdById(playerid);

        if(MemberList_GetFaction(member) != FactionVehicle_GetFaction(faction_vehicle))
        {
        	format(tmp_str, sizeof tmp_str, "Você precisa ser membro da facção %s para dirigir esse veiculo!", Faction_GetName(FactionVehicle_GetFaction(faction_vehicle)));
        	SendClientMessage(playerid, -1, tmp_str);
        	RemovePlayerFromVehicle(playerid);
        	return true;
        }

        if(MemberList_GetRank(member) < FactionVehicle_GetRank(faction_vehicle))
        {
			format(tmp_str, sizeof tmp_str, "Você precisa ser rank %s(%d) da facção %s para dirigir esse veiculo!", Faction_GetRankName(faction_vehicle, FactionVehicle_GetRank(faction_vehicle)), FactionVehicle_GetRank(faction_vehicle), Faction_GetName(FactionVehicle_GetFaction(faction_vehicle)));
        	SendClientMessage(playerid, -1, tmp_str);
        	RemovePlayerFromVehicle(playerid);
        	return true;
        }
    }
    return 1;
}


stock FactionVehicle_Define(vehicleid, faction, rank)
{
	if(gFactionVehicleIndex >= FACTION_VEHICLE_LIMIT) {
		print("FactionVehicle - maximo de veiculos atingido!");
		return FACTION_VEHICLE_INVALID;
	}

	if(FactionVehicle_IsDefined(gFactionVehicleIndex))
	{
		printf("FactionVehicle - criando veiculo ja alocado (%d) realocando para proximo id (%d)", gFactionVehicleIndex, 1 + gFactionVehicleIndex);
		gFactionVehicleIndex++;
		return FactionVehicle_Define(vehicleid, faction, rank); 
	}

	printf("FactionVehicle - definindo veiculo %d", gFactionVehicleIndex);
	FactionVehicle_SetDefined(gFactionVehicleIndex, true);
	FactionVehicle_SetFaction(gFactionVehicleIndex, faction);
	FactionVehicle_SetRank(gFactionVehicleIndex, rank);
	FactionVehicle_SetVehicleid(gFactionVehicleIndex, vehicleid);	

	return (gFactionVehicleIndex++);
}

stock FactionVehicle_Undef(faction_vehicle)
{
	FactionVehicle_SetDefined(faction_vehicle, false);
	FactionVehicle_SetFaction(faction_vehicle, -1);
	FactionVehicle_SetRank(faction_vehicle, 0);
	FactionVehicle_SetVehicleid(faction_vehicle, INVALID_VEHICLE_ID);

	gFactionVehicleIndex = faction_vehicle;
	return true;
}


/*                      
***  #####  ##### ######
***  ##     ##      ##   
***  ## ##  ####    ##  
***  ##  #  ##      ## 
***  #####  #####   ## 
*/

stock FactionVehicle_GetIdFromVehid(vehicleid)
{
	new tmp_id = FACTION_VEHICLE_INVALID;

	if(!IsValidVehicle(vehicleid)) {
		printf("veiculo invalido %d", vehicleid);
		return tmp_id;
	}

	for(new i = 0; i < FACTION_VEHICLE_LIMIT; i++)
	{
		if(!FactionVehicle_IsDefined(i))
			continue;

		printf("definido %d", i);
		if(FactionVehicle_GetVehicleid(i) == vehicleid)
		{
			printf("veh %d/%d, fac %d, rank %d", vehicleid, i, FactionVehicle_GetFaction(i), FactionVehicle_GetRank(i));
			tmp_id = i;
			break;
		}
	}
	return tmp_id;
}

stock FactionVehicle_IsDefined(faction_vehicle)
{
	return FactionVehicle[faction_vehicle][FactionVehicle_Created];
}

stock FactionVehicle_GetFaction(faction_vehicle)
{
	return FactionVehicle[faction_vehicle][FactionVehicle_Faction];
}

stock FactionVehicle_GetRank(faction_vehicle)
{
	return FactionVehicle[faction_vehicle][FactionVehicle_Rank];
}

stock FactionVehicle_GetVehicleid(faction_vehicle)
{
	return FactionVehicle[faction_vehicle][FactionVehicle_Vehicleid];
}


/*                      
***  #####  ##### ######
***  ##     ##      ##   
***  #####  ####    ##  
***     ##  ##      ## 
***  #####  #####   ## 
*/


stock FactionVehicle_SetDefined(faction_vehicle, bool:create)
{
	FactionVehicle[faction_vehicle][FactionVehicle_Created] = create;
	return true;
}

stock FactionVehicle_SetFaction(faction_vehicle, faction)
{
	FactionVehicle[faction_vehicle][FactionVehicle_Faction] = faction;
	return true;
}

stock FactionVehicle_SetRank(faction_vehicle, rank)
{
	FactionVehicle[faction_vehicle][FactionVehicle_Rank] = rank;
	return true;
}

stock FactionVehicle_SetVehicleid(faction_vehicle, vehicleid)
{
	FactionVehicle[faction_vehicle][FactionVehicle_Vehicleid] = vehicleid;
	return true;
}