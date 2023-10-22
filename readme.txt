Сгенерировать новый набор данных:
g++ generate.c -o ../build/generate.exe; ../build/generate.exe datasets/data_

Скомпилировать и запустить последовательную программу:
g++ task.c -fopenmp -o ../build/task.exe; ../build/task.exe ../datasets/data1

Скомпилировать и запустить параллельную программу:
g++ task_parallel.c -fopenmp -o ../build/task_parallel.exe; ../build/task_parallel.exe ../datasets/data1