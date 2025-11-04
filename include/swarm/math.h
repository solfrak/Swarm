#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// #include <glm/gtx/quaternion.hpp>

namespace swarm
{
    // Vector types
    using Vec2 = glm::vec2;
    using Vec3 = glm::vec3;
    using Vec4 = glm::vec4;

    using IVec2 = glm::ivec2;
    using IVec3 = glm::ivec3;
    using IVec4 = glm::ivec4;

    using UVec2 = glm::uvec2;
    using UVec3 = glm::uvec3;
    using UVec4 = glm::uvec4;

    // Matrix types
    using Mat2 = glm::mat2;
    using Mat3 = glm::mat3;
    using Mat4 = glm::mat4;

    // Quaternion
    using Quat = glm::quat;

    // Common color type
    using Color = glm::vec4;

    // Math utility functions
    namespace math
    {
        // === Matrix Construction ===
        inline Mat4 perspective(float fovy, float aspect, float near, float far)
        {
            return glm::perspective(fovy, aspect, near, far);
        }

        inline Mat4 ortho(float left, float right, float bottom, float top, float near, float far)
        {
            return glm::ortho(left, right, bottom, top, near, far);
        }

        inline Mat4 lookAt(const Vec3 &eye, const Vec3 &center, const Vec3 &up)
        {
            return glm::lookAt(eye, center, up);
        }

        inline Mat4 identity()
        {
            return Mat4(1.0f);
        }

        // === Matrix Transformations ===
        inline Mat4 translate(const Mat4 &m, const Vec3 &v)
        {
            return glm::translate(m, v);
        }

        inline Mat4 rotate(const Mat4 &m, float angle, const Vec3 &axis)
        {
            return glm::rotate(m, angle, axis);
        }

        inline Mat4 scale(const Mat4 &m, const Vec3 &v)
        {
            return glm::scale(m, v);
        }

        inline Mat4 inverse(const Mat4 &m)
        {
            return glm::inverse(m);
        }

        inline Mat4 transpose(const Mat4 &m)
        {
            return glm::transpose(m);
        }

        // === Vector Operations ===
        inline float length(const Vec3 &v)
        {
            return glm::length(v);
        }

        inline float distance(const Vec3 &a, const Vec3 &b)
        {
            return glm::distance(a, b);
        }

        inline Vec3 normalize(const Vec3 &v)
        {
            return glm::normalize(v);
        }

        inline float dot(const Vec3 &a, const Vec3 &b)
        {
            return glm::dot(a, b);
        }

        inline Vec3 cross(const Vec3 &a, const Vec3 &b)
        {
            return glm::cross(a, b);
        }

        inline Vec3 reflect(const Vec3 &incident, const Vec3 &normal)
        {
            return glm::reflect(incident, normal);
        }

        inline Vec3 refract(const Vec3 &incident, const Vec3 &normal, float eta)
        {
            return glm::refract(incident, normal, eta);
        }

        // === Interpolation ===
        inline float lerp(float a, float b, float t)
        {
            return glm::mix(a, b, t);
        }

        inline Vec3 lerp(const Vec3 &a, const Vec3 &b, float t)
        {
            return glm::mix(a, b, t);
        }

        inline Vec4 lerp(const Vec4 &a, const Vec4 &b, float t)
        {
            return glm::mix(a, b, t);
        }

        inline Quat slerp(const Quat &a, const Quat &b, float t)
        {
            return glm::slerp(a, b, t);
        }

        // === Clamping & Ranges ===
        inline float clamp(float value, float min, float max)
        {
            return glm::clamp(value, min, max);
        }

        inline Vec3 clamp(const Vec3 &v, float min, float max)
        {
            return glm::clamp(v, min, max);
        }

        inline float saturate(float value)
        {
            return glm::clamp(value, 0.0f, 1.0f);
        }

        // === Angle Conversions ===
        inline float radians(float degrees)
        {
            return glm::radians(degrees);
        }

        inline float degrees(float radians)
        {
            return glm::degrees(radians);
        }

        // === Common Math ===
        inline float min(float a, float b)
        {
            return glm::min(a, b);
        }

        inline float max(float a, float b)
        {
            return glm::max(a, b);
        }

        inline float abs(float x)
        {
            return glm::abs(x);
        }

        inline float sqrt(float x)
        {
            return glm::sqrt(x);
        }

        inline float pow(float x, float y)
        {
            return glm::pow(x, y);
        }

        inline float floor(float x)
        {
            return glm::floor(x);
        }

        inline float ceil(float x)
        {
            return glm::ceil(x);
        }

        inline float round(float x)
        {
            return glm::round(x);
        }

        // === Trigonometry ===
        inline float sin(float x)
        {
            return glm::sin(x);
        }

        inline float cos(float x)
        {
            return glm::cos(x);
        }

        inline float tan(float x)
        {
            return glm::tan(x);
        }

        inline float asin(float x)
        {
            return glm::asin(x);
        }

        inline float acos(float x)
        {
            return glm::acos(x);
        }

        inline float atan(float x)
        {
            return glm::atan(x);
        }

        inline float atan2(float y, float x)
        {
            return glm::atan(y, x);
        }

        // === Pointer Access (for uploading to GPU) ===
        inline float *valuePtr(Vec2 &v)
        {
            return glm::value_ptr(v);
        }

        inline const float *valuePtr(const Vec2 &v)
        {
            return glm::value_ptr(v);
        }

        inline float *valuePtr(Vec3 &v)
        {
            return glm::value_ptr(v);
        }

        inline const float *valuePtr(const Vec3 &v)
        {
            return glm::value_ptr(v);
        }

        inline float *valuePtr(Vec4 &v)
        {
            return glm::value_ptr(v);
        }

        inline const float *valuePtr(const Vec4 &v)
        {
            return glm::value_ptr(v);
        }

        inline float *valuePtr(Mat4 &m)
        {
            return glm::value_ptr(m);
        }

        inline const float *valuePtr(const Mat4 &m)
        {
            return glm::value_ptr(m);
        }

        // === Quaternion Operations ===
        inline Quat angleAxis(float angle, const Vec3 &axis)
        {
            return glm::angleAxis(angle, axis);
        }

        //
        // inline Mat4 toMat4(const Quat& q) {
        //     return glm::toMat4(q);
        // }

        inline Quat quatLookAt(const Vec3 &direction, const Vec3 &up)
        {
            return glm::quatLookAt(direction, up);
        }

        inline Vec3 eulerAngles(const Quat &q)
        {
            return glm::eulerAngles(q);
        }
    }
}
