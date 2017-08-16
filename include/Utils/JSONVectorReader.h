#ifndef JSON_VECTOR_READER_H
#define JSON_VECTOR_READER_H

#include <array>
#include <vector>
#include <string>

#include "Eigen/Geometry"
#include "boost/property_tree/ptree.hpp"

class JSONVectorReader
{
public:
	template<typename T, size_t N>
	static std::array<T, N> readVector(const boost::property_tree::ptree& JSONvector)
	{
		std::array<T, N> result;
		auto iter = result.begin();
		for (auto& element : JSONvector)
		{
			*iter++ = element.second.get_value<T>();
		}
		return result;
	}

	//static TUniformVariant readUniform(boost::property_tree::ptree& JSONvector); // functionality from LiveCam

	static Eigen::Vector2f readVector2f(const boost::property_tree::ptree& JSONvector);

	static Eigen::Vector3f readVector3f(const boost::property_tree::ptree& JSONvector);

	static Eigen::Vector4f readVector4f(const boost::property_tree::ptree& JSONvector);
};

#endif