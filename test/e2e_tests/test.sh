#!/bin/bash

if [ $# -lt 2 ]; then
    echo "❌ Использование: $0 <путь_к_бинарнику> <путь_к_папке_с_тестами>"
    exit 1
fi

avl="$1"      # Путь к бинарнику avl
set="$2"      # Путь к бинарнику set
data_dir="$3"  # Путь к папке с данными

if [ ! -d "$data_dir" ]; then
    echo "❌ Папка $data_dir не найдена!"
    exit 1
fi

files=("$data_dir"/*.txt)

for file in "${files[@]}"; do 
    echo "📂 Обработка файла: $file"

    echo "🔢 Сравнение деревьев:"
    
    result=$( "$avl" < "$file" )  # Запускаем программу и сохраняем результат
    expected=$( "$set" < "$file" )  # Читаем ожидаемый результат

    echo "🔹 Результат AVL: $result"
    echo "🔹 Ожидалось SET: $expected"

    if [ "$result" == "$expected" ]; then
        echo "✅ Тест пройден!"
    else
        echo "❌ Ошибка!"
        exit 1
    fi
    echo "-----------------------------------"
done
