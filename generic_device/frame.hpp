#pragma once

#include "future.hpp"
#include "object.hpp"
#include "resource.hpp"

namespace generic {

    class Frame : public Object
    {
    public:
        Frame();
       ~Frame();
        void invokeContinuation(ANARIDevice device) const;
        const void* map(const char* channel);

        int wait(ANARIWaitMask m);

        ResourceHandle getResourceHandle();

        void commit();

        void release();

        void retain();

        void setParameter(const char* name,
                          ANARIDataType type,
                          const void* mem);

        void unsetParameter(const char* name);
        void setDuration(float d);
        bool futureIsValid() const;
        void renewFuture();
        FuturePtr future();
        int getProperty(const char* name,
                        ANARIDataType type,
                        void* mem,
                        uint64_t size,
                        uint32_t waitMask);

        ANARIWorld world = nullptr;
        ANARICamera camera = nullptr;
        ANARIRenderer renderer = nullptr;
        unsigned size[2] = {0,0};
        ANARIDataType color;
        ANARIDataType depth;
        ANARIFrameCompletionCallback frame_completion_callback = nullptr;
        void* frame_completion_callback_user_data = nullptr;
        std::future<void> renderFuture;
        FuturePtr m_future;
        // last duration for rendering
        float duration = 0.f;

    private:
        ANARIFrame resourceHandle = nullptr;
    };

} // generic


