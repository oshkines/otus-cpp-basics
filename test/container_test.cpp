#include <gtest/gtest.h>
#include "listContainer.h"
//#include "../listContainer.h"

//TEST(TestCaseName, TestName) {
//  EXPECT_EQ(1, 1);
//  EXPECT_TRUE(true);
//}


// TEST(Basic, Basic) {
//     ASSERT_EQ(2+2, 4);
// }

TEST(listContainer, Empty) {

    listContainer<int> Container{ 10 };
    ASSERT_EQ(Container.getSize(), 0); //Проверить, равны ли два значения (val1 == val2).  

    ASSERT_TRUE(Container.emptry()); // Проверяет, что переданное условие является истинным.
}

// TEST(listContainer, BasicTest) {
//     listContainer<int> list(3);
//     ASSERT_EQ(0, list.getSize());

//     list.push_back(10);
//     EXPECT_EQ(1, list.getSize());
// }

TEST(listContainer, PushBack) {
    // Arrange
    listContainer<int> Container{ 10 };
    const size_t expected = 10;

    // Act (empty for this test)
    for (size_t i = 0; i < expected; ++i)
    {
        Container.push_back(i);
    }

    // Assert
    //std::cout << "Before the assert" << std::endl;
    ASSERT_EQ(Container.getSize(), expected); // This should fail
    //std::cout << "After the assert" << std::endl;
}

TEST(listContainer, RemoveTest) {
    // Arrange
    int ArrDelete[] = { 3, 5, 7 }; // В массиве указываем номер, а не индекс тех элементов которые хотим удалить
    listContainer<int> Container{ 10 };
    const size_t expected = 10;
    for (size_t i = 0; i < expected; ++i)
    {
        Container.push_back(i);
    }

    // Act (empty for this test)
    for (int j = sizeof(ArrDelete) / sizeof(ArrDelete[0]) - 1; j >= 0; j--)
    {
        int deleteIndex = ArrDelete[j] - 1;
        if (deleteIndex >= 0 && deleteIndex < Container.getSize()) {
            Container.removeAt(ArrDelete[j] - 1);
        }
        else
        {
            std::cout << "Index outside the collection" << std::endl;
        }
    }

    // Assert
    //std::cout << "Before the assert" << std::endl;
    ASSERT_EQ(Container.getSize(), expected - sizeof(ArrDelete) / sizeof(ArrDelete[0])); // This should fail
    //std::cout << "After the assert" << std::endl;
}

TEST(listContainer, RemoveTestTwo) {
    // Arrange
    listContainer<int> Container{ 10 };
    const size_t expected = 10;
    for (size_t i = 0; i < expected; ++i)
    {
        Container.push_back(i);
    }

    // Act (empty for this test)
    for (int i = expected; i > 0; --i)
    {
        Container.removeAt(i-1);
    }

    // Assert
    ASSERT_EQ(Container.getSize(), 0); // This should fail
}

TEST(listContainer, Insert) {
    // Arrange
    const size_t expected = 10;
    listContainer<int> Container{ expected };

    // Act (empty for this test)
    for (size_t i = 0; i < expected; ++i)
    {
        Container.insert(i, i);
    }

    // Assert
    //std::cout << "Before the assert" << std::endl;
    ASSERT_EQ(Container.getSize(), expected); // This should fail
    //std::cout << "After the assert" << std::endl;
}

TEST(listContainer, InsertTwo) {
    // Arrange
    const size_t expected = 10;
    listContainer<int> Container{ expected };

    // Act (empty for this test)
    for (size_t i = 0; i < expected; ++i)
    {
        Container.push_back(i);
    }

    Container.insert(34, 0);

    int place = Container.getSize() / 2;
    Container.insert(16, place);

    Container.push_back(25);

    // Assert
    //std::cout << "Before the assert" << std::endl;
    ASSERT_EQ(Container.getSize(), expected + 3); // This should fail
    //std::cout << "After the assert" << std::endl;
}
 TEST(listContainer, ClearContainer){
     // Arrange
     const size_t expected = 10;
     listContainer<int> Container{ expected };
     for (size_t i = 0; i < expected; ++i)
     {
         Container.push_back(i);
     }

     Container.clear();

     // Assert
     //std::cout << "Before the assert" << std::endl;
     ASSERT_EQ(Container.getSize(), 0);   

     ASSERT_TRUE(Container.emptry()); 
     //std::cout << "After the assert" << std::endl;
 }     

 // TEST(listContainer, FailedAssert){
 //    // Arrange
 //    listContainer<int> Container{10};

 //    for (size_t i = 0; i < 10; i++)
 //    {
 //        Container.push_back(i);
 //    }

 //    const size_t expected = Container.getSize();

 //    // Act (empty for this test)
 //        Container.insert(34, 0);

 //    // Assert
 //    std::cout << "Before the assert" << std::endl;
 //    ASSERT_EQ(Container.getSize(), expected); // This should fail
 //    std::cout << "After the assert" << std::endl;
 //}        

 //TEST(listContainer, FailedAssert){
 //    // Arrange
 //    listContainer<int> Container{10};

 //    const size_t expected = Container.getSize();

 //    int middle = expected / 2;

 //    // Act (empty for this test)
 //        Container.insert(16, middle);

 //    // Assert
 //    std::cout << "Before the assert" << std::endl;
 //    ASSERT_EQ(Container.getSize(), expected); // This should fail
 //    std::cout << "After the assert" << std::endl;
 //}     

 //TEST(listContainer, FailedAssert){
 //    // Arrange
 //    listContainer<int> Container{10};
 //    const size_t expected = Container.getSize();

 //    int ending = expected - 1;

 //    // Act (empty for this test)
 //        Container.insert(25, ending);

 //    // Assert
 //    std::cout << "Before the assert" << std::endl;
 //    ASSERT_EQ(Container.getSize(), expected); // This should fail
 //    std::cout << "After the assert" << std::endl;
 //} 

 TEST(listContainer, RemoveOne){
     // Arrange
     const size_t expected = 10;
     listContainer<int> Container{ expected };
     for (size_t i = 0; i < expected; ++i)
     {
         Container.push_back(i);
     }

     // Act (empty for this test)
     Container.removeAt(0);

     // Assert
     //std::cout << "Before the assert" << std::endl;
     ASSERT_EQ(Container.getSize(), expected - 1); // This should fail
     //std::cout << "After the assert" << std::endl;
 } 


 TEST(listContainer, RemoveMiddle){
     // Arrange
     const size_t expected = 10;
     listContainer<int> Container{ expected };
     for (size_t i = 0; i < expected; ++i)
     {
         Container.push_back(i);
     }

     int middle = expected / 2;

     // Act (empty for this test)
         Container.removeAt(middle);

     // Assert
     //std::cout << "Before the assert" << std::endl;
     ASSERT_EQ(Container.getSize(), expected - 1); // This should fail
     //std::cout << "After the assert" << std::endl;
 } 

 TEST(listContainer, RemoveEnding){
     // Arrange
     const size_t expected = 10;
     listContainer<int> Container{ expected };
     for (size_t i = 0; i < expected; ++i)
     {
         Container.push_back(i);
     }

     int ending = expected - 1;

     // Act (empty for this test)
         Container.removeAt(ending);

     // Assert
     //std::cout << "Before the assert" << std::endl;
     ASSERT_EQ(Container.getSize(), expected - 1); // This should fail
     //std::cout << "After the assert" << std::endl;
 } 

 TEST(listContainer, Operator_KvSk){
     // Arrange
     const int expected = 10;
     listContainer<int> Container{ expected };
     for (int i = 0; i < expected; ++i)
     {
         Container.push_back(i);
     }

 	for (int i = 0; i < expected; ++i)
 	{
 		std::cout << Container[i];// << ", ";
 		if (i+1 != expected) {
 			std::cout << ", ";
 		}
 	}
    std::cout << std::endl;
     // Assert
     //std::cout << "Before the assert" << std::endl;
     ASSERT_EQ(Container.getSize(), expected); // This should fail
     //std::cout << "After the assert" << std::endl;
 }     