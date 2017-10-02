/*

	comandos para admin:
		/administrarfaccoes (cria/apaga/edita)
		/darlider

	comandos para lider
		* /gerenciarfaccao
		* /convidar

	comandos para membros
		* /r
		* /membros
		* /sairfaccao
	

	MemberList_Add(player_name[], faction, rank)
	MemberList_Remove(memberid)

	MemberList_SetUsable(memberid, usable)
	MemberList_SetName(memberid, name)
	MemberList_SetFaction(memberid, faction)
	MemberList_SetRank(memberid, rank)
	MemberList_SetJoinDate(memberid, join_date)
	MemberList_SetLastPromotion(memberid, promotion)

	MemberList_GetMemberIdById(playerid)
	MemberList_IsUsable(memberid)
	MemberList_GetName(memberid)
	MemberList_GetFaction(memberid)
	MemberList_GetRank(memberid)
	MemberList_GetJoinDate(memberid)
	MemberList_GetLastPromotion(memberid)

*/

command(administrarfaccoes, playerid, params [])
{
	//if(IsPlayerAdmin(playerid))
	AdminFaction_ShowMenu(playerid);
	return true;
}

command(darlider, playerid, params []) 
{
	new dialog_list_content[512] = "#id\t#nome\n",
		dialog_list[FACTION_LIMIT],
		dialog_list_index,
		target = strval(params);

	if(!IsPlayerConnected(target))
		return false;

    inline Response(pid, dialogid, response, listitem, string:inputtext[])
    {
        #pragma unused pid, dialogid, response, inputtext

    	if(!response)
    		return true;

		new tmp_str[128],
			member = MemberList_GetMemberIdById(target),
			player_name[MAX_PLAYER_NAME];

		// # verificando se o jogador faz parte de alguma facção.
		if(member >= 0)
		{
			format(tmp_str, sizeof tmp_str, "Você foi removido da sua facção atual. (%s)", Faction_GetName(MemberList_GetFaction(member)));
			SendClientMessage(target, -1, tmp_str);
			MemberList_Remove(member);
		}

		GetPlayerName(target, player_name, sizeof player_name);
		MemberList_SetLeader(MemberList_Add(player_name, dialog_list[listitem], 10), true);

		format(tmp_str, sizeof tmp_str, "Você foi promovido a lider de uma facção. (%s)", Faction_GetName(dialog_list[listitem]));
		SendClientMessage(target, -1, tmp_str);
    }
    
    for(new i = 0; i < FACTION_LIMIT; i++) 
    {
    	if(!Faction_IsCreated(i))
    		continue;

    	format(dialog_list_content, sizeof dialog_list_content, "%s.%d\t%s\n", dialog_list_content, i, Faction_GetName(i));  	
    	dialog_list[dialog_list_index] = i;
    	dialog_list_index++;
    }

    Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_TABLIST_HEADERS, "Adicionando um novo Lider", dialog_list_content, "Selecionar", "Voltar");
	return true;
}