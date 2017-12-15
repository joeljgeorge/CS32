#include "provided.h"
#include "support.h"
#include <string>
#include <vector>
#include <list>
#include <queue>
#include <iostream>
#include <algorithm>
using namespace std;

class NavigatorImpl
{
public:
    NavigatorImpl();
    ~NavigatorImpl();
    bool loadMapData(string mapFile);
    NavResult navigate(string start, string end, vector<NavSegment>& directions) const;
private:
	SegmentMapper sMapper;
	AttractionMapper aMapper;
};

NavigatorImpl::NavigatorImpl()
{
}

NavigatorImpl::~NavigatorImpl()
{
}

bool NavigatorImpl::loadMapData(string mapFile)
{
	MapLoader ml;
	if(!(ml.load(mapFile)))
		return false;
	else {
		sMapper.init(ml);
		aMapper.init(ml);
		return true;
	}
}

NavResult NavigatorImpl::navigate(string start, string end, vector<NavSegment> &directions) const
{
	GeoCoord startGC;
	if (!(aMapper.getGeoCoord(start, startGC))) {
		cerr << "BAD SOURCE" << endl;
		return NAV_BAD_SOURCE;
	}

	GeoCoord endGC;
	if (!(aMapper.getGeoCoord(end, endGC))) {
		cerr << "BAD DESTINATION" << endl;
		return NAV_BAD_DESTINATION;
	}

	//navigation code
	//priority_queue<NaviNode, list<NaviNode>, Compare> openList;
	vector<NaviNode> closedList; 
	vector<NaviNode> openList;
	list<NaviNode> totalList; 

	NaviNode startingNode(nullptr, nullptr, startGC, 0, 0, 0, (sMapper.getSegments(startGC))[0].streetName); 

	//openList.push(startingNode);
	openList.push_back(startingNode);
	totalList.push_back(startingNode);

	//cerr << startingNode.gc.latitudeText << " " << startingNode.gc.longitudeText << endl;
	//cerr << openList.top().gc.latitudeText;
	while (!(openList.empty())) 
	{
		int pos = 0;
		NaviNode current = openList[0];
		for (int i = 0; i < openList.size(); i++) {
			if (openList[i].fScore < current.fScore) {
				current = openList[i];
				pos = i;
			}
		}

		vector<NaviNode>::iterator it = openList.begin();
		it += pos;
		openList.erase(it);

		vector<StreetSegment> vStreetSeg = sMapper.getSegments(current.gc); 

		//cerr << vStreetSeg.size() << endl;
		for (int i = 0; i < vStreetSeg.size(); i++) {
			//create two new geocoordinates and put those into two new nodes (parent value is current)
			GeoCoord gC1 = vStreetSeg[i].segment.start;
			GeoCoord gC2 = vStreetSeg[i].segment.end;
			NaviNode nextNode1(nullptr, nullptr, current.gc, 0, 0, 0, vStreetSeg[i].streetName);
			NaviNode nextNode2(nullptr, nullptr, current.gc, 0, 0, 0, vStreetSeg[i].streetName);

			/*if (gC1 == endGC || gC2 == endGC) {
				cerr << "FOUND!" << endl;
				return NAV_SUCCESS;
			}*/

			for (int j = 0; j < vStreetSeg[i].attractions.size(); j++) {
				GeoCoord attractionCoordinates = vStreetSeg[i].attractions[j].geocoordinates;

				if (attractionCoordinates == endGC) {

					NaviNode finalDestination = NaviNode(&(*(find(totalList.begin(), totalList.end(), current))), nullptr, attractionCoordinates, 0, 0, 0, vStreetSeg[i].streetName);
					NaviNode* traversalPtr = &finalDestination;
					bool justCrossed = false;
					for (; traversalPtr != nullptr; traversalPtr = traversalPtr->parent) {
						if (traversalPtr->parent != nullptr) {
							traversalPtr->parent->daughter = traversalPtr;
							GeoSegment seg = GeoSegment(traversalPtr->parent->gc, traversalPtr->gc);
							string streetName = traversalPtr->sName;
							string streetNamePrev = traversalPtr->parent->sName;
							float distance = distanceEarthMiles(traversalPtr->parent->gc, traversalPtr->gc);
							if (streetName == streetNamePrev) {
								string direction = directionOfLine(seg);
								NavSegment nSeg = NavSegment(direction, streetName, distance, seg);
								directions.push_back(nSeg);
							}
							else{
								GeoSegment seg = GeoSegment(traversalPtr->parent->gc, traversalPtr->gc);
								string direction = directionOfLine(seg);
								NavSegment nSeg = NavSegment(direction, streetName, distance, seg);
								directions.push_back(nSeg);

								GeoSegment geoseg1 = GeoSegment((*(traversalPtr->parent->parent)).gc, (*(traversalPtr->parent)).gc);
								string turn = findTurn(geoseg1, seg);
								NavSegment nSeg1 = NavSegment(turn, streetName);
								directions.push_back(nSeg1);
							}
						}
						//cerr << traversalPtr->gc.latitudeText << " " << traversalPtr->gc.longitudeText << endl;
						/*else {
							GeoSegment seg = GeoSegment(traversalPtr->gc, traversalPtr->daughter->gc);
							string streetName = traversalPtr->sName;
							float distance = distanceEarthMiles(traversalPtr->gc, traversalPtr->daughter->gc);
							string direction = directionOfLine(seg);
							NavSegment nSeg = NavSegment(direction, streetName, distance, seg);
							directions.push_back(nSeg);
						}*/
					}
					reverse(directions.begin(), directions.end());
					//cerr << "FOUND!" << endl;
					return NAV_SUCCESS;
				}
			}

			if (gC2 == current.gc) {
				//for the first end of the street
				float fScore1 = 0;
				float gScore1 = 0;
				float hScore1 = 0;
				//cerr << current.gScore << endl;
				gScore1 = current.gScore + distanceEarthMiles(current.gc, gC1);
				hScore1 = distanceEarthMiles(gC1, endGC);
				//cerr << hScore1 << endl;
				fScore1 = gScore1 + hScore1;
				NaviNode *ptrToCurrent = &(*(find(totalList.begin(), totalList.end(), current)));
				nextNode1 = NaviNode(ptrToCurrent, nullptr, gC1, fScore1, gScore1, hScore1, vStreetSeg[i].streetName);
				//cerr << nextNode1.fScore << endl;
				//cerr << nextNode1.gc.latitudeText << " " << nextNode1.gc.longitudeText << endl;

				vector<NaviNode>::iterator it1_open = find(openList.begin(), openList.end(), nextNode1);
				vector<NaviNode>::iterator it1_closed = find(closedList.begin(), closedList.end(), nextNode1);

				if (((it1_open != openList.end()) && ((*it1_open).fScore <= nextNode1.fScore)) || ((it1_closed != closedList.end()) && ((*it1_closed).fScore <= nextNode1.fScore))) {}
				else {
					openList.push_back(nextNode1);
					totalList.push_back(nextNode1);
				}
			}
			else if (gC1 == current.gc) {
				//for the second end of the street
				float fScore2 = 0;
				float gScore2 = 0;
				float hScore2 = 0;
				gScore2 = current.gScore + distanceEarthMiles(current.gc, gC2);
				//cerr << gScore2 << endl;
				hScore2 = distanceEarthMiles(gC2, endGC);
				//cerr << hScore2 << endl;
				fScore2 = gScore2 + hScore2;
				NaviNode *ptrToCurrent = &(*(find(totalList.begin(), totalList.end(), current)));
				nextNode2 = NaviNode(ptrToCurrent, nullptr, gC2, fScore2, gScore2, hScore2, vStreetSeg[i].streetName);
				//cerr << nextNode2.gc.latitudeText << " " << nextNode2.gc.longitudeText << endl;

				vector<NaviNode>::iterator it2_open = find(openList.begin(), openList.end(), nextNode2);
				vector<NaviNode>::iterator it2_closed = find(closedList.begin(), closedList.end(), nextNode2);

				if (((it2_open != openList.end() && (*it2_open).fScore <= nextNode2.fScore)) || ((it2_closed != closedList.end() && (*it2_closed).fScore <= nextNode2.fScore))) {}
				else {
					openList.push_back(nextNode2);
					totalList.push_back(nextNode2);
				}
			}
			else {//this is when we're starting at an attraction
				//for the first end of the street
				float fScore1 = 0;
				float gScore1 = 0;
				float hScore1 = 0;
				gScore1 = current.gScore + distanceEarthMiles(current.gc, gC1);
				//cerr << gScore1 << endl;
				hScore1 = distanceEarthMiles(gC1, endGC);
				//cerr << hScore1 << endl;
				fScore1 = gScore1 + hScore1;
				NaviNode *ptrToCurrent = &(*(find(totalList.begin(), totalList.end(), current)));
				nextNode1 = NaviNode(ptrToCurrent, nullptr, gC1, fScore1, gScore1, hScore1, vStreetSeg[i].streetName);
				//cerr << nextNode1.fScore << endl;
				//cerr << nextNode1.gc.latitudeText << " " << nextNode1.gc.longitudeText << endl;

				//for the second end of the street
				float fScore2 = 0;
				float gScore2 = 0;
				float hScore2 = 0;
				gScore2 = current.gScore + distanceEarthMiles(current.gc, gC2);
				hScore2 = distanceEarthMiles(gC2, endGC);
				//cerr << hScore2 << endl;
				fScore2 = gScore2 + hScore2;
				nextNode2 = NaviNode(ptrToCurrent, nullptr, gC2, fScore2, gScore2, hScore2, vStreetSeg[i].streetName);
				//cerr << nextNode2.fScore << endl;
				//cerr << nextNode2.gc.latitudeText << " " << nextNode2.gc.longitudeText << endl;

				vector<NaviNode>::iterator it1_open = find(openList.begin(), openList.end(), nextNode1);
				vector<NaviNode>::iterator it1_closed = find(closedList.begin(), closedList.end(), nextNode1);

				if (((it1_open != openList.end()) && ((*it1_open).fScore <= nextNode1.fScore)) || ((it1_closed != closedList.end()) && ((*it1_closed).fScore <= nextNode1.fScore))) {}
				else {
					openList.push_back(nextNode1);
					totalList.push_back(nextNode1);
				}

				vector<NaviNode>::iterator it2_open = find(openList.begin(), openList.end(), nextNode2);
				vector<NaviNode>::iterator it2_closed = find(closedList.begin(), closedList.end(), nextNode2);

				if (((it2_open != openList.end() && (*it2_open).fScore <= nextNode2.fScore)) || ((it2_closed != closedList.end() && (*it2_closed).fScore <= nextNode2.fScore))) {}
				else {
					openList.push_back(nextNode2);
					totalList.push_back(nextNode2);
				}

			}
		}
		closedList.push_back(current);
	}
	//cerr << "No route found..." << endl;
	return NAV_NO_ROUTE;  // This compiles, but may not be correct
}

//******************** Navigator functions ************************************

// These functions simply delegate to NavigatorImpl's functions.
// You probably don't want to change any of this code.

Navigator::Navigator()
{
    m_impl = new NavigatorImpl;
}

Navigator::~Navigator()
{
    delete m_impl;
}

bool Navigator::loadMapData(string mapFile)
{
    return m_impl->loadMapData(mapFile);
}

NavResult Navigator::navigate(string start, string end, vector<NavSegment>& directions) const
{
    return m_impl->navigate(start, end, directions);
}
