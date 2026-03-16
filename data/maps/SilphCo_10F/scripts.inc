SilphCo_10F_MapScripts::
	map_script MAP_SCRIPT_ON_LOAD, SilphCo_10F_OnLoad
	map_script MAP_SCRIPT_ON_TRANSITION, SilphCo_10F_OnTransition
	.byte 0

SilphCo_10F_OnLoad::
	call_if_unset FLAG_SILPH_10F_DOOR, EventScript_Close10FDoor
	end


SilphCo_10F_OnTransition::
	specialvar VAR_RESULT, ValidateGiovanniInteractionOverlayForCurrentMap
	goto_if_eq VAR_RESULT, FALSE, EventScript_Return
	goto_if_unset FLAG_SYS_GIOVANNI_MEMORY_MODE_ACTIVE, EventScript_Return
	goto_if_eq VAR_MODE_GIOVANNI_MEMORY, FALSE, SilphCo_10F_EventScript_MemoryDesyncReturnToHub
	goto_if_ne VAR_CHAPTER_ID, 2, SilphCo_10F_EventScript_MemoryDesyncReturnToHub
	goto SilphCo_10F_EventScript_BunkerFlow

SilphCo_10F_EventScript_MemoryDesyncReturnToHub::
	warp MAP_SILPH_CO_11F, 6, 14
	waitstate
	end

SilphCo_10F_EventScript_BunkerFlow::
	specialvar VAR_RESULT, Special_GiovanniCampaignDispatch
	goto_if_eq VAR_RESULT, 2, SilphCo_10F_EventScript_BunkerFlow_Return
	goto_if_eq VAR_RESULT, 0, SilphCo_10F_EventScript_BunkerFlow_Directive
	goto_if_eq VAR_RESULT, 3, SilphCo_10F_EventScript_BunkerFlow_Directive
	goto_if_eq VAR_RESULT, 4, SilphCo_10F_EventScript_BunkerFlow_Watchdog
	end

SilphCo_10F_EventScript_BunkerFlow_Return::
	warp MAP_SILPH_CO_11F, 6, 14
	waitstate
	end

SilphCo_10F_EventScript_BunkerFlow_Directive::
	call EventScript_GiovanniCampaign_PrintActiveDirective
	setvar VAR_GIO_SEGMENT, 2 @ GIO_SEGMENT_3
	end

SilphCo_10F_EventScript_BunkerFlow_Watchdog::
	call EventScript_GiovanniCampaign_WatchdogHint
	setvar VAR_GIO_SEGMENT, 2 @ GIO_SEGMENT_3
	end

SilphCo_10F_EventScript_ActEndpointFallback::
	goto_if_unset FLAG_SYS_GIOVANNI_MEMORY_MODE_ACTIVE, EventScript_Nothing
	goto_if_ne VAR_CHAPTER_ID, 2, SilphCo_10F_EventScript_MemoryDesyncReturnToHub
	goto_if_ge VAR_ROCKETOPS_CH2_STAGE, 3, SilphCo_10F_EventScript_MemoryDesyncReturnToHub
	warp MAP_SILPH_CO_11F, 6, 14
	waitstate
	end
SilphCo_10F_EventScript_WorkerF::
	lock
	faceplayer
	goto_if_ge VAR_MAP_SCENE_SILPH_CO_11F, 1, SilphCo_10F_EventScript_WorkerFRocketsGone
	msgbox SilphCo_10F_Text_WaaaImScared
	release
	end

SilphCo_10F_EventScript_WorkerFRocketsGone::
	msgbox SilphCo_10F_Text_KeepMeCryingASecret
	release
	end

SilphCo_10F_EventScript_FloorSign::
	msgbox SilphCo_10F_Text_FloorSign, MSGBOX_SIGN
	end

SilphCo_10F_EventScript_Travis::
	trainerbattle_single TRAINER_SCIENTIST_TRAVIS, SilphCo_10F_Text_TravisIntro, SilphCo_10F_Text_TravisDefeat
	msgbox SilphCo_10F_Text_TravisPostBattle, MSGBOX_AUTOCLOSE
	end

SilphCo_10F_EventScript_Grunt::
	trainerbattle_single TRAINER_TEAM_ROCKET_GRUNT_39, SilphCo_10F_Text_GruntIntro, SilphCo_10F_Text_GruntDefeat
	msgbox SilphCo_10F_Text_GruntPostBattle, MSGBOX_AUTOCLOSE
	end
