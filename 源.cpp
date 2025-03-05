#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <cmath>
#include <regex>

// 函数声明
std::unordered_map<std::string, int> createWordFrequencyVector(const std::string& text);
double calculateCosineSimilarity(const std::unordered_map<std::string, int>& vector1, const std::unordered_map<std::string, int>& vector2);

int main() {
    // 接收用户输入的文件路径
    std::string filePath1, filePath2;
    std::cout << "请输入第一篇论文的文件路径: ";
    std::cin >> filePath1;
    std::cout << "请输入第二篇论文的文件路径: ";
    std::cin >> filePath2;

    // 打开文件并检查是否成功
    std::ifstream file1(filePath1);
    std::ifstream file2(filePath2);

    if (!file1.is_open() || !file2.is_open()) {
        std::cerr << "无法打开文件，请检查文件路径是否正确。" << std::endl;
        return 1;
    }

    // 读取文件内容
    std::stringstream buffer1, buffer2;
    buffer1 << file1.rdbuf();
    buffer2 << file2.rdbuf();
    std::string paper1 = buffer1.str();
    std::string paper2 = buffer2.str();

    // 生成词频向量
    auto vector1 = createWordFrequencyVector(paper1);
    auto vector2 = createWordFrequencyVector(paper2);

    // 计算余弦相似度
    double similarity = calculateCosineSimilarity(vector1, vector2);

    // 输出相似度
    std::cout << "相似度: " << similarity << std::endl;

    return 0;
}

// 创建词频向量
std::unordered_map<std::string, int> createWordFrequencyVector(const std::string& text) {
    std::unordered_map<std::string, int> vector;
    std::regex pattern(R"([\w'-]+)", std::regex_constants::icase);
    auto words_begin = std::sregex_iterator(text.begin(), text.end(), pattern);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::string word = (*i).str();
        for (char& c : word) {
            c = tolower(c);
        }
        vector[word]++;
    }

    return vector;
}

// 计算余弦相似度
double calculateCosineSimilarity(const std::unordered_map<std::string, int>& vector1, const std::unordered_map<std::string, int>& vector2) {
    double dotProduct = 0.0;
    double norm1 = 0.0;
    double norm2 = 0.0;

    for (const auto& entry : vector1) {
        const std::string& word = entry.first;
        int freq1 = entry.second;
        if (vector2.count(word)) {
            int freq2 = vector2.at(word);
            dotProduct += freq1 * freq2;
        }
        norm1 += freq1 * freq1;
    }

    for (const auto& entry : vector2) {
        int freq2 = entry.second;
        norm2 += freq2 * freq2;
    }

    if (norm1 == 0.0 || norm2 == 0.0) {
        return 0.0;
    }

    return dotProduct / (std::sqrt(norm1) * std::sqrt(norm2));
}
