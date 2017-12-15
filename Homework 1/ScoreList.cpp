#include <iostream>
#include "ScoreList.h"

ScoreList::ScoreList() {
	return ;
}

bool ScoreList::add(unsigned long score) {
	if (0 <= score && score <= 100) {
		if (scoreList.insert(score) == -1)
			return false;
		else
			return true;
	}
	else
		return false;
}

bool ScoreList::remove(unsigned long score) {
	int position = scoreList.find(score);
	return scoreList.erase(position);
}

int ScoreList::size() const {
	return scoreList.size();
}

unsigned long ScoreList::minimum() const {
	if (scoreList.empty())
		return NO_SCORE;
	else {
		unsigned long min, temp = 0;
		scoreList.get(0, min); 
		for (int i = 0; i < scoreList.size(); i++) {
			scoreList.get(i, temp);
			if (temp < min)
				min = temp;
		}
		return min;
	}
}

unsigned long ScoreList::maximum() const {
	if (scoreList.empty())
		return NO_SCORE;
	else {
		unsigned long max, temp = 0;
		scoreList.get(0, max);
		for (int i = 0; i < scoreList.size(); i++) {
			scoreList.get(i, temp);
			if (temp > max)
				max = temp;
		}
		return max;
	}
}

