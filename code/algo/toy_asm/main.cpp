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

    // Преобразовать в int (если тип INT)
    operator int()
    {
        if (type == celltype::INT)
            return i;
        return -1;
    }

    // Преобразовать в string (если тип STR)
    operator string()
    {
        if (type == celltype::STR)
            return s;
        return "";
    }

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
memorycell operator-(const memorycell &a, const memorycell &b)
{
    if (a.type != celltype::INT || b.type != celltype::INT)
        return -1;
    return a.i - b.i;
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
    const pid_t pid_;
    const pid_t ppid_;

    // Позиция в исходнике на которой остановились
    size_t pos_;

    // Очередь сообщений
    queue<memorycell> msg_queue;

    // Исполнить строку line. Если произошла блокировка, false.
    bool step(const string &line);

    // Соответствующая ОС (для отправки сообщений и запуска программ)
    os &system_;

    // Мэп название переменной-адрес
    map<string, memorycell *> variables;

public:
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
                line_count++;

                if (line.c_str()[0] == ':')
                {
                    labels_.insert({line.c_str(), line_count});
                }

                source_code_.push_back(line.c_str());
            }

            file.close();
        }
    }

    // Выполнить следующие num_steps строк исходника (false, если закончил исполнение)
    bool exec(size_t num_steps)
    {
        for (long unsigned int i = 0; i < num_steps; i++)
        {
            if (source_code_.size() < pos_)
            {
                return false;
            }
            step(source_code_[pos_]);
            pos_++;
        }
        return true;
    }

    // Отправить себе сообщение (добавить в очередь)
    //void send(memorycell msg) {
    // FILL ME STEP 3
    //}

    // Все аллоцированные этим процессом переменные
    //vector<memorycell*> get_allocated() {
    // FILL ME STEP 3
    //}
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
    //array<memorycell, 4096> arena;

    // Доступные ячейки памяти
    //set<int> availible;

public:
    // Конструктор класса os - инициализировать переменные
    os(size_t quantum)
    {
        // Step 4
        // Fill me in
    }

    void kill_process(pid_t pid)
    {
        // Step 4
        // Fill me in
    }

    // Начать цикл RoundRobin
    void start()
    {
        // Step 2
        // Fill me in
    }

    // Добавить новый процесс в pid_table с исходником из аргументов, вернуть PID
    pid_t exec(string source_name, pid_t ppid)
    {
        // Step 2
        // Fill me in
    }

    // Отправить процессу сообщение
    void send(pid_t target, memorycell val)
    {
        // Fill me in
        // Step 3
    }

    // Получить ячейку из арены
    memorycell *allocate()
    {
        // Fill me in
        // Step 4
        return NULL;
    }
};

// Реализация функции step из класса process
bool process::step(const string &line)
{
    // Завести переменные
    if (line.rfind("int", 0) == 0)
    {
        mem_.push(memorycell(atoi(line.substr(4).c_str())));
    }
    else if (line.rfind("str", 0) == 0)
    {
        mem_.push(memorycell(line.c_str()));
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

        mem_.push(a - b);
    }
    else if (line == "%")
    {
        memorycell a = mem_.pop();
        memorycell b = mem_.pop();

        mem_.push(a % b);
    }
    else if (line == "/")
    {
        memorycell a = mem_.pop();
        memorycell b = mem_.pop();

        mem_.push(a / b);
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
        memorycell a = mem_.pop();

        memorycell b = 1;

        mem_.push(a - b);
    }
    else if (line == "sqrt")
    {
        memorycell a = mem_.pop();

        mem_.push(sqrt((float)a));
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
        int a = mem_.pop();

        cout << a;
    }
    else if (line == "halt")
    {
        pos_ = source_code_.size();
    }

    // Операции с другими процессами (посредством os)
    else if (line.rfind("run", 0) == 0)
    {
        // Step 2
        // Fill me in
    }
    else if (line == "send")
    {
        // Step 3
        // Fill me in
    }
    else if (line == "recv")
    {
        // Step 3
        // Fill me in
    }
    else if (line == "pid")
    {
        // Step 2
        // Fill me in
    }
    else if (line == "ppid")
    {
        // Step 2
        // Fill me in
    }

    // Операции с переменными
    else if (line.rfind("new", 0) == 0)
    {
        // Step 4
        // Fill me in
    }
    else if (line.rfind("set", 0) == 0)
    {
        // Step 4
        // Fill me in
    }
    else if (line.rfind("put", 0) == 0)
    {
        // Step 4
        // Fill me in
    }
    // Джампы
    else if (line.rfind("jmp", 0) == 0)
    {
        // Step 1
        // Fill me in
    }
    else if (line.rfind("jne", 0) == 0)
    {
        // Step 1
        // Fill me in
    }
    else if (line.rfind("jle", 0) == 0)
    {
        // Step 1
        // Fill me in
    }
    else if (line.rfind("jge", 0) == 0)
    {
        // Step 1pintable
        // Fill me in
    }
    else if (line.rfind("jl", 0) == 0)
    {
        // Step 1
        // Fill me in
    }
    else if (line.rfind("jg", 0) == 0)
    {
        // Step 1
        // Fill me in
    }
    else if (line.rfind("je", 0) == 0)
    {
        // Step 1
        // Fill me in
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
    os OS(8);

    pid_t pid;

    process proc("code.asm", OS, pid, 8);

    proc.exec(5);

    return 0;
}
