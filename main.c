#include "list.h"

#define TEST_IMPLEMENTATION
#include "test.h"

// Выставите эту переменную в true, если Ваш список является двунаправленным
static bool my_linked_list_is_bidirectional = "change me";

// Напишите функцию, выполняющую "нестандартное действие" _на итераторах_ в функции ниже, вместо примера
// Список аргументов и возвращаемый тип можно изменять как Вам удобно по заданию
// 
// Ниже пример поиска первого элемента в списке со значением равным _ch_
static ListIterator custom_action(List* const list, char ch) 
{
    for (ListIterator it = list_begin(list);
         !list_iterator_equal(it, list_end(list));
         list_iterator_increment(&it)) {

        if (list_iterator_deref(it, char) == ch) {
            return it;
        }
    }

    return list_end(list);
}

// Напишите тест для своего "нестандартного действия" на итераторах в функции ниже, вместо примера
// Внутри TEST_ASSERT() нужно написать логическое выражение,
// которое будет ожидаемо верным. Примеры, как доказывать утверждения, смотрите ещё ниже
TEST(custom)
{
    List* list = list_create();

    // Нет элемента 'e' в пустом списке
    {
        ListIterator found = custom_action(list, 'a');
        TEST_ASSERT(list_iterator_equal(found, list_end(list)) == true);
    }

    list_emplace_front(list, 'a');
    list_emplace_front(list, 'b');
    list_emplace_front(list, 'c');

    // Нет элемента 'e' в списке с элементами 'c', 'b', 'a'
    {
        ListIterator found = custom_action(list, 'e');
        TEST_ASSERT(list_iterator_equal(found, list_end(list)) == true);
    }

    list_emplace_back(list, 'd');
    list_emplace_back(list, 'e');
    list_emplace_back(list, 'f');

    // Найден элемент 'd' в списке с элементами: 'c', 'b', 'a', 'd', 'e', 'f'
    {
        ListIterator found = custom_action(list, 'd');
        TEST_ASSERT(!list_iterator_equal(found, list_end(list)) == true);
    }

    list_destroy(&list);
}

//
// Общие юнит-тесты
// 

TEST(create_list)
{
    List* list = list_create();
    TEST_ASSERT(*(void**)list == NULL);
    list_destroy(&list);
}

TEST(destroy_list)
{
    List* list = NULL;

    list_destroy(&list);

    list = list_create();

    list_destroy(&list);

    TEST_ASSERT(list == NULL);
}

TEST(insert_at_list_front)
{
    List* list = NULL;

    {
        ListIterator it = list_insert_front(list);
        TEST_ASSERT(it.current == NULL && it.list == NULL);
    }

    list = list_create();

    list_emplace_front(list, 'a');
    TEST_ASSERT(list_size(list) == 1);

    TEST_ASSERT(list_iterator_equal(list_first(list), list_last(list)) == true);

    list_emplace_front(list, 'b');
    list_emplace_front(list, 'c');

    TEST_ASSERT(list_iterator_deref(list_first(list), char) == 'c');

    TEST_ASSERT(list_iterator_deref(list_last(list), char) == 'a');

    list_destroy(&list);
}

TEST(insert_at_list_back)
{
    List* list = NULL;

    {
        ListIterator it = list_insert_back(list);
        TEST_ASSERT(it.current == NULL && it.list == NULL);
    }

    list = list_create();

    list_emplace_back(list, 'x');
    list_emplace_back(list, 'y');
    TEST_ASSERT(list_size(list) == 2);

    TEST_ASSERT(list_iterator_deref(list_first(list), char) == 'x');
    TEST_ASSERT(list_iterator_deref(list_last(list), char) == 'y');

    list_destroy(&list);
}

TEST(insert_after_list_element)
{
    List* list = NULL;

    {
        ListIterator it = list_insert_after(list, (ListIterator){0});
        TEST_ASSERT(it.current == NULL && it.list == NULL);
    }

    list = list_create();

    {
        List* other = list_create();

        ListIterator it = list_insert_after(list, list_end(other));
        TEST_ASSERT(it.current == NULL && it.list == NULL);

        list_destroy(&other);
    }

    {
        List* other = list_create();

        ListIterator other_it = list_insert_front(other);

        ListIterator it = list_insert_after(list, other_it);
        TEST_ASSERT(it.current == NULL && it.list == NULL);

        list_destroy(&other);
    }

    {
        ListIterator it, next, new;

        next = list_insert_front(list);
        it = list_insert_front(list);
        list_insert_front(list);

        new = list_insert_after(list, it);

        TEST_ASSERT(list_size(list) == 4);

        TEST_ASSERT(list_iterator_equal(next, new) == false);

        TEST_ASSERT(list_iterator_equal(new, list_last(list)) == false);


        if (my_linked_list_is_bidirectional) {
            list_iterator_decrement(&new);

            TEST_ASSERT(list_iterator_equal(new, it));
        } else {
            list_iterator_increment(&it);

            TEST_ASSERT(list_iterator_equal(it, new));
        }
    }

    {
        ListIterator last = list_insert_after(list, list_last(list));
        TEST_ASSERT(list_iterator_equal(last, list_last(list)));
    }

    list_destroy(&list);
}

TEST(remove_at_list_front)
{
    List* list = NULL;

    list_remove_front(list);

    list = list_create();

    list_remove_front(list);

    list_emplace_front(list, 'o');
    list_insert_front(list);

    list_remove_front(list);

    TEST_ASSERT(list_size(list) == 1);
    TEST_ASSERT(list_iterator_equal(list_first(list), list_last(list)));
    TEST_ASSERT(list_iterator_deref(list_first(list), char) == 'o');

    list_destroy(&list);
}

TEST(remove_at_list_back)
{
    List* list = NULL;

    list_remove_back(list);

    list = list_create();

    list_remove_back(list);

    list_emplace_back(list, '#');
    list_insert_back(list);

    list_remove_back(list);

    TEST_ASSERT(list_size(list) == 1);
    TEST_ASSERT(list_iterator_equal(list_first(list), list_last(list)));
    TEST_ASSERT(list_iterator_deref(list_last(list), char) == '#');

    list_destroy(&list);
}

TEST(remove_after_list_element)
{
    ListIterator last, target;

    List* list = NULL;

    list_remove_after(list, (ListIterator){});

    list = list_create();

    {
        List* other = list_create();

        list_insert_front(other);
        list_insert_front(other);

        list_remove_after(list, list_begin(other));

        list_destroy(&other);
    }

    list_insert_front(list);
    list_remove_after(list, list_begin(list));

    last = list_insert_front(list);
    list_remove_after(list, list_begin(list));

    TEST_ASSERT(list_size(list) == 1);

    list_remove_after(list, list_last(list));

    list_insert_front(list);
    list_insert_front(list);

    list_remove_after(list, list_begin(list));

    target = list_begin(list);
    list_iterator_increment(&target);

    TEST_ASSERT(list_iterator_equal(target, last));

    list_destroy(&list);
}

TEST(clear_list)
{
    List* list = NULL;

    list_clear(list);

    list = list_create();

    list_clear(list);

    list_insert_front(list);
    list_insert_front(list);
    list_insert_front(list);
    list_insert_front(list);
    list_insert_front(list);

    list_clear(list);

    TEST_ASSERT(list_size(list) == 0);

    list_destroy(&list);
}

TEST(front_list_element)
{
    List* list = NULL;

    {
        ListIterator it = list_begin(list);
        TEST_ASSERT(it.current == NULL && it.list == NULL);
    }

    list = list_create();

    {
        ListIterator it = list_begin(list);
        TEST_ASSERT(it.current == NULL && it.list == list);
    }

    {
        ListIterator new = list_insert_front(list);
        ListIterator it = list_begin(list);
        TEST_ASSERT(list_iterator_equal(it, new));
    }

    list_destroy(&list);
}

TEST(end_of_list)
{
    List* list = NULL;

    {
        ListIterator it = list_end(list);
        TEST_ASSERT(it.current == NULL && it.list == NULL);
    }

    list = list_create();

    {
        ListIterator it = list_end(list);
        TEST_ASSERT(it.list == list);
    }

    list_destroy(&list);
}

TEST(last_list_element)
{
    List* list = NULL;

    {
        ListIterator it = list_last(list);
        TEST_ASSERT(it.current == NULL && it.list == NULL);
    }

    list = list_create();

    {
        ListIterator it = list_last(list);
        TEST_ASSERT(it.list == list);
    }

    list_destroy(&list);
}

TEST(elements_count)
{
    int32_t size = 0;

    List* list = NULL;

    TEST_ASSERT(list_size(list) == 0);

    list = list_create();

    TEST_ASSERT(list_size(list) == 0);

    list_insert_front(list);
    list_insert_front(list);
    list_insert_front(list);
    list_insert_front(list);
    list_insert_front(list);

    for (ListIterator it = list_begin(list);
         !list_iterator_equal(it, list_end(list));
         list_iterator_increment(&it)) {
        ++size;
    }

    TEST_ASSERT(list_size(list) == size);

    if (my_linked_list_is_bidirectional) {
        for (ListIterator it = list_last(list);
             !list_iterator_equal(it, list_end(list));
             list_iterator_decrement(&it)) {
            --size;
        }

        TEST_ASSERT(size == 0);
    }

    list_destroy(&list);
}

TEST(middle_list_element)
{
    List* list = list_create();

    list_emplace_front(list, 'e');
    list_emplace_front(list, 'd');
    list_emplace_front(list, 'c');
    list_emplace_front(list, 'b');
    list_emplace_front(list, 'a');

    {
        ListIterator it = list_begin(list);
        list_iterator_advance(&it, list_size(list) / 2); 

        TEST_ASSERT(list_iterator_deref(it, char) == 'c');
    }

    if (my_linked_list_is_bidirectional) {
        ListIterator it = list_last(list);
        list_iterator_advance(&it, -(list_size(list) / 2));

        TEST_ASSERT(list_iterator_deref(it, char) == 'c');
    }

    list_destroy(&list);
}

int main(void) {
    return test_run((const test_fn[]){
        create_list,
        destroy_list,
        insert_at_list_front,
        insert_at_list_back,
        insert_after_list_element,
        remove_at_list_front,
        remove_at_list_back,
        remove_after_list_element,
        clear_list,
        front_list_element,
        end_of_list,
        last_list_element,
        elements_count,
        middle_list_element,
        custom,
        NULL,
    });
}
