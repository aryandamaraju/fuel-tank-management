// UMBC - CMSC 341 - Spring 2023 - Proj1
#include "fuel.h"

FuelSys::FuelSys() { m_current = nullptr; }

FuelSys::~FuelSys() {
  Tank *temp = m_current->m_next;
  while (temp != m_current) {
    Tank *nxt = temp->m_next;
    if (temp->m_pumps != nullptr) {
      Pump *head = temp->m_pumps;
      while (head != nullptr) {
        Pump *next = head->m_next;
        delete head;
        head = next;
      }
      temp->m_pumps = nullptr;
    }
    delete temp;
    temp = nxt;
  }
  m_current = nullptr;
}

bool FuelSys::addTank(int tankID, int tankCap, int tankFuel = 0) {
  bool var = false;
  Tank *temp = m_current;
  Tank *tank = new Tank(tankID, tankCap, tankFuel);

  if (tankCap < MINCAP || tankCap > DEFCAP) {
    return false;
  }

  if (m_current == nullptr) {
    m_current = tank;
    m_current->m_next = m_current;
    var = true;
  }

  else if (m_current->m_next == m_current) {
    m_current->m_next = tank;
    tank->m_next = temp;
    m_current = tank;
    var = true;
  }

  else {
    tank->m_next = temp->m_next;
    m_current->m_next = tank;
    m_current = tank;
    var = true;
  }

  return var;
}

bool FuelSys::removeTank(int tankID) {

  Tank *prev = m_current;
  bool var = false;
  if (m_current == nullptr) {
    return false;
  }

  if (m_current->m_next == m_current) {
    Pump *head = m_current->m_pumps;
    while (head != nullptr) {
      cout << head->m_pumpID << endl;
      Pump *next = head->m_next;
      delete head;
      head = next;
    }
    m_current->m_pumps = nullptr;
    delete m_current;
    m_current = nullptr;
    var = true;
  } else {
    Tank *temp = m_current;
    while (temp->m_tankID != tankID) {
      prev = temp;
      temp = temp->m_next;
    }
    Pump *head = temp->m_pumps;
    while (head != nullptr) {
      cout << head->m_pumpID << endl;
      Pump *next = head->m_next;
      delete head;
      head = next;
    }
    temp->m_pumps = nullptr;
    prev->m_next = temp->m_next;
    delete temp;
    m_current = prev;
    m_current->m_next = temp->m_next;
    var = true;
  }
  return var;
}

bool FuelSys::findTank(int tankID) {
  bool var = false;
  Tank *temp;
  if (m_current != nullptr) {
    temp = m_current;
    while (temp->m_next->m_tankID != tankID && temp != m_current->m_next) {
      temp = temp->m_next;
    }
    if (temp->m_next->m_tankID == tankID) {
      m_current = temp;
      var = true;
    }
  } else {
    var = false;
  }
  return var;
}

bool FuelSys::addPump(int tankID, int pumpID, int targetTank) {
  bool var = false;
  Tank *temp;
  Pump *pump = new Pump(pumpID, targetTank);
  if (findTank(tankID)) {
    temp = m_current->m_next;
    if (temp->m_pumps == nullptr) {
      temp->m_pumps = pump;
      pump->m_next = nullptr;
      var = true;
    } else {
      pump->m_next = temp->m_pumps;
      temp->m_pumps = pump;
      var = true;
    }
  }

  return var;
}

bool FuelSys::removePump(int tankID, int pumpID) {
  bool var = false;
  Tank *temp;
  Pump *pump, *prev;
  if (findTank(tankID)) {
    temp = m_current->m_next;
    pump = temp->m_pumps;
    if (pump == nullptr) {
      return false;
    }
    if (pumpID == pump->m_pumpID) {
      cout << "IN: " << pump->m_pumpID << endl;
      if (pump->m_next == nullptr) {
        temp->m_pumps = nullptr;
      } else {
        temp->m_pumps = pump->m_next;
      }
      delete pump;
      var = true;
    } else {
      while (pump != nullptr) {
        if (pump->m_pumpID == pumpID) {
          if (pump->m_next != nullptr) {
            prev->m_next = pump->m_next;
          } else {
            prev->m_next = nullptr;
          }
          delete pump;
          return true;
        }
        prev = pump;
        pump = pump->m_next;
      }
    }
  }
  return var;
}

int FuelSys::totalFuel() const {
  int total = 0;
  Tank *temp = m_current;
  while (temp->m_next != m_current) {
    total += temp->m_tankFuel;
  }
  return total;
}

bool FuelSys::drain(int tankID, int pumpID, int fuel) {}

bool FuelSys::fill(int tankID, int fuel) {
  bool var = false;
  Tank *temp = m_current;
  if (m_current->m_tankID == tankID) {
    temp->m_tankFuel += fuel;
    if (temp->m_tankFuel > temp->m_tankCapacity) {
      temp->m_tankFuel = temp->m_tankCapacity;
    }
    var = true;
  } else {
    while (temp->m_next != m_current) {
      if (temp->m_next->m_tankID == tankID) {
        temp->m_tankFuel += fuel;
        if (temp->m_tankFuel > temp->m_tankCapacity) {
          temp->m_tankFuel = temp->m_tankCapacity;
        }
        var = true;
      }
      temp = temp->m_next;
    }
  }
  return var;
}

const FuelSys &FuelSys::operator=(const FuelSys &rhs) {
  if (&rhs == this) {
    return *this;
  }

  return *this;
}

void FuelSys::dumpSys() const {
  if (m_current != nullptr) {
    Tank *tempTank = m_current->m_next; // we start at front item
    // we continue visting nodes until we reach the cursor
    while (m_current != nullptr && tempTank != m_current) {
      cout << "Tank " << tempTank->m_tankID << "(" << tempTank->m_tankFuel
           << " kg)" << endl;
      // now dump the targets for all pumps in this tank
      // we need to traverse the list of pumps
      dumpPumps(tempTank->m_pumps);
      tempTank = tempTank->m_next;
    }
    // at the end we visit the cursor (current)
    // this also covers the case that there is only one item
    cout << "Tank " << m_current->m_tankID << "(" << m_current->m_tankFuel
         << " kg)" << endl;
    dumpPumps(tempTank->m_pumps);
    cout << "The current tank is " << m_current->m_tankID << endl;
  } else
    cout << "There is no tank in the system!\n\n";
}

void FuelSys::dumpPumps(Pump *pumps) const {
  // this traverses the linked list to the end
  Pump *tempPump = pumps;
  while (tempPump != nullptr) {
    cout << " => pump " << tempPump->m_pumpID << "(To tank "
         << tempPump->m_target << ")" << endl;
    tempPump = tempPump->m_next;
  }
}