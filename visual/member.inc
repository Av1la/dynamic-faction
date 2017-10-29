#if defined _inc_MemberList
    #undef _inc_MemberList
#endif
/*
    MemberList.interface.p

    Lista de funções:
	Interface_MemberList(playerid, faction)
	Interface_MemberListMember(playerid, member)
*/

/**
 * Interface_MemberList
 *
 * Exibe a lista de membros em um dialog
 *
 * @param (int) (playerid) player id
 * @param (int) (faction) faction id
 * @return (bool) (undefined)  
 */
stock Interface_MemberList(playerid, faction)
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

        if(MemberList_IsLeader(dialog_list[listitem]) && !IsPlayerAdmin(playerid))
            return Interface_MemberList(playerid, faction);

    	Interface_MemberListMember(playerid, dialog_list[listitem]);
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

			format(dialog_list_content, sizeof dialog_list_content, "%s#%d\t%s\t(%d) %s\n", dialog_list_content, i, MemberList_GetName(i), MemberList_GetRank(i), Faction_GetRankName(faction, MemberList_GetRank(i)));
	    	dialog_list[dialog_list_index] = i;
	    	dialog_list_index++;
		}
	}

	Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_TABLIST_HEADERS, "Lista de Membros -> Gerenciando...", dialog_list_content, "Selecionar", "Fechar");
	return true;
}

/**
 * Interface_MemberListMember
 *
 * Exibe opcoes para modificar um membro da facção.
 *
 * @param (int) (playerid) player id
 * @param (int) (member) member id
 * @return (bool) (undefined)  
 */
stock Interface_MemberListMember(playerid, member)
{
	new tmp_str[256], tmp_name[MAX_PLAYER_NAME], dialog_title[128];
	GetPlayerName(playerid, tmp_name, sizeof tmp_name);

    inline Response(pid, dialogid, response, listitem, string:inputtext[])
    {
        #pragma unused pid, dialogid, inputtext

        // # botao voltar
    	if(!response)
    	{
    		Interface_MemberList(playerid, MemberList_GetFaction(member));
    		return false;
    	}

        // # botao selecionar
    	switch(listitem)
    	{
    		case 0:
    		{
                // # impossibilita a alteração no rank de um lider.
                if(MemberList_IsLeader(member))
                {
                    Interface_MemberListMember(playerid, member);
                    return true;                   
                }

                 // # impossibilita o lider/administrador de promover um jogador para um cargo superior a 9.
                if(MemberList_GetRank(member) >= 9)
                {
                    Interface_MemberListMember(playerid, member);
                    return true;
                }

    			format(tmp_str, sizeof tmp_str, "O jogador %s foi promovido. (por %s)", MemberList_GetName(member), tmp_name);
    			MemberList_SendMessage(MemberList_GetFaction(member), tmp_str);

    			MemberList_SetRank(member, MemberList_GetRank(member)+1);
    			Interface_MemberListMember(playerid, member);
    			return true;	
    		}
    		case 1:
    		{
                // impossibilita a alteração no rank de um lider.
                if(MemberList_IsLeader(member))
                {
                    Interface_MemberListMember(playerid, member);
                    return true;                   
                }

                // impossibilita o lider/administrador de rebaixar um jogador para um cargo inferior a 1.
                if(MemberList_GetRank(member) <= 1)
                {
                    Interface_MemberListMember(playerid, member);
                    return true;
                }

     			format(tmp_str, sizeof tmp_str, "O jogador %s foi rebaixado. (por %s)", MemberList_GetName(member), tmp_name);
    			MemberList_SendMessage(MemberList_GetFaction(member), tmp_str);

    			MemberList_SetRank(member, MemberList_GetRank(member)-1);
    			Interface_MemberListMember(playerid, member);
    			return true;	
    		}
    		case 2:
    		{
    			format(tmp_str, sizeof tmp_str, "O jogador %s foi expulso da facção. (por %s)", MemberList_GetName(member), tmp_name);
    			MemberList_SendMessage(MemberList_GetFaction(member), tmp_str);

    			MemberList_Remove(member);
    			Interface_MemberList(playerid, MemberList_GetFaction(member));
    			return true;
    		}
    	}
	}

    if(MemberList_IsLeader(member))
    {
	   format(tmp_str, sizeof tmp_str, "Promover (indisponivel)\nRebaixar (indisponivel)\n{ff0000}Expulsar da facção");
    }
    else 
    {
        if(MemberList_GetRank(member) == 9)
        {
            format(tmp_str, sizeof tmp_str, "Promover (indisponivel)\nRebaixar para %s(%d)\n{ff0000}Expulsar da facção", Faction_GetRankName(MemberList_GetFaction(member), MemberList_GetRank(member)-1), MemberList_GetRank(member)-1);
        }
        else if(MemberList_GetRank(member) == 1)
        {
            format(tmp_str, sizeof tmp_str, "Promover para %s(%d)\nRebaixar (indisponivel)\n{ff0000}Expulsar da facção", Faction_GetRankName(MemberList_GetFaction(member), MemberList_GetRank(member)+1), MemberList_GetRank(member)+1);
        }
        else
        {
            format(tmp_str, sizeof tmp_str, "Promover para %s(%d)\nRebaixar para %s(%d)\n{ff0000}Expulsar da facção", Faction_GetRankName(MemberList_GetFaction(member), MemberList_GetRank(member)+1), MemberList_GetRank(member)+1, Faction_GetRankName(MemberList_GetFaction(member), MemberList_GetRank(member)-1), MemberList_GetRank(member)-1);
        }
    }

    format(dialog_title, sizeof dialog_title, "Alterando %s %s", Faction_GetRankName(MemberList_GetFaction(member), MemberList_GetRank(member)), MemberList_GetName(member));
	Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_LIST, dialog_title, tmp_str, "Selecionar", " < Voltar");
	return true;
}