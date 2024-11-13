#ifndef SimpleReader_inl
#define SimpleReader_inl

#include "ModelReader.h"

#include <exception>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

namespace {
    template<typename stream>
    class stream_raii {
    public:
        template <class S = stream>
        inline stream_raii(typename std::enable_if_t<std::is_trivially_default_constructible<S>::value> = 0)
            : ifs_{}
        {}

        inline stream_raii(const std::string& filePath)
            : ifs_{ filePath }
        {}

        inline stream_raii(const std::filesystem::path& filePath)
            : ifs_{ filePath }
        {}

        inline ~stream_raii() {
            if (ifs_) {
                ifs_.close();
            }
        }

        inline stream& get() noexcept {
            return ifs_;
        }

    private:
        stream ifs_;
    };
};

MYCG_NS_BEGIN

template<typename CoordinateType>
inline CGModel<CoordinateType> readModelFromObjInputStream(const std::filesystem::path& filePath) {
    stream_raii<std::ifstream> ifs{ filePath };

    if (!ifs.get()) {
        throw std::invalid_argument("Bad state of input stream");
    }

    using TypedCGModel = CGModel< CoordinateType >;
    typename TypedCGModel::VertexArray vertexArray{};
    typename TypedCGModel::FaceArray faces{};

    std::string line;
    while (!ifs.get().eof()) {
        std::getline(ifs.get(), line);

        std::istringstream iss{ line };
        char skipSymbol;

        if (0 == line.compare(0, 2, "v ")) {
            iss >> skipSymbol;
            typename TypedCGModel::VertexType v;
            for (int i = 0; i < 3; i++) {
                iss >> v[i];
            }
            vertexArray.push_back(v);
        }
        else if (0 == line.compare(0, 2, "f ")) {
            typename TypedCGModel::FaceType f;
            int itrash, idx;
            iss >> skipSymbol;
            while (iss >> idx >> skipSymbol >> itrash >> skipSymbol >> itrash) {
                // in wavefront obj all indices start at 1, not zero
                f.push_back(idx-1);
            }
            faces.push_back(std::move(f));
        }
    }

    return CGModel{ vertexArray, faces };
}

template<typename CoordinateType>
inline std::future<CGModel<CoordinateType>> readModelFromObjInputStreamAsync(const std::filesystem::path& filePath, std::launch policy) {
    return std::async<CGModel<CoordinateType>(const std::filesystem::path&)>(policy, &readModelFromObjInputStream, filePath);
}


MYCG_NS_END

#endif // SimpleReader_inl