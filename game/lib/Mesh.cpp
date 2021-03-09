#include "Mesh.hpp"

Mesh::~Mesh()
{
    if (vb.id != SG_INVALID_ID) sg_destroy_buffer(vb);
}
