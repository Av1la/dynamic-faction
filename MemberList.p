
/*
	MemberList_Add(player_name[], faction, rank)
	MemberList_Remove(memberid)

	MemberList_SendMessage(faction, text[])

	MemberList_SetUsable(memberid, bool usable)
	MemberList_SetLeader(memberid, bool leader)
	MemberList_SetName(memberid, name)
	MemberList_SetFaction(memberid, faction)
	MemberList_SetRank(memberid, rank)
	MemberList_SetJoinDate(memberid, join_date)
	MemberList_SetLastPromotion(memberid, promotion)

	MemberList_GetMemberIdById(playerid)
	MemberList_IsUsable(memberid)
	MemberList_IsLeader(memberid)
	MemberList_GetFactionLeader(faction)
	MemberList_GetFactionTotMember(faction)
	MemberList_GetName(memberid)
	MemberList_GetFaction(memberid)
	MemberList_GetRank(memberid)
	MemberList_GetJoinDate(memberid)
	MemberList_GetLastPromotion(memberid)
*/

#define MEMBERLIST_LIMIT 100


enum E_MEMBERLIST
{
	bool:MemberList_Usable,
	bool:MemberList_Leader,

	MemberList_Name[MAX_PLAYER_NAME],
	MemberList_Faction,
	MemberList_Rank,
	MemberList_JoinDate,
	MemberList_LastPromotion
}

new MemberList[MEMBERLIST_LIMIT][E_MEMBERLIST];
new gMemberListIndex;

/**
 * MemberList_Add
 *
 * adiciona um jogador na memberlist
 *
 * @param (string) (player_name) nome do jogador
 * @param (int) (faction) faction id
 * @param (int) (rank) rank level
 * @return (int) (memberid)
 */
stock MemberList_Add(player_name[], faction, rank)
{
	if(gMemberListIndex >= MEMBERLIST_LIMIT)
		return -1;

	if(MemberList_IsUsable(gMemberListIndex))
	{
		gMemberListIndex++;
		return MemberList_Add(player_name, faction, rank);
	}

	MemberList_SetUsable(gMemberListIndex, true);
	MemberList_SetName(gMemberListIndex, player_name);
	MemberList_SetFaction(gMemberListIndex, faction);
	MemberList_SetRank(gMemberListIndex, rank);
	MemberList_SetJoinDate(gMemberListIndex, gettime());
	MemberList_SetLastPromotion(gMemberListIndex, gettime());

	gMemberListIndex++;
	return (gMemberListIndex - 1);
}

/**
 * MemberList_Remove
 *
 * remove um jogador da memberlist
 *
 * @param (int) (memberid) member id
 * @return (bool) (undefined)
 */
stock MemberList_Remove(memberid)
{
	if(memberid >= MEMBERLIST_LIMIT)
		return false;

	if(!MemberList_IsUsable(memberid))
		return false;

	MemberList_SetName(memberid, "");
	MemberList_SetFaction(memberid, -1);
	MemberList_SetRank(memberid, 0);
	MemberList_SetJoinDate(memberid, 0);
	MemberList_SetLastPromotion(memberid, 0);
	MemberList_SetUsable(memberid, false);
	gMemberListIndex = memberid;
	return true;
}

/**
 * MemberList_SendMessage
 *
 * envia uma mensagem para todos os membros da faccao
 *
 * @param (int) (faction) faction id
 * @param (string) (text) texto a ser enviado
 * @return (bool) (undefined)
 */
stock MemberList_SendMessage(faction, text[]) 
{
	new member, pfaction;
	for(new i = 0; i < MAX_PLAYERS; i++)
	{
		if(!IsPlayerConnected(i))
			continue;

		member  = MemberList_GetMemberIdById(i);
		if(member < 0)
			continue;

		pfaction = MemberList_GetFaction(member); 
		if(pfaction != faction)
			continue;

		SendClientMessage(i, -1, text);
	}
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
 * MemberList_IsUsable
 *
 * usado para verificar se o id na memberlista esta vago
 *
 * @param (int) (memberid) member id
 * @return (bool) (MemberList_Usable)
 */
stock MemberList_IsUsable(memberid)
{
	if(memberid == -1)
		return -1;

	return MemberList[memberid][MemberList_Usable];
}

/**
 * MemberList_IsLeader
 *
 * verifica se o member é um lider
 *
 * @param (int) (memberid) member id
 * @return (bool) (MemberList_Leader)
 */
stock MemberList_IsLeader(memberid)
{
	return MemberList[memberid][MemberList_Leader];
}

/**
 * MemberList_GetFactionLeader
 *
 * retorna o lider da faccao
 *
 * @param (int) (faction) faction id
 * @return (int) (member)
 */
stock MemberList_GetFactionLeader(faction) 
{
	new member = -1;
	for(new i = 0; i < MEMBERLIST_LIMIT; i++)
	{
		if(!MemberList_IsUsable(i))
			continue;

		if(MemberList_GetFaction(i) != faction)
			continue;

		if(!MemberList_IsLeader(i))
			continue;

		member = i;
		break;
	}
	return member;
}

/**
 * MemberList_GetFactionTotMember
 *
 * retorna o total de membros de uma faccao
 *
 * @param (int) (faction) faction id
 * @return (int) (total)
 */
stock MemberList_GetFactionTotMember(faction)
{
	new total;

	for(new i = 0; i < MEMBERLIST_LIMIT; i++)
	{
		if(!MemberList_IsUsable(i))
			continue;

		if(MemberList_GetFaction(i) != faction)
			continue;

		total++;
		break;
	}
	return total;	
}

/**
 * MemberList_GetMemberIdById
 *
 * encontra o id do jogador na member list e retorna
 *
 * @param (int) (playerid) player id
 * @return (int) (memberid)
 */
stock MemberList_GetMemberIdById(playerid)
{
	new tmp_str[MAX_PLAYER_NAME], memberid = -1;
	GetPlayerName(playerid, tmp_str, sizeof tmp_str);

	for(new i = 0; i < MEMBERLIST_LIMIT; i++) {
		if(!MemberList_IsUsable(i))
			continue;

		if(!strcmp(tmp_str, MemberList_GetName(i)))
		{
			memberid = i;
			break;
		}
	}

	return memberid;
}

/**
 * MemberList_GetName
 *
 * retorna o nome do jogador na memberlist
 *
 * @param (int) (memberid) member id
 * @return (string) (MemberList_Name)
 */
stock MemberList_GetName(memberid)
{
	new tmp_str[MAX_PLAYER_NAME];
	if(memberid >= 0)
	{
		strcat(tmp_str, MemberList[memberid][MemberList_Name]);
	}
	return tmp_str;	
}

/**
 * MemberList_GetFaction
 *
 * retorna a facção do jogador na memberlist
 *
 * @param (int) (memberid) member id
 * @return (int) (MemberList_Faction)
 */
stock MemberList_GetFaction(memberid)
{
	return MemberList[memberid][MemberList_Faction];
}

/**
 * MemberList_GetRank
 *
 * retorna o ranking do jogador na memberlist
 *
 * @param (int) (memberid) member id
 * @return (int) (MemberList_Rank)
 */
stock MemberList_GetRank(memberid)
{
	return MemberList[memberid][MemberList_Rank];
}

/**
 * MemberList_GetJoinDate
 *
 * retorna a data em que o jogador ingressou na memberlist
 *
 * @param (int) (memberid) member id
 * @return (int) (MemberList_JoinDate)
 */
stock MemberList_GetJoinDate(memberid)
{
	return MemberList[memberid][MemberList_JoinDate];
}

/**
 * MemberList_GetLastPromotion
 *
 * retorna a data da ultima promocao do jogador na memberlist
 *
 * @param (int) (memberid) member id
 * @return (int) (MemberList_LastPromotion)
 */
stock MemberList_GetLastPromotion(memberid)
{
	return MemberList[memberid][MemberList_LastPromotion];
}


/*                      
***  #####  ##### ######
***  ##     ##      ##   
***  #####  ####    ##  
***     ##  ##      ## 
***  #####  #####   ## 
*/


/**
 * MemberList_SetUsable
 *
 * define se a vaga na memberlist esta vaga ou não
 *
 * @param (int) (memberid) member id
 * @param (bool) (usable) ...
 * @return (bool) (undefined)
 */
stock MemberList_SetUsable(memberid, bool:usable)
{
	MemberList[memberid][MemberList_Usable] = usable;
	return true;
}

/**
 * MemberList_SetUsable
 *
 * define se o memberid é lider ou não
 *
 * @param (int) (memberid) member id
 * @param (bool) (leader) ...
 * @return (bool) (undefined)
 */
stock MemberList_SetLeader(memberid, bool:leader)
{
	MemberList[memberid][MemberList_Leader] = leader;
	return true;
}

/**
 * MemberList_SetName
 *
 * altera o nome do jogador
 *
 * @param (int) (memberid) member id
 * @param (int) (name) nome do jogador
 * @return (bool) (undefined)
 */
stock MemberList_SetName(memberid, name[])
{
	if(!MemberList_IsUsable(memberid))
		return false;

	format(MemberList[memberid][MemberList_Name], MAX_PLAYER_NAME, "%s", name);
	return true;
}

/**
 * MemberList_SetFaction
 *
 * altera a faccao do jogador
 *
 * @param (int) (memberid) member id
 * @param (int) (faction) id da facção
 * @return (bool) (undefined)
 */
stock MemberList_SetFaction(memberid, faction)
{
	if(!MemberList_IsUsable(memberid))
		return false;

	MemberList[memberid][MemberList_Faction] = faction;
	return true;
}

/**
 * MemberList_SetRank
 *
 * altera o ranking(cargo) do jogador na memberlist
 *
 * @param (int) (memberid) member id
 * @param (int) (rank) ranking do jogador (cargo)
 * @return (bool) (undefined)
 */
stock MemberList_SetRank(memberid, rank)
{
	if(!MemberList_IsUsable(memberid))
		return false;

	MemberList[memberid][MemberList_Rank] = rank;
	return true;
}

/**
 * MemberList_SetJoinDate
 *
 * altera a data de entrada na memberlist
 *
 * @param (int) (memberid) member id
 * @param (int) (join_date) data de entrada
 * @return (bool) (undefined)
 */
stock MemberList_SetJoinDate(memberid, join_date)
{
	if(!MemberList_IsUsable(memberid))
		return false;

	MemberList[memberid][MemberList_JoinDate] = join_date;
	return true;
}

/**
 * MemberList_SetLastPromotion
 *
 * altera a data da ultima promocao
 *
 * @param (int) (memberid) member id
 * @param (int) (promotion) data da ultima promocao
 * @return (bool) (undefined)
 */
stock MemberList_SetLastPromotion(memberid, promotion)
{
	if(!MemberList_IsUsable(memberid))
		return false;

	MemberList[memberid][MemberList_LastPromotion] = promotion;
	return true;
}
