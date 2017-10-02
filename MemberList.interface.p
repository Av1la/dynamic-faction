
/*
	Interface_LMemberList(playerid)
	Interface_LMemberListMember(playerid, member)
*/


stock Interface_LMemberList(playerid, faction)
{
	if(faction < 0 || faction >= FACTION_LIMIT)
		return false;
	
	new dialog_list_content[512] = ".id\t.nome\t.cargo\n",
		dialog_list[FACTION_LIMIT],
		dialog_list_index;

    inline Response(pid, dialogid, response, listitem, string:inputtext[])
    {
        #pragma unused pid, dialogid, inputtext

    	if(!response)
    		return false;

    	Interface_LMemberListMember(playerid, dialog_list[listitem]);
	}

	for(new rank = 10; rank > 0; rank--)
	{
		for(new i = 0; i < MEMBERLIST_LIMIT; i++) 
		{
			if(!MemberList_IsUsable(i))
				continue;

			if(MemberList_GetRank(i) != rank)
				continue;

			if(MemberList_GetFaction(i) != faction)
				continue;

			format(dialog_list_content, sizeof dialog_list_content, "%s#%d\t%s\t%d\n", dialog_list_content, i, MemberList_GetName(i), MemberList_GetRank(i));
	    	dialog_list[dialog_list_index] = i;
	    	dialog_list_index++;
		}
	}

	Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_TABLIST_HEADERS, "Lista de Membros -> Gerenciando...", dialog_list_content, "Selecionar", "Fechar");
	return true;
}

stock Interface_LMemberListMember(playerid, member)
{
	new tmp_str[256], tmp_name[MAX_PLAYER_NAME];
	GetPlayerName(playerid, tmp_name, sizeof tmp_name);

    inline Response(pid, dialogid, response, listitem, string:inputtext[])
    {
        #pragma unused pid, dialogid, inputtext

    	if(!response)
    	{
    		Interface_LMemberList(playerid, MemberList_GetFaction(member));
    		return false;
    	}

    	switch(listitem)
    	{
    		case 0:
    		{
    			format(tmp_str, sizeof tmp_str, "O jogador %s foi promovido. (por %s)", MemberList_GetName(member), tmp_name);
    			MemberList_SendMessage(MemberList_GetFaction(member), tmp_str);

    			MemberList_SetRank(member, MemberList_GetRank(member)+1);
    			Interface_LMemberListMember(playerid, member);
    			return true;	
    		}
    		case 1:
    		{
     			format(tmp_str, sizeof tmp_str, "O jogador %s foi rebaixado. (por %s)", MemberList_GetName(member), tmp_name);
    			MemberList_SendMessage(MemberList_GetFaction(member), tmp_str);

    			MemberList_SetRank(member, MemberList_GetRank(member)-1);
    			Interface_LMemberListMember(playerid, member);
    			return true;	
    		}
    		case 2:
    		{
    			format(tmp_str, sizeof tmp_str, "O jogador %s foi expulso da facção. (por %s)", MemberList_GetName(member), tmp_name);
    			MemberList_SendMessage(MemberList_GetFaction(member), tmp_str);

    			MemberList_Remove(member);
    			Interface_LMemberList(playerid, MemberList_GetFaction(member));
    			return true;
    		}
    	}
	}

	format(tmp_str, sizeof tmp_str, 
		"{009900}Promover{FFFFFF} para cargo %d\n{595959}Rebaixar{FFFFFF} cargo %d\n{ff0000}Expulsar da facção", 
		(MemberList_GetRank(member)+1), (MemberList_GetRank(member)-1));

	Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_LIST, 
		"Lista de Membros -> Gerenciando Membro", tmp_str, "Selecionar", "Fechar");

	return true;
}