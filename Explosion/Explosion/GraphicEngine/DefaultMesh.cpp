/*
Junsu Jang, Seongwon Jang
CS250
Class Project
2022 Spring
*/
#include "GraphicEngine.h"
#include "DefaultMesh.h"
#include "Function.h"
#include "Mesh.h"
#include <glm/ext/matrix_transform.hpp>
#include "../GraphicEngine/Noise.h"
using GE = GraphicEngine;
namespace EP //mean Explosion
{
	namespace Mesh
	{
        const float  EPSILON = 0.00001f;
        static bool DegenerateTri(const glm::vec3& v0, const  glm::vec3& v1, const  glm::vec3& v2)
        {
            return (glm::distance(v0, v1) < EPSILON ||
                glm::distance(v1, v2) < EPSILON ||
                glm::distance(v2, v0) < EPSILON);
        }

		
		void BuildIndexBuffer(int stacks, int slices, std::vector<GLuint>& idx_vtx, const std::vector<glm::vec3>& pos)
        {
            int p0 = 0, p1 = 0, p2 = 0;
            int p3 = 0, p4 = 0, p5 = 0;

            int stride{ slices + 1 };
            for (int i = 0; i < stacks; ++i)
            {
                int curr_row = i * stride;
                for (int j = 0; j < slices; ++j)
                {
                    p0 = curr_row + j;
                    p1 = p0 + 1;
                    p2 = p1 + stride;

                    if (!DegenerateTri(pos[p0], pos[p1], pos[p2]))
                    {
                        idx_vtx.push_back(p0);
                        idx_vtx.push_back(p1);
                        idx_vtx.push_back(p2);
                    }
                    p3 = p2;
                    p4 = p0 + stride;
                    p5 = p0;
                    if (!DegenerateTri(pos[p3], pos[p4], pos[p5]))
                    {
                        idx_vtx.push_back(p3);
                        idx_vtx.push_back(p4);
                        idx_vtx.push_back(p5);
                    }
                }
            }
        }

        void binding(GLuint& vao_hdl, std::vector<glm::vec3>& pos_vtx, std::vector<GLuint>& idx_vtx)
        {
            GLuint vbo_hdl;
            glCreateBuffers(1, &vbo_hdl);
            glNamedBufferStorage(vbo_hdl,
                sizeof(glm::vec3) * pos_vtx.size(), nullptr, GL_DYNAMIC_STORAGE_BIT);
            glNamedBufferSubData(vbo_hdl, 0,
                sizeof(glm::vec3) * pos_vtx.size(), pos_vtx.data());
            glCreateVertexArrays(1, &vao_hdl);
            glEnableVertexArrayAttrib(vao_hdl, 0);
            glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(glm::vec3));
            glVertexArrayAttribFormat(vao_hdl, 0, 3, GL_FLOAT, GL_FALSE, 0);
            glVertexArrayAttribBinding(vao_hdl, 0, 0);

            GLuint ebo_hdl;
            glCreateBuffers(1, &ebo_hdl);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_hdl);
            glNamedBufferStorage(ebo_hdl, sizeof(GLuint) * idx_vtx.size(),
                reinterpret_cast<GLvoid*>(idx_vtx.data()),
                GL_DYNAMIC_STORAGE_BIT);
            glVertexArrayElementBuffer(vao_hdl, ebo_hdl);
            glBindVertexArray(0);
        }

        void binding(GLuint& vao_hdl, std::vector<glm::vec3>& pos_vtx, std::vector<glm::vec3>& nrm_vtx, std::vector<GLuint>& idx_vtx)
        {
            GLuint vbo_hdl;
            glCreateBuffers(1, &vbo_hdl);
            glNamedBufferStorage(vbo_hdl,
                sizeof(glm::vec3) * pos_vtx.size() + sizeof(glm::vec3) * nrm_vtx.size(), nullptr, GL_DYNAMIC_STORAGE_BIT);
            glNamedBufferSubData(vbo_hdl, 0,
                sizeof(glm::vec3) * pos_vtx.size(), pos_vtx.data());

            glNamedBufferSubData(vbo_hdl, sizeof(glm::vec3) * pos_vtx.size(),
                sizeof(glm::vec3) * nrm_vtx.size(), nrm_vtx.data());
        	
            glCreateVertexArrays(1, &vao_hdl);
        	
            glEnableVertexArrayAttrib(vao_hdl, 0);
            glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(glm::vec3));
            glVertexArrayAttribFormat(vao_hdl, 0, 3, GL_FLOAT, GL_FALSE, 0);
            glVertexArrayAttribBinding(vao_hdl, 0, 0);

            glEnableVertexArrayAttrib(vao_hdl, 1);
            glVertexArrayVertexBuffer(vao_hdl, 1, vbo_hdl, sizeof(glm::vec3) * pos_vtx.size(), sizeof(glm::vec3));
            glVertexArrayAttribFormat(vao_hdl, 1, 3, GL_FLOAT, GL_FALSE, 0);
            glVertexArrayAttribBinding(vao_hdl, 1, 1);
        	
            GLuint ebo_hdl;
            glCreateBuffers(1, &ebo_hdl);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_hdl);
            glNamedBufferStorage(ebo_hdl, sizeof(GLuint) * idx_vtx.size(),
                reinterpret_cast<GLvoid*>(idx_vtx.data()),
                GL_DYNAMIC_STORAGE_BIT);
            glVertexArrayElementBuffer(vao_hdl, ebo_hdl);
            glBindVertexArray(0);
        }

        void binding(GLuint& vao_hdl, std::vector<glm::vec3>& pos_vtx, std::vector<glm::vec3>& nrm_vtx, std::vector<glm::vec2>& uv_vtx, std::vector<GLuint>& idx_vtx)
        {
            GLuint vbo_hdl;
            glCreateBuffers(1, &vbo_hdl);
            glNamedBufferStorage(vbo_hdl,
                sizeof(glm::vec3) * pos_vtx.size() + sizeof(glm::vec3) * nrm_vtx.size() + sizeof(glm::vec2) * uv_vtx.size(), nullptr, GL_DYNAMIC_STORAGE_BIT);
            glNamedBufferSubData(vbo_hdl, 0,
                sizeof(glm::vec3) * pos_vtx.size(), pos_vtx.data());

            glNamedBufferSubData(vbo_hdl, sizeof(glm::vec3) * pos_vtx.size(),
                sizeof(glm::vec3) * nrm_vtx.size(), nrm_vtx.data());

            glNamedBufferSubData(vbo_hdl, sizeof(glm::vec3) * pos_vtx.size() + sizeof(glm::vec3) * nrm_vtx.size(),
                sizeof(glm::vec2) * uv_vtx.size(), uv_vtx.data());
            glCreateVertexArrays(1, &vao_hdl);

            glEnableVertexArrayAttrib(vao_hdl, 0);
            glVertexArrayVertexBuffer(vao_hdl, 0, vbo_hdl, 0, sizeof(glm::vec3));
            glVertexArrayAttribFormat(vao_hdl, 0, 3, GL_FLOAT, GL_FALSE, 0);
            glVertexArrayAttribBinding(vao_hdl, 0, 0);

            glEnableVertexArrayAttrib(vao_hdl, 1);
            glVertexArrayVertexBuffer(vao_hdl, 1, vbo_hdl, sizeof(glm::vec3) * pos_vtx.size(), sizeof(glm::vec3));
            glVertexArrayAttribFormat(vao_hdl, 1, 3, GL_FLOAT, GL_FALSE, 0);
            glVertexArrayAttribBinding(vao_hdl, 1, 1);

            glEnableVertexArrayAttrib(vao_hdl, 2);
            glVertexArrayVertexBuffer(vao_hdl, 2, vbo_hdl, sizeof(glm::vec3) * pos_vtx.size() + sizeof(glm::vec3) * nrm_vtx.size(), sizeof(glm::vec2));
            glVertexArrayAttribFormat(vao_hdl, 2, 2, GL_FLOAT, GL_FALSE, 0);
            glVertexArrayAttribBinding(vao_hdl, 2, 2);

            GLuint ebo_hdl;
            glCreateBuffers(1, &ebo_hdl);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_hdl);
            glNamedBufferStorage(ebo_hdl, sizeof(GLuint) * idx_vtx.size(),
                reinterpret_cast<GLvoid*>(idx_vtx.data()),
                GL_DYNAMIC_STORAGE_BIT);
            glVertexArrayElementBuffer(vao_hdl, ebo_hdl);
            glBindVertexArray(0);
        }
        

        void computePlane(int stacks, int slices, 
            std::vector<glm::vec3> &pos_vtx,
            std::vector<glm::vec3> &nrm_vtx,
            std::vector<glm::vec2> &uv_vtx) {
            for (int stack = 0; stack <= stacks; ++stack)
            {
                float row = (float)stack / stacks;

                for (int slice = 0; slice <= slices; ++slice)
                {
                    float col = (float)slice / slices;

                    pos_vtx.push_back(glm::vec3{ col - 0.5f, row - 0.5f , 0.0f });
                    nrm_vtx.push_back(glm::vec3{ 0.0f, 0.0f, 1.0f });
                    uv_vtx.push_back(glm::vec2(col, row));
                }
            }
        }

		
        std::string GetPlane(int stacks, int slices)
        {
            std::string meshName = "Plane ST:" + std::to_string(stacks) + "SL:" + std::to_string(slices);
            if (ExistMesh(meshName))
            {
                return meshName;
            }
            else {
                MeshHandle* m = new MeshHandle(meshName);
                std::vector<glm::vec3> clr_vtx{  };
                std::vector<glm::vec3> pos_vtx{  };
                std::vector<glm::vec3> nrm_vtx{ };
                std::vector<glm::vec2> uv_vtx{};
                std::vector<GLuint> idx_vtx{ };
                computePlane(stacks, slices,pos_vtx,nrm_vtx,uv_vtx);
                BuildIndexBuffer(stacks, slices, idx_vtx, pos_vtx);
                GLuint vao_hdl;
                binding(vao_hdl,pos_vtx, nrm_vtx, uv_vtx, idx_vtx);
                m->vaoid = vao_hdl;
                m->draw_cnt = static_cast<GLuint>(idx_vtx.size());
                m->primitive_type = GL_TRIANGLES;
                m->primitive_cnt = 0;//I don't know
                GE::Meshs.insert({ meshName, m });
                return meshName;
            }
        }
		
        std::string GetCube(int stacks, int slices) {
            std::string meshName = "Cube ST:" + std::to_string(stacks) + "SL:" + std::to_string(slices);
            if (ExistMesh(meshName))
            {
                return meshName;
            }
            else {
                MeshHandle* m = new MeshHandle(meshName);
                std::vector<glm::vec3> clr_vtx{  };
                std::vector<glm::vec3> pos_vtx{  };
                std::vector<glm::vec3> nrm_vtx{ };
                std::vector<glm::vec2> uv_vtx{};
                std::vector<GLuint> idx_vtx{ };
                
                std::vector<glm::vec3> plane_pos_vtx{  };
                std::vector<glm::vec3> plane_nrm_vtx{ };
                std::vector<glm::vec2> plane_uv_vtx{};
                std::vector<GLuint> plane_idx_vtx{ };

                computePlane(stacks, slices, plane_pos_vtx, plane_nrm_vtx, plane_uv_vtx);
                BuildIndexBuffer(stacks, slices, plane_idx_vtx, plane_pos_vtx);

                glm::vec3 const translateArray[] =
                {
                    glm::vec3(+0.0f, +0.0f, +0.5f), // Z+
                    glm::vec3(+0.0f, +0.0f, -0.5f), // Z-
                    glm::vec3(+0.5f, +0.0f, +0.0f), // X+
                    glm::vec3(-0.5f, +0.0f, +0.0f), // X-
                    glm::vec3(+0.0f, +0.5f, +0.0f), // Y+
                    glm::vec3(+0.0f, -0.5f, +0.0f), // Y-
                };

                static constexpr float HALF_PI{ GraphicEngine::PI * 0.5f };
                glm::vec2 const rotateArray[] =
                {
                    glm::vec2(+0.0f, +0.0f),             // Z+
                    glm::vec2(+0.0f, (float)+GraphicEngine::PI),        // Z-
                    glm::vec2(+0.0f, (float)+HALF_PI),   // X+       
                    glm::vec2(+0.0f, (float)-HALF_PI),   // X-
                    glm::vec2((float)-HALF_PI, +0.0f),   // Y+
                    glm::vec2((float)+HALF_PI, +0.0f)    // Y-
                };

                for (int i = 0; i < 6; ++i)
                {
                    glm::mat4 transformMat = glm::translate(glm::mat4(1.0f), translateArray[i]) *
                        glm::rotate(glm::mat4(1.0f), rotateArray[i][1], { 0,1,0 }) *
                        glm::rotate(glm::mat4(1.0f), rotateArray[i][0], { 1,0,0 });

                    for (int j = 0; j < plane_pos_vtx.size(); ++j)
                    {
                        pos_vtx.push_back(glm::vec3(transformMat * glm::vec4(plane_pos_vtx[j], 1.0)));
                        nrm_vtx.push_back(glm::vec3(transformMat * glm::vec4(plane_nrm_vtx[j], 1.0)));
                        uv_vtx.push_back(plane_uv_vtx[j]);
                    }

                    for (int j = 0; j < plane_idx_vtx.size(); ++j) {
                        idx_vtx.push_back(plane_idx_vtx[j] + (GLuint)plane_pos_vtx.size() * i);
                    }
                }
                GLuint vao_hdl;
                binding(vao_hdl, pos_vtx, nrm_vtx, idx_vtx);
 
                m->vaoid = vao_hdl;
                m->draw_cnt = static_cast<GLuint>(idx_vtx.size());
                m->primitive_type = GL_TRIANGLES;
                m->primitive_cnt = 0;//I don't know
                GE::Meshs.insert({ meshName, m });
                return meshName;
            }
        }
        std::string GetCone(int stacks, int slices) {
            std::string meshName = "Cone ST:" + std::to_string(stacks) + "SL:" + std::to_string(slices);
            if (ExistMesh(meshName))
            {
                return meshName;
            }
            else {

                MeshHandle* m = new MeshHandle(meshName);
                std::vector<glm::vec3> clr_vtx{  };
                std::vector<glm::vec3> pos_vtx{  };
                std::vector<glm::vec3> nrm_vtx{ };
                std::vector<glm::vec2> uv_vtx{};
                std::vector<GLuint> idx_vtx{ };


                std::vector<glm::vec3> plane_pos_vtx{  };
                std::vector<glm::vec3> plane_nrm_vtx{ };
                std::vector<glm::vec2> plane_uv_vtx{};
                std::vector<GLuint> plane_idx_vtx{ };

                computePlane(stacks, slices, plane_pos_vtx, plane_nrm_vtx, plane_uv_vtx);
                BuildIndexBuffer(stacks, slices, plane_idx_vtx, plane_pos_vtx);

                float radius{ 0.5f };
                float row{}, col{};
                float pi{ 3.14159f };
                float alpha{};//, delA{ pi / slices };
                float h{ -0.5f }, delH{ 1.f / stacks };

                for (int i = 0; i <= stacks;++i) {
                    row = (float)i / stacks;
                    for (int j = 0;j <= slices;++j) {
                        col = (float)j / slices;
                        alpha = col * pi * 2.0f;
                        float x = radius * (0.5f - h) * sin(alpha);
                        float y = h;
                        float z = radius * (0.5f - h) * cos(alpha);
                        pos_vtx.push_back(glm::vec3{ x ,y,z });
                        nrm_vtx.push_back(glm::vec3{ x ,y ,z }/radius);
                        uv_vtx.push_back(glm::vec2{1.f - col,1.f-row});
                    }
                    for (int j = 0; j < plane_idx_vtx.size(); ++j) {
                        idx_vtx.push_back(plane_idx_vtx[j] + (GLuint)plane_idx_vtx.size() * i);
                    }
                    h += delH;
                }

                nrm_vtx.push_back(glm::vec3{ 0.0, -1, 0.0 });
                pos_vtx.push_back(glm::vec3{ 0.0,-radius, 0.0 });
                uv_vtx.push_back(glm::vec2(1, 1));
                alpha = 0.f;
                int p0 = (GLuint)pos_vtx.size() - 1;
                for (int j = 0;j <= slices;++j) {
                    col = (float)j / slices;
                    alpha = col * pi * 2.0f;
                    pos_vtx.push_back(glm::vec3{ radius * sin(alpha) , -radius, radius * cos(alpha) });
                    nrm_vtx.push_back(glm::vec3{ 0.0, -1, 0.0 });
                    //uv_vtx.push_back(glm::vec2(1, 1));
                };
                for (int j = 0;j <= slices;++j) {
                    idx_vtx.push_back(p0);
                    idx_vtx.push_back(p0+j+1);
                    idx_vtx.push_back(p0+j);
                }
                GLuint vao_hdl;
                binding(vao_hdl, pos_vtx, nrm_vtx, idx_vtx);
                m->vaoid = vao_hdl;
                m->draw_cnt = static_cast<GLuint>(idx_vtx.size());
                m->primitive_type = GL_TRIANGLES;
                m->primitive_cnt = 0;//I don't know
                GE::Meshs.insert({ meshName, m });
                return meshName;
            }
        }

		std::string GetCylinder(int stacks, int slices)
		{
            std::string meshName = "Cylinder ST:" + std::to_string(stacks) + "SL:" + std::to_string(slices);
            if (ExistMesh(meshName))
            {
                return meshName;
            }
            else
            {
                MeshHandle* m = new MeshHandle(meshName);
            	
                //int Plane_vert_size{(stacks+1) * (slices+1)};
                std::vector<GLuint>  Plane_idx_vtx{ };
                std::vector<glm::vec3>  Plane_pos_vtx{  };
                std::vector<glm::vec3>  Plane_nrm_vtx{  };
                std::vector<glm::vec2>  Plane_uv_vtx{  };
            	
                computePlane(stacks, slices, Plane_pos_vtx, Plane_nrm_vtx, Plane_uv_vtx);
                BuildIndexBuffer(stacks, slices, Plane_idx_vtx, Plane_pos_vtx);

                std::vector<glm::vec3> clr_vtx{  };
                std::vector<glm::vec3>  pos_vtx{  };
                std::vector<glm::vec3>  nrm_vtx{ };
                std::vector<glm::vec2>  uv_vtx{};
                std::vector<GLuint>  idx_vtx{ };
                float radius{ 0.5 };
                float row{}, col{};
                float alpha{};//, delA{ GraphicEngine::PI / slices };
                for (int i = 0; i <= stacks; ++i) {
                    row = (float)i / stacks;

                    for (int j = 0; j <= slices; ++j) {
                        col = (float)j / slices;
                        alpha = col * GraphicEngine::PI * 2.0f;
                        glm::vec3 pos = { radius * sin(alpha),row - radius,radius * cos(alpha) };
                        glm::vec3 nrm = glm::vec3{pos.x,0.0f,pos.z}/radius;
                        glm::vec2 uv = { 1.f - col,1.f - row };
                        pos_vtx.push_back(pos);
                        nrm_vtx.push_back(nrm);
                        uv_vtx.push_back(uv);
                    }
                    for (int j = 0; j < Plane_idx_vtx.size(); ++j) {
                        GLuint idx = Plane_idx_vtx[j] + (GLuint)Plane_idx_vtx.size() * i;
                        idx_vtx.push_back(idx);
                    }
                }

                pos_vtx.push_back({ 0.0,radius, 0.0 });
                nrm_vtx.push_back({ 0.0, radius, 0.0 });
                uv_vtx.push_back({1, 1});

                int p0 = (int)pos_vtx.size() - 1;
                for (int j = 0; j <= slices; ++j) {
                    col = (float)j / slices;
                    alpha = col * GraphicEngine::PI * 2.0f;
                    pos_vtx.push_back({ radius * sin(alpha) , radius, radius * cos(alpha) });
                    nrm_vtx.push_back({ 0.0, radius, 0.0 });
                    uv_vtx.push_back({ 1, 1 });
                };
                for (int j = 0; j <= slices; ++j) {
                    idx_vtx.push_back(p0);
                    idx_vtx.push_back(p0 + j);
                    idx_vtx.push_back(p0 + j + 1);
                }


                pos_vtx.push_back({ 0.0,-radius, 0.0 });
                nrm_vtx.push_back({ 0.0, -radius, 0.0 });
                uv_vtx.push_back({ 1, 1 });

                p0 = (int)(pos_vtx.size()) - 1;
                for (int j = 0; j <= slices; ++j) {
                    col = (float)j / slices;
                    alpha = col * GraphicEngine::PI * 2.0f;
                    pos_vtx.push_back({ radius * sin(alpha) , -radius, radius * cos(alpha) });
                    nrm_vtx.push_back({ 0.0, -radius, 0.0 });
                    uv_vtx.push_back({ 1, 1 });
                };
                for (int j = 0; j <= slices; ++j) {
                    idx_vtx.push_back(p0);
                    idx_vtx.push_back( p0 + j + 1);
                    idx_vtx.push_back( p0 + j);
                }

                GLuint vao_hdl;
                binding(vao_hdl, pos_vtx, nrm_vtx, idx_vtx);
                m->vaoid = vao_hdl;
                m->draw_cnt = (GLuint)idx_vtx.size();
                m->primitive_type = GL_TRIANGLES;
                m->primitive_cnt = (GLuint)idx_vtx.size();
                GE::Meshs.insert({ meshName, m });
                return meshName;
            }
		}

        std::string GetSphere(int stacks, int slices)
        {
            std::string meshName = "Sphere ST:" + std::to_string(stacks) + "SL:" + std::to_string(slices);
            if (ExistMesh(meshName))
            {
                return meshName;
            }
            else
            {
                MeshHandle* m = new MeshHandle(meshName);
                float Radius = 0.5f;
                std::vector<glm::vec3> pos_vtx;
                std::vector<glm::vec3> nrm_vtx;
                std::vector<glm::vec2> uv_vtx;
                std::vector<GLuint> idx_vtx;
                for (int i = 0; i <= stacks; ++i)
                {
                    float row = static_cast<float>(i) / stacks;
                    float beta = GraphicEngine::PI * (row - 0.5f);
                    for (int j = 0; j <= slices; j++)
                    {
                        float col = static_cast<float>(j) / slices;
                        float alpha = col * GraphicEngine::PI * 2.f;
                        glm::vec3 pos = glm::vec3{
                        	Radius * sin(alpha) * cos(beta),
                        	Radius * sin(beta),
                        	Radius * cos(alpha) * cos(beta) };
                        glm::vec3 nrm = pos / Radius;
                        //glm::vec2 uv = glm::vec2{ col,row };
                        pos_vtx.push_back(pos);
                        nrm_vtx.push_back(nrm);
                       // uv_vtx.push_back(uv);
                    }
                }
                BuildIndexBuffer(stacks, slices, idx_vtx, pos_vtx);

                GLuint vao_hdl;
                binding(vao_hdl, pos_vtx, nrm_vtx, idx_vtx);
                m->vaoid = vao_hdl;
                m->draw_cnt = (GLuint)idx_vtx.size();
                m->primitive_type = GL_TRIANGLES;
                m->primitive_cnt = (GLuint)idx_vtx.size();
                GE::Meshs.insert({ meshName, m });
                return meshName;
            }
        }

        std::string GetTorus(int stacks, int slices, float startAngle, float endAngle)
        {
            std::string meshName = "Torus ST:" + std::to_string(stacks) + "SL:" + std::to_string(slices);
            if (ExistMesh(meshName))
            {
                return meshName;
            }
            MeshHandle* m = new MeshHandle(meshName);
            std::vector<glm::vec3> pos_vtx;
            std::vector<glm::vec3> nrm_vtx;
            std::vector<glm::vec2> uv_vtx;
            std::vector<GLuint> idx_vtx;
            float r = 0.15f;
            float R = 0.35f;
            float end = endAngle - startAngle;
            for (int i = 0; i <= stacks; ++i)
            {
                float row = static_cast<float>(i) / static_cast<float>(stacks);
                float alpha = startAngle + row * end;
                //alpha *= -1;
                float sinAlpha = sin(alpha);
                float cosAlpha = cos(alpha);
                for (int j = slices; j >= 0; --j)
                {
                    float col = static_cast<float>(j) / static_cast<float>(slices);

                    float beta = col * GraphicEngine::PI * 2;


                    glm::vec3 pos = glm::vec3
                    ((R + r * cos(beta)) * sinAlpha,
                        r * sin(beta)
                        , (R + r * cos(beta)) * cosAlpha);
                    glm::vec3 c = glm::vec3(R * sin(alpha), 0, R * cos(alpha));
                    glm::vec3 nrm = (pos - c) / r;
                   // glm::vec2 uv = { col,row };

                    pos_vtx.push_back(pos);
                    nrm_vtx.push_back(nrm);
                }
            }
            BuildIndexBuffer(stacks, slices,idx_vtx,pos_vtx);

            GLuint vao_hdl;
            binding(vao_hdl, pos_vtx, nrm_vtx, idx_vtx);
            m->vaoid = vao_hdl;
            m->draw_cnt = (GLuint)idx_vtx.size();
            m->primitive_type = GL_TRIANGLES;
            m->primitive_cnt = (GLuint)idx_vtx.size();
            GE::Meshs.insert({ meshName, m });
            return meshName;
        }
        std::string GetTube(int stacks, int slices,float thickness)
        {
            std::string meshName = "Tube ST:" + std::to_string(stacks) + "SL:" + std::to_string(slices);
            if (ExistMesh(meshName))
            {
                return meshName;
            }
            else
            {
                MeshHandle* m = new MeshHandle(meshName);

                //int Plane_vert_size{ (stacks + 1) * (slices + 1) };
                std::vector<GLuint>  Plane_idx_vtx{ };
                std::vector<glm::vec3>  Plane_pos_vtx{ };
                std::vector<glm::vec3>  Plane_nrm_vtx{ };
                std::vector<glm::vec2>  Plane_uv_vtx{ };

                computePlane(stacks, slices, Plane_pos_vtx, Plane_nrm_vtx, Plane_uv_vtx);
                BuildIndexBuffer(stacks, slices, Plane_idx_vtx, Plane_pos_vtx);

                std::vector<glm::vec3> clr_vtx{  };
                std::vector<glm::vec3>  pos_vtx{  };
                std::vector<glm::vec3>  nrm_vtx{ };
                std::vector<glm::vec2>  uv_vtx{};
                std::vector<GLuint>  idx_vtx{ };
                float radius{ 0.5 };
                float circleRadius{radius - thickness};
                float row{}, col{};
                float alpha{};//, delA{ GraphicEngine::PI / slices };

                
                for (int i = 0; i <= stacks; ++i) {
                    row = (float)i / stacks;
                    for (int j = 0; j <= slices; ++j) {
                        col = (float)j / slices;
                        alpha = col * GraphicEngine::PI * 2.0f;
                        glm::vec3 pos = { circleRadius * sin(alpha),row - radius,circleRadius * cos(alpha) };
                        glm::vec3 nrm = glm::vec3{ -pos.x,0.0f,-pos.z }/ circleRadius;
                        glm::vec2 uv = { 1.f - col,1.f - row };
                        pos_vtx.push_back(pos);
                        nrm_vtx.push_back(nrm);
                        uv_vtx.push_back(uv);
                    }
                    for (int j = (int)Plane_idx_vtx.size()-1; j >=0; --j) {
                        GLuint idx =  Plane_idx_vtx[j] + (GLuint)Plane_idx_vtx.size() * i;
                        idx_vtx.push_back(idx);
                    }
                }
                int idxNum{ (int)pos_vtx.size() };
                for (int i = 0; i <= stacks; ++i) {
                    row = (float)i / stacks;

                    for (int j = 0; j <= slices; ++j) {
                        col = (float)j / slices;
                        alpha = col * GraphicEngine::PI * 2.0f;
                        glm::vec3 pos = { radius * sin(alpha),row - radius,radius * cos(alpha) };
                        glm::vec3 nrm = glm::vec3{ pos.x,0.0f,pos.z }/radius;
                        glm::vec2 uv = { 1.f - col,1.f - row };
                        pos_vtx.push_back(pos);
                        nrm_vtx.push_back(nrm);
                        uv_vtx.push_back(uv);
                    }
                    for (int j = 0; j < Plane_idx_vtx.size(); ++j) {
                        GLuint idx = idxNum +Plane_idx_vtx[j] + (GLuint)Plane_idx_vtx.size() * i;
                        idx_vtx.push_back(idx);
                    }

                }
                

                Plane_idx_vtx.clear();
                BuildIndexBuffer(1, slices, Plane_idx_vtx, Plane_pos_vtx);
                
                for (int i = 0;i < 2;++i) {
                    idxNum = { (int)pos_vtx.size() };
                    for (int j = 0; j <= slices; ++j) {
                        col = (float)j / slices;
                        alpha = col * GraphicEngine::PI * 2.0f;
                        pos_vtx.push_back({ radius * sin(alpha) , i - radius, radius * cos(alpha) });
                        nrm_vtx.push_back({ 0.0, i - radius, 0.0 });
                        uv_vtx.push_back({ 1, 1 });
                    };
                    for (int j = 0; j <= slices; ++j) {
                        col = (float)j / slices;
                        alpha = col * GraphicEngine::PI * 2.0f;
                        pos_vtx.push_back({ circleRadius * sin(alpha) , i - radius, circleRadius * cos(alpha) });
                        nrm_vtx.push_back({ 0.0, i - radius, 0.0 });
                        uv_vtx.push_back({ 1, 1 });
                    };
                    for (int j = (int)Plane_idx_vtx.size()-1; j >=0; --j) {
                        GLuint idx = idxNum + Plane_idx_vtx[j];
                        idx_vtx.push_back(idx);
                    }
                    std::reverse(Plane_idx_vtx.begin(), Plane_idx_vtx.end());
                }
                GLuint vao_hdl;
                binding(vao_hdl, pos_vtx, nrm_vtx, idx_vtx);
                m->vaoid = vao_hdl;
                m->draw_cnt = (GLuint)idx_vtx.size();
                m->primitive_type = GL_TRIANGLES;
                m->primitive_cnt = (GLuint)idx_vtx.size();
                GE::Meshs.insert({ meshName, m });
                return meshName;
            }
        }
        std::string GetFrustum(float fov, float ar, float near, float far,float width)
        {
            std::string meshName = "Frustum FOV:" + std::to_string(fov) + "AR:" + std::to_string(ar) + "NEAR:" + std::to_string(near) + "FAR:" + std::to_string(far);
            if (ExistMesh(meshName))
            {
                return meshName;
            }
            else
            {
                MeshHandle* m = new MeshHandle(meshName);
                std::vector<glm::vec3> pos_vtx{};
                std::vector<GLuint> idx_vtx{};

                pos_vtx.push_back({}); // 0

                float n = near;
                float f = far;
                float a = ar;
                float W = 2 * n * tan(fov /2);
                W = width * n /f;
                float H = W / a;

                pos_vtx.push_back({ -W/2 ,H / 2,-n });// 1
                pos_vtx.push_back({ W / 2,H / 2,-n });// 2
                pos_vtx.push_back({ W / 2 ,-H / 2,-n });// 3
                pos_vtx.push_back({ -W / 2 ,-H / 2 ,-n });// 4
                W = 2 * f * tan(fov /2);
                W = width;
                H = W / a;
                pos_vtx.push_back({ -W / 2,H / 2 ,-f });// 5
                pos_vtx.push_back({ W / 2,H / 2 ,-f });// 6
                pos_vtx.push_back({ W / 2,-H / 2 ,-f });// 7
                pos_vtx.push_back({ -W / 2 ,-H / 2 ,-f });// 8
                
                idx_vtx.push_back(0);
                idx_vtx.push_back(5);
                idx_vtx.push_back(0);
                idx_vtx.push_back(6);
                idx_vtx.push_back(0);
                idx_vtx.push_back(7);
                idx_vtx.push_back(0);
                idx_vtx.push_back(8);

                idx_vtx.push_back(1);
                idx_vtx.push_back(2);
                idx_vtx.push_back(2);
                idx_vtx.push_back(3);
                idx_vtx.push_back(3);
                idx_vtx.push_back(4);
                idx_vtx.push_back(4);
                idx_vtx.push_back(1);

                idx_vtx.push_back(5);
                idx_vtx.push_back(6);
                idx_vtx.push_back(6);
                idx_vtx.push_back(7);
                idx_vtx.push_back(7);
                idx_vtx.push_back(8);
                idx_vtx.push_back(8);
                idx_vtx.push_back(5);

                GLuint vao_hdl;
                binding(vao_hdl, pos_vtx, idx_vtx);
                m->vaoid = vao_hdl;
                m->draw_cnt = (GLuint)idx_vtx.size();
                m->primitive_type = GL_LINES;
                m->primitive_cnt = (GLuint)idx_vtx.size();
                GE::Meshs.insert({ meshName, m });
                return meshName;
                
            }
        }
        std::string GetRandomMesh()
        {
            int ranNum{ static_cast<int>(GraphicEngine::GetRandomNum(0.,100.))%7 };
            switch (ranNum)
            {
            case 0:
                return GetSphere();
            case 1:
                return GetCube();
            case 2:
                return GetCone();
            case 3:
                return GetCylinder();
            case 4:
                return GetPlane();
            case 5:
                return GetTorus();
            case 6:
                return GetTube();
            case 7:
                return GetFrustum();
            default:
                break;
            }
            return  GetSphere();
        }
	}

}