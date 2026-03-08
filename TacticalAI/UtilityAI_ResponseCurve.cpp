#include "UtilityAI_ResponseCurve.h"

#include <cmath>
#include <algorithm>

namespace UtilityAI
{

	float NormalizeInput(float value, float min, float max)
	{
		const float normalizedValue = (value - min) / (max - min);
		return normalizedValue;
	}

	float CalculateResponse(float x, ResponseCurve curve)
	{
		const auto min = curve.min;
		const auto max = curve.max;
		const auto type = curve.type;
		const auto m = curve.m;
		const auto k = curve.k;
		const auto b = curve.b;
		const auto c = curve.c;

		float input = std::clamp(x, min, max);
		input = NormalizeInput(input, min, max);

		float y = 0.0f;
		// Polynomial
		float n;
		// Logistic
		const float e = 2.71828f; // Euler's number. I think? Lecture slides did not explain if this is what it's supposed to be.
		const float exponent = -input + c;
		const float denominator = 1.0f + 1000.0f * e * pow(m, exponent);

		switch ( type )
		{
			case Step:
				if ( input != 0.0f ) { return 1.0f; }
				else { return 0.0f; }
			case Linear:
			case Polynomial:
				n = input - c;
				y = m * pow(n, k) + b;
				break;
			case Logistic:
				y = (k / denominator) + b;
				break;
				//case Logit: // Not implemented yet
					//break;
		}

		y = std::clamp(y, 0.0f, 1.0f);
		return y;
	}

}
