#define _CRT_SECURE_NO_WARNINGS 1
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <cmath>
#include <regex>

// 函数声明
std::unordered_map<std::string, int> createWordFrequencyVector(const std::string& text); // 生成词频向量
double calculateCosineSimilarity(const std::unordered_map<std::string, int>& vector1, const std::unordered_map<std::string, int>& vector2); // 计算余弦相似度

int main() {
    // 接收用户输入的文件路径
    std::string filePath1, filePath2, filePath3;
    std::cout << "请输入第一篇论文的文件路径: ";
    std::cin >> filePath1; // 读取第一篇论文的文件路径
    std::cout << "请输入第二篇论文的文件路径: ";
    std::cin >> filePath2; // 读取第二篇论文的文件路径
    std::cout << "请输入查重结果的文件路径: ";
    std::cin >> filePath3; // 读取查重结果的文件路径

    // 打开文件并检查是否成功
    std::ifstream file1(filePath1); // 打开第一篇论文文件
    std::ifstream file2(filePath2); // 打开第二篇论文文件

    if (!file1.is_open() || !file2.is_open()) { // 检查文件是否成功打开
        std::cerr << "无法打开文件，请检查文件路径是否正确。" << std::endl;
        return 1; // 如果文件打开失败，程序退出
    }

    // 读取文件内容
    std::stringstream buffer1, buffer2; // 定义字符串流，用于存储文件内容
    buffer1 << file1.rdbuf(); // 将第一篇论文的内容读入buffer1
    buffer2 << file2.rdbuf(); // 将第二篇论文的内容读入buffer2
    std::string paper1 = buffer1.str(); // 将buffer1的内容转换为字符串
    std::string paper2 = buffer2.str(); // 将buffer2的内容转换为字符串

    // 生成词频向量
    auto vector1 = createWordFrequencyVector(paper1); // 生成第一篇论文的词频向量
    auto vector2 = createWordFrequencyVector(paper2); // 生成第二篇论文的词频向量

    // 计算余弦相似度
    double similarity = calculateCosineSimilarity(vector1, vector2); // 计算两篇论文的余弦相似度
    double threshold = 0.7; // 设置相似度阈值

    // 输出相似度
    std::cout << "相似度: " << similarity << std::endl; // 打印相似度到控制台

    // 将相似度写入输出文件
    std::ofstream outfile(filePath3); // 打开输出文件
    if (outfile.is_open()) { // 检查文件是否成功打开
        outfile << similarity << '\n'; // 将相似度写入文件
        outfile.close(); // 关闭文件
    } else {
        std::cerr << "无法打开输出文件" << std::endl; // 如果文件打开失败，输出错误信息
    }

    // 输出检测结果
    if (similarity >= threshold) { // 如果相似度大于阈值
        std::cout << "这两篇论文可能存在相似性或抄袭！" << std::endl; // 输出可能存在抄袭
    } else {
        std::cout << "这两篇论文不太可能存在相似性或抄袭。" << std::endl; // 输出不太可能存在抄袭
    }

    return 0; // 程序正常退出
}

/**
 * 创建词频向量
 * @param text 输入的文本
 * @return 词频向量（哈希表，键为单词，值为词频）
 */
std::unordered_map<std::string, int> createWordFrequencyVector(const std::string& text) {
    std::unordered_map<std::string, int> vector; // 定义词频向量（哈希表）
    std::regex pattern(R"([\w'-]+)", std::regex_constants::icase); // 定义正则表达式，匹配单词（忽略大小写）
    auto words_begin = std::sregex_iterator(text.begin(), text.end(), pattern); // 定义正则表达式迭代器，指向文本开头
    auto words_end = std::sregex_iterator(); // 定义正则表达式迭代器，指向文本结尾

    // 遍历所有匹配的单词
    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::string word = (*i).str(); // 获取当前匹配的单词
        // 将单词转换为小写
        for (char& c : word) {
            c = tolower(c); // 将每个字符转换为小写
        }
        vector[word]++; // 更新词频向量中的词频
    }

    return vector; // 返回生成的词频向量
}

/**
 * 计算余弦相似度
 * @param vector1 第一个词频向量
 * @param vector2 第二个词频向量
 * @return 余弦相似度
 */
double calculateCosineSimilarity(const std::unordered_map<std::string, int>& vector1, const std::unordered_map<std::string, int>& vector2) {
    double dotProduct = 0.0; // 点积
    double norm1 = 0.0;      // 向量1的模长平方
    double norm2 = 0.0;      // 向量2的模长平方

    // 计算点积和向量1的模长平方
    for (const auto& entry : vector1) { // 遍历向量1中的每个单词
        const std::string& word = entry.first; // 获取单词
        int freq1 = entry.second; // 获取单词在向量1中的词频
        if (vector2.count(word)) { // 如果向量2中也包含该单词
            int freq2 = vector2.at(word); // 获取单词在向量2中的词频
            dotProduct += freq1 * freq2; // 更新点积
        }
        norm1 += freq1 * freq1; // 更新向量1的模长平方
    }

    // 计算向量2的模长平方
    for (const auto& entry : vector2) { // 遍历向量2中的每个单词
        int freq2 = entry.second; // 获取单词在向量2中的词频
        norm2 += freq2 * freq2; // 更新向量2的模长平方
    }

    // 避免除以零
    if (norm1 == 0.0 || norm2 == 0.0) {
        return 0.0; // 如果任一向量模长为0，返回相似度0
    }

    // 计算余弦相似度
    return dotProduct / (std::sqrt(norm1) * std::sqrt(norm2)); // 返回余弦相似度
}
