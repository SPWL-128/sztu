#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <vector>

// 前向声明，避免循环包含
class LibraryManager;

// 抽象基类：所有用户的公共接口
class AllUser {
public:
    std::string id;     // 用户编号
    std::string name;   // 用户名


    AllUser(const std::string& id, const std::string& name);
    virtual ~AllUser() = default;

    // 获取用户类型（用于显示）
    virtual std::string getType() const = 0;

    // 查看图书（每个用户有不同的实现）
    virtual void viewBooks(LibraryManager& lm) = 0;

    // 借阅图书
    virtual void borrowBook(LibraryManager& lm) = 0;

    // 归还图书
    virtual void returnBook(LibraryManager& lm) = 0;
};

// 学生类
class Student : public AllUser {
public:
    Student(const std::string& id, const std::string& name);
    std::string getType() const override;
    void viewBooks(LibraryManager& lm) override;
    void borrowBook(LibraryManager& lm) override;
    void returnBook(LibraryManager& lm) override;
};

// 教师类
class Teacher : public AllUser {
public:
    Teacher(const std::string& id, const std::string& name);
    std::string getType() const override;
    void viewBooks(LibraryManager& lm) override;
    void borrowBook(LibraryManager& lm) override;
    void returnBook(LibraryManager& lm) override;
};

// 管理员类
class Admin : public AllUser {
public:
    Admin(const std::string& id, const std::string& name);
    std::string getType() const override;
    void viewBooks(LibraryManager& lm) override;
    void borrowBook(LibraryManager& lm) override;
    void returnBook(LibraryManager& lm) override;
};