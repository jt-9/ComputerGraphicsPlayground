#ifndef SimpleReader_h
#define SimpleReader_h

#pragma once

#include "MyCGDefines.h"
#include "CGModel.h"

#include <future>
#include <filesystem>

MYCG_NS_BEGIN
template<typename CoordinateType>
CGModel< CoordinateType > readModelFromObjInputStream(const std::filesystem::path& filePath);

template<typename CoordinateType>
std::future<CGModel<CoordinateType>> readModelFromObjInputStreamAsync(const std::filesystem::path& filePath, std::launch policy);
MYCG_NS_END

#include "ModelReader-inl.h"

#endif // SimpleReader_h