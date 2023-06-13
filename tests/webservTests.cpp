#include <gtest/gtest.h>
#include <iostream>

int ft_strlen(char *str)
{
    int i;

   if (!str)
        return (0);
    i = 0;
    while (str[i])
        i++;
        
    return (i);
}

// Series of unit tests, to tests ft_strlen function 

TEST(MyTestSuite, Test1) {
  char *str = nullptr;
  EXPECT_EQ(ft_strlen(str), 0);
}

TEST(MyTestSuite, Test2) {
  char str[4] = "Cle";
  EXPECT_EQ(ft_strlen(str), 3) << "Length of strings differ";
}

// Google test main, that allows to run all tests 

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}