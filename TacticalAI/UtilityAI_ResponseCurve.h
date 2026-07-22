#pragma once

typedef enum
{
	Step,
	Linear,
	Polynomial,
	Logistic,
	Logit
} ResponseCurveType;

struct ResponseCurve
{
	ResponseCurveType type;
	float m;
	float k;
	float b;
	float c;
	float min;
	float max;
};

float NormalizeInput(float value, float min, float max);
float CalculateResponse(float x, ResponseCurve curve);
