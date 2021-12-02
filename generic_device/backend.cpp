#include <algorithm>
#include <functional>
#include <iostream>
#include <thread>
#include <visionaray/math/math.h>
#include <visionaray/bvh.h>
#include <visionaray/texture/texture.h>
#include <visionaray/aligned_vector.h>
#include <visionaray/cpu_buffer_rt.h>
#include <visionaray/generic_material.h>
#include <visionaray/kernels.h>
#include <visionaray/material.h>
#include <visionaray/phase_function.h>
#include <visionaray/random_generator.h>
#include <visionaray/sampling.h>
#include <visionaray/scheduler.h>
#include <visionaray/thin_lens_camera.h>
#include "array.hpp"
#include "backend.hpp"
#include "logging.hpp"

using namespace visionaray;

namespace generic {

    namespace backend {
        enum class Algorithm { Pathtracing, AmbientOcclusion, };

        struct RenderTarget : render_target
        {
            auto ref() { return accumBuffer.ref(); }

            void clear_color_buffer(const vec4f& color = vec4f(0.f))
            {
                accumBuffer.clear_color_buffer(color);
            }

            void begin_frame()
            {
                accumBuffer.begin_frame();
            }

            void end_frame()
            {
                accumBuffer.end_frame();

                parallel_for(pool,tiled_range2d<int>(0,width(),64,0,height(),64),
                    [&](range2d<int> r) {
                        for (int y=r.cols().begin(); y!=r.cols().end(); ++y) {
                            for (int x=r.rows().begin(); x!=r.rows().end(); ++x) {
                                 vec4f src = accumBuffer.color()[y*width()+x];

                                 if (1) {
                                     src.xyz() = linear_to_srgb(src.xyz());
                                     int32_t r = clamp(int32_t(src.x*256.f),0,255);
                                     int32_t g = clamp(int32_t(src.y*256.f),0,255);
                                     int32_t b = clamp(int32_t(src.z*256.f),0,255);
                                     int32_t a = clamp(int32_t(src.w*256.f),0,255);
                                     uint32_t &dst = ((uint32_t*)colorPtr)[y*width()+x];

                                     dst = (r<<0) + (g<<8) + (b<<16) + (a<<24);
                                 }
                            }
                        }
                    });
            }

            void resize(int w, int h)
            {
                render_target::resize(w,h);accumBuffer.resize(w,h);
            }

            thread_pool pool{std::thread::hardware_concurrency()};

            cpu_buffer_rt<PF_RGBA32F, PF_DEPTH32F> accumBuffer;

            bool sRGB = false;

            void* colorPtr = nullptr;
            void* depthPtr = nullptr;

            void reset(int width, int height, pixel_format color, pixel_format depth, bool srgb)
            {
                resize(width,height);

                if (color==PF_RGBA8 && depth==PF_UNSPECIFIED) { // TODO: use anari types!
                    colorPtr = new uint32_t[width*height]; // TODO: delete!!!
                } else if (color==PF_RGBA8 && depth==PF_DEPTH32F) {
                    colorPtr = new uint32_t[width*height];
                    depthPtr = new float[width*height];
                } else if (color==PF_RGBA32F && depth==PF_UNSPECIFIED) {
                    colorPtr = new vec4f[width*height];
                } else if (color==PF_RGBA32F && depth==PF_DEPTH32F) {
                    colorPtr = new vec4f[width*height];
                    depthPtr = new float[width*height];
                }

                sRGB = srgb;
            }
        };

        struct StructuredVolume
        {
            StructuredVolume() = default;

            void reset(Volume& volume)
            {
                handle = (ANARIVolume)volume.getResourceHandle();

                ANARISpatialField f = volume.field;
                StructuredRegular* sr = (StructuredRegular*)GetResource(f);
                ANARIArray3D d = sr->data;
                if (d != handle3f) { // new volume data!
                    Array3D* data = (Array3D*)GetResource(d);

                    storage3f = texture<float, 3>((unsigned)data->numItems[0],
                                                  (unsigned)data->numItems[1],
                                                  (unsigned)data->numItems[2]);
                    storage3f.reset((const float*)data->data);
                    storage3f.set_filter_mode(Linear);
                    storage3f.set_address_mode(Clamp);

                    texture3f = texture_ref<float, 3>(storage3f);

                    bbox = aabb({0.f,0.f,0.f},{(float)data->numItems[0],(float)data->numItems[1],(float)data->numItems[2]});

                    handle3f = d;
                }

                ANARIArray1D color = volume.color;
                ANARIArray1D opacity = volume.opacity;

                if (color != handleRGB || opacity != handleA) { // TF changed
                    Array1D* rgb = (Array1D*)GetResource(color);
                    Array1D* a = (Array1D*)GetResource(opacity);

                    aligned_vector<vec4f> rgba(rgb->numItems[0]);
                    for (uint64_t i = 0; i < rgb->numItems[0]; ++i) {
                        vec4f val(((vec3f*)rgb->data)[i],((float*)a->data)[i]);
                        rgba[i] = val;
                    }

                    storageRGBA = texture<vec4f, 1>(rgb->numItems[0]);
                    storageRGBA.reset(rgba.data());
                    storageRGBA.set_filter_mode(Linear);
                    storageRGBA.set_address_mode(Clamp);

                    textureRGBA = texture_ref<vec4f, 1>(storageRGBA);

                    handleRGB = color;
                    handleA = opacity;
                }
            }

            VSNRAY_FUNC
            vec3 albedo(vec3 const& pos)
            {
                float voxel = (float)tex3D(texture3f, pos / bbox.size());

                // normalize to [0..1]
                //voxel = normalize(volume, voxel);

                vec4f rgba = tex1D(textureRGBA, voxel);
                return rgba.xyz();
            }

            VSNRAY_FUNC
            float mu(vec3 const& pos)
            {
                float voxel = (float)tex3D(texture3f, pos / bbox.size());

                // normalize to [0..1]
                //voxel = normalize(volume, voxel);

                vec4f rgba = tex1D(textureRGBA, voxel);
                return rgba.w;
            }

            template <typename Ray>
            VSNRAY_FUNC
            bool sample_interaction(Ray& r, float d, random_generator<float>& gen)
            {
                float t = 0.0f;
                vec3 pos;

                do
                {
                    t -= log(1.0f - gen.next()) / mu_;

                    pos = r.ori + r.dir * t;
                    if (t >= d)
                    {
                        return false;
                    }
                }
                while (mu(pos) < gen.next() * mu_);

                r.ori = pos;
                return true;
            }

            VSNRAY_FUNC
            inline vec3f gradient(vec3f texCoord, vec3f delta)
            {
                return vec3f(+tex3D(texture3f,vec3f(texCoord.x+delta.x,texCoord.y,texCoord.z))
                             -tex3D(texture3f,vec3f(texCoord.x-delta.x,texCoord.y,texCoord.z)),
                             +tex3D(texture3f,vec3f(texCoord.x,texCoord.y+delta.y,texCoord.z))
                             -tex3D(texture3f,vec3f(texCoord.x,texCoord.y-delta.y,texCoord.z)),
                             +tex3D(texture3f,vec3f(texCoord.x,texCoord.y,texCoord.z+delta.z))
                             -tex3D(texture3f,vec3f(texCoord.x,texCoord.y,texCoord.z-delta.z)));
            }

            texture<float, 3> storage3f;
            texture_ref<float, 3> texture3f;
            texture<vec4f, 1> storageRGBA;
            texture_ref<vec4f, 1> textureRGBA;

            aabb bbox;
            float mu_ = 1.f;

            ANARIVolume handle = nullptr;
            ANARIArray3D handle3f = nullptr;
            ANARIArray1D handleRGB = nullptr;
            ANARIArray1D handleA = nullptr;
        };

        typedef index_bvh<basic_triangle<3,float>> TriangleBVH;
        typedef index_bvh<typename TriangleBVH::bvh_inst> TriangleTLAS;

        struct Mesh
        {
            TriangleBVH bvh;
            ANARIGeometry handle = nullptr;
            unsigned geomID = unsigned(-1);
        };

        struct Material
        {
            ANARIMaterial handle = nullptr;
            unsigned matID = unsigned(-1);
        };

        struct Renderer
        {
            Algorithm algorithm;
            RenderTarget renderTarget;
            thin_lens_camera cam;
            vec4f backgroundColor;
            tiled_sched<ray> sched{std::thread::hardware_concurrency()};
            aligned_vector<StructuredVolume> structuredVolumes;
            std::vector<Mesh> meshes;
            std::vector<Material> materials;
            struct {
                bool hasChanged = false;
                aligned_vector<TriangleBVH::bvh_inst> bvhInsts;
                TriangleTLAS tlas;
                aligned_vector<generic_material<matte<float>>> materials;
            } surfaces;
            unsigned accumID=0;

            void renderFrame()
            {
                static unsigned frame_num = 0;
                pixel_sampler::basic_jittered_blend_type<float> blend_params;
                blend_params.spp = 1;
                float alpha = 1.f / ++accumID;
                //float alpha = 1.f / 1.f;
                blend_params.sfactor = alpha;
                blend_params.dfactor = 1.f - alpha;
                auto sparams = make_sched_params(blend_params,cam,renderTarget);

                if (algorithm==Algorithm::Pathtracing) {

                    if (structuredVolumes.size()==1) { // single volume only
                        StructuredVolume& volume = structuredVolumes[0];

                        float heightf = (float)renderTarget.height();

                        sched.frame([&](ray r, random_generator<float>& gen, int x, int y) {
                            result_record<float> result;
                            
                            henyey_greenstein<float> f;
                            f.g = 0.f; // isotropic

                            vec3f throughput(1.f);

                            auto hit_rec = intersect(r, volume.bbox);

                            unsigned bounce = 0;

                            if (hit_rec.hit)
                            {
                                r.ori += r.dir * hit_rec.tnear;
                                hit_rec.tfar -= hit_rec.tnear;

                                while (volume.sample_interaction(r, hit_rec.tfar, gen))
                                {
                                    // Is the path length exceeded?
                                    if (bounce++ >= 1024)
                                    {
                                        throughput = vec3(0.0f);
                                        break;
                                    }

                                    throughput *= volume.albedo(r.ori);

                                    // Russian roulette absorption
                                    float prob = max_element(throughput);
                                    if (prob < 0.2f)
                                    {
                                        if (gen.next() > prob)
                                        {
                                            throughput = vec3(0.0f);
                                            break;
                                        }
                                        throughput /= prob;
                                    }

                                    // Sample phase function
                                    vec3 scatter_dir;
                                    float pdf;
                                    f.sample(-r.dir, scatter_dir, pdf, gen);
                                    r.dir = scatter_dir;

                                    hit_rec = intersect(r, volume.bbox);
                                }
                            }

                            // Look up the environment
                            float t = y / heightf;
                            vec3 Ld = (1.0f - t)*vec3f(1.0f, 1.0f, 1.0f) + t * vec3f(0.5f, 0.7f, 1.0f);
                            vec3 L = throughput;

                            result.hit = hit_rec.hit;
                            result.color = bounce ? vec4f(L, 1.f) : backgroundColor;
                            return result;
                        }, sparams);
                    } else if (!surfaces.bvhInsts.empty()) {
                        auto kparams = make_kernel_params(
                            &surfaces.tlas,
                            &surfaces.tlas+1,
                            surfaces.materials.begin(),
                            10,
                            1e-5f,
                            backgroundColor,
                            vec4f{1.f,1.f,1.f,1.f}
                        );
                        pathtracing::kernel<decltype(kparams)> kernel;
                        kernel.params = kparams;
                        sched.frame(kernel,sparams);
                    }
                } else if (algorithm==Algorithm::AmbientOcclusion) {

                    if (1) { // single volume only
                        StructuredVolume& volume = structuredVolumes[0];

                        float dt = 2.f;
                        vec3f gradientDelta = 1.f/volume.bbox.size();
                        bool volumetricAO = true;

                        sched.frame([&](ray r, random_generator<float>& gen, int x, int y) {
                            result_record<float> result;

                            auto hit_rec = intersect(r, volume.bbox);
                            result.hit = hit_rec.hit;

                            if (!hit_rec.hit) {
                                result.color = backgroundColor;
                                return result;
                            }

                            result.color = vec4f(0.f);

                            float t = max(0.f,hit_rec.tnear);
                            float tmax = hit_rec.tfar;
                            vec3f pos = r.ori + r.dir * t;

                            // TODO: vertex-centric!
                            vec3f texCoord = pos/volume.bbox.size();

                            vec3f inc = r.dir*dt/volume.bbox.size();

                            while (t < tmax) {
                                float voxel = tex3D(volume.texture3f,texCoord);
                                vec4f color = tex1D(volume.textureRGBA,voxel);

                                // shading
                                vec3f grad = volume.gradient(texCoord,gradientDelta);
                                if (volumetricAO && length(grad) > .15f) {
                                    vec3f n = normalize(grad);
                                    n = faceforward(n,-r.dir,n);
                                    vec3f u, v, w=n;
                                    make_orthonormal_basis(u,v,w);

                                    float radius = 1.f;
                                    int numSamples = 2;
                                    for (int i=0; i<numSamples; ++i) {
                                        auto sp = cosine_sample_hemisphere(gen.next(),gen.next());
                                        vec3f dir = normalize(sp.x*u+sp.y*v+sp.z+w);

                                        ray aoRay;
                                        aoRay.ori = texCoord + dir * 1e-3f;
                                        aoRay.dir = dir;
                                        aoRay.tmin = 0.f;
                                        aoRay.tmax = radius;
                                        auto ao_rec = intersect(aoRay,volume.bbox);
                                        aoRay.tmax = fminf(aoRay.tmax,ao_rec.tfar);

                                        vec3f texCoordAO = aoRay.ori/volume.bbox.size();
                                        vec3f incAO = aoRay.dir*dt/volume.bbox.size();

                                        float tAO = 0.f;
                                        while (tAO < aoRay.tmax) {
                                            float voxelAO = tex3D(volume.texture3f,texCoordAO);
                                            vec4f colorAO = tex1D(volume.textureRGBA,voxelAO);

                                            color *= colorAO.w;

                                            texCoordAO += incAO;
                                            tAO += dt;
                                        }
                                    }
                                }

                                // opacity correction
                                color.w = 1.f-powf(1.f-color.w,dt);

                                // premultiplied alpha
                                color.xyz() *= color.w;

                                // compositing
                                result.color += color * (1.f-result.color.w);

                                // step on
                                texCoord += inc;
                                t += dt;
                            }

                            result.color.xyz() += (1.f-result.color.w) * backgroundColor.xyz();
                            result.color.w += (1.f-result.color.w) * backgroundColor.w;

                            return result;
                        }, sparams);
                    }
                }
            }
        };


        Renderer renderer;

        enum class ExecutionOrder {
            Object = 9999, // catch error messages first
            StructuredRegular = 6,
            TriangleGeom = 6,
            Matte = 6,
            Volume = 5,
            Surface = 5,
            World = 4,
            Camera = 3,
            PerspectiveCamera = 2,
            Renderer = 1,
            AO = 1,
            Pathtracer = 1,
            Frame = 0,
        };

        typedef std::function<void()> CommitFunc;

        struct Commit {
            CommitFunc func;
            ExecutionOrder order;
        };

        std::vector<Commit> outstandingCommits;

        static void flushCommitBuffer() {
            std::sort(outstandingCommits.begin(),outstandingCommits.end(),
                      [](const Commit& a, const Commit& b)
                      {
                          return a.order > b.order;
                      });

            for (auto commit : outstandingCommits) {
                commit.func();
            }

            outstandingCommits.clear();
        }

        void enqueueCommit(CommitFunc func, ExecutionOrder order) {
            outstandingCommits.push_back({func,order});
        }
    } // backend

    //--- API ---------------------------------------------
    namespace backend {

        void commit(Object& obj)
        {
            enqueueCommit([]() {
                LOG(logging::Level::Warning) << "Backend: no commit() implementation found";
            }, ExecutionOrder::Object);
        }

        void commit(World& world)
        {
            enqueueCommit([]() {
                if (renderer.surfaces.hasChanged) {
                    renderer.surfaces.bvhInsts.clear();
                    renderer.surfaces.materials.clear();

                    for (unsigned i=0; i<renderer.meshes.size(); ++i) {
                        renderer.surfaces.bvhInsts.push_back(renderer.meshes[i].bvh.inst({mat3x3::identity(),vec3f(0.f)}));
                    }

                    lbvh_builder builder;

                    renderer.surfaces.tlas = builder.build(TriangleTLAS{},
                                                           renderer.surfaces.bvhInsts.data(),
                                                           renderer.surfaces.bvhInsts.size());

                    for (unsigned i=0; i<renderer.materials.size(); ++i) {
                        matte<float> mat;
                        mat.ca() = from_rgb(vec3f{.8f,.8f,.8f});
                        mat.cd() = from_rgb(vec3f{.8f,.8f,.8f});
                        mat.ka() = 1.f;
                        mat.kd() = 1.f;
                        renderer.surfaces.materials.push_back(mat);
                    }

                    renderer.surfaces.hasChanged = false;
                }

                // TODO: same for volumes?
            }, ExecutionOrder::World);
        }

        void commit(Camera& cam)
        {
            enqueueCommit([&cam]() {
                vec3f eye(cam.position);
                vec3f dir(cam.direction);
                vec3f center = eye+dir;
                vec3f up(cam.up);
                if (eye!=renderer.cam.eye() || center!=renderer.cam.center() || up!=renderer.cam.up()) {
                    renderer.cam.look_at(eye,center,up);
                    renderer.cam.set_lens_radius(cam.apertureRadius);
                    renderer.cam.set_focal_distance(cam.focusDistance);
                    renderer.accumID = 0;
                }
            }, ExecutionOrder::Camera);
        }

        void commit(PerspectiveCamera& cam)
        {
            enqueueCommit([&cam]() {
                renderer.cam.perspective(cam.fovy,cam.aspect,.001f,1000.f);
            }, ExecutionOrder::PerspectiveCamera);
        }

        void commit(Frame& frame)
        {
            enqueueCommit([&frame]() {
                pixel_format color
                    = frame.color==ANARI_UFIXED8_VEC4 || ANARI_UFIXED8_RGBA_SRGB
                        ? PF_RGBA8 : PF_RGBA32F;

                pixel_format depth
                    = frame.color==ANARI_FLOAT32 ? PF_DEPTH32F : PF_UNSPECIFIED;

                bool sRGB = frame.color==ANARI_UFIXED8_RGBA_SRGB;

                renderer.renderTarget.reset(frame.size[0],frame.size[1],color,depth,sRGB);

                renderer.cam.set_viewport(0,0,frame.size[0],frame.size[1]);
            }, ExecutionOrder::Frame);
        }

        void commit(TriangleGeom& geom)
        {
            enqueueCommit([&geom]() {
                auto it = std::find_if(renderer.meshes.begin(),renderer.meshes.end(),
                                       [&geom](const Mesh& mesh) {
                                           return mesh.handle != nullptr
                                               && mesh.handle == geom.getResourceHandle();
                                       });

                unsigned geomID(-1);

                if (it == renderer.meshes.end()) {
                    renderer.meshes.emplace_back();
                    it = renderer.meshes.end()-1;
                    geomID = renderer.meshes.size()-1;
                } else {
                    geomID = std::distance(it,renderer.meshes.begin());
                }

                it->handle = (ANARIGeometry)geom.getResourceHandle();
                it->geomID = geomID;

                aligned_vector<basic_triangle<3,float>> triangles;

                if (geom.primitive_index != nullptr) {
                    Array1D* vertex = (Array1D*)GetResource(geom.vertex_position);
                    Array1D* index = (Array1D*)GetResource(geom.primitive_index);

                    vec3f* vertices = (vec3f*)vertex->data;
                    vec3ui* indices = (vec3ui*)index->data;

                    triangles.resize(index->numItems[0]);

                    for (uint32_t i=0; i<index->numItems[0]; ++i) {
                        vec3f v1 = vertices[indices[i].x];
                        vec3f v2 = vertices[indices[i].y];
                        vec3f v3 = vertices[indices[i].z];

                        triangles[i].prim_id = i;
                        triangles[i].geom_id = geomID;
                        triangles[i].v1 = v1;
                        triangles[i].e1 = v2-v1;
                        triangles[i].e2 = v3-v1;
                    }

                    binned_sah_builder builder;
                    builder.enable_spatial_splits(true);

                    it->bvh = builder.build(TriangleBVH{},triangles.data(),triangles.size());
                } else {
                    assert(0 && "not implemented yet!!");
                }

                renderer.surfaces.hasChanged = true;
            }, ExecutionOrder::TriangleGeom);
        }

        void commit(Matte& mat)
        {
            enqueueCommit([&mat]() {
                auto it = std::find_if(renderer.materials.begin(),renderer.materials.end(),
                                       [&mat](const Material& material) {
                                           return material.handle != nullptr
                                               && material.handle == mat.getResourceHandle();
                                       });

                // Problem here is that really, materials are identified by a combination
                // geomID _and_ matID. So we'd have to check on commit(Surface) if this is
                // a new _combination_, and possibly insert a new material and set geomIDs
                // accordingly..
                //if (it == renderer.materials.end()) {
                    renderer.materials.emplace_back();
                    it = renderer.materials.end()-1;
                //}

                it->handle = (ANARIMaterial)mat.getResourceHandle();
                it->matID = std::distance(it,renderer.materials.begin());

                renderer.surfaces.hasChanged = true;
            }, ExecutionOrder::Matte);
        }

        void commit(Surface& surf)
        {
            enqueueCommit([&surf]() {
                assert(surf.geometry != nullptr);

                if (surf.material == nullptr) {
                    // TODO: this might be too many materials, always adding the dflt. one
                    auto it = std::find_if(renderer.meshes.begin(),renderer.meshes.end(),
                                           [&surf](const Mesh& mesh) {
                                               return mesh.handle == surf.geometry;
                                           });
                    renderer.materials.resize(it->geomID+1);

                    // Mark as dflt. material
                    renderer.materials[it->geomID].handle = nullptr;
                    renderer.materials[it->geomID].matID = unsigned(-1);
                }

                renderer.surfaces.hasChanged = true;
            }, ExecutionOrder::Surface);
        }

        void commit(StructuredRegular& sr)
        {
        }

        void commit(Volume& vol)
        {
            enqueueCommit([&vol]() {
                auto it = std::find_if(renderer.structuredVolumes.begin(),
                                       renderer.structuredVolumes.end(),
                                       [&vol](const StructuredVolume& sv) {
                                           return sv.handle != nullptr
                                               && sv.handle == vol.getResourceHandle();
                                       });

                if (it == renderer.structuredVolumes.end()) {
                    renderer.structuredVolumes.emplace_back();
                    renderer.structuredVolumes.back().reset(vol);
                } else {
                    it->reset(vol);
                }

                renderer.accumID = 0;
            }, ExecutionOrder::Volume);
        }

        void commit(::generic::Renderer& rend)
        {
            enqueueCommit([&rend]() {
                renderer.backgroundColor = vec4f(rend.backgroundColor);
            }, ExecutionOrder::Renderer);
        }

        void commit(Pathtracer& pt)
        {
            enqueueCommit([&pt]() {
                renderer.algorithm = Algorithm::Pathtracing;
            }, ExecutionOrder::Pathtracer);
        }

        void commit(AO& ao)
        {
            enqueueCommit([&ao]() {
                renderer.algorithm = Algorithm::AmbientOcclusion;
            }, ExecutionOrder::AO);
        }

        void* map(Frame& frame)
        {
            return renderer.renderTarget.colorPtr;
        }

        void renderFrame(Frame& frame)
        {
            flushCommitBuffer();

            frame.renderFuture = std::async([]() {
                renderer.renderFrame();
            });
        }

        int wait(Frame& frame, ANARIWaitMask)
        {
            frame.renderFuture.wait();
            return 1;
        }

    } // backend
} // generic


