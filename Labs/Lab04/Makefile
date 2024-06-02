# 编译器
CXX = g++-11
# 编译器标志
CXXFLAGS = -g -Wall -std=c++11 -I include
# 链接标志
LDFLAGS = -lgtest -lpthread -lrt
# 目标可执行文件
TARGET = bin/Scaner
# 源文件目录
SRCDIR = src
# 目标文件目录
OBJDIR = obj
# 源文件
SRCS = $(SRCDIR)/Scaner.cpp $(SRCDIR)/ping.cpp $(SRCDIR)/TCPConnectScan.cpp \
	   $(SRCDIR)/TCPFinScan.cpp $(SRCDIR)/TCPSynScan.cpp $(SRCDIR)/UDPScan.cpp \
	   $(SRCDIR)/utils.cpp
# 目标文件
OBJS = $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRCS))

# 默认目标
all: $(TARGET)

# 链接目标可执行文件
$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# 编译源文件为目标文件
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

# 清理生成的文件
clean:
	rm -f $(TARGET) $(OBJS)