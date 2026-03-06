#pragma once
#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <algorithm>
#include "AllUser.h"
#include "Book.h"

class LibraryManager {
private:
    std::vector<std::shared_ptr<AllUser>> users;   // 所有用户
    std::vector<Book> books;                        // 所有图书
    std::shared_ptr<AllUser> currentUser;           // 当前登录用户

public:
    LibraryManager();
    ~LibraryManager() = default;

    // 主循环
    void run();

    // ---------- 以下函数需要被用户类调用，故设为 public ----------
    // 根据编号查找用户（返回原始指针，不改变所有权）
    AllUser* findUserById(const std::string& id);

    // 根据输入（编号或书名）查找图书（返回指针）
    Book* findBook(const std::string& input);

    // 为目标用户借书（由子类调用）
    bool borrowFor(const std::string& userId, const std::string& bookInput);

    // 为目标用户还书
    bool returnFor(const std::string& userId, const std::string& bookInput);

private:
    // 根据用户编号获取用户名
    std::string getUserNameById(const std::string& id);

    // 一级菜单：用户/图书管理
    void manageUsersAndBooks();

    // 二级菜单：添加
    void addMenu();
    void addStudent();
    void addTeacher();
    void addAdmin();
    void addBook();

    // 二级菜单：删除
    void removeMenu();
    void removeStudent();
    void removeTeacher();
    void removeAdmin();
    void removeBook();

    // 登录验证
    bool login();

    // 登录后的操作菜单（根据用户类型显示不同选项）
    void userMenu();

    // 通用功能：显示当前用户信息（包括借阅书籍）
    void displayCurrentUserInfo();

    // 管理员专用：列举所有图书（排序后输出）
    void listAllBooks();
};