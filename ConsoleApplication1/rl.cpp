#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <fstream>
#include "opencv2/imgproc/imgproc.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <Windows.h>
#include <tchar.h>
#include <cstdio>
#include <ctime>
#include <iostream>
#include <string>
#include <set>
#include "rl.h"

string* RL::getPossibleActions()
{
	return new string[3]{"hitRight", "hitLeft", "wait"};
}

void RL::act(string action)
{
	if (action == "hitRight") {
		INPUT input;
		WORD vkey = VK_RIGHT; // see link below
		input.type = INPUT_KEYBOARD;
		input.ki.wScan = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
		input.ki.time = 0;
		input.ki.dwExtraInfo = 0;
		input.ki.wVk = vkey;
		input.ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN
		SendInput(1, &input, sizeof(INPUT));

		input.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &input, sizeof(INPUT));
	}
	else if (action == "hitLeft") {
		INPUT input;
		WORD vkey = VK_LEFT; // see link below
		input.type = INPUT_KEYBOARD;
		input.ki.wScan = MapVirtualKey(vkey, MAPVK_VK_TO_VSC);
		input.ki.time = 0;
		input.ki.dwExtraInfo = 0;
		input.ki.wVk = vkey;
		input.ki.dwFlags = 0; // there is no KEYEVENTF_KEYDOWN
		SendInput(1, &input, sizeof(INPUT));

		input.ki.dwFlags = KEYEVENTF_KEYUP;
		SendInput(1, &input, sizeof(INPUT));
	}
	//else sit & wait
}

void RL::recaulculate(string state, string action, double reward)
{
	//find state in this->state;
	map<string, map<string, pair<double, int>>>::const_iterator it = this->stateMap.find(state);
	//foreach iterator
	if (it == this->stateMap.end())
	{
		//nothing found
		map<string, pair<double, int>> map2 = map<string, pair<double, int>>();
		map2.insert(pair<string, pair<double, int>>(action, pair<double, int>(reward, 1)));
		this->stateMap.insert(pair<string, map<string, pair<double, int>>>(state, map2));
	}
	else {
		map<string, pair<double, int>> map1 = it._Ptr->_Myval.second;
		map<string, pair<double, int>>::const_iterator it2 = map1.find(action);
		if (it2 == map1.end())
		{
			map1.insert(pair<string, pair<double, int>>(action, pair<double, int>(reward, 1)));
		}
		else {
			double reward = it2._Ptr->_Myval.second.first;
			int timesExplored = it2._Ptr->_Myval.second.second;

			double newReward = (reward * timesExplored / 2 + reward) / (timesExplored / 2 + 1);
			it2._Ptr->_Myval.second = pair<double, int>(timesExplored + 1, newReward);
		}
	}	
}

string RL::getAction(string state)
{
	//find state in this->state;
	map<string, map<string, pair<double, int>>>::const_iterator it = this->stateMap.find(state);
	if (it == this->stateMap.end())
	{
		//nothing found
		//select action on random
		return "hitLeft";
	}
	else {
		map<string, pair<double, int>> map1 = it._Ptr->_Myval.second;
		set<string> actionsSet = set<string>();
		for (auto const &ent1 : map1) {
			double reward =  ent1.second.first;
			int timesExplored = ent1.second.second;
			actionsSet.insert(ent1.first);

			//if reward is good, roll a random(1, exploredCount) if, random == 1, explore random, else take action
			//if reward is poor, continue
		}
		//if for cycle didn't return a value, this means no viable options are available in memory;
		//select action on random
		return "hitLeft";
	}
}

void RL::init()
{
	//load from file and deserialize this->state;
}

void RL::finish()
{
	//serialize this->state; and save to file
}
