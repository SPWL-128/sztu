#pragma once
#include <iostream>
#include <string>
#include <vector>

// 图书类
class Book {
public:
    std::string id;          // 图书编号
    std::string title;       // 书名
    int quantity;            // 库存数量

    // 借阅者信息（用户编号, 用户名）
    struct Borrower {
        std::string userId;
        std::string userName;
    };
    std::vector<Borrower> borrowers;   // 当前借阅此书的用户列表

    Book(const std::string& id, const std::string& title, int qty);

    // 借出给指定用户
    bool lendTo(const std::string& userId, const std::string& userName);

    // 从指定用户归还
    bool returnFrom(const std::string& userId);

    // 显示图书信息（不包含借阅者）
    void display() const;

    // 显示图书信息（包含借阅者，用于管理员）
    void displayWithBorrowers() const;
};