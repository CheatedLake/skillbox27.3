#include <iostream>
#include <vector>
#include <cassert>

class Worker {
public:
  static int m_IDgenerator;
  Worker(Worker* _parent, int _teamSize) : m_parentID(_parent) { //_parent == nullptr? manager : worker
//    assert(_parent != 0 && _teamSize == 0);
    if (_parent == 0 && _teamSize > 0) {
      m_ID = m_IDgenerator++;
      m_team.reserve(_teamSize);
    }
  };
  void occupyWorkers() {
    int cap = m_team.capacity();
    for (int i = 0; i < cap; ++i) {
      Worker* worker = new Worker(this, 0);
      m_team.push_back(worker);
      std::cout << "Add employer " << worker << std::endl;
    }

  };

  void getDebugInfo() {
    std::cout << "Root member: " << this << " with ID " << m_ID << " has " << m_team.size() << " childs" << std::endl;
    for (const Worker* item : m_team) {
      std::cout << "Child " << item << " has ID " << item->m_ID << " with parent " << item->m_parentID << std::endl;
    }
  }

  ~Worker() {
    for (auto& item : m_team) {
      delete item;
      item = nullptr;
    }
  }

private:
  Worker* m_parentID = nullptr;
  int m_ID = 0;
  std::vector<Worker*> m_team;
};

int Worker::m_IDgenerator = 1;

int main() {
  std::vector<Worker*> company;
  std::cout << "Welcome! Enter number of work teams:";
  int teams = 0;
  std::cin >> teams;
  for (int i = 0; i < teams; ++i) {
    std::cout << "Enter number of workers in team " << i+1 << "/" << teams << ": ";
    int workers = 0;
    std::cin >> workers;
    //init team (equal 'manager')
    Worker* newWorker = new Worker(nullptr, workers);
    newWorker->occupyWorkers();
    company.push_back(newWorker);
  }

  for (auto& item : company) {
    item->getDebugInfo();
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