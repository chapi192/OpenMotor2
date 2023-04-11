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
	void setGrainDensity(float newGrainDensity);
	void setGrainMass(float newGrainMass);
	void setGeometry(GrainGeometry geometry);

	float getGrainRadius();
	float getGrainDepth();
	float getGeometryRadiusInner();
	float getGeometryRadiusOuter();
	float getGrainDensity();
	float getGrainMass();
	int getNumberOfSpecializations();
	GrainGeometry getGrainGeometry();

private:
	float m_grainRadius = 0.5f;
	float m_grainDepth = 0.5f;
	float m_geometryInnerRadius = 0.f;
	float m_geometryOuterRadius = 0.f;
	int m_numberOfSpecializations = 0; // the points on the star, the amount of bone ends, the amount of fins, etc.

	float m_grainDensity = 0.f;
	float m_grainMass = 0.f;

	GrainGeometry m_geometry = GrainGeometry::TUBE;
};
