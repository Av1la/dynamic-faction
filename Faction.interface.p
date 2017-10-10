
/*
    Faction.interface.p

    Lista de Funções:
    Interface_Faction(playerid)

	Interface_FactionCreate(playerid)
	Interface_FactionDelete(playerid)
	Interface_FactionList(playerid)

	Interface_FactionEdit(playerid, faction)
	Interface_FactionEditOpt(playerid, faction, option)
*/

/**
 * Interface_Faction
 *
 * Exibe o menu de gerenciamento de facções.
 *
 * @param (int) (playerid) player id
 * @return (bool) (undefined)  
 */
stock Interface_Faction(playerid)
{
    inline Response(pid, dialogid, response, listitem, string:inputtext[])
    {
        #pragma unused pid, dialogid, inputtext

    	if(!response)
    		return false;

    	switch(listitem)
    	{
    		case 0: return Interface_FactionCreate(playerid);
    		case 1: return Interface_FactionDelete(playerid);
    		case 2: return Interface_FactionList(playerid);
    	}
    }
    Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_LIST, "Admin Faction Interface -> Main", "Criar uma Facção\nApagar uma Facção\nListar Facções", "Selecionar", "Fechar");
	return true;
}

/**
 * Interface_FactionCreate
 *
 * Cria uma nova facção.
 *
 * @param (int) (playerid) player id
 * @return (bool) (undefined)  
 */
stock Interface_FactionCreate(playerid)
{
	Interface_FactionEdit(playerid, Faction_Create("Nome Indefindo", "{FFFFFF}", 0xFFFFFF));
	return true;
}

/**
 * Interface_FactionDelete
 *
 * Exibe uma lista de facções, ao selecionar uma a mesma sera apagada.
 *
 * @param (int) (playerid) player id
 * @return (bool) (undefined)  
 */
stock Interface_FactionDelete(playerid)
{
	new dialog_list_content[512] = "#id\t#nome\t#total membros\t#lider\n",
		dialog_list[FACTION_LIMIT],
		dialog_list_index;

    inline Response(pid, dialogid, response, listitem, string:inputtext[])
    {
        #pragma unused pid, dialogid, response, inputtext

    	if(!response)
    		return Interface_Faction(playerid);

        MemberList_RemoveAllFromFaction(dialog_list[listitem]);
    	Faction_Delete(dialog_list[listitem]);
    	Interface_Faction(playerid);
    }
    
    for(new i = 0; i < FACTION_LIMIT; i++) 
    {
    	if(!Faction_IsCreated(i))
    		continue;

    	format(dialog_list_content, sizeof dialog_list_content, "%s.%d\t%s\t%d\t%s\n", dialog_list_content, i, Faction_GetName(i), 0, "Indefinido");
    	
    	dialog_list[dialog_list_index] = i;
    	dialog_list_index++;
    }

    Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_TABLIST_HEADERS, "Admin Faction Interface -> Delete", dialog_list_content, "Selecionar", "Voltar");
	return true;
}

/**
 * Interface_FactionList
 *
 * Exibe uma lista de facções, ao selecionar abre um menu para modifica-la
 *
 * @param (int) (playerid) player id
 * @return (bool) (undefined)  
 */
stock Interface_FactionList(playerid)
{
	new dialog_list_content[512] = "#id\t#nome\t#total membros\t#lider\n",
		dialog_list[FACTION_LIMIT],
		dialog_list_index,
		leader_name[MAX_PLAYER_NAME];

    inline Response(pid, dialogid, response, listitem, string:inputtext[])
    {
        #pragma unused pid, dialogid, response, inputtext

    	if(!response)
    		return Interface_Faction(playerid);

    	Interface_FactionEdit(playerid, dialog_list[listitem]);
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

/**
 * Interface_FactionEdit
 *
 * Exibe opcoes para editar uma determinada facção.
 *
 * @param (int) (playerid) player id
 * @param (int) (faction) faction id
 * @return (bool) (undefined)  
 */
stock Interface_FactionEdit(playerid, faction)
{
    inline Response(pid, dialogid, response, listitem, string:inputtext[])
    {
        #pragma unused pid, dialogid, response, inputtext

    	if(!response)
    		return Interface_Faction(playerid);


        if(listitem < 4)
            Interface_FactionEditOpt(playerid, faction, listitem);
        else
            Interface_MemberList(playerid, faction);

    }    	

    Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_LIST, 
    	"Admin Faction Interface -> Edit", "Alterar Nome\nAlterar embed color\nAlterar hex color\nAlterar maximo de membros\nListar Membros", "Selecionar", "Fechar");
	return true;
}

/**
 * Interface_FactionEditOpt
 *
 * Funcao complementar de Interface_FactionEdit
 *
 * @param (int) (playerid) player id
 * @param (int) (faction) faction id
 * @param (int) (option) option id
 * @return (bool) (undefined)  
 */
stock Interface_FactionEditOpt(playerid, faction, option)
{
    inline Response(pid, dialogid, response, listitem, string:inputtext[])
    {
        #pragma unused pid, dialogid, response, listitem
		
    	if(!response) 
    		return Interface_FactionEdit(playerid, faction);

		switch(option)
		{
			case 0:
			{
				Faction_SetName(faction, inputtext);
				Interface_FactionEdit(playerid, faction);
				return true;
			}
			case 1:
			{
				Faction_SetEmbedColor(faction, inputtext);
				Interface_FactionEdit(playerid, faction);
				return true;
			}
			case 2:
			{
				Faction_SetHexColor(faction, strval(inputtext));
				Interface_FactionEdit(playerid, faction);
				return true;
			}
			case 3:
			{
				Faction_SetMaxMembers(faction, strval(inputtext));
				Interface_FactionEdit(playerid, faction);
				return true;
			}
		}
    }

    Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_INPUT, "Admin Faction Interface -> Edit", "Insira o novo valor abaixo:", "Modificar", "Voltar");
	return true;	
}