#include <string.h>
#include "logging.hpp"
#include "volume.hpp"

namespace generic {

    Volume::Volume()
        : resourceHandle(new std::remove_pointer_t<ANARIVolume>)
    {
    }

    Volume::~Volume()
    {
    }

    ResourceHandle Volume::getResourceHandle()
    {
        return resourceHandle;
    }

    void Volume::commit()
    {
    }

    void Volume::setParameter(const char* name,
                              ANARIDataType type,
                              const void* mem)
    {
        if (strncmp(name,"field",5)==0 && type==ANARI_SPATIAL_FIELD) {
            field = *(ANARISpatialField*)mem; // TODO: reference count
        } else if (strncmp(name,"valueRange",10)==0 && type==ANARI_FLOAT32_BOX1) {
            memcpy(valueRange,mem,sizeof(valueRange));
        } else if (strncmp(name,"color",5)==0 && type==ANARI_ARRAY1D) {
            color = *(ANARIArray1D*)mem; // TODO: reference count
        } else if (strncmp(name,"color.position",14)==0 && type==ANARI_ARRAY1D) {
            color_position = *(ANARIArray1D*)mem; // TODO: reference count
        } else if (strncmp(name,"opacity",7)==0 && type==ANARI_ARRAY1D) {
            opacity = *(ANARIArray1D*)mem; // TODO: reference count
        } else if (strncmp(name,"opacity.position",16)==0 && type==ANARI_ARRAY1D) {
            opacity_position = *(ANARIArray1D*)mem; // TODO: reference count
        } else if (strncmp(name,"densityScale",12)==0 && type==ANARI_FLOAT32) {
            memcpy(&densityScale,mem,sizeof(densityScale));
        } else {
            LOG(logging::Level::Warning) << "Volume: Unsupported parameter "
                << "/ parameter type: " << name << " / " << type;
        }
    }

    std::unique_ptr<Volume> createVolume(const char* subtype)
    {
        (void)subtype;
        return std::make_unique<Volume>();
    }

} // generic


