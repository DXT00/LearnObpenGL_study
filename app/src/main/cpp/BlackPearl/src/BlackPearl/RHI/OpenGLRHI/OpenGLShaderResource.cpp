#include "pch.h"
#include "OpenGLShaderResource.h"
#include "OpenGLShader.h"
#include "OpenGLBindingSet.h"
#include "OpenGLTexture.h"
#include "OpenGLSampler.h"
#include "BlackPearl/RHI/Common/RHIUtils.h"

namespace BlackPearl {
	FOpenGLShaderParameterCache::FOpenGLShaderParameterCache()
	{
	}
	FOpenGLShaderParameterCache::~FOpenGLShaderParameterCache()
	{
	}
	void FOpenGLShaderParameterCache::InitializeResources(int32_t UniformArraySize)
	{
	}
	void FOpenGLShaderParameterCache::MarkAllDirty()
	{
	}
	void FOpenGLShaderParameterCache::Set(uint32_t BufferIndex, uint32_t ByteOffset, uint32_t NumBytes, const void* NewValues)
	{
	}
	void FOpenGLShaderParameterCache::CommitPackedGlobals(const FOpenGLLinkedProgram* LinkedProgram, int32_t Stage)
	{
	}

	void FOpenGLShaderParameterCache::CommitPackedUniformBuffers(FOpenGLLinkedProgram* LinkedProgram, int32_t Stage, const std::vector<IBindingSet*> bindings)
	{
		for (size_t i = 0; i < bindings.size(); i++)
		{
			BindingSet* bindingSet = static_cast<BindingSet*>(bindings[i]);
			for (BindingSetItem& binding : bindingSet->desc.bindings)
			{

				switch (binding.type)
				{
				case RHIResourceType::RT_Texture_SRV:
				{
					 Texture* texture = static_cast<Texture*>(binding.resourceHandle);

					//const auto subresource = binding.subresources.resolve(texture->getDesc(), false);
					//const auto textureViewType = getTextureViewType(binding.format, texture->desc.format);
					//auto& view = texture->getSubresourceView(subresource, binding.dimension, binding.format, textureViewType);

					//VkDescriptorImageInfo& imageInfo = descriptorImageInfo.emplace_back();


					////VkDescriptorImageInfo imageInfo;
					//imageInfo.imageView = view.view;
					//imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					/*		imageInfo = VkDescriptorImageInfo()
								.setImageView(view.view)
								.setImageLayout(vk::ImageLayout::eShaderReadOnlyOptimal);*/

					/*generateWriteDescriptorData(layoutBinding.binding,
						layoutBinding.descriptorType,
						&imageInfo, nullptr, nullptr);*/
				}

				break;

				case RHIResourceType::RT_Texture_UAV:
				{
					Texture* texture = static_cast<Texture*>(binding.resourceHandle);

					const auto subresource = binding.subresources.resolve(texture->desc, true);
					const auto textureViewType = getTextureViewType(binding.format, texture->desc.format);
					auto& view = texture->getSubresourceView(subresource, binding.dimension, binding.format, textureViewType);

					//VkDescriptorImageInfo& imageInfo = descriptorImageInfo.emplace_back();
					////imageInfo = vk::DescriptorImageInfo()
					////	.setImageView(view.view)
					////	.setImageLayout(vk::ImageLayout::eGeneral);


					////VkDescriptorImageInfo imageInfo;
					//imageInfo.imageView = view.view;
					//imageInfo.imageLayout = VK_IMAGE_LAYOUT_GENERAL;

					//generateWriteDescriptorData(layoutBinding.binding,
					//	layoutBinding.descriptorType,
					//	&imageInfo, nullptr, nullptr);
				}

				break;

				case RHIResourceType::RT_TypedBuffer_SRV:
				case RHIResourceType::RT_TypedBuffer_UAV:
				{
					Buffer* buffer = static_cast<Buffer*>(binding.resourceHandle);

					//auto vkformat = VkUtil::convertFormat(binding.format);

					//const auto range = binding.range.resolve(buffer->desc);
					//uint64_t viewInfoHash = 0;
					//hash_combine(viewInfoHash, range.byteOffset);
					//hash_combine(viewInfoHash, range.byteSize);
					//hash_combine(viewInfoHash, (uint64_t)vkformat);

					//const auto& bufferViewFound = buffer->viewCache.find(viewInfoHash);
					//auto& bufferViewRef = (bufferViewFound != buffer->viewCache.end()) ? bufferViewFound->second : buffer->viewCache[viewInfoHash];
					//if (bufferViewFound == buffer->viewCache.end())
					//{
					//	assert(binding.format != Format::UNKNOWN);

					//	VkBufferViewCreateInfo bufferViewInfo{};
					//	bufferViewInfo.sType = VK_STRUCTURE_TYPE_BUFFER_VIEW_CREATE_INFO;
					//	bufferViewInfo.buffer = buffer->buffer;
					//	bufferViewInfo.offset = range.byteOffset;
					//	bufferViewInfo.range = range.byteSize;
					//	bufferViewInfo.format = vkformat;

					//	//vkCreateBufferView(m_Context.device, &bufferViewInfo, m_Context.allocationCallbacks, &bufferViewRef);
					//	if (vkCreateBufferView(m_Context.device, &bufferViewInfo, m_Context.allocationCallbacks, &bufferViewRef) != VK_SUCCESS) {
					//		throw std::runtime_error("failed to create buffer view!");
					//	}
					//	/*	 VkBufferViewCreateInfo bufferViewInfo{}
					//			.setBuffer(buffer->buffer)
					//			.setOffset(range.byteOffset)
					//			.setRange(range.byteSize)
					//			.setFormat(vk::Format(vkformat));

					//		res = m_Context.device.createBufferView(&bufferViewInfo, m_Context.allocationCallbacks, &bufferViewRef);
					//		ASSERT_VK_OK(res);*/
					//}

					//generateWriteDescriptorData(layoutBinding.binding,
					//	layoutBinding.descriptorType,
					//	nullptr, nullptr, &bufferViewRef);
				}
				break;

				case RHIResourceType::RT_StructuredBuffer_SRV:
				case RHIResourceType::RT_StructuredBuffer_UAV:
				case RHIResourceType::RT_RawBuffer_SRV:
				case RHIResourceType::RT_RawBuffer_UAV:
				case RHIResourceType::RT_ConstantBuffer:
				case RHIResourceType::RT_VolatileConstantBuffer:
				{
					Buffer* buffer = static_cast<Buffer*>(binding.resourceHandle);

					//const auto range = binding.range.resolve(buffer->desc);

					//auto& bufferInfo = descriptorBufferInfo.emplace_back();
					////VkDescriptorBufferInfo bufferInfo{};

					//bufferInfo.buffer = buffer->buffer;
					//bufferInfo.offset = range.byteOffset;
					//bufferInfo.range = range.byteSize;

					//assert(buffer->buffer);
					//generateWriteDescriptorData(layoutBinding.binding,
					//	layoutBinding.descriptorType,
					//	nullptr, &bufferInfo, nullptr);
				}

				break;

				case RHIResourceType::RT_Sampler:
				{
					const auto& sampler = static_cast<Sampler*>(binding.resourceHandle);

					//VkDescriptorImageInfo& imageInfo = descriptorImageInfo.emplace_back();
					//imageInfo.sampler = sampler->sampler;
					///*	imageInfo = vk::DescriptorImageInfo()
					//		.setSampler(sampler->sampler);*/

					//generateWriteDescriptorData(layoutBinding.binding,
					//	layoutBinding.descriptorType,
					//	&imageInfo, nullptr, nullptr);
				}

				break;

				case RHIResourceType::RT_RayTracingAccelStruct:
					RHIUtils::NotImplemented();
					break;

				case RHIResourceType::RT_PushConstants:
					RHIUtils::NotSupported();
					break;

				case RHIResourceType::RT_None:
				case RHIResourceType::RT_Count:
				default:
					RHIUtils::InvalidEnum();
				}
			}
			}
		

	}
	void FOpenGLShaderParameterCache::CommitPackedUniformBuffers(FOpenGLLinkedProgram* LinkedProgram, int32_t Stage, const std::vector<IBindingSet*> bindings)
	{
		//SCOPE_CYCLE_COUNTER(STAT_OpenGLConstantBufferUpdateTime);
		//VERIFY_GL_SCOPE();

		// Uniform Buffers are split into precision/type; the list of RHI UBs is traversed and if a new one was set, its
		// contents are copied per precision/type into corresponding scratch buffers which are then uploaded to the program
		const FOpenGLShaderBindings& Bindings = LinkedProgram->Config.Shaders[Stage].Bindings;
		//check(Bindings.NumUniformBuffers <= FOpenGLRHIState::MAX_UNIFORM_BUFFERS_PER_SHADER_STAGE);

		if (Bindings.bFlattenUB)
		{
			int32_t LastInfoIndex = 0;
			for (int32_t BufferIndex = 0; BufferIndex < Bindings.NumUniformBuffers; ++BufferIndex)
			{
				const OpenGLUniformBuffer* UniformBuffer = (OpenGLUniformBuffer*)RHIUniformBuffers[BufferIndex];
				assert(UniformBuffer);

				if (!UniformBuffer->bIsEmulatedUniformBuffer)
				{
					continue;
				}

				const uint32_t*  SourceData = UniformBuffer->EmulatedBufferData->Data.GetData();
				for (int32_t InfoIndex = LastInfoIndex; InfoIndex < UniformBuffersCopyInfo.Num(); ++InfoIndex)
				{
					const CrossCompiler::FUniformBufferCopyInfo& Info = UniformBuffersCopyInfo[InfoIndex];
					if (Info.SourceUBIndex == BufferIndex)
					{
						assert((Info.DestOffsetInFloats + Info.SizeInFloats) * sizeof(float) <= (uint32_t)GlobalUniformArraySize);
						float* RESTRICT ScratchMem = (float*)PackedGlobalUniforms[Info.DestUBTypeIndex];
						ScratchMem += Info.DestOffsetInFloats;
						FMemory::Memcpy(ScratchMem, SourceData + Info.SourceOffsetInFloats, Info.SizeInFloats * sizeof(float));
						PackedGlobalUniformDirty[Info.DestUBTypeIndex].MarkDirtyRange(Info.DestOffsetInFloats / NumFloatsInFloat4, (Info.SizeInFloats + NumFloatsInFloat4 - 1) / NumFloatsInFloat4);
					}
					else
					{
						LastInfoIndex = InfoIndex;
						break;
					}
				}
			}
		}
		else
		{
			const auto& PackedUniformBufferInfos = LinkedProgram->StagePackedUniformInfo[Stage].PackedUniformBufferInfos;
			int32_t LastCopyInfoIndex = 0;
			auto& EmulatedUniformBufferSet = LinkedProgram->StagePackedUniformInfo[Stage].LastEmulatedUniformBufferSet;
			for (int32_t BufferIndex = 0; BufferIndex < Bindings.NumUniformBuffers; ++BufferIndex)
			{
				const OpenGLUniformBuffer* UniformBuffer = (OpenGLUniformBuffer*)RHIUniformBuffers[BufferIndex];

				if (UniformBuffer && !UniformBuffer->bIsEmulatedUniformBuffer)
				{
					continue;
				}

				// Workaround for null UBs (FORT-323429), additional logging here is to give us a chance to investigate the higher level issue causing the null UB.
//#if !UE_BUILD_SHIPPING
//				UE_CLOG(UniformBuffer == nullptr && EmulatedUniformBufferSet.IsValidIndex(BufferIndex), LogRHI, Fatal, TEXT("CommitPackedUniformBuffers null UB stage %d, idx %d (%d), %s"), Stage, BufferIndex, EmulatedUniformBufferSet.Num(), *LinkedProgram->Config.ProgramKey.ToString());
//#endif
				if (UniformBuffer && EmulatedUniformBufferSet.IsValidIndex(BufferIndex) && EmulatedUniformBufferSet[BufferIndex] != UniformBuffer->UniqueID)
				{
					EmulatedUniformBufferSet[BufferIndex] = UniformBuffer->UniqueID;

					// Go through the list of copy commands and perform the appropriate copy into the scratch buffer
					for (int32_t InfoIndex = LastCopyInfoIndex; InfoIndex < UniformBuffersCopyInfo.Num(); ++InfoIndex)
					{
						const CrossCompiler::FUniformBufferCopyInfo& Info = UniformBuffersCopyInfo[InfoIndex];
						if (Info.SourceUBIndex == BufferIndex)
						{
							const uint32_t*  SourceData = UniformBuffer->EmulatedBufferData->Data.GetData();
							SourceData += Info.SourceOffsetInFloats;
							float*  ScratchMem = (float*)PackedUniformsScratch[Info.DestUBTypeIndex];
							ScratchMem += Info.DestOffsetInFloats;
							FMemory::Memcpy(ScratchMem, SourceData, Info.SizeInFloats * sizeof(float));
						}
						else if (Info.SourceUBIndex > BufferIndex)
						{
							// Done finding current copies
							LastCopyInfoIndex = InfoIndex;
							break;
						}

						// keep going since we could have skipped this loop when skipping cached UBs...
					}

					// Upload the split buffers to the program
					const auto& UniformBufferUploadInfoList = PackedUniformBufferInfos[BufferIndex];
					for (int32_t InfoIndex = 0; InfoIndex < UniformBufferUploadInfoList.size(); ++InfoIndex)
					{
						auto& UBInfo = Bindings.PackedUniformBuffers[BufferIndex];
						const auto& UniformInfo = UniformBufferUploadInfoList[InfoIndex];
						if (UniformInfo.Location < 0)
						{
							// Optimized out
							continue;
						}

						const void*  UniformData = PackedUniformsScratch[UniformInfo.Index];
						int32_t NumVectors = UBInfo[InfoIndex].Size / sizeof(float4);
						assert(UniformInfo.ArrayType == UBInfo[InfoIndex].TypeName);
						switch (UniformInfo.Index)
						{
						case CrossCompiler::PACKED_TYPEINDEX_HIGHP:
						case CrossCompiler::PACKED_TYPEINDEX_MEDIUMP:
						case CrossCompiler::PACKED_TYPEINDEX_LOWP:
							FOpenGL::ProgramUniform4fv(LinkedProgram->Config.Shaders[Stage].Resource, UniformInfo.Location, NumVectors, (GLfloat*)UniformData);
							break;

						case CrossCompiler::PACKED_TYPEINDEX_INT:
							FOpenGL::ProgramUniform4iv(LinkedProgram->Config.Shaders[Stage].Resource, UniformInfo.Location, NumVectors, (GLint*)UniformData);
							break;

						case CrossCompiler::PACKED_TYPEINDEX_UINT:
							FOpenGL::ProgramUniform4uiv(LinkedProgram->Config.Shaders[Stage].Resource, UniformInfo.Location, NumVectors, (GLuint*)UniformData);
							break;
						}
					}
				}
			}
		}
	}
	void FOpenGLShaderParameterCache::FRange::MarkDirtyRange(uint32_t NewStartVector, uint32_t NewNumVectors)
	{
	}
}

