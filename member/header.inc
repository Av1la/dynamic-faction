#if defined _inc_header
	#undef _inc_header
#endif

#define MEMBERLIST_LIMIT 	100
#define MEMBERLIST_INVALID 	-1

enum E_MEMBERLIST_ATTRIB
{
	bool:MemberList_Usable,
	bool:MemberList_Leader,

	MemberList_Name[MAX_PLAYER_NAME],
	MemberList_Faction,
	MemberList_Rank,
	MemberList_JoinDate,
	MemberList_LastPromotion
}

new gMemberListIndex;
new MemberList[MEMBERLIST_LIMIT][E_MEMBERLIST_ATTRIB];
new MemberList_Invite[MAX_PLAYERS] = {MEMBERLIST_INVALID, ...};
