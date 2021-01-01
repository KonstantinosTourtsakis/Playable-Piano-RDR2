/*
	THIS FILE IS A PART OF RDR 2 SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2019
*/

#include "script.h"
#include <string>
#include <vector>
#include "keyboard.h"
#include <ctime>;
#include "PianoCoords.h"

Vector3 piano;
float heading;
const char* scenarios[3] = { "PROP_HUMAN_PIANO", "PROP_HUMAN_PIANO_RIVERBOAT", "PROP_HUMAN_PIANO_SKETCHY"}; //different piano scenarios (music changes)
const char* scenario;
bool enabled = false;

Hash joaat(const char* button)
{
	return MISC::GET_HASH_KEY(button);
}

bool isPlayerNear(float x, float y, float z)
{
	Vector3 myPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true, true);

	float distance = MISC::GET_DISTANCE_BETWEEN_COORDS(myPos.x, myPos.y, myPos.z, x, y, z, true);
	if (distance <= 1.50)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool isPlayerNearPiano() //Piano coords are added through this function. isPlayerNear(x, y, z) is used to check if the player is close to the specified coordinates
{
	//Vector3 piano is used to play the scenario on the current piano
	//pianos[12] is an array that stores the piano coordinates
	int i;
	for (i = 0; i < 14; i++)
	{
		if (isPlayerNear(pianos[i].x, pianos[i].y, pianos[i].z))
		{
			piano.x = pianos[i].x;
			piano.y = pianos[i].y;
			piano.z = pianos[i].z;
			heading = pianos[i].heading;
			return true;
		}
	}
	return false;
}



int Piano1; //Button prompt (void main())
void update()
{
	Ped playerPed = PLAYER::PLAYER_PED_ID();
	
		
	HUD::_UIPROMPT_SET_ENABLED(Piano1, 0);
	HUD::_UIPROMPT_SET_VISIBLE(Piano1, 0);
	int i;
	srand(time(NULL));
	if (isPlayerNearPiano())
	{

		HUD::_UIPROMPT_SET_ENABLED(Piano1, 1);
		HUD::_UIPROMPT_SET_VISIBLE(Piano1, 1);

		if (enabled == true)
		{
			HUD::_UIPROMPT_SET_TEXT(Piano1, MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", "Stop playing"));
		}
		else
		{
			HUD::_UIPROMPT_SET_TEXT(Piano1, MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", "Play piano"));
		}

		if (HUD::_UIPROMPT_HAS_HOLD_MODE_COMPLETED(Piano1))      //IsKeyJustUp(0x45) || (PAD::IS_CONTROL_JUST_PRESSED(0, joaat("INPUT_FRONTEND_X")))
		{
			enabled = !enabled; //used to toggle the mod state (prevents the player from trying to use the scenario during transitions (play - stop)
			i = rand() % 3;
			scenario = scenarios[i];
			if (enabled)
			{
				TASK::TASK_START_SCENARIO_AT_POSITION(playerPed, MISC::GET_HASH_KEY(scenario), piano.x, piano.y, piano.z, heading, -1, 1, 1, 0, 0, 1);
				if (!isPlayerNear(-1639.11, -1364.80, 83.85))
				{
					MONEY::_MONEY_DECREMENT_CASH_BALANCE(50);
				}
				HUD::_UIPROMPT_SET_ENABLED(Piano1, 0); //resets the prompt during the delay
				HUD::_UIPROMPT_SET_VISIBLE(Piano1, 0);
				WAIT(10000);
			}
			else
			{
				TASK::CLEAR_PED_TASKS(PLAYER::PLAYER_PED_ID(), 1, 1);
				HUD::_UIPROMPT_SET_ENABLED(Piano1, 0);
				HUD::_UIPROMPT_SET_VISIBLE(Piano1, 0);
				WAIT(3000);
			}
		}
	}
}


void main()
{		
	
	Piano1 = HUD::_UIPROMPT_REGISTER_BEGIN();
	HUD::_UIPROMPT_SET_CONTROL_ACTION(Piano1, MISC::GET_HASH_KEY("INPUT_ENTER"));
	HUD::_UIPROMPT_SET_TEXT(Piano1, MISC::_CREATE_VAR_STRING(10, "LITERAL_STRING", "Play piano"));
	HUD::_UIPROMPT_SET_HOLD_MODE(Piano1, 1);
	//HUD::_UIPROMPT_SET_STANDARD_MODE(Piano1, 1);
	HUD::_UIPROMPT_REGISTER_END(Piano1);

	while (true)
	{
		update();
		WAIT(0);
	}
}

void ScriptMain()
{	
	srand(GetTickCount());
	main();
}
