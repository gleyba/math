#include "math/vector_float.h"

#include <utility>
#include "CppUnitTest.h"

using math::float2;
using math::float3;
using math::float4;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;


namespace Microsoft { namespace VisualStudio { namespace CppUnitTestFramework {

template<> inline std::wstring ToString<math::float2>(const math::float2& t) { RETURN_WIDE_STRING(t); }
template<> inline std::wstring ToString<math::float3>(const math::float3& t) { RETURN_WIDE_STRING(t); }
template<> inline std::wstring ToString<math::float4>(const math::float4& t) { RETURN_WIDE_STRING(t); }

}}} // namespace Microsoft::VisualStudio::CppUnitTestFramework


namespace unittest {

TEST_CLASS(math_vector_float_float2) {
public:

	TEST_METHOD(address_of)
	{
		float2 v(5, 6);
		float* ptr = &v.x;

		Assert::AreEqual(v.x, ptr[0]);
		Assert::AreEqual(v.y, ptr[1]);
	}

	TEST_METHOD(aspect_ratio)
	{
		using math::approx_equal;
		using math::aspect_ratio;

		Assert::IsTrue(approx_equal(1.0f, aspect_ratio(float2::unit_xy)));
		Assert::IsTrue(approx_equal(3.0f / 2.0f, aspect_ratio(float2(3, 2))));
	}

	TEST_METHOD(approx_equal)
	{
		using math::approx_equal;

		float2 v(1, 2);
		Assert::IsFalse(approx_equal(v, float2(100, 2)));
		Assert::IsFalse(approx_equal(v, float2(1, 100)));
		Assert::IsTrue(approx_equal(v, v));
		Assert::IsTrue(approx_equal(v, float2(1, 2)));

		Assert::IsTrue(approx_equal(v, float2(1, 2), 0.0f));
		Assert::IsFalse(approx_equal(v, float2(1.0001f, 2), 0.0f));
		Assert::IsFalse(approx_equal(v, float2(1, 2.0002f), 0.0f));
	}

	TEST_METHOD(assignments)
	{
		float2 v(5, 6);

		// copy assignment
		float2 vc;
		vc = v;
		Assert::IsTrue((vc.x == v.x) && (vc.y == v.y));

		// move assignment
		float2 vm;
		vm = std::move(v);
		Assert::IsTrue((vm.x == v.x) && (vm.y == v.y));
	}

	TEST_METHOD(binary_operators)
	{
		float2 v(1, 2);

		// operator+
		Assert::AreEqual(float2(11, 12), v + 10);
		Assert::AreEqual(float2(11, 12), 10 + v);
		Assert::AreEqual(float2(1.11f, 2.22f), v + float2(0.11f, 0.22f));
		Assert::AreEqual(float2(1.11f, 2.22f), float2(0.11f, 0.22f) + v);

		// operator-
		Assert::AreEqual(float2(0, 1), v - 1);
		Assert::AreEqual(float2(0, -1), 1 - v);
		Assert::AreEqual(float2::zero, v - float2(1, 2));
		Assert::AreEqual(float2::zero, float2(1, 2) - v);

		// operator*
		Assert::AreEqual(float2(10, 20), v * 10);
		Assert::AreEqual(float2(10, 20), 10 * v);
		Assert::AreEqual(float2(1, 4), v * v);

		// operator/
		Assert::AreEqual(float2(0.5, 1), v / 2);
		Assert::AreEqual(float2::zero, 0.0f / v);
		Assert::AreEqual(float2(1, 1), v / v);
	}

	TEST_METHOD(clamp)
	{
		using math::clamp;

		float2 lo(-5, -7);
		float2 hi(5, 7);
		float2 v(2, 3);

		Assert::AreEqual(lo, clamp(float2(-9, -10), lo, hi));
		Assert::AreEqual(v, clamp(v, lo, hi));
		Assert::AreEqual(hi, clamp(float2(9, 10), lo, hi));

		// default lo and hi
		Assert::AreEqual(float2::zero, clamp(float2(-5)));
		Assert::AreEqual(float2(0.5), clamp(float2(0.5)));
		Assert::AreEqual(float2::unit_xy, clamp(float2(5)));
	}

	TEST_METHOD(compound_assignment_operators)
	{
		float2 v(1, 2);
		(v += 5) += 5;
		Assert::AreEqual(float2(11, 12), v);

		(v -= 7) -= 3;
		Assert::AreEqual(float2(1, 2), v);

		(v *= 2) *= 3;
		Assert::AreEqual(float2(6, 12), v);

		(v /= 3) /= 2;
		Assert::AreEqual(float2(1, 2), v);

		(v += v) += v;
		Assert::AreEqual(float2(4, 8), v);

		v -= v;
		Assert::AreEqual(float2::zero, v);
	}

	TEST_METHOD(ctors)
	{
		float2 v0;
		Assert::IsTrue((v0.x == 0.0f) && (v0.y == 0.0f));

		float2 v1(24);
		Assert::IsTrue((v1.x == 24.0f) && (v1.y == 24.0f));

		float2 v2(1, 2);
		Assert::IsTrue((v2.x == 1.0f) && (v2.y == 2.0f));

		// copy ctor
		float2 vc = v2;
		Assert::IsTrue((vc.x == v2.x) && (vc.y == v2.y));

		// move ctor
		float2 vm = std::move(v2);
		Assert::IsTrue((vm.x == vc.x) && (vm.y == vc.y));
	}

	TEST_METHOD(dot_product)
	{
		using math::dot;
		using math::len_squared;

		float2 u(2, 3);
		float2 v(4, 5);
		float2 w(7, 8);

		Assert::AreEqual(2.0f * 4 + 3 * 5, dot(u, v));
		Assert::AreEqual(0.0f, dot(u, float2::zero));
		Assert::AreEqual(len_squared(u), dot(u, u), L"U * U = |U| * |U|");
		Assert::AreEqual(dot(u, v), dot(v, u), L"U * V = V * U");
		Assert::AreEqual(dot(2 * u, v), 2 * dot(u, v), L"(aU) * V = a(U * V)");
		Assert::AreEqual(dot(u + v, w), dot(u, w) + dot(v, w), L"(U + V)*W = U*W + V*W");
	}

	TEST_METHOD(equal_operator)
	{
		float2 v(1, 2);

		Assert::AreNotEqual(v, float2(100, 2));
		Assert::AreNotEqual(v, float2(1, 100));

		Assert::AreEqual(v, v);
		Assert::AreEqual(v, float2(1, 2));
	}

	TEST_METHOD(greater_than)
	{
		using math::greater_than;

		Assert::IsFalse(greater_than(float2(0, 5), 1));
		Assert::IsFalse(greater_than(float2(1, 5), 1));
		Assert::IsFalse(greater_than(float2(4, 0), 1));
		Assert::IsFalse(greater_than(float2(4, 1), 1));

		Assert::IsTrue(greater_than(float2(4, 5), 1));
	}

	TEST_METHOD(is_normalized)
	{
		using math::is_normalized;
		using math::len;

		Assert::IsTrue(is_normalized(float2::unit_x));
		Assert::IsTrue(is_normalized(float2::unit_y));

		float2 v(1, 2);
		Assert::IsTrue(len(v) > 1 && (!is_normalized(v)));
	}

	TEST_METHOD(len_and_len_squared)
	{
		using math::approx_equal;
		using math::len;
		using math::len_squared;

		float2 u(2, 3);
		float2 v(4, 5);

		Assert::AreEqual(2.f * 2 + 3 * 3, len_squared(u));
		Assert::AreEqual(std::sqrt(2.f * 2 + 3 * 3), len(u));

		Assert::AreEqual(2 * len(u), len(2 * u), L"|aU| = |a| * |U|");
		Assert::IsTrue(approx_equal(len(u + v), len(u) + len(v), 0.1f), L"|U + V| <= |U| + |V|");
	}

	TEST_METHOD(lerp)
	{
		using math::lerp;

		Assert::AreEqual(float2(0.0f), lerp(float2::zero, float2::unit_xy, 0.0f));
		Assert::AreEqual(float2(0.6f), lerp(float2::zero, float2::unit_xy, 0.6f));
		Assert::AreEqual(float2(1.0f), lerp(float2::zero, float2::unit_xy, 1.0f));

		float2 v(24);
		Assert::AreEqual(v, lerp(v, v, 0.4f));
		Assert::AreEqual(v, lerp(v, v, 0.7f));
	}

	TEST_METHOD(normalize)
	{
		using math::approx_equal;
		using math::len;
		using math::normalize;

		Assert::AreEqual(float2::zero, normalize(float2::zero));
		Assert::AreEqual(float2::unit_x, normalize(float2::unit_x));
		Assert::AreEqual(float2::unit_y, normalize(float2::unit_y));

		Assert::AreEqual(float2::unit_x, normalize(float2(24, 0)));
		Assert::AreEqual(float2::unit_y, normalize(float2(0, 24)));

		float2 u(-8, 6);
		Assert::IsTrue(approx_equal(1.0f, len(normalize(u))));
	}

	TEST_METHOD(static_members)
	{
		Assert::AreEqual(float2(0, 0), float2::zero);
		Assert::AreEqual(float2(1, 0), float2::unit_x);
		Assert::AreEqual(float2(0, 1), float2::unit_y);
		Assert::AreEqual(float2(1, 1), float2::unit_xy);
	}

	TEST_METHOD(unary_operators)
	{
		float2 v(1, 2);

		Assert::AreEqual(float2(-1, -2), -v);
		Assert::AreEqual(float2(1, 2), -(-v));
	}
};

TEST_CLASS(math_vector_float_float3) {
public:

	TEST_METHOD(address_of)
	{
		float3 v(5, 6, 7);
		float* ptr = &v.x;

		Assert::AreEqual(v.x, ptr[0]);
		Assert::AreEqual(v.y, ptr[1]);
		Assert::AreEqual(v.z, ptr[2]);
	}

	TEST_METHOD(approx_equal)
	{
		using math::approx_equal;

		float3 v(1, 2, 3);
		Assert::IsFalse(approx_equal(v, float3(100, 2, 3)));
		Assert::IsFalse(approx_equal(v, float3(1, 100, 3)));
		Assert::IsFalse(approx_equal(v, float3(1, 2, 100)));
		Assert::IsTrue(approx_equal(v, v));
		Assert::IsTrue(approx_equal(v, float3(1, 2, 3)));

		Assert::IsTrue(approx_equal(v, float3(1, 2, 3), 0.0f));
		Assert::IsFalse(approx_equal(v, float3(1.0001f, 2, 3), 0.0f));
		Assert::IsFalse(approx_equal(v, float3(1, 2.0002f, 3), 0.0f));
		Assert::IsFalse(approx_equal(v, float3(1, 2, 3.0003f), 0.0f));
	}

	TEST_METHOD(assignments)
	{
		float3 v(5, 6, 7);

		// copy assignment
		float3 vc;
		vc = v;
		Assert::IsTrue((vc.x == v.x) && (vc.y == v.y) && (vc.z == v.z));

		// move assignment
		float3 vm;
		vm = std::move(v);
		Assert::IsTrue((vm.x == v.x) && (vm.y == v.y) && (vm.z == v.z));
	}

	TEST_METHOD(binary_operators)
	{
		float3 v(1, 2, 3);

		// operator+
		Assert::AreEqual(float3(11, 12, 13), v + 10);
		Assert::AreEqual(float3(11, 12, 13), 10 + v);
		Assert::AreEqual(float3(1.11f, 2.22f, 3.33f), v + float3(0.11f, 0.22f, 0.33f));
		Assert::AreEqual(float3(1.11f, 2.22f, 3.33f), float3(0.11f, 0.22f, 0.33f) + v);

		// operator-
		Assert::AreEqual(float3(0, 1, 2), v - 1);
		Assert::AreEqual(float3(0, -1, -2), 1 - v);
		Assert::AreEqual(float3::zero, v - float3(1, 2, 3));
		Assert::AreEqual(float3::zero, float3(1, 2, 3) - v);

		// operator*
		Assert::AreEqual(float3(10, 20, 30), v * 10);
		Assert::AreEqual(float3(10, 20, 30), 10 * v);
		Assert::AreEqual(float3(1, 4, 9), v * v);

		// operator/
		Assert::AreEqual(float3(0.5, 1, 1.5), v / 2);
		Assert::AreEqual(float3::zero, 0 / v);
		Assert::AreEqual(float3(1, 1, 1), v / v);
	}

	TEST_METHOD(clamp)
	{
		using math::clamp;

		float3 lo(-5, -7, -9);
		float3 hi(5, 7, 9);
		float3 v(2, 3, 4);

		Assert::AreEqual(lo, clamp(float3(-9, -10, -11), lo, hi));
		Assert::AreEqual(v, clamp(v, lo, hi));
		Assert::AreEqual(hi, clamp(float3(9, 10, 11), lo, hi));

		// default lo and hi
		Assert::AreEqual(float3::zero, clamp(float3(-5)));
		Assert::AreEqual(float3(0.5), clamp(float3(0.5)));
		Assert::AreEqual(float3::unit_xyz, clamp(float3(5)));
	}

	TEST_METHOD(compound_assignment_operators)
	{
		float3 v(1, 2, 3);
		(v += 5) += 5;
		Assert::AreEqual(float3(11, 12, 13), v);

		(v -= 7) -= 3;
		Assert::AreEqual(float3(1, 2, 3), v);

		(v *= 2) *= 3;
		Assert::AreEqual(float3(6, 12, 18), v);

		(v /= 3) /= 2;
		Assert::AreEqual(float3(1, 2, 3), v);

		(v += v) += v;
		Assert::AreEqual(float3(4, 8, 12), v);

		v -= v;
		Assert::AreEqual(float3::zero, v);
	}

	TEST_METHOD(cross_product)
	{
		using math::cross;

		float3 u(1, 2, 3);
		float3 v(4, 5, -6);
		float3 w(7, -8, 9);

		Assert::AreEqual(cross(u, v), cross(-v, u), L"U x V = -V x U");

		Assert::AreEqual(
			cross((5 * u) + (7 * v), w),
			5 * cross(u, w) + 7 * cross(v, w),
			L"(aU + bV) x W = a(U x W) + b(V x W)");

		Assert::AreEqual(
			cross(u + v, w),
			cross(u, w) + cross(v, w),
			L"(U + V) x W = U x W + V x W");

		Assert::AreEqual(
			cross(w, u + v),
			cross(w, u) + cross(w, v),
			L"W x (U + V) = W x U + W x V");

		Assert::AreEqual(
			cross(u, cross(v, w)),
			(dot(u, w) * v) - (dot(u, v) * w),
			L"U x (V x W) = (U * W) * V - (U * V) * W");

		auto r1 = 5 * cross(u, v);
		auto r2 = cross(5 * u, v);
		auto r3 = cross(u, 5 * v);
		Assert::IsTrue(r1 == r2 && r2 == r3, L"(aU) x V = U x (aV) = a(U x V)");

		// scalar triple product
		float s1 = dot(cross(u, v), w);		// (U x V)*W
		float s2 = dot(cross(v, w), u);		// (V x W)*U
		float s3 = dot(cross(w, u), v);		// (W x U)*V
		Assert::IsTrue(s1 == s2 && s2 == s3);
		Assert::AreEqual(s1, dot(-cross(v, u), w), L"(U x V)*W = -(V x U)*W");
		Assert::AreEqual(s2, dot(-cross(w, v), u), L"(V x W)*U = -(W x V)*U");
		Assert::AreEqual(s3, dot(-cross(u, w), v), L"(W x U)*V = -(U x W)*V");

		float3 uv = cross(u, v);
		Assert::AreEqual(0.0f, dot(uv, u));
		Assert::AreEqual(0.0f, dot(uv, v));
		Assert::AreEqual(float3::zero, cross(u, u));
	}

	TEST_METHOD(ctors)
	{
		float3 v0;
		Assert::IsTrue((v0.x == 0) && (v0.y == 0) && (v0.z == 0));

		float3 v1(24);
		Assert::IsTrue((v1.x == 24) && (v1.y == 24) && (v1.z == 24));

		float2 vec2(3, 4);
		float3 v3(vec2, 5);
		Assert::IsTrue((v3.x == vec2.x) && (v3.y == vec2.y) && (v3.z == 5));

		float3 v4(1, 2, 3);
		Assert::IsTrue((v4.x == 1) && (v4.y == 2) && (v4.z == 3));

		// copy ctor
		float3 vc = v4;
		Assert::IsTrue((vc.x == v4.x) && (vc.y == v4.y) && (vc.z == v4.z));

		// move ctor
		float3 vm = std::move(v4);
		Assert::IsTrue((vm.x == vc.x) && (vm.y == vc.y) && (vm.z == vc.z));
	}

	TEST_METHOD(dot_product)
	{
		using math::dot;
		using math::len_squared;

		float3 u(2, 3, 4);
		float3 v(4, 5, 6);
		float3 w(7, 8, 9);

		Assert::AreEqual(2.f * 4 + 3 * 5 + 4 * 6, dot(u, v));
		Assert::AreEqual(0.f, dot(u, float3::zero));
		Assert::AreEqual(len_squared(u), dot(u, u), L"U * U = |U| * |U|");
		Assert::AreEqual(dot(u, v), dot(v, u), L"U * V = V * U");
		Assert::AreEqual(dot(2 * u, v), 2 * dot(u, v), L"(aU) * V = a(U * V)");
		Assert::AreEqual(dot(u + v, w), dot(u, w) + dot(v, w), L"(U + V)*W = U*W + V*W");
	}

	TEST_METHOD(equal_operator)
	{
		float3 v(1, 2, 3);

		Assert::AreNotEqual(v, float3(100, 2, 3));
		Assert::AreNotEqual(v, float3(1, 100, 3));
		Assert::AreNotEqual(v, float3(1, 2, 100));

		Assert::AreEqual(v, v);
		Assert::AreEqual(v, float3(1, 2, 3));
	}

	TEST_METHOD(greater_than)
	{
		using math::greater_than;

		Assert::IsFalse(greater_than(float3(0, 5, 7), 1));
		Assert::IsFalse(greater_than(float3(1, 5, 7), 1));
		Assert::IsFalse(greater_than(float3(4, 0, 7), 1));
		Assert::IsFalse(greater_than(float3(4, 1, 7), 1));
		Assert::IsFalse(greater_than(float3(4, 5, 0), 1));
		Assert::IsFalse(greater_than(float3(4, 5, 1), 1));

		Assert::IsTrue(greater_than(float3(4, 5, 7), 1));
	}

	TEST_METHOD(is_normalized)
	{
		using math::is_normalized;
		using math::len;

		Assert::IsTrue(is_normalized(float3::unit_x));
		Assert::IsTrue(is_normalized(float3::unit_y));
		Assert::IsTrue(is_normalized(float3::unit_z));

		float3 v(1, 2, 3);
		Assert::IsTrue(len(v) > 1 && (!is_normalized(v)));
	}

	TEST_METHOD(len_and_len_squared)
	{
		using math::approx_equal;
		using math::len;
		using math::len_squared;

		float3 u(2, 3, 4);
		float3 v(4, 5, 6);

		Assert::AreEqual(2.0f * 2 + 3 * 3 + 4 * 4, len_squared(u));
		Assert::AreEqual(std::sqrt(2.0f * 2 + 3 * 3 + 4 * 4), len(u));

		Assert::AreEqual(2 * len(u), len(2 * u), L"|aU| = |a| * |U|");
		Assert::IsTrue(approx_equal(len(u + v), len(u) + len(v), 0.1f), L"|U + V| <= |U| + |V|");
	}

	TEST_METHOD(lerp)
	{
		using math::lerp;

		Assert::AreEqual(float3(0.0f), lerp(float3::zero, float3::unit_xyz, 0.0f));
		Assert::AreEqual(float3(0.6f), lerp(float3::zero, float3::unit_xyz, 0.6f));
		Assert::AreEqual(float3(1.0f), lerp(float3::zero, float3::unit_xyz, 1.0f));

		float3 v(24);
		Assert::AreEqual(v, lerp(v, v, 0.4f));
		Assert::AreEqual(v, lerp(v, v, 0.7f));
	}

	TEST_METHOD(normalize)
	{
		using math::approx_equal;
		using math::len;
		using math::normalize;

		Assert::AreEqual(float3::zero, normalize(float3::zero));
		Assert::AreEqual(float3::unit_x, normalize(float3::unit_x));
		Assert::AreEqual(float3::unit_y, normalize(float3::unit_y));
		Assert::AreEqual(float3::unit_z, normalize(float3::unit_z));

		Assert::AreEqual(float3::unit_x, normalize(float3(24, 0, 0)));
		Assert::AreEqual(float3::unit_y, normalize(float3(0, 24, 0)));
		Assert::AreEqual(float3::unit_z, normalize(float3(0, 0, 24)));

		float3 u(-8, 6, 24);
		Assert::IsTrue(approx_equal(1.f, len(normalize(u))));
	}

	TEST_METHOD(rgb)
	{
		using math::rgb;

		Assert::AreEqual(float3::zero, rgb(0));
		Assert::AreEqual(float3::unit_x, rgb(0xff'00'00));
		Assert::AreEqual(float3::unit_y, rgb(0x00'ff'00));
		Assert::AreEqual(float3::unit_z, rgb(0x00'00'ff));
		Assert::AreEqual(float3::unit_xyz, rgb(0xffffff));

		Assert::AreEqual(float3(0xa1 / 255.0f, 0xb2 / 255.0f, 0xe3 / 255.0f), rgb(0xa1'b2'e3));
	}

	TEST_METHOD(static_members)
	{
		Assert::AreEqual(float3(0, 0, 0), float3::zero);
		Assert::AreEqual(float3(1, 0, 0), float3::unit_x);
		Assert::AreEqual(float3(0, 1, 0), float3::unit_y);
		Assert::AreEqual(float3(0, 0, 1), float3::unit_z);
		Assert::AreEqual(float3(1, 1, 0), float3::unit_xy);
		Assert::AreEqual(float3(1, 1, 1), float3::unit_xyz);
	}

	TEST_METHOD(unary_operators)
	{
		float3 v(1, 2, 3);

		Assert::AreEqual(float3(-1, -2, -3), -v);
		Assert::AreEqual(float3(1, 2, 3), -(-v));
	}
};

TEST_CLASS(math_vector_float_float4) {
public:

	TEST_METHOD(address_of)
	{
		float4 v(5, 6, 7, 8);
		float* ptr = &v.x;

		Assert::AreEqual(v.x, ptr[0]);
		Assert::AreEqual(v.y, ptr[1]);
		Assert::AreEqual(v.z, ptr[2]);
		Assert::AreEqual(v.w, ptr[3]);
	}

	TEST_METHOD(approx_equal)
	{
		using math::approx_equal;

		float4 v(1, 2, 3, 4);
		Assert::IsFalse(approx_equal(v, float4(100, 2, 3, 4)));
		Assert::IsFalse(approx_equal(v, float4(1, 100, 3, 4)));
		Assert::IsFalse(approx_equal(v, float4(1, 2, 100, 4)));
		Assert::IsFalse(approx_equal(v, float4(1, 2, 3, 100)));
		Assert::IsTrue(approx_equal(v, v));
		Assert::IsTrue(approx_equal(v, float4(1, 2, 3, 4)));

		Assert::IsTrue(approx_equal(v, float4(1, 2, 3, 4), 0.0f));
		Assert::IsFalse(approx_equal(v, float4(1.0001f, 2, 3, 4), 0.0f));
		Assert::IsFalse(approx_equal(v, float4(1, 2.0002f, 3, 4), 0.0f));
		Assert::IsFalse(approx_equal(v, float4(1, 2, 3.0003f, 4), 0.0f));
		Assert::IsFalse(approx_equal(v, float4(1, 2, 3, 4.0004f), 0.0f));
	}

	TEST_METHOD(assignments)
	{
		float4 v(5, 6, 7, 8);

		// copy assignment
		float4 vc;
		vc = v;
		Assert::IsTrue((vc.x == v.x) && (vc.y == v.y) && (vc.z == v.z) && (vc.w == v.w));

		// move assignment
		float4 vm;
		vm = std::move(v);
		Assert::IsTrue((vm.x == v.x) && (vm.y == v.y) && (vm.z == v.z) && (vm.w == v.w));
	}

	TEST_METHOD(binary_operators)
	{
		float4 v(1, 2, 3, 4);

		// operator+
		Assert::AreEqual(float4(11, 12, 13, 14), v + 10);
		Assert::AreEqual(float4(11, 12, 13, 14), 10 + v);
		Assert::AreEqual(float4(1.11f, 2.22f, 3.33f, 4.44f), v + float4(0.11f, 0.22f, 0.33f, 0.44f));
		Assert::AreEqual(float4(1.11f, 2.22f, 3.33f, 4.44f), float4(0.11f, 0.22f, 0.33f, 0.44f) + v);

		// operator-
		Assert::AreEqual(float4(0, 1, 2, 3), v - 1);
		Assert::AreEqual(float4(0, -1, -2, -3), 1 - v);
		Assert::AreEqual(float4::zero, v - float4(1, 2, 3, 4));
		Assert::AreEqual(float4::zero, float4(1, 2, 3, 4) - v);

		// operator*
		Assert::AreEqual(float4(10, 20, 30, 40), v * 10);
		Assert::AreEqual(float4(10, 20, 30, 40), 10 * v);
		Assert::AreEqual(float4(1, 4, 9, 16), v * v);

		// operator/
		Assert::AreEqual(float4(0.5, 1, 1.5, 2), v / 2);
		Assert::AreEqual(float4::zero, 0 / v);
		Assert::AreEqual(float4::unit_xyzw, v / v);
	}

	TEST_METHOD(clamp)
	{
		using math::clamp;

		float4 lo(-5, -7, -9, -11);
		float4 hi(5, 7, 9, 11);
		float4 v(2, 3, 4, 5);

		Assert::AreEqual(lo, clamp(float4(-9, -10, -11, -12), lo, hi));
		Assert::AreEqual(v, clamp(v, lo, hi));
		Assert::AreEqual(hi, clamp(float4(9, 10, 11, 12), lo, hi));

		// default lo and hi
		Assert::AreEqual(float4::zero, clamp(float4(-5)));
		Assert::AreEqual(float4(0.5), clamp(float4(0.5)));
		Assert::AreEqual(float4::unit_xyzw, clamp(float4(5)));
	}

	TEST_METHOD(compound_assignment_operators)
	{
		float4 v(1, 2, 3, 4);
		(v += 5) += 5;
		Assert::AreEqual(float4(11, 12, 13, 14), v);

		(v -= 7) -= 3;
		Assert::AreEqual(float4(1, 2, 3, 4), v);

		(v *= 2) *= 3;
		Assert::AreEqual(float4(6, 12, 18, 24), v);

		(v /= 3) /= 2;
		Assert::AreEqual(float4(1, 2, 3, 4), v);

		(v += v) += v;
		Assert::AreEqual(float4(4, 8, 12, 16), v);

		v -= v;
		Assert::AreEqual(float4::zero, v);
	}

	TEST_METHOD(ctors)
	{
		float4 v0;
		Assert::IsTrue((v0.x == 0) && (v0.y == 0) && (v0.z == 0) && (v0.w == 0));

		float4 v1(24);
		Assert::IsTrue((v1.x == 24) && (v1.y == 24) && (v1.z == 24) && (v1.w == 24));

		float4 v4(1, 2, 3, 4);
		Assert::IsTrue((v4.x == 1) && (v4.y == 2) && (v4.z == 3) && (v4.w == 4));

		// copy ctor
		float4 vc = v4;
		Assert::IsTrue((vc.x == v4.x) && (vc.y == v4.y) && (vc.z == v4.z) && (vc.w == v4.w));

		// move ctor
		float4 vm = std::move(v4);
		Assert::IsTrue((vm.x == vc.x) && (vm.y == vc.y) && (vm.z == vc.z) && (vm.w == vc.w));
	}

	TEST_METHOD(dot_product)
	{
		using math::dot;
		using math::len_squared;

		float4 u(2, 3, 4, 5);
		float4 v(5, 6, 7, 8);
		float4 w(9, 10, 11, 12);

		Assert::AreEqual(2.0f * 5 + 3 * 6 + 4 * 7 + 5 * 8, dot(u, v));
		Assert::AreEqual(0.0f, dot(u, float4::zero));
		Assert::AreEqual(len_squared(u), dot(u, u), L"U * U = |U| * |U|");
		Assert::AreEqual(dot(u, v), dot(v, u), L"U * V = V * U");
		Assert::AreEqual(dot(2 * u, v), 2 * dot(u, v), L"(aU) * V = a(U * V)");
		Assert::AreEqual(dot(u + v, w), dot(u, w) + dot(v, w), L"(U + V)*W = U*W + V*W");
	}

	TEST_METHOD(equal_operator)
	{
		float4 v(1, 2, 3, 4);

		Assert::AreNotEqual(v, float4(100, 2, 3, 4));
		Assert::AreNotEqual(v, float4(1, 100, 3, 4));
		Assert::AreNotEqual(v, float4(1, 2, 100, 4));
		Assert::AreNotEqual(v, float4(1, 2, 3, 100));

		Assert::AreEqual(v, v);
		Assert::AreEqual(v, float4(1, 2, 3, 4));
	}

	TEST_METHOD(greater_than)
	{
		using math::greater_than;

		Assert::IsFalse(greater_than(float4(0, 5, 7, 9), 1));
		Assert::IsFalse(greater_than(float4(1, 5, 7, 9), 1));
		Assert::IsFalse(greater_than(float4(4, 0, 7, 9), 1));
		Assert::IsFalse(greater_than(float4(4, 1, 7, 9), 1));
		Assert::IsFalse(greater_than(float4(4, 5, 0, 9), 1));
		Assert::IsFalse(greater_than(float4(4, 5, 1, 9), 1));
		Assert::IsFalse(greater_than(float4(4, 5, 7, 0), 1));
		Assert::IsFalse(greater_than(float4(4, 5, 7, 1), 1));

		Assert::IsTrue(greater_than(float4(4, 5, 7, 9), 1));
	}

	TEST_METHOD(is_normalized)
	{
		using math::is_normalized;
		using math::len;

		Assert::IsTrue(is_normalized(float4::unit_x));
		Assert::IsTrue(is_normalized(float4::unit_y));
		Assert::IsTrue(is_normalized(float4::unit_z));
		Assert::IsTrue(is_normalized(float4::unit_w));

		float4 v(1, 2, 3, 4);
		Assert::IsTrue(len(v) > 1 && (!is_normalized(v)));
	}

	TEST_METHOD(len_and_len_squared)
	{
		using math::approx_equal;
		using math::len;
		using math::len_squared;

		float4 u(2, 3, 4, 5);
		float4 v(4, 5, 6, 7);

		Assert::AreEqual(2.0f * 2 + 3 * 3 + 4 * 4 + 5 * 5, len_squared(u));
		Assert::AreEqual(std::sqrt(2.0f * 2 + 3 * 3 + 4 * 4 + 5 * 5), len(u));

		Assert::AreEqual(2 * len(u), len(2 * u), L"|aU| = |a| * |U|");
		Assert::IsTrue(approx_equal(len(u + v), len(u) + len(v), 0.1f), L"|U + V| <= |U| + |V|");
	}

	TEST_METHOD(lerp)
	{
		using math::lerp;

		Assert::AreEqual(float4(0.0f), lerp(float4::zero, float4::unit_xyzw, 0.0f));
		Assert::AreEqual(float4(0.6f), lerp(float4::zero, float4::unit_xyzw, 0.6f));
		Assert::AreEqual(float4(1.0f), lerp(float4::zero, float4::unit_xyzw, 1.0f));

		float4 v(24);
		Assert::AreEqual(v, lerp(v, v, 0.4f));
		Assert::AreEqual(v, lerp(v, v, 0.7f));
	}

	TEST_METHOD(normalize)
	{
		using math::approx_equal;
		using math::len;
		using math::normalize;

		Assert::AreEqual(float4::zero, normalize(float4::zero));
		Assert::AreEqual(float4::unit_x, normalize(float4::unit_x));
		Assert::AreEqual(float4::unit_y, normalize(float4::unit_y));
		Assert::AreEqual(float4::unit_z, normalize(float4::unit_z));
		Assert::AreEqual(float4::unit_w, normalize(float4::unit_w));

		Assert::AreEqual(float4::unit_x, normalize(float4(24, 0, 0, 0)));
		Assert::AreEqual(float4::unit_y, normalize(float4(0, 24, 0, 0)));
		Assert::AreEqual(float4::unit_z, normalize(float4(0, 0, 24, 0)));
		Assert::AreEqual(float4::unit_w, normalize(float4(0, 0, 0, 24)));

		float4 u(-8, 6, 24, -0.1f);
		Assert::IsTrue(approx_equal(1.f, len(normalize(u))));
	}

	TEST_METHOD(rgba)
	{
		using math::rgba;

		Assert::AreEqual(float4::zero, rgba(0));
		Assert::AreEqual(float4::unit_x, rgba(0xff'00'00'00));
		Assert::AreEqual(float4::unit_y, rgba(0x00'ff'00'00));
		Assert::AreEqual(float4::unit_z, rgba(0x00'00'ff'00));
		Assert::AreEqual(float4::unit_w, rgba(0x00'00'00'ff));
		Assert::AreEqual(float4::unit_xyzw, rgba(0xffffffff));

		Assert::AreEqual(float4(0xA1 / 255.0f, 0xB2 / 255.0f, 0xE3 / 255.0f, 0x18 / 255.0f), rgba(0xa1'b2'e3'18));
	}

	TEST_METHOD(static_members)
	{
		Assert::AreEqual(float4(0, 0, 0, 0), float4::zero);
		Assert::AreEqual(float4(1, 0, 0, 0), float4::unit_x);
		Assert::AreEqual(float4(0, 1, 0, 0), float4::unit_y);
		Assert::AreEqual(float4(0, 0, 1, 0), float4::unit_z);
		Assert::AreEqual(float4(0, 0, 0, 1), float4::unit_w);
		Assert::AreEqual(float4(1, 1, 1, 1), float4::unit_xyzw);
	}

	TEST_METHOD(unary_operators)
	{
		float4 v(1, 2, 3, 4);

		Assert::AreEqual(float4(-1, -2, -3, -4), -v);
		Assert::AreEqual(float4(1, 2, 3, 4), -(-v));
	}
};

} // namespace unittest
