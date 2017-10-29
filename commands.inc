#if defined _inc_commands
	#undef _inc_commands
#endif
	
/*	
	comandos para admin:
	/gerirfacs
	/darlider
	/setfac [facid] 
	/setrank [rank (10 = lider)]
	/vehfac [fac]
	/vehrank [rank]

	comandos para lider:
	/gerirfac
	/convidar

	comandos para membros:
	/aceitarconvite
	/r
	/membros
	/sairfaccao
*/

/*
	COMANDOS PARA ADMINISTRADORES
	*****************************
	*****************************
*/

/**
 * Comando - /gerirfacs
 *
 * Exibe um dialog para gerenciamento de facções
 *
 * @return (bool) (undefined)
 */
command(gerirfacs, playerid, params [])
{
	if(IsPlayerAdmin(playerid)) {
		Interface_Faction(playerid);
	}
	return true;
}

/**
 * Comando - /darlider
 *
 * Adiciona um jogador na lista de membros da facção como lider.
 *
 * @param (int) (playerid) player id
 * @return (bool) (undefined)
 */
command(darlider, playerid, params []) 
{
	if(IsPlayerAdmin(playerid)) {
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
	}
	return true;
}

/**
 * Comando - /setfac
 *
 * usado parar alterar a propria facção
 *
 * @return (bool) (undefined)
 */	
command(setfac, playerid, params [])
{
	if(IsPlayerAdmin(playerid))
	{
		new member = MemberList_GetMemberIdById(playerid);
		new input  = strval(params);
		new player_name[MAX_PLAYER_NAME];

		if(isnull(params))
		{
			SendClientMessage(playerid, -1, "uso correto: /setfac [faction]");
			return true;
		}

		if(!Faction_IsCreated(input))
			return true;

		if(input == MemberList_GetFaction(member))
			return true;

		if(member != MEMBERLIST_INVALID)
		{
			MemberList_Remove(member);
		}

		GetPlayerName(playerid, player_name, sizeof player_name);
		MemberList_Add(player_name, input, 1);
		return true;
	}
	return false;
}


/**
 * Comando - /setrank
 *
 * usado para alterar o proprio rank na faccao atual.
 *
 * @return (bool) (undefined)
 */	
command(setrank, playerid, params [])
{
	if(IsPlayerAdmin(playerid))
	{
		new member = MemberList_GetMemberIdById(playerid);
		new input  = strval(params);

		if(isnull(params))
		{
			SendClientMessage(playerid, -1, "uso correto: /setrank [rank de 1 a 10]");
			return true;
		}

		if(member == MEMBERLIST_INVALID)
			return true;

		if(MemberList_IsLeader(member) && input < 10)
		{
			MemberList_SetLeader(member, false);
		}

		if(!MemberList_IsLeader(member) && input == 10)
		{
			MemberList_SetLeader(member, true);
		}

		MemberList_SetRank(member, input);
		return true;
	}
	return false;
}

/**
 * Comando - /vehfac
 *
 * define o veiculo id como veiculo privado da faccao.
 *
 * @return (bool) (undefined)
 */	

 /*
command(vehfac, playerid, params [])
{
	if(IsPlayerAdmin(playerid))
	{
		if(!IsPlayerInAnyVehicle(playerid))
			return SendClientMessage(playerid, -1, "Você precisa estar em um veiculo.");

		if(isnull(params))
			return SendClientMessage(playerid, -1, "uso correto: /vehfac [faction]");

		if(!Faction_IsCreated(strval(params)))
			return SendClientMessage(playerid, -1, "Facção invalida!");

		new vehicleid = GetPlayerVehicleID(playerid);
		new vehicle   = FactionVehicle_GetIdFromVehid(vehicleid);
		
		if(vehicle != FACTION_VEHICLE_INVALID)
		{
			SendClientMessage(playerid, -1, "Veiculo ja definido.. removendo... removido!");
			FactionVehicle_Undef(vehicle);
		}

		FactionVehicle_Define(vehicleid, strval(params), 1);
		ChangeVehicleColor(vehicleid, Faction_GetColorVehicle(strval(params)), Faction_GetColorVehicle(strval(params)));

		SendClientMessage(playerid, -1, "Veiculo definido!");
		return true;
	}
	return false;
} */

/**
 * Comando - /vehrank
 *
 * altera o rank de permissao do veiculo atual.
 *
 * @return (bool) (undefined)
 */	
 /*
command(vehrank, playerid, params [])
{
	if(IsPlayerAdmin(playerid))
	{
		if(!IsPlayerInAnyVehicle(playerid))
			return SendClientMessage(playerid, -1, "Você precisa estar em um veiculo.");

		if(isnull(params))
			return SendClientMessage(playerid, -1, "uso correto: /vehrank [rank de 1 a 10]");

		if(strval(params) > 10 || strval(params) < 1)
			return SendClientMessage(playerid, -1, "Rank invalido, use de 1 a 10.");

		new vehicleid = GetPlayerVehicleID(playerid);
		new vehicle   = FactionVehicle_GetIdFromVehid(vehicleid);

		if(vehicle == FACTION_VEHICLE_INVALID)
			return SendClientMessage(playerid, -1, "O veiculo não pertence a nenhuma facção!"); 

		FactionVehicle_SetRank(vehicle, strval(params));
		return true;
	}
	return false;
}*/

/*
	
	COMANDOS PARA LIDERES
	*********************
	*********************

*/

/**
 * Comando - /gerirfac
 *
 * usado pelo lider para alterar informacoes referente a facção.
 *
 * @return (bool) (undefined)
 */	
command(gerirfac, playerid, params [])
{
	new member = MemberList_GetMemberIdById(playerid);
	
	if(member < 0) 
		return SendClientMessage(playerid, -1, "Você não faz parte de nenhuma facção.");

	if(!MemberList_IsLeader(member))
		return SendClientMessage(playerid, -1, "Você não é o lider da facção.");

	Interface_MemberList(playerid, MemberList_GetFaction(member));
	return true;
}

/**
 * Comando - /convidar
 *
 * usado pelo lider para convidar um novo membro.
 *
 * @return (bool) (undefined)
 */	
command(convidar, playerid, params [])
{
	new target 	= strval(params),
		member 	= MemberList_GetMemberIdById(playerid),
		faction = MemberList_GetFaction(member),
		tmp_str[128];

	if(member < 0) 
		return SendClientMessage(playerid, -1, "Você não faz parte de nenhuma facção.");

	if(!MemberList_IsLeader(member))
		return SendClientMessage(playerid, -1, "Você não é o lider da facção.");

	if(isnull(params))
		return SendClientMessage(playerid, -1, "Sintax: /convidar [playerid]");

	if(!IsPlayerConnected(target))
		return SendClientMessage(playerid, -1, "Jogador invalido!");

	if(MemberList_GetFactionTotMember(faction) >= Faction_GetMaxMembers(faction))
		return SendClientMessage(playerid, -1, "A facção não possui vagas suficientes!");

	if(MemberList_GetMemberIdById(target) > 0) 
		return SendClientMessage(playerid, -1, "O jogador ja possui uma facção!");

	format(tmp_str, sizeof tmp_str, " Você foi convidado para participar de uma facção. (%s por %s)", Faction_GetName(faction), MemberList_GetName(member));
	SendClientMessage(target, -1, tmp_str);

	MemberList_SetInvite(target, faction);
	return true;
}

/**
 * Comando - /aceitarconvite
 *
 * usado para aceitar um convite de facção.
 *
 * @return (bool) (undefined)
 */	
command(aceitarconvite, playerid, param [])
{
	new player_name[MAX_PLAYER_NAME],
		tmp_str[128];
	
	if(MemberList_GetMemberIdById(playerid) > 0) 
		return true;

	if(MemberList_GetInvite(playerid) < 0)
		return SendClientMessage(playerid, -1, "Você não foi convidado por nenhuma facção!");

	GetPlayerName(playerid, player_name, sizeof player_name);
	
	format(tmp_str, sizeof tmp_str, "%s ingressou na facção.", player_name);
	MemberList_SendMessage(MemberList_GetInvite(playerid), tmp_str);

	SendClientMessage(playerid, -1, "Parabens! Você ingressou na facção com sucesso!");
	MemberList_Add(player_name, MemberList_GetInvite(playerid), 1);
	MemberList_SetInvite(playerid, -1);
	return true;
}

/*
	
	COMANDOS PARA MEMBROS
	*********************
	*********************

*/

/**
 * Comando - /r
 *
 * Envia uma mensagem para todos online na facção.
 *
 * @param (string) (text) texto a ser enviado
 * @return (bool) (undefined)
 */
command(r, playerid, params []) 
{
	new member = MemberList_GetMemberIdById(playerid),
		faction,
		player_name[MAX_PLAYER_NAME],
		tmp_str[128];

	if(member < 0) 
	{
		SendClientMessage(playerid, -1, "Você não faz parte de nenhuma facção.");
		return true;
	}

	faction = MemberList_GetFaction(member);

	if(!Faction_IsCreated(faction))
	{
		SendClientMessage(playerid, -1, "Houve algum problema com sua facção..");
		SendClientMessage(playerid, -1, "Você foi removido da lista de membros automaticamente, consulte um administrador..");
		MemberList_Remove(member);
		return true;
	}

	GetPlayerName(playerid, player_name, sizeof player_name);
	format(tmp_str, sizeof tmp_str, "(%s) %s via Radio: %s",  Faction_GetRankName(faction, MemberList_GetRank(member)), player_name, params);
	MemberList_SendMessage(faction, tmp_str);
	return true;
}

/**
 * Comando - /membros
 *
 * exibe a lista de membros da facção que estão online
 *
 * @return (bool) (undefined)
 */
command(membros, playerid, params [])
{
	new member = MemberList_GetMemberIdById(playerid),
		faction,
		tmp_str[128];

	if(member < 0) 
	{
		SendClientMessage(playerid, -1, "Você não faz parte de nenhuma facção.");
		return true;
	}

	faction = MemberList_GetFaction(member);

	if(!Faction_IsCreated(faction))
	{
		SendClientMessage(playerid, -1, "Houve algum problema com sua facção..");
		SendClientMessage(playerid, -1, "Você foi removido da lista de membros automaticamente, consulte um administrador..");
		MemberList_Remove(member);
		return true;
	}

	SendClientMessage(playerid, -1, "Lista de membros online:");
	for(new i = 0; i < MEMBERLIST_LIMIT; i++)
	{
		if(!MemberList_IsUsable(i))
			continue;

		if(MemberList_GetFaction(i) != faction)
			continue;

		format(tmp_str, sizeof tmp_str, "%s / %s (%d)", MemberList_GetName(i), Faction_GetRankName(faction, MemberList_GetRank(i)), MemberList_GetRank(i));
		SendClientMessage(playerid, -1, tmp_str);
	}
	return true;
}


/**
 * Comando - /sairfaccao
 *
 * o jogador usa para sair da facção.
 *
 * @return (bool) (undefined)
 */
command(sairfaccao, playerid, params [])
{
	new member = MemberList_GetMemberIdById(playerid);

	if(member < 0) 
	{
		SendClientMessage(playerid, -1, "Você não faz parte de nenhuma facção.");
		return true;
	}

	SendClientMessage(playerid, -1, "Você foi removido da lista de membros da facção.");
	MemberList_Remove(member);
	return true;
}