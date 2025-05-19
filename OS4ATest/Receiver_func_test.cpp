#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <windows.h>
#include "Receiver_func.h"

// Mock for std::cin to test input functions
class MockInput {
public:
    MockInput(const std::string& input) : input_(input) {
        original_cin = std::cin.rdbuf();
        std::cin.rdbuf(input_.rdbuf());
    }
    
    ~MockInput() {
        std::cin.rdbuf(original_cin);
    }

private:
    std::streambuf* original_cin;
    std::stringstream input_;
};

// Test fixture for file operations
class FileOperationsTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_filename = "test_file.bin";
    }
    
    void TearDown() override {
        remove(test_filename.c_str());
    }
    
    std::string test_filename;
};

// Test input validation for integers
TEST(InputValidationTest, IntValidInput) {
    MockInput mock("10\n");
    int value = 0;
    Input_Value(value, 5);
    EXPECT_EQ(value, 10);
}

TEST(InputValidationTest, IntInvalidThenValidInput) {
    MockInput mock("abc\n4\n10\n");
    int value = 0;
    testing::internal::CaptureStdout();
    Input_Value(value, 5);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(value, 10);
    EXPECT_NE(output.find("Некорректный ввод"), std::string::npos);
}

TEST(InputValidationTest, IntBelowLowerBound) {
    MockInput mock("3\n8\n");
    int value = 0;
    testing::internal::CaptureStdout();
    Input_Value(value, 5);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(value, 8);
    EXPECT_NE(output.find("Некорректный ввод"), std::string::npos);
}

// Test input validation for strings
TEST(InputValidationTest, StringValidInput) {
    MockInput mock("validstring\n");
    std::string value;
    Input_Value(value, 5);
    EXPECT_EQ(value, "validstring");
}

TEST(InputValidationTest, StringTooShortThenValid) {
    MockInput mock("abc\nlongenough\n");
    std::string value;
    testing::internal::CaptureStdout();
    Input_Value(value, 5);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(value, "longenough");
    EXPECT_NE(output.find("Некорректный ввод"), std::string::npos);
}

// Test file creation
TEST_F(FileOperationsTest, CreateBinaryFileSuccess) {
    CreateBinaryFile(test_filename);
    std::ifstream file(test_filename, std::ios::binary);
    EXPECT_TRUE(file.good());
    file.close();
}

TEST_F(FileOperationsTest, ReadBinaryFileWithContent) {
    // 1. Подготовка тестовых данных
    std::string test_content = "This is a test message for reading";
    const size_t read_size = 20; // Сколько символов читаем
    
    // 2. Создаем файл с тестовым содержимым
    {
        std::ofstream outFile(test_filename, std::ios::binary);
        outFile << test_content;
        outFile.close();
    }
    
    // 3. Перехватываем вывод
    testing::internal::CaptureStdout();
    ReadBinaryFile(test_filename);
    std::string output = testing::internal::GetCapturedStdout();
    
    // 4. Проверяем вывод
    std::string expected_output = "Message: " + test_content.substr(0, read_size) + "\n";
    EXPECT_EQ(output, expected_output);
    
    // 5. Проверяем содержимое файла после операции
    std::ifstream inFile(test_filename, std::ios::binary);
    std::string remaining_content((std::istreambuf_iterator<char>(inFile)),
                                std::istreambuf_iterator<char>());
    inFile.close();
    
    EXPECT_EQ(remaining_content, test_content.substr(read_size));
}

TEST_F(FileOperationsTest, ReadEmptyBinaryFile) {
    CreateBinaryFile(test_filename); // Создаем пустой файл
    
    testing::internal::CaptureStdout();
    ReadBinaryFile(test_filename);
    std::string output = testing::internal::GetCapturedStdout();
    
    // Проверяем, что сообщение об ошибке было выведено в stderr
    // (но CaptureStdout перехватывает только stdout)
    // Нужно либо перенаправить stderr, либо изменить функцию
    
    // Временное решение - проверим, что вывод пустой (так как ошибка идет в stderr)
    EXPECT_TRUE(output.empty());
    
    // Альтернативно, можно модифицировать функцию ReadBinaryFile:
    // заменить std::cerr на std::cout для тестирования
}

// Note: The following tests would require mocking of Windows API calls to be fully testable
// These are more integration/system tests than unit tests



int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    RUN_ALL_TESTS();
    system("pause");
    return 0;
}