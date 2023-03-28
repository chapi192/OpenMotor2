#pragma once
#include <string>

enum GrainGeometry { TUBE, TUBE_AND_ROD, STAR, DOG_BONE, MULTIPERFORATED, DENDRITE, WAGON_WHEEL, MULTIFIN, DOUBLE_ANCHOR, GRATE };

class Grain
{
public:
	void setGrainRadius(float newGrainRadius);
	void setGrainDepth(float newGrainDepth);
	void setGeometryRadiusInner(float newInnerRadius);
	void setGeometryRadiusOuter(float newOuterRadius);
	void setNumberOfSpecializations(int newNumSpecializations);
	void setGeometry(GrainGeometry geometry);

	float getGrainRadius();
	float getGrainDepth();
	float getGeometryRadiusInner();
	float getGeometryRadiusOuter();
	int getNumberOfSpecializations();
	GrainGeometry getGrainGeometry();

private:
	float m_grainRadius;
	float m_grainDepth;
	float m_geometryInnerRadius = 0;
	float m_geometryOuterRadius;
	int m_numberOfSpecializations; // the points on the star, the amount of bone ends, the amount of fins, etc.
	GrainGeometry m_geometry = GrainGeometry::TUBE;
};
