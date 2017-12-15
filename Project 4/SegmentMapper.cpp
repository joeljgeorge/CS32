#include "provided.h"
#include "MyMap.h"
#include "support.h"
#include <vector>
using namespace std;

class SegmentMapperImpl
{
public:
	SegmentMapperImpl();
	~SegmentMapperImpl();
	void init(const MapLoader& ml);
	vector<StreetSegment> getSegments(const GeoCoord& gc) const;
private:
	MyMap<GeoCoord, vector<StreetSegment>> geoCoordMap; 
};

SegmentMapperImpl::SegmentMapperImpl()
{}

SegmentMapperImpl::~SegmentMapperImpl()
{}

void SegmentMapperImpl::init(const MapLoader& ml)
{
	for (int i = 0; i < ml.getNumSegments(); i++) {
		StreetSegment temp;
		ml.getSegment(i, temp);

		GeoCoord gC1 = temp.segment.start;
		vector<StreetSegment> gVector1;//TODO: You don't need gVector2 AND gVector1, just do one or the other
		gVector1.push_back(temp);

		GeoCoord gC2 = temp.segment.end;
		vector<StreetSegment> gVector2;
		gVector2.push_back(temp);

		vector<StreetSegment> *fValue1 = geoCoordMap.find(gC1); 
		vector<StreetSegment> *fValue2 = geoCoordMap.find(gC2);

		if (fValue1 == nullptr)
			geoCoordMap.associate(gC1, gVector1);
		else {
			(*fValue1).push_back(temp); 
		}
		if (fValue2 == nullptr)
			geoCoordMap.associate(gC2, gVector2);
		else {
			(*fValue2).push_back(temp); 
		}
		for (int j = 0; j < temp.attractions.size(); j++) {
			GeoCoord attraction = temp.attractions[j].geocoordinates;
			vector<StreetSegment> aVector;
			aVector.push_back(temp);
			vector<StreetSegment> *aValue = geoCoordMap.find(attraction);
			if (aValue == nullptr) {
				geoCoordMap.associate(attraction, aVector);
			}
			else{
				(*aValue).push_back(temp);
			}
		}
	}
}

vector<StreetSegment> SegmentMapperImpl::getSegments(const GeoCoord& gc) const
{
	const vector<StreetSegment> *return_val = geoCoordMap.find(gc);
	vector<StreetSegment> emptyVector;
	if (return_val == nullptr)
		return  emptyVector;
	else {
		return *return_val;
	}
}

//******************** SegmentMapper functions ********************************

// These functions simply delegate to SegmentMapperImpl's functions.
// You probably don't want to change any of this code.

SegmentMapper::SegmentMapper()
{
	m_impl = new SegmentMapperImpl;
}

SegmentMapper::~SegmentMapper()
{
	delete m_impl;
}

void SegmentMapper::init(const MapLoader& ml)
{
	m_impl->init(ml);
}

vector<StreetSegment> SegmentMapper::getSegments(const GeoCoord& gc) const
{
	return m_impl->getSegments(gc);
}
