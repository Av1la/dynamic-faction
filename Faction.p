/*
	Faction_Create(name[], embed_color[], hex_color)
	Faction_Delete();

	Faction_SetCreated(faction, bool:created)
	Faction_SetUsable(faction, bool:usable)
	Faction_SetName(faction, name)
	Faction_SetEmbedColor(faction, embbed_color)
	Faction_SetHexColor(faction, hex_color)
	Faction_SetMaxMembers(faction, max_members)

	Faction_IsCreated(faction)
	Faction_IsUsable(faction)
	Faction_GetName(faction)
	Faction_GetEmbbedColor(faction)
	Faction_GetHexColor(faction)
	Faction_GetMaxMembers(faction)
*/

#define FACTION_LIMIT 				10
#define FACTION_NAME_LIMIT 			24
#define FACTION_RANK_LIMIT 			10
#define FACTION_RANK_NAME_LIMIT 	12

enum E_FACTION_ATTRIB
{
	bool:Faction_Created,
	bool:Faction_Usable,

	Faction_Name[FACTION_NAME_LIMIT],
	Faction_EmbedColor[9],
	Faction_HexColor,

	Faction_MaxMembers,
} 

enum E_FACTION_RANK
{
	bool:FactionRank_Usable,
	FactionRank_Name[FACTION_RANK_NAME_LIMIT],
}

new Faction[FACTION_LIMIT][E_FACTION_ATTRIB];
new FactionRank[FACTION_LIMIT][E_FACTION_RANK];

new gFactionIndex;

/**
 * Faction_Create
 *
 * cria uma nova facção
 *
 * @param (string) (name) nome da faccao
 * @param (string) (embed_color) cor em embed
 * @param (int) (hex_color) cor em hexadecimal
 * @return (bool) (undefined)
 */
stock Faction_Create(name[], embed_color[], hex_color)
{
	if(gFactionIndex >= FACTION_LIMIT)
		return -1;

	if(Faction_IsCreated(gFactionIndex))
	{
		gFactionIndex++;
		return Faction_Create(name, embed_color, hex_color);
	}

	Faction_SetCreated(gFactionIndex, true);
	Faction_SetUsable(gFactionIndex, false);

	Faction_SetName(gFactionIndex, name);
	Faction_SetEmbedColor(gFactionIndex, embed_color);
	Faction_SetHexColor(gFactionIndex, hex_color);
	Faction_SetMaxMembers(gFactionIndex, 10);

	gFactionIndex++;
	return (gFactionIndex - 1);
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
	Faction_SetEmbedColor(faction, "");
	Faction_SetHexColor(faction, 0);
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
 * @return (bool) (Faction_Created)
 */
stock Faction_IsCreated(faction)
{
	return Faction[faction][Faction_Created];
}

/**
 * Faction_IsUsable
 *
 * se a faccao pode ser usada
 *
 * @param (int) (faction) id da facção
 * @return (bool) (Faction_Usable)
 */
stock Faction_IsUsable(faction)
{
	return Faction[faction][Faction_Usable];
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
	strcat(tmp_str, Faction[faction][Faction_Name]);
	return tmp_str;
}

/**
 * Faction_GetEmbbedColor
 *
 * retorna a cor em embed
 *
 * @param (int) (faction) id da facção
 * @return (string) (embed_color)
 */
stock Faction_GetEmbbedColor(faction)
{
	new tmp_str[FACTION_NAME_LIMIT];
	strcat(tmp_str, Faction[faction][Faction_EmbedColor]);
	return tmp_str;
}

/**
 * Faction_GetHexColor
 *
 * retorna a cor em hexadecimal
 *
 * @param (int) (faction) id da facção
 * @return (int) (hex_color)
 */
stock Faction_GetHexColor(faction)
{
	return Faction[faction][Faction_HexColor];
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
	return Faction[faction][Faction_MaxMembers];
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
	Faction[faction][Faction_Created]  = created;
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
	Faction[faction][Faction_Usable]  = usable;
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

	format(Faction[faction][Faction_Name], FACTION_NAME_LIMIT, "%s", name);
	return true;
}

/**
 * Faction_SetEmbedColor
 *
 * altera a color embed
 *
 * @param (int) (faction) id da facção
 * @param (string) (embbed_color) valor hexadecimal que representa a cor em string
 * @return (bool) (undefined)
 */
stock Faction_SetEmbedColor(faction, embed_color[])
{
	if(!Faction_IsCreated(faction))
		return false;

	format(Faction[faction][Faction_EmbedColor], 9, "%s", embed_color);
	return true;
}

/**
 * Faction_SetHexColor
 *
 * altera a cor em hexadecimal
 *
 * @param (int) (faction) id da facção
 * @param (int) (name) valor hexadecimal que representa a cor
 * @return (bool) (undefined)
 */
stock Faction_SetHexColor(faction, hex_color)
{
	if(!Faction_IsCreated(faction))
		return false;

	Faction[faction][Faction_HexColor] = hex_color;
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

	Faction[faction][Faction_MaxMembers] = max_members;
	return true;
}