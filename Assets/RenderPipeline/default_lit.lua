local pipeline = require "RenderPipeline.pipeline"

local vertexAttribute = {
    {
        attribute_location=0,
        attribute_binding=0,
        attribute_offset=0,
        attribute_format='Float3',
    }
}

local vertexStream = {
    stream_binding=0,
    stream_stride=12,
    stream_rate="Vertex" 
}


local blendMode="Opaque"
local shaders={
    {
        stage='vertex',
        shader='Triangle.vert',
    },
    {
        stage='fragment',
        shader='Triangle.frag'
    },
}
local depth = {write=true, test=true}

pipeline.PushVertexAttribute(vertexAttribute)
pipeline.PushVertexStream(vertexStream)
pipeline.PushPipeline("default_lit",blendMode, shaders, depth)
pipeline.Build()

return pipeline