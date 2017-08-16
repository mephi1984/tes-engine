#include "include/Utils/JSONVectorReader.h"

Eigen::Vector2f JSONVectorReader::readVector2f(const boost::property_tree::ptree& JSONvector)
{
	auto vector = readVector<float, 2>(JSONvector);
	return{ vector[0], vector[1] };
}

Eigen::Vector3f JSONVectorReader::readVector3f(const boost::property_tree::ptree& JSONvector)
{
	auto vector = readVector<float, 3>(JSONvector);
	return{ vector[0], vector[1], vector[2] };
}

Eigen::Vector4f JSONVectorReader::readVector4f(const boost::property_tree::ptree& JSONvector)
{
	auto vector = readVector<float, 4>(JSONvector);
	return{ vector[0], vector[1], vector[2], vector[3] };
}

//TUniformVariant JSONVectorReader::readUniform(boost::property_tree::ptree& JSONvector)  // functionality from LiveCam
//{
//	std::vector<std::string> input;
//	for (auto& element : JSONvector)
//	{
//		input.push_back(element.second.get_value<std::string>());
//	}
//
//	std::string name = input.front();
//
//	if (input.size() == 1 && input[1].find('.') != -1)
//	{
//		return { TUniform1i(name, std::stoi(input[1])) };
//	}
//	else
//	{
//		std::vector<float> output;
//		for (auto &element = input.begin() + 1; element != input.end(); ++element)
//		{
//			output.push_back(std::stof(*element));
//		}
//		switch (output.size())
//		{
//		case 1: return { TUniform1f(name, output[0]) };
//		case 2: return { TUniform2f(name, { output[0], output[1] }) };
//		case 3: return { TUniform3f(name, { output[0], output[1], output[2] }) };
//		case 4: return { TUniform4f(name, { output[0], output[1], output[2], output[3] }) };
//		}
//	}
//}