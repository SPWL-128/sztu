#include "Book.h"
#include <algorithm>

Book::Book(const std::string& id, const std::string& title, int qty)
    : id(id), title(title), quantity(qty) {}

bool Book::lendTo(const std::string& userId, const std::string& userName) {
    if (quantity <= 0) return false;
    // 检查是否已经借阅（防止重复借阅同一本书）
    auto it = std::find_if(borrowers.begin(), borrowers.end(),
        [&](const Borrower& b) { return b.userId == userId; });
    if (it != borrowers.end()) return false; // 已借阅

    borrowers.push_back({userId, userName});
    quantity--;
    return true;
}

bool Book::returnFrom(const std::string& userId) {
    auto it = std::find_if(borrowers.begin(), borrowers.end(),
        [&](const Borrower& b) { return b.userId == userId; });
    if (it == borrowers.end()) return false;

    borrowers.erase(it);
    quantity++;
    return true;
}

void Book::display() const {
    std::cout << "图书编号: " << id << "  书名: " << title << "  库存: " << quantity << std::endl;
}

void Book::displayWithBorrowers() const {
    display();
    if (borrowers.empty()) {
        std::cout << "当前无人借阅此书" << std::endl;
    } else {
        std::cout << "正在借阅此书的用户: " << std::endl;
        for (const auto& b : borrowers) {
            std::cout << "  用户编号: " << b.userId << "  用户名: " << b.userName << std::endl;
        }
    }
}