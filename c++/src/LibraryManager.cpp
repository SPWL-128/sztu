#include "LibraryManager.h"
#include <iostream>
#include <limits>
#include <cctype>

LibraryManager::LibraryManager() {
    // 可以添加一些初始测试数据（可选）
    // 例如：
    // users.push_back(std::make_shared<Student>("S001", "张三"));
    // users.push_back(std::make_shared<Teacher>("T001", "李老师"));
    // users.push_back(std::make_shared<Admin>("A001", "管理员"));
    // books.emplace_back("B001", "C++ Primer", 3);
    // books.emplace_back("B002", "Effective Modern C++", 2);
}

void LibraryManager::run() {
    while (true) {
        std::cout << "\n========== 图书管理系统 ==========" << std::endl;
        std::cout << "1. 管理 用户 / 图书" << std::endl;
        std::cout << "2. 作为用户登录" << std::endl;
        std::cout << "0. 退出系统" << std::endl;
        std::cout << "请选择: ";

        int choice;
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "输入错误，请重新输入！" << std::endl;
            continue;
        }

        switch (choice) {
            case 1:
                manageUsersAndBooks();
                break;
            case 2:
                if (login()) {
                    userMenu();
                }
                break;
            case 0:
                std::cout << "感谢使用，再见！" << std::endl;
                return;
            default:
                std::cout << "无效选项，请重新选择！" << std::endl;
        }
    }
}

// ---------- 用户/图书管理 ----------
void LibraryManager::manageUsersAndBooks() {
    while (true) {
        std::cout << "\n--- 用户/图书管理 ---" << std::endl;
        std::cout << "1. 添加" << std::endl;
        std::cout << "2. 删除" << std::endl;
        std::cout << "0. 返回上一级" << std::endl;
        std::cout << "请选择: ";

        int choice;
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "输入错误，请重新输入！" << std::endl;
            continue;
        }

        switch (choice) {
            case 1:
                addMenu();
                break;
            case 2:
                removeMenu();
                break;
            case 0:
                return;
            default:
                std::cout << "无效选项！" << std::endl;
        }
    }
}

void LibraryManager::addMenu() {
    while (true) {
        std::cout << "\n--- 添加 ---" << std::endl;
        std::cout << "1. 添加学生" << std::endl;
        std::cout << "2. 添加教师" << std::endl;
        std::cout << "3. 添加管理员" << std::endl;
        std::cout << "4. 添加图书" << std::endl;
        std::cout << "0. 返回" << std::endl;
        std::cout << "请选择: ";

        int choice;
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "输入错误，请重新输入！" << std::endl;
            continue;
        }

        switch (choice) {
            case 1: addStudent(); break;
            case 2: addTeacher(); break;
            case 3: addAdmin(); break;
            case 4: addBook(); break;
            case 0: return;
            default: std::cout << "无效选项！" << std::endl;
        }
    }
}

void LibraryManager::addStudent() {
    std::string id, name;
    std::cout << "请输入学生编号: ";
    std::cin >> id;
    std::cout << "请输入学生姓名: ";
    std::cin >> name;

    // 检查编号是否已存在
    if (findUserById(id) != nullptr) {
        std::cout << "编号已存在，添加失败！" << std::endl;
        return;
    }
    users.push_back(std::make_shared<Student>(id, name));
    std::cout << "学生添加成功！" << std::endl;
}

void LibraryManager::addTeacher() {
    std::string id, name;
    std::cout << "请输入教师编号: ";
    std::cin >> id;
    std::cout << "请输入教师姓名: ";
    std::cin >> name;

    if (findUserById(id) != nullptr) {
        std::cout << "编号已存在，添加失败！" << std::endl;
        return;
    }
    users.push_back(std::make_shared<Teacher>(id, name));
    std::cout << "教师添加成功！" << std::endl;
}

void LibraryManager::addAdmin() {
    std::string id, name;
    std::cout << "请输入管理员编号: ";
    std::cin >> id;
    std::cout << "请输入管理员姓名: ";
    std::cin >> name;

    if (findUserById(id) != nullptr) {
        std::cout << "编号已存在，添加失败！" << std::endl;
        return;
    }
    users.push_back(std::make_shared<Admin>(id, name));
    std::cout << "管理员添加成功！" << std::endl;
}

void LibraryManager::addBook() {
    std::string id, title;
    int qty;
    std::cout << "请输入图书编号: ";
    std::cin >> id;
    std::cout << "请输入图书名称: ";
    std::cin.ignore();
    std::getline(std::cin, title);
    std::cout << "请输入图书数量: ";
    std::cin >> qty;

    // 检查编号是否已存在
    for (const auto& b : books) {
        if (b.id == id) {
            std::cout << "图书编号已存在，添加失败！" << std::endl;
            return;
        }
    }
    books.emplace_back(id, title, qty);
    std::cout << "图书添加成功！" << std::endl;
}

void LibraryManager::removeMenu() {
    while (true) {
        std::cout << "\n--- 删除 ---" << std::endl;
        std::cout << "1. 删除学生" << std::endl;
        std::cout << "2. 删除教师" << std::endl;
        std::cout << "3. 删除管理员" << std::endl;
        std::cout << "4. 删除图书" << std::endl;
        std::cout << "0. 返回" << std::endl;
        std::cout << "请选择: ";

        int choice;
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "输入错误，请重新输入！" << std::endl;
            continue;
        }

        switch (choice) {
            case 1: removeStudent(); break;
            case 2: removeTeacher(); break;
            case 3: removeAdmin(); break;
            case 4: removeBook(); break;
            case 0: return;
            default: std::cout << "无效选项！" << std::endl;
        }
    }
}

void LibraryManager::removeStudent() {
    std::string id, name;
    std::cout << "请输入要删除的学生编号: ";
    std::cin >> id;
    std::cout << "请输入要删除的学生姓名: ";
    std::cin >> name;

    auto it = std::find_if(users.begin(), users.end(),
        [&](const std::shared_ptr<AllUser>& u) {
            return u->id == id && u->name == name && u->getType() == "学生";
        });
    if (it == users.end()) {
        std::cout << "未找到匹配的学生！" << std::endl;
        return;
    }

    std::cout << "确认删除该学生？(y/n): ";
    char confirm;
    std::cin >> confirm;
    if (confirm == 'y' || confirm == 'Y') {
        users.erase(it);
        std::cout << "学生已删除。" << std::endl;
    } else {
        std::cout << "取消删除。" << std::endl;
    }
}

void LibraryManager::removeTeacher() {
    std::string id, name;
    std::cout << "请输入要删除的教师编号: ";
    std::cin >> id;
    std::cout << "请输入要删除的教师姓名: ";
    std::cin >> name;

    auto it = std::find_if(users.begin(), users.end(),
        [&](const std::shared_ptr<AllUser>& u) {
            return u->id == id && u->name == name && u->getType() == "教师";
        });
    if (it == users.end()) {
        std::cout << "未找到匹配的教师！" << std::endl;
        return;
    }

    std::cout << "确认删除该教师？(y/n): ";
    char confirm;
    std::cin >> confirm;
    if (confirm == 'y' || confirm == 'Y') {
        users.erase(it);
        std::cout << "教师已删除。" << std::endl;
    } else {
        std::cout << "取消删除。" << std::endl;
    }
}

void LibraryManager::removeAdmin() {
    std::string id, name;
    std::cout << "请输入要删除的管理员编号: ";
    std::cin >> id;
    std::cout << "请输入要删除的管理员姓名: ";
    std::cin >> name;

    auto it = std::find_if(users.begin(), users.end(),
        [&](const std::shared_ptr<AllUser>& u) {
            return u->id == id && u->name == name && u->getType() == "管理员";
        });
    if (it == users.end()) {
        std::cout << "未找到匹配的管理员！" << std::endl;
        return;
    }

    std::cout << "确认删除该管理员？(y/n): ";
    char confirm;
    std::cin >> confirm;
    if (confirm == 'y' || confirm == 'Y') {
        users.erase(it);
        std::cout << "管理员已删除。" << std::endl;
    } else {
        std::cout << "取消删除。" << std::endl;
    }
}

void LibraryManager::removeBook() {
    std::string id, title;
    std::cout << "请输入要删除的图书编号: ";
    std::cin >> id;
    std::cout << "请输入要删除的图书书名: ";
    std::cin.ignore();
    std::getline(std::cin, title);

    auto it = std::find_if(books.begin(), books.end(),
        [&](const Book& b) { return b.id == id && b.title == title; });
    if (it == books.end()) {
        std::cout << "未找到匹配的图书！" << std::endl;
        return;
    }

    // 检查是否有未归还的借阅
    if (!it->borrowers.empty()) {
        std::cout << "该书尚有未归还的借阅，无法删除！" << std::endl;
        return;
    }

    std::cout << "确认删除该图书？(y/n): ";
    char confirm;
    std::cin >> confirm;
    if (confirm == 'y' || confirm == 'Y') {
        books.erase(it);
        std::cout << "图书已删除。" << std::endl;
    } else {
        std::cout << "取消删除。" << std::endl;
    }
}

// ---------- 登录验证 ----------
bool LibraryManager::login() {
    std::string id, name;
    std::cout << "请输入用户编号: ";
    std::cin >> id;
    std::cout << "请输入用户名: ";
    std::cin >> name;

    auto* user = findUserById(id);
    if (user && user->name == name) {
        // 找到匹配的用户，设置当前用户（需要从users中获取shared_ptr）
        for (auto& u : users) {
            if (u->id == id) {
                currentUser = u;
                break;
            }
        }
        std::cout << "登录成功！欢迎 " << name << " (" << currentUser->getType() << ")" << std::endl;
        return true;
    } else {
        std::cout << "登录失败：用户编号或用户名错误！" << std::endl;
        return false;
    }
}

// ---------- 用户菜单 ----------
void LibraryManager::userMenu() {
    while (true) {
        std::cout << "\n--- 用户操作菜单 ---" << std::endl;
        std::cout << "1. 显示我的信息" << std::endl;
        std::cout << "2. 查看图书" << std::endl;
        std::cout << "3. 借阅图书" << std::endl;
        std::cout << "4. 归还图书" << std::endl;
        // 管理员额外选项
        if (currentUser->getType() == "管理员") {
            std::cout << "5. 列举所有图书" << std::endl;
            std::cout << "0. 退出登录" << std::endl;
        } else {
            std::cout << "0. 退出登录" << std::endl;
        }
        std::cout << "请选择: ";

        int choice;
        std::cin >> choice;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "输入错误，请重新输入！" << std::endl;
            continue;
        }

        if (choice == 0) {
            currentUser.reset();
            std::cout << "已退出登录。" << std::endl;
            break;
        }

        switch (choice) {
            case 1:
                displayCurrentUserInfo();
                break;
            case 2:
                currentUser->viewBooks(*this);
                break;
            case 3:
                currentUser->borrowBook(*this);
                break;
            case 4:
                currentUser->returnBook(*this);
                break;
            case 5:
                if (currentUser->getType() == "管理员") {
                    listAllBooks();
                } else {
                    std::cout << "无效选项！" << std::endl;
                }
                break;
            default:
                std::cout << "无效选项！" << std::endl;
        }
    }
}

// ---------- 辅助函数 ----------
AllUser* LibraryManager::findUserById(const std::string& id) {
    for (auto& u : users) {
        if (u->id == id) return u.get();
    }
    return nullptr;
}

Book* LibraryManager::findBook(const std::string& input) {
    // 先按编号精确匹配
    for (auto& b : books) {
        if (b.id == input) return &b;
    }
    // 再按书名模糊匹配（这里简单比较，可以改为包含关系）
    for (auto& b : books) {
        if (b.title == input) return &b;
    }
    return nullptr;
}

std::string LibraryManager::getUserNameById(const std::string& id) {
    auto* u = findUserById(id);
    return u ? u->name : "";
}

bool LibraryManager::borrowFor(const std::string& userId, const std::string& bookInput) {
    auto* user = findUserById(userId);
    if (!user) return false;
    auto* book = findBook(bookInput);
    if (!book) return false;
    return book->lendTo(userId, user->name);
}

bool LibraryManager::returnFor(const std::string& userId, const std::string& bookInput) {
    auto* book = findBook(bookInput);
    if (!book) return false;
    return book->returnFrom(userId);
}

void LibraryManager::displayCurrentUserInfo() {
    std::cout << "\n用户编号: " << currentUser->id << std::endl;
    std::cout << "用户名: " << currentUser->name << std::endl;
    std::cout << "用户权限: " << currentUser->getType() << std::endl;

    // 查找该用户借阅的书籍
    std::cout << "借阅的图书: ";
    bool hasBorrowed = false;
    for (const auto& book : books) {
        for (const auto& b : book.borrowers) {
            if (b.userId == currentUser->id) {
                std::cout << "\n  编号: " << book.id << " 书名: " << book.title;
                hasBorrowed = true;
            }
        }
    }
    if (!hasBorrowed) {
        std::cout << "无" << std::endl;
    } else {
        std::cout << std::endl;
    }
}

void LibraryManager::listAllBooks() {
    // 按图书编号排序
    std::vector<Book> sorted = books;
    std::sort(sorted.begin(), sorted.end(),
        [](const Book& a, const Book& b) { return a.id < b.id; });

    std::cout << "\n所有图书信息（按编号排序）:" << std::endl;
    for (const auto& book : sorted) {
        book.display();
    }
}