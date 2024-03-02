#ifndef __LIST_H__
#define __LIST_H__

#include <stdint.h>
#include <stdbool.h>

#define BEGIN_MACRO_FUNC do {
#define END_MACRO_FUNC } while (0)

// Узел листа содержит указатели на соседей и тип данных согласно варантам
typedef struct list_node ListNode;

// Структура списка содержит указатели на границы списка согласно варианту
typedef struct list List;

typedef struct list_iterator {
    ListNode* current; // На какой узел указывает итератор
    const List* list; // Какому листу принадлежит итератор
} ListIterator;

// Создаёт список
List* list_create(void);

// Уничтожает список: удаляет все узлы
void list_destroy(List** const list);

// Вставляет новый элемент в начало списка и возвращает итератор на него
ListIterator list_insert_front(List* const list);

// Вставляет новый элемент в начало списка и сразу присваевает ему значение
//  
// Пример: list_emplace_front(list, 1);
#define list_emplace_front(list_p, value) BEGIN_MACRO_FUNC \
    ListIterator it = list_insert_front(list_p); \
    if (!list_iterator_equal(it, list_end(list_p))) { \
        *(__typeof(value)*)list_iterator_data(it) = value; \
    } \
END_MACRO_FUNC

// Вставляет новый элемент в конец списка и возвращает итератор на него
ListIterator list_insert_back(List* const list);

// Вставляет новый элемент в конец списка и сразу присваевает ему значение
//  
// Пример: list_emplace_back(list, 9000);
#define list_emplace_back(list_p, value) BEGIN_MACRO_FUNC \
    ListIterator it = list_insert_back(list_p); \
    if (!list_iterator_equal(it, list_end(list_p))) { \
        *(__typeof(value)*)list_iterator_data(it) = value; \
    } \
END_MACRO_FUNC

// Вставляет элемент после итератора и возвращает итератор на новый элемент
ListIterator list_insert_after(List* const list, const ListIterator iterator);

// Вставляет элемент после итератора и сразу присваевает ему значение
//  
// Пример: list_emplace_after(list, it, 5);
#define list_emplace_after(list_p, iterator, value) BEGIN_MACRO_FUNC \
    ListIterator it = list_insert_after(list_p, iterator); \
    if (!list_iterator_equal(it, list_end(list_p))) { \
        *(__typeof(value)*)list_iterator_data(it) = value; \
    } \
END_MACRO_FUNC

// Удаляет элемент из начала списка
void list_remove_front(List* const list);

// Удаляет элемент из конца списка
void list_remove_back(List* const list);

// Удаляет элемент после итератора
void list_remove_after(List* const list, const ListIterator iterator);

// Очищает список: после этой операции, список должен стать пустым
void list_clear(List* const list);

// Получить итератор на первый элемент списка
ListIterator list_begin(const List* const list);

// Синоним list_begin, возвращает итератор на первый элемент
#define list_first(list_p) list_begin(list_p)

// Получить итератор на последний элемент списка
ListIterator list_last(const List* const list);

// Получить итератор элемента списка после последнего, т.е. NULL или другое
ListIterator list_end(const List* const list);

// Вычислить длину списка
int32_t list_size(const List* const list);

// Проверить, что список пуст
bool list_empty(const List* const list);

// Печать списка: значения узлов идут через пробел, в конце должен быть перенос строки
void list_print(const List* const list);

//
// List iterator
//

// Получить указатель на данные узла итератора
void* list_iterator_data(const ListIterator iterator);

// Разыменовать указатель по типу type на данные итератора
// 
// Пример: list_iterator_deref(list_begin(list), int) = 5;
// `-- Присвоить значение 5 первому элементу списка
#define list_iterator_deref(iterator, type) *(type*)list_iterator_data(iterator)

// Продвинуть итератор на distance шагов вперед или назад
void list_iterator_advance(ListIterator* const iterator, int32_t distance);

// Подвинуть итератор на следующий элемент
void list_iterator_increment(ListIterator* const iterator);

// Подвинуть итератор на предыдущий элемент
// Если структура списка не имеет указателей на предыдущих соседей,
// то реализовывать не нужно, просто оставьте тело функции пустым
void list_iterator_decrement(ListIterator* const iterator);

// Итераторы равны, если они принадлежат одинаковым спискам и
// указывают на одни и те же элементы в нём
bool list_iterator_equal(const ListIterator lhs, const ListIterator rhs);

#endif // __LIST_H__
