#include <map>
#include <string>
#include <queue>
#include <stack>
#include <set>
#include <iostream>
#include <fstream>
#include <cassert>
#include <cmath>
#include <memory>
#include <algorithm>
#include <vector>

using namespace std;

// Тип данных в ячейке памяти
enum class celltype
{
    NONE = 0,
    INT = 1,
    STR = 2
};

// Является ли строка числом?
bool isnumber(const string &s)
{
    return !s.empty() && all_of(s.begin(), s.end(), ::isdigit);
}

// Класс ячейки памяти
struct memorycell
{
    // Одно из полей заполнено (соответственно типу - INT или STR)
    int i;
    string s;

    // Тип ячейки
    celltype type;

    // Конструктор из int
    memorycell(int ival)
        : i(ival), s(), type(celltype::INT)
    {
    }

    // Конструктор из string
    memorycell(const string &sval)
        : i(0), s(sval), type(celltype::STR)
    {
    }

    // Пустая клетка
    memorycell()
        : i(0), s(""), type(celltype::NONE)
    {
    }

    // Перевести в строку (если строка - вернуть её же, если число - закавычить)
    string printable()
    {
        if (type == celltype::INT)
            return to_string(i);
        return s;
    }
};

// Сложение (если INT) или конкатенация (если STR)
memorycell operator+(const memorycell &a, const memorycell &b)
{
    if (a.type != b.type)
        return -1;

    switch (a.type)
    {
    case celltype::INT:
        return a.i + b.i;

    case celltype::STR:
        return a.s + b.s;

    default:
        return -1;
    }
}

// Разность (только для INT)
int operator-(const memorycell &a, const memorycell &b)
{
    return a.i - b.i;
}

// Модуль (только для INT)
int operator%(const memorycell &a, const memorycell &b)
{
    return a.i % b.i;
}

int operator/(const memorycell &a, const memorycell &b)
{
    return a.i / b.i;
}

int operator*(const memorycell &a, const memorycell &b)
{
    return a.i * b.i;
}

bool operator==(const memorycell &a, const memorycell &b)
{
    if (a.type != celltype::INT && b.type != celltype::INT)
        return a.s == a.s;
    return a.i == b.i;
}

bool operator!=(const memorycell &a, const memorycell &b)
{
    if (a.type != celltype::INT && b.type != celltype::INT)
        return a.s != a.s;
    return a.i != b.i;
}

bool operator>(const memorycell &a, const memorycell &b)
{
    if (a.type != celltype::INT && b.type != celltype::INT)
        return -1;
    return a.i > b.i;
}

bool operator>=(const memorycell &a, const memorycell &b)
{
    if (a.type != celltype::INT && b.type != celltype::INT)
        return -1;
    return a.i >= b.i;
}

bool operator<(const memorycell &a, const memorycell &b)
{
    if (a.type != celltype::INT && b.type != celltype::INT)
        return -1;
    return a.i < b.i;
}

bool operator<=(const memorycell &a, const memorycell &b)
{
    if (a.type != celltype::INT && b.type != celltype::INT)
        return -1;
    return a.i <= b.i;
}

struct fixed_stack
{
    vector<memorycell> s;

    int last_cell;

    fixed_stack() : s(4096), last_cell(0) {}

    void push(memorycell val)
    {
        if (last_cell != 4096)
        {
            last_cell++;
            s[last_cell] = val;
        }
    }
    memorycell pop()
    {
        return s[last_cell--];
    }
    memorycell top()
    {
        return s[last_cell];
    }
};

class os;

// Класс одного процесса
class process
{
private:
    // Исходный код
    vector<string> source_code_;

    // Лэйблы (имя-строка)
    map<string, size_t> labels_;

    // Память процесса (стек)
    fixed_stack mem_;

    // Данные о pid процесса и его родителя

    const pid_t ppid_;

    // Позиция в исходнике на которой остановились
    size_t pos_;

    // Очередь сообщенийpreact
    queue<memorycell> msg_queue;

    // Исполнить строку line. Если произошла блокировка, false.
    bool step(const string &line);

    // Соответствующая ОС (для отправки сообщений и запуска программ)
    os &system_;

    // Мэп название переменной-адрес
    map<string, memorycell *> variables;

public:
    const pid_t pid_;
    // Конструктор, инициализирующий внутренние переменные
    process(string fname, os &system, size_t pid, size_t ppid) : pid_(pid), ppid_(ppid), system_(system), pos_(0)
    {
        ifstream file;

        file.open(fname, ios::in);

        if (file.is_open())
        {
            string line;
            int line_count = 0;

            while (getline(file, line))
            {
                if (line[0] == ':')
                {
                    labels_.insert({line.substr(1, line.size() - 1), line_count});
                }
                else if (!line.empty())
                {
                    line_count++;
                    source_code_.push_back(line.c_str());
                }
            }

            file.close();
        }
    }

    // Выполнить следующие num_steps строк исходника (false, если закончил исполнение)
    bool exec(size_t num_steps)
    {
        if (source_code_.size() < num_steps)
        {
            num_steps = source_code_.size();
        }

        for (size_t i = 0; i < num_steps; i++)
        {
            /*  cout << "Pos: " << pos_ << " | " << source_code_[pos_] << endl; */

            if (source_code_.size() <= pos_)
            {
                return false;
            }

            bool result = step(source_code_[pos_]);

            if (!result)
            {
                break;
            }

            pos_++;
        }

        return true;
    }

    void send(memorycell msg)
    {
        msg_queue.push(msg);
    }

    // Все аллоцированные этим процессом переменные
    vector<memorycell *> get_allocated()
    {

        vector<memorycell *> addrs;

        for (auto el : variables)
        {
            addrs.push_back(el.second);
        }

        return addrs;
    }
};

// Класс операционной системы
class os
{
private:
    // Таблица PID (идентификатор процесса) - указатель на процесс
    map<pid_t, unique_ptr<process>> pid_table;

    // Размер кванта в алгоритме Round Robin
    size_t quantum_;

    // Максимальный используемый PID
    pid_t pid_max;

    // Очередь PID-ов алгоритма Round Robin
    queue<pid_t> round_robin;

    // Step 4
    // Арена всех ячеек памяти для переменных (heap)
    vector<memorycell> arena;

    set<int> unused_variables;

public:
    // Конструктор класса os - инициализировать переменные
    os(size_t quantum, pid_t pid_max) : quantum_(quantum), pid_max(pid_max), arena(4096)
    {
        for (int i = 0; i < arena.size(); i++)
        {
            unused_variables.insert(i);
        }
    }

    void kill_process(pid_t pid)
    {
        auto alloc = pid_table[pid]->get_allocated();
        auto ptr = arena.data();

        for (auto el : alloc)
        {
            auto x = el - ptr;

            unused_variables.insert(x);
        }

        pid_table.erase(pid);
    }

    // Начать цикл RoundRobin
    void start()
    {
        while (!(round_robin.empty()))
        {
            pid_t proc_pid = round_robin.front();
            round_robin.pop();

            if (pid_table.count(proc_pid) != 0)
            {

                bool result = pid_table[proc_pid]->exec(quantum_);

                if (result)
                {
                    round_robin.push(proc_pid);
                }
                else
                {

                    kill_process(proc_pid);
                }
            }
        }
    }

    // Добавить новый процесс в pid_table с исходником из аргументов, вернуть PID
    pid_t exec(string fname, pid_t ppid)
    {
        pid_max = pid_max + 1;

        pid_table.emplace(pid_max, new process(fname, *(this), pid_max, ppid));

        round_robin.push(pid_max);

        return pid_max;
    }

    // Отправить процессу сообщение
    void send(pid_t target, memorycell val)
    {
        pid_table.at(target)->send(val);
    }

    // Получить ячейку из арены
    memorycell *allocate()
    {
        int idx = *unused_variables.begin();

        auto addr = &arena.at(idx);

        return addr;
    }
};

// Реализация функции step из класса process
bool process::step(const string &line)
{

    // Завести переменные
    if (line.rfind("int", 0) == 0)
    {
        int n = atoi(line.substr(4).c_str());
        mem_.push(n);
    }
    else if (line.rfind("str", 0) == 0)
    {
        string s = line.substr(4);

        mem_.push(memorycell(s));
    }

    // Арифметика
    else if (line == "+")
    {
        memorycell a = mem_.pop();
        memorycell b = mem_.pop();

        mem_.push(a + b);
    }
    else if (line == "-")
    {
        memorycell a = mem_.pop();
        memorycell b = mem_.pop();

        mem_.push(b - a);
    }
    else if (line == "%")
    {
        memorycell a = mem_.pop();
        memorycell b = mem_.pop();

        mem_.push(b % a);
    }
    else if (line == "/")
    {
        memorycell a = mem_.pop();
        memorycell b = mem_.pop();

        mem_.push(b / a);
    }
    else if (line == "*")
    {
        memorycell a = mem_.pop();
        memorycell b = mem_.pop();

        mem_.push(a * b);
    }
    else if (line == "inc")
    {
        memorycell a = mem_.pop();

        memorycell b = 1;

        mem_.push(a + b);
    }
    else if (line == "dec")
    {
        int a = mem_.pop().i;

        int b = 1;

        mem_.push(a - b);
    }
    else if (line == "sqrt")
    {
        int a = mem_.pop().i;

        mem_.push(sqrt(a));
    }

    // Сервисные функции
    else if (line == "dup")
    {

        mem_.push(mem_.s[mem_.last_cell]);
    }
    else if (line == "swap")
    {
        memorycell a = mem_.pop();

        memorycell b = mem_.pop();

        mem_.push(b);

        mem_.push(a);
    }
    else if (line == "pop")
    {
        mem_.pop();
    }
    else if (line == "print")
    {
        string a = mem_.pop().printable();

        cout << a << endl;
    }
    else if (line == "halt")
    {
        pos_ = source_code_.size();
    }

    // Операции с другими процессами (посредством os)
    else if (line.rfind("run", 0) == 0)
    {
        string s = line.substr(4);

        pid_t pid = system_.exec(s, pid_);

        mem_.push(pid);
    }
    else if (line == "send")
    {
        string val = mem_.pop().printable();

        pid_t pid = mem_.pop().i;

        system_.send(pid, val);
    }
    else if (line == "recv")
    {

        if (msg_queue.empty())
        {
            // Возвращаем false чтобы залочить процесс

            return false;
        }
        else
        {
            memorycell msg = msg_queue.front();

            msg_queue.pop();

            mem_.push(msg);

            return true;
        }
    }
    else if (line == "pid")
    {
        mem_.push(pid_);
    }
    else if (line == "ppid")
    {
        mem_.push(ppid_);
    }

    // Операции с переменными
    else if (line.rfind("new", 0) == 0)
    {
        auto var = system_.allocate();

        string name = line.substr(4);

        variables[name] = var;
    }
    else if (line.rfind("set", 0) == 0)
    {

        string name = line.substr(4);

        memorycell value = mem_.pop();

        *variables[name] = value;
    }
    else if (line.rfind("put", 0) == 0)
    {
        string name = line.substr(4);
        mem_.push(*variables[name]);
    }
    // Джампы
    else if (line.rfind("jmp", 0) == 0)
    {

        string label = (line.substr(4));

        pos_ = labels_.at(label);
    }
    else if (line.rfind("jne", 0) == 0)
    {
        auto a = mem_.pop();

        auto b = mem_.pop();

        if (a != b)
        {
            string label = (line.substr(4));

            pos_ = labels_.at(label);
        }
    }
    else if (line.rfind("jle", 0) == 0)
    {
        memorycell a = mem_.pop();

        memorycell b = mem_.pop();

        if (a <= b)
        {
            string label = (line.substr(4));

            pos_ = labels_.at(label);
        }
    }
    else if (line.rfind("jge", 0) == 0)
    {
        memorycell a = mem_.pop();

        memorycell b = mem_.pop();

        if (a >= b)
        {
            string label = (line.substr(4));

            pos_ = labels_.at(label);
        }
    }
    else if (line.rfind("jl", 0) == 0)
    {
        memorycell a = mem_.pop();

        memorycell b = mem_.pop();

        if (a < b)
        {
            string label = (line.substr(3));

            pos_ = labels_.at(label);
        }
    }
    else if (line.rfind("jg", 0) == 0)
    {
        memorycell a = mem_.pop();

        memorycell b = mem_.pop();

        if (a > b)
        {
            string label = (line.substr(3));

            pos_ = labels_.at(label);
        }
    }
    else if (line.rfind("je", 0) == 0)
    {
        memorycell a = mem_.pop();

        memorycell b = mem_.pop();

        if (a == b)
        {
            string label = (line.substr(3));

            pos_ = labels_.at(label) - 1;
        }
    }

    // Остальное
    else if (line.rfind("pass", 0) == 0)
    {
    }
    else
    {
        cout << "Invalid CMD!: " << line << endl;
    }
    return true;
}

int main()
{
    // Launch OS with 5 steps of execution
    os OS(10, 0);
    OS.exec("code.asm", 0);

    OS.start();

    return 0;
}
