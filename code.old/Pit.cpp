// BlockOut 3000
// Copyright(c) 2018 Borislav Stanimirov
//
// Distributed under the MIT Software License
// See accompanying file LICENSE.txt or copy at
// http://opensource.org/licenses/MIT
//
#include "Pit.hpp"

#include "Resources.hpp"
#include "Figure.hpp"
#include "Primitives.hpp"
#include "Preferences.hpp"
#include "CubeTemplate.hpp"
#include "Flat3DPVM.hpp"

#include <Magnum/GL/Renderer.h>

#include <cassert>

using namespace Magnum;
using namespace std;

// fill memory with zero
inline void zero_memory(void* mem, size_t size)
{
    ::memset(mem, 0, size);
}


class PitLayer
{
public:
    PitLayer(const Vector2ui& size)
        : m_size(size)
        , m_data(new bool[size.x() * size.y()])
    {
        reset();

        m_solid
            .setPrimitive(GL::MeshPrimitive::Triangles)
            .addVertexBuffer(m_solidBuffer, 0, Shaders::Flat3D::Position{});
        m_wire
            .setPrimitive(GL::MeshPrimitive::Lines)
            .addVertexBuffer(m_wireBuffer, 0, Shaders::Flat3D::Position{});
    }

    ~PitLayer()
    {
        delete[] m_data;
    }

    bool isEmpty() const
    {
        return !m_numElements;
    }

    bool isFull() const
    {
        return m_numElements == m_size.x() * m_size.y();
    }

    bool at(unsigned x, unsigned y) const
    {
        assert(x < m_size.x() && y < m_size.y());
        return m_data[y * m_size.x() + x];
    }

    void setAt(unsigned x, unsigned y)
    {
        assert(x < m_size.x() && y < m_size.y());
        assert(!at(x, y));
        m_data[y * m_size.x() + x] = true;
        ++m_numElements;
        m_isDirty = true;
    }

    void update(uint32_t dt)
    {
        if (m_isDirty)
        {
            if (!isEmpty())
            {
                // time to recreate buffers
                const auto& ct = Resources::cubeTemplate();

                vector<Triangle> solidTriangles;
                solidTriangles.reserve(m_numElements * ct.triangles().size());

                vector<LineSegment> wireSegments;
                wireSegments.reserve(m_numElements * ct.wireSegments().size());

                for (unsigned y = 0; y < m_size.y(); ++y)
                {
                    for (unsigned x = 0; x < m_size.x(); ++x)
                    {
                        if (at(x, y))
                        {
                            auto s = solidTriangles.insert(solidTriangles.end(), ct.triangles().begin(), ct.triangles().end());
                            while (s != solidTriangles.end())
                            {
                                for (auto& elem : *s)
                                {
                                    elem += Vector3(float(x), float(y), 0);
                                }
                                ++s;
                            }

                            auto w = wireSegments.insert(wireSegments.end(), ct.wideWireSegments().begin(), ct.wideWireSegments().end());
                            while (w != wireSegments.end())
                            {
                                for (auto& elem : *w)
                                {
                                    elem += Vector3(float(x), float(y), 0);
                                }
                                ++w;
                            }
                        }
                    }
                }

                m_solidBuffer.setData(solidTriangles, GL::BufferUsage::DynamicDraw);
                m_solid.setCount(Int(solidTriangles.size()) * 3);
                m_wireBuffer.setData(wireSegments, GL::BufferUsage::DynamicDraw);
                m_wire.setCount(Int(wireSegments.size()) * 2);
            }

            m_isDirty = false;
        }

        if (m_currentDropTimer)
        {
            m_currentDropTimer -= dt;
            if (m_currentDropTimer < 0)
                m_currentDropTimer = 0;

            m_posModifier = (m_currentDrop * float(m_currentDropTimer)) / DROP_TIME;
        }
    }

    void draw(const float pos, const Color4& solidColor, const Color4& wireColor) const
    {
        assert(!isEmpty());
        auto& shader = Resources::shaderFlat3D();

        shader.setModel(Matrix4::translation({ 0, 0, pos + m_posModifier }));

        shader.setColor(solidColor);
        m_solid.draw(shader);

        shader.setColor(wireColor);
        m_wire.draw(shader);
    }

    // animate drop after a layer has been erased
    void drop(float distance)
    {
        m_currentDrop = distance;
        m_posModifier = distance;
        m_currentDropTimer = DROP_TIME;
    }

    // layer has been erased, to be pushed to the end
    void reset()
    {
        zero_memory(m_data, m_size.x() * m_size.y());
        m_isDirty = false;
        m_numElements = 0;
    }

private:
    const Vector2ui m_size;
    bool* m_data;

    // number of occupied fields
    unsigned m_numElements;

    bool m_isDirty; // buffers need regen

    GL::Buffer m_wireBuffer;
    mutable GL::Mesh m_wire;
    GL::Buffer m_solidBuffer;
    mutable GL::Mesh m_solid;

    // used to animate drops
    float m_posModifier = 0; // modify the position
    float m_currentDrop = 0; // lerp value
    int m_currentDropTimer = 0; // timer

    static const int DROP_TIME = 150; // ms
};

Pit::Pit(const Vector3ui& size)
    : m_size(size)
    , m_layers(size.z())
{
    createBuffers();

    for (auto& l : m_layers)
    {
        l = new PitLayer(size.xy());
    }
}

Pit::~Pit()
{
    for (auto& l : m_layers)
    {
        delete l;
    }
}

void Pit::draw()
{
    auto& shader = Resources::shaderFlat3D();

    GL::Renderer::setDepthMask(false);

    shader.setProjView(m_projView);
    shader.setModel({});

    constexpr Color4 solidColor = { 0.066f, 0.066f, 0.15f, 1 };
    shader.setColor(solidColor);
    m_solid.draw(shader);

    Color4 wireColor = { 0.13f, 0.30f, 0.15f, 1 };
    shader.setColor(wireColor);
    m_wire.draw(shader);

    GL::Renderer::setDepthMask(true);

    for (unsigned i = 0; i < m_layers.size(); ++i)
    {
        auto l = m_layers[i];
        if (!l->isEmpty())
        {
            l->draw(float(i), Preferences::layerColor(i), Color4(0, 0, 0, 1));
        }
    }

}

#if !defined(_countof)
#    define _countof(x) (sizeof(x)/sizeof(x[0]));
#endif

#if defined(_MSC_VER)
#   pragma warning(disable: 4456)
#endif

bool Pit::canFitFigure(const vector<Vector3i>& elements) const
{
    for (const auto& e : elements)
    {
        if (e.x() < 0 || e.x() >= int(m_size.x())
            || e.y() < 0 || e.y() >= int(m_size.y())
            || e.z() < 0 || e.z() >= int(m_size.z()))
        {
            // element is outside of pit
            return false;
        }

        if (m_layers[e.z()]->at(e.x(), e.y()))
        {
            return false;
        }
    }

    return true;
}

void Pit::adoptFigure(const vector<Vector3i>& elements)
{
    for (const auto& e : elements)
    {
        assert(e.x() >= 0 && e.x() < int(m_size.x())
            && e.y() >= 0 && e.y() < int(m_size.y())
            && e.z() >= 0 && e.z() < int(m_size.z()));

        m_layers[e.z()]->setAt(e.x(), e.y());
    }
}

void Pit::update(uint32_t dt)
{
    // custom remove_if so as to drop layers efficiently
    vector<PitLayer*> fullLayers;
    fullLayers.reserve(m_layers.size());

    auto available = m_layers.begin();
    for (auto i = m_layers.begin(); i != m_layers.end(); ++i)
    {
        if ((*i)->isFull())
        {
            fullLayers.push_back(*i);
        }
        else
        {
            if (i - available)
            {
                // at least one layer dropped before this one
                (*i)->drop(float(i - available));
            }
            *available++ = *i;
        }
    }

    assert(m_layers.end() - available == ptrdiff_t(fullLayers.size()));
    for (auto l : fullLayers)
    {
        *available++ = l;
        l->reset();
    }

    m_topNonEmptyLayer = 0;
    for (auto l : m_layers)
    {
        l->update(dt);
        if (!l->isEmpty())
        {
            ++m_topNonEmptyLayer;
        }
    }
}
