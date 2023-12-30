local PipelineModule = {
    name="None",
    vertex_inputs = {
        name = "None",
        vertex_attributes = {},
        vertex_streams = {}
    },
    pipeline = {
        blendMode="Opaque", 
        shaders = {},
        depth = {write = false, test = false},
        wireframe = false,
    }
}

function PipelineModule.PushVertexAttribute(attribute)
    PipelineModule.vertex_inputs.vertex_attributes = attribute
end

function PipelineModule.PushVertexStream(stream)
    PipelineModule.vertex_inputs.vertex_streams = stream
end

function PipelineModule.PushPipeline(blendMode, shaders, depth, wireframe)
    PipelineModule.pipeline.blendMode = blendMode
    PipelineModule.pipeline.shaders = shaders
    PipelineModule.pipeline.depth = depth
    PipelineModule.pipeline.wireframe = wireframe
end

function PipelineModule.Build()
    return PipelineModule
end

return PipelineModule