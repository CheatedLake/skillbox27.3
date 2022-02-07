#include <iostream>
#include <vector>
//#include <cassert>
#include <ctime>

class Worker {
public:
  static int m_IDgenerator;
  Worker(Worker* _parent, int _teamSize) : m_parentID(_parent) { //_parent == nullptr? manager : worker
//    assert(_parent != 0 && _teamSize == 0);
    if (_parent == 0 && _teamSize > 0) {
      m_ID = m_IDgenerator++;         // next manager will get unique ID
      m_team.reserve(_teamSize);   // set capacity once
      freeWorkers = _teamSize;        // init free workers counter
    }
  };
  void occupyWorkers() {
    int cap = m_team.capacity();
    // fill team with new workers by team capacity
    for (int i = 0; i < cap; ++i) {
      Worker* worker = new Worker(this, 0);
      m_team.push_back(worker);
    }
  };

  void getDebugInfo() const {
    // delete on release
    std::cout << "Root member: " << this << " with ID " << m_ID << " has " << m_team.size() << " childs" << std::endl;
    for (const Worker* item : m_team) {
      std::cout << "Child " << item << " has ID " << item->m_ID << " and job " << item->jobType << std::endl;
    }
  }

  void setTasks(int _id) {
    std::srand(m_ID + _id);   //get unique rand() grain
    int taskCounter = rand() % m_team.size() + 1;
    for (auto worker : m_team) {  //jobs no more than workers, so I choose this way
      if (worker->jobType != '0')
        continue;
      if (taskCounter != 0) {
        int type = rand() % 3 + 1;
        worker->jobType = (type == 1) ? 'A' : (type == 2) ? 'B' : 'C';
        --freeWorkers;
        --taskCounter;  //its easier to track here than calc each time
      }
    }
  }

  int getFreeWorkers() const {
    return freeWorkers;
  }

  ~Worker() {
    for (auto& item : m_team) {
      delete item;
      item = nullptr;
    }
  }

private:
  Worker* m_parentID = nullptr;   // nullptr means it is manager
  int m_ID = 0;                   // manager has unique ID, worker has 0
  std::vector<Worker*> m_team;    // our team
  int freeWorkers = 0;            // easy way to getFreeWorkers()
  char jobType = '0';             // '0' - free, also can be 'A','B','C'
};

class Director{     // class to manage jobs for company
public:
  bool isEveryoneWork(const std::vector<Worker*> _company) {
    int result = 0;
    for (Worker* team : _company) {
      result += team->getFreeWorkers();   // 'get' because director not a friend(class) for workers :)
    }
    return result == 0;
  }

  void setJob(std::vector<Worker*> _company, int _id) {
    // I think EVERY command nave to be sent to EVERY manager (maybe its not, not clear from task's condition)
    for (Worker* team : _company) {
      team->setTasks(_id);
    }
  }

  void printDebug(std::vector<Worker*> _company) {
    // delete on release
    for (auto &item: _company) {
      item->getDebugInfo();
    }
  }
};

int Worker::m_IDgenerator = 1;  //each manager (parent = nullptr) wil increase value to get unique ID of Worker exampl

int main() {
  std::srand(std::time(nullptr));
  std::vector<Worker*> company;
  std::cout << "Hi! Let's create new company. Enter number of work teams:";
  int teams = 0;
  std::cin >> teams;
  for (int i = 0; i < teams; ++i) {
    std::cout << "Enter number of workers in team " << i+1 << "/" << teams << ": ";
    int workers = 0;
    std::cin >> workers;
    //init team (equal 'manager with team')
    Worker* newWorker = new Worker(nullptr, workers);
    newWorker->occupyWorkers();
    company.push_back(newWorker);
  }

  // init director to manage company
  Director newDirector;
  std::cout << "Welcome new director!" << std::endl;
  while (!newDirector.isEveryoneWork(company)) {
    std::cout << "Enter director's command to process: ";
    int command = 0;
    std::cin >> command;
    newDirector.setJob(company, command);
//    newDirector.printDebug(company);  // <- this wil show status of all company
  }
  std::cout << "Great! Everyone has job now!" << std::endl;
  //cleaner
  for (Worker* manager : company) {
    delete manager;
    manager = nullptr;
  }
  return 0;
}

/*
Задание 3. Симуляция работы компании
Реализуйте симуляцию работы компании. В компании осуществляется вертикальное управление с иерархией.
 Есть основные рабочие подчинённые, разбитые на команды. У каждой команды есть свой менеджер среднего звена.
 Во главе всей компании стоит её руководитель — глава компании.
Глава компании осуществляет стратегическое управление. Он определяет основной вектор движения — задачи верхнего уровня.
 Менеджеры среднего звена разбивают эти задачи на подзадачи и раздают их своим подчинённым. Подчинённые осуществляют выполнение работ.
Реализация данной логики должна проходить упрощённым образом.
 Указания главы компании поступают из стандартного ввода и являются не более чем целочисленными идентификаторами.
Данные идентификаторы служат зерном для генератора случайных чисел.
 Каждый из менеджеров, принимая себе эту целочисленную команду, добавляет к ней свой порядковый номер и вызывает с результирующей суммой функцию std::srand().
На основе этой суммы вычисляется общее количество задач, которое требуется выполнить данной командой, — от 1 до количества работников в группе.
 Всего существует три типа задач для работников: A, B и C. Они тоже выбираются случайно и распределяются между незанятыми рабочими.
При старте программы пользователь указывает в стандартном вводе количество команд и количество работников в каждой из команд.
Далее пользователь вводит целые числа, которые соответствуют указаниям руководителя компании. Программа завершается, когда все работники заняты какими-то задачами.

Советы и рекомендации
Пользуйтесь всем изученным в модуле, включая иерархии классов для представления сотрудников и указатель this, если потребуется.

Что оценивается
Корректность работы программы и элегантность архитектурных решений.
 */