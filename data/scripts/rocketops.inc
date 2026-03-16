@ Shared RocketOps terminal flow and chapter command dispatch.

EventScript_GiovanniCampaign_PrintActiveDirective::
	specialvar VAR_RESULT, Special_LoadGiovanniActiveDirective
	msgbox gStringVar4, MSGBOX_AUTOCLOSE
	return

EventScript_GiovanniCampaign_PrintHubMonologueOnce::
	goto_if_eq VAR_GIO_CHAPTER, 1, EventScript_GiovanniCampaign_PrintHubMonologueOnce_C1
	goto_if_eq VAR_GIO_CHAPTER, 2, EventScript_GiovanniCampaign_PrintHubMonologueOnce_C2
	goto_if_eq VAR_GIO_CHAPTER, 3, EventScript_GiovanniCampaign_PrintHubMonologueOnce_C3
	return

EventScript_GiovanniCampaign_PrintHubMonologueOnce_C1::
	goto_if_set FLAG_GIO_MEM_CH1_HUB_MONOLOGUE_SEEN, EventScript_Return
	setflag FLAG_GIO_MEM_CH1_HUB_MONOLOGUE_SEEN
	msgbox Text_GiovanniHubMonologue_C1, MSGBOX_AUTOCLOSE
	return

EventScript_GiovanniCampaign_PrintHubMonologueOnce_C2::
	goto_if_set FLAG_GIO_MEM_CH2_HUB_MONOLOGUE_SEEN, EventScript_Return
	setflag FLAG_GIO_MEM_CH2_HUB_MONOLOGUE_SEEN
	msgbox Text_GiovanniHubMonologue_C2, MSGBOX_AUTOCLOSE
	return

EventScript_GiovanniCampaign_PrintHubMonologueOnce_C3::
	goto_if_set FLAG_GIO_MEM_CH3_HUB_MONOLOGUE_SEEN, EventScript_Return
	setflag FLAG_GIO_MEM_CH3_HUB_MONOLOGUE_SEEN
	msgbox Text_GiovanniHubMonologue_C3, MSGBOX_AUTOCLOSE
	return

EventScript_GiovanniCampaign_PrintHubBriefing::
	call EventScript_GiovanniCampaign_PrintActiveDirective
	call EventScript_GiovanniCampaign_PrintHubMonologueOnce
	return

EventScript_RocketOps_Terminal::
	lockall
	specialvar VAR_RESULT, Special_RocketOps_OpenTerminal
	goto_if_eq VAR_RESULT, FALSE, EventScript_RocketOps_TerminalOffline
	msgbox Text_RocketOpsTerminalBoot, MSGBOX_AUTOCLOSE
	setvar VAR_GIO_SEGMENT, 1 @ GIO_SEGMENT_2
	specialvar VAR_0x8006, Special_RocketOps_GetCommandMenu
	call EventScript_RocketOps_ShowCommandMenu
	goto_if_eq VAR_0x8004, 4, EventScript_RocketOps_Close
	specialvar VAR_RESULT, Special_RocketOps_ValidateCommandContext
	goto_if_eq VAR_RESULT, TRUE, EventScript_RocketOps_CommandDispatch
	call EventScript_RocketOps_CommandDenied
	goto EventScript_RocketOps_Close

EventScript_RocketOps_ShowCommandMenu::
	switch VAR_0x8006
	case MULTICHOICE_ROCKETOPS_COMMANDS_C1, EventScript_RocketOps_ShowCommandMenu_C1
	case MULTICHOICE_ROCKETOPS_COMMANDS_C2, EventScript_RocketOps_ShowCommandMenu_C2
	case MULTICHOICE_ROCKETOPS_COMMANDS_C3, EventScript_RocketOps_ShowCommandMenu_C3
	multichoice 17, 0, MULTICHOICE_ROCKETOPS_COMMANDS, FALSE
	copyvar VAR_0x8004, VAR_RESULT
	return

EventScript_RocketOps_ShowCommandMenu_C1::
	multichoice 17, 0, MULTICHOICE_ROCKETOPS_COMMANDS_C1, FALSE
	copyvar VAR_0x8004, VAR_RESULT
	goto_if_eq VAR_0x8004, 3, EventScript_RocketOps_ShowCommandMenu_C1_Exit
	return

EventScript_RocketOps_ShowCommandMenu_C1_Exit::
	setvar VAR_0x8004, 4
	return

EventScript_RocketOps_ShowCommandMenu_C2::
	multichoice 17, 0, MULTICHOICE_ROCKETOPS_COMMANDS_C2, FALSE
	copyvar VAR_0x8004, VAR_RESULT
	goto_if_eq VAR_0x8004, 3, EventScript_RocketOps_ShowCommandMenu_C2_Exit
	goto_if_eq VAR_0x8004, 1, EventScript_RocketOps_ShowCommandMenu_C2_DestroyData
	goto_if_eq VAR_0x8004, 2, EventScript_RocketOps_ShowCommandMenu_C2_ExtractStaff
	return

EventScript_RocketOps_ShowCommandMenu_C2_DestroyData::
	setvar VAR_0x8004, 2
	return

EventScript_RocketOps_ShowCommandMenu_C2_ExtractStaff::
	setvar VAR_0x8004, 3
	return

EventScript_RocketOps_ShowCommandMenu_C2_Exit::
	setvar VAR_0x8004, 4
	return

EventScript_RocketOps_ShowCommandMenu_C3::
	multichoice 17, 0, MULTICHOICE_ROCKETOPS_COMMANDS_C3, FALSE
	copyvar VAR_0x8004, VAR_RESULT
	goto_if_eq VAR_0x8004, 3, EventScript_RocketOps_ShowCommandMenu_C3_Exit
	goto_if_eq VAR_0x8004, 0, EventScript_RocketOps_ShowCommandMenu_C3_DeployAgent
	goto_if_eq VAR_0x8004, 1, EventScript_RocketOps_ShowCommandMenu_C3_DestroyData
	goto_if_eq VAR_0x8004, 2, EventScript_RocketOps_ShowCommandMenu_C3_ExtractStaff
	return

EventScript_RocketOps_ShowCommandMenu_C3_DeployAgent::
	setvar VAR_0x8004, 1
	return

EventScript_RocketOps_ShowCommandMenu_C3_DestroyData::
	setvar VAR_0x8004, 2
	return

EventScript_RocketOps_ShowCommandMenu_C3_ExtractStaff::
	setvar VAR_0x8004, 3
	return

EventScript_RocketOps_ShowCommandMenu_C3_Exit::
	setvar VAR_0x8004, 4
	return

EventScript_RocketOps_CommandDispatch::
	setvar VAR_GIO_SEGMENT, 2 @ GIO_SEGMENT_3
	goto_if_eq VAR_0x8004, 0, EventScript_RocketOps_Command_SecureRoute
	goto_if_eq VAR_0x8004, 1, EventScript_RocketOps_Command_DeployAgent
	goto_if_eq VAR_0x8004, 2, EventScript_RocketOps_Command_DestroyData
	goto_if_eq VAR_0x8004, 3, EventScript_RocketOps_Command_ExtractStaff
	goto EventScript_RocketOps_TriggerLinkMismatch

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
	goto_if_eq VAR_GIO_CHAPTER, 1, EventScript_RocketOps_Command_SecureRoute_C1
	goto_if_eq VAR_GIO_CHAPTER, 2, EventScript_RocketOps_Command_SecureRoute_C2
	goto_if_eq VAR_GIO_CHAPTER, 3, EventScript_RocketOps_Command_SecureRoute_C3
	goto EventScript_RocketOps_CommandChapterMismatch

EventScript_RocketOps_Command_DeployAgent::
	goto_if_eq VAR_GIO_CHAPTER, 1, EventScript_RocketOps_Command_DeployAgent_C1
	goto_if_eq VAR_GIO_CHAPTER, 2, EventScript_RocketOps_Command_DeployAgent_C2
	goto_if_eq VAR_GIO_CHAPTER, 3, EventScript_RocketOps_Command_DeployAgent_C3
	goto EventScript_RocketOps_CommandChapterMismatch

EventScript_RocketOps_Command_DestroyData::
	goto_if_eq VAR_GIO_CHAPTER, 1, EventScript_RocketOps_Command_DestroyData_C1
	goto_if_eq VAR_GIO_CHAPTER, 2, EventScript_RocketOps_Command_DestroyData_C2
	goto_if_eq VAR_GIO_CHAPTER, 3, EventScript_RocketOps_Command_DestroyData_C3
	goto EventScript_RocketOps_CommandChapterMismatch

EventScript_RocketOps_Command_ExtractStaff::
	goto_if_eq VAR_GIO_CHAPTER, 1, EventScript_RocketOps_Command_ExtractStaff_C1
	goto_if_eq VAR_GIO_CHAPTER, 2, EventScript_RocketOps_Command_ExtractStaff_C2
	goto_if_eq VAR_GIO_CHAPTER, 3, EventScript_RocketOps_Command_ExtractStaff_C3
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
	goto_if_eq VAR_GIO_CHAPTER, 1, EventScript_GiovanniCampaign_ReturnToHub_Ch1
	goto_if_eq VAR_GIO_CHAPTER, 2, EventScript_GiovanniCampaign_ReturnToHub_Ch2
	goto_if_eq VAR_GIO_CHAPTER, 3, EventScript_GiovanniCampaign_ReturnToHub_Ch3
	goto EventScript_RocketOps_Close

EventScript_GiovanniCampaign_ReturnToHub_Ch1::
	goto EventScript_GiovanniHub_WarpChapter1

EventScript_GiovanniCampaign_ReturnToHub_Ch2::
	goto EventScript_GiovanniHub_WarpChapter2

EventScript_GiovanniCampaign_ReturnToHub_Ch3::
	goto EventScript_GiovanniHub_WarpChapter3

EventScript_GiovanniCampaign_PacingRedirect::
	goto_if_eq VAR_GIO_CHAPTER, 1, EventScript_GiovanniCampaign_WatchdogHint_C1
	goto_if_eq VAR_GIO_CHAPTER, 2, EventScript_GiovanniCampaign_WatchdogHint_C2
	goto_if_eq VAR_GIO_CHAPTER, 3, EventScript_GiovanniCampaign_WatchdogHint_C3
	msgbox Text_RocketOpsWatchdogHint_Generic, MSGBOX_AUTOCLOSE
	call EventScript_GiovanniCampaign_PrintActiveDirective
	return

EventScript_GiovanniCampaign_WatchdogHint::
	call EventScript_GiovanniCampaign_PacingRedirect
	return

EventScript_GiovanniCampaign_WatchdogHint_C1::
	msgbox Text_RocketOpsWatchdogHint_C1, MSGBOX_AUTOCLOSE
	call EventScript_GiovanniCampaign_PrintActiveDirective
	return

EventScript_GiovanniCampaign_WatchdogHint_C2::
	msgbox Text_RocketOpsWatchdogHint_C2, MSGBOX_AUTOCLOSE
	call EventScript_GiovanniCampaign_PrintActiveDirective
	return

EventScript_GiovanniCampaign_WatchdogHint_C3::
	msgbox Text_RocketOpsWatchdogHint_C3, MSGBOX_AUTOCLOSE
	call EventScript_GiovanniCampaign_PrintActiveDirective
	return

EventScript_RocketOps_UseControlNodeTerminal::
	setvar VAR_0x8005, 1 @ ROCKETOPS_TRIGGER_CONTROL_NODE
	goto EventScript_RocketOps_Terminal

EventScript_RocketOps_UseSurveillanceNodeTerminal::
	setvar VAR_0x8005, 2 @ ROCKETOPS_TRIGGER_SURVEILLANCE_NODE
	goto EventScript_RocketOps_Terminal

EventScript_RocketOps_UseArchiveNodeTerminal::
	setvar VAR_0x8005, 3 @ ROCKETOPS_TRIGGER_ARCHIVE_NODE
	goto EventScript_RocketOps_Terminal

EventScript_RocketOps_UseExtractionNodeTerminal::
	setvar VAR_0x8005, 4 @ ROCKETOPS_TRIGGER_EXTRACTION_NODE
	goto EventScript_RocketOps_Terminal

EventScript_RocketOps_TerminalOffline::
	msgbox Text_RocketOpsTerminalOffline

EventScript_RocketOps_Close::
	releaseall
	end


Text_GiovanniHubMonologue_C1::
	.string "Order is not a victory lap—it is a\n"
	.string "habit we build until it outlives us.\p"
	.string "I do not gamble on chaos. I shape\n"
	.string "lasting order and make it hold.$"

Text_GiovanniHubMonologue_C2::
	.string "Anomaly follows us in red shoes.\n"
	.string "That boy turns probability into noise.\p"
	.string "Even so, I do not gamble on chaos.\n"
	.string "I force order to answer the anomaly.$"

Text_GiovanniHubMonologue_C3::
	.string "Controlled collapse is still collapse.\n"
	.string "The architect owns every broken beam.\p"
	.string "I did not gamble on chaos—I rationed it.\n"
	.string "Responsibility is the weight that remains.$"

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
