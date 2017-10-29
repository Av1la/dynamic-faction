#if defined _inc_database
	#undef _inc_database
#endif

/*
	SQLite_FactionLoad()

	SQLite_FactionInsert()
	SQLite_FactionUpdate()
	SQLite_FactionDelete()
*/

#include <YSI\y_hooks> 

new 
	DB:db_Faction,
	DBStatement:stmt_FactionLoad,
	DBStatement:stmt_MemberListLoad;

hook OnGameModeInit()
{
	db_Faction = db_open("Faction.db");

	
	stmt_FactionLoad 		= db_prepare(db_Faction, "SELECT * FROM `Faction`");
	stmt_MemberListLoad		= db_prepare(db_Faction, "SELECT * FROM `MemberList`");

	/*stmt_FactionCheck		= db_prepare(db_Faction, "SELECT COUNT(*) FROM `Faction` WHERE `name` = ?");
	stmt_factionInsert		= db_prepare(db_Faction, "INSERT INTO `Faction` VALUES (?, ?, ?)");
	stmt_factionUpdate		= db_prepare(db_Faction, "UPDATE `Faction` SET `name` = ?, `uname` = ?, `color` = ? WHERE `sqlid` = ?");
	stmt_factionDelete		= db_prepare(db_Faction, "DELETE FROM `Faction` WHERE `sqlid` = ?");*/	

	SQLite_FactionLoad();
	SQLite_MemberListLoad();
	return true;
}

hook OnGameModeExit()
{
	db_close(db_Faction);
	return true;
}


stock SQLite_FactionLoad()
{
	new 
		load_total,
		faction,
		id,
		name[24],
		type,
		max_members,
		color_vehicle,
		color_embed[9],
		color_hex,
		rank_name[10 + 1][24];

	stmt_bind_result_field(stmt_FactionLoad, 0, DB::TYPE_INTEGER, id);
	stmt_bind_result_field(stmt_FactionLoad, 1, DB::TYPE_INTEGER, type);
	stmt_bind_result_field(stmt_FactionLoad, 2, DB::TYPE_STRING, name, FACTION_NAME_LIMIT);
	stmt_bind_result_field(stmt_FactionLoad, 3, DB::TYPE_INTEGER, max_members);
	stmt_bind_result_field(stmt_FactionLoad, 4, DB::TYPE_INTEGER, color_vehicle);
	stmt_bind_result_field(stmt_FactionLoad, 5, DB::TYPE_STRING, color_embed, 9);
	stmt_bind_result_field(stmt_FactionLoad, 6, DB::TYPE_INTEGER, color_hex);

	for(new i = 1; i <= 10; i++)
	{
		stmt_bind_result_field(stmt_FactionLoad, 6 + i, DB::TYPE_STRING, rank_name[i], 24);
	}

	print(" #database start...");

	if(stmt_execute(stmt_FactionLoad))
	{
		while(stmt_fetch_row(stmt_FactionLoad))
		{
			if(!isnull(name))
			{
				faction = Faction_Create(name, type);
				
				Faction_SetSQLId(faction, id);
				Faction_SetMaxMembers(faction, max_members);
				
				Faction_SetColorVehicle(faction, color_vehicle);
				Faction_SetColorEmbed(faction, color_embed);
				Faction_SetColorHex(faction, color_hex);				
				
				for(new i = 1; i <= 10; i++)
				{
					Faction_SetRankName(faction, i, rank_name[i]);
				}

				printf(" <'%s' carregado>", name);
				load_total++;
			}
		}
	}

	printf("  %d facções carregadas", load_total);
	return true;
}


stock SQLite_MemberListLoad()
{
	new 
		faction,
		member,
		id, 
		name[24], 
		bool:leader,
		faction_sqlid, 
		rank, 
		join_date, 
		last_promotion;

	stmt_bind_result_field(stmt_MemberListLoad, 0, DB::TYPE_INTEGER, id);
	stmt_bind_result_field(stmt_MemberListLoad, 1, DB::TYPE_STRING, name, MAX_PLAYER_NAME);
	stmt_bind_result_field(stmt_MemberListLoad, 2, DB::TYPE_INTEGER, leader);
	stmt_bind_result_field(stmt_MemberListLoad, 3, DB::TYPE_INTEGER, faction_sqlid);
	stmt_bind_result_field(stmt_MemberListLoad, 4, DB::TYPE_INTEGER, rank);
	stmt_bind_result_field(stmt_MemberListLoad, 5, DB::TYPE_INTEGER, join_date);
	stmt_bind_result_field(stmt_MemberListLoad, 6, DB::TYPE_INTEGER, last_promotion);



	if(stmt_execute(stmt_MemberListLoad))
	{
		while(stmt_fetch_row(stmt_MemberListLoad))
		{
			if(!isnull(name))
			{
				faction = Faction_GetIdBySQLId(faction_sqlid);
				member 	= MemberList_Add(name, faction, rank);

				MemberList_SetLeader(member, leader);
				MemberList_SetJoinDate(member, join_date);
				MemberList_SetLastPromotion(member, last_promotion);
			}
		}
	}
	return true;
}