#ifndef SUPPORT
#define SUPPORT
#include "provided.h"
#include <string>

bool operator<(const GeoCoord &GeoCoord1, const GeoCoord &GeoCoord2);

bool operator>(const GeoCoord &GeoCoord1, const GeoCoord &GeoCoord2);

bool operator==(const GeoCoord &GeoCoord1, const GeoCoord &GeoCoord2);

struct NaviNode {
public:
	NaviNode(NaviNode* parent_input, NaviNode* daughter_input, GeoCoord gc_input, float fScore_input, float gScore_input, float hScore_input, std::string streetName) {
		parent = parent_input;
		daughter = daughter_input;
		gc = gc_input;
		fScore = fScore_input;
		gScore = gScore_input;
		hScore = hScore_input;
		sName = streetName;
	}
	NaviNode* parent;
	NaviNode *daughter;
	GeoCoord gc;
	float fScore;
	float gScore;
	float hScore;
	std::string sName;

};

bool operator<(const NaviNode &Node1, const NaviNode &Node2);

bool operator>(const NaviNode &Node1, const NaviNode &Node2);

bool operator==(const NaviNode &Node1, const NaviNode &Node2);

std::string directionOfLine(GeoSegment geoseg);

std::string findTurn(GeoSegment seg1, GeoSegment seg2);

#endif SUPPORT
