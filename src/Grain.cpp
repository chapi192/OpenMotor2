#include "Grain.hpp"

void Grain::setGrainRadius(float newGrainRadius) { m_grainRadius = newGrainRadius; }

void Grain::setGrainDepth(float newGrainDepth) { m_grainDepth = newGrainDepth; }

void Grain::setGeometryRadiusInner(float newInnerRadius) { m_geometryInnerRadius = newInnerRadius; }

void Grain::setGeometryRadiusOuter(float newOuterRadius) { m_geometryOuterRadius = newOuterRadius; }

void Grain::setGrainDensity(float newGrainDensity) { m_grainDensity = newGrainDensity; }

void Grain::setGrainMass(float newGrainMass) { m_grainMass = newGrainMass; }

void Grain::setNumberOfSpecializations(int newNumSpecializations) { m_numberOfSpecializations = newNumSpecializations; }

void Grain::setGeometry(GrainGeometry geometry) { m_geometry = geometry; }

float Grain::getGrainRadius() { return m_grainRadius; }

float Grain::getGrainDepth() { return m_grainDepth; }

float Grain::getGeometryRadiusInner() { return m_geometryInnerRadius; }

float Grain::getGeometryRadiusOuter() { return m_geometryOuterRadius; }

float Grain::getGrainDensity() { return m_grainDensity; }

float Grain::getGrainMass() { return m_grainMass; }

int Grain::getNumberOfSpecializations() { return m_numberOfSpecializations; }

GrainGeometry Grain::getGrainGeometry() { return m_geometry; }

