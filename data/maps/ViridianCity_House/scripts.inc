ViridianCity_House_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, ViridianCity_House_OnTransition
	.byte 0

ViridianCity_House_OnTransition::
	specialvar VAR_RESULT, ValidateGiovanniInteractionOverlayForCurrentMap
	goto_if_eq VAR_RESULT, FALSE, EventScript_Return
	goto_if_unset FLAG_SYS_GIOVANNI_MEMORY_MODE_ACTIVE, EventScript_Return
	goto_if_eq VAR_MODE_GIOVANNI_MEMORY, FALSE, ViridianCity_House_EventScript_MemoryDesyncReturnToHub
	goto_if_ne VAR_CHAPTER_ID, 3, ViridianCity_House_EventScript_MemoryDesyncReturnToHub
	goto ViridianCity_House_EventScript_ChapterHubEntry

ViridianCity_House_EventScript_MemoryDesyncReturnToHub::
	warp MAP_VIRIDIAN_CITY_GYM, 17, 21
	waitstate
	end

ViridianCity_House_EventScript_ChapterHubEntry::
	specialvar VAR_RESULT, Special_GiovanniCampaignDispatch
	goto_if_eq VAR_RESULT, 2, ViridianCity_House_EventScript_ChapterHubReturnScene
	goto_if_eq VAR_RESULT, 0, ViridianCity_House_EventScript_ChapterHubDirective
	goto_if_eq VAR_RESULT, 3, ViridianCity_House_EventScript_ChapterHubDirective
	goto_if_eq VAR_RESULT, 4, ViridianCity_House_EventScript_ChapterHubWatchdog
	end

ViridianCity_House_EventScript_ChapterHubReturnScene::
	msgbox ViridianCity_Gym_Text_MemoryAct5PresentResume, MSGBOX_AUTOCLOSE
	setvar VAR_GIO_SEGMENT, 6 @ GIO_SEGMENT_7
	end

ViridianCity_House_EventScript_ChapterHubDirective::
	call EventScript_GiovanniCampaign_PrintHubBriefing
	setvar VAR_GIO_SEGMENT, 2 @ GIO_SEGMENT_3
	end

ViridianCity_House_EventScript_ChapterHubWatchdog::
	call EventScript_GiovanniCampaign_WatchdogHint
	setvar VAR_GIO_SEGMENT, 2 @ GIO_SEGMENT_3
	end

ViridianCity_House_EventScript_BaldingMan::
	msgbox ViridianCity_House_Text_NicknamingIsFun, MSGBOX_NPC
	end

ViridianCity_House_EventScript_LittleGirl::
	msgbox ViridianCity_House_Text_MyDaddyLovesMonsToo, MSGBOX_NPC
	end

ViridianCity_House_EventScript_Speary::
	lock
	faceplayer
	waitse
	playmoncry SPECIES_SPEAROW, CRY_MODE_NORMAL
	msgbox ViridianCity_House_Text_Speary
	waitmoncry
	release
	end

ViridianCity_House_EventScript_NicknameSign::
	msgbox ViridianCity_House_Text_SpearowNameSpeary, MSGBOX_SIGN
	end
