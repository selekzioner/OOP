#include "Matrix.hpp"
#include <iostream>
#include <iomanip>
#include <random>
#include <chrono>


template<typename T>
Matrix<T> diagonal(size_t n, T diag = T{ 1 }, T val = T{ 0 })
{
	Matrix<T> m{ n, n };
	const auto& [row, col] = m.shape();
	for (size_t i = 0; i < row; i++) {
		for (size_t k = 0; k < col; k++) {
			m.at(i, k) = i == k ? diag : val;
		}
	}
	return m;
}

template<typename T>
void sum_test(std::ostream& stream)
{
	stream << "Sum:" << typeid(T).name() << std::endl;
	auto threads = { 1u, 2u, 3u, 4u, 5u, 6u };
	auto dimensions = { 100, 150, 200, 300, 500, 700, 1000, 2000, 5000 };
	stream << std::setw(6) << "dim/th" << ";";
	for (auto th : threads) {
		stream << std::setw(12) << th << ";";
	}
	stream << std::endl;
	stream << std::setprecision(7);
	for (auto dim : dimensions) {
		auto d1 = diagonal<T>(dim);
		auto d2 = diagonal<T>(dim);
		auto result = diagonal<T>(dim, 2);
		const auto& [row, col] = d1.shape();
		stream << std::setw(6) << col << ";"; stream.flush();
		for (auto th : threads) {
			d1.change_thread_count(th);
			auto duration = std::numeric_limits<double>::max();
			const auto cnt = dim < 10000 ? 10 : 1;
			const auto prev = d1;
			for (auto i = 0; i < cnt; ++i) {
				auto start = std::chrono::high_resolution_clock::now();
				d1 += d2;
				duration = std::min(duration, std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start).count());
				if (d1 != result) {
					stream << std::endl << "ERROR: calculation"; stream.flush();
					break;
				}
				d1 = prev;
			}
			stream << std::setw(12) << std::fixed << duration << ";"; stream.flush();
		}
		stream << std::endl;
	}
}


template<typename T>
void substract_test(std::ostream& stream)
{
	stream << "Substract:" << typeid(T).name() << std::endl;
	auto threads = { 1u, 2u, 3u, 4u, 5u, 6u };
	auto dimensions = { 100, 150, 200, 300, 500, 700, 1000, 2000, 5000 };
	stream << std::setw(6) << "dim/th" << ";";
	for (auto th : threads) {
		stream << std::setw(12) << th << ";";
	}
	stream << std::endl;
	stream << std::setprecision(7);
	for (auto dim : dimensions) {
		auto d1 = diagonal<T>(dim, 2);
		auto d2 = diagonal<T>(dim);
		auto result = diagonal<T>(dim);
		const auto& [row, col] = d1.shape();
		stream << std::setw(6) << col << ";"; stream.flush();
		const auto prev = d1;
		for (auto th : threads) {
			d1.change_thread_count(th);
			auto duration = std::numeric_limits<double>::max();
			const auto cnt = dim < 10000 ? 10 : 1;
			for (auto i = 0; i < cnt; ++i) {
				auto start = std::chrono::high_resolution_clock::now();
				d1 -= d2;
				duration = std::min(duration, std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start).count());
				if (d1 != result) {
					stream << std::endl << "ERROR: calculation"; stream.flush();
					break;
				}
				d1 = prev;
			}
			stream << std::setw(12) << std::fixed << duration << ";"; stream.flush();
		}
		stream << std::endl;
	}
}


template<typename T>
void multiply_test(std::ostream& stream)
{
	stream << "Multiply:" << typeid(T).name() << std::endl;
	auto threads = { 1u, 2u, 3u, 4u, 5u, 6u };
	auto dimensions = { 10, 25, 50, 100, 150, 200, 300, 500, 1000 };
	stream << std::setw(6) << "dim/th" << ";";
	for (auto th : threads) {
		stream << std::setw(12) << th << ";";
	}
	stream << std::endl;
	stream << std::setprecision(7);
	for (auto dim : dimensions) {
		auto d1 = diagonal<T>(dim);
		auto d2 = diagonal<T>(dim);
		auto result = diagonal<T>(dim);
		const auto& [row, col] = d1.shape();
		stream << std::setw(6) << col << ";"; stream.flush();
		const auto prev = d1;
		for (auto th : threads) {
			d1.change_thread_count(th);
			auto duration = std::numeric_limits<double>::max();
			const auto cnt = dim < 10000 ? 10 : 1;
			for (auto i = 0; i < cnt; ++i) {
				auto start = std::chrono::high_resolution_clock::now();
				d1 *=  d2;
				duration = std::min(duration, std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start).count());
				if (d1 != result) {
					stream << std::endl << "ERROR: calculation"; stream.flush();
					break;
				}
				d1 = prev;
			}
			stream << std::setw(12) << std::fixed << duration << ";"; stream.flush();
		}
		stream << std::endl;
	}
}


template<typename T>
void det_test(std::ostream& stream)
{
	stream << "Determinant:" << typeid(T).name() << std::endl;
	auto threads = { 1u, 2u, 3u, 4u, 5u, 6u };
	auto dimensions = { 10, 50, 100, 150, 200, 300, 500, 1000 };
	stream << std::setw(6) << "dim/th" << ";";
	for (auto th : threads) {
		stream << std::setw(12) << th << ";";
	}
	stream << std::endl;
	stream << std::setprecision(7);
	for (auto dim : dimensions) {
		auto d1 = diagonal<T>(dim, 0, 1);
		double result = ((double)(dim & 1) != 0 ? 1 : -1) * ((double)dim - 1);
		const auto& [row, col] = d1.shape();
		stream << std::setw(6) << col << ";"; stream.flush();
		for (auto th : threads) {
			d1.change_thread_count(th);
			auto duration = std::numeric_limits<double>::max();
			const auto cnt = dim < 10000 ? 10 : 1;
			for (auto i = 0; i < cnt; ++i) {
				auto start = std::chrono::high_resolution_clock::now();
				auto det = d1.det();
				duration = std::min(duration, std::chrono::duration<double>(std::chrono::high_resolution_clock::now() - start).count());
				if (std::abs(det - result) > 1e10) {
					stream << std::endl << "ERROR: calculation"; stream.flush();
					break;
				}
			}
			stream << std::setw(12) << std::fixed << duration << ";"; stream.flush();
		}
		stream << std::endl;
	}
}


int main()
{
	sum_test<double>(std::cout);
	sum_test<long long>(std::cout);
	sum_test<int>(std::cout);

	substract_test<double>(std::cout);
	substract_test<long long>(std::cout);
	substract_test<int>(std::cout);

	multiply_test<double>(std::cout);
	multiply_test<long long>(std::cout);
	multiply_test<int>(std::cout);

	det_test<double>(std::cout);
	det_test<long long>(std::cout);
	det_test<int>(std::cout);
	
	return 0;
}