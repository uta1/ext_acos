#include "quadratic_equations_solution.cpp"
#include <gtest/gtest.h>
 
TEST(TwoRootsTests, N1) { 
    double *a = new double(0);
    double *b = new double(0);

    ASSERT_EQ(2, SolveQuadraticEquation(1.0, -4.0, -21.0, a, b));

    ASSERT_EQ(-3.0, *a);
    ASSERT_EQ(7.0, *b);

    delete a;
    delete b;
}

TEST(TwoRootsTests, N2) { 
    double *a = new double(0);
    double *b = new double(0);

    ASSERT_EQ(2, SolveQuadraticEquation(1.0, -1.0, 0.0, a, b));

    ASSERT_EQ(0.0, *a);
    ASSERT_EQ(1.0, *b);

    delete a;
    delete b;
}

TEST(OneRootTests, N1) { 
    double *a = new double(0);
    double *b = new double(0);

    ASSERT_EQ(1, SolveQuadraticEquation(3.0, 0.0, 0.0, a, b));

    ASSERT_EQ(0.0, *a);

    delete a;
    delete b;
}

TEST(OneRootTests, N2) { 
    double *a = new double(0);
    double *b = new double(0);

    ASSERT_EQ(1, SolveQuadraticEquation(1.0, -3.0, 2.25, a, b));

    ASSERT_EQ(1.5, *a);
}

TEST(OneRootTests, N3) { 
    double *a = new double(0);
    double *b = new double(0);

    ASSERT_EQ(1, SolveQuadraticEquation(0.0, -1.0, 17.5, a, b));

    ASSERT_EQ(17.5, *a);

    delete a;
    delete b;
}

TEST(OneRootTests, N4) { 
    double *a = new double(0);
    double *b = new double(0);

    ASSERT_EQ(1, SolveQuadraticEquation(0.0, 8.0, 0.0, a, b));

    ASSERT_EQ(0.0, *a);

    delete a;
    delete b;
}

TEST(ZeroRootsTests, N1) { 
    double *a = new double(0);
    double *b = new double(0);

    ASSERT_EQ(0, SolveQuadraticEquation(1.0, 1.0, 1.0, a, b));

    delete a;
    delete b;
}

TEST(ZeroRootsTests, N2) { 
    double *a = new double(0);
    double *b = new double(0);

    ASSERT_EQ(0, SolveQuadraticEquation(2.0, 0.0, 1.0, a, b));

    delete a;
    delete b;
}

TEST(ZeroRootsTests, N3) { 
    double *a = new double(0);
    double *b = new double(0);

    ASSERT_EQ(0, SolveQuadraticEquation(0.0, 0.0, -1.0, a, b));

    delete a;
    delete b;
}

TEST(INFRootsTests, N3) { 
    double *a = new double(0);
    double *b = new double(0);

    ASSERT_EQ(SS_INF_ROOTS, SolveQuadraticEquation(0.0, 0.0, 0.0, a, b));

    delete a;
    delete b;
}

#if DEBUG == 1
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
#endif
