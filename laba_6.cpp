#include <iostream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <cmath>

using namespace std;

class Matrix {
private:
	int rows, cols;
	vector<vector<double>> data;
	//вырезает столбец и строку для нахождения определителя
	vector<vector<double>> getMinor(
		const vector<vector<double>>& m, int r, int c) const {
		std::vector<vector<double>> result;
		for (int i = 0; i < m.size(); i++) {
			if (i == r) continue;
			vector<double> row;
			for (int j = 0; j < m.size(); j++) {
				if (j == c) continue;
				row.push_back(m[i][j]);
			}
			result.push_back(row);
		}
		return result;
	}
	//считает определитель
	double det(const vector<vector<double>>& m) const {
		int n = m.size();
		if (n == 1) return m[0][0];
		if (n == 2) return m[0][0] * m[1][1] - m[0][1] * m[1][0];

		double result = 0.0;
		for (int j = 0; j < n; j++) {
			double sign = (j % 2 == 0) ? 1 : -1;
			result += sign * m[0][j] * det(getMinor(m, 0, j));
		}
		return result;
	}
	//матрица кофакторов(вспомогательная функция для нахождения обратной матрицы)
	Matrix cofactors() const {
		Matrix result(rows, cols);
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++) {
				double sign = ((i + j) % 2 == 0) ? 1 : -1;
				result.data[i][j] = sign * det(getMinor(data, i, j));
			}
		return result;
	}

public:
	//конст
	Matrix(int m, int n, bool random = false, double min = -10.0, double max = 10.0)
		: rows(m), cols(n), data(m, vector<double>(n, 0.0)) {

		if (m <= 0 || n <= 0)
			throw invalid_argument("Размер матрицы должен быть положительным");

		if (!random) {
			return; // остаются нули
		}

		static bool generatorStarted = false;
		if (!generatorStarted) {
			srand(time(nullptr));
			generatorStarted = true;
		}

		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				data[i][j] = min + (double)rand() / RAND_MAX * (max - min);
	}

	// ГЕТТЕРЫ
	int getRows() const {
		return rows;
	}
	int getCols() const {
		return cols;
	}
	//вывод
	void print() const {
		for (int i = 0; i < rows; i++) {
			std::cout << "| ";
			for (int j = 0; j < cols; j++) {
				cout << setw(8) << fixed << setprecision(2) << data[i][j] << " ";
			}
			cout << "|\n";
		}
	}
	//сложение
	Matrix operator+(const Matrix& other) const {
		if (rows != other.rows || cols != other.cols) {
			throw invalid_argument("Матрицы должны быть одного размера");
		}

		Matrix result(rows, cols);
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				result.data[i][j] = data[i][j] + other.data[i][j];
		return result;
	}
	//вычитание
	Matrix operator-(const Matrix& other) const {
		if (rows != other.rows || cols != other.cols)
			throw invalid_argument("Матрицы должны быть одного размера");

		Matrix result(rows, cols);
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				result.data[i][j] = data[i][j] - other.data[i][j];
		return result;
	}
	//умножение
	Matrix operator*(const Matrix& other) const {
		if (cols != other.rows)
			throw invalid_argument("Число столбцов первой матрицы должно равняться числу строк второй");

		Matrix result(rows, other.cols);
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < other.cols; j++)
				for (int k = 0; k < cols; k++)
					result.data[i][j] += data[i][k] * other.data[k][j];
		return result;
	}
	//умножение на скаляр
	Matrix operator*(double scalar) const {
		Matrix result(rows, cols);
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				result.data[i][j] = data[i][j] * scalar;
		return result;
	}

	//деление на скаляр
	Matrix operator/(double scalar) const {
		if (scalar == 0)
			throw invalid_argument("Деление на ноль");

		Matrix result(rows, cols);
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				result.data[i][j] = data[i][j] / scalar;
		return result;
	}
	//определитель
	double determinant() const {
		if (rows != cols)
			throw invalid_argument("Определитель: матрица должна быть квадратной");
		return det(data);
	}
	//транспонирование
	Matrix transpose() const {
		Matrix result(cols, rows);
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				result.data[j][i] = data[i][j];
		return result;
	}
	//обратная матрица 
	Matrix inverse() const {
		if (rows != cols)
			throw invalid_argument("Обратная матрица: матрица должна быть квадратной");
		double d = determinant();
		if (d == 0)
			throw invalid_argument("Обратная матрица не существует, определитель = 0");
		return cofactors().transpose() * (1.0 / d);
	}
	//деление на матрицу
	Matrix operator/(const Matrix& other) const {
		return *this * other.inverse();
	}
	//Комбинированные операции присваивания (+=, -=, *=, /=)
	Matrix& operator+=(const Matrix& other) {
		*this = *this + other;
		return *this;
	}

	Matrix& operator-=(const Matrix& other) {
		*this = *this - other;
		return *this;
	}

	Matrix& operator*=(const Matrix& other) {
		*this = *this * other;
		return *this;
	}

	Matrix& operator/=(const Matrix& other) {
		*this = *this / other;
		return *this;
	}

	// и для скаляров
	Matrix& operator*=(double scalar) {
		*this = *this * scalar;
		return *this;
	}

	Matrix& operator/=(double scalar) {
		*this = *this / scalar;
		return *this;
	}

	//Операции сравнения на равенство/неравенство
	bool operator==(const Matrix& other) const {
		if (rows != other.rows || cols != other.cols)
			return false;

		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				if (data[i][j] != other.data[i][j])
					return false;

		return true;
	}

	bool operator!=(const Matrix& other) const {
		return !(*this == other);
	}
	//возведение в степень
	Matrix stepen(int n) const {
		if (rows != cols)
			throw invalid_argument("Возведение в степень только для квадратной матрицы");

		if (n < 0)
			throw invalid_argument("Степень должна быть >= 0");

		if (n == 0) {
			Matrix result(rows, cols, 0, 0);
			for (int i = 0; i < rows; i++)
				result.data[i][i] = 1;
			return result;
		}

		Matrix result = *this;

		for (int i = 1; i < n; i++)
			result = result * (*this);

		return result;
	}
	//норма
	double norm() const {
		double sum = 0.0;

		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				sum += data[i][j] * data[i][j];

		return sqrt(sum);
	}
	//проверка типа матрицы
	//квадратная матрица
	bool isSquare() const {
		return rows == cols;
	}
	//нулевая матрица
	bool isZero() const {
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				if (data[i][j] != 0)
					return false;

		return true;
	}
	//единичная матрица
	bool isIdentity() const {
		if (!isSquare())
			return false;

		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++) {
				if (i == j && data[i][j] != 1)
					return false;

				if (i != j && data[i][j] != 0)
					return false;
			}

		return true;
	}
	//диагональная матрица
	bool isDiagonal() const {
		if (!isSquare())
			return false;

		for (int i = 0; i < rows; i++)
			for (int j = 0; j < cols; j++)
				if (i != j && data[i][j] != 0)
					return false;

		return true;
	}
	//симметричная матрица
	bool isSymmetric() const {
		if (!isSquare())
			return false;

		for (int i = 0; i < rows; i++)
			for (int j = i + 1; j < cols; j++)
				if (data[i][j] != data[j][i])
					return false;

		return true;
	}
	//верхняя треугольная 
	bool isUpperTriangular() const {
		if (!isSquare())
			return false;

		for (int i = 1; i < rows; i++)
			for (int j = 0; j < i; j++)
				if (data[i][j] != 0)
					return false;

		return true;
	}
	//нижняя треугольная 
	bool isLowerTriangular() const {
		if (!isSquare())
			return false;

		for (int i = 0; i < rows; i++)
			for (int j = i + 1; j < cols; j++)
				if (data[i][j] != 0)
					return false;

		return true;
	}
	friend ostream& operator<<(ostream& out, const Matrix& m);
	friend istream& operator>>(istream& in, Matrix& m);




};
ostream& operator<<(ostream& out, const Matrix& m) {
	for (int i = 0; i < m.rows; i++) {
		for (int j = 0; j < m.cols; j++)
			out << setw(8) << fixed << setprecision(2) << m.data[i][j] << " ";
		out << endl;
	}
	return out;
}

istream& operator>>(istream& in, Matrix& m) {
	for (int i = 0; i < m.rows; i++)
		for (int j = 0; j < m.cols; j++)
			in >> m.data[i][j];

	return in;
}


int main() {
	setlocale(LC_ALL, "Russian");

	int r, c;

	cout << "Введите размер матрицы A (строки столбцы) [будет заполнена случайными числами]: ";
	cin >> r >> c;
	Matrix A(r, c, true);   // случайная

	cout << "Введите размер матрицы B (строки столбцы): ";
	cin >> r >> c;
	Matrix B(r, c, false);  // пустая (для ввода)

	cout << "Введите матрицу B:\n";
	cin >> B;

	int choice;

	do {
		cout << "\n===== МЕНЮ =====\n";
		cout << "1. Показать матрицы A и B\n";
		cout << "2. A + B\n";
		cout << "3. A - B\n";
		cout << "4. A * B\n";
		cout << "5. A / B\n";
		cout << "6. A в степень\n";
		cout << "7. Определитель A\n";
		cout << "8. Обратная A\n";
		cout << "9. Норма A\n";
		cout << "10. Проверка типа A\n";
		cout << "11. A * число (скаляр)\n";
		cout << "12. A / число (скаляр)\n";
		cout << "0. Выход\n";
		cout << "Выбор: ";
		cin >> choice;

		try {
			switch (choice) {

			case 1:
				cout << "\nA:\n" << A;
				cout << "\nB:\n" << B;
				break;

			case 2:
				cout << "\nA + B:\n" << (A + B);
				break;

			case 3:
				cout << "\nA - B:\n" << (A - B);
				break;

			case 4:
				cout << "\nA * B:\n" << (A * B);
				break;

			case 5:
				cout << "\nA / B:\n" << (A / B);
				break;

			case 6: {
				int n;
				cout << "Введите степень: ";
				cin >> n;
				cout << "\nA^" << n << ":\n" << A.stepen(n);
				break;
			}

			case 7:
				cout << "det(A) = " << A.determinant() << endl;
				break;

			case 8:
				cout << "\nA^-1:\n" << A.inverse();
				break;

			case 9:
				cout << "||A|| = " << A.norm() << endl;
				break;

			case 10:
				cout << "Является типом(1)||Не является типом(0)" << endl;
				cout << "Квадратная матрица: " << A.isSquare() << endl;
				cout << "Нулевая матрица: " << A.isZero() << endl;
				cout << "Единичная матрица: " << A.isIdentity() << endl;
				cout << "Диагональная матрица: " << A.isDiagonal() << endl;
				cout << "Симметричная матрица: " << A.isSymmetric() << endl;
				cout << "Верхняя треугольная матрица: " << A.isUpperTriangular() << endl;
				cout << "Нижняя треугольная матрица: " << A.isLowerTriangular() << endl;
				break;
			case 11: {
				double k;
				cout << "Введите скаляр: ";
				cin >> k;
				cout << "\nA * " << k << ":\n" << (A * k);
				break;
			}

			case 12: {
				double k;
				cout << "Введите скаляр: ";
				cin >> k;
				cout << "\nA / " << k << ":\n" << (A / k);
				break;
			}

			case 0:
				cout << "Выход...\n";
				break;

			default:
				cout << "Неверный выбор!\n";
			}
		}
		catch (exception& e) {
			cout << "Ошибка: " << e.what() << endl;
		}

	} while (choice != 0);

	return 0;
}
