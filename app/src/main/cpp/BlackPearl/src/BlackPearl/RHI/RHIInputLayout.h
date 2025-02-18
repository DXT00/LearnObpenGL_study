#pragma once

#include "RHIDefinitions.h"
#include "RefCountPtr.h"
#include "RHIResources.h"
#include "BlackPearl/Core.h"
#include <string>
using namespace std;

namespace BlackPearl {

	enum class ElementDataType {
		Char,
		Char3,
		Int,
		Int2,
		Int3,
		Int4,
		Float,
		Float2,
		Float3,
		Float4,
		Mat3,
		Mat4,
		False,
		True
	};
	
	static uint32_t GetDataSize(ElementDataType type) {

		switch (type) {
		case ElementDataType::Char:      return sizeof(char);
		case ElementDataType::Char3:     return 3 * sizeof(char);
		case ElementDataType::Int:      return sizeof(int);
		case ElementDataType::Int2:     return 2 * sizeof(int);
		case ElementDataType::Int3:     return 3 * sizeof(int);
		case ElementDataType::Int4:     return 4 * sizeof(int);

		case ElementDataType::Float:    return sizeof(float);
		case ElementDataType::Float2:   return 2 * sizeof(float);
		case ElementDataType::Float3:   return 3 * sizeof(float);
		case ElementDataType::Float4:   return 4 * sizeof(float);

		case ElementDataType::Mat3:		return 3 * 3 * sizeof(float);
		case ElementDataType::Mat4:		return 4 * 4 * sizeof(float);
		case ElementDataType::False:	return 1;
		case ElementDataType::True:		return 1;


		}
		GE_ASSERT(false, "Unknown ElementDataType!")
			return 0;
	}


	struct BufferElement {
		ElementDataType Type;
		std::string Name;
		bool Normalized;
		uint32_t Offset;
		uint32_t ElementSize;
		uint32_t Location;
		BufferElement() {

		}
		BufferElement(ElementDataType type, std::string name, bool normalized, uint32_t location)
			:Type(type),
			Name(name),
			Normalized(normalized),
			Offset(0),
			ElementSize(GetDataSize(type)),
			Location(location) {
		}

		uint32_t GetElementCount() {
			switch (Type) {
			case ElementDataType::Int:      return 1;
			case ElementDataType::Int2:     return 2;
			case ElementDataType::Int3:     return 3;
			case ElementDataType::Int4:     return 4;
			case ElementDataType::Float:    return 1;
			case ElementDataType::Float2:   return 2;
			case ElementDataType::Float3:   return 3;
			case ElementDataType::Float4:   return 4;
			case ElementDataType::Mat3:		return 3 * 3;
			case ElementDataType::Mat4:		return 4 * 4;
			case ElementDataType::False:	return 1;
			case ElementDataType::True:		return 1;
			}
			GE_ASSERT(false, "Unknown ElementDataType!")
				return 0;
		}
		void operator=(const BufferElement& rhs) {
			Type = rhs.Type;
			Name = rhs.Name;
			Normalized = rhs.Normalized;
			Offset = rhs.Offset;
			ElementSize = rhs.ElementSize;
			Location = rhs.Location;
			//return *this;
		}
	};


    struct VertexAttributeDesc
    {
        string name;
        Format format = Format::UNKNOWN;
        uint32_t streamIndex = 0;
        uint32_t arraySize = 1;
        uint32_t bufferIndex = 0;
        uint32_t offset = 0;
        // note: for most APIs, all strides for a given bufferIndex must be identical
        uint32_t elementStride = 0;
        bool isInstanced = false;
        bool divisor = false;
        bool bNormalized = true;
		bool bShouldConvertToFloat = false;

        VertexAttributeDesc& setName(const string& value) { name = value; return *this; }
        constexpr VertexAttributeDesc& setFormat(Format value) { format = value; return *this; }
        constexpr VertexAttributeDesc& setArraySize(uint32_t value) { arraySize = value; return *this; }
        constexpr VertexAttributeDesc& setBufferIndex(uint32_t value) { bufferIndex = value; return *this; }
        constexpr VertexAttributeDesc& setOffset(uint32_t value) { offset = value; return *this; }
        constexpr VertexAttributeDesc& setElementStride(uint32_t value) { elementStride = value; return *this; }
        constexpr VertexAttributeDesc& setIsInstanced(bool value) { isInstanced = value; return *this; }
    };

    class IInputLayout : public IResource
    {
    public:
        [[nodiscard]] virtual uint32_t getNumAttributes() const = 0;
        [[nodiscard]] virtual const VertexAttributeDesc* getAttributeDesc(uint32_t index) const = 0;
    };

    typedef RefCountPtr<IInputLayout> InputLayoutHandle;


}
