#pragma once

#include <vector>
#include <future>
#include <type_traits>


template<typename T, typename Dummy = std::enable_if_t<std::is_arithmetic_v<T>>>
class Matrix {
public:
	Matrix() = default;

	explicit Matrix(size_t rows, size_t columns);
	explicit Matrix(std::vector<std::vector<T>>&& vector);
	
	Matrix(const Matrix& matrix) = default;
	Matrix(Matrix&& matrix) = default;

	Matrix& operator=(const Matrix& matrix) = default;
	Matrix& operator=(Matrix&& matrix) noexcept = default;
	
	T& at(const size_t row, const size_t column) {
		return data_[row][column];
	}
	
	const T& at(const size_t row, const size_t column) const {
		return data_.at(row).at(column);
	}

	static void change_thread_count(const size_t count) {
		is_default_threads_ = false;
		thread_count_ = count;
	}

	static void set_default_thread_count() {
		is_default_threads_ = true;
	}
	
	const std::vector<std::vector<T>>& data() const {
		return data_;
	}

	size_t rows() const {
		return data_.size();
	}

	size_t columns() const {
		return data_.empty() ? 0 : data_.front().size();
	}

	std::pair<size_t, size_t> shape() const {
		return std::make_pair(rows(), columns());
	}

	Matrix operator+(const Matrix& matrix) const;
	Matrix operator-(const Matrix& matrix) const;
	Matrix operator*(const Matrix& matrix) const;

	Matrix& operator+=(const Matrix& matrix);
	Matrix& operator-=(const Matrix& matrix);
	Matrix& operator*=(const Matrix& matrix);

	using det_t = double;
	det_t det() const;

	bool operator==(const Matrix& matrix) const {
		return data_ == matrix.data_;
	}

	bool operator!=(const Matrix& matrix) const {
		return data_ != matrix.data_;
	}

	void clear() {
		data_.clear();
	}

	void swap_rows(const size_t i, const size_t j) {
		std::swap(data_[i], data_[j]);
	}

	size_t col_max(size_t column) const;
	
	~Matrix() = default;
private:
	std::vector<std::vector<T>> data_;
	
	static inline size_t thread_count_ = 8;
	static inline bool is_default_threads_ = true;
	enum class oper_type { add, sub, mult, det };
	static void determine_thread_count(oper_type type, std::pair<size_t, size_t> shape);

	static void set_operation_threads(void (*operation)(const Matrix&, const Matrix&, Matrix&, size_t, size_t),
		const Matrix& left, const Matrix& right, Matrix& result);

	static void set_det_threads(const Matrix& init_matrix, det_t& det);
	static void set_double_copy_threads(const Matrix& init_matrix, Matrix<double>& matrix);

	static void add(const Matrix& left, const Matrix& right, Matrix& result, size_t begin, size_t end);
	static void subtract(const Matrix& left, const Matrix& right, Matrix& result, size_t begin, size_t end);
	static void multiply(const Matrix& left, const Matrix& right, Matrix& result, size_t begin, size_t end);
	static void triangulation(Matrix<double>& matrix, size_t current, size_t begin, size_t end);

	static void double_copy(const Matrix& init_matrix, Matrix<double>& result, size_t begin, size_t end);
};


template <typename T, typename Dummy>
Matrix<T, Dummy>::Matrix(const size_t rows, const size_t columns)
{
	data_.resize(rows);
	for (auto& i : data_) {
		i.resize(columns);
	}
}


template <typename T, typename Dummy>
Matrix<T, Dummy>::Matrix(std::vector<std::vector<T>>&& vector)
{
	if (vector.empty()) {
		throw::std::invalid_argument("Empty matrix\n");
	}
	auto col = vector.front().size();
	for (auto& i : vector) {
		if (i.size() != col) {
			throw::std::invalid_argument("Incorrect matrix shapes\n");
		}
	}
	data_ = std::move(vector);
}



template <typename T, typename Dummy>
Matrix<T, Dummy> Matrix<T, Dummy>::operator+(const Matrix& matrix) const
{
	Matrix result(*this);
	if (shape() != matrix.shape()) {
		throw::std::invalid_argument("Different matrix shapes\n");
	}

	determine_thread_count(oper_type::add, shape());
	set_operation_threads(add, *this, matrix, result);
	return result;
}


template <typename T, typename Dummy>
Matrix<T, Dummy> Matrix<T, Dummy>::operator-(const Matrix& matrix) const
{
	Matrix result(*this);
	if (shape() != matrix.shape()) {
		throw::std::invalid_argument("Different matrix shapes\n");
	}

	determine_thread_count(oper_type::sub, shape());
	set_operation_threads(subtract, *this, matrix, result);
	return result;
}


template <typename T, typename Dummy>
Matrix<T, Dummy> Matrix<T, Dummy>::operator*(const Matrix& matrix) const
{
	Matrix result(this->rows(), matrix.columns());
	if (columns() != matrix.rows()) {
		throw::std::invalid_argument("Incorrect matrix shapes\n");
	}

	determine_thread_count(oper_type::mult, result.shape());
	set_operation_threads(multiply, *this, matrix, result);
	return result;
}


template <typename T, typename Dummy>
Matrix<T, Dummy>& Matrix<T, Dummy>::operator+=(const Matrix& matrix)
{
	if (shape() != matrix.shape()) {
		throw::std::invalid_argument("Different matrix shapes\n");
	}

	determine_thread_count(oper_type::add, shape());
	set_operation_threads(add, matrix, matrix, *this);
	return *this;
}


template <typename T, typename Dummy>
Matrix<T, Dummy>& Matrix<T, Dummy>::operator-=(const Matrix& matrix)
{
	if (shape() != matrix.shape()) {
		throw::std::invalid_argument("Different matrix shapes\n");
	}

	determine_thread_count(oper_type::sub, shape());
	set_operation_threads(subtract, matrix, matrix, *this);
	return *this;
}


template <typename T, typename Dummy>
Matrix<T, Dummy>& Matrix<T, Dummy>::operator*=(const Matrix& matrix)
{
	Matrix result(this->rows(), matrix.columns());
	if (columns() != matrix.rows()) {
		throw::std::invalid_argument("Incorrect matrix shapes\n");
	}

	determine_thread_count(oper_type::mult, result.shape());
	set_operation_threads(multiply, *this, matrix, result);
	*this = result;
	return *this;
}


template <typename T, typename Dummy>
typename Matrix<T, Dummy>::det_t Matrix<T, Dummy>::det() const
{
	if (rows() != columns() || rows() == 0) {
		throw std::invalid_argument("Invalid matrix shapes\n");
	}
	if (rows() == 2) {
		return static_cast<det_t>(data_[0][0]) * static_cast<det_t>(data_[1][1])
				- static_cast<det_t>(data_[0][1]) * static_cast<det_t>(data_[1][0]);
	}
	det_t det = 1;
	determine_thread_count(oper_type::det, shape());
	set_det_threads(*this, det);
	
	return det;
}


template <typename T, typename Dummy>
void Matrix<T, Dummy>::double_copy(const Matrix& init_matrix, Matrix<double>& result, const size_t begin, const size_t end)
{
	for (auto i = begin; i < end; ++i) {
		for (size_t j = 0; j < result.columns(); ++j) {
			result.at(i, j) = static_cast<double>(init_matrix.at(i, j));
		}
	}
}


template <typename T, typename Dummy>
size_t Matrix<T, Dummy>::col_max(const size_t column) const
{
	T max = std::abs(data_[column][column]);
	auto max_pos = column;
	for (auto i = column + 1; i < rows(); ++i) {
		T element = std::abs(data_[i][column]);
		if (element > max) {
			max = element;
			max_pos = i;
		}
	}
	return max_pos;
}


template <typename T, typename Dummy>
void Matrix<T, Dummy>::determine_thread_count(oper_type type, const std::pair<size_t, size_t> shape)
{
	if (is_default_threads_) {
		const auto& [rows, columns] = shape;
		const auto mean_size = (rows + columns) / 2;
		switch (type) {
		case oper_type::mult:
			if (mean_size < 25)
				thread_count_ = 1;
			else
				thread_count_ = 4;
			break;

		case oper_type::det:
			if (mean_size < 400)
				thread_count_ = 1;
			else
				thread_count_ = 4;
		default:
			if (mean_size < 200)
				thread_count_ = 1;
			else if (std::is_same_v<T, double>) {
				thread_count_ = 5;
			}
			else
				thread_count_ = 6;
		}
		
	}
	thread_count_ = thread_count_ > std::thread::hardware_concurrency()
		? std::thread::hardware_concurrency() : thread_count_;
}


template <typename T, typename Dummy>
void Matrix<T, Dummy>::set_operation_threads(void (*operation)(const Matrix&, const Matrix&, Matrix&, size_t, size_t),
	const Matrix& left, const Matrix& right, Matrix& result)
{	
	std::vector<std::future<void>> threads;

	for (size_t i = 0; i < thread_count_ - 1; ++i) {
		auto begin = static_cast<size_t>(i * left.rows() / thread_count_);
		auto end = static_cast<size_t>((i + 1) * left.rows() / thread_count_);

		threads.push_back(std::async(operation, std::ref(left), std::ref(right), std::ref(result), begin, end));
	}
	const auto begin = static_cast<size_t>((thread_count_ - 1) * left.rows() / thread_count_);
	operation(left, right, result, begin, left.rows());

	for (auto& i : threads) {
		i.get();
	}
}


template <typename T, typename Dummy>
void Matrix<T, Dummy>::set_det_threads(const Matrix& init_matrix, det_t& det)
{
	Matrix<double> matrix(init_matrix.rows(), init_matrix.columns());
	if constexpr (!std::is_same_v<T, double>) {
		set_double_copy_threads(init_matrix, matrix);
	}
	else {
		matrix = init_matrix;
	}

	auto sgn = 1;
	for (size_t i = 0; i < matrix.rows() - 1; ++i) {
		const auto imax = matrix.col_max(i);
		if (std::abs(matrix.at(imax, i)) < std::numeric_limits<double>::epsilon()) {
			det = 0;
			break;
		}
		if (i != imax) {
			sgn *= -1;
			matrix.swap_rows(i, imax);
		}

		std::vector<std::future<void>> threads;
		double n = static_cast<double>(matrix.rows() - i - 1) / static_cast<double>(thread_count_);
		for (size_t j = 0; j < thread_count_; j++) {
			auto begin = static_cast<size_t>(j * n + 1 + i);
			auto end = static_cast<size_t>((j + 1) * n + 1 + i);

			if (j < thread_count_ - 1) {
				threads.push_back(std::async(triangulation, std::ref(matrix), i, begin, end));
			}
			else {
				triangulation(matrix, i, begin, end);
			}
		}
		for (auto& j : threads) {
			j.get();
		}
	}

	
	if (det == 0) {
		return;
	}
	det = 1;
	for (size_t i = 0; i < matrix.rows(); ++i) {
		det *= matrix.at(i, i);
	}
	det *= sgn;
}


template <typename T, typename Dummy>
void Matrix<T, Dummy>::set_double_copy_threads(const Matrix& init_matrix, Matrix<double>& matrix)
{
	std::vector<std::future<void>> threads;
	for (size_t i = 0; i < thread_count_ - 1; ++i) {
		auto begin = static_cast<size_t>(i * matrix.rows() / thread_count_);
		auto end = static_cast<size_t>((i + 1) * matrix.rows() / thread_count_);

		threads.push_back(std::async(double_copy, std::ref(init_matrix), std::ref(matrix), begin, end));
	}
	const auto begin = static_cast<size_t>((thread_count_ - 1) * matrix.rows() / thread_count_);
	double_copy(init_matrix, matrix, begin, matrix.rows());

	for (auto& i : threads) {
		i.get();
	}
}


template <typename T, typename Dummy>
void Matrix<T, Dummy>::add(const Matrix& left, const Matrix& right, Matrix& result, const size_t begin, const size_t end)
{
	for (auto i = begin; i < end; ++i) {
		for (size_t j = 0; j < left.columns(); ++j) {
			result.data_[i][j] += right.data_[i][j];
		}
	}
}


template <typename T, typename Dummy>
void Matrix<T, Dummy>::subtract(const Matrix& left, const Matrix& right, Matrix& result, const size_t begin, const size_t end)
{
	for (auto i = begin; i < end; ++i) {
		for (size_t j = 0; j < left.columns(); ++j) {
			result.data_[i][j] -= right.data_[i][j];
		}
	}
}


template <typename T, typename Dummy>
void Matrix<T, Dummy>::multiply(const Matrix& left, const Matrix& right, Matrix& result, const size_t begin, const size_t end)
{
	for (auto i = begin; i < end; ++i) {
		for (size_t j = 0; j < right.columns(); ++j) {
			for (size_t k = 0; k < left.columns(); ++k) {
				result.data_[i][j] += left.data_[i][k] * right.data_[k][j];
			}
		}
	}
}


template <typename T, typename Dummy>
void Matrix<T, Dummy>::triangulation(Matrix<double>& matrix, const size_t current, const size_t begin, const size_t end)
{
	for (auto j = begin; j < end; ++j) {
		const auto mul = -matrix.at(j, current) / matrix.at(current, current);
		for (auto k = current; k < matrix.rows(); ++k) {
			matrix.at(j, k) += matrix.at(current, k) * mul;
		}
	}
}