/*
 * Copyright (C) 2017-2018 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#pragma once
#include "runtime/memory_manager/memory_constants.h"
#include "runtime/mem_obj/mem_obj.h"
#include "runtime/helpers/basic_math.h"
#include "public/cl_ext_private.h"
#include "igfxfmid.h"

namespace OCLRT {
class Buffer;
class Device;
class MemoryManager;

typedef Buffer *(*BufferCreatFunc)(Context *context,
                                   cl_mem_flags flags,
                                   size_t size,
                                   void *memoryStorage,
                                   void *hostPtr,
                                   GraphicsAllocation *gfxAllocation,
                                   bool zeroCopy,
                                   bool isHostPtrSVM,
                                   bool isImageRedescribed);

typedef struct {
    BufferCreatFunc createBufferFunction;
} BufferFuncs;

extern BufferFuncs bufferFactory[IGFX_MAX_CORE];

class Buffer : public MemObj {
  public:
    const static size_t maxBufferSizeForReadWriteOnCpu = 10 * MB;
    const static cl_ulong maskMagic = 0xFFFFFFFFFFFFFFFFLL;
    static const cl_ulong objectMagic = MemObj::objectMagic | 0x02;
    bool forceDisallowCPUCopy = false;

    ~Buffer() override;

    static void validateInputAndCreateBuffer(cl_context &context,
                                             MemoryProperties properties,
                                             size_t size,
                                             void *hostPtr,
                                             cl_int &retVal,
                                             cl_mem &buffer);

    static Buffer *create(Context *context,
                          cl_mem_flags flags,
                          size_t size,
                          void *hostPtr,
                          cl_int &errcodeRet);

    static Buffer *create(Context *context,
                          MemoryProperties properties,
                          size_t size,
                          void *hostPtr,
                          cl_int &errcodeRet);

    static Buffer *createSharedBuffer(Context *context,
                                      cl_mem_flags flags,
                                      SharingHandler *sharingHandler,
                                      GraphicsAllocation *graphicsAllocation);

    static Buffer *createBufferHw(Context *context,
                                  cl_mem_flags flags,
                                  size_t size,
                                  void *memoryStorage,
                                  void *hostPtr,
                                  GraphicsAllocation *gfxAllocation,
                                  bool zeroCopy,
                                  bool isHostPtrSVM,
                                  bool isImageRedescribed);

    static Buffer *createBufferHwFromDevice(const Device *device,
                                            cl_mem_flags flags,
                                            size_t size,
                                            void *memoryStorage,
                                            void *hostPtr,
                                            GraphicsAllocation *gfxAllocation,
                                            bool zeroCopy,
                                            bool isHostPtrSVM,
                                            bool isImageRedescribed);

    Buffer *createSubBuffer(cl_mem_flags flags,
                            const cl_buffer_region *region,
                            cl_int &errcodeRet);

    static void setSurfaceState(const Device *device,
                                void *surfaceState,
                                size_t svmSize,
                                void *svmPtr,
                                GraphicsAllocation *gfxAlloc = nullptr,
                                cl_mem_flags flags = 0);

    BufferCreatFunc createFunction = nullptr;
    bool isSubBuffer();
    bool isValidSubBufferOffset(size_t offset);
    uint64_t setArgStateless(void *memory, uint32_t patchSize) { return setArgStateless(memory, patchSize, false); }
    uint64_t setArgStateless(void *memory, uint32_t patchSize, bool set32BitAddressing);
    virtual void setArgStateful(void *memory, bool forceNonAuxMode) = 0;
    bool bufferRectPitchSet(const size_t *bufferOrigin,
                            const size_t *region,
                            size_t &bufferRowPitch,
                            size_t &bufferSlicePitch,
                            size_t &hostRowPitch,
                            size_t &hostSlicePitch);

    static size_t calculateHostPtrSize(const size_t *origin, const size_t *region, size_t rowPitch, size_t slicePitch);

    void transferDataToHostPtr(MemObjSizeArray &copySize, MemObjOffsetArray &copyOffset) override;
    void transferDataFromHostPtr(MemObjSizeArray &copySize, MemObjOffsetArray &copyOffset) override;

    bool isReadWriteOnCpuAllowed(cl_bool blocking, cl_uint numEventsInWaitList, void *ptr, size_t size);

  protected:
    Buffer(Context *context,
           cl_mem_flags flags,
           size_t size,
           void *memoryStorage,
           void *hostPtr,
           GraphicsAllocation *gfxAllocation,
           bool zeroCopy,
           bool isHostPtrSVM,
           bool isObjectRedescribed);

    Buffer();

    static void checkMemory(cl_mem_flags flags,
                            size_t size,
                            void *hostPtr,
                            cl_int &errcodeRet,
                            bool &isZeroCopy,
                            bool &copyMemoryFromHostPtr,
                            MemoryManager *memMngr);
    static GraphicsAllocation::AllocationType getGraphicsAllocationType(cl_mem_flags flags, bool sharedContext, bool renderCompressedBuffers);
    static bool isReadOnlyMemoryPermittedByFlags(cl_mem_flags flags);

    void transferData(void *dst, void *src, size_t copySize, size_t copyOffset);
};

template <typename GfxFamily>
class BufferHw : public Buffer {
  public:
    BufferHw(Context *context,
             cl_mem_flags flags,
             size_t size,
             void *memoryStorage,
             void *hostPtr,
             GraphicsAllocation *gfxAllocation,
             bool zeroCopy,
             bool isHostPtrSVM,
             bool isObjectRedescribed)
        : Buffer(context, flags, size, memoryStorage, hostPtr, gfxAllocation,
                 zeroCopy, isHostPtrSVM, isObjectRedescribed) {}

    void setArgStateful(void *memory, bool forceNonAuxMode) override;

    static Buffer *create(Context *context,
                          cl_mem_flags flags,
                          size_t size,
                          void *memoryStorage,
                          void *hostPtr,
                          GraphicsAllocation *gfxAllocation,
                          bool zeroCopy,
                          bool isHostPtrSVM,
                          bool isObjectRedescribed) {
        auto buffer = new BufferHw<GfxFamily>(context,
                                              flags,
                                              size,
                                              memoryStorage,
                                              hostPtr,
                                              gfxAllocation,
                                              zeroCopy,
                                              isHostPtrSVM,
                                              isObjectRedescribed);
        buffer->surfaceType = SURFACE_STATE::SURFACE_TYPE_SURFTYPE_1D;
        return buffer;
    }

    typedef typename GfxFamily::RENDER_SURFACE_STATE SURFACE_STATE;
    typename SURFACE_STATE::SURFACE_TYPE surfaceType;
};
} // namespace OCLRT
