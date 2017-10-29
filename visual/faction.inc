#if defined _inc_Faction
    #undef _inc_Faction
#endif
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


#include <YSI\y_hooks>

enum
{
    INTERFACE_DEFINE_FAC_NAME,
    INTERFACE_DEFINE_FAC_EMBCOLOR,
    INTERFACE_DEFINE_FAC_HEXCOLOR,
    INTERFACE_DEFINE_FAC_MAXMEMBERS
}

new Text:ColorBox[64];
new bool:gPlayerColorMenuOpen[MAX_PLAYERS];

hook OnGameModeInit()
{
    new 
        Float:pos_x = 19.0,
        Float:pos_y = 139.0, 
        count;

    for(new i = 0; i < sizeof ColorBox; i++)
    {
        ColorBox[i] = TextDrawCreate(pos_x, pos_y, "box");
        
        TextDrawBackgroundColor(ColorBox[i], Color_GetHex(Color_GetVehicleColor(i)));
        TextDrawLetterSize(ColorBox[i], 0.0, 18.0);
        TextDrawTextSize(ColorBox[i], 18.0, 18.0);
        TextDrawAlignment(ColorBox[i], 1);
        TextDrawUseBox(ColorBox[i], 1);
        TextDrawFont(ColorBox[i], 5);
        TextDrawSetProportional(ColorBox[i], 1);
        TextDrawSetSelectable(ColorBox[i], 1);
        TextDrawSetPreviewModel(ColorBox[i], 19349);

        pos_x = pos_x + 19.0;
        if(++count == 8)
        {
            count = 0;
            pos_y = pos_y + 19.0;
            pos_x = 19.0;
        }
    }
    return true;
}

hook OnGameModeExit()
{
    for(new i; i < sizeof ColorBox; i++)
        TextDrawDestroy(ColorBox[i]);

    return true;
}


/**
 * hook OnPlayerClickTextDraw
 *
 * Função chamada quando o clica em um textdraw selecionavel.
 *
 * @param (int) (playerid) player id
 * @param (Text) (clickedid) textdraw
 * @return (bool) (undefined)  
 */
hook OnPlayerClickTextDraw(playerid, Text:clickedid)
{
    for(new i = 0; i < sizeof ColorBox; i++)
    {
        if(clickedid == ColorBox[i])
        {
            new faction = GetPVarInt(playerid, "ColorMenu_FactionId");

            Faction_SetColorVehicle(faction, Color_GetVehicleColor(i));
            Faction_SetColorHex(faction, Color_GetHex(Color_GetVehicleColor(i)));
            Faction_SetColorEmbed(faction, Color_HexToEmbed(Color_GetHex(Color_GetVehicleColor(i))));

            Interface_FactionEdit(playerid, faction);
            Interface_ColorMenuHide(playerid);
        }
    }
    return true;
}


/**
 * hook OnPlayerKeyStateChange
 *
 * Função chamada quando o jogador pressiona uma tecla.
 *
 * @param (int) (playerid) player id
 * @param (int) (newkeys) tecla atual
 * @param (int) (oldkeys) tecla anterior
 * @return (bool) (undefined)  
 */
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(newkeys == KEY_NO)
    {
        Interface_ColorMenuHide(playerid);   
    }
    return true;
}

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
    Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_LIST, "Gerenciando facções..", "Criar uma facção\nApagar uma facção\nGerenciar facções", "Selecionar", "Fechar");
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
	Interface_FactionEdit(playerid, Faction_Create("Nome Indefindo", 0));
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

    Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_TABLIST_HEADERS, "Selecione uma facção para apagar!", dialog_list_content, "{ff0000}APAGAR", "Voltar");
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
    new dialog_list[512];

    inline Response(pid, dialogid, response, listitem, string:inputtext[])
    {
        #pragma unused pid, dialogid, response, inputtext

    	if(!response)
        {
            if(Interface_ColorMenuIsOpen(playerid))
            {
                Interface_ColorMenuHide(playerid);
            }

    		return Interface_Faction(playerid);
        }

        switch(listitem)
        {
            case 0: Interface_FactionDefineOpt(playerid, faction, INTERFACE_DEFINE_FAC_NAME);
            case 1: Interface_ColorMenuShow(playerid, faction);
            case 2: Interface_FactionDefineOpt(playerid, faction, INTERFACE_DEFINE_FAC_MAXMEMBERS);
            case 3: Interface_MemberList(playerid, faction);
            case 4: Interface_FactionEdit(playerid, faction);
            case 5: Interface_FactionEdit(playerid, faction);
        }
    }    	
    
    format(dialog_list, sizeof dialog_list, "\
        #opcao\t#valor\n\
        \
        Definir nome \t%s\n\
        Definir cores \t%sDemonstração{FFFFFF}\n\
        Definir maximo de membros \t%d\n\
        \
        Gerenciar membros \t...\n\
        Gerenciar veiculos \t...\n\
        Gerenciar ranks \t...\n"
        , Faction_GetName(faction), Faction_GetColorEmbed(faction), /*Faction_GetHexColor(faction),*/ Faction_GetMaxMembers(faction)
    );

    Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_TABLIST_HEADERS, "Gerenciando facção..", dialog_list, "Selecionar", "Fechar");
	return true;
}

/**
 * Interface_FactionChangeColor
 *
 * Funcao complementar de Interface_FactionEdit - exibe lista de cores para edicao
 *
 * @param (int) (playerid) player id
 * @param (int) (faction) faction id
 * @return (bool) (undefined)  
 */
stock Interface_FactionChangeColor(playerid, faction)
{
    inline Response(pid, dialogid, response, listitem, string:inputtext[])
    {
        #pragma unused pid, dialogid, response, listitem, inputtext
        
        if(!response) 
            return Interface_FactionEdit(playerid, faction);

        switch(listitem)
        {
            case 0:
            {
                Interface_FactionDefineOpt(playerid, faction, INTERFACE_DEFINE_FAC_EMBCOLOR);
                return true;
            }
            case 1:
            {
                Interface_FactionDefineOpt(playerid, faction, INTERFACE_DEFINE_FAC_HEXCOLOR);
                return true;
            }
        }
    }

    Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_LIST, "Gerenciando cores..", "Cor embed\nCor hex", "Modificar", "Voltar");
    return true;
}

/**
 * Interface_FactionDefineOpt
 *
 * Funcao complementar de Interface_FactionEdit
 *
 * @param (int) (playerid) player id
 * @param (int) (faction) faction id
 * @param (int) (option) option id
 * @return (bool) (undefined)  
 */
stock Interface_FactionDefineOpt(playerid, faction, option)
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
				Faction_SetColorEmbed(faction, inputtext);
				Interface_FactionEdit(playerid, faction);
				return true;
			}
			case 2:
			{
				Faction_SetColorHex(faction, strval(inputtext));
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

    Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_INPUT, "Gerenciador de Facções", "Insira o novo valor abaixo:", "Modificar", "Voltar");
	return true;	
}

/**
 * Interface_ColorMenuShow
 *
 * Exibe um menu semelhante ao selecionador de cores do gta_sa.
 *
 * @param (int) (playerid) player id
 * @param (int) (faction) faction id
 * @return (bool) (undefined)  
 */
stock Interface_ColorMenuShow(playerid, faction) 
{
    Interface_FactionEdit(playerid, faction);

    for(new i = 0; i < sizeof ColorBox; i++)
        TextDrawShowForPlayer(playerid,ColorBox[i]);

    Interface_ColorMenuSetOpen(playerid, true);
    SelectTextDraw(playerid, 0xFFFFFF66);
    SetPVarInt(playerid, "ColorMenu_FactionId", faction);
    return true;
}


/**
 * Interface_ColorMenuHide
 *
 * Fecha um menu semelhante ao selecionador de cores do gta_sa.
 *
 * @param (int) (playerid) player id
 * @return (bool) (undefined)  
 */
stock Interface_ColorMenuHide(playerid)
{
    CancelSelectTextDraw(playerid);
    
    for(new i; i < sizeof ColorBox; i++)
        TextDrawHideForPlayer(playerid, ColorBox[i]);

    Interface_ColorMenuSetOpen(playerid, false);
    SetPVarInt(playerid, "ColorMenu_FactionId", -1);
    return true;
}

/**
 * Interface_ColorMenuIsOpen
 *
 * Retorna se o menu esta fechado ou aberto.
 *
 * @param (int) (playerid) player id
 * @return (int) (gPlayerColorMenuOpen)  
 */
stock Interface_ColorMenuIsOpen(playerid)
{
    return gPlayerColorMenuOpen[playerid];
}

/**
 * Interface_ColorMenuSetOpen
 *
 * Define se o menu esta aberto ou fechado.
 *
 * @param (int) (playerid) player id
 * @param (bool) (open) valor em bool
 * @return (bool) (undefined)  
 */
stock Interface_ColorMenuSetOpen(playerid, bool:open)
{
    gPlayerColorMenuOpen[playerid] = open;
    return true;
}