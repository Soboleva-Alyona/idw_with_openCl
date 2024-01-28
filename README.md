## Задание
Есть функция для вычисления интерполяции по двумерным координатам IDW inverse distance weight [в этом файле](./Data.h).

Надо её распараллелить на OpenCL. Написать код для хоста и для девайса.


##  Результат

Код девайса находится в [cl.h](cl.h).

Код для хоста (выбор девайса и запуск самого kernel) находится в [opencl_utils.h](opencl_utils.h).

Функция сравнения описана в [main](main.cpp). Сравнение проводилось на данных из [Table1_Propane.csv](Table1_Propane.csv).


Были измерены время работы последовательной функции и kernel:
````
Seq time: 540.000000 ms
OpenCl kernel Execution time is: 2.633 ms
````


#### Note:
Команда для запуска исполняемого файла:
```
./main.exe -I "PATH_TO_OCL_SDK/include" -L "PATH_TO_OCL_SDK/lib/x86_64" -lOpenCl
```