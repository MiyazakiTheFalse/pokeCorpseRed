Route6_MapScripts::
	.byte 0

Route6_EventScript_RickyDrill::
	getpartysize
	goto_if_lt VAR_RESULT, 2, Route6_EventScript_RickyNeedTraining
	trainerbattle_single TRAINER_CAMPER_RICKY, Route6_Text_RickyIntro, Route6_Text_RickyDefeat, Route6_EventScript_RickyPassedDrill, NO_MUSIC
	msgbox Route6_Text_RickyPostBattleRepeat, MSGBOX_AUTOCLOSE
	end

Route6_EventScript_RickyNeedTraining::
	msgbox Route6_Text_RickyNotEnoughMons, MSGBOX_AUTOCLOSE
	end

Route6_EventScript_RickyPassedDrill::
	msgbox Route6_Text_RickyPostBattle
	end

Route6_EventScript_NancyDrill::
	getpartysize
	goto_if_lt VAR_RESULT, 2, Route6_EventScript_NancyNeedTraining
	trainerbattle_single TRAINER_PICNICKER_NANCY, Route6_Text_NancyIntro, Route6_Text_NancyDefeat, Route6_EventScript_NancyPassedDrill, NO_MUSIC
	msgbox Route6_Text_NancyPostBattleRepeat, MSGBOX_AUTOCLOSE
	end

Route6_EventScript_NancyNeedTraining::
	msgbox Route6_Text_RickyNotEnoughMons, MSGBOX_AUTOCLOSE
	end

Route6_EventScript_NancyPassedDrill::
	msgbox Route6_Text_NancyPostBattle
	end

Route6_EventScript_UndergroundPathSign::
	msgbox Route6_Text_UndergroundPathSign, MSGBOX_SIGN
	end
