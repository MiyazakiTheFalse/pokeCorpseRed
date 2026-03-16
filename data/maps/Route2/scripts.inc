Route2_MapScripts::
	map_script MAP_SCRIPT_ON_TRANSITION, Route2_OnTransition
	.byte 0

Route2_OnTransition::
	specialvar VAR_RESULT, ValidateGiovanniInteractionOverlayForCurrentMap
	goto_if_eq VAR_RESULT, FALSE, EventScript_Return
	goto_if_set FLAG_SYS_GIOVANNI_MEMORY_MODE_ACTIVE, Route2_EventScript_MemoryRoutingCheck
	goto_if_unset FLAG_SYS_GIOVANNI_MEMORY_MODE_ACTIVE, EventScript_Return
	goto Route2_EventScript_MemoryDesyncReturnToHub

Route2_EventScript_MemoryRoutingCheck::
	goto_if_unset FLAG_SYS_GIOVANNI_MEMORY_MODE_ACTIVE, Route2_EventScript_MemoryDesyncReturnToHub
	goto_if_eq VAR_GIO_CHAPTER, 1, EventScript_Return
	goto Route2_EventScript_MemoryDesyncReturnToHub

Route2_EventScript_MemoryDesyncReturnToHub::
	warp MAP_ROCKET_HIDEOUT_B4F, 19, 6
	waitstate
	end

Route2_EventScript_RouteSign::
	msgbox Route2_Text_RouteSign, MSGBOX_SIGN
	end

Route2_EventScript_DiglettsCaveSign::
	msgbox Route2_Text_DiglettsCave, MSGBOX_SIGN
	end

Route2_EventScript_MemoryRouteCheckpoint::
	setvar VAR_0x8004, 1
	call EventScript_GiovanniSharedWarp_ResolvePrecedence
	goto_if_eq VAR_RESULT, GIO_SHARED_WARP_RESULT_NORMAL_ROUTE, EventScript_Nothing
	goto_if_eq VAR_RESULT, GIO_SHARED_WARP_RESULT_LOCKED_FALLBACK, Route2_EventScript_MemoryDesyncReturnToHub
	setflag FLAG_GIO_MEM_CH1_STARTED
	setvar VAR_GIO_CHAPTER, 1
	setvar VAR_GIO_ACT, 1
	setvar VAR_GIO_CHECKPOINT_ID, 0
	end
