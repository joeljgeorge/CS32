#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <iostream>
#include <fstream> 
#include <string>
#include <vector>
using namespace std;

class MapLoaderImpl
{
public:
	MapLoaderImpl();
	~MapLoaderImpl();
	bool load(string mapFile);
	size_t getNumSegments() const;
	bool getSegment(size_t segNum, StreetSegment& seg) const;
private:
	vector<StreetSegment> vStreets;
	int numOfStreetSegments;
};

MapLoaderImpl::MapLoaderImpl()
{
	numOfStreetSegments = 0;
}

MapLoaderImpl::~MapLoaderImpl()
{
	numOfStreetSegments = 0;
}

bool MapLoaderImpl::load(string mapFile)
{
	ifstream infile(mapFile);
	if (!infile)
	{
		cerr << "Error: Cannot open data.txt!";
		return false;
	}
	string s;
	while (getline(infile, s)) {
		StreetSegment sSegment;
		sSegment.streetName = s;
		if (getline(infile, s)) {
			int i = 0;
			string latitude1 = "";
			string longitude1 = "";
			string latitude2 = "";
			string longitude2 = "";
			for (i = 0; i < s.size() && s[i] != ' ' && s[i] != ','; i++)//code to construct GeoSegment, then add it to StreetSegment 
			{
				//if (s[i] != ',')
					latitude1 += s[i];
			}

			for(;s[i] == ' ' || s[i] == ','; i++){}

			for (; i < s.size() && s[i] != ' ' && s[i] != ','; i++)
			{
				/*if (s[i] != ',')*/
					longitude1 += s[i];
			}

			for (; s[i] == ' ' || s[i] == ','; i++) {}

			for (; i < s.size() && s[i] != ' ' && s[i] != ','; i++)
			{
				/*if (s[i] == ' ' || s[i] == ',')
					break;
				if (s[i] != ',')*/
					latitude2 += s[i];

			}

			for (; s[i] == ' ' || s[i] == ','; i++) {}

			for (; i < s.size() && s[i] != ' ' && s[i] != ','; i++)
			{
			/*	if (s[i] == ' ' || s[i] == ',')
					break;
				if (s[i] != ',')*/
					longitude2 += s[i];

			}
			/*cerr << latitude1 << " " << longitude1 << endl;
			cerr << latitude2 << " " << longitude2 << endl;*/
			sSegment.segment.start = GeoCoord(latitude1, longitude1);
			sSegment.segment.end = GeoCoord(latitude2, longitude2);  
		}
		else {
			cerr << "HELP!" << endl;
			return false;
		}

		int numAttractions = 0;
		if (getline(infile, s)) {
			numAttractions = stoi(s);
		}
		else {
			cerr << "HELP!" << endl;
			return false;
		}

		if (numAttractions > 0) {
			for (int i = 0; i < numAttractions; i++) {
				string attractionString = "";
				string attractionName = "";
				string latitude = "";
				string longitude = "";
				Attraction attractionSite;
				getline(infile, attractionString);
				int j = 0;
				for (j = 0; j < attractionString.size(); j++) {
					if (attractionString[j] == '|')
						break;
					attractionName += attractionString[j];
				}
				for (j++; j < attractionString.size() && attractionString[j] != ' ' && attractionString[j] != ','; j++) {
					/*if (attractionString[j] == ' ')
						break;
					if(attractionString[j] != ',')*/
						latitude += attractionString[j];
				}
				//another for loop for spaces and commas
				for (; attractionString[j] == ' ' || attractionString[j] == ','; j++) {}

				for (; j < attractionString.size() && attractionString[j] != ' ' && attractionString[j] != ','; j++) {
				/*	if (attractionString[j] == ' ')
						break;
					if (attractionString[j] != ',')*/
						longitude += attractionString[j];
				}
				attractionSite.name = attractionName;
				attractionSite.geocoordinates = GeoCoord(latitude, longitude);
				sSegment.attractions.push_back(attractionSite);
			}
		}
		vStreets.push_back(sSegment); 
		numOfStreetSegments++;
	}
	return true;
}

size_t MapLoaderImpl::getNumSegments() const
{
	return numOfStreetSegments;
}

bool MapLoaderImpl::getSegment(size_t segNum, StreetSegment &seg) const
{
	if (segNum >= numOfStreetSegments)
		return false;
	else {
		seg = vStreets[segNum];
		return true;
	}
}

//******************** MapLoader functions ************************************

// These functions simply delegate to MapLoaderImpl's functions.
// You probably don't want to change any of this code.

MapLoader::MapLoader()
{
	m_impl = new MapLoaderImpl;
}

MapLoader::~MapLoader()
{
	delete m_impl;
}

bool MapLoader::load(string mapFile)
{
	return m_impl->load(mapFile);
}

size_t MapLoader::getNumSegments() const
{
	return m_impl->getNumSegments();
}

bool MapLoader::getSegment(size_t segNum, StreetSegment& seg) const
{
   return m_impl->getSegment(segNum, seg);
}
