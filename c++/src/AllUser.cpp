#include "AllUser.h"
#include "LibraryManager.h"
#include <iostream>
#include <limits>

// ---------- AllUser 基类 ----------
AllUser::AllUser(const std::string& id, const std::string& name)
    : id(id), name(name) {}

// ---------- Student ----------
Student::Student(const std::string& id, const std::string& name)
    : AllUser(id, name) {}

std::string Student::getType() const {
    return "学生";
}

void Student::viewBooks(LibraryManager& lm) {
    // 学生仅查看图书基本信息
    std::cout << "请输入图书编号或书名: ";
    std::string input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, input);

    auto* book = lm.findBook(input);
    if (book) {
        book->display();
    } else {
        std::cout << "没有此书！" << std::endl;
    }
}

void Student::borrowBook(LibraryManager& lm) {
    std::cout << "请输入要借阅的图书编号或书名: ";
    std::string input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, input);

    if (lm.borrowFor(this->id, input)) {
        std::cout << "借阅成功！" << std::endl;
    } else {
        std::cout << "借阅失败（可能图书不存在或库存不足）" << std::endl;
    }
}

void Student::returnBook(LibraryManager& lm) {
    std::cout << "请输入要归还的图书编号或书名: ";
    std::string input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, input);

    if (lm.returnFor(this->id, input)) {
        std::cout << "归还成功！" << std::endl;
    } else {
        std::cout << "归还失败（可能您未借阅此书或输入有误）" << std::endl;
    }
}

// ---------- Teacher ----------
Teacher::Teacher(const std::string& id, const std::string& name)
    : AllUser(id, name) {}

std::string Teacher::getType() const {
    return "教师";
}

void Teacher::viewBooks(LibraryManager& lm) {
    // 教师同学生，只查看基本信息（需求未说明教师有额外信息）
    std::cout << "请输入图书编号或书名: ";
    std::string input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, input);

    auto* book = lm.findBook(input);
    if (book) {
        book->display();
    } else {
        std::cout << "没有此书！" << std::endl;
    }
}

void Teacher::borrowBook(LibraryManager& lm) {
    std::cout << "请选择借书对象: 1.为自己借书  2.为学生借书" << std::endl;
    int choice;
    std::cin >> choice;
    if (choice == 1) {
        std::cout << "请输入要借阅的图书编号或书名: ";
        std::string input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, input);
        if (lm.borrowFor(this->id, input)) {
            std::cout << "借阅成功！" << std::endl;
        } else {
            std::cout << "借阅失败" << std::endl;
        }
    } else if (choice == 2) {
        std::cout << "请输入学生编号: ";
        std::string studentId;
        std::cin >> studentId;
        // 验证该用户是否存在且为学生
        auto* user = lm.findUserById(studentId);
        if (!user || user->getType() != "学生") {
            std::cout << "学生编号无效！" << std::endl;
            return;
        }
        std::cout << "请输入要借阅的图书编号或书名: ";
        std::string input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, input);
        if (lm.borrowFor(studentId, input)) {
            std::cout << "为学生借书成功！" << std::endl;
        } else {
            std::cout << "借阅失败" << std::endl;
        }
    } else {
        std::cout << "无效选择" << std::endl;
    }
}

void Teacher::returnBook(LibraryManager& lm) {
    std::cout << "请选择还书对象: 1.为自己还书  2.为学生还书" << std::endl;
    int choice;
    std::cin >> choice;
    if (choice == 1) {
        std::cout << "请输入要归还的图书编号或书名: ";
        std::string input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, input);
        if (lm.returnFor(this->id, input)) {
            std::cout << "归还成功！" << std::endl;
        } else {
            std::cout << "归还失败" << std::endl;
        }
    } else if (choice == 2) {
        std::cout << "请输入学生编号: ";
        std::string studentId;
        std::cin >> studentId;
        auto* user = lm.findUserById(studentId);
        if (!user || user->getType() != "学生") {
            std::cout << "学生编号无效！" << std::endl;
            return;
        }
        std::cout << "请输入要归还的图书编号或书名: ";
        std::string input;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::getline(std::cin, input);
        if (lm.returnFor(studentId, input)) {
            std::cout << "为学生还书成功！" << std::endl;
        } else {
            std::cout << "归还失败" << std::endl;
        }
    } else {
        std::cout << "无效选择" << std::endl;
    }
}

// ---------- Admin ----------
Admin::Admin(const std::string& id, const std::string& name)
    : AllUser(id, name) {}

std::string Admin::getType() const {
    return "管理员";
}

void Admin::viewBooks(LibraryManager& lm) {
    std::cout << "请输入图书编号或书名: ";
    std::string input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, input);

    auto* book = lm.findBook(input);
    if (book) {
        // 管理员显示图书信息时附带借阅者信息
        book->displayWithBorrowers();
    } else {
        std::cout << "没有此书！" << std::endl;
    }
}

void Admin::borrowBook(LibraryManager& lm) {
    std::cout << "请输入用户编号: ";
    std::string userId;
    std::cin >> userId;
    auto* user = lm.findUserById(userId);
    if (!user) {
        std::cout << "用户不存在！" << std::endl;
        return;
    }
    std::cout << "请输入要借阅的图书编号或书名: ";
    std::string input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, input);
    if (lm.borrowFor(userId, input)) {
        std::cout << "借阅成功！" << std::endl;
    } else {
        std::cout << "借阅失败" << std::endl;
    }
}

void Admin::returnBook(LibraryManager& lm) {
    std::cout << "请输入用户编号: ";
    std::string userId;
    std::cin >> userId;
    auto* user = lm.findUserById(userId);
    if (!user) {
        std::cout << "用户不存在！" << std::endl;
        return;
    }
    std::cout << "请输入要归还的图书编号或书名: ";
    std::string input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, input);
    if (lm.returnFor(userId, input)) {
        std::cout << "归还成功！" << std::endl;
    } else {
        std::cout << "归还失败" << std::endl;
    }
}