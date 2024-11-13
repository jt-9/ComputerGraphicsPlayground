// MymtlConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "MymtlConsole.h"

#include "ApeMath.h"

#include "unique_rc.hpp"
#include "windows_dc_deleter.hpp"
#include "windows_gdi_deleter.hpp"


#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <numbers>
#include <memory>

namespace {
	template<typename T>
	using GDIObjectHandle = raii::unique_rc<T, raii::gdi_delete_object_nullptr<T>>;

	using namespace std::string_literals;

	template<typename T, std::size_t N>
	inline constexpr auto prepareToPrint(const mymtl::VectorN<T, N>& v) noexcept {
		auto buffer = "{ "s;

		auto constIt = v.begin();
		for (decltype(v.size()) i = 0; i < v.size(); i++, ++constIt) {
			if (i > 0) {
				buffer += ', ';
			}

			std::to_string(*constIt);
		}

		buffer += " }"s;

		return buffer;
	}

	template<typename T>
	inline constexpr auto prepareToPrint(const mymtl::Vector1<T>& v) noexcept {
		return "{ "s + std::to_string(v.x) + " }"s;
	}

	template<typename T>
	inline constexpr auto prepareToPrint(const mymtl::Vector2<T>& v) noexcept {
		return "{ "s + std::to_string(v.x) + ", "s + std::to_string(v.y) + " }"s;
	}

	template<typename T>
	inline constexpr auto prepareToPrint(const mymtl::Vector3<T>& v) noexcept {
		return "{ "s + std::to_string(v.x) + ", "s + std::to_string(v.y) + ", "s + std::to_string(v.z) + " }"s;
	}

	template<typename T, std::size_t N, std::size_t M>
	inline constexpr auto prepareToPrint(const mymtl::MatrixNxM<T, N, M>& m) noexcept {
		auto buffer = "{ "s;

		auto constIt = m.cbegin();
		for (decltype(m.rows()) i = 0; i < m.rows(); i++) {
			if (i > 0) {
				buffer += "\n  "s;
			}

			for (decltype(m.cols()) j = 0; j < m.cols(); j++, ++constIt) {
				if (j > 0) {
					buffer += '\t';
				}

				buffer += std::to_string(*constIt);
			}
		}

		buffer += " }"s;

		return buffer;
	}

	constexpr unsigned int runtime_sixty() // Emulate runtime input
	{
		return 60u;
	}

	template<typename T, std::size_t N, std::size_t M>
	inline constexpr void swapMatrix(mymtl::MatrixNxM<T, N, M>& m1, mymtl::MatrixNxM<T, N, M>& m2) noexcept
	{
		using namespace mymtl;

		using std::swap;
		swap(m1, m2);
		//m1.swap(m2);
	}
}

void testSwapMatrix() noexcept {
	using namespace mymtl;
	
	using std::cout;

	MatrixNxM<float, 3, 7> m1{ -3.1415f, 2, 3, -1.5f, -4, 1.1f, 9, 2, 21, -2.5f, 4.5f, 10, -10, -1, 2.718281828f };
	MatrixNxM<float, 3, 7> m2{ -5, 6, -17, 8, -2, 0, 5.5f, 3, 11, 12, 34, 32, 3.5, 1, 5.7f, 3.1415926f };

	cout << "-------------------------------------------------------" << "\n";
	cout << "Matrix swap test #1\n";
	cout << "Matrix m1 of " << m1.rows() << " x " << m1.cols() << "\n" << prepareToPrint(m1) << "\n";
	cout << "\n";
	cout << "Matrix m2 of " << m2.rows() << " x " << m2.cols() << "\n" << prepareToPrint(m2) << "\n";
	cout << "\n";

	swapMatrix(m1, m2);

	cout << "m1 <-> m2, m1 of " << m1.rows() << " x " << m1.cols() << "\n" << prepareToPrint(m1) << "\n";
	cout << "\n";
	cout << "m2 <-> m1, m2 of " << m2.rows() << " x " << m2.cols() << "\n" << prepareToPrint(m2) << "\n";
	cout << "=======================================================" << "\n";
}

void testUnaryMinusMatrix() noexcept {
	using namespace mymtl;
	using namespace details;

	using std::cout;

	constexpr MatrixNxM<float, 3, 7> m1{ -3.1415f, 2, 3, -1.5f, -4, 1.1f, 9, 2, 21, -2.5f, 4.5f, 10, -10, -1, 2.718281828f };

	cout << "-------------------------------------------------------" << "\n";
	cout << "Matrix unary minus test #1\n";
	cout << "Matrix m1 of " << m1.rows() << " x " << m1.cols() << "\n" << prepareToPrint(m1) << "\n";
	cout << "\n";

	constexpr auto m2{ -m1 };

	cout << "m2 := -m1, m2 of " << m2.rows() << " x " << m2.cols() << "\n" << prepareToPrint(m2) << "\n";
	cout << "=======================================================" << "\n";
}

void testSumMatrix() noexcept {
	using namespace mymtl;
	using namespace details;

	using std::cout;

	constexpr Matrix4<float> m1{ -3.1415f, 2, 3, -1.5f, -4, 1.1f, 9, 2, 21, -2.5f, 4.5f, 10, -10, -1, 2.718281828f };
	constexpr Matrix4<float> m2{ -5, 6, -17, 8, -2, 0, 5.5f, 3, 11, 12, 34, 32, 3.5, 1, 5.7f, 3.1415926f };
	
	cout << "-------------------------------------------------------" << "\n";
	cout << "Matrix summation test #1\n";
	cout << "Matrix m1 of " << m1.rows() << " x " << m1.cols() << "\n" << prepareToPrint(m1) << "\n";
	cout << "\n";
	cout << "Matrix m2 of " << m2.rows() << " x " << m2.cols() << "\n" << prepareToPrint(m2) << "\n";
	cout << "\n";

	constexpr auto m3 = m1 + m2;
	cout << "m3 := m1 + m2 of " << m3.rows() << " x " << m3.cols() << "\n" << prepareToPrint(m3) << "\n";
	cout << "=======================================================" << "\n";
}

void testSubMatrix() noexcept {
	using namespace mymtl;

	using std::cout;

	constexpr Matrix4<float> m1{ -3.1415f, 2, 3, -1.5f, -4, 1.1f, 9, 2, 21, -2.5f, 4.5f, 10, -10, -1, 2.718281828f };
	constexpr Matrix4<float> m2{ -5, 6, -17, 8, -2, 0, 5.5f, 3, 11, 12, 34, 32, 3.5, 1, 5.7f, 3.1415926f };

	cout << "-------------------------------------------------------" << "\n";
	cout << "Matrix subtraction test #1\n";
	cout << "Matrix m1 of " << m1.rows() << " x " << m1.cols() << "\n" << prepareToPrint(m1) << "\n";
	cout << "\n";
	cout << "Matrix m2 of " << m2.rows() << " x " << m2.cols() << "\n" << prepareToPrint(m2) << "\n";
	cout << "\n";

	constexpr auto m3 = m1 - m2;
	cout << "m3 := m1 - m2 of " << m3.rows() << " x " << m3.cols() << "\n" << prepareToPrint(m3) << "\n";
	cout << "=======================================================" << "\n";
}

void testEqMatrix() noexcept {
	using namespace mymtl;

	using std::cout;

	constexpr Matrix4<float> m1{ -3.1415f, 2, 3, -1.5f, -4, 1.1f, 9, 2, 21, -2.5f, 4.5f, 10, -10, -1, 2.718281828f };
	constexpr auto m2{ m1 };

	cout << "-------------------------------------------------------" << "\n";
	cout << "Matrix equality test #1\n";
	cout << "Matrix m1 of " << m1.rows() << " x " << m1.cols() << "\n" << prepareToPrint(m1) << "\n";
	cout << "\n";
	cout << "Matrix m2 of " << m2.rows() << " x " << m2.cols() << "\n" << prepareToPrint(m2) << "\n";
	cout << "\n";

	constexpr auto eq = m1 == m2;
	cout << "m1 == m2 ? " << std::to_string(eq) << "\n";
	cout << "m1 != m2 ? " << std::to_string(!eq) << "\n";
	cout << "=======================================================" << "\n";
}

constexpr void testMatrixAt() noexcept {

	using namespace mymtl;

	using std::cout;

	Matrix4<float> m1{ -3.1415f, 2, 3, -1.5f, -4, 1.1f, 9, 2, 21, -2.5f, 4.5f, 10, -10, -1, 2.718281828f };

	try
	{
		// Set element 6, where the index is determined at runtime
		m1.at(runtime_sixty(), 30) = 157;
	}
	catch (std::out_of_range const& exc)
	{
		cout << exc.what() << '\n';
	}
}

template<typename T, typename V>
constexpr auto mySum(T t, V v) /*-> (t + u)*/ noexcept {
	return t + v;
}

void testMultMatrix() noexcept
{
	using namespace mymtl;

	using std::cout;

	{
		constexpr Matrix4<float> m1{ -3.1415f, 2, 3, -1.5f, -4, 1.1f, 9, 2, 21, -2.5f, 4.5f, 10, -10, -1, 2.718281828f, -2.5 };
		constexpr Matrix4<float> m2{ -5, 6, -17, 8, -2, 0, 5.5f, 3, 11, 12, 34, 32, 3.5, 1, 5.7f, 3.1415926f };

		cout << "-------------------------------------------------------" << "\n";
		cout << "Matrix multiplication test #1\n";
		cout << "Matrix m1 of " << m1.rows() << " x " << m1.cols() << "\n" << prepareToPrint(m1) << "\n";
		cout << "\n";
		cout << "Matrix m2 of " << m2.rows() << " x " << m2.cols() << "\n" << prepareToPrint(m2) << "\n";
		cout << "\n";

		constexpr auto m3 = m1 * m2;
		cout << "m3 := m1 * m2 of " << m3.rows() << " x " << m3.cols() << "\n" << prepareToPrint(m3) << "\n";
		cout << "=======================================================" << "\n";
		
		cout << "-------------------------------------------------------" << "\n";
		cout << "Matrix multiplication test #2 copy constructor, then *=\n";
		cout << "\n";

		auto m4{ m1 };
		m4 *= m2;
		cout << "m4 := m1, m4 *= m1 of " << m3.rows() << " x " << m3.cols() << "\n" << prepareToPrint(m3) << "\n";
		cout << "\n";

		const auto eq = m3 == m4;
		cout << "m3 == m4 ? " << std::to_string(eq) << "\n";
		cout << "=======================================================" << "\n";
	}

	{
		constexpr MatrixNxM<float, 2, 4> m1{ 3, 2, 3, -1, -2, 2.75, 0.5, -1 };
		constexpr MatrixNxM<float, 4, 1> m2{ -5, 6, -1, 8 };
		
		cout << "-------------------------------------------------------" << "\n";
		cout << "Matrix multiplication test #3 (1xn)*(nx1)\n";
		cout << "Matrix m1 of " << m1.rows() << " x " << m1.cols() << "\n" << prepareToPrint(m1) << "\n";
		cout << "\n";
		cout << "Matrix m2 of " << m2.rows() << " x " << m2.cols() << "\n" << prepareToPrint(m2) << "\n";
		cout << "\n";

		constexpr auto m3 = m1 * m2;
		cout << "m3 := m1 * m2 of " << m3.rows() << " x " << m3.cols() << "\n" << prepareToPrint(m3) << "\n";
		cout << "=======================================================" << "\n";
	}

	{
		Matrix4<float> m1{ 3, 2, 3, -1, 
							-2, 2.75, 0.5, -1, 
							7, 2, -4, 2, 
							1, -1 };

		cout << "-------------------------------------------------------" << "\n";
		cout << "Matrix multiplication test #4 (nxn)*(nxn) matrix squared\n";
		cout << "Matrix m1 of " << m1.rows() << " x " << m1.cols() << "\n" << prepareToPrint(m1) << "\n";
		cout << "\n";

		auto m2 = m1 * m1;
		m1 = m1 * m1;
		cout << "m2 := m1 * m1 of " << m2.rows() << " x " << m2.cols() << "\n" << prepareToPrint(m2) << "\n";
		cout << "\n";
		cout << "m1 := m1 * m1 of " << m1.rows() << " x " << m1.cols() << "\n" << prepareToPrint(m1) << "\n";
		cout << "\n";
		const auto eq = m1 == m2;
		cout << "m1 == m2 ? " << std::to_string(eq) << "\n";
		cout << "=======================================================" << "\n";
	}

	{
		constexpr auto closeToUpperBound = 2000'000'000;
		constexpr Matrix4<std::int32_t> m1{ 
			closeToUpperBound+'A', closeToUpperBound+'B', closeToUpperBound+'C', 'D',
			'E', 'F', 'G', 'H',
			'I', 'G', 'K', 'L',
			closeToUpperBound+'M', closeToUpperBound+'N', closeToUpperBound+'O', closeToUpperBound+'P',
		};

		constexpr Matrix4<std::int32_t> m2{
			closeToUpperBound-'a', 'b', 'c', 'd',
			'e', closeToUpperBound-'f', 'g', 'h',
			'i', 'g', closeToUpperBound-'k', 'l',
			'm', 'n', 'o', closeToUpperBound -'p',
		};

		cout << "-------------------------------------------------------" << "\n";
		cout << "Matrix multiplication test #5 (4x4)*(4x4) of std::int32_t with positive overflowing\n";
		cout << "Matrix m1 of " << m1.rows() << " x " << m1.cols() << "\n" << prepareToPrint(m1) << "\n";
		cout << "\n";
		cout << "Matrix m2 of " << m2.rows() << " x " << m2.cols() << "\n" << prepareToPrint(m2) << "\n";
		cout << "\n";

		constexpr Matrix4<std::int64_t> m3 = operator*<std::int32_t, std::int32_t, std::int64_t>(m1, m2);
		cout << "m3 := m1 * m2 of " << m3.rows() << " x " << m3.cols() << "\n" << prepareToPrint(m3) << "\n";
		cout << "=======================================================" << "\n";
	}

	{
		constexpr auto closeToUpperBound = 2000'000'000;
		constexpr Matrix4<std::int32_t> m1{
			closeToUpperBound + 'A', closeToUpperBound + 'B', closeToUpperBound + 'C', 'D',
			'E', 'F', 'G', 'H',
			'I', 'G', 'K', 'L',
			closeToUpperBound + 'M', closeToUpperBound + 'N', closeToUpperBound + 'O', closeToUpperBound + 'P',
		};

		MatrixNxM<std::int32_t, 4, 1> m2{
			closeToUpperBound - 'a', 'b', closeToUpperBound+'c', 'd',
		};

		cout << "-------------------------------------------------------" << "\n";
		cout << "Matrix multiplication test #6 (4x4)*(4x1) of std::int32_t with positive overflowing\n";
		cout << "Matrix m1 of " << m1.rows() << " x " << m1.cols() << "\n" << prepareToPrint(m1) << "\n";
		cout << "\n";
		cout << "Matrix m2 of " << m2.rows() << " x " << m2.cols() << "\n" << prepareToPrint(m2) << "\n";
		cout << "\n";

		MatrixNxM<std::int64_t, 4, 1> m3 = matrix_multiply_ert<std::remove_cv_t<decltype(m3)>::value_type>(m1, m2);
		cout << "m3 := m1 * m2 of " << m3.rows() << " x " << m3.cols() << "\n" << prepareToPrint(m3) << "\n";
		cout << "=======================================================" << "\n";

		//decltype(std::declval<int>() * std::declval<int>()) temp = -45;
	}
}

void testSumMatrix1x1() noexcept {
	using namespace mymtl;

	using std::cout;
	constexpr Matrix1<double> m1_1x1{ -3.1415 };
	constexpr Matrix1<int> m2_1x1{ -5 };

	{
		cout << "-------------------------------------------------------" << "\n";
		cout << "Matrix summation test #1\n";
		cout << "Matrix m1 of " << m1_1x1.rows() << " x " << m1_1x1.cols() << "\n" << prepareToPrint(m1_1x1) << "\n";
		cout << "\n";
		cout << "Matrix m2 of " << m2_1x1.rows() << " x " << m2_1x1.cols() << "\n" << prepareToPrint(m2_1x1) << "\n";
		cout << "\n";

		constexpr auto m3 = m1_1x1 + m2_1x1;
		cout << "m3 := m1 + m2 of " << m3.rows() << " x " << m3.cols() << "\n" << prepareToPrint(m3) << "\n";
		cout << "=======================================================" << "\n";
	}

	{
		cout << "-------------------------------------------------------" << "\n";
		cout << "Matrix summation test #2\n";
		cout << "Matrix m1 of " << m1_1x1.rows() << " x " << m1_1x1.cols() << "\n" << prepareToPrint(m1_1x1) << "\n";
		cout << "\n";
		cout << "Matrix m2 of " << m2_1x1.rows() << " x " << m2_1x1.cols() << "\n" << prepareToPrint(m2_1x1) << "\n";
		cout << "\n";

		constexpr auto m3 = m1_1x1 + (-m2_1x1);
		cout << "m3 := m1 + (-m2) of " << m3.rows() << " x " << m3.cols() << "\n" << prepareToPrint(m3) << "\n";
		cout << "=======================================================" << "\n";
	}
}

void testMultMatrix1x1() noexcept {
	using namespace mymtl;
	using namespace std::numbers;

	using std::cout;

	
	{
		Matrix1<double> m1{ pi };

		cout << "-------------------------------------------------------" << "\n";
		cout << "Matrix1x1 multiplication test #1 operator::*=\n";
		cout << "Matrix m1 of " << m1.rows() << " x " << m1.cols() << "\n" << prepareToPrint(m1) << "\n";
		cout << "\n";

		m1 *= m1;
		cout << "m1 := m1^2 of " << m1.rows() << " x " << m1.cols() << "\n" << prepareToPrint(m1) << "\n";
		cout << "=======================================================" << "\n";
	}

	{
		Matrix1<double> m1{ pi };

		{
			cout << "-------------------------------------------------------" << "\n";
			cout << "Matrix1x1 multiplication test #2 operator::*\n";
			cout << "Matrix m1 of " << m1.rows() << " x " << m1.cols() << "\n" << prepareToPrint(m1) << "\n";
			cout << "\n";

			auto m2 = m1 * m1;
			cout << "m2 := m1^2 of " << m2.rows() << " x " << m2.cols() << "\n" << prepareToPrint(m2) << "\n";
			cout << "=======================================================" << "\n";
		}

		{
			cout << "-------------------------------------------------------" << "\n";
			cout << "Matrix1x1 multiplication test #3 MatrixMultiply\n";
			cout << "Matrix m1 of " << m1.rows() << " x " << m1.cols() << "\n" << prepareToPrint(m1) << "\n";
			cout << "\n";

			decltype(m1) m3 = matrix_multiply_ert<std::remove_cv_t<decltype(m3)>::value_type>(m1, m1);

			cout << "m3 := m1^2 of " << m3.rows() << " x " << m3.cols() << "\n" << prepareToPrint(m3) << "\n";
			cout << "=======================================================" << "\n";
		}

		{
			cout << "-------------------------------------------------------" << "\n";
			cout << "Matrix1x1 multiplication test #4 MatrixMultiply truncate to Matrix<int>\n";
			cout << "Matrix m1 of " << m1.rows() << " x " << m1.cols() << "\n" << prepareToPrint(m1) << "\n";
			cout << "\n";

			MatrixN<int, m1.rows()> m4 = matrix_multiply_ert<std::remove_cv_t<decltype(m4)>::value_type> (m1, m1);

			cout << "m4 := m1^2 of " << m4.rows() << " x " << m4.cols() << "\n" << prepareToPrint(m4) << "\n";
			cout << "=======================================================" << "\n";
		}
	}
}

void testMatrix() noexcept {
	testMultMatrix();

	testSwapMatrix();

	testUnaryMinusMatrix();

	testSumMatrix();
	testSubMatrix();
	testEqMatrix();

	testMatrixAt();

	testSumMatrix1x1();
	testMultMatrix1x1();

	std::cout << std::endl;
}

void testSumVector3() noexcept {
	using namespace mymtl;

	using std::cout;

	constexpr Vector3<double> v1{ -3.1415f, 2, 5 };
	constexpr Vector3<float> v2{ -5, 6, -2.7818281828459045f };

	{
		cout << "-------------------------------------------------------" << "\n";
		cout << "Vector summation test #1\n";
		cout << "Vector v1 of " << v1.size() << ": " << prepareToPrint(v1) << "\n";
		cout << "Vector v2 of " << v2.size() << ": " << prepareToPrint(v2) << "\n";
		cout << "\n";

		constexpr auto v3 = v1 + v2;

		cout << "v3 := v1 + v2, v3 of " << v3.size() << ": " << prepareToPrint(v3) << "\n";
		cout << "Is Vector3<T>::operator +(v1, v2) noexcept? noexcept(" << noexcept(v1 + v2) << ")\n";
		cout << "=======================================================" << "\n";
	}

	{
		cout << "-------------------------------------------------------" << "\n";
		cout << "Vector summation test #2 copy constructor, then *=\n";

		auto v3{ v1 };
		cout << "Vector v3 of " << v3.size() << ": " << prepareToPrint(v3) << "\n";
		cout << "Vector v2 of " << v2.size() << ": " << prepareToPrint(v2) << "\n";
		cout << "\n";

		v3 += v2;

		cout << "v3{ v1 }, v3 += v2, v3 of " << v3.size() << ": " << prepareToPrint(v3) << "\n";
		cout << "Is Vector3<T>::operator +=(v2) noexcept? noexcept(" << noexcept(std::declval<std::remove_cvref_t<decltype(v3)>>() += v2) << ")\n";
		cout << "=======================================================" << "\n";
	}

	//auto v1Norm = vector_normalize(v1);
	//auto v1Length = vector_magnitude(v1Norm);
	//--v1Length;
}

void testSwapVector3() noexcept {
	using namespace mymtl;

	using std::cout;

	Vector3<float> v1{ -3.1415f, 2, 5 };
	Vector3<float> v2{ -5, 6, -2.7818281828459045f };

	cout << "-------------------------------------------------------" << "\n";
	cout << "Vector swap test #1 method\n";
	cout << "Vector v1 of " << v1.size() << ": " << prepareToPrint(v1) << "\n";
	cout << "Vector v2 of " << v2.size() << ": " << prepareToPrint(v2) << "\n";
	cout << "\n";

	v1.swap(v2);

	cout << "v1 <-> v2, v1 of " << v1.size() << ": " << prepareToPrint(v1) << "\n";
	cout << "v2 <-> v1, v2 of " << v2.size() << ": " << prepareToPrint(v2) << "\n";
	cout << "=======================================================" << "\n";

	cout << "-------------------------------------------------------" << "\n";
	cout << "Vector swap test #2 free function\n";
	cout << "Vector v1 of " << v1.size() << ": " << prepareToPrint(v1) << "\n";
	cout << "Vector v2 of " << v2.size() << ": " << prepareToPrint(v2) << "\n";
	cout << "\n";

	using std::swap;
	swap(v1, v2);

	cout << "v1 <-> v2, v1 of " << v1.size() << ": " << prepareToPrint(v1) << "\n";
	cout << "v2 <-> v1, v2 of " << v2.size() << ": " << prepareToPrint(v2) << "\n";
	cout << "=======================================================" << "\n";
}

void testVector() noexcept {
	testSumVector3();
	testSwapVector3();
}

void testUniqueHandle() noexcept {
	using std::cout;

	GDIObjectHandle<HPEN> blackPen{ static_cast<HPEN>(GetStockObject(BLACK_PEN)) }, whitePen{ static_cast<HPEN>(GetStockObject(WHITE_PEN)) };

	cout << "-------------------------------------------------------" << "\n";
	cout << "UniqueHandle Threeway comparison <=> test #1\n";

	cout << "pen1{" << blackPen.get() << "} <=> pen2{" << whitePen.get() << "} is " << std::to_string((blackPen <=> whitePen)._Value) << "\n";
	cout << "pen1{" << blackPen.get() << "} <=> pen1{" << blackPen.get() << "} is " << std::to_string((blackPen <=> blackPen)._Value) << "\n";
	cout << "pen2{" << whitePen.get() << "} <=> pen1{" << blackPen.get() << "} is " << std::to_string((whitePen <=> blackPen)._Value) << "\n";

	cout << "=======================================================" << "\n";

	cout << "-------------------------------------------------------" << "\n";
	cout << "UniqueHandle swap test #1\n";
	cout << "UniqueHandle pen1{ " << blackPen.get() << " } and pen2{ " << whitePen.get() << " }" << "\n";

	using std::swap;
	swap(blackPen, whitePen);
	cout << "After swap: pen1 <-> pen2\n";
	cout << "UniqueHandle pen1{ " << blackPen.get() << " } and pen2{ " << whitePen.get() << " }" << "\n";

	cout << "=======================================================" << "\n";

	//auto c1 = whitePen == blackPen;
	//auto c2 = whitePen != blackPen;
	//auto c3 = whitePen < blackPen;

	//auto c4 = !whitePen;
}

void testBarycentricCoordinates() noexcept
{
	using namespace mymtl;
	//using namespace details;

	constexpr Vector2f a2{ 1, -1 }, b2{ -2, 2 }, c2{ 3, 3 }, p2{ 2, 2.5 };

	constexpr auto notEqual = a2 != c2;

	constexpr auto bc1 = mymtl::vector_barycentric(a2, b2, c2, p2);

	std::cout << "2D case barycentric coordinates " << prepareToPrint(bc1) << "\n\tof p" << prepareToPrint(p2) << " to triangle a" << prepareToPrint(a2) << " b" << prepareToPrint(b2) << " c" << prepareToPrint(c2) << std::endl;

	constexpr Vector3f a3{ a2.x, a2.y, 1 }, b3{ b2.x, b2.y, 1 }, c3{ c2.x, c2.y, 1 }, p3{ p2.x, p2.y, 1 };
	constexpr auto bc2 = mymtl::vector_barycentric(a3, b3, c3, p3);
	std::cout << "3D case barycentric coordinates " << prepareToPrint(bc2) << "\n\tof p" << prepareToPrint(p3) << " to triangle a" << prepareToPrint(a3) << " b" << prepareToPrint(b3) << " c" << prepareToPrint(c3) << std::endl;
}

struct XMFLOAT4X4
{
	union
	{
		struct
		{
			FLOAT _11, _12, _13, _14;
			FLOAT _21, _22, _23, _24;
			FLOAT _31, _32, _33, _34;
			FLOAT _41, _42, _43, _44;
		};
		FLOAT m[4][4];
	};

	//_XMFLOAT4X4() {};
	//_XMFLOAT4X4(FLOAT m00, FLOAT m01, FLOAT m02, FLOAT m03,
	//	FLOAT m10, FLOAT m11, FLOAT m12, FLOAT m13,
	//	FLOAT m20, FLOAT m21, FLOAT m22, FLOAT m23,
	//	FLOAT m30, FLOAT m31, FLOAT m32, FLOAT m33);
	//_XMFLOAT4X4(CONST FLOAT* pArray);

	//FLOAT       operator() (UINT Row, UINT Column) CONST { return m[Row][Column]; }
	//FLOAT& operator() (UINT Row, UINT Column) { return m[Row][Column]; }

	//_XMFLOAT4X4& operator= (CONST _XMFLOAT4X4& Float4x4);

};

int main() noexcept {
	testBarycentricCoordinates();

	testMatrix();

	testVector();

	//testUniqueHandle();

	return 0;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
