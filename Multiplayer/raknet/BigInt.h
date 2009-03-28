/*
 * BigInts are stored as 32-bit integer arrays.
 * Each integer in the array is referred to as a limb ala GMP.
 * Lower numbered limbs are less significant to the number represented.
 * eg, limb 0 is the least significant limb.
 * Also known as little-endian digit order
 */

#ifndef BIG_INT_HPP
#define BIG_INT_HPP

#include "Platform.h"
//#include <string>

namespace big
{
	// returns the degree of the base 2 monic polynomial
	// (the number of bits used to represent the number)
	// eg, 0 0 0 0 1 0 1 1 ... => 28 out of 32 used
	uint32_t Degree(uint32_t v);

	// returns the number of limbs that are actually used
	int LimbDegree(const uint32_t *n, int limbs);

	// return bits used
	uint32_t Degree(const uint32_t *n, int limbs);

	// lhs = rhs (unequal limbs)
	void Set(uint32_t *lhs, int lhs_limbs, const uint32_t *rhs, int rhs_limbs);

	// lhs = rhs (equal limbs)
	void Set(uint32_t *lhs, int limbs, const uint32_t *rhs);

	// lhs = rhs (32-bit extension)
	void Set32(uint32_t *lhs, int lhs_limbs, const uint32_t rhs);

	// Comparisons where both operands have the same number of limbs
	bool Less(int limbs, const uint32_t *lhs, const uint32_t *rhs);
	bool Greater(int limbs, const uint32_t *lhs, const uint32_t *rhs);
	bool Equal(int limbs, const uint32_t *lhs, const uint32_t *rhs);

	// lhs < rhs
	bool Less(const uint32_t *lhs, int lhs_limbs, const uint32_t *rhs, int rhs_limbs);

	// lhs >= rhs
	inline bool GreaterOrEqual(const uint32_t *lhs, int lhs_limbs, const uint32_t *rhs, int rhs_limbs)
	{
		return !Less(lhs, lhs_limbs, rhs, rhs_limbs);
	}

	// lhs > rhs
	bool Greater(const uint32_t *lhs, int lhs_limbs, const uint32_t *rhs, int rhs_limbs);

	// lhs <= rhs
	inline bool LessOrEqual(const uint32_t *lhs, int lhs_limbs, const uint32_t *rhs, int rhs_limbs)
	{
		return !Greater(lhs, lhs_limbs, rhs, rhs_limbs);
	}

	// lhs == rhs
	bool Equal(const uint32_t *lhs, int lhs_limbs, const uint32_t *rhs, int rhs_limbs);

	// lhs == rhs
	bool Equal32(const uint32_t *lhs, int lhs_limbs, uint32_t rhs);

	// n >>= shift*32
	void LimbShiftRight(uint32_t *n, int limbs, int rhs_shift);

	// n <<= shift*32
	void LimbShiftLeft(uint32_t *n, int limbs, int rhs_shift);

	// lhs = rhs >>> shift
	// Precondition: 0 <= shift < 31
	void BitShiftRight(uint32_t *result, int result_limbs, const uint32_t *lhs, int lhs_limbs, int rhs_shift);

	// lhs = rhs <<< shift
	// Precondition: 0 <= shift < 31
	void BitShiftLeft(uint32_t *result, int result_limbs, const uint32_t *lhs, int lhs_limbs, int rhs_shift);

	// lhs += rhs, return carry out
	// precondition: lhs_limbs >= rhs_limbs
	uint32_t Add(uint32_t *lhs, int lhs_limbs, const uint32_t *rhs, int rhs_limbs);

	// out = lhs + rhs, return carry out
	// precondition: lhs_limbs >= rhs_limbs
	uint32_t Add(uint32_t *out, const uint32_t *lhs, int lhs_limbs, const uint32_t *rhs, int rhs_limbs);

	// lhs += rhs, return carry out
	// precondition: lhs_limbs > 0
	uint32_t Add32(uint32_t *lhs, int lhs_limbs, uint32_t rhs);

	// lhs -= rhs, return borrow out
	// precondition: lhs_limbs >= rhs_limbs
	int32_t Subtract(uint32_t *lhs, int lhs_limbs, const uint32_t *rhs, int rhs_limbs);

	// out = lhs - rhs, return borrow out
	// precondition: lhs_limbs >= rhs_limbs
	int32_t Subtract(uint32_t *out, const uint32_t *lhs, int lhs_limbs, const uint32_t *rhs, int rhs_limbs);

	// lhs -= rhs, return borrow out
	// precondition: lhs_limbs > 0, result limbs = lhs_limbs
	int32_t Subtract32(uint32_t *lhs, int lhs_limbs, uint32_t rhs);

	// n = -n
	void Negate(uint32_t *n, int limbs);

	// n = ~n, only invert bits up to the MSB, but none above that
	void BitNot(uint32_t *n, int limbs);

	// n = ~n, invert all bits, even ones above MSB
	void LimbNot(uint32_t *n, int limbs);

	// Return the carry out from A += B << S
    uint32_t AddLeftShift32(
    	int limbs,		// Number of limbs in parameter A and B
    	uint32_t *A,			// Large number
    	const uint32_t *B,	// Large number
    	uint32_t S);			// 32-bit number

	// Return the carry out from result = A * B
    uint32_t Multiply32(
    	int limbs,		// Number of limbs in parameter A, result
    	uint32_t *result,	// Large number
    	const uint32_t *A,	// Large number
    	uint32_t B);			// 32-bit number

	// Return the carry out from X = X * M + A
    uint32_t MultiplyAdd32(
    	int limbs,	// Number of limbs in parameter A and B
    	uint32_t *X,		// Large number
    	uint32_t M,		// Large number
    	uint32_t A);		// 32-bit number

	// Return the carry out from A += B * M
    uint32_t AddMultiply32(
    	int limbs,		// Number of limbs in parameter A and B
    	uint32_t *A,			// Large number
    	const uint32_t *B,	// Large number
    	uint32_t M);			// 32-bit number

	// product = x * y
	void SimpleMultiply(
		int limbs,		// Number of limbs in parameters x, y
		uint32_t *product,	// Large number; buffer size = limbs*2
		const uint32_t *x,	// Large number
		const uint32_t *y);	// Large number

	// product = x ^ 2
	void SimpleSquare(
		int limbs,		// Number of limbs in parameter x
		uint32_t *product,	// Large number; buffer size = limbs*2
		const uint32_t *x);	// Large number

	/*
	 * Multiply two large numbers using the Schoolbook method
	 * Only produces the low y_limbs of the result 
	 * 
	 * The product buffer may not be pointed to by x or y
	 */
	void HalfSchoolbookMultiply(
		uint32_t *product,	// Buffer size = x_limbs+y_limbs
		const uint32_t *x,	// Number to multiply, buffer size = x_limbs
		int x_limbs,	// Size of x
		const uint32_t *y,	// Number to multiply, buffer size = y_limbs
		int y_limbs);	// Size of y

	/*
	 * Multiply two large numbers using the Schoolbook method
	 * 
	 * The product buffer may not be pointed to by x or y
	 */
	void SchoolbookMultiply(
		uint32_t *product,	// Buffer size = x_limbs+y_limbs
		const uint32_t *x,	// Number to multiply, buffer size = x_limbs
		int x_limbs,	// Size of x
		const uint32_t *y,	// Number to multiply, buffer size = y_limbs
		int y_limbs);	// Size of y

	// product = xy
	// memory space for product may not overlap with x,y
    void Multiply(
    	int limbs,		// Number of limbs in x,y
    	uint32_t *product,	// Product; buffer size = limbs*2
    	const uint32_t *x,	// Large number; buffer size = limbs
    	const uint32_t *y);	// Large number; buffer size = limbs

	// product = low half of x * y product
	void SimpleMultiplyLowHalf(
		int limbs,		// Number of limbs in parameters x, y
		uint32_t *product,	// Large number; buffer size = limbs
		const uint32_t *x,	// Large number
		const uint32_t *y);	// Large number

	// product = x^2
	// memory space for product may not overlap with x
    void Square(
    	int limbs,		// Number of limbs in x
    	uint32_t *product,	// Product; buffer size = limbs*2
    	const uint32_t *x);	// Large number; buffer size = limbs

	// Multiply two large, 2's complement signed numbers: result = a0 * b0
    void SignedMultiply(
    	int limbs,		// Number of limbs in parameters a0,b0
    	uint32_t *result,	// Output, buffer size = limbs*2
    	const uint32_t *a0,	// Large number, buffer size = limbs
    	const uint32_t *b0);	// Large number, buffer size = limbs

	// Returns the remainder of N / divisor for a 32-bit divisor
    uint32_t Modulus32(
    	int limbs,     // Number of limbs in parameter N
    	const uint32_t *N,  // Large number, buffer size = limbs
    	uint32_t divisor);  // 32-bit number

	/*
	 * 'A' is overwritten with the quotient of the operation
	 * Returns the remainder of 'A' / divisor for a 32-bit divisor
	 * 
	 * Does not check for divide-by-zero
	 */
    uint32_t Divide32(
    	int limbs,		// Number of limbs in parameter A
    	uint32_t *A,			// Large number, buffer size = limbs
    	uint32_t divisor);	// 32-bit number

	// returns (n ^ -1) Mod 2^32
	uint32_t MulInverseGF32(uint32_t n);

	/*
	 * Schoolbook division algorithm
	 * 
	 * Returns true on success and false on failure (like divide by 0)
	 * 
	 * Quotient and Remainder pointers can be the same as any other
	*/
	bool SchoolbookDivide(
		const uint32_t *dividend,	// Large number (numerator), buffer size = dividend_limbs
		int dividend_limbs,		// Dividend limbs
		const uint32_t *divisor,		// Large number (denominator), buffer size = divisor_limbs
		int divisor_limbs,		// Divisor limbs
		uint32_t *quotient,			// Quotient of division, buffer size = dividend_limbs
		uint32_t *remainder);		// Remainder of division, buffer size = divisor_limbs

	// Convert bigint to string
	//std::string ToStr(const uint32_t *n, int limbs, int base = 10);

	// Convert string to bigint
	// Return 0 if string contains non-digit characters, else number of limbs used
	int ToInt(uint32_t *lhs, int max_limbs, const char *rhs, uint32_t base = 10);

	/*
	 * Computes: result = (n ^ -1) (Mod modulus)
	 * Such that: result * n (Mod modulus) = 1
	 * Using Extended Euclid's Algorithm (GCDe)
	 * 
	 * This is not always possible, so it will return false iff not possible.
	 */
	bool InvMod(
		const uint32_t *n,		//	Large number, buffer size = n_limbs
		int n_limbs,		//	Size of n
		const uint32_t *modulus,	//	Large number, buffer size = limbs
		int limbs,			//	Size of modulus
		uint32_t *result);		//	Large number, buffer size = limbs

	/*
	 * Computes: result = GCD(a, b)  (greatest common divisor)
	 * 
	 * Length of result is the length of the smallest argument
	 */
	void GCD(
		const uint32_t *a,	//	Large number, buffer size = a_limbs
		int a_limbs,	//	Size of a
		const uint32_t *b,	//	Large number, buffer size = b_limbs
		int b_limbs,	//	Size of b
		uint32_t *result);	//	Large number, buffer size = min(a, b)

	// Calculates mod_inv from low limb of modulus
	uint32_t MonModInv(uint32_t modulus0);

	// Compute n_residue for Montgomery reduction
	void MonInputResidue(
		const uint32_t *n,		//	Large number, buffer size = n_limbs
		int n_limbs,		//	Number of limbs in n
		const uint32_t *modulus,	//	Large number, buffer size = m_limbs
		int m_limbs,		//	Number of limbs in modulus
		uint32_t *n_residue);	//	Result, buffer size = m_limbs

	// result = a * b * r^-1 (Mod modulus) in Montgomery domain
	void MonPro(
		int limbs,				// Number of limbs in each parameter
		const uint32_t *a_residue,	// Large number, buffer size = limbs
		const uint32_t *b_residue,	// Large number, buffer size = limbs
		const uint32_t *modulus,		// Large number, buffer size = limbs
		uint32_t mod_inv,			// MonModInv() return
		uint32_t *result);			// Large number, buffer size = limbs

	// result = a * r^-1 (Mod modulus) in Montgomery domain
	// The result may be greater than the modulus, but this is okay since
	// the result is still in the RNS.  MonFinish() corrects this at the end.
	void MonReduce(
		int limbs,			// Number of limbs in each parameter
		uint32_t *s,				// Large number, buffer size = limbs*2, gets clobbered
		const uint32_t *modulus,	// Large number, buffer size = limbs
		uint32_t mod_inv,		// MonModInv() return
		uint32_t *result);		// Large number, buffer size = limbs

	// result = a * r^-1 (Mod modulus) in Montgomery domain
	void MonFinish(
		int limbs,			// Number of limbs in each parameter
		uint32_t *n,				// Large number, buffer size = limbs
		const uint32_t *modulus,	// Large number, buffer size = limbs
		uint32_t mod_inv);		// MonModInv() return

	// Computes: result = base ^ exponent (Mod modulus)
	// Using Montgomery multiplication with simple squaring method
	// Base parameter must be a Montgomery Residue created with MonInputResidue()
	void MonExpMod(
		const uint32_t *base,	//	Base for exponentiation, buffer size = mod_limbs
		const uint32_t *exponent,//	Exponent, buffer size = exponent_limbs
		int exponent_limbs,	//	Number of limbs in exponent
		const uint32_t *modulus,	//	Modulus, buffer size = mod_limbs
		int mod_limbs,		//	Number of limbs in modulus
		uint32_t mod_inv,		//	MonModInv() return
		uint32_t *result);		//	Result, buffer size = mod_limbs

	// Computes: result = base ^ exponent (Mod modulus)
	// Using Montgomery multiplication with simple squaring method
	void ExpMod(
		const uint32_t *base,	//	Base for exponentiation, buffer size = base_limbs
		int base_limbs,		//	Number of limbs in base
		const uint32_t *exponent,//	Exponent, buffer size = exponent_limbs
		int exponent_limbs,	//	Number of limbs in exponent
		const uint32_t *modulus,	//	Modulus, buffer size = mod_limbs
		int mod_limbs,		//	Number of limbs in modulus
		uint32_t mod_inv,		//	MonModInv() return
		uint32_t *result);		//	Result, buffer size = mod_limbs

	// Computes: result = base ^ exponent (Mod modulus=mod_p*mod_q)
	// Using Montgomery multiplication with Chinese Remainder Theorem
	void ExpCRT(
		const uint32_t *base,	//	Base for exponentiation, buffer size = base_limbs
		int base_limbs,		//	Number of limbs in base
		const uint32_t *exponent,//	Exponent, buffer size = exponent_limbs
		int exponent_limbs,	//	Number of limbs in exponent
		const uint32_t *mod_p,	//	Large number, factorization of modulus, buffer size = p_limbs
		uint32_t p_inv,			//	MonModInv() return
		const uint32_t *mod_q,	//	Large number, factorization of modulus, buffer size = q_limbs
		uint32_t q_inv,			//	MonModInv() return
		const uint32_t *pinvq,	//	Large number, InvMod(p, q) precalculated, buffer size = phi_limbs
		int mod_limbs,		//	Number of limbs in p, q and phi
		uint32_t *result);		//	Result, buffer size = mod_limbs*2
}

#endif // include guard
