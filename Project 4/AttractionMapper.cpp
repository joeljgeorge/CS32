#include "provided.h"
#include <iostream>
#include "MyMap.h"
#include <string>
#include <cctype>
using namespace std;

class AttractionMapperImpl
{
public:
	AttractionMapperImpl();
	~AttractionMapperImpl();
	void init(const MapLoader& ml);
	bool getGeoCoord(string attraction, GeoCoord& gc) const;
private:
	MyMap<string, GeoCoord> attractionMap;
};

AttractionMapperImpl::AttractionMapperImpl()
{}

AttractionMapperImpl::~AttractionMapperImpl()
{}

void AttractionMapperImpl::init(const MapLoader& ml)
{
	for (int i = 0; i < ml.getNumSegments(); i++) {
		StreetSegment temp; 
		ml.getSegment(i, temp); 
		int size = temp.attractions.size();
		if ( size > 0) {
			for (int j = 0; j < size; j++) {
				string attractionName = temp.attractions[j].name;
				string formattedName = "";
					for (int k = 0; k < attractionName.size(); k++) {
						formattedName += tolower(attractionName[k]);
					}
				attractionMap.associate(formattedName, (temp.attractions[j]).geocoordinates);
			}
		}
	}
}

bool AttractionMapperImpl::getGeoCoord(string attraction, GeoCoord& gc) const
{
	string formattedName = "";
	for (int i = 0; i < attraction.size(); i++) { 
		formattedName += tolower(attraction[i]);
	}
	const GeoCoord* ptrGeoCoord = attractionMap.find(formattedName);
	if (ptrGeoCoord == nullptr) {
		return false;
	}
	else {
		gc = *ptrGeoCoord;
		return true;
	}
	return false;  // This compiles, but may not be correct
}

//******************** AttractionMapper functions *****************************

// These functions simply delegate to AttractionMapperImpl's functions.
// You probably don't want to change any of this code.

AttractionMapper::AttractionMapper()
{
	m_impl = new AttractionMapperImpl;
}

AttractionMapper::~AttractionMapper()
{
	delete m_impl;
}

void AttractionMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

bool AttractionMapper::getGeoCoord(string attraction, GeoCoord& gc) const
{
	return m_impl->getGeoCoord(attraction, gc);
}
