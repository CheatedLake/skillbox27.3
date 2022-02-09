#include <iostream>
#include <vector>
#include <cassert>
#include <ctime>

class Manager;

enum TaskType {
  None = -1,
  A,
  B,
  C
};

class Worker {
public:
  Worker(Manager* _parent) : m_parentID(_parent) {};

  bool isBusy() const {
    return taskType != None;
  }

  TaskType getTaskType() const {
    return taskType;
  }

  void solveTask(TaskType _type) {
    taskType = _type;
  }
  friend class Manager;

private:
  Manager* m_parentID = nullptr;
  TaskType taskType = None;             // 'None' - free, also can be 'A','B','C'
};


class Manager : public Worker {         // no inheritance
public:
  static int m_IDgenerator;
  Manager() : Worker(this) {
    m_ID = m_IDgenerator++;             // next manager will get unique ID
        std::cout << "Manager " << m_ID << " created with team size: " << m_workers.size() << std::endl;
  };

  void occupyWorkers(int _teamSize) {
    assert(_teamSize != 0);
    if (_teamSize > 0) {
      m_workers.reserve(_teamSize);  // set capacity once
      // fill team with new workers by team capacity right here
      int cap = m_workers.capacity();
      for (int i = 0; i < cap; ++i) {
        Worker *worker = new Worker(this);
        m_workers.push_back(worker);
      }
    }
  }

  bool isBusy() const {
    for (const auto worker : m_workers) {
      if (!worker->isBusy()) {
        return false;
      }
    }
    return true;
  }

  void getDebugInfo() const {
    // delete on release
    std::cout << "Manager " << m_ID << " has " << m_workers.size() << " workers" << std::endl;
    for (const Worker* item : m_workers) {
      std::cout << "Worker " << item <<  " has job " << (int)item->getTaskType() << std::endl;
    }
  }

  int getID() const {
    return m_ID;
  }

  ~Manager() {
    for (auto& item : m_workers) {
      delete item;
      item = nullptr;
    }
  }

// PUBLIC team for this manager
std::vector<Worker*> m_workers;
private:
  int m_ID = 0;                         // each Manager have unique ID
  };


class Director : public Manager {       // class to manage jobs for company
public:
  Director(int _teamsCount)  {
    m_teams.reserve(_teamsCount);
  }
  bool isEveryoneBusy() {
    for (Manager* team : m_teams) {
      if (!team->isBusy()) {
        return false;
      }
    }
    return true;
  }

  void addTeam(Manager* _manager) {
    m_teams.push_back(_manager);
  }

  void giveTask(int _id) {
    // I think each command nave to be sent to all managers
    for (Manager* team : m_teams) {
      std::srand(team->getID() + _id);   //get unique rand() grain
      int tasksCount = rand() % team->m_workers.size() + 1;
      for (int i = 0; i < tasksCount; ++i)
      {
        TaskType task = (TaskType)(std::rand() % 3);
        for (int j = 0; j < team->m_workers.size(); ++j)
        {
          if (!team->m_workers[j]->isBusy())
          {
            team->m_workers[j]->solveTask(task);
            break;
          }
        }
      }
    }
  }

  void printInfo() {
    // delete on release
    for (auto &item: m_teams) {
      item->getDebugInfo();
    }
  }

  ~Director() {
    for (Manager* team : m_teams) {
      delete team;
      team = nullptr;
    }
  }

private:
  std::vector<Manager*> m_teams;
};

int Manager::m_IDgenerator = 1;  //each manager (parent = nullptr) will increase value to get unique ID of Worker exampl

int main() {
  std::srand(std::time(nullptr));
  // init director to manage company
  std::cout << "Welcome new director! Let's create new company. Enter number of work teams:";
  int teamsCount = 0;
  std::cin >> teamsCount;
  Director newDirector (teamsCount);
  for (int i = 0; i < teamsCount; ++i) {
    std::cout << "Enter number of workers in team " << i+1 << "/" << teamsCount << ": ";
    int workers = 0;
    std::cin >> workers;
    //init manager with team
    Manager* newManager = new Manager;
    newManager->occupyWorkers(workers);
    newDirector.addTeam(newManager);
  }
//  newDirector.printInfo();
  while (!newDirector.isEveryoneBusy()) {
    std::cout << "Enter director's command to process: ";
    int command = 0;
    std::cin >> command;
    newDirector.giveTask(command);
    newDirector.printInfo();  // <- this wil show status of all company
  }
  std::cout << "Great! Everyone has job now!" << std::endl;
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