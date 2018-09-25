/*! \file */
#include <cstdio>
#include <cassert>
#include <cmath>
#include <iostream>

///Allowable error
const double EPS = 1e-9;

/*! ---------------------------------
* Equality check taking into
* account the error
* (first argument == second argument)
* 
* @param [in] firstArg  first argument
* @param [in] secondArg second argument
* 
* @return True if |x-y| < EPS and false else
*
* -----------------------------------
*/
bool IsEqual(double firstArg, double secondArg) {
    return std::abs(firstArg - secondArg) < EPS;
}

/*! ---------------------------------
* Check: is the first argument more than the second one
* taking into account the error
*
* @param [in] firstArg  first argument
* @param [in] secondArg second argument
* 
* @return True if x > y + EPS and false else
*
* -----------------------------------
*/
bool IsMore(double firstArg, double secondArg) {
    return firstArg > secondArg + EPS;
}

/// Flag of an infinite number of roots
const int SS_INF_ROOTS = -1;

/*! ---------------------------------
* Solves a linear equation ax + b = 0
* 
* @param [in] a         a-coefficient
* @param [in] b         b-coefficient
* @param [out] result   Pointer to the root
* 
* @return Number of roots
*
* @note In case of infinite number of roots,
*       returns SS_INF_ROOTS
*
* @author uta1
*
* @date 06.09.18
*
* -----------------------------------
*/
int SolveQuadraticEquation(double a,
                           double b,
                           double *result
) {
    assert(std::isfinite(a));
    assert(std::isfinite(b));
    assert(result != nullptr);
    
    if (IsEqual(a, 0)) {
        return IsEqual(b, 0) ? SS_INF_ROOTS : 0;
    }
    
    *result = -b / a;
    
    return 1;
}

/*! ---------------------------------
* Solves a square equation ax^2 + bx + c = 0
* 
* @param [in] a         a-coefficient
* @param [in] b         b-coefficient
* @param [in] c         c-coefficient
* @param [out] result1  Pointer to the 1st root
*                       result1 < result2 (if 2 roots)
* @param [out] result2  Pointer to the 2nd root
* 
* @return Number of roots
*
* @note In case of infinite number of roots,
*       returns SS_INF_ROOTS
*
* @author uta1
*
* @date 06.09.18
*
* -----------------------------------
*/
int SolveQuadraticEquation(double a,
                           double b,
                           double c,
                           double *result1,
                           double *result2
) {
    assert(std::isfinite(a));
    assert(std::isfinite(b));
    assert(std::isfinite(c));
    assert(result1 != nullptr);
    assert(result2 != nullptr);
    assert(result1 != result2);

    if (IsEqual(a, 0)) {
        return SolveQuadraticEquation(b, c, result1);  
    }
    
    double D = b * b - 4 * a * c;
    
    if (IsEqual(D, 0)) {
        *result1 = -b / 2 / a;
        return 1;
    }
    
    if (IsMore(0, D)) {
        return 0;
    }

    double sqrtOfD = std::sqrt(D);
    
    *result1 = (-b - sqrtOfD) / 2 / a;
    *result2 = (-b + sqrtOfD) / 2 / a;

    return 2;
}

#if DEBUG==0
int main() {
    double a;
    double b;
    double c;

    printf("Enter coefficients of quadratic equation: ");
    
    //Poor parsing, I can feed your program with any shit I wish and it will be working and returning irrelevant results,
    //fix it, ok?
    std::cin >> a >> b >> c;
    
    double result1;
    double result2;
    
    int nRoots = SolveQuadraticEquation(a, b, c, &result1, &result2);
    
    switch(nRoots) {
        case 0:
          printf("No real roots\n");
          break;
        case 1:
          printf("One real root: %lf\n", result1);
          break;
        case 2:
          printf("Two real roots: %lf %lf\n", result1, result2);
          break;
        case SS_INF_ROOTS:
          printf("Infinitely many real roots\n");
          break;
    }
 
    return 0;
}
#endif
