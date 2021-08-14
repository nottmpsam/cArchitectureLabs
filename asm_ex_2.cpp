#include <iostream>
//msvc
int main()
{
	const int RANGE = 5;
	int arr[RANGE] = {
		0b10101011,
		0b11101000,
		0b10110001,
		0b10101011,
		0b11101110 };
		
		
	int mask;
	const int REAL_BITS = 8;
	
	for (int i = 0; i < RANGE; ++i) {
		mask = 1 << (REAL_BITS - 1);
		while (mask) {
			std::cout << ((mask & arr[i]) >= 1 ? '1' : '0');
			mask >>= 1;
		}
		std::cout << " ~ " << arr[i] << std::endl;
	}
	int w{};
	int _index{};
	_asm
	{
		mov esi, REAL_BITS; запись в esi количество действующих битов
	    A0:
			mov ecx, esi
			dec esi ; сдвиг для первой строки равен n - 1
			A1 :
				cmp ecx, 0
				je R1
				lea ebx, arr; записываем в ebx адрес начала массива
				xor eax, eax; обнуляем eax (в нем мы будем хранить вес текущего столбца)
				dec ecx
				mov edx, RANGE; в edx будем хранить количество элементов в массиве 
				A2 :
					mov edi, 1; текущая маска
					shl edi, cl; смещаем edi на нужное количество бит 
					and edi, [ebx]
					cmp edi, 0
					je SKIPINC; если edi равен 0, то пропускаем инкременитирование
					inc eax
					SKIPINC :
					dec edx
					add ebx, type w ; переход к следующей строке (к элементу)
					cmp edx, 0
					jne A2; пока не пройдемся по всем элементам нашего массива

					cmp esi, ecx; инициализируем значение w, если это первый столбец (w хранит минимальный вес в столбцах)
					jne A4
					mov w, eax
					mov _index, ecx ; запоминаем индекс минимального столбца
					jmp A1 ; 
				A4 :
					cmp eax, w ; сравниваем значения w и значение в eax. если w больше, чем значение в eax - обновляем значение w
 					jg A1
					mov w, eax
					mov _index, ecx ; также запоминаем индекс минимального столбца
			jmp A1 ; возвращемся к флагу A1 (будем обрабатывать след. столбец)

			R1 :
				cmp esi, [_index] ; если индекс минимального столбца равен значени в esi, то переход к флагу R2
				je R2
				mov edx, RANGE ; в edx хранится количество элементов в массиве
				lea ebx, arr  ; записываем в ebx адрес начала массива
				R3 :
					mov edi, 1 ; маска, сдвигается на индекс минимального столбца
					mov eax, 1 ; маска, сдвигается на индекс в esi (индекс, в котором должен стоять наш минимальный столбец)
					mov ecx, _index
					shl  edi, cl
					and edi, [ebx]
					mov ecx, esi
					shl eax, cl
					and eax, [ebx]
					cmp eax, edi ; если значения в eax, edi равны 0, то  переход к флагу R4
					je R4
					cmp eax, 1 ; если значения в eax, edi >= 1, то переход к флагу R4
					jl R5
					cmp edi, 1
					jl R5
					jmp R4
				R5 : 
					; в флаге R5 меняем биты местами
					mov eax, 1 
					shl eax, cl
					xor [ebx], eax
					mov eax, 1
					mov ecx, _index
					shl eax, cl
					xor [ebx], eax
				R4 :
					add ebx, type w ; 
					dec edx 
					cmp edx, 0 ; если в edx = 0 - выход
					jne R3
				R2 :
					cmp esi, 0 ; если рассмотрели все столбцы, то мы закончили.
					je EXIT
					jmp A0 ; иначе переход к флагу A0
	EXIT :
	}
	std::cout << "sorted..." << std::endl;
	for (int i = 0; i < RANGE; ++i) {
		mask = 1 << (REAL_BITS - 1);
		while (mask) {
			std::cout << ((mask & arr[i]) >= 1 ? '1' : '0');
			mask >>= 1;
		}
		std::cout << " ~ " << arr[i] << std::endl;
	}
	return 0;
}
