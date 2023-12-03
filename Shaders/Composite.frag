#version 450 core

const float pureWhite = 1.0;

layout(location = 0) in vec2 uv;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform sampler2D sceneColor;

vec3 ACESToneMapping(vec3 color, float luminance) {
    const float A = 2.51f;
	const float B = 0.03f;
	const float C = 2.43f;
	const float D = 0.59f;
	const float E = 0.14f;

    color *= luminance;

    return (color * (A * color + B)) / (color * (C * color + D) + E);
}

void main() {
    vec3 color = texture(sceneColor, uv).xyz;
    float luminance = dot(color, vec3(0.2126, 0.7152, 0.0722));
    float mappedLuminance = (luminance * (1.0 + luminance / (pureWhite * pureWhite))) / (1.0 + luminance); // adjust lumi
    vec3 mappedColor = ACESToneMapping(color, mappedLuminance);

    outColor = vec4(mappedColor, 1.0);
}