#include <iostream>
#include <stack>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>
#include <map>
#include <stdexcept>

class RPNCalculator {
private:
    std::stack<double> stack;
    std::vector<std::string> history;

    // 斐波那契数列计算
    double fibonacci(int n) {
        if (n <= 0) return 0;
        if (n == 1) return 1;
        double a = 0, b = 1;
        for (int i = 2; i <= n; i++) {
            double temp = a + b;
            a = b;
            b = temp;
        }
        return b;
    }

    // 杨辉三角计算
    double pascalTriangle(int row, int col) {
        if (col < 0 || col > row) return 0;
        if (col == 0 || col == row) return 1;
        
        double result = 1;
        for (int i = 1; i <= col; i++) {
            result = result * (row - i + 1) / i;
        }
        return result;
    }

public:
    void push(double value) {
        stack.push(value);
    }

    double pop() {
        if (stack.empty()) {
            throw std::runtime_error("错误: 栈为空");
        }
        double value = stack.top();
        stack.pop();
        return value;
    }

    void calculate(const std::string& operation) {
        // 记录操作历史
        history.push_back(operation);
        
        if (operation == "+") {
            if (stack.size() < 2) throw std::runtime_error("错误: 栈空，需要两个操作数");
            double b = pop();
            double a = pop();
            push(a + b);
        } else if (operation == "-") {
            if (stack.size() < 2) throw std::runtime_error("错误: 栈空，需要两个操作数");
            double b = pop();
            double a = pop();
            push(a - b);
        } else if (operation == "*") {
            if (stack.size() < 2) throw std::runtime_error("错误: 栈空，需要两个操作数");
            double b = pop();
            double a = pop();
            push(a * b);
        } else if (operation == "/") {
            if (stack.size() < 2) throw std::runtime_error("错误: 栈空，需要两个操作数");
            double b = pop();
            if (b == 0) throw std::runtime_error("错误: 除零错误");
            double a = pop();
            push(a / b);
        } else if (operation == "sqrt") {
            if (stack.size() < 1) throw std::runtime_error("错误: 栈空，需要一个操作数");
            double a = pop();
            if (a < 0) throw std::runtime_error("错误: 负数不能开平方根");
            push(std::sqrt(a));
        } else if (operation == "pow") {
            if (stack.size() < 2) throw std::runtime_error("错误: 栈空，需要两个操作数");
            double exponent = pop();
            double base = pop();
            push(std::pow(base, exponent));
        } else if (operation == "sin") {
            if (stack.size() < 1) throw std::runtime_error("错误: 栈空，需要一个操作数");
            double a = pop();
            push(std::sin(a * M_PI / 180.0)); // 转换为弧度
        } else if (operation == "cos") {
            if (stack.size() < 1) throw std::runtime_error("错误: 栈空，需要一个操作数");
            double a = pop();
            push(std::cos(a * M_PI / 180.0));
        } else if (operation == "fib") {
            if (stack.size() < 1) throw std::runtime_error("错误: 栈空，需要一个操作数");
            int n = static_cast<int>(pop());
            if (n < 0) throw std::runtime_error("错误: 斐波那契数列索引不能为负数");
            push(fibonacci(n));
        } else if (operation == "pascal") {
            if (stack.size() < 2) throw std::runtime_error("错误: 栈空，需要两个操作数");
            int col = static_cast<int>(pop());
            int row = static_cast<int>(pop());
            if (row < 0 || col < 0) throw std::runtime_error("错误: 杨辉三角行列索引不能为负数");
            push(pascalTriangle(row, col));
        } else {
            // 尝试解析为数字
            try {
                double value = std::stod(operation);
                push(value);
            } catch (...) {
                throw std::runtime_error("错误: 未知操作符 '" + operation + "'");
            }
        }
    }

    void clear() {
        while (!stack.empty()) stack.pop();
        history.clear();
    }

    void displayStack() {
        std::stack<double> temp = stack;
        std::vector<double> elements;
        
        while (!temp.empty()) {
            elements.push_back(temp.top());
            temp.pop();
        }
        
        std::cout << "当前栈: ";
        for (auto it = elements.rbegin(); it != elements.rend(); ++it) {
            std::cout << *it << " ";
        }
        std::cout << std::endl;
    }

    void displayHistory() {
        std::cout << "计算历史: ";
        for (const auto& op : history) {
            std::cout << op << " ";
        }
        std::cout << std::endl;
    }

    double getResult() {
        if (stack.empty()) {
            throw std::runtime_error("错误: 栈为空");
        }
        return stack.top();
    }

    bool processExpression(const std::string& expression) {
        std::istringstream iss(expression);
        std::string token;
        
        try {
            while (iss >> token) {
                calculate(token);
            }
            return true;
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
            return false;
        }
    }
};

void showHelp() {
    std::cout << "\n=== RPN 计算器帮助 ===" << std::endl;
    std::cout << "基础运算: + - * /" << std::endl;
    std::cout << "数学函数: sqrt pow sin cos" << std::endl;
    std::cout << "高级功能: fib (斐波那契) pascal (杨辉三角)" << std::endl;
    std::cout << "栈操作: clear stack history help quit" << std::endl;
    std::cout << "示例:" << std::endl;
    std::cout << "  5 5 +           -> 10" << std::endl;
    std::cout << "  9 sqrt          -> 3" << std::endl;
    std::cout << "  2 3 pow         -> 8" << std::endl;
    std::cout << "  5 fib           -> 5 (斐波那契第5项)" << std::endl;
    std::cout << "  4 2 pascal      -> 6 (杨辉三角第4行第2列)" << std::endl;
    std::cout << "====================\n" << std::endl;
}

int main() {
    RPNCalculator calc;
    std::string input;
    
    std::cout << "=== 高级 RPN 计算器 ===" << std::endl;
    std::cout << "输入 'help' 查看帮助, 'quit' 退出" << std::endl;
    
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        
        if (input == "q" || input == "quit") break;
        if (input == "help") {
            showHelp();
            continue;
        }
        if (input == "clear") {
            calc.clear();
            std::cout << "栈已清空" << std::endl;
            continue;
        }
        if (input == "stack") {
            calc.displayStack();
            continue;
        }
        if (input == "history") {
            calc.displayHistory();
            continue;
        }
        if (input.empty()) continue;
        
        try {
            if (calc.processExpression(input)) {
                double result = calc.getResult();
                std::cout << "结果: " << result << std::endl;
            }
        } catch (const std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
    
    std::cout << "感谢使用 RPN 计算器！" << std::endl;
    return 0;
}
