#pragma once
#include "RHIDefinitions.h"
#include "RefCountPtr.h"
#include "RHIResources.h"
#include "RHIInputLayout.h"
#include "BlackPearl/Core.h"
namespace BlackPearl {
    struct BufferDesc
    {
        uint64_t byteSize = 0;
        uint32_t structStride = 0; // if non-zero it's structured
        uint32_t maxVersions = 0; // only valid and required to be nonzero for volatile buffers on Vulkan
        std::string debugName;
        Format format = Format::UNKNOWN; // for typed buffer views
        bool canHaveUAVs = false;
        bool canHaveTypedViews = false;
        bool canHaveRawViews = false;
        bool isVertexBuffer = false;
        bool isIndexBuffer = false;
        bool isConstantBuffer = false;
        bool isDrawIndirectArgs = false;
        bool isAccelStructBuildInput = false;
        bool isAccelStructStorage = false;
        bool isShaderBindingTable = false;

		UniformBufferLayout* uniformBufferLayout;
        // A dynamic/upload buffer whose contents only live in the current command list
        bool isVolatile = false;

		//todo:: only gl
		bool isDynamic = false;
        // Indicates that the buffer is created with no backing memory,
        // and memory is bound to the texture later using bindBufferMemory.
        // On DX12, the buffer resource is created at the time of memory binding.
        bool isVirtual = false;

        ResourceStates initialState = ResourceStates::Common;

        // see TextureDesc::keepInitialState
        bool keepInitialState = false;

        CpuAccessMode cpuAccess = CpuAccessMode::None;

        SharedResourceFlags sharedResourceFlags = SharedResourceFlags::None;

        constexpr BufferDesc& setByteSize(uint64_t value) { byteSize = value; return *this; }
        constexpr BufferDesc& setStructStride(uint32_t value) { structStride = value; return *this; }
        constexpr BufferDesc& setMaxVersions(uint32_t value) { maxVersions = value; return *this; }
        BufferDesc& setDebugName(const std::string& value) { debugName = value; return *this; }
        constexpr BufferDesc& setFormat(Format value) { format = value; return *this; }
        constexpr BufferDesc& setCanHaveUAVs(bool value) { canHaveUAVs = value; return *this; }
        constexpr BufferDesc& setCanHaveTypedViews(bool value) { canHaveTypedViews = value; return *this; }
        constexpr BufferDesc& setCanHaveRawViews(bool value) { canHaveRawViews = value; return *this; }
        constexpr BufferDesc& setIsVertexBuffer(bool value) { isVertexBuffer = value; return *this; }
        constexpr BufferDesc& setIsIndexBuffer(bool value) { isIndexBuffer = value; return *this; }
        constexpr BufferDesc& setIsConstantBuffer(bool value) { isConstantBuffer = value; return *this; }
        constexpr BufferDesc& setIsDrawIndirectArgs(bool value) { isDrawIndirectArgs = value; return *this; }
        constexpr BufferDesc& setIsAccelStructBuildInput(bool value) { isAccelStructBuildInput = value; return *this; }
        constexpr BufferDesc& setIsAccelStructStorage(bool value) { isAccelStructStorage = value; return *this; }
        constexpr BufferDesc& setIsShaderBindingTable(bool value) { isShaderBindingTable = value; return *this; }
        constexpr BufferDesc& setIsVolatile(bool value) { isVolatile = value; return *this; }
        constexpr BufferDesc& setIsVirtual(bool value) { isVirtual = value; return *this; }
        constexpr BufferDesc& setInitialState(ResourceStates value) { initialState = value; return *this; }
        constexpr BufferDesc& setKeepInitialState(bool value) { keepInitialState = value; return *this; }
        constexpr BufferDesc& setCpuAccess(CpuAccessMode value) { cpuAccess = value; return *this; }
    };

    struct BufferRange
    {
        uint64_t byteOffset = 0;
        uint64_t byteSize = 0;

        BufferRange() = default;

        BufferRange(uint64_t _byteOffset, uint64_t _byteSize)
            : byteOffset(_byteOffset)
            , byteSize(_byteSize)
        { }

        [[nodiscard]] BufferRange resolve(const BufferDesc& desc) const;
        [[nodiscard]] constexpr bool isEntireBuffer(const BufferDesc& desc) const { return (byteOffset == 0) && (byteSize == ~0ull || byteSize == desc.byteSize); }
        constexpr bool operator== (const BufferRange& other) const { return byteOffset == other.byteOffset && byteSize == other.byteSize; }

        constexpr BufferRange& setByteOffset(uint64_t value) { byteOffset = value; return *this; }
        constexpr BufferRange& setByteSize(uint64_t value) { byteSize = value; return *this; }
    };

    static const BufferRange EntireBuffer = BufferRange(0, ~0ull);

    class IBuffer :public IResource
    {
    public:
        [[nodiscard]] virtual const BufferDesc& getDesc() const = 0;
    };


	class VertexBufferLayout {
	public:
		VertexBufferLayout() { //GE_CORE_INFO("VertexBufferLayout defult constructor!")
		}
		VertexBufferLayout(std::initializer_list<BufferElement> elements)
			:m_Elememts(elements) {
			CalculateStrideAndOffset();
			UpdateDesc();

		};
		void CalculateStrideAndOffset();

		inline std::vector<BufferElement> GetElements() const { return m_Elememts; }
		inline BufferElement GetElement(uint32_t i) const {
			GE_ASSERT((i < ElementSize()), "i exceed max elements size");
			return m_Elememts[i];
		}

		bool HasElement(uint32_t i) {
			if (i < ElementSize())
				return true;
			return false;
		}
		void AddElement(const BufferElement& element) {
			m_Elememts.push_back(element);
			CalculateStrideAndOffset();
			UpdateDesc();
		}
		uint32_t ElementSize() const { return m_Elememts.size(); }
		uint32_t GetStride() const { return m_Stride; }
		//for directx
		virtual void UpdateDesc() {}

		VertexBufferLayout(const VertexBufferLayout& rhs) {
			m_Elememts.resize(rhs.ElementSize());
			for (size_t i = 0; i < rhs.ElementSize(); i++)
			{
				m_Elememts[i] = rhs.m_Elememts[i];
			}
			m_Stride = rhs.m_Stride;

		}
		VertexBufferLayout& operator = (const VertexBufferLayout& rhs) {
			this->m_Elememts.resize(rhs.ElementSize());
			for (size_t i = 0; i < rhs.ElementSize(); i++)
			{
				this->m_Elememts[i] = rhs.m_Elememts[i];
			}
			this->m_Stride = rhs.m_Stride;
			return *this;
		}

	protected:
		std::vector<BufferElement> m_Elememts;
		uint32_t m_Stride = 0;
	};

    typedef RefCountPtr<IBuffer> BufferHandle;

}

