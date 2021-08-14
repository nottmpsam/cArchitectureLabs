#include <iostream>
#include <ctime>
//msvc
int main()
{
	const int range = 4;
	int matrix[range][range];
	srand(time(0));
	for (int i = 0; i < range; ++i)
		for (int j = 0; j < range; ++j)
			matrix[i][j] = rand() % 20;
	std::cout << "Matrix: " << std::endl;
	for (int i = 0; i < range; ++i)
	{
		for (int j = 0; j < range; ++j)
		{
			std::cout << matrix[i][j] << " ";
		}
		std::cout << std::endl;
	}
	

	int temp0 = 0;
	int temp1 = 0;
	bool flag = false;
	int sum;
	_asm {
		; адрес начала массива
		lea ebx, matrix; для строк
		lea esi, matrix; для столбцов
		mov ecx, 0; счётчик

		L1 :
			mov edi, 0; начальное смещение элемента в строке
			mov eax, 0; eax будет содержать сумму элементов в строке
			mov edx, range; количество элементов в строке

		L2 :
			add eax, [ebx][edi]; прибавляем к eax элемент массива
			add edi, type sum; type sum = 4; смещаем edi на 4 байт
			dec edx; уменьшение счетчика внутри цикла L2
			cmp edx, 0; переход к L2, если edx не равно 0
			jne L2

		mov temp0, eax; запись в temp0, текущую сумму элементов в строке
		mov eax, 0; eax будет содержать сумму элементов столбцов

		add eax, [esi]; в eax добавляем первый элемент текущего столбца
		mov edx, range; счетчик внутри L3
		mov edi, type sum;

		mov temp1, ecx;
		shl temp1, 2; temp1 - количество байт, которое нужно прибавить к edi, чтобы получить нужный столбец

		L3 :
			cmp edx, range; если это первый проход, переходим к H1
			je H1
			mov edi, type sum; ;
			imul edi, range; обновим edi на 2 строку, 1 столбец.; далее, edi смещается на строку. (идем с конца, т.к.edx = range - 1)
			H1 :
				imul edi, edx; получили нужную строку(элемент указывает на 1 столбец)
				add edi, temp1; прибавляем нужное количество байт, чтобы получить нужный столбец
				add eax, matrix[edi]; прибавляем к eax элемент массива
				dec edx; уменьшение счетчика внутри L3
				cmp edx, 1; переход к L3, если edx не равно 1
				jne L3

		cmp eax, temp0; выход, если eax(сумма элементов в столбце) не равно сумме элементов в строке
		jne EXIT

		inc ecx; инкрементируем ecx
		cmp ecx, range; если ecx равен нашей размерности, то переходим к L5
		je L5

		; увеличиваем индексы у строк и столбцов :
		mov eax, range; загружаем количество элементов в строке
		imul eax, type sum; умножаем количество элементов в строке на размер элемента
		add ebx, eax; прибавляем к смещению(для строк) eax
		add esi, type sum; прибавляем к смещению(для столбцов) размер элемента(4 байт)
		jmp L1
		L5 :
			mov flag, 1; переключаем флаг в истину
			mov sum, eax; сохраняем в sum сумму
		EXIT :
	}
	if (!flag) {
		std::cout << "Matrix is not magic" << std::endl;
	}
	else {
		std::cout << "Matrix is magic" << std::endl;
		std::cout << "Every row and column is equal to " << sum << std::endl;
	}
}
