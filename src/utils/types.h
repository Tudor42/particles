#pragma once

namespace math {
    #include <Eigen/Dense>

    #ifdef USE_DOUBLE
    typedef double Real;

    #define REAL_MAX DBL_MAX
    #define REAL_MIN DBL_MIN
    #define RealParameter DoubleParameter
    #define RealParameterType ParameterBase::DOUBLE
    #define RealVectorParameter DoubleVectorParameter
    #define RealVectorParameterType ParameterBase::VEC_DOUBLE
    #else
    typedef float Real;

    #define REAL_MAX FLT_MAX
    #define REAL_MIN FLT_MIN
    #define RealParameter FloatParameter
    #define RealParameterType ParameterBase::FLOAT
    #define RealVectorParameter FloatVectorParameter
    #define RealVectorParameterType ParameterBase::VEC_FLOAT
    #endif

    using Vector2r = Eigen::Matrix<Real, 2, 1, Eigen::DontAlign>;
    using Vector3r = Eigen::Matrix<Real, 3, 1, Eigen::DontAlign>;
    using Vector4r = Eigen::Matrix<Real, 4, 1, Eigen::DontAlign>;
    using Vector5r = Eigen::Matrix<Real, 5, 1, Eigen::DontAlign>;
    using Vector6r = Eigen::Matrix<Real, 6, 1, Eigen::DontAlign>;
    using Matrix2r = Eigen::Matrix<Real, 2, 2, Eigen::DontAlign>;
    using Matrix3r = Eigen::Matrix<Real, 3, 3, Eigen::DontAlign>;
    using Matrix4r = Eigen::Matrix<Real, 4, 4, Eigen::DontAlign>;
    using Matrix5r = Eigen::Matrix<Real, 5, 5, Eigen::DontAlign>;
    using Matrix6r = Eigen::Matrix<Real, 6, 6, Eigen::DontAlign>;
    using Vector2i = Eigen::Matrix<int, 2, 1, Eigen::DontAlign>;
    using Vector3f = Eigen::Matrix<float, 3, 1, Eigen::DontAlign>;
    using Vector4f = Eigen::Matrix<float, 4, 1, Eigen::DontAlign>;
    using Matrix3f = Eigen::Matrix<float, 3, 3, Eigen::DontAlign>;
    using AlignedBox2r = Eigen::AlignedBox<Real, 2>;
    using AlignedBox3r = Eigen::AlignedBox<Real, 3>;
    using AngleAxisr = Eigen::AngleAxis<Real>;
    using Quaternionr = Eigen::Quaternion<Real, Eigen::DontAlign>;
    using VectorXr = Eigen::Matrix<Real, -1, 1, 0, -1, 1>;
    using MatrixXr = Eigen::Matrix<Real, -1, -1, 0, -1, -1>;
    
    #if defined(WIN32) || defined(_WIN32) || defined(WIN64)	   
    #define FORCE_INLINE __forceinline
    #else
    #define FORCE_INLINE __attribute__((always_inline))
    #endif

}