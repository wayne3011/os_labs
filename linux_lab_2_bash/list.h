//
// Created by ackbarca on 09.03.24.
//

#ifndef LINUX_LAB_2_BASH_LIST_H
#define LINUX_LAB_2_BASH_LIST_H
#define NOT_FOUND -1
typedef struct Node {
  int pid;
  int number;
  struct Node *next;
} Node;

typedef struct List {
  Node *head;
  Node *tail;
} List;

List *createList();
void append(List *list, int pid, int number);
int deleteByNumber(List *list, int number);
void deleteAtIndex(List *list, int index);
void deleteList(List *list);
#endif // LINUX_LAB_2_BASH_LIST_H
