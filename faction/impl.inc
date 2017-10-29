#if defined _inc_impl
	#undef _inc_impl
#endif

/*
	Faction_Create(name[], embed_color[], color_hex)
	Faction_Delete(faction);

	Faction_SetCreated(faction, bool:created)
	Faction_SetUsable(faction, bool:usable)
	Faction_SetType(faction, type)
	Faction_SetName(faction, name)
	Faction_SetColorVehicle(faction, color_vehicle)
	Faction_SetColorEmbed(faction, color_embbed)
	Faction_SetColorHex(faction, color_hex)
	Faction_SetMaxMembers(faction, max_members)
	Faction_SetRankName(faction, rank, rank_name[])

	Faction_IsCreated(faction)
	Faction_IsUsable(faction)
	Faction_GetType(faction)
	Faction_GetName(faction)
	Faction_GetColorVehicle(faction)
	Faction_GetColorEmbed(faction)
	Faction_GetColorHex(faction)
	Faction_GetMaxMembers(faction)
	Faction_GetRankName(faction, rank)
*/

#include <YSI\y_hooks>

hook OnGameModeInit()
{
	print("<Kernel\\Faction\\Faction.p>");
	return true;
}

/**
 * Faction_Create
 *
 * cria uma nova facção
 *
 * @param (string) (name) nome da faccao
 * @param (int) (type) tipo da faccao
 * @param (string) (embed_color) cor em embed
 * @param (int) (color_hex) cor em hexadecimal
 * @return (bool) (undefined)
 */
stock Faction_Create(name [], type)
{
	if(gFactionIndex >= FACTION_LIMIT)
		return -1;

	if(Faction_IsCreated(gFactionIndex))
	{
		gFactionIndex++;
		return Faction_Create(name, type);
	}

	Faction_SetCreated(gFactionIndex, true);
	Faction_SetUsable(gFactionIndex, false);

	Faction_SetType(gFactionIndex, type);
	Faction_SetName(gFactionIndex, name);
	Faction_SetColorVehicle(gFactionIndex, 1);
	Faction_SetColorEmbed(gFactionIndex, "{FFFFFF}");
	Faction_SetColorHex(gFactionIndex, 0x000000FF);
	Faction_SetMaxMembers(gFactionIndex, 10);

	return (gFactionIndex++);
}

/**
 * Faction_Delete
 *
 * deleta uma faccao
 * quando a faccao é deletada o index de criacao de faccao é igualado ao id que foi apagado por ultimo. (gFactionIndex)
 *
 * @param (int) (facion) id da facção
 * @return (bool) (undefined)
 */
stock Faction_Delete(faction)
{
	Faction_SetName(faction, "");
	Faction_SetColorEmbed(faction, "");
	Faction_SetColorHex(faction, 0);
	Faction_SetMaxMembers(faction, 0);

	Faction_SetCreated(faction, false);
	Faction_SetUsable(faction, false);

	gFactionIndex = faction;
	return true;
}


/*                      
***  #####  ##### ######
***  ##     ##      ##   
***  ## ##  ####    ##  
***  ##  #  ##      ## 
***  #####  #####   ## 
*/


/**
 * Faction_IsCreated
 *
 * se a faccao existe
 *
 * @param (int) (faction) id da facção
 * @return (bool) (FACTION_CREATED)
 */
stock Faction_IsCreated(faction)
{
	if(faction >= FACTION_LIMIT || faction < 0)
		return false;

	return Faction[faction][FACTION_CREATED];
}

/**
 * Faction_IsUsable
 *
 * se a faccao pode ser usada
 *
 * @param (int) (faction) id da facção
 * @return (bool) (FACTION_USABLE)
 */
stock Faction_IsUsable(faction)
{
	if(faction >= FACTION_LIMIT || faction < 0)
		return false;

	return Faction[faction][FACTION_USABLE];
}


/**
 * Faction_GetIdBySQLId
 *
 * retorna o id da faccao
 *
 * @param (int) (sqlid) id no banco de dados
 * @return (int) (faction)
 */
stock Faction_GetIdBySQLId(sqlid)
{
	new tmp_id = -1;
	for(new i = 0; i < FACTION_LIMIT; i++)
	{
		if(!Faction_IsCreated(i))
			continue;

		if(Faction_GetSQLId(i) == sqlid)
		{
			tmp_id = i;
			break;
		}
	}
	return tmp_id;
}

/**
 * Faction_GetType
 *
 * retorna o tipo da faccao
 *
 * @param (int) (faction) id da facção
 * @return (int) (FACTION_TYPE)
 */
stock Faction_GetType(faction)
{
	return Faction[faction][FACTION_TYPE];
}

/**
 * Faction_GetName
 *
 * retorna o nome da facção
 *
 * @param (int) (faction) id da facção
 * @return (string) (facion_name)
 */
stock Faction_GetName(faction) 
{
	new tmp_str[FACTION_NAME_LIMIT];
	strcat(tmp_str, Faction[faction][FACTION_NAME]);
	return tmp_str;
}

/**
 * Faction_GetColorVehicle
 *
 * retorna a cor id
 *
 * @param (int) (faction) id da facção
 * @return (int) (FACTION_COLOR_VEHICLE)
 */
stock Faction_GetColorVehicle(faction)
{
	return Faction[faction][FACTION_COLOR_VEHICLE];
}

/**
 * Faction_GetColorEmbed
 *
 * retorna a cor em embed
 *
 * @param (int) (faction) id da facção
 * @return (string) (embed_color)
 */
stock Faction_GetColorEmbed(faction)
{
	new tmp_str[FACTION_NAME_LIMIT];
	strcat(tmp_str, Faction[faction][FACTION_COLOR_EMBED]);
	return tmp_str;
}

/**
 * Faction_GetColorHex
 *
 * retorna a cor em hexadecimal
 *
 * @param (int) (faction) id da facção
 * @return (int) (color_hex)
 */
stock Faction_GetColorHex(faction)
{
	return Faction[faction][FACTION_COLOR_HEX];
}

/**
 * Faction_GetMaxMembers
 *
 * retorna o maximo de membros que a organizacao suporta
 *
 * @param (int) (faction) id da facção
 * @return (int) (max_members)
 */
stock Faction_GetMaxMembers(faction)
{
	return Faction[faction][FACTION_MAX_MEMBER];
}

/**
 * Faction_GetSQLId
 *
 * retorna o id no banco de dados
 *
 * @param (int) (faction) id da facção
 * @return (int) (FACTION_SQLID)
 */
stock Faction_GetSQLId(faction)
{
	return Faction[faction][FACTION_SQLID];
}

/**
 * Faction_GetRankName
 *
 * retorna nome do rank (cargo)
 *
 * @param (int) (faction) faction id
 * @param (int) (rank) rank id
 * @return (bool) (undefined)
 */
stock Faction_GetRankName(faction, rank)
{
	new tmp_str[FACTION_RANK_NAME_LIMIT] = "Indefinido";
	
	if(!Faction_IsCreated(faction))
		return tmp_str;

	if(isnull(FactionRank[faction][rank][FACTIONRANK_NAME]))
		return tmp_str;

	format(tmp_str, sizeof tmp_str, "%s", FactionRank[faction][rank][FACTIONRANK_NAME]);
	return tmp_str;
}


/*                      
***  #####  ##### ######
***  ##     ##      ##   
***  #####  ####    ##  
***     ##  ##      ## 
***  #####  #####   ## 
*/


/**
 * Faction_SetCreated
 *
 * torna verdadeiro ou falso a existencia da faccção
 *
 * @param (int) (faction) id da facção
 * @param (bool) (created) se a facção existe ou não
 * @return (bool) (undefined)
 */
stock Faction_SetCreated(faction, bool:created)
{
	Faction[faction][FACTION_CREATED]  = created;
	return true;
}

/**
 * Faction_SetUsable
 *
 * torna verdadeiro ou falso a usabilidade da faccção
 *
 * @param (int) (faction) id da facção
 * @param (bool) (usable) se a faccao podera ser usada ou nao
 * @return (bool) (undefined)
 */
stock Faction_SetUsable(faction, bool:usable)
{
	Faction[faction][FACTION_USABLE]  = usable;
	return true;
}

/**
 * Faction_SetType
 *
 * retorna o tipo da faccao
 *
 * @param (int) (faction) id da facção
 * @return (bool) (undefined)
 */
stock Faction_SetType(faction, type)
{
	Faction[faction][FACTION_TYPE] = type;
	return true;
}

/**
 * Faction_SetName
 *
 * altera o nome da facção
 *
 * @param (int) (faction) id da facção
 * @param (string) (name) nome a ser alterado
 * @return (bool) (undefined)
 */
stock Faction_SetName(faction, name[])
{
	if(!Faction_IsCreated(faction))
		return false;

	format(Faction[faction][FACTION_NAME], FACTION_NAME_LIMIT, "%s", name);
	return true;
}

/**
 * Faction_SetColorVehicle
 *
 * altera a cor para veiculos
 *
 * @param (int) (faction) id da facção
 * @param (int) (name) valor da cor
 * @return (bool) (undefined)
 */
stock Faction_SetColorVehicle(faction, color_vehicle)
{
	if(!Faction_IsCreated(faction))
		return false;

	Faction[faction][FACTION_COLOR_VEHICLE] = color_vehicle;
	return true;
}

/**
 * Faction_SetColorEmbed
 *
 * altera a color embed
 *
 * @param (int) (faction) id da facção
 * @param (string) (color_embbed) valor hexadecimal que representa a cor em string
 * @return (bool) (undefined)
 */
stock Faction_SetColorEmbed(faction, embed_color[])
{
	if(!Faction_IsCreated(faction))
		return false;

	format(Faction[faction][FACTION_COLOR_EMBED], 9, "%s", embed_color);
	return true;
}

/**
 * Faction_SetColorHex
 *
 * altera a cor em hexadecimal
 *
 * @param (int) (faction) id da facção
 * @param (int) (name) valor hexadecimal que representa a cor
 * @return (bool) (undefined)
 */
stock Faction_SetColorHex(faction, color_hex)
{
	if(!Faction_IsCreated(faction))
		return false;

	Faction[faction][FACTION_COLOR_HEX] = color_hex;
	return true;
}

/**
 * Faction_SetMaxMembers
 *
 * altera o numero maximo de membros que a facção suporta
 *
 * @param (int) (faction) id da facção
 * @param (int) (max_members) maximo de jogadores permitidos
 * @return (bool) (undefined)
 */
stock Faction_SetMaxMembers(faction, max_members)
{
	if(!Faction_IsCreated(faction))
		return false;

	Faction[faction][FACTION_MAX_MEMBER] = max_members;
	return true;
}

/**
 * Faction_SetMaxMembers
 *
 * altera o numero maximo de membros que a facção suporta
 *
 * @param (int) (faction) id da facção
 * @param (int) (max_members) maximo de jogadores permitidos
 * @return (bool) (undefined)
 */
stock Faction_SetSQLId(faction, sqlid)
{
	if(!Faction_IsCreated(faction))
		return false;

	Faction[faction][FACTION_SQLID] = sqlid;
	return true;
}

/**
 * Faction_SetRankName
 *
 * altera o nome do rank (cargo)
 *
 * @param (int) (faction) faction id
 * @param (int) (rank) rank id
 * @return (bool) (undefined)
 */
stock Faction_SetRankName(faction, rank, rank_name[])
{
	if(!Faction_IsCreated(faction))
		return false;
	
	format(FactionRank[faction][rank][FACTIONRANK_NAME], FACTION_RANK_NAME_LIMIT, "%s", rank_name);
	return true;
}
