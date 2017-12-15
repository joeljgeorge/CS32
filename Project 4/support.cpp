#include "support.h"
#include "provided.h"

bool operator<(const GeoCoord &GeoCoord1, const GeoCoord &GeoCoord2) {
	if (GeoCoord1.latitude < GeoCoord2.latitude)
		return true;
	else if (GeoCoord1.latitude > GeoCoord2.latitude)
		return false;
	else if (GeoCoord1.longitude < GeoCoord2.longitude)
		return true;
	return false;
}

bool operator>(const GeoCoord &GeoCoord1, const GeoCoord &GeoCoord2) {
	if (GeoCoord1.latitude > GeoCoord2.latitude)
		return true;
	else if (GeoCoord1.latitude < GeoCoord2.latitude)
		return false;
	else if (GeoCoord1.longitude > GeoCoord2.longitude)
		return true;
	return false;
}

bool operator==(const GeoCoord &GeoCoord1, const GeoCoord &GeoCoord2) {
	if (GeoCoord1.latitude == GeoCoord2.latitude && GeoCoord1.longitude == GeoCoord2.longitude)
		return true;
	return false;
}

bool operator<(const NaviNode &Node1, const NaviNode &Node2) {
	return Node1.fScore < Node2.fScore;
}

bool operator>(const NaviNode &Node1, const NaviNode &Node2) {
	return Node1.fScore > Node2.fScore;
}

bool operator==(const NaviNode &Node1, const NaviNode &Node2) {
	return Node1.gc.latitude == Node2.gc.latitude && Node1.gc.longitude == Node2.gc.longitude;
}

std::string directionOfLine(GeoSegment geoseg) {
	const double angle = angleOfLine(geoseg);
	if (0 <= angle && angle <= 22.5) {
		return "east";
	}
	else if (22.5 < angle && angle <= 67.5) {
		return "northeast";
	}
	else if (67.5 < angle && angle <= 112.5) {
		return "north";
	}
	else if (112.5 < angle && angle <= 157.5) {
		return "northwest";
	}
	else if (157.5 < angle && angle <= 202.5) {
		return "west";
	}
	else if (202.5 < angle && angle <= 247.5) {
		return "southwest";
	}
	else if (247.5 < angle && angle <= 292.5) {
		return "south";
	}
	else if (292.5 < angle && angle <= 337.5) {
		return "southeast";
	}
	else if (337.5 < angle && angle < 360) {
		return "east";
	}
}

std::string findTurn(GeoSegment seg1, GeoSegment seg2) {
	if (angleBetween2Lines(seg1, seg2) >= 180)
		return "right";
	else
		return "left";
}
