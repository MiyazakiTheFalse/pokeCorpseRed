@ Shared RocketOps terminal flow and chapter command dispatch.

EventScript_GiovanniCampaign_PrintActiveDirective::
	specialvar VAR_RESULT, Special_LoadGiovanniActiveDirective
	msgbox gStringVar4, MSGBOX_AUTOCLOSE
	return

EventScript_RocketOps_Terminal::
	lockall
	specialvar VAR_RESULT, Special_RocketOps_OpenTerminal
	goto_if_eq VAR_RESULT, FALSE, EventScript_RocketOps_TerminalOffline
	msgbox Text_RocketOpsTerminalBoot, MSGBOX_AUTOCLOSE
	setvar VAR_GIO_SEGMENT, 1 @ GIO_SEGMENT_2
	call EventScript_RocketOps_AssignImmediateCommand
	goto_if_eq VAR_RESULT, FALSE, EventScript_RocketOps_TriggerLinkMismatch
	specialvar VAR_RESULT, Special_RocketOps_ValidateCommandContext
	goto_if_eq VAR_RESULT, TRUE, EventScript_RocketOps_CommandDispatch
	call EventScript_RocketOps_CommandDenied
	goto EventScript_RocketOps_Close

EventScript_RocketOps_CommandDispatch::
	setvar VAR_GIO_SEGMENT, 2 @ GIO_SEGMENT_3
	goto_if_eq VAR_0x8004, 0, EventScript_RocketOps_Command_SecureRoute
	goto_if_eq VAR_0x8004, 1, EventScript_RocketOps_Command_DeployAgent
	goto_if_eq VAR_0x8004, 2, EventScript_RocketOps_Command_DestroyData
	goto_if_eq VAR_0x8004, 3, EventScript_RocketOps_Command_ExtractStaff
	goto EventScript_RocketOps_TriggerLinkMismatch

EventScript_RocketOps_AssignImmediateCommand::
	switch VAR_0x8005
	case 1, EventScript_RocketOps_AssignImmediateCommand_SecureRoute
	case 2, EventScript_RocketOps_AssignImmediateCommand_DeployAgent
	case 3, EventScript_RocketOps_AssignImmediateCommand_DestroyData
	case 4, EventScript_RocketOps_AssignImmediateCommand_ExtractStaff
	setvar VAR_RESULT, FALSE
	return

EventScript_RocketOps_AssignImmediateCommand_SecureRoute::
	setvar VAR_0x8004, 0
	setvar VAR_RESULT, TRUE
	return

EventScript_RocketOps_AssignImmediateCommand_DeployAgent::
	setvar VAR_0x8004, 1
	setvar VAR_RESULT, TRUE
	return

EventScript_RocketOps_AssignImmediateCommand_DestroyData::
	setvar VAR_0x8004, 2
	setvar VAR_RESULT, TRUE
	return

EventScript_RocketOps_AssignImmediateCommand_ExtractStaff::
	setvar VAR_0x8004, 3
	setvar VAR_RESULT, TRUE
	return

EventScript_RocketOps_CommandDenied::
	goto_if_eq VAR_0x8004, 0, EventScript_RocketOps_CommandDenied_SecureRoute
	goto_if_eq VAR_0x8004, 1, EventScript_RocketOps_CommandDenied_DeployAgent
	goto_if_eq VAR_0x8004, 2, EventScript_RocketOps_CommandDenied_DestroyData
	goto_if_eq VAR_0x8004, 3, EventScript_RocketOps_CommandDenied_ExtractStaff
	msgbox Text_RocketOpsTriggerLinkMismatch
	return

EventScript_RocketOps_CommandDenied_SecureRoute::
	msgbox Text_RocketOpsFailSecureRoute
	return

EventScript_RocketOps_CommandDenied_DeployAgent::
	msgbox Text_RocketOpsFailDeployAgent
	return

EventScript_RocketOps_CommandDenied_DestroyData::
	msgbox Text_RocketOpsFailDestroyData
	addvar VAR_ROCKETOPS_ALERT, 1
	return

EventScript_RocketOps_CommandDenied_ExtractStaff::
	msgbox Text_RocketOpsFailExtractStaff
	addvar VAR_ROCKETOPS_ALERT, 1
	return

EventScript_RocketOps_TriggerLinkMismatch::
	msgbox Text_RocketOpsTriggerLinkMismatch
	goto EventScript_RocketOps_Close

EventScript_RocketOps_Command_SecureRoute::
	goto_if_eq VAR_ROCKETOPS_CHAPTER, 1, EventScript_RocketOps_Command_SecureRoute_C1
	goto_if_eq VAR_ROCKETOPS_CHAPTER, 2, EventScript_RocketOps_Command_SecureRoute_C2
	goto_if_eq VAR_ROCKETOPS_CHAPTER, 3, EventScript_RocketOps_Command_SecureRoute_C3
	goto EventScript_RocketOps_CommandChapterMismatch

EventScript_RocketOps_Command_DeployAgent::
	goto_if_eq VAR_ROCKETOPS_CHAPTER, 1, EventScript_RocketOps_Command_DeployAgent_C1
	goto_if_eq VAR_ROCKETOPS_CHAPTER, 2, EventScript_RocketOps_Command_DeployAgent_C2
	goto_if_eq VAR_ROCKETOPS_CHAPTER, 3, EventScript_RocketOps_Command_DeployAgent_C3
	goto EventScript_RocketOps_CommandChapterMismatch

EventScript_RocketOps_Command_DestroyData::
	goto_if_eq VAR_ROCKETOPS_CHAPTER, 1, EventScript_RocketOps_Command_DestroyData_C1
	goto_if_eq VAR_ROCKETOPS_CHAPTER, 2, EventScript_RocketOps_Command_DestroyData_C2
	goto_if_eq VAR_ROCKETOPS_CHAPTER, 3, EventScript_RocketOps_Command_DestroyData_C3
	goto EventScript_RocketOps_CommandChapterMismatch

EventScript_RocketOps_Command_ExtractStaff::
	goto_if_eq VAR_ROCKETOPS_CHAPTER, 1, EventScript_RocketOps_Command_ExtractStaff_C1
	goto_if_eq VAR_ROCKETOPS_CHAPTER, 2, EventScript_RocketOps_Command_ExtractStaff_C2
	goto_if_eq VAR_ROCKETOPS_CHAPTER, 3, EventScript_RocketOps_Command_ExtractStaff_C3
	goto EventScript_RocketOps_CommandChapterMismatch

EventScript_RocketOps_CommandChapterMismatch::
	msgbox Text_RocketOpsCommandChapterMismatch
	goto EventScript_RocketOps_Close

EventScript_RocketOps_CommandFinalize::
	setvar VAR_GIO_SEGMENT, 4 @ GIO_SEGMENT_5
	specialvar VAR_RESULT, Special_RocketOps_ApplyCommandEffect
	goto_if_eq VAR_RESULT, FALSE, EventScript_RocketOps_Close
	special Special_RocketOps_WritebackState
	call EventScript_GiovanniCampaign_PrintActiveDirective
	setvar VAR_GIO_SEGMENT, 4 @ GIO_SEGMENT_5
	goto EventScript_GiovanniCampaign_ReturnToHub

EventScript_GiovanniCampaign_ReturnToHub::
	goto_if_eq VAR_ROCKETOPS_CHAPTER, 1, EventScript_GiovanniCampaign_ReturnToHub_Ch1
	goto_if_eq VAR_ROCKETOPS_CHAPTER, 2, EventScript_GiovanniCampaign_ReturnToHub_Ch2
	goto_if_eq VAR_ROCKETOPS_CHAPTER, 3, EventScript_GiovanniCampaign_ReturnToHub_Ch3
	goto EventScript_RocketOps_Close

EventScript_GiovanniCampaign_ReturnToHub_Ch1::
	warp MAP_ROCKET_HIDEOUT_B4F, 19, 6
	waitstate
	releaseall
	end

EventScript_GiovanniCampaign_ReturnToHub_Ch2::
	warp MAP_SILPH_CO_11F, 6, 14
	waitstate
	releaseall
	end

EventScript_GiovanniCampaign_ReturnToHub_Ch3::
	warp MAP_VIRIDIAN_CITY_GYM, 17, 20
	waitstate
	releaseall
	end

EventScript_GiovanniCampaign_WatchdogHint::
	goto_if_eq VAR_ROCKETOPS_CHAPTER, 1, EventScript_GiovanniCampaign_WatchdogHint_C1
	goto_if_eq VAR_ROCKETOPS_CHAPTER, 2, EventScript_GiovanniCampaign_WatchdogHint_C2
	goto_if_eq VAR_ROCKETOPS_CHAPTER, 3, EventScript_GiovanniCampaign_WatchdogHint_C3
	msgbox Text_RocketOpsWatchdogHint_Generic, MSGBOX_AUTOCLOSE
	return

EventScript_GiovanniCampaign_WatchdogHint_C1::
	msgbox Text_RocketOpsWatchdogHint_C1, MSGBOX_AUTOCLOSE
	return

EventScript_GiovanniCampaign_WatchdogHint_C2::
	msgbox Text_RocketOpsWatchdogHint_C2, MSGBOX_AUTOCLOSE
	return

EventScript_GiovanniCampaign_WatchdogHint_C3::
	msgbox Text_RocketOpsWatchdogHint_C3, MSGBOX_AUTOCLOSE
	return

EventScript_RocketOps_TerminalOffline::
	msgbox Text_RocketOpsTerminalOffline

EventScript_RocketOps_Close::
	releaseall
	end

Text_RocketOpsTerminalBoot::
	.string "[ROCKETOPS] Link established. Routing\n"
	.string "node command now.$"

Text_RocketOpsTriggerLinkMismatch::
	.string "[ROCKETOPS] Node handshake failed.\n"
	.string "No command package found.$"

Text_RocketOpsTerminalOffline::
	.string "The terminal does not respond outside\n"
	.string "Giovanni's active memory operation.$"

Text_RocketOpsCommandChapterMismatch::
	.string "[ROCKETOPS] Chapter context mismatch.\n"
	.string "Command routing denied.$"

Text_RocketOpsFailSecureRoute::
	.string "Route cannot be secured from this\n"
	.string "terminal.$"

Text_RocketOpsFailDeployAgent::
	.string "No valid insertion lane available.$"

Text_RocketOpsFailDestroyData::
	.string "Data core is mirrored; purge denied.$"

Text_RocketOpsFailExtractStaff::
	.string "Extraction window closed. Hostiles\n"
	.string "too close.$"

Text_RocketOpsWatchdogHint_Generic::
	.string "[ROCKETOPS] Progress stalled. Return to\n"
	.string "a command terminal for new orders.$"

Text_RocketOpsWatchdogHint_C1::
	.string "[ROCKETOPS:C1] Operation stalled. Route\n"
	.string "through a control/surveillance/evac node.$"

Text_RocketOpsWatchdogHint_C2::
	.string "[ROCKETOPS:C2] Momentum dropped. Push\n"
	.string "secure route, purge data, then extract.$"

Text_RocketOpsWatchdogHint_C3::
	.string "[ROCKETOPS:C3] Escort chain stalled. Lock\n"
	.string "checkpoint two and execute extraction.$"
