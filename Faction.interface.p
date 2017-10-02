// arquivo nao documentado


/*
    Interface_AFaction(playerid)

	Interface_AFactionCreate(playerid)
	Interface_AFactionDelete(playerid)
	Interface_AFactionList(playerid)

	Interface_AFactionEdit(playerid, faction)
	Interface_AFactionEditOpt(playerid, faction, option)
*/


stock Interface_AFaction(playerid)
{
    inline Response(pid, dialogid, response, listitem, string:inputtext[])
    {
        #pragma unused pid, dialogid, inputtext

    	if(!response)
    		return false;

    	switch(listitem)
    	{
    		case 0: return Interface_AFactionCreate(playerid);
    		case 1: return Interface_AFactionDelete(playerid);
    		case 2: return Interface_AFactionList(playerid);
    	}
    }
    Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_LIST, "Admin Faction Interface -> Main", "Criar uma Facção\nApagar uma Facção\nListar Facções", "Selecionar", "Fechar");
	return true;
}

stock Interface_AFactionCreate(playerid)
{
	Interface_AFactionEdit(playerid, Faction_Create("Nome Indefindo", "{FFFFFF}", 0xFFFFFF));
	return true;
}

stock Interface_AFactionDelete(playerid)
{
	new dialog_list_content[512] = "#id\t#nome\t#total membros\t#lider\n",
		dialog_list[FACTION_LIMIT],
		dialog_list_index;

    inline Response(pid, dialogid, response, listitem, string:inputtext[])
    {
        #pragma unused pid, dialogid, response, inputtext

    	if(!response)
    		return Interface_AFaction(playerid);

    	Faction_Delete(dialog_list[listitem]);
    	Interface_AFaction(playerid);
    }
    
    for(new i = 0; i < FACTION_LIMIT; i++) 
    {
    	if(!Faction_IsCreated(i))
    		continue;

    	format(dialog_list_content, sizeof dialog_list_content, "%s.%d\t%s\t%d\t%s\n", dialog_list_content, i, Faction_GetName(i), 0, "Indefinido");
    	
    	dialog_list[dialog_list_index] = i;
    	dialog_list_index++;
    }

    Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_TABLIST_HEADERS, 
    	"Admin Faction Interface -> Delete", dialog_list_content, "Selecionar", "Voltar");
	return true;
}

stock Interface_AFactionList(playerid)
{
	new dialog_list_content[512] = "#id\t#nome\t#total membros\t#lider\n",
		dialog_list[FACTION_LIMIT],
		dialog_list_index,
		leader_name[MAX_PLAYER_NAME];

    inline Response(pid, dialogid, response, listitem, string:inputtext[])
    {
        #pragma unused pid, dialogid, response, inputtext

    	if(!response)
    		return Interface_AFaction(playerid);

    	Interface_AFactionEdit(playerid, dialog_list[listitem]);
    }

    
    for(new i = 0; i < FACTION_LIMIT; i++) 
    {
    	if(!Faction_IsCreated(i))
    		continue;

    	format(leader_name, sizeof leader_name, "%s", MemberList_GetName(MemberList_GetFactionLeader(i)));
    	if(isnull(leader_name))
    	{
    		format(dialog_list_content, sizeof dialog_list_content, "%s.%d\t%s\t%d\t%s\n", dialog_list_content, i, Faction_GetName(i), MemberList_GetFactionTotMember(i), "Ninguem");
    	}
    	else
    	{
    		format(dialog_list_content, sizeof dialog_list_content, "%s.%d\t%s\t%d\t%s\n", dialog_list_content, i, Faction_GetName(i), MemberList_GetFactionTotMember(i), leader_name);
    	}
    	
    	dialog_list[dialog_list_index] = i;
    	dialog_list_index++;
    }

    Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_TABLIST_HEADERS, 
    	"Admin Faction Interface -> List", dialog_list_content, "Selecionar", "Voltar");
	return true;
}

stock Interface_AFactionEdit(playerid, faction)
{
    inline Response(pid, dialogid, response, listitem, string:inputtext[])
    {
        #pragma unused pid, dialogid, response, inputtext

    	if(!response)
    		return Interface_AFaction(playerid);

		Interface_AFactionEditOpt(playerid, faction, listitem);
    }    	

    Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_LIST, 
    	"Admin Faction Interface -> Edit", "Alterar Nome\nAlterar embed color\nAlterar hex color\nAlterar maximo de membros", "Selecionar", "Fechar");
	return true;
}

stock Interface_AFactionEditOpt(playerid, faction, option)
{
    inline Response(pid, dialogid, response, listitem, string:inputtext[])
    {
        #pragma unused pid, dialogid, response, listitem
		
    	if(!response) 
    		return Interface_AFactionEdit(playerid, faction);

		switch(option)
		{
			case 0:
			{
				Faction_SetName(faction, inputtext);
				Interface_AFactionEdit(playerid, faction);
				return true;
			}
			case 1:
			{
				Faction_SetEmbedColor(faction, inputtext);
				Interface_AFactionEdit(playerid, faction);
				return true;
			}
			case 2:
			{
				Faction_SetHexColor(faction, strval(inputtext));
				Interface_AFactionEdit(playerid, faction);
				return true;
			}
			case 3:
			{
				Faction_SetMaxMembers(faction, strval(inputtext));
				Interface_AFactionEdit(playerid, faction);
				return true;
			}
		}
    }

    Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_INPUT, 
    	"Admin Faction Interface -> Edit", "Insira o novo valor abaixo:", "Modificar", "Voltar");
	return true;	
}