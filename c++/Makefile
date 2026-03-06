# 编译器
CC = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -g

# 目录
INCDIR = include
SRCDIR = src
OUTDIR = output

# 目标文件
EXECUTABLE = $(OUTDIR)/LibraryManager.exe

# 自动获取源文件
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OUTDIR)/%.o)

# 头文件路径
INCLUDES = -I$(INCDIR)

# 默认目标
all: $(EXECUTABLE)

# 链接
$(EXECUTABLE): $(OBJECTS) | $(OUTDIR)
	$(CC) $^ -o $@

# 编译
$(OUTDIR)/%.o: $(SRCDIR)/%.cpp | $(OUTDIR)
	$(CC) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# 创建输出目录
$(OUTDIR):
	mkdir -p $(OUTDIR)

# 清理
clean:
	rm -f $(OUTDIR)/*.o $(EXECUTABLE)
	rmdir $(OUTDIR) 2>/dev/null || true

.PHONY: all clean