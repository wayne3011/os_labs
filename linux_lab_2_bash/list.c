#include "list.h"
#include <stdlib.h>

// Создание нового списка
List* createList() {
    List* newList = (List*)malloc(sizeof(List));
    if (newList != NULL) {
        newList->head = NULL;
        newList->tail = NULL;
    }
    return newList;
}

// Добавление элемента в конец списка
void append(List* list, int pid, int number) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (newNode != NULL) {
        newNode->pid = pid;
        newNode->number = number;
        newNode->next = NULL;

        if (list->head == NULL) {
            list->head = newNode;
            list->tail = newNode;
        } else {
            list->tail->next = newNode;
            list->tail = newNode;
        }
    }
}

int deleteByNumber(List* list, int number) {
    Node* current = list->head;
    Node* prev = NULL;
    int result = 0;
    while (current != NULL) {
        if (current->number == number) {
            if (prev == NULL) {
                list->head = current->next;

                if (list->head == NULL) {
                    list->tail = NULL;
                }

                result = current->pid;
                free(current);
                return result;
            } else {
                prev->next = current->next;

                if (current->next == NULL) {
                    list->tail = prev;
                }
                result = current->pid;
                free(current);
                return result;
            }
        }

        prev = current;
        current = current->next;
    }

    if(current == NULL) return -1;
    return 0;
}

// Удаление всего списка
void deleteList(List* list) {
    Node* current = list->head;
    while (current != NULL) {
        Node* temp = current;
        current = current->next;
        free(temp);
    }

    free(list);
}