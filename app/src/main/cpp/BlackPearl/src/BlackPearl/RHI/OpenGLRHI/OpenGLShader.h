#pragma once
#include "glad/glad.h"
#include<glm/glm.hpp>
#include<string>
#include<memory>
#include <unordered_map>
#include "BlackPearl/Math/vector.h"
#include "BlackPearl/RHI/RHIShader.h"
#include <BlackPearl/Renderer/Renderer.h>
#include "OpenGLShaderResource.h"

namespace BlackPearl {

	class LightSources;
	class BindingSet;
	class Shader : public RefCounter<IShader>
	{
	public:
		ShaderDesc desc;
		//TODO:: use glProgramBinary
		Shader(const ShaderDesc& _desc, const void* binaryCode, size_t binarySize);
		Shader(const ShaderDesc& _desc, const std::string& filepath);
		~Shader();
		void Bind()const;
		void Unbind() const;
		std::string ReadFile(const std::string& filepath);
		
		std::unordered_map<GLenum, std::string> Shader::PreProcess(const std::string& source, const std::string& commonSource);
		void Compile(const std::unordered_map<GLenum, std::string>& shaderSources);
		void Compile(GLenum shaderType);
		void SetLightUniform(LightSources lightSources);

		void SetUniform1i(const std::string &name, int val) const;
		void SetUniform1ui(const std::string & name, const unsigned int val) const;
		void SetUniform1f(const std::string &name, float val) const;
		void SetUniformMat4f(const std::string &name, const glm::mat4 &mat) const;
		void SetUniformMat4f(const std::string& name, const float* mat4x4, uint32_t count) const;

		void SetUniformMat3x4f(const std::string& name, const float* mat3x4, uint32_t count = 1) const;

		void SetUniformVec3f(const std::string & name, const glm::vec3& value) const;
		void SetUniformVec2f(const std::string& name, const glm::vec2& value) const;
		void SetUniformVec3f(const std::string& name, const math::float3& value) const;
		void SetUniformVec2f(const std::string& name, const math::float2& value) const;
		void SetUniformVec2i(const std::string& name, const glm::ivec2& value) const;
		

		std::string GetPath() { return m_ShaderPath; }
		const ShaderDesc& getDesc() const override { return desc; }
		void getBytecode(const void** ppBytecode, size_t* pSize) const override;

        GLuint m_ShaderID = 0;
		/** External bindings for this shader. */
		FOpenGLShaderBindings Bindings;

		FOpenGLCompiledShaderKey ShaderCodeKey;

    private:
		Shader(
			const std::string& vertexSrc, 
			const std::string& fragmentSrc, 
			const std::string& geometrySrc, 
			const std::string& tessCtlSrc,
			const std::string& tessEvlSrc);

        GLuint m_RendererID = -1;
		std::string m_ShaderPath;
		std::string m_GlslCode;

		std::string m_FragmentCommonStruct;
		std::string m_CommonStructPath="assets/shaders/common/CommonStruct.glsl";

		/*
		 
		  
		FragmentShader = FShaderCodeLibrary::CreatePixelShader(Platform, PSO.GraphicsDesc.FragmentShader);


		case SF_Vertex: Shader = RHICreateVertexShader(ShaderCodeView, ShaderHash); CheckShaderCreation(Shader, Index); break;
		FPixelShaderRHIRef FShaderCodeLibrary::CreatePixelShader(EShaderPlatform Platform, const FSHAHash& Hash)

		*/
	/*	FOpenGLShader::FOpenGLShader(TArrayView<const uint8> Code, const FSHAHash& Hash, GLenum TypeEnum)
		{
			FMemory::Memzero(&Bindings, sizeof(Bindings));

			FShaderCodeReader ShaderCode(Code);

			FMemoryReaderView Ar(Code, true);

			Ar.SetLimitSize(ShaderCode.GetActualShaderCodeSize());

			FOpenGLCodeHeader Header = { 0 };
			Ar << Header;

			if (Header.GlslMarker != 0x474c534c
				|| (TypeEnum == GL_VERTEX_SHADER && Header.FrequencyMarker != 0x5653)
				|| (TypeEnum == GL_FRAGMENT_SHADER && Header.FrequencyMarker != 0x5053)
				|| (TypeEnum == GL_GEOMETRY_SHADER && Header.FrequencyMarker != 0x4753)
				|| (TypeEnum == GL_COMPUTE_SHADER && Header.FrequencyMarker != 0x4353)
				)
			{
				UE_LOG(LogRHI, Fatal,
					TEXT("Corrupt shader bytecode. GlslMarker=0x%08x FrequencyMarker=0x%04x"),
					Header.GlslMarker,
					Header.FrequencyMarker
				);
				return;
			}

			Bindings = Header.Bindings;
			UniformBuffersCopyInfo = Header.UniformBuffersCopyInfo;
			UE::RHICore::InitStaticUniformBufferSlots(StaticSlots, Bindings.ShaderResourceTable);

			int32 CodeOffset = Ar.Tell();*/



	};



	/**
	 * Shader binding information. for shader bindingSet
	 */
	struct FOpenGLShaderBindings
	{
		std::vector<std::vector<FPackedArrayInfo>>	PackedUniformBuffers;
		std::vector<FPackedArrayInfo>			PackedGlobalArrays;
		std::vector<FOpenGLShaderVarying>					InputVaryings;
		std::vector<FOpenGLShaderVarying>					OutputVaryings;
	//	FShaderResourceTable							ShaderResourceTable;
		//CrossCompiler::FShaderBindingInOutMask			InOutMask;

		uint8_t	NumSamplers;
		uint8_t	NumUniformBuffers;
		uint8_t	NumUAVs;
		bool	bFlattenUB;

		//FSHAHash VaryingHash; // Not serialized, built during load to allow us to diff varying info but avoid the memory overhead.

		FOpenGLShaderBindings() :
			NumSamplers(0),
			NumUniformBuffers(0),
			NumUAVs(0),
			bFlattenUB(false)
		{
		}

		friend bool operator==(const FOpenGLShaderBindings& A, const FOpenGLShaderBindings& B)
		{
			assert(0);
			return false;

			//bool bEqual = true;

			////bEqual &= A.InOutMask == B.InOutMask;
			//bEqual &= A.NumSamplers == B.NumSamplers;
			//bEqual &= A.NumUniformBuffers == B.NumUniformBuffers;
			//bEqual &= A.NumUAVs == B.NumUAVs;
			//bEqual &= A.bFlattenUB == B.bFlattenUB;
			//bEqual &= A.PackedGlobalArrays.size() == B.PackedGlobalArrays.size();
			//bEqual &= A.PackedUniformBuffers.size() == B.PackedUniformBuffers.size();
			//bEqual &= A.InputVaryings.size() == B.InputVaryings.size();
			//bEqual &= A.OutputVaryings.size() == B.OutputVaryings.size();
			////bEqual &= A.ShaderResourceTable == B.ShaderResourceTable;
			////bEqual &= A.VaryingHash == B.VaryingHash;

			//if (!bEqual)
			//{
			//	return bEqual;
			//}

			//bEqual &= FMemory::Memcmp(A.PackedGlobalArrays.data(), B.PackedGlobalArrays.data(), A.PackedGlobalArrays.GetTypeSize() * A.PackedGlobalArrays.Num()) == 0;

			//for (int32_t Item = 0; bEqual && Item < A.PackedUniformBuffers.size(); Item++)
			//{
			//	const TArray<CrossCompiler::FPackedArrayInfo>& ArrayA = A.PackedUniformBuffers[Item];
			//	const TArray<CrossCompiler::FPackedArrayInfo>& ArrayB = B.PackedUniformBuffers[Item];

			//	bEqual = bEqual && (ArrayA.Num() == ArrayB.Num()) && (FMemory::Memcmp(ArrayA.GetData(), ArrayB.GetData(), ArrayA.GetTypeSize() * ArrayA.Num()) == 0);
			//}


			//for (int32 Item = 0; bEqual && Item < A.InputVaryings.Num(); Item++)
			//{
			//	bEqual &= A.InputVaryings[Item] == B.InputVaryings[Item];
			//}

			//for (int32 Item = 0; bEqual && Item < A.OutputVaryings.Num(); Item++)
			//{
			//	bEqual &= A.OutputVaryings[Item] == B.OutputVaryings[Item];
			//}

			//return bEqual;
		}

		friend uint32_t GetTypeHash(const FOpenGLShaderBindings& Binding)
		{
			//uint32 Hash = 0;
			//Hash = Binding.InOutMask.Bitmask;
			//Hash ^= Binding.NumSamplers << 16;
			//Hash ^= Binding.NumUniformBuffers << 24;
			//Hash ^= Binding.NumUAVs;
			//Hash ^= Binding.bFlattenUB << 8;
			//Hash ^= FCrc::MemCrc_DEPRECATED(Binding.PackedGlobalArrays.GetData(), Binding.PackedGlobalArrays.GetTypeSize() * Binding.PackedGlobalArrays.Num());

			////@todo-rco: Do we need to calc Binding.ShaderResourceTable.GetTypeHash()?

			//for (int32 Item = 0; Item < Binding.PackedUniformBuffers.Num(); Item++)
			//{
			//	const TArray<CrossCompiler::FPackedArrayInfo>& Array = Binding.PackedUniformBuffers[Item];
			//	Hash ^= FCrc::MemCrc_DEPRECATED(Array.GetData(), Array.GetTypeSize() * Array.Num());
			//}

			//for (int32 Item = 0; Item < Binding.InputVaryings.Num(); Item++)
			//{
			//	Hash ^= GetTypeHash(Binding.InputVaryings[Item]);
			//}

			//for (int32 Item = 0; Item < Binding.OutputVaryings.Num(); Item++)
			//{
			//	Hash ^= GetTypeHash(Binding.OutputVaryings[Item]);
			//}

			//Hash ^= GetTypeHash(Binding.VaryingHash);

			//return Hash;
			return 0;
		}
	};

	
	class FOpenGLLinkedProgramConfiguration
	{
	public:

		struct ShaderInfo
		{
			FOpenGLShaderBindings Bindings;
			GLuint Resource;
			FOpenGLCompiledShaderKey ShaderKey; // This is the key to the shader within FOpenGLCompiledShader container
			bool bValid; // To mark that stage is valid for this program, even when shader Resource could be zero
		}
		Shaders[ShaderType::NUM_COMPILE_SHADER_STAGES];
		FOpenGLProgramKey ProgramKey;
		std::vector<BindingSet*> bindingSet;

		FOpenGLLinkedProgramConfiguration()
		{
			for (int32_t Stage = 0; Stage < ShaderType::NUM_COMPILE_SHADER_STAGES; Stage++)
			{
				Shaders[Stage].Resource = 0;
				Shaders[Stage].bValid = false;
			}
		}

		friend bool operator ==(const FOpenGLLinkedProgramConfiguration& A, const FOpenGLLinkedProgramConfiguration& B)
		{
			bool bEqual = true;
			for (int32_t Stage = 0; Stage < ShaderType::NUM_COMPILE_SHADER_STAGES && bEqual; Stage++)
			{
				bEqual &= A.Shaders[Stage].Resource == B.Shaders[Stage].Resource;
				bEqual &= A.Shaders[Stage].bValid == B.Shaders[Stage].bValid;
				bEqual &= A.Shaders[Stage].Bindings == B.Shaders[Stage].Bindings;
			}
			return bEqual;
		}
		//TODO::
		friend uint32_t GetTypeHash(const FOpenGLLinkedProgramConfiguration& Config)
		{
			assert(0);
			return 0;
			//return GetTypeHash(Config.ProgramKey);
		}
	};


	class FOpenGLLinkedProgram {
	public:

		struct FPackedUniformInfo
		{
			GLint	Location;
			//uint8_t	ArrayType;	// OGL_PACKED_ARRAYINDEX_TYPE
			uint8_t	Index;		// OGL_PACKED_INDEX_TYPE
			std::string Name;
		};

		FOpenGLLinkedProgram(const FOpenGLLinkedProgramConfiguration& config, GLuint program)
			:Config(config), Program(program) {

		}
		FOpenGLLinkedProgramConfiguration Config;

		std::vector<FPackedUniformInfo> PackedUniformSamplers;
		std::vector<FPackedUniformInfo> PackedUniformImages;
		std::vector<FPackedUniformInfo> PackedUniformBuffers;


		// Holds information needed per stage regarding packed uniform globals and uniform buffers
		struct FStagePackedUniformInfo
		{
			// Packed Uniform Arrays (regular globals); array elements per precision/type
			std::vector<FPackedUniformInfo>			PackedUniformInfos;

			// Packed Uniform Buffers; outer array is per Uniform Buffer; inner array is per precision/type
			std::vector<std::vector<FPackedUniformInfo>>	PackedUniformBufferInfos;

			// Holds the unique ID of the last uniform buffer uploaded to the program; since we don't reuse uniform buffers
			// (can't modify existing ones), we use this as a check for dirty/need to mem copy on Mobile
			std::vector<uint32_t>						LastEmulatedUniformBufferSet;
		};
		FStagePackedUniformInfo	StagePackedUniformInfo[CrossCompiler::NUM_SHADER_STAGES];
		void ConfigureShaderStage(int Stage, uint32_t FirstUniformBuffer);
		void ConfigureBindingSets(uint32_t FirstUniformBuffer);
		// FOpenGLLinkedProgram(Shader* vertexShader, Shader pixelShader, Shader* geometryShader);
		GLuint		Program;
		bool		bDrawn;
	};

}