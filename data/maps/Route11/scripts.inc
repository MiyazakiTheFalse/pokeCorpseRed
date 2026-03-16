Route11_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route11_OnTransition
	.byte 0

Route11_OnTransition::
	specialvar VAR_RESULT, ValidateGiovanniInteractionOverlayForCurrentMap
	goto_if_eq VAR_RESULT, FALSE, EventScript_Return
	goto_if_set FLAG_SYS_GIOVANNI_MEMORY_MODE_ACTIVE, Route11_EventScript_MemoryRoutingCheck
	goto_if_unset FLAG_SYS_GIOVANNI_MEMORY_MODE_ACTIVE, EventScript_Return
	goto Route11_EventScript_MemoryDesyncReturnToHub

Route11_EventScript_MemoryRoutingCheck::
	goto_if_unset FLAG_SYS_GIOVANNI_MEMORY_MODE_ACTIVE, Route11_EventScript_MemoryDesyncReturnToHub
	goto_if_eq VAR_GIO_CHAPTER, 1, EventScript_Return
	goto Route11_EventScript_MemoryDesyncReturnToHub

Route11_EventScript_MemoryDesyncReturnToHub::
	warp MAP_ROCKET_HIDEOUT_B4F, 19, 6
	waitstate
	end

Route11_EventScript_DiglettsCaveSign::
	msgbox Route11_Text_DiglettsCave, MSGBOX_SIGN
	end

Route11_EventScript_MemoryRouteCheckpoint::
	goto_if_set FLAG_GIO_MEM_CH1_DUGTRIO_BOSS_DEFEATED, Route11_EventScript_MemoryRouteCheckpoint_Save
	lockall
	msgbox Route11_Text_MemoryDiglettSwarmStart, MSGBOX_AUTOCLOSE
	setflag FLAG_SYS_SPECIAL_WILD_BATTLE
	setwildbattle SPECIES_DIGLETT, 24
	dowildbattle
	setwildbattle SPECIES_DIGLETT, 25
	dowildbattle
	setwildbattle SPECIES_DUGTRIO, 28
	dowildbattle
	clearflag FLAG_SYS_SPECIAL_WILD_BATTLE
	msgbox Route11_Text_MemoryDiglettSwarmClear, MSGBOX_AUTOCLOSE
	playse SE_M_DIG
	setvar VAR_0x8004, 0
	setvar VAR_0x8005, 2
	setvar VAR_0x8006, 3
	setvar VAR_0x8007, 2
	special ShakeScreen
	setflag FLAG_GIO_MEM_CH1_DIGLETT_SWARM_VISUAL
	releaseall

Route11_EventScript_MemoryRouteCheckpoint_Save::
	setvar VAR_0x8004, 1
	call EventScript_GiovanniSharedWarp_ResolvePrecedence
	goto_if_eq VAR_RESULT, GIO_SHARED_WARP_RESULT_NORMAL_ROUTE, EventScript_Nothing
	goto_if_eq VAR_RESULT, GIO_SHARED_WARP_RESULT_LOCKED_FALLBACK, Route11_EventScript_MemoryDesyncReturnToHub
	setflag FLAG_GIO_MEM_CH1_DUGTRIO_BOSS_DEFEATED
	setflag FLAG_GIO_MEM_CH1_DIGLETT_SWARM_VISUAL
	setvar VAR_GIO_ACT, 2
	setvar VAR_GIO_CHECKPOINT_ID, 1
	special SaveGiovanniMemoryModeCheckpoint
	end
