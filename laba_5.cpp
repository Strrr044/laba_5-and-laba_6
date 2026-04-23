#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cstdlib>
#include <ctime>

using namespace std;

class MyVector {
private:
	int* data;
	int size;
	int capacity;

	void EnsureCapacity(int requiredCapacity) {
		if (requiredCapacity <= capacity) return;
		int newCapacity = capacity * 3 / 2 + 1;
		int* newData = new int[newCapacity]();
		for (int i = 0; i < size; ++i)
			newData[i] = data[i];
		delete[] data;
		data = newData;
		capacity = newCapacity;
	}




public:
	//кон по умолчанию
	MyVector() : size(0), capacity(10) {
		data = new int[capacity]();
	}

	//кон
	MyVector(int capa) : size(0), capacity(capa) {
		data = new int[capacity]();
	}

	size_t getSize() const {
		return size;

	}

	size_t getCapacity() const {
		return capacity;
	}

	void print() const {
		for (int i = 0; i < size; i++) {
			cout << data[i] << " ";
		}
	}
	// Добавление в конец
	void PushBack(int value) {
		EnsureCapacity(size + 1);
		data[size++] = value;
	}
	// Добавление в конец
	void PushFront(int value) {
		EnsureCapacity(size + 1);
		for (int i = size; i > 0; --i)
			data[i] = data[i - 1];
		data[0] = value;
		++size;
	}
	// Вставка по индексу
	void Insert(int index, int value) {
		if (index <0 || index > size) {
			cout << "Неверный индекс!" << endl;
			return;
		}
		EnsureCapacity(size + 1);
		for (int i = size; i > index; --i)
			data[i] = data[i - 1];
		data[index] = value;
		++size;
	}
	//удаление по индексу
	void RemoveAt(int index) {
		if (index < 0 || index >= size) {
			cout << "Неверный индекс!" << endl;
			return;
		}
		for (int i = index; i < size - 1; ++i)
			data[i] = data[i + 1];
		--size;
	}
	// Удаление всех элементов с указанным значением
	void Remove(int value) {
		int windex = 0;
		for (int i = 0; i < size; i++) {
			if (data[i] != value)
				data[windex++] = data[i];
		}
		size = windex;
	}
	//удаление первого элемента из массива
	void PopFront() {
		if (size > 0)
			RemoveAt(0);

	}
	// Удаление последнего элемента
	void PopBack() {
		if (size > 0)
			size--;
	}
	//обнуление массива
	void Clear() {
		for (int i = 0; i < size; i++) {
			data[i] = 0;
		}
		size = 0;
	}
	// Проверка на пустоту
	bool IsEmpty() {
		return size == 0;

	}
	// Подогнать capacity под size
	void TrimToSize() {
		if (size == capacity) return;
		int newCapacity;
		if (size > 0)
			newCapacity = size;
		else
			newCapacity = 0;
		int* newData = new int[newCapacity]();
		for (int i = 0; i < size; ++i)
			newData[i] = data[i];
		delete[] data;
		data = newData;
		capacity = newCapacity;
	}
	//линейный поиск слева направо первого вхождения
	int IndexOf(int value) {
		for (int i = 0; i < size; ++i)
			if (data[i] == value) return i;
		return -1;
	}
	//// Линейный поиск справа налево
	int LastIndexOf(int value) {
		for (int i = size - 1; i >= 0; --i)
			if (data[i] == value) return i;
		return -1;
	}
	// Обратный порядок элементов
	void Reverse() {
		for (int i = 0; i < size / 2; i++) {
			int temp = data[i];
			data[i] = data[size - 1 - i];
			data[size - 1 - i] = temp;
		}
	}
	// Сортировка по возрастанию
	void SortAsc() {
		sort(data, data + size);
	}

	// Сортировка по убыванию
	void SortDesc() {
		sort(data, data + size, greater<int>());
	}

	// Случайное перемешивание
	void Shuffle() {
		for (int i = size - 1; i > 0; i--) {
			int j = rand() % (i + 1);
			int temp = data[i];
			data[i] = data[j];
			data[j] = temp;
		}
	}
	// Заполнение случайными значениями
	void RandomFill(int count, int minval, int maxval) {
		EnsureCapacity(count);
		for (int i = 0; i < count; i++) {
			data[i] = minval + rand() % (maxval - minval + 1);
		}
		size = count;
	}
	// Сравнение двух векторов
	bool Equals(const MyVector& other) const {
		if (size != other.size) return false;
		for (int i = 0; i < size; i++) {
			if (data[i] != other.data[i]) return false;
		}
		return true;
	}
	// Получение элемента по индексу
	int GetElementAt(int index) {
		if (index < 0 || index >= size) {
			throw out_of_range("Неверный индекс!");
		}
		return data[index];

	}
	//копирование
	MyVector* Clone() {
		MyVector* newVector = new MyVector(capacity);
		newVector->size = size;
		for (int i = 0; i < size; i++) {
			newVector->data[i] = data[i];
		}
		return newVector;
	}
	//конст коп
	MyVector(const MyVector& other) {
		size = other.size;
		capacity = other.capacity;
		data = new int[capacity]();

		for (int i = 0; i < size; i++) {
			data[i] = other.data[i];
		}
	}

	// Деструктор
	~MyVector() {
		delete[] data;
	}
	//=
	MyVector& operator=(const MyVector& other) {
		if (this == &other) return *this;
		delete[] data;
		size = other.size;
		capacity = other.capacity;
		data = new int[capacity]();
		for (int i = 0; i < size; i++) {
			data[i] = other.data[i];
		}
		return *this;
	}
	//[]
	int& operator[](int index) {
		if (index < 0 || index >= size) {
			throw out_of_range("Неверный индекс!");
		}
		return data[index];

	}
	//==
	bool operator==(const MyVector& other) const {
		if (size != other.size) return false;
		for (int i = 0; i < size; i++) {
			if (data[i] != other.data[i]) return false;
		}
		return true;
	}
	//  >>
	friend istream& operator>>(istream& in, MyVector& vec) {
		int n;
		cout << "Введите количество элементов: ";
		in >> n;
		vec.Clear();
		vec.EnsureCapacity(n);
		cout << "Введите " << n << " элементов:" << endl;
		for (int i = 0; i < n; i++) {
			int val;
			in >> val;
			vec.PushBack(val);
		}
		return in;
	}

	// <<
	friend ostream& operator<<(ostream& out, const MyVector& vec) {
		out << "[";
		for (int i = 0; i < vec.size; i++) {
			out << vec.data[i];
			if (i < vec.size - 1) out << ", ";
		}
		out << "]";
		return out;
	}

};


int main() {
	setlocale(LC_ALL, "");
	srand(time(0));

	MyVector v;

	cout << "\n~~~~ PushBack ~~~~\n";
	v.PushBack(10);
	v.PushBack(20);
	v.PushBack(30);
	v.PushBack(30);
	cout << v << "\n";

	cout << "\n~~~~ PushFront ~~~~\n";
	v.PushFront(5);
	cout << v << "\n";

	cout << "\n~~~~ Insert ~~~~\n";
	v.Insert(2, 99);
	cout << v << "\n";

	cout << "\n~~~~ RemoveAt ~~~~\n";
	v.RemoveAt(2);
	cout << v << "\n";

	cout << "\n~~~~ Remove ~~~~\n";
	v.Remove(30);
	cout << v << "\n";

	cout << "\n~~~~ PopFront ~~~~\n";
	v.PopFront();
	cout << v << "\n";

	cout << "\n~~~~ PopBack ~~~~\n";
	v.PopBack();
	cout << v << "\n";

	cout << "\n~~~~ Clear + IsEmpty ~~~~\n";
	v.Clear();
	cout << (v.IsEmpty() ? "Empty" : "Not empty") << "\n";

	// восстановим данные для дальнейших тестов
	v.PushBack(3);
	v.PushBack(1);
	v.PushBack(2);
	v.PushBack(2);
	v.PushBack(5);

	cout << "\n~~~~ TrimToSize ~~~~\n";
	v.TrimToSize();
	cout << v << "\n";

	cout << "\n~~~~ IndexOf / LastIndexOf ~~~~\n";
	cout << "IndexOf(2): " << v.IndexOf(2) << "\n";
	cout << "LastIndexOf(2): " << v.LastIndexOf(2) << "\n";

	cout << "\n~~~~ Reverse ~~~~\n";
	v.Reverse();
	cout << v << "\n";

	cout << "\n~~~~ SortAsc ~~~~\n";
	v.SortAsc();
	cout << v << "\n";

	cout << "\n~~~~ SortDesc ~~~~\n";
	v.SortDesc();
	cout << v << "\n";

	cout << "\n~~~~ Shuffle ~~~~\n";
	v.Shuffle();
	cout << v << "\n";

	cout << "\n~~~~ RandomFill ~~~~\n";
	MyVector r;
	r.RandomFill(5, 1, 10);
	cout << r << "\n";

	cout << "\n~~~~ Equals + operator== ~~~~\n";
	MyVector v2 = v;
	cout << (v == v2 ? "Equal" : "Not equal") << "\n";

	cout << "\n~~~~ GetElementAt ~~~~\n";
	try {
		cout << v.GetElementAt(1) << "\n";
	}
	catch (exception& e) {
		cout << e.what() << "\n";
	}

	cout << "\n~~~~ Clone ~~~~\n";
	MyVector* c = v.Clone();
	cout << *c << "\n";
	delete c;

	cout << "\n~~~~ operator[] ~~~~\n";
	cout << "v[0] = " << v[0] << "\n";

	cout << "\n~~~~ operator= ~~~~\n";
	MyVector a;
	a = v;
	cout << a << "\n";

	cout << "\n~~~~ operator>> ~~~~\n";
	MyVector input;
	cin >> input;
	cout << input << "\n";

	return 0;
}
