#include <stdio.h>
#include <stdlib.h>
#include "AVL tree/AVL.h"

// Вспомогательная рекурсивная функция для красивого вывода
// space - текущее количество отступов, count - шаг отступа (например, 8 пробелов)
void print_tree_util(Node_avl* root, int space, int count) {
    if (root == NULL) {
        return;
    }

    // Увеличиваем расстояние между уровнями
    space += count;

    // Сначала печатаем правое поддерево (оно будет сверху в консоли)
    print_tree_util(root->right, space, count);

    // Печатаем текущий узел после отступов
    printf("\n");
    for (int i = count; i < space; i++) {
        printf(" ");
    }
    // Выводим ключ и его фактор баланса в скобках для самопроверки
    printf("%d(%d)\n", root->data, get_balance_factor(root));

    // Печатаем левое поддерево (оно будет снизу в консоли)
    print_tree_util(root->left, space, count);
}

// Основная обертка для вывода дерева
void print_tree(Node_avl* root) {
    printf("\n--- Структура AVL-дерева (повернуто на 90 градусов влево) ---");
    // 8 - это количество пробелов на один уровень глубины
    print_tree_util(root, 0, 8); 
    printf("-------------------------------------------------------------\n");
}

int main() {
    Node_avl* root = NULL;
    
    // Данные из твоего ручного теста
    int keys[] = {40, 20, 10, 25, 30, 22, 50, 24};
    int n = sizeof(keys) / sizeof(keys[0]);

    printf("=== ТЕСТ 1: Последовательная вставка элементов ===\n");
    for (int i = 0; i < n; i++) {
        printf("\nВставляем узел: %d", keys[i]);
        root = insert(root, keys[i]);
        print_tree(root);
    }

    printf("\n=== ТЕСТ 2: Проверка удаления элементов ===\n");
    
    // Давай удалим узел 25 (корень), чтобы проверить самый сложный случай удаления с двумя детьми
    printf("\nУдаляем корень дерева (25):");
    root = del(root, 25);
    print_tree(root);

    // Удалим лист 5
    printf("\nУдаляем лист (5):");
    root = del(root, 50);
    print_tree(root);

    // Очистка памяти (в реальном коде стоило бы написать free_tree, но для теста пока оставим так)
    return 0;
}