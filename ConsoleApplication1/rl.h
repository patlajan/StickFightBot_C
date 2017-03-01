#pragma once
#include <string>
#include <vector>
#include <map>

using namespace std;

class RL {
public:
	string* getPossibleActions();
	void act(string);
	void recaulculate(string, string, double);
	string getAction(string);
	void init();
	void finish();

private:
	map<string, map<string, pair<double, int>>> stateMap;
};